//#define __3D_IMAGE_OBJECT__					// by sonee

//----------------------------------------------------------------------
// MTopView.cpp
//----------------------------------------------------------------------
// [ Effect Layer ]  - 출력 순서에 따라서..
// 
//		-- Tile 출력
//	* Tile에 바로 붙은 Effect
//
// 
//	* Sector의 Effect			
//		-- 각각의 Character 출력
//	* Character에 붙은 AttachEffect
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

extern int			g_nZoneLarge;
extern int			g_nZoneSmall;
extern bool			g_bZonePlayerInLarge;
//extern HANDLE		g_hFileThread;

extern CMessageArray*	g_pSystemMessage;
extern CMessageArray*	g_pGameMessage;
extern CMessageArray*	g_pNoticeMessage;

extern MScreenEffectManager*	g_pInventoryEffectManager;

extern bool FileOpenBinary(const char* filename, ifstream& file);

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MTopView*	g_pTopView = NULL;
extern bool g_bFrameChanged;		

bool g_bMouseInPortal  = false;

// 임시 땜빵.. 우헤헤
POINT g_MouseSector = { 0, 0 };		// mouse가 가리키는 sector좌표

extern int	g_x;
extern int	g_y;

extern int	g_UpdateDelay;

extern MWorkThread*	g_pLoadingThread;

int g_ShowImageObjectID = 0;

// 넷마블용
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
// 한 filter좌표의 화면비율 결정..
//----------------------------------------------------------------------
float MTopView::s_LightWidth	= (float)CLIPSURFACE_WIDTH / SCREENLIGHT_WIDTH;
float MTopView::s_LightHeight	= (float)CLIPSURFACE_HEIGHT / SCREENLIGHT_HEIGHT;

//----------------------------------------------------------------------
//
// defines
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 전체 Creature의 그림 종류수
//----------------------------------------------------------------------
#define	MAX_CREATURE_BODY		3
#define	MAX_CREATURE_ADDON		4
#define	MAX_ANIMATION			2
#define	MAX_ALPHAEFFECT			10
#define	MAX_NORMALEFFECT		1

//----------------------------------------------------------------------
// Light 3D의 설정 값
//-----------------------------------------------------------------------
#define	MAX_LIGHT_SETSIZE		12

//-----------------------------------------------------------------------
// 스나이핑에서 어둡게 출력할때의 shift값
//-----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// FONT 관련
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
// 시간이 오래된걸 선택해야 한다.		
// left < right 에 대한 return값
//
// 현 순서를 유지할려면 false를
// 순서를 바꿀려면 true를 return해야 한다.
//
// 앞에 것이 먼저 출력될 것이므로.. 시간이 빠른 것(적은 것)이 앞에 있어야 한다.
// 
// PQ는 작은걸 뒤로 보낸다..
// 그러므로... 시간이 느린 것(큰것)이 뒤로 가야한다.
bool 
TextComparison::operator () (DRAWTEXT_NODE * left, DRAWTEXT_NODE * right) const
{ 
	// 먼저 설정된 글자인 경우.. OK
	// 시간이 같으면 일단 뒤에 들어간다. 
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

	// 선택된 것들
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
	
	// 시야
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
//	// 2001.7.14 주석처리
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
	// AlphBox를 g_pTopView->Init()전에 사용하기 위해서.
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

	// 넷마블용
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
// View에 필요한 각 부분들은 초기화 시킨다.
//----------------------------------------------------------------------
bool
MTopView::Init()
{
	//------------------------------------------
	// 이미 init됐다면 init하지 않는다.
	//------------------------------------------
	if (m_bInit)
	{
		return false;
	}

	//------------------------------------------
	// 적절한 Texture Memory 계산
	//------------------------------------------
	DDSCAPS2 ddsCaps2;
	DWORD dwTotal;
	DWORD dwFree;
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
	ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
	HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

	DEBUG_ADD_FORMAT("[TextureMemory] Before Init View = %d/%d", dwFree, dwTotal);
	
	//------------------------------------------
	// 적절한 effect texture개수
	//------------------------------------------
	// 1500000정도는 다른데 사용된다고 본다.
	// 평균적인 한 Texture의 memory size = 65536 - -;;
	//------------------------------------------
	int freeMemory = dwFree-1500000;

	if (freeMemory < 0) freeMemory = 0;

	int num = freeMemory / 20000 / 12;   	
	
	num = max(num, 30);	// 30보다는 커야한다.
	num = min(num, 60);	// 60이 최고다.
	
	//num = 20;

	// 재설정.. - -;
	//g_pClientConfig->MAX_TEXTUREPART_EFFECT				= num*3/2;
	//g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW		= num*3;
	//g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= num;
	//g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW			= 240 + num*7;

	//------------------------------------------------------------------------
	// 캐릭터 그림자를 무조건 검게 출력한다고 했을 때의 계산
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
		// 적절한 Texture Memory 계산
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
// 2D <--> 3D 바뀔때 사용..
//----------------------------------------------------------------------
bool
MTopView::InitChanges()
{
	//------------------------------------------
	// 아직 init되지 않았다면 return
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
		// 캐릭터 loading이 하나도 안됐다고 체크해둔다.
		// 그림자 때문인데.. 냠냠.. - -; 2001.10.6
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
	// UI의 Font등록
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
	// File 닫기
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
	// Tile Surface제거
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
//	// 2001.7.14 주석처리
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
	// list , map 제거
	//----------------------------------------------------------------------
	ClearItemNameList();
	ClearOutputCreature();	
	ClearTextList();

	DEBUG_ADD("MTV-Rel-Loaded");

	m_listLoadedCreatureSprite.Release();
	m_listLoadedMonsterSprite.Release();
	m_mapImageObject.clear();

	//----------------------------------------------------------------------
	// FPK 지우기
	//----------------------------------------------------------------------
	DEBUG_ADD("MTV-Rel-FPK");

	m_CreatureFPK.Release();			// Creature frames
	m_AddonFPK.Release();				// Player addon frames
	m_OustersFPK.Release();				// Player addon frames
	m_ItemTileFPK.Release();			// Tile에 있는 Item에 대한..
	m_ItemDropFPK.Release();			// 바닥으로 떨어지는 Item에 대한..
	m_ImageObjectFPK.Release();		// ImageObject에 대한 frames
	m_ImageObjectShadowFPK.Release();

	// 승진 관련 fpk 지우기
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
//	m_InteractionObjectFPK.Release();		// ImageObject에 대한 frames

	DEBUG_ADD("MTV-Rel-EffectFPK");

	m_EffectAlphaFPK.Release();		// Effect에 대한 frames
	m_EffectScreenFPK.Release();
	m_EffectShadowFPK.Release();		// Effect에 대한 frames
	m_EffectNormalFPK.Release();		// Effect에 대한 frames
		
	DEBUG_ADD("MTV-Rel-CFPK");

	m_CreatureShadowFPK.Release();			// Creature frames
	m_AddonShadowFPK.Release();			// Player addon frames	
	m_OustersShadowFPK.Release();			// Player addon frames	
		
	//------------------------------------------------------
	// SpritePack 제거
	//------------------------------------------------------		
	DEBUG_ADD("MTV-Rel-SPK");

	m_TileSPK.Release();				// Tile
	m_ImageObjectSPK.Release();		// ImageObject Sprite들
//	m_InteractionObjectSPK.Release();		// ImageObject Sprite들
	m_CreatureSPK.Release();			// Creature Sprite들
	m_AddonSPK.Release();		// Sprite들		
	m_OustersSPK.Release();
	
	DEBUG_ADD("MTV-Rel-ItemSPK");

	m_ItemTileISPK.Release();			// Sprite들		
	m_ItemDropISPK.Release();			//
	m_ItemBrokenSPK.Release();			//
	m_ItemRealSPK.Release();			// Sprite들				

	DEBUG_ADD("MTV-Rel-EffectSPk");
	m_EffectAlphaSPK.Release();		// AlphaSprite들
	m_EffectScreenSPK.Release();		// AlphaSprite들
	m_EffectShadowSPK.Release();		// ShadowSprite들
	m_EffectNormalSPK.Release();		// NormalSprite들
	m_EffectScreenPPK.Release();
	m_EffectAlphaPPK.Release();

	DEBUG_ADD("MTV-Rel-Other");

	m_WeatherSPK.Release();			// WeatherSprite들
	m_GuildSPK.Release();

	m_EtcSPK.Release();				// 기타...
	m_OustersFinSPK.Release();
	m_AdvacementQuestEnding.Release();

	// 승직 관련 SPK 제거
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

	m_AddonSSPK.Release();			// Sprite들		
	m_OustersSSPK.Release();			// Sprite들		
	m_ImageObjectSSPK.Release();
//	m_InteractionObjectSSPK.Release();
	m_CreatureSSPK.Release();

	//----------------------------------------------------------------------
	// File Index Table 지우기
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
	// SFP Array제거
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
// 출력 대상이 되는 Surface를 결정한다.
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
//		// 검은색
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
//		// HP색깔
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
	
	// 모두 다시 그려준다.
	//m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	//------------------------------------------------------------
	// 가속을 사용하는 경우에는 
	// Texture가 VideoMemory에 올라가 있으므로 Restore해야 한다.
	// 다시 Load.. - -;;
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
		// minimap texture초기화
		//------------------------------------------------------------
		// 2001.7.14 주석처리
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
		ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);
		m_EffectAlphaSPK.LoadFromFile(effectFile2);
		effectFile2.close();	

		//------------------------------------------------------------
		//
		// Init Effect TextureSurface
		//
		//------------------------------------------------------------		
		//--------------------------------------------
		// AlphaSPK을 이용해서 TexturePack을 생성한다.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite를 메모리에서 삭제한다.
		// --> TextureSurface를 사용할 것이므로.. 필요가 없다.
		//------------------------------------------------------------		
		m_EffectAlphaSPK.Release();	
	}
	*/
}


//----------------------------------------------------------------------
// 사용할 Surface들을 초기화 시킨다.
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
	// 3D가속이 되면 VideoMemory에.. (가능하면. - -;)
	//
	// 안되면.. system memory에... 
	// (왜? 음.. 어딘가에 관련된 코드가 있지 싶은데 찾기 귀찮다. 음냐)
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
		// 일단 video memory에 생성이 되도록 해보고..
		// 안되면 system memory에 생성한다.
//		if (!m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, TILESURFACE_HEIGHT))
		{
			DEBUG_ADD("[ InitGame ]  MTopView::InitSurface() - TileSurface vidmem failed");
			
			m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, 
											TILESURFACE_HEIGHT, 
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
		m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, 
									TILESURFACE_HEIGHT,
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
// 색깔들을 초기화 한다.
//----------------------------------------------------------------------
bool
MTopView::InitColors()
{
	//---------------------------------------------------
	// 늘 같은 색
	//---------------------------------------------------
	// item
	m_ColorNameItem					= g_pClientConfig->COLOR_NAME_ITEM;
	m_ColorNameItemOption			= g_pClientConfig->COLOR_NAME_ITEM_OPTION;

	// 종족별
	m_ColorNameVampire				= g_pClientConfig->COLOR_NAME_VAMPIRE;
	m_ColorNameSlayer				= g_pClientConfig->COLOR_NAME_SLAYER;
	m_ColorNameNPC					= g_pClientConfig->COLOR_NAME_NPC;
	
	m_ColorNameAlignment[0]			= g_pClientConfig->COLOR_NAME_EVIL_MORE;
	m_ColorNameAlignment[1]			= g_pClientConfig->COLOR_NAME_EVIL;
	m_ColorNameAlignment[2]			= g_pClientConfig->COLOR_NAME_NEUTRAL;
	m_ColorNameAlignment[3]			= g_pClientConfig->COLOR_NAME_GOOD;
	m_ColorNameAlignment[4]			= g_pClientConfig->COLOR_NAME_GOOD_MORE;	


	//---------------------------------------------------
	// 5:6:5인 경우는 바로 값을 읽으면 된다.
	//---------------------------------------------------
	if (CDirectDraw::Is565())
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT;

		m_ColorOutlineItem				= g_pClientConfig->COLOR_OUTLINE_ITEM;

		// 공격 가능?
		m_ColorOutlineNPC				= g_pClientConfig->COLOR_OUTLINE_NPC;
		m_ColorOutlineAttackPossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE;
		m_ColorOutlineAttackImpossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE;	
	}
	//---------------------------------------------------
	// 5:5:5인 경우는 값을 변환
	//---------------------------------------------------
	else
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT);

		// item
		m_ColorOutlineItem				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ITEM);

		// 공격 가능?
		m_ColorOutlineNPC				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_NPC);
		m_ColorOutlineAttackPossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE);
		m_ColorOutlineAttackImpossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE);
	}


	//---------------------------------------------------
	// 3D 가속과 관련이 있는 색깔...
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
// SpritePack을 읽어서  memory에 Load한다.
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
	ofstream	ImageObjectSFile(FILE_SSPRITE_IMAGEOBJECT, std::ios::binary);	
	ofstream	ImageObjectSIndexFile(FILE_SSPRITEINDEX_IMAGEOBJECT, std::ios::binary);	

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
	// 3d가속이 되면 m_pImageObjectShadowManager를 사용하고
	// 아니면, m_ImageObjectSSPK를 사용한다.
	/*
	ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, std::ios::binary);
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
// 		ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, std::ios::binary);
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
	// 일단 전체 크기만큼 memory는 잡아둔다.
	//------------------------------------------------------------
	//  Init Creature SpriteSet
	//------------------------------------------------------------	
	if (m_CreatureSPK.GetSize()==0)
	{
		/*
		WORD size;
		ifstream CreaturePackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, std::ios::binary);
		if (!FileOpenBinary(FILE_ISPRITEINDEX_CREATURE, CreaturePackIndexFile))
			return false;
		CreaturePackIndexFile.read((char*)&size, 2);	// Sprite의 개수
		CreaturePackIndexFile.close();	

		m_CreatureSPK.Init( size, CDirectDraw::Is565() );
		*/
//		if (m_CreatureSPKFile.is_open())
//		{
//			m_CreatureSPKFile.close();
//		}
//		
//		// 화일만 열어둔다.
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
	ofstream	creatureShadowFile(FILE_SSPRITE_CREATURE, std::ios::binary);	
	ofstream	creatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, std::ios::binary);
	m_CreatureSSPK.SaveToFile(creatureShadowFile, creatureShadowIndexFile);
	creatureShadowFile.close();
	creatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// 테스트 : 일반SpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	// 남
	/*
	CSpritePack CreatureShadowSPK;
	ifstream	CreatureShadowFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
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
	// 남 - Save  ShadowSpritePack	
	//------------------------------------------------------------
	ofstream	CreatureShadowFile(FILE_SSPRITE_CREATURE, std::ios::binary);	
	ofstream	CreatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, std::ios::binary);	

	m_CreatureSSPK.SaveToFile(CreatureShadowFile, CreatureShadowIndexFile);

	CreatureShadowFile.close();
	CreatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Creature Shadow SpritePack
	//------------------------------------------------------------
	// 3d가속이 되면 m_pCreatureShadowManager를 사용하고
	// 아니면, m_CreatureSSPK를 사용한다.
	//------------------------------------------------------------
	// 3d가속이 되면 m_pAlphaEffectTextureManager를 사용하고
	// 아니면, m_EffectAlphaSPK를 사용한다.
	
	// 전체 개수만 잡아둔다.
	/*
	ifstream CreatureShadowPackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, std::ios::binary);
	if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, CreatureShadowPackIndexFile))
		return false;
	CreatureShadowPackIndexFile.read((char*)&size, 2);	// Sprite의 개수
	CreatureShadowPackIndexFile.close();	

	m_CreatureSSPK.Init( size );
	*/
	/*
	// 임시로 Load
	// 전체 loading해두는 부분
	ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, std::ios::binary);
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
		ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, std::ios::binary);
		if (!FileOpenBinary(FILE_SSPRITE_CREATURE, CreatureShadowFile2))
			return false;
		m_CreatureSSPK.LoadFromFile(CreatureShadowFile2);
		CreatureShadowFile2.close();
		*/
		/*
		ifstream indexFile;//(indexFilename, std::ios::binary);
		if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, indexFile))
			return false;

		int packSize;
		indexFile.read((char*)&packSize, 2);
		indexFile.close();

		m_CreatureSSPK.Init( packSize );
		*/
		//------------------------------------------------------------
		// 그림자 load
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
	ofstream	clothesFile("Clothes.spk", std::ios::binary);	
	ofstream	clothesIndexFile("Clothes.spki", std::ios::binary);	

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
		ifstream	AddonFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
		if (!FileOpenBinary(FILE_ISPRITE_ADDON, AddonFile2))
			return false;
		
		// 2001.8.20 주석처리..
		//m_AddonSPK.LoadFromFile(AddonFile2);
		//m_AddonSPK.Init( 20000, CDirectDraw::Is565() );
		
		// 개수만 잡아둔다.
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
//	// 화일만 열어둔다.
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
		// 그림자 load
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
	// 그림자 - Load  Clothes SpritePack	
	//------------------------------------------------------------
	// 남
	/*
	ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_ADDON_MALE, AddonMaleShadowFile2))
		return false;
	m_AddonMaleShadowSPK.LoadFromFile(AddonMaleShadowFile2);
	AddonMaleShadowFile2.close();
	*/

	//------------------------------------------------------------
	// 테스트 : 일반SpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	/*
	// 남
	CSpritePack AddonMaleShadowSPK;
	ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
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
	// 남 - Save  ShadowSpritePack	
	//------------------------------------------------------------
	ofstream	AddonMaleShadowFile(FILE_SSPRITE_ADDON_MALE, std::ios::binary);	
	ofstream	AddonMaleShadowIndexFile(FILE_SSPRITEINDEX_ADDON_MALE, std::ios::binary);	

	m_AddonMaleSSPK.SaveToFile(AddonMaleShadowFile, AddonMaleShadowIndexFile);

	AddonMaleShadowFile.close();
	AddonMaleShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Addon Shadow SpritePack
	//------------------------------------------------------------
	// Male
	/*
	ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
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
		ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, std::ios::binary);
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
	ofstream	itemTileFile(FILE_SPRITE_ITEMTILE, std::ios::binary);	
	ofstream	itemTileIndexFile(FILE_SPRITEINDEX_ITEMTILE, std::ios::binary);	

	m_ItemTileSPK.SaveToFile(itemTileFile, itemTileIndexFile);

	itemTileFile.close();
	itemTileIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  ItemTile SpritePack	
	//------------------------------------------------------------
	if (m_ItemTileISPK.GetSize()==0)
	{
//		ifstream	itemTileFile2;//(FILE_SPRITE_ITEMTILE, std::ios::binary);
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
//		ifstream	itemDropFile2;//(FILE_SPRITE_itemDrop, std::ios::binary);
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
//		ifstream	itemBrokenFile2;//(FILE_SPRITE_itemBroken, std::ios::binary);
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
	ofstream	tileFile(FILE_SPRITE_TILE, std::ios::binary);	
	ofstream	tileIndexFile(FILE_SPRITEINDEX_TILE, std::ios::binary);	

	m_TileSPK.SaveToFile(tileFile, tileIndexFile);

	tileFile.close();
	tileIndexFile.close();	

	//------------------------------------------------------------	
	// Load  Tile SpritePack	
	//------------------------------------------------------------	
	ifstream	tileFile2(FILE_SPRITE_TILE, std::ios::binary);
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

	// TileSPK Index를 Load한다.
	CFileIndexTable		TileIndex;

	ifstream TilePackIndexFile(FILE_SPRITEINDEX_TILE, std::ios::binary);
	TileIndex.LoadFromFile( TilePackIndexFile );
	TilePackIndexFile.close();	

	// TileSPK에서 특정한 SpriteID의 Tile들만으로 Set을 생성한다.
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
//		ifstream TilePackIndexFile;//(FILE_SPRITEINDEX_TILE, std::ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITEINDEX_TILE").c_str(), TilePackIndexFile))
//			return false;
//		
//		//-----------------------------------------------------------
//		// Index를 Load한다.
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
//		//m_TileSPKFile.open(FILE_SPRITE_TILE, std::ios::binary);
////		if (!FileOpenBinary(FILE_SPRITE_TILE, m_TileSPKFile))
////			return false;
//	}

//	UI_DrawProgress(9);
//	DrawTitleLoading();

	//*/
	
	/* TILE INDEX 만들기
	CSpritePack spk;
	ifstream TilePackIndexFile(FILE_SPRITE_TILE, std::ios::binary);
	spk.LoadFromFile(TilePackIndexFile);	
	TilePackIndexFile.close();	

	ofstream TilePackIndexFile2(FILE_SPRITE_TILE, std::ios::binary);
	ofstream TilePackIndexFile3(FILE_SPRITEINDEX_TILE, std::ios::binary);
	spk.SaveToFile(TilePackIndexFile2, TilePackIndexFile3);
	TilePackIndexFile2.close();
	TilePackIndexFile3.close();

	m_TileSPK.Init( spk.GetSize(), CDirectDraw::Is565() );
	*/


	/*
	ifstream	TilePackFile(FILE_SPRITE_TILE, std::ios::binary);		
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
	// Index없는 SPK에 Index생성하기
	/*
	CSpritePack tempSPK;
	ifstream	ioFile2(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);
	tempSPK.LoadFromFile(ioFile2);
	ioFile2.close();

	ofstream	ioFile(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);	
	ofstream	ioIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, std::ios::binary);	

	tempSPK.SaveToFile(ioFile, ioIndexFile);

	ioFile.close();
	ioIndexFile.close();	
	*/

	/*
	CSpriteFilePositionArray	ImageObjectSFPArray;
	ImageObjectSFPArray.Init( 21 );

	// ImageObjectSPK Index를 Load한다.
	CFileIndexTable		ImageObjectIndex;

	ifstream ImageObjectPackIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, std::ios::binary);
	ImageObjectIndex.LoadFromFile( ImageObjectPackIndexFile );
	ImageObjectPackIndexFile.close();	

	// ImageObjectSPK에서 특정한 SpriteID의 ImageObject들만으로 Set을 생성한다.
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
		
//		ifstream ImageObjectPackIndexFile;//(FILE_SPRITEINDEX_IMAGEOBJECT, std::ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_IMAGEOBJECT, ImageObjectPackIndexFile))
//			return false;
//
//		//-----------------------------------------------------------
//		// Index를 Load한다.
//		//-----------------------------------------------------------
//		m_ImageObjectSPKI.LoadFromFile( ImageObjectPackIndexFile );
//		ImageObjectPackIndexFile.close();	
//
//		m_ImageObjectSPK.Init( m_ImageObjectSPKI.GetSize(), CDirectDraw::Is565() );
//
//		m_ImageObjectSPKFile.open(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);
		m_ImageObjectSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_IMAGEOBJECT").c_str() );
		// 컥.. 이거 왜 다 로딩하지. - -;;
		// 2001.8.20 주석처리
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
	ifstream	ImageObjectPackFile(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);		
	m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, ImageObjectSFPArray);
	ImageObjectPackFile.close();	
	*/

	//------------------------------------------------------------
	//
	//
	//                   EffectSPK 초기화
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
	ofstream	effectFile(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);	
	ofstream	effectIndexFile(FILE_ASPRITEINDEX_ALPHAEFFECT, std::ios::binary);	

	m_EffectAlphaSPK.SaveToFile(effectFile, effectIndexFile);

	effectFile.close();
	effectIndexFile.close();
	*/

	//------------------------------------------------------------
	//
	// Load  EffectPack
	//
	//------------------------------------------------------------
	// 3d가속이 되면 m_pAlphaEffectTextureManager를 사용하고
	// 아니면, m_EffectAlphaSPK를 사용한다.
/*
	ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);
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
		//ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, std::ios::binary);
//		if (!FileOpenBinary(FILE_ASPRITEINDEX_ALPHAEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File만 loading하고 
//		m_EffectAlphaSPKI.LoadFromFile( effectFileIndex );
//
//		// 개수만큼 초기화만 해둔다.
//		m_EffectAlphaSPK.Init( m_EffectAlphaSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loading을 위해서 file을 열어둔다.
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
		//ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, std::ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_SCREENEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File만 loading하고 
//		m_EffectScreenSPKI.LoadFromFile( effectFileIndex );
//
//		// 개수만큼 초기화만 해둔다.
//		m_EffectScreenSPK.Init( m_EffectScreenSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loading을 위해서 file을 열어둔다.
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
//		ifstream	effectFile2;
//		if (!FileOpenBinary(FILE_SSPRITE_SHADOWEFFECT, effectFile2))
//			return false;
//		m_EffectShadowSPK.LoadFromFile(effectFile2);
//		effectFile2.close();			
		m_EffectShadowSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_SHADOWEFFECT").c_str());
	}

//	UI_DrawProgress(12);
//	DrawTitleLoading();

	//ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, std::ios::binary);
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
		// AlphaSPK을 이용해서 TexturePack을 생성한다.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return false;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite를 메모리에서 삭제한다.
		// --> TextureSurface를 사용할 것이므로.. 필요가 없다.
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
	ofstream	EtcFile(FILE_SPRITE_ETC, std::ios::binary);	
	ofstream	EtcIndexFile(FILE_SPRITEINDEX_ETC, std::ios::binary);	

	m_EtcSPK.SaveToFile(EtcFile, EtcIndexFile);

	EtcFile.close();
	EtcIndexFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Etc SpritePack	
	//------------------------------------------------------------	
	if (m_EtcSPK.GetSize()==0)
	{
		ifstream	EtcFile2;//(FILE_SPRITE_ETC, std::ios::binary);
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
//		ifstream	ioFile2;//(FILE_SPRITE_ETC, std::ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_INTERACTIONOBJECT").c_str(), ioFile2))
//			return false;
//		m_InteractionObjectSPK.LoadFromFile(ioFile2);
//		ioFile2.close();
//	}

//	if (m_InteractionObjectSSPK.GetSize()==0)
//	{
//		/*
//		ifstream	ioFile3;//(FILE_SPRITE_ETC, std::ios::binary);
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
	
	ofstream	NormalEffectFile(FILE_SPRITE_NORMALEFFECT, std::ios::binary);	
	ofstream	NormalEffectIndexFile(FILE_SPRITEINDEX_NORMALEFFECT, std::ios::binary);	

	m_EffectNormalSPK.SaveToFile(NormalEffectFile, NormalEffectIndexFile);

	NormalEffectFile.close();
	NormalEffectIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_EffectNormalSPK.GetSize()==0)
	{
//		ifstream	NormalEffectFile2;//(FILE_SPRITE_NORMALEFFECT, std::ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str(), NormalEffectFile2))
//			return false;
//		m_EffectNormalSPK.LoadFromFile(NormalEffectFile2);
//		NormalEffectFile2.close();	
		m_EffectNormalSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str());
	}


	//------------------------------------------------------------
	//
	//
	//                   Weather SpritePack 초기화
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_WeatherSPK.GetSize()==0)
	{
		ifstream	WeatherFile2;//(FILE_SPRITE_WEATHER, std::ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_WEATHER").c_str(), WeatherFile2))
			return false;
		m_WeatherSPK.LoadFromFile(WeatherFile2);
		WeatherFile2.close();
	}

	//------------------------------------------------------------
	//
	//
	//                   Shadow Test 초기화
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
	// AlphaSPK을 이용해서 TexturePack을 생성한다.
	//--------------------------------------------
	if (!m_ShadowTPK.Init( ShadowASPK ))
	{
		MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
		return false;
	}

	//------------------------------------------------------------	
	// EffectAlphaSprite를 메모리에서 삭제한다.
	// --> TextureSurface를 사용할 것이므로.. 필요가 없다.
	//------------------------------------------------------------		
	ShadowASPK.Release();
	*/
	
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	 // 2001.7.14 주석처리
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
	ofstream	MapTest1("Data\\Image\\map_a.spr", std::ios::binary);		
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
	ofstream	MapTest2("Data\\Image\\map_h.spr", std::ios::binary);		
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
	ofstream	MapTest3("Data\\Image\\map_e.spr", std::ios::binary);		
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
	ofstream	MapTest4("Data\\Image\\map_c.spr", std::ios::binary);		
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
	ofstream	MapTest5("Data\\Image\\map_d.spr", std::ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest5 );
	MapTest5.close();

	*/

	//------------------------------------------------------------
	//
	//
	//                   Guild SpritePack 초기화
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Guild SpritePack	
	//------------------------------------------------------------
	/*
	if (m_GuildSPK.GetSize()==0)
	{
		ifstream	guildFile2;//(FILE_SPRITE_WEATHER, std::ios::binary);
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
// Filter 초기화
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
	ofstream	FilterLight2DFile(FILE_FILTER_LIGHT2D, std::ios::binary);		
	m_Filter.SaveToFile(FilterLight2DFile);
	FilterLight2DFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Light2D FilterPack
	//------------------------------------------------------------	
	//ifstream	FilterLight2DFile2(FILE_FILTER_LIGHT2D, std::ios::binary);
	//m_Filter.LoadFromFile(FilterLight2DFile2);
	//FilterLight2DFile2.close();


	//------------------------------------------------------------	
	//
	//  3D 시야처리를 위한 Light Filter
	//
	//------------------------------------------------------------	
	//-----------------------------------------------
	// LightBuffer Texture 초기화
	//-----------------------------------------------	
	if (m_pLightBufferTexture!=NULL)
	{
		//m_pLightBufferTexture->Restore();
		delete m_pLightBufferTexture;
		m_pLightBufferTexture = NULL;
	}
	//------------------------------------------------------------	
	// 2D light에서 한 점이 차지하는 화면의 pixel크기
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
		// Light FilterPack 초기화
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter 생성
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126};
		int l,i,j;

		// MAX_LIGHT_SETSIZE만큼 밝기의 크기 종류가 있다.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k값이 0~24까지..

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
				
					if (k <= maxCenter) k = 0;			// maxCenter까지는 0
					else if (k >= maxK) k = maxLight;	// 넘으면 maxLight
					else k = k - maxCenter;				// 나머지는 - maxCenter

					// 밝기의 SET단계를 만든다.
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
		ofstream	LightFilter3DFile(FILE_FILTER_LIGHT3D, std::ios::binary);
		m_LightFTP.SaveToFile(LightFilter3DFile);
		LightFilter3DFile.close();	

		*/
		
		//------------------------------------------------------------	
		// Load  Light3D FilterPack
		//------------------------------------------------------------	
		ifstream	LightFilter3DFile2;//(FILE_FILTER_LIGHT3D, std::ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT3D").c_str(), LightFilter3DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter3DFile2);
		LightFilter3DFile2.close();

		//------------------------------------------------------
		// lightBuffer 초기화
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );		
  		
		m_pLightBufferTexture = new CSpriteSurface;
		m_pLightBufferTexture->InitTextureSurface(SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT, 0, CDirect3D::GetPixelFormat4444());	
	
	}
	//------------------------------------------------------------	
	//
	//  2D 시야처리를 위한 Light Filter
	//
	//------------------------------------------------------------	
	else
	{
		/*
		//------------------------------------------------------
		// Light FilterPack 초기화
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter 생성
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126 };
		int l,i,j;

		// MAX_LIGHT_SETSIZE만큼 밝기의 크기 종류가 있다.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k값이 0~24까지..

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
				
					if (k <= maxCenter) k = 0;			// maxCenter까지는 0
					else if (k >= maxK) k = maxLight;	// 넘으면 maxLight
					else k = k - maxCenter;				// 나머지는 - maxCenter

					// 밝기의 SET단계를 만든다.
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
		ofstream	LightFilter2DFile(FILE_FILTER_LIGHT2D, std::ios::binary);
		m_LightFTP.SaveToFile(LightFilter2DFile);
		LightFilter2DFile.close();	
		*/

		//------------------------------------------------------
		// lightBuffer 초기화
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );	
		
		//------------------------------------------------------------	
		// Load  Light2D FilterPack
		//------------------------------------------------------------	
		ifstream	LightFilter2DFile2;//(FILE_FILTER_LIGHT2D, std::ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT2D").c_str(), LightFilter2DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter2DFile2);
		LightFilter2DFile2.close();

		m_p2DLightPixelWidth = new int [SCREENLIGHT_WIDTH];
		m_p2DLightPixelHeight = new int [SCREENLIGHT_HEIGHT];

		//----------------------------------------------------------------
		// LightBuffer의 한 점이 의미하는 화면상의 pixel가로 길이
		//----------------------------------------------------------------
		// 가로의 합 = 100 * 8 = 800
		const int pPixelWidth[SCREENLIGHT_WIDTH] = 
		{
			12, 13, 12, 13, 12,	13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13, 
			12, 13, 12, 13, 12, 13, 12, 13 
		};

		//----------------------------------------------------------------
		// LightBuffer의 한 점이 의미하는 화면상의 pixel세로 길이
		//----------------------------------------------------------------
		// 개수는 SCREENLIGHT_HEIGHT	
		// total값이 CLIPSURFACE_HEIGHT와 같으면 된다.
		// 현재는 600.  10*24 + 9*40	
		//----------------------------------------------------------------
		const int pPixelHeight[SCREENLIGHT_HEIGHT] = 
		{
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
			10,9,9,10, 9,9,10,9,
		};

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
	// Player를 가리는 ImageObject들을 처리할 Filter
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

			//k -= 50;	// 빈 공간

			k >>= 2;	// 부드러운(?) 공간

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
			
			// 0과 1값만...
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
	ofstream	ImageObjectFilterFile(FILE_FILTER_IMAGEOBJECT, std::ios::binary);
	m_ImageObjectFilter.SaveToFile(ImageObjectFilterFile);
	ImageObjectFilterFile.close();	
	*/
	
	//------------------------------------------------------------	
	// Load  Light3D FilterPack
	//------------------------------------------------------------	
	if (m_ImageObjectFilter.IsNotInit())
	{
		ifstream	ImageObjectFilterFile2;//(FILE_FILTER_IMAGEOBJECT, std::ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_IMAGEOBJECT").c_str(), ImageObjectFilterFile2))
			return false;
		m_ImageObjectFilter.LoadFromFile(ImageObjectFilterFile2);
		ImageObjectFilterFile2.close();
	}

	return true;
}

//----------------------------------------------------------------------
// Character의 정보를 생성한다.
//----------------------------------------------------------------------
// m_CreatureFrame[종류][Action][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitCreatureFrames()
{		
	//------------------------------------------------------------
	//
	//  Creature Body의 Frame 정보를 생성한다.
	//
	//------------------------------------------------------------
		 
	/*
	m_CreatureFPK.Init(MAX_CREATURE_BODY);	

	//-----------------------
	// 첫번째 Creature(Woman)
	//-----------------------
	// 동작 개수 정의
	m_CreatureFPK[0].Init(ACTION_MAX);

	// n번째 Sprite
	WORD n = 0;
	int i,j,k;

	// 각 동작에 8방향의 FrameArray가 있다.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[0][k].Init(8);

	// 0~8번째 방향에는 각각 8개의 Frame이 있다.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[0][k][i].Init(8);		
		
		// 8개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<8; j++)
		{
			for (k=0; k<ACTION_MAX; k++)
				m_CreatureFPK[0][k][i][j].Set(n, 0,-40);			

			n ++;
		}  		
	}	

	//-----------------------
	// 두번째 Creature(Skeleton)
	//-----------------------
	// 동작 개수 정의
	m_CreatureFPK[1].Init(ACTION_MAX);

	// 각 동작에 8방향의 FrameArray가 있다.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[1][k].Init(8);	

	// sprite 순서
	int step[] = { 2,3,4,3,2,1,0,1 };


	// n번째 Sprite
	n = 64;

	// 0~8번째 방향에는 각각 8개의 Frame이 있다.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[1][k][i].Init(8);		
		
		// 8개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<8; j++)
		{			
			for (k=0; k<ACTION_MAX; k++)			
				m_CreatureFPK[1][k][i][j].Set(n,step[j], 0,-40);
		}  

		n += 5;
	}	


	//-----------------------
	// 세번째 Creature(Vamp1)
	//-----------------------
	// 동작 개수 정의
	m_CreatureFPK[2].Init(ACTION_MAX_VAMPIRE);

	// n번째 Sprite
	n = 104;
	
	// 각 동작에 8방향의 FrameArray가 있다.
	for (k=0; k<ACTION_MAX_VAMPIRE; k++)
		m_CreatureFPK[2][k].Init(8);

	//------------------------------------------------
	// ACTION_STAND - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_STAND][i].Init(16);
		
		int index = 0;
		// 4개의 Frame에 대한 정보를 Set한다.
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
	// ACTION_MOVE - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_MOVE][i].Init(8);
		
		// 8개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<7; j++)
		{
			// 0~7 frame
			m_CreatureFPK[2][ACTION_MOVE][i][j].Set(n, 15,-60);
			n ++;
		}
		// 8번째 frame
		m_CreatureFPK[2][ACTION_MOVE][i][7].Set(n-7, 15,-60);
	}

	//------------------------------------------------
	// ACTION_ATTACK - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_ATTACK][i].Init(8);
		
		// 8개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<8; j++)
		{
			m_CreatureFPK[2][ACTION_ATTACK][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_MAGIC - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_MAGIC][i].Init(8);
		
		// 8개의 Frame에 대한 정보를 Set한다.
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
	// ACTION_DAMAGED - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_DAMAGED][i].Init(6);
		
		// 6개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<6; j++)
		{
			m_CreatureFPK[2][ACTION_DAMAGED][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_DRAINED - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_DRAINED][i].Init(7);
		
		// 7개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_DRAINED][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_DRAINED][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_DRAINED][i][6].Set(n-4, 15,-60);
	}

	//------------------------------------------------
	// ACTION_DIE - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_DIE][i].Init(4);
		
		// 4개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<4; j++)
		{
			m_CreatureFPK[2][ACTION_DIE][i][j].Set(n, 15,-60);
			//n ++;
		}
	}

	//------------------------------------------------
	// ACTION_VAMPIRE_DRAIN - 8방향에 대해서
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 방향당 Frame수 
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i].Init(7);
		
		// 7개의 Frame에 대한 정보를 Set한다.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][6].Set(n-4, 15,-60);
	}

	
	ofstream packFile(FILE_CFRAME_CREATURE, std::ios::binary);
	ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, std::ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/
	
	///*
	// Load from File
	ifstream file;//(FILE_CFRAME_CREATURE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE").c_str(), file))
		return false;
	m_CreatureFPK.LoadFromFile(file);
	file.close();
	//*/	
	DrawTitleLoading();
	//m_CreatureFPK.InfoToFile("log\\CreatureFPK.txt");

	// Frame2 (좀비)의 죽기 동작을 y좌표 ,40씩 해준다.
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
	
	ofstream packFile(FILE_CFRAME_CREATURE, std::ios::binary);
	ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, std::ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();
	*/


	/*
	// Damaged 0,1을 1,0으로 바꾼다.
	for (int ct=0; ct<m_CreatureFPK.GetSize(); ct++)
	{
		DIRECTION_FRAME_ARRAY& damaged = m_CreatureFPK[ct][ACTION_DAMAGED];

		for (int d=0; d<8; d++)
		{
			FRAME_ARRAY& dfr = damaged[d];

			// 0과 1 framd을 바꿔준다.
			CFrame frame0 = dfr[0];
			CFrame frame1 = dfr[1];

			CFrame temp = frame0;			
			frame0 = frame1;
			frame1 = temp;
		}
	}
	*/

	//m_CreatureFPK.InfoToFile("creature.txt");

	
	// 정지 4frame을 6frame으로 바꿔서 다시 18frame으로..
	/*
	DIRECTION_FRAME_ARRAY& stand = m_CreatureFPK[1][ACTION_STAND];
	DIRECTION_FRAME_ARRAY standTemp;
	DIRECTION_FRAME_ARRAY standTemp2;
	standTemp = stand;
	standTemp2.Init( 8 );
	for (int d=0; d<8; d++)
	{		
		// 4frame을  6frame으로..		
		standTemp2[d].Init( standTemp[d].GetSize(),2 );
		for (int f=0; f<standTemp[d].GetSize(); f++)
		{
			standTemp2[d][f] = standTemp[d][f];			
		}
		standTemp2[d][4] = standTemp[d][2];	
		standTemp2[d][5] = standTemp[d][1];	

		// 6frame을 18frame으로..
		stand[d].Init( standTemp2[d].GetSize()*3 );
		int ff=0;
		for (f=0; f<standTemp2[d].GetSize(); f++)
		{
			stand[d][ff++] = standTemp2[d][f];			
			stand[d][ff++] = standTemp2[d][f];
			stand[d][ff++] = standTemp2[d][f];
		}
	}	

	// 걷기 동작 6frame을 12frame으로...
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

	ofstream packFile(FILE_CFRAME_CREATURE, std::ios::binary);
	ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, std::ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/

	//------------------------------------------------------------
	//
	// Creature Shadow FPK - Loading
	//
	//------------------------------------------------------------
	ifstream fileShadow;//(FILE_CFRAME_CREATURE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE_SHADOW").c_str(), fileShadow))
		return false;
	m_CreatureShadowFPK.LoadFromFile(fileShadow);
	fileShadow.close();
	DrawTitleLoading();
	//m_CreatureShadowFPK.InfoToFile("log\\CreatureShadowFPK.txt");
	
	//------------------------------------------------------------
	//
	//  addon의 Frame 정보를 생성한다.
	//
	//------------------------------------------------------------
	//
	/*
	m_AddonFPK.Init(MAX_CREATURE_ADDON);
	

	//--------------
	// 첫번째 옷
	//--------------
	// 1가지 동작만 있다.
	m_AddonFPK[0].Init(1);
	m_AddonFPK[1].Init(1);
	m_AddonFPK[2].Init(1);
	m_AddonFPK[3].Init(1);

	// 그 1가지 동작에 8방향의 FrameArray가 있다.
	m_AddonFPK[0][0].Init(8);
	m_AddonFPK[1][0].Init(8);
	m_AddonFPK[2][0].Init(8);
	m_AddonFPK[3][0].Init(8);

	// n번째 tile
	//n = n + 80;
	n = 0;

	// 0~8번째 방향에는 각각 8개의 Frame이 있다.
	for (i=0; i<8; i++)
	{
		m_AddonFPK[0][0][i].Init(8);
		m_AddonFPK[1][0][i].Init(8);
		m_AddonFPK[2][0][i].Init(8);
		m_AddonFPK[3][0][i].Init(8);
		
		// 8개의 Frame에 대한 정보를 Set한다.
		for (int j=0; j<8; j++)
		{
			// 83~122 : Shirt 그림이 있다고 했을 경우
			m_AddonFPK[0][0][i][j].Set(n,step[j], 0,-40);

			// 123~162 Pants 그림이 있다고 했을 경우
			m_AddonFPK[1][0][i][j].Set(n,40,step[j], 0,-40);

			// Boots
			m_AddonFPK[2][0][i][j].Set(n,80,step[j], 0,-40);

			// Cloak
			m_AddonFPK[3][0][i][j].Set(n,120,step[j], 0,-40);
		}

		n+=5;
	}	

	packFile.open(FILE_CFRAME_ADDON, std::ios::binary);
	indexFile.open(FILE_CFRAMEINDEX_ADDON, std::ios::binary);
	m_AddonFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();
	*/

	///*
	
	ifstream AdvancementOustersFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	ifstream AdvancementOustersShadowFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS_SHADOW").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersShadowFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	ifstream AdvancementVampireManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN").c_str(), 
		AdvancementVampireManFile))
		return false;
	m_AdvancementVampireManFPK.LoadFromFile( AdvancementVampireManFile );
	AdvancementVampireManFile.close();

	ifstream AdvancementVampireManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN_SHADOW").c_str(), 
		AdvancementVampireManShadowFile))
		return false;
	m_AdvancementVampireManShadowFPK.LoadFromFile( AdvancementVampireManShadowFile );
	AdvancementVampireManShadowFile.close();

	ifstream AdvancementVampireWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN").c_str(), 
		AdvancementVampireWomanFile))
		return false;
	m_AdvancementVampireWomanFPK.LoadFromFile( AdvancementVampireWomanFile );
	AdvancementVampireWomanFile.close();

	ifstream AdvancementVampireWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN_SHADOW").c_str(), 
		AdvancementVampireWomanShadowFile))
		return false;
	m_AdvancementVampireWomanShadowFPK.LoadFromFile( AdvancementVampireWomanShadowFile );
	AdvancementVampireWomanShadowFile.close();


	ifstream AdvancementSlayerManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN").c_str(), 
		AdvancementSlayerManFile))
		return false;
	m_AdvancementSlayerManFPK.LoadFromFile( AdvancementSlayerManFile );
	AdvancementSlayerManFile.close();

	ifstream AdvancementSlayerManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN_SHADOW").c_str(), 
		AdvancementSlayerManShadowFile))
		return false;
	m_AdvancementSlayerManShadowFPK.LoadFromFile( AdvancementSlayerManShadowFile );
	AdvancementSlayerManShadowFile.close();


	ifstream AdvancementSlayerWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN").c_str(), 
		AdvancementSlayerWomanFile))
		return false;
	m_AdvancementSlayerWomanFPK.LoadFromFile( AdvancementSlayerWomanFile );
	AdvancementSlayerWomanFile.close();

	ifstream AdvancementSlayerWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN_SHADOW").c_str(), 
		AdvancementSlayerWomanShadowFile))
		return false;
	m_AdvancementSlayerWomanShadowFPK.LoadFromFile( AdvancementSlayerWomanShadowFile );
	AdvancementSlayerWomanShadowFile.close(); 

	/*
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	ifstream AdvancementSlayerFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	ifstream AdvancementVampireFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();
	
	ifstream AdvancementSlayerShadowFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_SHADOW").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerShadowFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	ifstream AdvancementVampireShadowFile;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_SHADOW").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireShadowFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();

*/
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	ifstream AddonFile2;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON").c_str(), AddonFile2))
		return false;
	m_AddonFPK.LoadFromFile(AddonFile2);
	AddonFile2.close();
	ifstream OustersFile2;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS").c_str(), OustersFile2))
		return false;
	m_OustersFPK.LoadFromFile(OustersFile2);
	OustersFile2.close();
	DrawTitleLoading();

	//------------------------------------------------
	// 그림자 - Load from File
	//------------------------------------------------
	ifstream AddonShadowFile2;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON_SHADOW").c_str(), AddonShadowFile2))
		return false;
	m_AddonShadowFPK.LoadFromFile(AddonShadowFile2);
	AddonShadowFile2.close();
	
	ifstream OustersShadowFile2;//(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS_SHADOW").c_str(), OustersShadowFile2))
		return false;
	m_OustersShadowFPK.LoadFromFile(OustersShadowFile2);
	OustersShadowFile2.close();
	

	DrawTitleLoading();

	// 총동작에 머리카락 빠진것 추가
	// 이래도 뭔가 안되네!!
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

	// 변경..
	/*
	int add, a, d, f;
	for (add=0; add<MAX_ADDONID_MALE; add++)
	{
		ACTION_FRAME_ARRAY& AFA = m_AddonMaleFPK[add];

		// motorcycle에는 action추가
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
					
					// motor동작을 3 --> 4 frame으로 바꾼다.
					if (a==ACTION_SLAYER_MOTOR_MOVE)
					{
						if (FA.GetSize()!=0)
						{
							FRAME_ARRAY newFA;
							newFA.Init( 4 );
							// 3개짜리 FA를 4개로 만들어야 한다.
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
	// [남자] 칼동작은 6 frame으로 바꾸기...
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
	
	// [여자] 칼동작은 6 frame으로 바꾸기...
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

	ofstream packFileMale(FILE_CFRAME_ADDON_MALE, std::ios::binary);
	ofstream indexFileMale(FILE_CFRAMEINDEX_ADDON_MALE, std::ios::binary);
	m_AddonMaleFPK.SaveToFile(packFileMale, indexFileMale);	
	packFileMale.close();
	indexFileMale.close();

	ofstream packFileFemale(FILE_CFRAME_ADDON_FEMALE, std::ios::binary);
	ofstream indexFileFemale(FILE_CFRAMEINDEX_ADDON_FEMALE, std::ios::binary);
	m_AddonFemaleFPK.SaveToFile(packFileFemale, indexFileFemale);	
	packFileFemale.close();
	indexFileFemale.close();

	*/

	
	// 여자 총 동작 바꾸기
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
// CImageFramePack			m_ItemTileFPK;			// Tile에 있는 Item에 대한..
// m_ImageFrame[종류]
//----------------------------------------------------------------------
bool
MTopView::InitImageFrames()
{
	//------------------------------------------------------------
	//
	//  Item on Tile 정보를 생성한다.
	//
	//------------------------------------------------------------
	/*
	m_ItemTileFPK.Init( 28 );
	
	// Item 0	
	for (int i=0; i<28; i++)
		m_ItemTileFPK[i].Set(i, 10, 10);

	ofstream packFile(FILE_IFRAME_ITEMTILE, std::ios::binary);
	ofstream indexFile(FILE_IFRAMEINDEX_ITEMTILE, std::ios::binary);

	m_ItemTileFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
	*/

	///*
	// Load from File
	ifstream file2;//(FILE_IFRAME_ITEMTILE, std::ios::binary);
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
// m_ImageObjectFPK[종류][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitAnimationFrames()
{		
	//------------------------------------------------------------
	//
	//  Animation Frame 정보를 생성한다.
	//
	//------------------------------------------------------------

	//------------------------------------------------------------
	//
	//  이 정보는 FrameSet이다.
	//
	//  SpriteSet에서의 SpriteID를 저장하고 있어야 한다.
	//
	//------------------------------------------------------------
	
	ifstream file2;//(FILE_AFRAME_ANIMATIONOBJECT, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT").c_str(), file2))
		return false;
	m_ImageObjectFPK.LoadFromFile(file2);
	file2.close();

	ifstream file3;//(FILE_AFRAME_ANIMATIONOBJECT, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT_SHADOW").c_str(), file3))
		return false;
	m_ImageObjectShadowFPK.LoadFromFile(file3);
	file3.close();	

//#ifdef OUTPUT_DEBUG
//	m_InteractionObjectFPK.Init( 1 );
//	
//	//------------------------------------------
//	// 첫번째 Animation Object
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
//	ofstream iopackFile(g_pFileDef->getProperty("FILE_AFRAME_INTERACTIONOBJECT").c_str(), std::ios::binary);
//	ofstream ioindexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_INTERACTIONOBJECT").c_str(), std::ios::binary);
//	m_InteractionObjectFPK.SaveToFile(iopackFile, ioindexFile);
//	iopackFile.close();
//	ioindexFile.close();	
//#endif

	///*
	// Load from File
//	ifstream iofile2;//(FILE_AFRAME_ANIMATIONOBJECT, std::ios::binary);
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
	ifstream itemdropfile2;//(FILE_AFRAME_ANIMATIONOBJECT, std::ios::binary);
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
//	ofstream itemdropfile3(g_pFileDef->getProperty("FILE_AFRAME_ITEMDROP").c_str(), std::ios::binary);
//	ofstream itemdropfile4(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMDROP").c_str(), std::ios::binary);
//	m_ItemDropFPK.SaveToFile(itemdropfile3, itemdropfile4);
//	itemdropfile4.close();
//	itemdropfile3.close();

	//------------------------------------------------------------	
	// 다시 생성
	//------------------------------------------------------------	
	/*
	const int numItems	= 121;
	const int numFrames	= 6;

	// frame 변화 값 
	// sprite는 item마다 4개씩인데 6 frame을 만들어야 한다.
	int nFrame[numFrames] = { 3, 0, 1, 2, 3, 0 };

	m_ItemDropFPK.Init( numItems );	// 전체 item 개수만큼의 ani frame

	int spriteID = 0;
	for (int i=0; i<numItems; i++)
	{
		m_ItemDropFPK[i].Init( numFrames );		// 각각 6 frame씩

		for (int f=0; f<numFrames; f++)
		{
			int sid = spriteID+nFrame[f];

			// 중심에 오도록 좌표 보정..
			int cx = TILE_X_HALF - (m_ItemDropSPK[sid].GetWidth()>>1);
			int cy = TILE_Y_HALF - (m_ItemDropSPK[sid].GetHeight()>>1);

			m_ItemDropFPK[i][f].Set( sid, cx, cy );
		}
		
		spriteID += 4;
	}

	//------------------------------------------------------------	
	// 저장
	//------------------------------------------------------------	
	ofstream packFile(FILE_AFRAME_ITEMDROP, std::ios::binary);
	ofstream indexFile(FILE_AFRAMEINDEX_ITEMDROP, std::ios::binary);
	m_ItemDropFPK.SaveToFile(packFile, indexFile);
	packFile.close();
	indexFile.close();
	*/

		//------------------------------------------------------------
	//
	//  Item Broken 정보를 생성한다.
	//
	//------------------------------------------------------------
	///*
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
	const int maxSlayerItemBroken = (MSlayerGear::MAX_GEAR_SLAYER+2-12) * 3;
	const int maxVampireItemBroken = (MVampireGear::MAX_GEAR_VAMPIRE+2-12) * 3;
	const int maxOustersItemBroken = (MOustersGear::MAX_GEAR_OUSTERS+1-12) * 3;

	m_ItemBrokenFPK.Init( 5 );	// 슬레이어 남,여 + 뱀파이어 남,여


	// { SpriteID, CX, CY }
	//------------------------------------------------------------
	// 슬레이어 남자
	//------------------------------------------------------------
	int slayerMale[maxSlayerItemBroken][3] =
	{
		// 정상적인거
		{ SPRITEID_NULL,	33, 1 },	// 모자,
		{ SPRITEID_NULL,	 38, 18 },	// 목걸이
		{ SPRITEID_NULL,	16, 19 },	// 상의
		{ 1,	62, 19 },	// 방패	
		{ 2,	19, 11 },	// 칼		
		{ SPRITEID_NULL,	9, 49 },	// 장갑
		{ SPRITEID_NULL,	27, 54 },	// 벨트
		{ SPRITEID_NULL,	26, 59 },	// 하의
		{ 5,	59, 51 },	// 팔찌2		
		{ 5,	24, 51 },	// 팔찌1
		{ 6,	20, 60 },	// 반지1
		{ 6,	28, 60 },	// 반지2
		{ 6,	55, 60 },	// 반지3
		{ 6,	63, 60 },	// 반지4
		{ SPRITEID_NULL,	20, 122 },	// 신발
		{ 3,	18, 12 },	// 십자가
		{ 4,	20, 12 },	// 총

		// 약간 부서진거
		{ 9,	40, 10 },	// 모자,
		{ 10,	42, 19 },	// 목걸이
		{ 7,	33, 18 },	// 상의
		{ 14,	62, 19 },	// 방패	
		{ 15,	19, 11 },	// 칼		
		{ 11,	30, 35 },	// 장갑
		{ 12,	37, 36 },	// 벨트
		{ 8,	36, 35 },	// 하의
		{ 18,	59, 51 },	// 팔찌2		
		{ 18,	24, 51 },	// 팔찌1
		{ 19,	20, 60 },	// 반지1
		{ 19,	28, 60 },	// 반지2
		{ 19,	55, 60 },	// 반지3
		{ 19,	63, 60 },	// 반지4
		{ 13,	36, 72 },	// 신발
		{ 16,	18, 12 },	// 십자가
		{ 17,	20, 12 },	// 총

		// 다 부서져 가는거
		{ 22,	40, 10 },	// 모자,
		{ 23,	42, 19 },	// 목걸이
		{ 20,	33, 18 },	// 상의
		{ 27,	62, 19 },	// 방패	
		{ 28,	19, 11 },	// 칼		
		{ 24,	30, 35 },	// 장갑
		{ 25,	37, 36 },	// 벨트
		{ 21,	36, 35 },	// 하의
		{ 31,	59, 51 },	// 팔찌2		
		{ 31,	24, 51 },	// 팔찌1
		{ 32,	20, 60 },	// 반지1
		{ 32,	28, 60 },	// 반지2
		{ 32,	55, 60 },	// 반지3
		{ 32,	63, 60 },	// 반지4
		{ 26,	36, 72 },	// 신발
		{ 29,	18, 12 },	// 십자가
		{ 30,	20, 12 },	// 총
	};

	//------------------------------------------------------------
	// 슬레이어 여자
	//------------------------------------------------------------
	int slayerFemale[maxSlayerItemBroken][3] =
	{
		// 정상적인거
		{ SPRITEID_NULL,	33, 1 },	// 모자,
		{ SPRITEID_NULL,	 38, 18 },	// 목걸이
		{ SPRITEID_NULL,	16, 19 },	// 상의
		{ 1,	62, 19 },	// 방패	
		{ 2,	19, 11 },	// 칼		
		{ SPRITEID_NULL,	9, 49 },	// 장갑
		{ SPRITEID_NULL,	27, 54 },	// 벨트
		{ SPRITEID_NULL,	26, 59 },	// 하의
		{ 5,	59, 51 },	// 팔찌2		
		{ 5,	24, 51 },	// 팔찌1
		{ 6,	20, 60 },	// 반지1
		{ 6,	28, 60 },	// 반지2
		{ 6,	55, 60 },	// 반지3
		{ 6,	63, 60 },	// 반지4
		{ SPRITEID_NULL,	20, 122 },	// 신발
		{ 3,	18, 12 },	// 십자가
		{ 4,	20, 12 },	// 총

		// 약간 부서진거
		{ 36,	40, 10 },	// 모자,
		{ 37,	41, 20 },	// 목걸이
		{ 34,	33, 20 },	// 상의
		{ 14,	62, 19 },	// 방패	
		{ 15,	19, 11 },	// 칼		
		{ 38,	30, 37 },	// 장갑
		{ 39,	40, 32 },	// 벨트
		{ 35,	38, 35 },	// 하의
		{ 18,	59, 51 },	// 팔찌2		
		{ 18,	24, 51 },	// 팔찌1
		{ 19,	20, 60 },	// 반지1
		{ 19,	28, 60 },	// 반지2
		{ 19,	55, 60 },	// 반지3
		{ 19,	63, 60 },	// 반지4
		{ 40,	39, 55 },	// 신발
		{ 16,	18, 12 },	// 십자가
		{ 17,	20, 12 },	// 총

		// 다 부서져 가는거
		{ 43,	40, 10 },	// 모자,
		{ 44,	41, 20 },	// 목걸이
		{ 41,	33, 20 },	// 상의
		{ 14,	62, 19 },	// 방패	
		{ 15,	19, 11 },	// 칼		
		{ 45,	30, 37 },	// 장갑
		{ 46,	40, 32 },	// 벨트
		{ 42,	38, 35 },	// 하의
		{ 18,	59, 51 },	// 팔찌2		
		{ 18,	24, 51 },	// 팔찌1
		{ 19,	20, 60 },	// 반지1
		{ 19,	28, 60 },	// 반지2
		{ 19,	55, 60 },	// 반지3
		{ 19,	63, 60 },	// 반지4
		{ 47,	39, 55 },	// 신발
		{ 16,	18, 12 },	// 십자가
		{ 17,	20, 12 },	// 총
	};

	//------------------------------------------------------------
	// 뱀파이어 남자
	//------------------------------------------------------------
	int vampireMale[maxVampireItemBroken][3] =
	{
		// 정상적인거
		{ SPRITEID_NULL,	40, 22 },	// 목걸이
		{ SPRITEID_NULL,	19, 10 },	// 상의
		{ 49,	22, 45 },	// 팔찌1
		{ 49,	62, 45 },	// 팔찌2
		{ 50,	18, 54 },	// 반지1
		{ 50,	26, 54 },	// 반지2
		{ 50,	58, 54 },	// 반지3
		{ 50,	66, 54 },	// 반지4
		{ 62,	22, 18 },	// 귀걸이1
		{ 62,	62, 18 },	// 귀걸이2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// 약간 부서진거
		{ 52,	43, 18 },	// 목걸이
		{ 51,	30, 14 },	// 상의
		{ 53,	22, 45 },	// 팔찌1
		{ 53,	62, 45 },	// 팔찌2
		{ 54,	18, 54 },	// 반지1
		{ 54,	26, 54 },	// 반지2
		{ 54,	58, 54 },	// 반지3
		{ 54,	66, 54 },	// 반지4
		{ 63,	22, 18 },	// 귀걸이1
		{ 63,	62, 18 },	// 귀걸이2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// 다 부서진거
		{ 56,	43, 18 },	// 목걸이
		{ 55,	30, 14 },	// 상의
		{ 57,	22, 45 },	// 팔찌1
		{ 57,	62, 45 },	// 팔찌2
		{ 58,	18, 54 },	// 반지1
		{ 58,	26, 54 },	// 반지2
		{ 58,	58, 54 },	// 반지3
		{ 58,	66, 54 },	// 반지4
		{ 64,	22, 18 },	// 귀걸이1
		{ 64,	62, 18 },	// 귀걸이2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};

	//------------------------------------------------------------
	// 뱀파이어 여자
	//------------------------------------------------------------
	int vampireFemale[maxVampireItemBroken][3] =
	{
		// 정상적인거
		{ SPRITEID_NULL,	40, 22 },	// 목걸이
		{ SPRITEID_NULL,	19, 10 },	// 상의
		{ 49,	22, 45 },	// 팔찌1
		{ 49,	62, 45 },	// 팔찌2
		{ 50,	18, 54 },	// 반지1
		{ 50,	58, 54 },	// 반지3
		{ 50,	26, 54 },	// 반지2		
		{ 50,	66, 54 },	// 반지4
		{ 62,	22, 18 },	// 귀걸이1
		{ 62,	62, 18 },	// 귀걸이2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// 약간 부서진거
		{ 52,	42, 18 },	// 목걸이
		{ 60,	31, 19 },	// 상의
		{ 53,	22, 45 },	// 팔찌1
		{ 53,	62, 45 },	// 팔찌2
		{ 54,	18, 54 },	// 반지1
		{ 54,	58, 54 },	// 반지3
		{ 54,	26, 54 },	// 반지2		
		{ 54,	66, 54 },	// 반지4
		{ 63,	22, 18 },	// 귀걸이1
		{ 63,	62, 18 },	// 귀걸이2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// 다 부서진거
		{ 56,	42, 18 },	// 목걸이
		{ 61,	31, 19 },	// 상의
		{ 57,	22, 45 },	// 팔찌1
		{ 57,	62, 45 },	// 팔찌2
		{ 58,	18, 54 },	// 반지1
		{ 58,	58, 54 },	// 반지3
		{ 58,	26, 54 },	// 반지2		
		{ 58,	66, 54 },	// 반지4
		{ 64,	22, 18 },	// 귀걸이1
		{ 64,	62, 18 },	// 귀걸이2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};		

	//------------------------------------------------------------
	// 아우스터즈
	//------------------------------------------------------------
	int Ousters[maxOustersItemBroken][3] =
	{
		// 정상적인거
		{ 77,	39, 8 },				// 서클릿
		{ SPRITEID_NULL ,	34, 18 },				// 옷
		{ 89,	17, 33 },				// 왼손				// 리스틀릿은 92
		{ 89,	61, 33 },				// 오른손
		{ SPRITEID_NULL,	40, 69 },				// 신발
		{ 74,	19, 18},				// 암스밴드1
		{ 74,	57, 18},				// 암스밴드2
		{ 83,	19, 63 },				// 링1
		{ 83,	65, 63 },				// 링2
		{ 86,	35, 22 },				// 목걸이1
		{ 86,	42, 22 },				// 목걸이2
		{ 86,	49, 22 },				// 목걸이3
		{ 80,	26, 63 },				// 정령석1		
		{ 80,	58, 63 },				// 정령석2
		{ 80,	26, 71 },				// 정령석3
		{ 80,	58, 71 },				// 정령석 4 
		{ 92,	61, 33 },				// 리스틀릿 

		// 중간
		{ 78,	40, 22 },				// 서클릿
		{ 72,	19, 10 },	// 옷
		{ 90,	22, 45 },				// 왼손				// 리스틀릿은 92
		{ 90,	62, 45 },				// 오른손
		{ 95,	18, 54 },				// 신발
		{ 75,	26, 54 },				// 암스밴드1
		{ 75,	58, 54 },				// 암스밴드2
		{ 84,	66, 54 },				// 링1
		{ 84,	22, 18 },				// 링2
		{ 87,	62, 18 },				// 목걸이1
		{ 87,	16, 22 },				// 목걸이2
		{ 87,	16, 22 },				// 목걸이3
		
		{ 81,	26, 63 },				// 정령석1		
		{ 81,	58, 63 },				// 정령석2
		{ 81,	26, 71 },				// 정령석3
		{ 81,	58, 71 },				// 정령석 4 

		{ 93,	61, 33 },				// 리스틀릿  

		// 나쁜거
		{ 79,	40, 22 },				// 서클릿
		{ 73,	19, 10 },	// 옷
		{ 91,	22, 45 },				// 왼손				// 리스틀릿은 92
		{ 91,	62, 45 },				// 오른손
		{ 96,	18, 54 },				// 신발
		{ 76,	26, 54 },				// 암스밴드1
		{ 76,	58, 54 },				// 암스밴드2
		{ 85,	66, 54 },				// 링1
		{ 85,	22, 18 },				// 링2
		{ 88,	62, 18 },				// 목걸이1
		{ 88,	16, 22 },				// 목걸이2
		{ 88,	16, 22 },				// 목걸이3
		{ 82,	26, 63 },				// 정령석1		
		{ 82,	58, 63 },				// 정령석2
		{ 82,	26, 71 },				// 정령석3
		{ 82,	58, 71 },				// 정령석 4 
		{ 94,	61, 33 },				// 리스틀릿
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
	
	ofstream packFile(g_pFileDef->getProperty("FILE_AFRAME_ITEMBROKEN").c_str(), std::ios::binary);
	ofstream indexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMBROKEN").c_str(), std::ios::binary);

	m_ItemBrokenFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
#endif
	//*/

	///*
	// Load from File
	ifstream fileItemBroken2;//(FILE_IFRAME_ITEMTILE, std::ios::binary);
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
// m_EffectAlphaFPK[종류][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitEffectFrames()
{
	//------------------------------------------------
	//
	//   Effect NormalSPK
	//
	//------------------------------------------------	
	// 0번 : HolyWater 날아가는 effect
	// 1번 : Bomb 날아가는 effect
	/*
	m_EffectNormalFPK.Init( 2 );

	for (int e=0; e<2; e++)
	{
		m_EffectNormalFPK[e].Init( 8 );

		for (int d=0; d<8; d++)
		{
			m_EffectNormalFPK[e][d].Init( 4 );	// 각 4 frame

			for (int f=0; f<4; f++)
			{
				int spriteID = e*4 + f;
				int cx = -(m_EffectNormalSPK[spriteID].GetWidth()>>1);
				int cy = -(m_EffectNormalSPK[spriteID].GetHeight()>>1);

				m_EffectNormalFPK[e][d][f].Set(spriteID, cx, cy, 0);
			}
		}	
	}

	ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, std::ios::binary);
	ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, std::ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/
	/*
	m_EffectNormalFPK.Init(MAX_NORMALEFFECT);

	//-------------------
	// 첫번째 Effect
	//-------------------
	// 8방향이 있다.
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

	ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, std::ios::binary);
	ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, std::ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	//--------------------------------------------------
	// 하나의 FRR을 읽어서.. 
	// EFRR로 변환시켜서..
	// EFPK를 생성한다.
	//--------------------------------------------------
	// [ TEST CODE ]
	/*
	ifstream fileFA;//(FILE_EFRAME_NORMALEFFECT, std::ios::binary);
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

	ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, std::ios::binary);
	ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, std::ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	///*
	// Load from NormalFile
	ifstream NormalFile2;//(FILE_EFRAME_NORMALEFFECT, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_NORMALEFFECT").c_str(), NormalFile2))
		return false;
	m_EffectNormalFPK.LoadFromFile(NormalFile2);
	NormalFile2.close();
	//*/

	// 세부정보 출력하기
	/*
	ofstream infoFile("Log\\EffectNormal.txt");	

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
	// 첫번째 Effect
	//-------------------
	// 8방향이 있다.
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
		// 그 8방향씩에  8 Frames
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

	ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, std::ios::binary);
	ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, std::ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	///*
	// Load from AlphaFile
	ifstream AlphaFile2;//(FILE_EFRAME_ALPHAEFFECT, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_ALPHAEFFECT").c_str(), AlphaFile2))
		return false;
	//ifstream AlphaFile2("effect.efpk", std::ios::binary);
	m_EffectAlphaFPK.LoadFromFile(AlphaFile2);
	AlphaFile2.close();
	//*/

	/*
	// 세부 정보 출력하기
	//m_EffectAlphaFPK.InfoToFile("Log\\Effect.txt");
	ofstream infoFile("Log\\Effect.txt");	

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
	// 각 effect의 frame수 출력하기
	//---------------------------------------------------------------
	ofstream file("log\\EffectList.txt");
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& DEA = m_EffectAlphaFPK[e];
		
		file << "[" << e << "] " << (int)DEA[0].GetSize() << endl;
	}
	file.close();
	*/
	

	//---------------------------------------------------------------
	// 빛 조절하기
	//---------------------------------------------------------------
	/*
	int light;
	
	// effect 종류
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[e];

		// 방향별
		for (int d=0; d<8; d++)
		{
			EFFECTFRAME_ARRAY& EA = effect[d];
			
			// frame별
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

	ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, std::ios::binary);
	ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, std::ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	/*
	//---------------------------------------------------------------
	// 피 sprite 중복 frame넣기
	//---------------------------------------------------------------
	DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[EFFECTSPRITETYPE_SLAYER_DIE];

	for (int d=0; d<8; d++)
	{
		EFFECTFRAME_ARRAY& EA = effect[d];

		int frameMax = EA.GetSize()*3 + 30;

		EFFECTFRAME_ARRAY newEA;

		newEA.Init( frameMax );

		// 모두 끝 동작...
		for (int i=0; i<frameMax; i++)
		{
			newEA[i] = EA[EA.GetSize()-1];
		}
		
		// 두frame씩, 정지 frame 계속, 한 frame거꾸로
		for (i=0; i<EA.GetSize(); i++)
		{
			newEA[i*2] = EA[i];
			newEA[i*2,1] = EA[i];

			newEA[frameMax-1 - i] = EA[i];
		}	

		EA = newEA;
	}

	ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, std::ios::binary);
	ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, std::ios::binary);
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
//		ofstream packshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), std::ios::binary);
//		ofstream indexshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAMEINDEX_SHADOWEFFECT").c_str(), std::ios::binary);
//		m_EffectShadowFPK.SaveToFile(packshadowEffectFile, indexshadowEffectFile);
//		packshadowEffectFile.close();
//		indexshadowEffectFile.close();
//	#endif

	///*
	// Load from shadowEffectFile
	ifstream shadowEffectFile2;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), shadowEffectFile2))
		return false;
	m_EffectShadowFPK.LoadFromFile(shadowEffectFile2);
	shadowEffectFile2.close();

	// Load from ScreenFile
	ifstream ScreenFile2;//(FILE_EFRAME_ScreenEFFECT, std::ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SCREENEFFECT").c_str(), ScreenFile2))
		return false;
	//ifstream ScreenFile2("effect.efpk", std::ios::binary);
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
	/* // 2001.7.14 주석처리
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	ifstream	MapTest1;
	if (!FileOpenBinary(filename, MapTest1))
		return;
	m_pMinimapSPR->LoadFromFile( MapTest1 );
	*/

	//------------------------------------------------------------	
	// 표시해야할 ZoneInfo가 있는 경우
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
		// Sprite자체를 바꿔버린다. - -;
		//----------------------------------------------------------------
		for (spY=0; spY<spHeight; spY++)
		{
			zoneY = spY * zoneHeight / spHeight;

			spX = 0;
			pPixel = m_pMinimapSPR->GetPixelLine( spY );

			count = *pPixel++;		// 반복 회수
			
			//----------------------------------------------------------------
			// 한 줄에 대한 체크..
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
					// 안전지대이면 녹색으로 반투명..
					//---------------------------------------------------------
					if (pZoneInfo->IsSafeSector(zoneX, zoneY))
					{
						// 1점 녹색으로 alpha blending - -;
						CSpriteSurface::memcpyAlpha(pPixel, &green, 1);
					}

					// 다음 점
					pPixel++;
					spX++;
				}
			}
		}
	}
	*/

	/* // 2001.7.14 주석처리
	//------------------------------------------------------------
	// 3D 가속이면...
	//------------------------------------------------------------
	if (CDirect3D::IsHAL())
	{
		InitMinimapTexture();
	}

	// m_SectorToMinimapWidth - 한 sector는 가로 몇 pixel인가?
	// m_SectorToMinimapHeight - 한 sector는 세로 몇 pixel인가?
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
//	// 스프라이트를 만들자 by 쑥갓
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
//	// 3D 가속이면...
//	//------------------------------------------------------------
//	if (!CDirect3D::IsHAL())
//		return;
//
//	// 2001.7.14 주석처리
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
//	// texture size 결정
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
//	// 초 필살 더티 테스트 코드.. 음냐... - -;;
//	//
//	//-----------------------------------------------------------
//	//
//	// m_pMinimapTexture에 minimap을 출력한다.
//	//
//	//-----------------------------------------------------------
//	//-----------------------------------------------------------
//	// 2의 승수만 지원하는 경우..
//	//-----------------------------------------------------------
//	bool bDifferentSize;	// sprite와 texture의 크기가 다른가?
//	if (CDirect3D::IsTexturePow2())
//	{
//		bDifferentSize = true;
//
//		// width와 height가 다른 경우...
//		// 작은 쪽에 맞춰서 똑같이 한다.
//		// Square로 맞춘다..고 할 수 있지.. 음하..
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
//		// Texture크기보다 Sprite 크기가 더 큰 경우..
//		//-----------------------------------------------------------
//		// 즉, 하드웨어에서 Sprite크기만큼의 Texture를 지원하지 못하는 경우이다.		
//		// shift를 이용해서 크기를 줄인다.
//		while (spWidth > width || spHeight > height)
//		{
//			smallShift ++;
//
//			spWidth >>= 1;
//			spHeight >>= 1;
//		}
//	}
//	//-----------------------------------------------------------
//	// 아무런 size나 관계 없는 경우
//	//-----------------------------------------------------------
//	else
//	{
//		if (CDirect3D::IsTextureSquareOnly())
//		{
//			// width와 height가 다른 경우...
//			// 큰 쪽에 맞춰서 똑같이 한다.
//			// Square로 맞춘다..고 할 수 있지.. 음하..
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
//	// TextureSurface 생성
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
//	// 검게 칠하는 부분..
//	//---------------------------------------------------
//	//if (bDifferentSize)
//	{
//		/*
//		// Sprite가 차지하는 영역을 제외한 부분을 검게~~
//		DWORD width2 = (width - spWidth) << 1;	// *2 
//		pSurface += spWidth;
//
//		// 오른쪽 옆부분
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
//		// 아래쪽
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
//		// Texture Surface 초기화
//		//---------------------------------------------------
//		/*// 2001.7.14 주석처리
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
//	// Sprite출력
//	//---------------------------------------------------
//	// 크기를 고려해서..
//
//	 // 2001.7.14 주석처리
//	// alpha값은 16 == 반투명
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
//	// 출력할때의 크기
//	//---------------------------------------------------------------
//	spWidth = m_pMinimapSPR->GetWidth();
//	spHeight = m_pMinimapSPR->GetHeight();
//	if (CDirect3D::IsTexturePow2())	
//	{
//		width = 1;
//		height = 1;
//
//		// width와 height는 spWidth와 spHeight보다 커야 한다.
//		// square로 맞춰준다.
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
//		// 큰 크기로 맞추어서 Square로 만든다.
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
	// Addon도 어케 해야되는데 - -;
	// Effect도... - -;

	//----------------------------------------------------------
	// 이미 Loading된 것들 제거
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
	// Loading 된게 없다고 표시
	//----------------------------------------------------------
	int num = g_pCreatureSpriteTable->GetSize();

	for (int i=0; i<num; i++)
	{
		(*g_pCreatureSpriteTable)[i].bLoad = false;
	}
	
	if (bUse)
	{
		// 이미 Loading된 것들 제거		
		ifstream file(FILE_CFRAME_CREATURE2, std::ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();		
	}
	else
	{
		// 이미 Loading된 것들 제거
		ifstream file(FILE_CFRAME_CREATURE, std::ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();
	}
}
*/

//----------------------------------------------------------------------
// Load From File CreatureSPK
//----------------------------------------------------------------------
// 현재 Zone에서 출현할 Creature에 대한 Sprite들을 Load한다.
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
	// Load되었는지 체크해본다.
	//----------------------------------------------------------------------
	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
	{
		// 이미 Load된 경우
	}
	//----------------------------------------------------------------------
	// Load해야되는 경우
	//----------------------------------------------------------------------
	// CreatureAction에 따라서 g_pCreatureActionSpriteTable을 이용할 것이므로
	// 실제로 loading하진 않는다.  2001.11.21
	//----------------------------------------------------------------------			
	else
	{
		if(g_pUserOption->IsPreLoadMonster)
		{
			
			// 현재 Zone에서 필요한 Sprite들을 Load하면 된다.
//			ifstream	creatureFile;//(FILE_ISPRITE_CREATURE, std::ios::binary);
//			ifstream	creatureShadowFile;
//
//			//------------------------------------------------------------
//			// sprite load
//			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_ISPRITE_CREATURE, creatureFile))
//				return;	
			

			//------------------------------------------------------------
			// Half Frame을 사용할때
			//------------------------------------------------------------
			if (0)//g_pUserOption->UseHalfFrame)
			{
				/*
				int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

				//------------------------------------------------------------	
				// 필요한 EFPK를 생성하면서 사용된 SpriteID도 구한다.
				//------------------------------------------------------------
				COrderedList<int> intList;
				ACTION_FRAME_ARRAY& AFA = CreatureFPK[frameID];
				int numAction = AFA.GetSize();

				// orderedList의 효율을 높히기 위해서 거꾸로 ..
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

				ifstream CreaturePackIndexFile(FILE_ISPRITEINDEX_CREATURE, std::ios::binary);			
				
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
			// 전체 Frame 다 사용
			//------------------------------------------------------------
			else
			{
				
				TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
				TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
				long			fp	= (*g_pCreatureSpriteTable)[spriteType].SpriteFilePosition;

				m_CreatureSPK.LoadFromFilePart(first, last);
				// n번째 creature load	
//				m_CreatureSPK.LoadFromFilePart(creatureFile, 
//												fp,
//												first, 
//												last
//												);
				
			}

// 			creatureFile.close();

			//------------------------------------------------------------
			// 그림자 load
			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_SSPRITE_CREATURE, creatureShadowFile))
//				return;	

		
//			if (!CDirect3D::IsHAL())
			{
				//------------------------------------------------------------
				// Half Frame 사용
				//------------------------------------------------------------
				if (g_pUserOption->UseHalfFrame)
				{
				}
				//------------------------------------------------------------
				// 전체 Frame 다 사용
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

		// Load했다고 표시한다.
		(*g_pCreatureSpriteTable)[spriteType].bLoad = TRUE;

		//--------------------------------------------------------
		// Player그림일경우는 추가하지 않는다.
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
		// 그림자 Pack생성
		//------------------------------------------------------------
		m_CreatureSSPK.InitPart(m_CreatureSPK, 
								(*g_pCreatureTable)[n].FirstSpriteID,
								(*g_pCreatureTable)[n].LastSpriteID);

		//------------------------------------------------------------
		//
		// Init 그림자 TextureSurface
		//
		//------------------------------------------------------------
		if (CDirect3D::IsHAL())
		{	
			//--------------------------------------------
			// AlphaSPK을 이용해서 TexturePack을 생성한다.
			//--------------------------------------------
			if (!m_CreatureTPK.InitPart( m_CreatureSSPK,
										(*g_pCreatureTable)[n].FirstSpriteID,
										(*g_pCreatureTable)[n].LastSpriteID))
			{
				MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
				return;
			}

			//------------------------------------------------------------	
			// EffectAlphaSprite를 메모리에서 삭제한다.
			// --> TextureSurface를 사용할 것이므로.. 필요가 없다.
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
// 현재 보이는 Addon을 Load한다.
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
//	// Load되었는지 체크해본다.
//	//----------------------------------------------------------------------
//	if ((*g_pAddonSpriteTable)[frameID].bLoad)
//	{
//		// 이미 Load된 경우
//	}
//	//----------------------------------------------------------------------
//	// Load해야되는 경우
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
// 현재 보이는 CreatureAction을 Load한다.
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
//	// Load되었는지 체크해본다.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureActionSpriteTable)[frameID].bLoad)
//	{
//		// 이미 Load된 경우
//	}
//	//----------------------------------------------------------------------
//	// Load해야되는 경우
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
//		//int spriteType = frameID + 2;	// mapping table을 만들어야 하는데.
//										// slayer남녀를 제외하면 순서대로다..
//
//		// loading됐다고 표시해둬야 존 이동 후 release할 수 있다.
//		// map으로 해야한다.
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
//	// Load되었는지 체크해본다.
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
// 현재 Load된 Creature Sprite를 메모리 해제시킨다.
//----------------------------------------------------------------------
void
MTopView::ReleaseCreatureSPK(int n)
{
//	//--------------------------------------------------------
//	// Player그림일경우는 제거하지 않는다.
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
//	// Player그림일경우는 제거하지 않는다.
//	if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
//	{
//		return;
//	}
//
//	//----------------------------------------------------------------------
//	// Load되었는지 체크해본다.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
//	{
//		/*
//		// 이미 Load된 경우
//		// n번째 creature의 sprite를 메모리 해제
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
//		// load하지 않았다고 표시한다.
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
//		// Load되지 않은 경우
//	}
	//--------------------------------------------------------
	// Player그림일경우는 제거하지 않는다.
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
		// Player그림일경우는 제거하지 않는다.
		if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
		{
			return;
		}
		
		//----------------------------------------------------------------------
		// Load되었는지 체크해본다.
		//----------------------------------------------------------------------
		if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
		{
		/*
		// 이미 Load된 경우
		// n번째 creature의 sprite를 메모리 해제
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
			
			// load하지 않았다고 표시한다.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
			
			m_listLoadedCreatureSprite.Remove( spriteType );
			
			if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
			{
				m_listLoadedMonsterSprite.Remove( spriteType );
			}
		}
		else
		{
			// Load되지 않은 경우
		}
		



	}
}

//----------------------------------------------------------------------
// Add MonsterSpriteTypes
//----------------------------------------------------------------------
// Hallu에서 참조하게 될 것이당...
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
// list는 사용하는 creature들의 sprite type 이다.
//----------------------------------------------------------------------
void
MTopView::ReleaseUselessCreatureSPKExcept(const INT_ORDERED_LIST& listUse)
{
	// 지금은 몹+NPC 스프라이트가 얼마 없으므로 release하지 않는다.
	//return;

	//--------------------------------------------------------
	// load된 것들 중에서 사용중인거는 빼고..
	//--------------------------------------------------------
	m_listLoadedCreatureSprite -= listUse;

	//--------------------------------------------------------
	// 나머지는 제거한다.
	//--------------------------------------------------------
	INT_ORDERED_LIST::DATA_LIST::const_iterator iSpriteType = m_listLoadedCreatureSprite.GetIterator();

	for (int i=0; i<m_listLoadedCreatureSprite.GetSize(); i++)
	{
		int spriteType = *iSpriteType;
		
		//--------------------------------------------------------
		// Player그림일경우는 제거하지 않는다.
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
			// 이미 Load된 경우
			// n번째 creature의 sprite를 메모리 해제

			//--------------------------------------------------------
			// Creature SPK 제거
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;

			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{
				m_CreatureSPK.ReleasePart(first, last);
			}

			//--------------------------------------------------------
			// Creature SSPK 제거
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
			
			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{			
				m_CreatureSSPK.ReleasePart( first, last );
			}
			*/
			// loading한 action만 제거
			// 2001.11.21
			int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

			if (frameID!=FRAMEID_NULL)
			{
				ReleaseCreatureSPK( frameID );
			}

			// load하지 않았다고 표시한다.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
		}

		iSpriteType++;
	}

	//--------------------------------------------------------
	// load된 것 다시 설정
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
// 일부의 Tile과 ImageObject를 Load한다.
//----------------------------------------------------------------------
bool		
MTopView::LoadFromFileTileAndImageObjectSet(const CSpriteSetManager &TileSSM, const CSpriteSetManager &ImageObjectSSM)
{
	//--------------------------------------------------------
	//
	// Tile 일부 Load
	//
	//--------------------------------------------------------
//	ifstream	TileSPKFile;//(FILE_SPRITE_TILE, std::ios::binary);	
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
	// ImageObject 일부 Load
	//
	//--------------------------------------------------------
//	ifstream	ImageObjectSPKFile;//(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);	
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
// File에서 LargeZone에서만 사용할 TileSprite을 Load한다.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKLargeZone(ifstream & file)
{
	//------------------------------------------------------------
	// File에서 TileSprite에 대한 정보를 Load한다.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArrayLargeZone;

	m_pTileSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 필요 없는거 제거
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
		
		// oldTileID에서 newTileID 제거한다.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release Part");
		
		// 예전의 zone에만 존재하는 TileID들을 제거한다.
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
	ifstream	TilePackFile;//(FILE_SPRITE_TILE, std::ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;
	
	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArrayLargeZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 주석처리
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
// File에서 LargeZone에서만 사용할 ImageObjectSprite들을 Load한다.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKLargeZone(ifstream & file)
{
	//------------------------------------------------------------
	// File에서 ImageObjectSprite에 대한 정보를 Load한다.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArrayLargeZone;

	m_pImageObjectSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 필요 없는거 제거
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

		// oldImageObjectID에서 newImageObjectID 제거한다.
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

		// 예전의 zone에만 존재하는 ImageObjectID들을 제거한다.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray LargeZone : size=%d", m_pImageObjectSFPArrayLargeZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArrayLargeZone);

	ImageObjectPackFile.close();

	return bLoadOK;
	*/

	/*
	// 2001.8.20 주석처리
	// 새로운 SFPA 생성.
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
// File에서 SmallZone에서만 사용할 TileSprite을 Load한다.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKSmallZone(ifstream & file)
{
	//------------------------------------------------------------
	// File에서 TileSprite에 대한 정보를 Load한다.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArraySmallZone;

	m_pTileSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 필요 없는거 제거
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
		
		// oldTileID에서 newTileID 제거한다.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release");
		
		// 예전의 zone에만 존재하는 TileID들을 제거한다.
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
	ifstream	TilePackFile;//(FILE_SPRITE_TILE, std::ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;

	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArraySmallZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 주석처리
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
// File에서 SmallZone에서만 사용할 ImageObjectSprite들을 Load한다.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKSmallZone(ifstream & file)
{
	//------------------------------------------------------------
	// File에서 ImageObjectSprite에 대한 정보를 Load한다.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArraySmallZone;

	m_pImageObjectSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 필요 없는거 제거
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

		// oldImageObjectID에서 newImageObjectID 제거한다.
		oldImageObjectID -= newImageObjectID;
		
		// 예전의 zone에만 존재하는 ImageObjectID들을 제거한다.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}
	

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray SmallZone : size=%d", m_pImageObjectSFPArraySmallZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, std::ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArraySmallZone);

	ImageObjectPackFile.close();
	
	return bLoadOK;
	*/
	/*
	// 2001.8.20 주석처리
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
// SmallZone에만 존재하는 Tile들을 제거한다.
//----------------------------------------------------------------------
void	
MTopView::ReleaseTileSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseTileSPKSmallZone" );
	
	// m_pTileSFPArraySmallZone와 m_pTileSFPArrayLargeZone를 sort하고
	// SmallZone - LargeZone해서 남은 것을 제거한다.
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

	// SmallZoneTileID에서 LargeZoneTileID를 제거한다.
	SmallZoneTileID -= LargeZoneTileID;
	
	// SmallZone에만 존재하는 TileID들을 제거한다.
	m_TileSPK.ReleasePart( SmallZoneTileID );

	m_pTileSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseTileSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// Release ImageObjectSPK SmallZone
//----------------------------------------------------------------------
// SmallZone에만 존재하는 ImageObject들을 제거한다.
//----------------------------------------------------------------------
void	
MTopView::ReleaseImageObjectSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone" );
	
	// m_pImageObjectSFPArraySmallZone와 m_pImageObjectSFPArrayLargeZone를 sort하고
	// SmallZone - LargeZone해서 남은 것을 제거한다.
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

	// SmallZoneImageObjectID에서 LargeZoneImageObjectID를 제거한다.
	SmallZoneImageObjectID -= LargeZoneImageObjectID;
	
	// SmallZone에만 존재하는 ImageObjectID들을 제거한다.
	m_ImageObjectSPK.ReleasePart( SmallZoneImageObjectID );

	m_pImageObjectSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// 보여줄 Zone을 설정한다.
//----------------------------------------------------------------------
void
MTopView::SetZone(MZone* pZone)
{
	m_pZone		= pZone;
	
	// 모두 다시 그려준다.
	m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	// 좌표 설정
	//----------------------------------------
	// 시야 관련..
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
// 화면 상의 좌표 (x,y)는 Zone에서의 pixel좌표로는 어느 점인가?
//----------------------------------------------------------------------
POINT	
MTopView::ScreenToPixel(int x, int y)
{
	// 현재 화면의 (x,y)의 좌표를 구한다.
	POINT zonePixel;	// 화면상의 처음 Sector가 나타내는 Zone의 pixel좌표

	// 화면의 첫 Sector가 나타내는 Map에서의 Pixel좌표를 구하고
	// 화면좌표 (x,y)를 더해주면 
	// 화면 상의 좌표가 나타내는 Map에서의 Pixel좌표를 구하는 것이다
	
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
// Zone의 pixel좌표를 화면의 좌표로 바꾼다.
//----------------------------------------------------------------------
POINT
MTopView::PixelToScreen(int x, int y)
{
	POINT screenPixel;

	// zone의 pixel좌표에서 
	// 화면의 첫점이 가리키는 zone의 좌표를 빼면 된다.
	screenPixel.x = x - m_FirstZonePixel.x;
	screenPixel.y = y - m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// Map To Screen
//----------------------------------------------------------------------
// Map의 sector좌표를 화면의 좌표로 바꾼다.
//----------------------------------------------------------------------
POINT
MTopView::MapToScreen(int sX, int sY)
{
	POINT screenPixel;

	// Map의 Sector좌표를 Pixel좌표로 바꾼다.
	screenPixel = MapToPixel(sX, sY);

	// Pixel좌표를 Screen좌표로 바꾼다.
	screenPixel.x -= m_FirstZonePixel.x;
	screenPixel.y -= m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// 화면 좌표 (x,y)가 가리키는 위치는 
// Zone에서의 어떤(sX,sY) Sector일까?
//----------------------------------------------------------------------
POINT
MTopView::GetSelectedSector(int x, int y)
{
	POINT point;

	//-------------------------------------------------
	// 준비된 값들
	//-------------------------------------------------
	// 화면 상의 첫 Sector : m_FirstSector
	// 좌표보정값          : m_PlusPoint
	//-------------------------------------------------

	//-------------------------------------------------
	// 방법
	//-------------------------------------------------
	// 1. 화면의 (0,0)이 Zone에서 나타내는 
	//    pixel단위좌표를 알아낸다.(pX,pY) - MapToPixel
	//
	// 2. (pX,pY) + (x,y)를 구한다.
	//
	// 3. (pX,x, pY,y)가 Zone에서 나타내는
	//    Sector좌표를 알아낸다.           - PixelToMap
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
// 다른 Creature를 향해서 바라본다.
//----------------------------------------------------------------------
BYTE
MTopView::GetDirectionToPosition(int originX, int originY, int destX, int destY)
{
	int	stepX = destX - originX,
		stepY = destY - originY;

	// 0일 때 check
	float	k	= (stepX==0)? 0 : (float)(stepY) / stepX;	// 기울기
									

	//--------------------------------------------------
	// 방향을 정해야 한다.	
	//--------------------------------------------------
	if (stepY == 0)
	{
		// X축
		// - -;;
		if (stepX == 0)
			return DIRECTION_DOWN;
		else if (stepX > 0)
			return DIRECTION_RIGHT;
		else 
			return DIRECTION_LEFT;
	}
	else
	if (stepY < 0)	// UP쪽으로
	{
		// y축 위
		if (stepX == 0)
		{
			return DIRECTION_UP;
		}
		// 1사분면
		else if (stepX > 0)
		{
			if (k < -BASIS_DIRECTION_HIGH)
				return DIRECTION_UP;
			else if (k <= -BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTUP;
			else
				return DIRECTION_RIGHT;
		}
		// 2사분면
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
	// 아래쪽
	else
	{		
		// y축 아래
		if (stepX == 0)
		{
			return DIRECTION_DOWN;
		}
		// 4사분면
		else if (stepX > 0)
		{
			if (k > BASIS_DIRECTION_HIGH)
				return DIRECTION_DOWN;
			else if (k >= BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTDOWN;
			else
				return DIRECTION_RIGHT;
		}
		// 3사분면
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
// start부터 end까지 step씩..
// (r,g,b)색깔로 Fade in 시킨다.
// 0(원색)~31(없음)
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
		// 3D 가속인 경우엔 4:4:4이다.
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
// Fade In/Out 처리
//----------------------------------------------------------------------	
void 
MTopView::DrawFade()
{	
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_FADE_SCREEN))
		return;

	//--------------------------------------------------------
	// Fade In/Out 시키는 중이면 출력..
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
//				// 3D 가속인 경우엔 4:4:4이다.
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
		rect.right = SURFACE_WIDTH;
		rect.bottom = SURFACE_HEIGHT;	

//		//--------------------------------------------------------
//		// 3D 가속의 경우
//		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			WORD alpha = (32-m_FadeValue)>>1;			
//			WORD pixel = alpha << 12;
//
//			// m_FadeColor로 화면을 덮는다.
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
			// 검정색이면.. 쉽게 된다~
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
			// 아니면...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(&rect, m_FadeColor, m_FadeValue);
			}

			m_pSurface->Unlock();
		}

		//------------------------------------------------
		// 다음 fade값
		//------------------------------------------------
		// 2004, 6, 21, sobeit add start - 질드레 연출 땜에 추가..
		if(m_delayFrame)
		{
			if(g_CurrentFrame - TempFadeFrame >= m_delayFrame)
			{
				if( m_FadeEnd == -1 && 1 == m_FadeValue ) // 질드레 연출땜에 어두워 진담에 잠시 유지..^^;
				{
					if(g_CurrentFrame - TempFadeFrame> 16*5) // 5초간 유지
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
		// 2004, 6, 21, sobeit add end - 질드레 연출 땜에 추가..
			m_FadeValue += m_FadeInc;

		//------------------------------------------------
		// 끝인가?
		//------------------------------------------------
		// 증가하고 있는 경우
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
// 화면 좌표 (x,y)가 가리키는 위치를 선택하면 
// Zone의 어떤(sX,sY) Object가 선택될까?
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObject(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. 화면좌표 (x,y)의 Zone에서의 Sector(sX,sY)좌표를 구한다.
	// 
	// 2. (sX,sY) 근처에 Object가 있으면 
	//    그 Object의 화면 상의 좌표를 구해서
	//    (x,y)가 속하면 "선택한다"
	//    아니면, 그 다음의 근처 Sector를 check해봐야 한다.
	//
	// (!) Player는 제외다. 없다고 생각하고 check한다.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 근처 Sector를 찾는 순서
	//--------------------------------------------------------------	
	// - 그림은 아래쪽 Sector에 있는 것일수록 늦게 출력되므로
	//   선택될 우선순위가 높다.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X가 선택됐을 경우, Ground,Item 등... check범위
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     Flying이 64 pixel(두 타일) 높이 뜰 경우..
	//      .  .  .     X가 선택됐을 경우, Flying... check범위
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 각 Sector에 대해서 check해봐야 하는 것
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
	// 아직 게임 화면이 안 그려진 경우이면..
	// Player가 다크니스 속에 있다면..
	// 그냥 return
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

	POINT firstZonePixel;	// 화면상의 처음 Sector가 나타내는 Zone의 pixel좌표
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)의 Zone에서의 pixel좌표를 구한다.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)의 Zone에서의 sector좌표를 구한다.
	//--------------------------------------------------------------						
	sectorPoint = PixelToMap(pixelPoint.x, pixelPoint.y);

	g_MouseSector = sectorPoint;

	//--------------------------------------------------------------
	// 포탈 체크
	//--------------------------------------------------------------
	// 여기서 하는건 웃기지만.. -_-;;
	// 처리의 편의를 위해서.. 임시로... 할할... ㅡ.ㅡ;;
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
				// 이동 가능한 zone의 ID
				PORTAL_INFO portalInfo = *iPortal;			

				//-------------------------------------------------------
				// 임시 출력
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
				// portal의 종류에 따라 갈수 있는곳인지 체크
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
				// 갈 수 있으면 포탈 커서 출력
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
								// 필살 하드코딩~~
								zoneID = 60001;
								//strcpy(pZoneName, "엘리베이터");
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
						//strcpy(pZoneName, "갈 수 없는 곳");
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

	// Object의 영역
//	POINT	objectPixelPoint;
	//RECT	rect;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//ofstream file("log.txt");


	// 현재 check하는 좌표
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : 같은 높이에 있는 경우 
	//   일반적인 Object보다 아래쪽 Sector에 존재할 수 있다.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 3,		// 한 줄씩 더 증가시켰다. 음냐리~
			sX2 = sectorPoint.x + 3;

	// 박쥐 높이 때문에.. +를 더했다.
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 8;

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
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
	// 있을법한 Sector를 check한다.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{			
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("ckF1(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 어떤 Object가 있을 경우
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object의 종류에 따라서 달리 check한다.
				// 선택할 수 있는 것은 Creature와 Item뿐이다.

				//------------------------------------------------
				//
				// Flying Creature가 있는 경우
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
				
						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// FlyingCreature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// FlyingCreature의 Zone의 좌표를 계산한다.
							/*
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack에서 높이를 설정하므로,
												// Flying인 경우도 똑같다.

							// Creature가 화면에 존재하는 영역을 계산해낸다.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/

							// 다크니스에 있으면 
							// 선택한 위치가 Creature의 영역에 속하면
							// 공격 mode인 경우는 공격가능한 캐릭터만 선택
							// 아니면 아무나.
							if (
								(g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 사라지는 상태면
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
	sY2 = sectorPoint.y + 4;		// 한 줄씩 더 증가시켰다.

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
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
	// Check대상이 되는 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// 출력 시점이 sY1보다 적은 경우의 
	// ImageObject들을 Check한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject인 경우만 check한다.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// 출력 시점이 sY1보다 적은 경우..는 Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				/*
//				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame에 관계된 sprite를 구한다.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//
//				// 초기화 되지 않은 경우면..
//				if (m_ImageObjectSPK[ sprite ].IsNotInit())
//				{
//					iImageObjectCheck ++;
//					continue;
//				}
//						
//				// 좌표 보정
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//
//				// 화면에 존재하는 영역을 계산해낸다.
//				rect.left	= objectPixelPoint.x;
//				rect.top	= objectPixelPoint.y;
//				rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//				rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//				*/		
//				// 선택한 위치가 ImageObject의 영역에 속하면
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
//			// 아니면.. Object check할때 Check해야되는 경우이다.
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
	// 있을법한 Sector를 check한다.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("gS(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 어떤 Object가 있을 경우
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object의 종류에 따라서 달리 check한다.
				// 선택할 수 있는 것은 Creature와 Item뿐이다.

				//------------------------------------------------
				//
				// Flying Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// FlyingCreature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// FlyingCreature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// FlyingCreature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack에서 높이를 설정하므로,
												// Flying인 경우도 똑같다.

							// 화면에 존재하는 영역을 계산해낸다.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/
							
							// 선택한 위치가 Creature의 영역에 속하면
							if ((
								g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001|| 
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 사라지는 상태면
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
				// Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// Creature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// Creature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// Creature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// 화면에 존재하는 영역을 계산해낸다.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/			
							// 선택한 위치가 Creature의 영역에 속하면
							if ((g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 사라지는 상태면
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
				// Effect가 있는 경우
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
				// UndergroundCreature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{							
							// 선택한 위치가 Creature의 영역에 속하면
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
				// Item이 있는 경우
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

					// Item가 차지하는 영역을 구한다.
					// FRAME_PACK에서 정보를 읽어야 할 듯!!
					// Item의 Sector좌표를 Pixel좌표로 바꿔야 한다.
					/*
					// Item의 Zone의 좌표를 계산한다.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// Zone의 좌표를 화면의 좌표로 바꾼다.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item가 화면에 존재하는 영역을 계산해낸다.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// 현재 Sprite가 화면에 출력되는 위치
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// 화면에 존재하는 영역을 계산해낸다.
					rect.left	= objectPixelPoint.x;
					rect.top	= objectPixelPoint.y;
					rect.right	= rect.left + m_ItemTileSPK[ frame.GetSpriteID() ].GetWidth();
					rect.bottom	= rect.top + m_ItemTileSPK[ frame.GetSpriteID() ].GetHeight();
					*/			

					//------------------------------------------------
					// 뱀파이어거나
					// 슬레이어인 경우는 볼 수 있는 상황이라면.. 
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
						// 시체인 경우는 
						// 시체 Creature의 영역을 봐야 한다.
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
							// 선택한 위치가 Item의 영역에 속하면
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
		// 출력 시점이 currentY보다 적은 경우의 
		// ImageObject들을 Check한다.
		// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
		//------------------------------------------------------
//		while (iImageObjectCheck != m_mapImageObject.rend())
//		{
//			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
			// InteractionObject인 경우만 check한다.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// 출력 시점이 sY1보다 적은 경우..는 Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					/*
//					// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame에 관계된 sprite를 구한다.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//
//					// 초기화 되지 않은 경우면..
//					if (m_ImageObjectSPK[ sprite ].IsNotInit())
//					{
//						iImageObjectCheck ++;
//						continue;
//					}
//							
//					// 좌표 보정
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//
//					// 화면에 존재하는 영역을 계산해낸다.
//					rect.left	= objectPixelPoint.x;
//					rect.top	= objectPixelPoint.y;
//					rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//					rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//					*/			
//					// 선택한 위치가 ImageObject의 영역에 속하면
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
//				// 아니면.. Object check할때 Check해야되는 경우이다.
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
	// 출력 시점이 currentY보다 적은 경우의 
	// ImageObject들을 Check한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
//	while (iImageObjectCheck != m_mapImageObject.rend())
//	{
//		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
		// InteractionObject인 경우만 check한다.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//			/*
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame에 관계된 sprite를 구한다.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//
//			// 초기화 되지 않은 경우면..
//			if (m_ImageObjectSPK[ sprite ].IsNotInit())
//			{
//				iImageObjectCheck ++;
//				continue;
//			}
//							
//			// 좌표 보정
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//
//			// 화면에 존재하는 영역을 계산해낸다.
//			rect.left	= objectPixelPoint.x;
//			rect.top	= objectPixelPoint.y;
//			rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//			rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//			*/				
//			// 선택한 위치가 ImageObject의 영역에 속하면
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
	// 아무것도 없다면.. GroundEffect도 체크한다.
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
		// 아무것도 없다면.. FakeCreature(Pet)도 체크한다.
		//------------------------------------------------------
		int fakeCreatureNum = g_pZone->GetFakeCreatureNumber();
		MZone::CREATURE_MAP::const_iterator iFakeCreature = g_pZone->GetFakeCreatureBegin();
		for(int fc = 0; fc < fakeCreatureNum; fc++)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)iFakeCreature->second;

			// 펫인경우만 검색
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
// 화면 좌표 (x,y)가 가리키는 위치를 선택하면 
// Zone의 어떤(sX,sY) Object가 선택될까?
//
// Sprite에서 pixel정보를 읽어서 비교한다.
//
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObjectSprite(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. 화면좌표 (x,y)의 Zone에서의 Sector(sX,sY)좌표를 구한다.
	// 
	// 2. (sX,sY) 근처에 Object가 있으면 
	//    그 Object의 화면 상의 좌표를 구해서
	//    (x,y)가 속하면 "선택한다"
	//    아니면, 그 다음의 근처 Sector를 check해봐야 한다.
	//
	// (!) Player는 제외다. 없다고 생각하고 check한다.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 근처 Sector를 찾는 순서
	//--------------------------------------------------------------	
	// - 그림은 아래쪽 Sector에 있는 것일수록 늦게 출력되므로
	//   선택될 우선순위가 높다.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X가 선택됐을 경우, Ground,Item 등... check범위
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     Flying이 64 pixel(두 타일) 높이 뜰 경우..
	//      .  .  .     X가 선택됐을 경우, Flying... check범위
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 각 Sector에 대해서 check해봐야 하는 것
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

	POINT firstZonePixel;	// 화면상의 처음 Sector가 나타내는 Zone의 pixel좌표
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)의 Zone에서의 pixel좌표를 구한다.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)의 Zone에서의 sector좌표를 구한다.
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

	// Object의 영역
	POINT	objectPixelPoint;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//ofstream file("log.txt");


	// 현재 check하는 좌표
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : 같은 높이에 있는 경우 
	//   일반적인 Object보다 아래쪽 Sector에 존재할 수 있다.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 1,
			sX2 = sectorPoint.x + 1;
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 5;

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
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
	// 있을법한 Sector를 check한다.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 어떤 Object가 있을 경우
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object의 종류에 따라서 달리 check한다.
				// 선택할 수 있는 것은 Creature와 Item뿐이다.

				//------------------------------------------------
				//
				// Flying Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// FlyingCreature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// FlyingCreature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack에서 높이를 설정하므로,
												// Flying인 경우도 똑같다.

							// mouse좌표가 Sprite에 속하는지 알아보기
							// Sprite에서의 좌표가 색깔이 있는 경우일때
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
	// Zone의 영역이 아닌 경우에 Skip...
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
	// Check대상이 되는 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// 출력 시점이 sY1보다 적은 경우의 
	// ImageObject들을 Check한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject인 경우만 check한다.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// 출력 시점이 sY1보다 적은 경우..는 Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame에 관계된 sprite를 구한다.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//						
//				// 좌표 보정
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//									
//				// InteractionObject는 (point.x, point.y)에 출력되어 있을 것이다.
//				// mouse좌표가 Sprite에 속하는지 알아보기
//				// Sprite에서의 좌표가 색깔이 있는 경우일때
//				if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//						x - objectPixelPoint.x, 
//						y - objectPixelPoint.y 
//					))
//				{	
//					pInteractionObject->SetAction( 1 );
//					return pInteractionObject;
//				}
//			}
//			// 아니면.. Object check할때 Check해야되는 경우이다.
//			else
//			{
//				break;
//			}
//		}

		iImageObjectCheck ++;
	}

	//------------------------------------------------------	
	// 있을법한 Sector를 check한다.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 어떤 Object가 있을 경우
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object의 종류에 따라서 달리 check한다.
				// 선택할 수 있는 것은 Creature와 Item뿐이다.

				//------------------------------------------------
				//
				// Flying Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{

							// FlyingCreature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// FlyingCreature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// FlyingCreature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack에서 높이를 설정하므로,
												// Flying인 경우도 똑같다.

							// mouse좌표가 Sprite에 속하는지 알아보기
							// Sprite에서의 좌표가 색깔이 있는 경우일때
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
				// Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{

							// Creature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// Creature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// Creature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// mouse좌표가 Sprite에 속하는지 알아보기
							// Sprite에서의 좌표가 색깔이 있는 경우일때
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
				// Underground Creature가 있는 경우
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

						// Player이면 선택하지 않는다.
						if (pCreature->GetID()!=pid)	
						{

							// Creature가 차지하는 영역을 구한다.
							// FRAME_PACK에서 정보를 읽어야 할 듯!!
							// Creature의 Sector좌표를 Pixel좌표로 바꿔야 한다.

							// Creature의 Zone의 좌표를 계산한다.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone의 좌표를 화면의 좌표로 바꾼다.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature가 화면에 존재하는 영역을 계산해낸다.	
							// 좌표 보정
							//frame = m_CreatureFPK[pCreature->GetCreatureFrameID()][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 현재 Sprite가 화면에 출력되는 위치
							//objectPixelPoint.x += frame.GetCX();
							//objectPixelPoint.y += frame.GetCY();

							// mouse좌표가 Sprite에 속하는지 알아보기
							// Sprite에서의 좌표가 색깔이 있는 경우일때
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
				// Item이 있는 경우
				//
				//------------------------------------------------				
				pObject = (MObject*)sector.GetItem();

				if (pObject != NULL)
				{	
					//file << "Exist Item!" << endl;

					pItem = (MItem*)pObject;

					// Item가 차지하는 영역을 구한다.
					// FRAME_PACK에서 정보를 읽어야 할 듯!!
					// Item의 Sector좌표를 Pixel좌표로 바꿔야 한다.

					// Item의 Zone의 좌표를 계산한다.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// Zone의 좌표를 화면의 좌표로 바꾼다.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item가 화면에 존재하는 영역을 계산해낸다.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// 현재 Sprite가 화면에 출력되는 위치
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// mouse좌표가 Sprite에 속하는지 알아보기
					// Sprite에서의 좌표가 색깔이 있는 경우일때
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
		// 출력 시점이 currentY보다 적은 경우의 
		// ImageObject들을 Check한다.
		// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
		//------------------------------------------------------
		while (iImageObjectCheck != m_mapImageObject.rend())
		{
			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

			// InteractionObject인 경우만 check한다.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// 출력 시점이 sY1보다 적은 경우..는 Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame에 관계된 sprite를 구한다.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//							
//					// 좌표 보정
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//										
//					// InteractionObject는 (point.x, point.y)에 출력되어 있을 것이다.
//					// mouse좌표가 Sprite에 속하는지 알아보기
//					// Sprite에서의 좌표가 색깔이 있는 경우일때
//					if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//							x - objectPixelPoint.x, 
//							y - objectPixelPoint.y 
//						))
//					{							
//						return pInteractionObject;
//					}
//				}
//				// 아니면.. Object check할때 Check해야되는 경우이다.
//				else
//				{
//					break;
//				}
//			}

			iImageObjectCheck ++;
		}
	}	

	//------------------------------------------------------
	// 출력 시점이 currentY보다 적은 경우의 
	// ImageObject들을 Check한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject인 경우만 check한다.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// 그림의 좌표를 현재 화면의 좌표에 맞추기								
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame에 관계된 sprite를 구한다.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//							
//			// 좌표 보정
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//									
//			// InteractionObject는 (point.x, point.y)에 출력되어 있을 것이다.
//			// mouse좌표가 Sprite에 속하는지 알아보기
//			// Sprite에서의 좌표가 색깔이 있는 경우일때
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
// 다 지워줘야 한다.
//----------------------------------------------------------------------
void			
MTopView::ClearItemNameList()
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	// 모든 node를 지운다.
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		// 메모리에서 node제거
		delete pNode;
		
		iItemName ++;
	}

	m_listDrawItemName.clear();

}

//----------------------------------------------------------------------
// Add ItemName
//----------------------------------------------------------------------
// 외부에서 new를 해줘야 한다.
//
// pNode->GetDistance로 sort해서 추가해야 한다.
// 오름차순..
//----------------------------------------------------------------------
// item이 많은 경우는 뒤에서부터 체크하는게 빠르다.
// MAX값이 정해져 있기 때문에...
// 그러나.. 고치기 귀찮군.. --;
// 이거 치는 시간에 고쳤겠다.. 음냐..
//----------------------------------------------------------------------
void		 	
MTopView::AddItemName(DRAWITEMNAME_NODE* pNode)
{		
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	BOOL bInsertEnd = TRUE;

	//-------------------------------------------------------
	// 거리를 비교하면서 ItemName을 추가한다.
	//-------------------------------------------------------
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pOldNode = *iItemName;

		//------------------------------------------
		// 비교 비교~
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
		// 맨 끝에 추가
		m_listDrawItemName.push_back( pNode );
	}
	
	//-------------------------------------------------------
	// MAX를 넘어간 경우
	//-------------------------------------------------------
	if (m_listDrawItemName.size() > g_pClientConfig->MAX_DRAWITEMNAME)
	{
		// 마지막 걸 제거한다.
		DRAWITEMNAME_NODE* pDeleteNode = m_listDrawItemName.back();

		delete pDeleteNode;

		m_listDrawItemName.pop_back();
	}
}

//----------------------------------------------------------------------
// DrawItemNameList
//----------------------------------------------------------------------
// 각각의 Item마다 AddText를 한다.
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

	// minimap을 그리는 경우.. 
	// 좌표 땜시...
	// 아래쪽부터 그린다..

	// 2001.7.14 주석처리
	//if (g_pUserOption->DrawMinimap)// && g_pPlayer->IsSlayer())
	{
		// 우헤헤 하드 코딩~~
		y = 160;
	}

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		MItem* pItem = pNode->GetItem();
	
		
		const char* pItemName = pItem->GetName();

		// option이 붙은거는 파란색..
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
		else if(pItem->GetItemOptionListCount() > 1)
		{
			nameColor = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
		}
		else
		{
			nameColor = m_ColorNameItemOption;
		}

		if (pItemName != NULL)
		{
			// 화면 안에 글자가 들어오도록..
			x = CLIPSURFACE_WIDTH - g_GetStringWidth( pItemName, g_ClientPrintInfo[FONTID_ITEM]->hfont );
			x2 = CLIPSURFACE_WIDTH;
			
			y2 = y + g_pClientConfig->FONT_ITEM_HEIGHT;

			// 검은색 박스 영역
			RECT rect = {
				x, 
				y,
				x2,
				y2
			};

			// mouse로 선택된 Item인 경우
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

			// item Name의 선택 영역
			pNode->SetRect( rect );
		}

		// 다음..
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
// 화면 좌표(x,y)를 선택한 경우
// 그 좌표에 ItemName이 있는가?
//----------------------------------------------------------------------
MItem*
MTopView::SelectItemName(int x, int y)
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		//--------------------------------------------
		// ItemName의 선택영역에 속하면?
		//--------------------------------------------
		if (pNode->IsPointInRect(x,y))
		{			
			// item pointer를 넘겨준다.
			// zone에 없는 경우도 있을 수 있으므로...
			// zone에서 읽어와야 한다.
			return m_pZone->GetItem( pNode->GetID() );		
		}
	
		// 다음 item name
		iItemName ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Clear TextList
//----------------------------------------------------------------------
// 다 지워줘야 한다.
//----------------------------------------------------------------------
void			
MTopView::ClearTextList()
{
	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		// 메모리에서 node제거
		delete pNode;
		
		m_pqDrawText.pop();
	}
}

//----------------------------------------------------------------------
// Add Text
//----------------------------------------------------------------------
// DrawTextList에 출력할 string의 정보를 추가한다.
// 외부에서 new를 해줘야 한다.
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

	// 나중에 찍히게 하기
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
// Unlock상태에서 출력한다고 가정한다.
//----------------------------------------------------------------------
void			
MTopView::DrawTextList()
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawTextList" );
	#endif
	
	//DRAWTEXT_PQ::iterator iText = m_pqDrawText.begin();

	//--------------------------------------------------------
	// 3D 가속 사용이면...
	// 일단 샥~~ 어둡게.. --;
	// Lock/Unlock을 덜 쓰기 위해서..
	//
	// 그런데! 
	// 글자 찍을 때, 바로 직전에 box를 그려줘야 한다. 켁...
	//--------------------------------------------------------
	/*
	if (CDirect3D::IsHAL())
	{
		m_pSurface->Unlock();

		// 모든 node를 지운다.
		while (iText != m_pqDrawText.end())
		{
			DRAWTEXT_NODE* pNode = *iText;

			//--------------------------------------------------------
			// 글자출력되는 부분에 검은색 상자 출력
			//--------------------------------------------------------
			RECT rect = { pNode->GetX(), pNode->GetY(), 
						pNode->GetXPlusWidth(), pNode->GetY(),15 };
			
			// 검은 박스 출력..
			DrawBox3D(&rect, 0x7000);			

			iText++;
		}

		m_pSurface->Lock();	
		
		// 다시 처음으로..
		iText = m_pqDrawText.begin();
	}
	*/

	//m_pSurface->Unlock();
	//--------------------------------------------------------
	// 모든 node를 출력하고 지운다.
	//--------------------------------------------------------
	RECT rect2;

	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		//--------------------------------------------------------
		// Box를 출력할까?
		//--------------------------------------------------------
		if (pNode->IsExistBox())
		{
			//--------------------------------------------------------
			// 글자출력되는 부분에 검은색 상자 출력
			//--------------------------------------------------------
			RECT rect = pNode->GetBox();
			
			//--------------------------------------------------------
			// 투명한 박스인 경우
			//--------------------------------------------------------
			if (pNode->IsTransBox())
			{
				//--------------------------------------------------------
				// 3D 가속의 경우
				//--------------------------------------------------------
//				if (CDirect3D::IsHAL())
//				{
//					// Unlock / lock... 으악~~ 이거 시간 많이 잡아먹을건데..
//					//m_pSurface->Unlock();
//
//					// 박스 출력..
//					#ifdef OUTPUT_DEBUG_DRAW_3DBOX
//						DEBUG_ADD_FORMAT("TextList-Box3D( %d, %d, %d, %d ), color=%x",
//														rect.left, rect.top, rect.right, rect.bottom, pNode->GetBoxColor());
//					#endif
//
//					// 바닥 깔기
//					WORD boxColor = pNode->GetBoxColor();
//
//					rect2 = rect;
//					DrawBox3D(&rect2, boxColor);
//
//					if (pNode->IsExistBoxOutline())
//					{
//						// 외곽 박스 그리기
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
//						// 선
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

						// 외곽 박스 그리기
						rect2 = rect;
						DRAW_CHAT_BOX_OUTLINE( rect2, color )

						// Lock
						m_pSurface->Lock();

						WORD boxColor = pNode->GetBoxColor();

						// 바닥 그리기
						rect2 = rect;
						DRAW_ALPHA_BOX_2D( rect2, boxColor );
						
						// 검정색인 경우
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
						// 다른 색인 경우
						else
						{
							m_pSurface->BltColorAlpha(&rect, boxColor, 15);
						}

						// 선
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
			// 투명한 박스가 아닌 경우에
			//--------------------------------------------------------
			else
			{
				m_pSurface->FillRect(&rect, pNode->GetBoxColor());				
			}
		}

		//--------------------------------------------------------
		// 출력할 글자가 있는 경우..
		//--------------------------------------------------------
		const char* pString = pNode->GetString();

		if (pString!=NULL && pString[0]!='\0')
		{
			//--------------------------------------------------------
			// 출력 좌표
			//--------------------------------------------------------
			int x = pNode->GetX();
			int y = pNode->GetY();

			//--------------------------------------------------------
			// font 설정
			//--------------------------------------------------------	
			PrintInfo* pPrintInfo = g_ClientPrintInfo[pNode->GetFont()];				

			//--------------------------------------------------------
			// flag에 따른 처리도 해줘야 한다....				
			//--------------------------------------------------------
			if (pNode->IsOutline())
			{
				// 역상 color설정
				pPrintInfo->text_color = (~pNode->GetColor()) & 0x00FFFFFF;
					
				// 출력
				g_Print(x-1, y-1, pString, pPrintInfo);
				g_Print(x+1, y+1, pString, pPrintInfo);
			}

			// text color설정
			pPrintInfo->text_color = pNode->GetColor();
				
			//--------------------------------------------------------
			// string 출력
			//--------------------------------------------------------
			g_Print(x, y, pString, pPrintInfo);
		}

		// 메모리에서 제거한다.
		delete pNode;
		
		// 다음..
		m_pqDrawText.pop();
	}

	// 모든 node를 제거한다.
	//m_pqDrawText.clear();

	// Lock상태
	//m_pSurface->Lock();

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawTextList" );
	#endif
}

//----------------------------------------------------------------------
// Clear LightBufferFilter
//----------------------------------------------------------------------
// LightBuffer를 초기화한다.
// 기본적으로 Player의 시야를 표현해줘도 된다.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter3D()
{
	//if (CDirect3D::IsHAL() && 
	//if	(m_DarkBits)
	{
		//--------------------------------------------------
		// 빛의 값 설정
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
		// 초기화 - 화면 전체를 채울 빛..
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
// LightBuffer를 초기화한다.
// 기본적으로 Player의 시야를 표현해줘도 된다.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter2D()
{
	//if (1)//CDirect3D::IsHAL() && m_b3DLight && m_DarkBits)
	{
		//--------------------------------------------------
		// 빛의 값 설정
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
		// 초기화 - 화면 전체를 채울 빛..
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
// LightFilter를 추가한다.
// Screen좌표를 LightBuffer에 맞도록 바꿔야한다.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)로..
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
		|| bForceLight) // 무조건 출력해야하는 빛
	{
		// tile 좌표 보정 
		 //x += TILE_X_HALF;
		 //y += TILE_Y_HALF;

		// Light Filter의 범위를 넘지 않도록 한다.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range에 따라서 출력할 lightFilter를 결정해야한다.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter 출력 시점 결정
		/// 그리고, filter좌표 (fx,fy)로 바꿔야 하고..
		// filter의 중심을 (x,y)에 오도록 맞춰야 한다.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)가 어떤 좌표인지에 따라서 계산을 다르게 해준다.
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

		// filter 중심 맞추기
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter에 Light filter를 추가한다.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterAdd( fx, fy, m_LightFTP[filterID] );		

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add LightFilter To Screen
//----------------------------------------------------------------------
// LightFilter를 추가한다.
// Screen좌표를 LightBuffer에 맞도록 바꿔야한다.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)로..
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
		|| bForceLight)	// 무조건 출력해야하는 빛
	{
		// tile 좌표 보정 
		//x += TILE_X_HALF;
		//y += TILE_Y_HALF;

		// Light Filter의 범위를 넘지 않도록 한다.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range에 따라서 출력할 lightFilter를 결정해야한다.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter 출력 시점 결정
		/// 그리고, filter좌표 (fx,fy)로 바꿔야 하고..
		// filter의 중심을 (x,y)에 오도록 맞춰야 한다.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)가 어떤 좌표인지에 따라서 계산을 다르게 해준다.
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

		// filter 중심 맞추기
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter에 Light filter를 추가한다.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterSub( fx, fy, m_LightFTP[filterID] );

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add CreatureOutput All
//----------------------------------------------------------------------
// m_pZone의 Creature map을 통째로~ 추가
//----------------------------------------------------------------------
// OutputMap에서의 ID는 Creature의..
//
//    2 bytes + 2 bytes    = 4 bytes 로 구성되어 있다.
// [pixel y좌표][object id]
//
// 일반 creature는 추가할지 말지를 숫자로 결정하고..
// FakeCreature는 tile에는 추가안되므로.. 무조건 여기서 추가해야한다.
//----------------------------------------------------------------------
void	
MTopView::AddOutputCreatureAll()
{
	// 초기화
	m_mapCreature.clear();

	// [새기술3] 관 속에 있을때는 자기만 출력하게 한다.
	if (g_pPlayer->IsInCasket())
	{
		m_bTileSearchForCreature = false;	

		QWORD key = GetOutputCreatureID(g_pPlayer);

		// (key값, pCreature)를 추가한다.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, g_pPlayer )
		);

		return;
	}


	MZone::CREATURE_MAP::const_iterator iCreature;

	MCreature* pCreature;

	int num = m_pZone->GetCreatureNumber();

	// 20명 이상 있으면 ... 
	// 그냥 검색하는게 낫다고 판단.. 정말일까? - -;
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
	// Tile을 검색해서 출력하는 경우가 아니면..
	// 모든 Creature를 OutputMap에 추가해야 한다.
	//---------------------------------------------------------
	if (!m_bTileSearchForCreature)
	{
		iCreature = m_pZone->GetCreatureBegin();

		for (int i=0; i<num; i++)
		{
			pCreature = iCreature->second;

			// key 값 = [pixel Y좌표][object id]
			QWORD key = GetOutputCreatureID(pCreature);

			// (key값, pCreature)를 추가한다.
			m_mapCreature.insert(
				CREATURE_OUTPUT_MAP::value_type( key, pCreature )
			);
		
			// 다음 Creature
			iCreature++;
		}
	}

	//---------------------------------------------------------
	// Fake Creature
	//---------------------------------------------------------
	iCreature = m_pZone->GetFakeCreatureBegin();
	num = m_pZone->GetFakeCreatureNumber();
	
	//---------------------------------------------------------
	// 모든 Creature를 OutputMap에 추가해야 한다.
	//---------------------------------------------------------
	for (int i=0; i<num; i++)
	{
		pCreature = iCreature->second;

		// key 값 = [pixel Y좌표][object id]
		QWORD key = GetOutputCreatureID(pCreature);

		// (key값, pCreature)를 추가한다.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, pCreature )
		);
	
		// 다음 Creature
		iCreature++;
	}
}

//----------------------------------------------------------------------
// Add OutputCreature
//----------------------------------------------------------------------
// pCreature를 추가한다.
//----------------------------------------------------------------------
bool	
MTopView::AddOutputCreature(MCreature* pCreature)
{	
	// key값 생성
	QWORD key = GetOutputCreatureID(pCreature);

	// 이미 있는지 확인해보고 없으면 추가.
	if (m_mapCreature.find( key ) == m_mapCreature.end())
	{			
		// (key값, pCreature)를 추가한다.
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
// id와 관련있는 creature를 제거한다.
//----------------------------------------------------------------------
bool	
MTopView::RemoveOutputCreature(MCreature* pCreature)
{
	// 이미 있는지 확인해보고 있으면 제거
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
// ID가 id인 Creature의 좌표가 y0에서 y1으로 바뀐걸 update한다.
//----------------------------------------------------------------------
// id와 관련있는 creature의 정보(출력좌표관련id)를 Update한다.
// 있으면.. 제거하고.. 다시 추가해야 한다.
//----------------------------------------------------------------------
bool			
MTopView::UpdateOutputCreature(TYPE_OBJECTID id, int y0, int y1)
{
	// 이미 있는지 확인해보고 있으면 update
	CREATURE_OUTPUT_MAP::iterator iCreature 
		= m_mapCreature.find( GetOutputCreatureID(id, y0) );

	if (iCreature != m_mapCreature.end())
	{			
		MCreature* pCreature = (*iCreature).second;

		// 제거
		m_mapCreature.erase( iCreature );

		// (key값, pCreature)를 다시 추가한다.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( 
					GetOutputCreatureID(id, y1),	// key값
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
// 검은색 반투명 박스를 그린다. 3D
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
//	// 다른 색깔일 경우.. 다시 설정..
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
// 2D/3D가속에 관계없이...
// 현재의 Lock/Unlock 상태에 관계없이... 작동한다.
//
// m_pSurface에 
// (r,g,b)와 alpha값(1~31)에 따라서 pRect영역에 투명한 box를 그린다.
//----------------------------------------------------------------------
void
MTopView::DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	WORD color;

	//--------------------------------------------------------
	// Win2000인 경우.. 여기서 엄청난 속도저하가 생긴다.. 왜 그럴까?
	// 일단은.. AlphaBox를 사용하지 않는다.
	// 테스트 결과.. 이게 문제가 아니었다... 뭐가 문제일까. - -;
	//--------------------------------------------------------
	/*
	if (g_WindowsNT)
	{
		//------------------------------------------------
		// Unlock 상태로 만든다.
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (bLock)
		{
			m_pSurface->Unlock();
		}
		
		color = CDirectDraw::Color(r,g,b);

		m_pSurface->FillRect(pRect, color);

		//------------------------------------------------
		// 원래 lock 된 상태면 다시 되돌린다.
		//------------------------------------------------
		if (bLock)
		{
			m_pSurface->Lock();
		}
	}
	*/
	//--------------------------------------------------------
	// 98인 경우..
	//--------------------------------------------------------
	//else
	{
		//--------------------------------------------------------
		// 3D 가속의 경우
		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			//------------------------------------------------
//			// Unlock 상태로 만든다.
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
//			// 다른 색깔일 경우.. 다시 설정..
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
//			// 원래 lock 된 상태면 다시 되돌린다.
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
			// Lock 상태로 만든다.
			//------------------------------------------------
			BOOL bUnlock = !m_pSurface->IsLock();
			if (bUnlock)
			{
				m_pSurface->Lock();
			}

			color = CDirectDraw::Color(r,g,b);

			//-------------------------------------------------
			// 검정색이면.. 쉽게 된다~
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
			// 아니면...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(pRect, color, alpha);
			}

			//------------------------------------------------
			// 원래의 Lock 상태로 되돌린다.
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
// Debug에 필요한 정보를 출력한다.
//
// 글자 출력할때는 Unlock상태..
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
	// Surface의 정보를 저장해둔다.
	//S_SURFACEINFO		SurfaceInfo;
	//SetSurfaceInfo(&SurfaceInfo, m_pSurface->GetDDSD());

	//-----------------------------------------------------------------
	// Font 선택
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
	// Zone이름 출력
	//-----------------------------------------------------------------
	/* // 2001.7.14 주석처리
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
	// 게임 시간 출력
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
		// 30초에 한번씩은 시간을 바꿔준다.
		if (g_CurrentTime - lastDisplayGameTime > 30000)
		{
			g_pGameTime->SetCurrentTime( g_CurrentTime );

			lastDisplayGameTime = g_CurrentTime;
		}
	}
	*/

	//-----------------------------------------------------------------
	//
	// Item 부서진거 출력
	//
	//-----------------------------------------------------------------
	y += 10;
	DrawItemBroken(5, y);

	y += 70;	// ItemBroken의 크기.. 귀찮아서 기냥.. ㅋㅋ

	//-----------------------------------------------------------------
	//
	// Event Message 출력
	//
	//-----------------------------------------------------------------
	int strY = 30;
	int strX = 400;

	// Event String 출력
	DrawEventString( strX, strY );

	//-----------------------------------------------------------------
	//
	// System Message 출력
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

	// 5초마다 한번씩.. scroll
	static DWORD lastTime = g_CurrentTime;
	if (g_CurrentTime - lastTime >= g_pClientConfig->DELAY_SYSTEMMESSAGE)
	{
		g_pSystemMessage->Add("\0");		
		lastTime = g_CurrentTime;
	}

	// 7초마다 한번씩.. scroll
	static DWORD lastNoticeTime = g_CurrentTime;
	if (g_CurrentTime - lastNoticeTime >= (g_pClientConfig->DELAY_SYSTEMMESSAGE + 2000) )
	{
		g_pNoticeMessage->Add("\0");
		lastNoticeTime = g_CurrentTime;
	}

	//-----------------------------------------------------------------
	//
	// Game Message 출력
	//
	//-----------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
		strX = 10;
		strY = 410;
		break;

	case RACE_VAMPIRE:
		strX = 10;
		strY = 440;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point 올릴려고 누르는 버튼이 있는 위치
			strX = 85;
		}
		break;

	case RACE_OUSTERS:
		strX = 10;
		strY = 410;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point 올릴려고 누르는 버튼이 있는 위치
			strX = 85;
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

	// 5초마다 한번씩.. scroll
	static DWORD gamelastTime = g_CurrentTime;
	if (g_CurrentTime - gamelastTime >= g_pClientConfig->DELAY_GAMEMESSAGE)
	{
		g_pGameMessage->Add("\0");
		gamelastTime = g_CurrentTime;
	}

	//-----------------------------------------------------------------
	//
	// 총알 남은거 출력
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
	// 캐릭터 상태 출력
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
	

	// 퀘스트 부분은 퀘스트 인터페이스로 뺀다. 아래부분은 사용되지 않을듯..
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
						// 아이템 모으기
						sprintf(str,"지금은 아이템 모으는중 ㅋㅋㅋ %d/%d %d",QuestEvent->parameter3, mkq->GetGoal(), QuestEvent->parameter4 - (timeGetTime() / 1000));
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
// Debug에 필요한 정보를 출력한다.
//----------------------------------------------------------------------
void
MTopView::DrawDebugInfo(CDirectDrawSurface* pSurface)
{ 
	#ifdef	OUTPUT_DEBUG		
		//----------------------------------------------------------------
		// debug용 code
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

		sprintf(str, "ID = %d / %d명 [Weapon=%s] [align=%d]", g_pPlayer->GetID(), m_pZone->GetCreatureNumber(), attackMode, g_pPlayer->GetAlignment());	
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

		// Timer 출력
		//sprintf(str, "Timer = %ld", g_CurrentTime);	
		//g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,20, str, 0xFFFFFF);
		

		// Mouse좌표 찍기	
		//sprintf(str, "%d, %d", g_x, g_y);	
		//g_pBack->GDI_Text(11,101, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,100, str, RGB(255,255,255));

		//sprintf(str, "ImageObject=%d개", m_mapImageObject.size());
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

		// 남은 Texture메모리 test
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
// ClientTest용 Help
//----------------------------------------------------------------------
void
MTopView::DrawTestHelp()
{
	#ifdef OUTPUT_DEBUG
		//--------------------------------------------------------------------
		//
		// F1을 누른 상태 --> 도움말 출력
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
				"         ALPHA-TEST CLIENT 도움말",
				"---------------------------------------------------------------------",
				"  [ESC] Menu",
				"  [F1~F8] 벨트 아이템 사용",
				"  [Scroll Lock] 화면 캡쳐(--> \\ScreenShot)",
				"  [Ctrl + M/T/Z] 지도/게임시간/존 이름",
				"  [Tab] 아이템창 on/off",			
				"  [Ctrl + Tab] 장착창 on/off",			
				"  [L-Ctrl + ~] 벨트 on/off",			
				"  [L-Ctrl + S] 기술 아이콘 선택",			
				"  [L-Ctrl + Space] 아이템창, 장착창 on/off",
				"---------------------------------------------------------------------",
				"  [L-Button] 이동, 추적, 공격",
				"  [L-Shift + L-Button] 강제 공격",
				"  [L-Ctrl + L-Button] 적만 선택 공격",
				"  [R-Button] 마법, 기술",
				"---------------------------------------------------------------------",
				"  [채팅할때] ! 를 사용하면 존 채팅"
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
// 전부 다시 그릴건지   
// 이전의 Tile을 이용해서 일부만 그릴건지를 선택한다.
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
		// 타일에 포탈 정보 출력하기
		if (g_pDXInput->KeyDown(DIK_T) && g_pDXInput->KeyDown(DIK_LCONTROL))
		{
			SetFirstDraw();
		}
	#endif	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD("Draw");
	#endif

	/*
	// Work Thread를 이용한 loading test
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
	// 비공식(-_-;) 영어 폰트 사용안함 - -;
	//------------------------------------------------------------
	//gC_font.NotUseNoPublicEng();		

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] Before Draw");
	//#endif	

	//------------------------------------------------------------
	// Clip영역 안에서만 출력한다.
	//------------------------------------------------------------
	int clipRight = m_pSurface->GetClipRight();
	int clipBottom	= m_pSurface->GetClipBottom();
	m_pSurface->SetClipRightBottom(CLIPSURFACE_WIDTH, CLIPSURFACE_HEIGHT);		

	//------------------------------------------------------------
	// Tile을 처음 그릴 때, 다~ 그린다.
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
		light.dvTheta =       0.5f; //원뿔의 중앙 크기
		light.dvPhi =         0.8f; //원뿔의 외곽 크기
		light.dvAttenuation0 = 1.0f;
		light.dvFalloff		= 1.0f;
		
		CDirect3D::GetDevice()->SetLight( 0, &light );	
		*/
		//*/
	}
	//------------------------------------------------------------
	// 이전에 그린 Tile을 다시 이용한다.
	//------------------------------------------------------------
	//else
	{			
		if (CDirect3D::IsHAL())// && m_b3DLight)
		{
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "IsLost?" );
			#endif

			// 도움이 될까..
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

				light.dvTheta -=       0.01f; //원뿔의 중앙 크기
				light.dvPhi -=         0.01f; //원뿔의 외곽 크기
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}

			if (g_pDXInput->KeyDown(DIK_A))
			{
				D3DLIGHT7 light;
				CDirect3D::GetDevice()->GetLight( 0, &light );

				light.dvTheta +=       0.01f; //원뿔의 중앙 크기
				light.dvPhi +=         0.01f; //원뿔의 외곽 크기
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}
			*/
			
			///*
			//test.Rotate( 0.1f, 0.1f, 0 );

			//------------------------------------------------
			// LightBuffer 초기화
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


			// mouse좌표의 빛			
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
			/* // 2001.7.14 주석처리
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0,//440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test용 도움말
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
			// 제거된 Surface들을 지워줘야 한다.
			// 반드시, EndScene()이 끝나고 지워야 한다.
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
			// LightBuffer 초기화
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
			/* // 2001.7.14 주석처리
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0, //440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test용 도움말
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
	// 정보 출력
	//------------------------------------------------
	DrawInformation();

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Draw");
	//#endif	
	

	//char str[80];
	//sprintf(str, "ImageObject=%d개", m_mapImageObject.size());
	//m_pSurface->GDI_Text(100,580, str, 0xFFFFFF);

	//------------------------------------------------------------
	// 정상적인 출력
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
// m_LightBufferFilter를 m_pLightBufferTexture로 blt하고
// m_pLightBufferTexture를 texture로 설정해서
// 화면에 LightBufferFilter를 출력하게 된다.
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
		// Texture설정하고 출력
		//------------------------------------------------
		RECT rect = { 0, 0, CLIPSURFACE_WIDTH, CLIPSURFACE_HEIGHT };
			
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
// m_LightBufferFilter를 m_pLightBufferTexture로 blt하고
// m_pLightBufferTexture를 texture로 설정해서
// 화면에 LightBufferFilter를 출력하게 된다.
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
				*lpSurfaceTemp10;

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
				// 9줄씩 출력할때...
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

					// 다음..
					lpSurface = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len점 찍기
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp9, len, light);
						//}
						

						// 다음 filter값
						pFilter++;
						pPW++;

						// 다음 출력 위치 --> 8 pixel 뒤
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
					}	
				}
				//--------------------------------------------
				// 10줄씩 출력
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

					// 다음..
					lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len점 찍기
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
						//}
						

						// 다음 filter값
						pFilter++;
						pPW++;

						// 다음 출력 위치 --> 8 pixel 뒤
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
				// 9줄씩 출력할때...
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

					// 다음..
					lpSurface = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len점 찍기
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp9, len, light);
						//}
						

						// 다음 filter값
						pFilter++;
						pPW++;

						// 다음 출력 위치 --> 8 pixel 뒤
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
					}	
				}
				//--------------------------------------------
				// 10줄씩 출력
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

					// 다음..
					lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len점 찍기
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
						//}
						

						// 다음 filter값
						pFilter++;
						pPW++;

						// 다음 출력 위치 --> 8 pixel 뒤
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
// 화면에 있는 ImageObject들을 모두 검색한다.
//----------------------------------------------------------------------
void
MTopView::DetermineImageObject()
{	
	//---------------------------------------------------------------	
	// firstPoint에 출력될 Zone에서의 첫번째 Sector
	//---------------------------------------------------------------	
	// Player를 중심으로 해서 출력해야하기 때문에
	// 왼쪽~~~~ 위~ 부터 출력한다.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// Zone의 경계에 갔을 때,
	// 검은 부분 안 보이게 하기..
	//---------------------------------------------------------------	
	// X 좌표 맞추기..
	//---------------------------------------------------------------	
	if (firstSector.x <= 0)
	{	
		firstSector.x = 0;			
	}
	else if (firstSector.x+SECTOR_WIDTH+1 >= m_pZone->GetWidth())
	{
		firstSector.x = m_pZone->GetWidth()-SECTOR_WIDTH-1;
	}

	//---------------------------------------------------------------	
	// Y 좌표 맞추기
	//---------------------------------------------------------------	
	if (firstSector.y <= 0)
	{
		firstSector.y = 0;
	}
	else if (firstSector.y+SECTOR_HEIGHT+1 >= m_pZone->GetHeight())
	{
		firstSector.y = m_pZone->GetHeight()-SECTOR_HEIGHT-1;	
	}

	//----------------------------------------------------------------------
	//
	//                         ImageObject 검색
	//
	//----------------------------------------------------------------------	
	//POINT firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	int sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	int sY1 = firstSector.y + SECTOR_SKIP_UP;
	int sX2 = firstSector.x + SECTOR_WIDTH+1;
	int sY2 = firstSector.y + SECTOR_HEIGHT+1;
	
	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
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
	// 이전에 있던 ImageObject를 모두 지운다.
	//------------------------------------------------------
	m_mapImageObject.clear();	

	//------------------------------------------------------
	// 각 Sector의 ImageObject검색
	//------------------------------------------------------
	int y,x,i;
	for (y=sY1; y<=sY2; y++)
	{				
		for (x=sX1; x<=sX2; x++)
		{				
			const MSector& sector = m_pZone->GetSector(x,y);

			// ImageObject가 있다면.. 모두~ 기억해둔다.
			// 단, 중복을 허용하지 않는다.
			if (sector.IsExistImageObject())
			{
				OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

				// Sector에 있는 모든 ImageObject들을 검색한다.
				for (i=0; i<sector.GetImageObjectSize(); i++)
				{
					MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

					//----------------------------------------
					// 출력 시점을 체크해야 되지만,
					// 어떤 ImageObject들은
					// 자신의 ViewSector가 아닌 viewpoint를
					// 갖는 경우도 있다. -_-;
					// 그래서.. 무조건 추가한다.
					//----------------------------------------
					// 출력 시점이 됐으면.. 추가한다.
					//if (pImageObject->GetViewpoint()==y				// 정해진 줄
					//	|| y==sY2-1									// 끝줄
					//	|| pImageObject->GetViewpoint() < sY1)		// 첫줄
					{						
						// 이미 있는지 확인해보고 없으면 추가.
						QWORD key = GetOutputImageObjectID( pImageObject );

						if (m_mapImageObject.find( key )
							== m_mapImageObject.end())
						{			
							//----------------------------------------
							// Key값 = (Viewpoint << 32) | ID
							//----------------------------------------
							// Viewpoint에 의해서 sort된다.
							// Viewpoint와 ID에 의해서 제거할 수 있다.
							//----------------------------------------
							m_mapImageObject.insert(
								IMAGEOBJECT_OUTPUT_MAP::value_type(
									key,	// key값
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
// m_FirstPoint는 바로 전의~ 화면 좌표이고..
// parameter의 firstSector가 현재의 화면 좌표이다.
//
// 화면에서 없어지는 ImageObject는 m_mapImageObject에서 없애고
// 화면에 나타나는 ImageObject는 m_mapImageObject에 추가한다.
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
	//               ImageObject 갱신
	//
	//---------------------------------------------------------------
	// newFirstSector와 m_FirstSector(바로 이전의 좌표)를 비교해서
	// 다른 경우... check~~해야한다.
	//---------------------------------------------------------------
	// 상하로 좌표가 달라진 경우
	//---------------------------------------------------------------
	if (newFirstSector.y != m_FirstSector.y)
	{		
		int	sX1=newFirstSector.x + SECTOR_SKIP_LEFT, 
				sX2=newFirstSector.x + SECTOR_WIDTH+1, 
				sX01=m_FirstSector.x + SECTOR_SKIP_LEFT, 
				sX02=m_FirstSector.x + SECTOR_WIDTH+1, 
				eraseY1, eraseY2,		// 지워야될 것들이 있는 줄
				lastY,					// 화면의 마지막 줄(지우면 안되는 것들)
				newY1, newY2;			// 새로 나타나는 것들이 있는 줄
		
		// 체크할 필요가 있는가?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone영역을 벗어나는 경우
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
		// 위쪽으로 이동한 경우
		//-----------------------------------
		if (newFirstSector.y - m_FirstSector.y < 0)
		{
			// 아래쪽에서 사라지는 건물이 있는지 확인해봐야한다.
			// --> 이전 화면의 마지막 줄에는 있지만,
			//     현재 화면의 마지막 줄에는 없는 것들은 사라지는 것이다.
			
			// 다시 살릴 것
			lastY	= newFirstSector.y + SECTOR_HEIGHT+1;	// 현재 화면의 마지막 줄

			// 지워야 될 것 : lasyY+1 ~ eraseY
			eraseY1	= lastY+1;							
			eraseY2	= m_FirstSector.y + SECTOR_HEIGHT+1;	// 이전 화면의 마지막 줄			

			// 새로 나타나는 것
			newY1	= newFirstSector.y + SECTOR_SKIP_UP;
			newY2	= m_FirstSector.y + SECTOR_SKIP_UP - 1;

			//------------------------------------------------------
			// Zone의 영역이 아닌 경우에 Skip...
			//------------------------------------------------------
			if (lastY >= m_pZone->GetHeight())
			{
				//lastY = m_pZone->GetHeight()-1;				
				// 살릴 필요 없다.
				bCheckLast = false;
			}

			if (eraseY1 >= m_pZone->GetHeight())
			{
				//eraseY1 = m_pZone->GetHeight()-1;
				// erase할 필요 없다. 
				bCheckErase = false;
			}
			else if (eraseY2 >= m_pZone->GetHeight())
			{
				eraseY2 = m_pZone->GetHeight()-1;								
			}		

			if (newY2 < 0) 
			{				
				//newY2 = 0;
				// new할 필요 없다.
				bCheckNew = false;
			}		
			else if (newY1 < 0) 
			{				
				newY1 = 0;	
			}
			
		}
		//-----------------------------------
		// 아래쪽으로 이동한 경우
		//-----------------------------------
		else
		{	
			// 다시 살릴 것
			lastY	= newFirstSector.y + SECTOR_SKIP_UP;	// 현재 화면의 마지막 줄

			// 지워야될 것 : eraseY ~ lastY-1
			eraseY1	= m_FirstSector.y + SECTOR_SKIP_UP;	// 이전 화면의 마지막 줄
			eraseY2	= lastY-1;		

			// 새로 나타나는 것
			newY1	= m_FirstSector.y + SECTOR_HEIGHT+1 + 1;
			newY2	= newFirstSector.y + SECTOR_HEIGHT+1;

			//------------------------------------------------------
			// Zone의 영역이 아닌 경우에 Skip...
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
		// erase1 ~ erase2까지는 지울만한(?) 것들.
		//----------------------------------------------
		// 일단 m_mapImageObject에서 지운다.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (y=eraseY1; y<=eraseY2; y++)
			{
				for (x=sX01; x<=sX02; x++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject가 있다면.. 모두~ 기억해둔다.
					// 단, 중복을 허용하지 않는다.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector에 있는 모든 ImageObject들을 검색한다.
						for (int i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key값 = (Viewpoint << 32) | ID
							//----------------------------------------
							// 이미 있는지 확인해보고 있으면 제거.
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
		// lastY는 다시 추가한다.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (x=sX1; x<=sX2; x++)
			{
				const MSector& sector = m_pZone->GetSector(x, lastY);

				// ImageObject가 있으면..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector에 있는 모든 ImageObject들을 검색한다.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

						// 출력 시점이 됐으면.. 추가한다.
						//if (pImageObject->GetViewpoint()==y				// 정해진 줄
						//	|| y==sY2-1									// 끝줄
						//	|| pImageObject->GetViewpoint() < sY1)		// 첫줄
						{		
							QWORD key = GetOutputImageObjectID(pImageObject);
							// 이미 있는지 확인해보고 없으면 추가.
							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key값 = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint에 의해서 sort된다.
								// Viewpoint와 ID에 의해서 제거할 수 있다.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key값
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
		// 새로 나타는 것들 : newY1 ~ newY2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (y=newY1; y<=newY2; y++)
			{
				for (x=sX1; x<=sX2; x++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject가 있으면..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector에 있는 모든 ImageObject들을 검색한다.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							// 출력 시점이 됐으면.. 추가한다.						
							//if (viewPoint==y				// 정해진 줄
							//	|| viewPoint > newY2									// 끝줄
							//	|| viewPoint < newY1)		// 첫줄
							{						
								// 이미 있는지 확인해보고 없으면 추가.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key ) == m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key값 = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint에 의해서 sort된다.
									// Viewpoint와 ID에 의해서 제거할 수 있다.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key값
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
	// 좌우로 좌표가 달라진 경우
	//---------------------------------------------------------------
	if (newFirstSector.x != m_FirstSector.x)
	{
		int	sY1=newFirstSector.y + SECTOR_SKIP_UP, 
				sY2=newFirstSector.y + SECTOR_HEIGHT+1, 
				sY01=m_FirstSector.y + SECTOR_SKIP_UP, 
				sY02=m_FirstSector.y + SECTOR_HEIGHT+1, 
				eraseX1, eraseX2,		// 지워야될 것들이 있는 줄
				lastX,					// 화면의 마지막 줄(지우면 안되는 것들)
				newX1, newX2;			// 새로 나타나는 것들이 있는 줄

		// 체크할 필요가 있는가?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone영역을 벗어나는 경우
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
		// 왼쪽으로 이동한 경우
		//-----------------------------------
		if (newFirstSector.x - m_FirstSector.x < 0)
		{
			// 오른쪽에서 사라지는 건물이 있는지 확인해봐야한다.
			// --> 이전 화면의 마지막 줄에는 있지만,
			//     현재 화면의 마지막 줄에는 없는 것들은 사라지는 것이다.
			
			// 다시 살릴 것
			lastX	= newFirstSector.x + SECTOR_WIDTH+1;	// 현재 화면의 마지막 줄

			// 지워야 될 것 : lasyX+1 ~ eraseX
			eraseX1	= lastX+1;							
			eraseX2	= m_FirstSector.x + SECTOR_WIDTH+1;	// 이전 화면의 마지막 줄			

			// 새로 나타나는 것
			newX1	= newFirstSector.x + SECTOR_SKIP_LEFT;
			newX2	= m_FirstSector.x + SECTOR_SKIP_LEFT - 1;

			//------------------------------------------------------
			// Zone의 영역이 아닌 경우에 Skip...
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
		// 오른쪽으로 이동한 경우
		//-----------------------------------
		else
		{	
			// 제거             추가      추가
			// eraseX1~eraseX2~lastX ~~~~ newX1~newX2
			// 다시 살릴 것
			lastX	= newFirstSector.x + SECTOR_SKIP_LEFT;	// 현재 화면의 마지막 줄

			// 지워야될 것 : eraseX ~ lastX-1
			eraseX1	= m_FirstSector.x + SECTOR_SKIP_LEFT;	// 이전 화면의 마지막 줄
			eraseX2	= lastX-1;		

			// 새로 나타나는 것
			newX1	= m_FirstSector.x + SECTOR_WIDTH+1 + 1;
			newX2	= newFirstSector.x + SECTOR_WIDTH+1;

			//------------------------------------------------------
			// Zone의 영역이 아닌 경우에 Skip...
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
		// erase1 ~ erase2까지는 지울만한(?) 것들.
		//----------------------------------------------
		// 일단 m_mapImageObject에서 지운다.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (x=eraseX1; x<=eraseX2; x++)
			{
				for (y=sY01; y<=sY02; y++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject가 있다면.. 모두~ 기억해둔다.
					// 단, 중복을 허용하지 않는다.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector에 있는 모든 ImageObject들을 검색한다.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key값 = (Viewpoint << 32) | ID
							//----------------------------------------
							// 이미 있는지 확인해보고 있으면 제거.
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
		// lastX는 다시 추가한다.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (y=sY1; y<=sY2; y++)
			{
				const MSector& sector = m_pZone->GetSector(lastX, y);

				// ImageObject가 있으면..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector에 있는 모든 ImageObject들을 검색한다.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
						// 출력 시점이 됐으면.. 추가한다.
						//if (pImageObject->GetViewpoint()==y				// 정해진 줄
							//|| y==sY2-1									// 끝줄
							//|| pImageObject->GetViewpoint() < sY1)		// 첫줄
						{						
							// 이미 있는지 확인해보고 없으면 추가.
							QWORD key = GetOutputImageObjectID( pImageObject );

							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key값 = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint에 의해서 sort된다.
								// Viewpoint와 ID에 의해서 제거할 수 있다.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key값
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
		// 새로 나타는 것들 : newX1 ~ newX2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (x=newX1; x<=newX2; x++)
			{
				for (y=sY1; y<=sY2; y++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject가 있으면..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector에 있는 모든 ImageObject들을 검색한다.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
							// 출력 시점이 됐으면.. 추가한다.
							//if (viewPoint<=y			// 위에 꺼
							//	|| viewPoint > sY2)		// 끝줄
								//|| viewPoint < newX1)		// 첫줄
							{								
								// 이미 있는지 확인해보고 없으면 추가.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key )
									== m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key값 = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint에 의해서 sort된다.
									// Viewpoint와 ID에 의해서 제거할 수 있다.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key값
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
// Pixel 좌표인 (Xp, Yp)가 
//       화면의 (0, 0)에 오도록 하여 나머지 부분을 출력한다.
//
// pSurface의 (X,Y)에 Zone의 (Xp,Yp)부분부터 출력한다.
//----------------------------------------------------------------------
//
// 이전에 출력한 Tile을 이용해서 현재 Tile을 구성해준 뒤,
// 다른 Object를 출력한다.
//
//----------------------------------------------------------------------
void	
MTopView::DrawZone(int firstPointX,int firstPointY)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawZone" );
	#endif

	//-------------------------------------------------
	// ItemName list를 지워준다.
	//-------------------------------------------------
	ClearItemNameList();	


	// mouse로 선택된 캐릭터..
	m_pSelectedCreature = NULL;


	//-------------------------------------------------
	//
	// Object를 출력하기 위한 정보
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
	// Player를 가리는 ImageObject들에 대한 정보
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
	// 첫 Sector가 출력될 좌표 보정(smooth scroll을 위해서)
	// Player의 좌표를 중심으로 해서 정한다.
	//-------------------------------------------------
	m_PlusPoint.x = g_pPlayer->GetSX() - firstPointX;
	m_PlusPoint.y = g_pPlayer->GetSY() - firstPointY;
	firstPointX -= g_pPlayer->GetSX();
	firstPointY -= g_pPlayer->GetSY();


	//---------------------------------------------------------------	
	// firstPoint에 출력될 Zone에서의 첫번째 Sector
	//---------------------------------------------------------------	
	// Player를 중심으로 해서 출력해야하기 때문에
	// 왼쪽~~~~ 위~ 부터 출력한다.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// Zone의 경계에 갔을 때,
	// 검은 부분 안 보이게 하기..
	//---------------------------------------------------------------	
	// X 좌표 맞추기..
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
	if (firstSector.x+SECTOR_WIDTH+1 >= m_pZone->GetWidth())
	{
		if (firstSector.x+SECTOR_WIDTH+1==m_pZone->GetWidth())
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

		firstSector.x = m_pZone->GetWidth()-SECTOR_WIDTH-1;
	}

	//---------------------------------------------------------------	
	// Y 좌표 맞추기
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
	else if (firstSector.y+SECTOR_HEIGHT >= m_pZone->GetHeight())
	{
		if (firstSector.y+SECTOR_HEIGHT == m_pZone->GetHeight())
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

		firstSector.y = m_pZone->GetHeight()-SECTOR_HEIGHT;	
	}

	//---------------------------------------------------------------
	// 새로 나타나거나 화면에서 없어지는 ImageObject검색
	//---------------------------------------------------------------
	// 2001.8.22

	__BEGIN_PROFILE("UpdateImageObject")

	UpdateImageObject(firstSector);	

	__END_PROFILE("UpdateImageObject")

	
	//---------------------------------------------------------------
	// 외부에서 필요한 정보를 생성..
	// 화면 상의 첫 위치에 출력되는 Sector의 좌표
	//---------------------------------------------------------------
	m_FirstSector	= firstSector;	

	//---------------------------------------------------------------
	// 화면의 (0,0)이 나타내는 Zone의 Pixel좌표
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
	// Player를 가리는 ImageObject들을 처리할 Filter의 출력 좌표
	//---------------------------------------------------------------
	m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - (m_ImageObjectFilter.GetWidth()>>1) + TILE_X_HALF;
	m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - (m_ImageObjectFilter.GetHeight()>>1) 
						- (g_pPlayer->IsFlyingCreature()? 3:1 )*TILE_Y;
	//filterPoint.x = 310;	// 410 - 100;
	//filterPoint.y = 116;	// 204 - 100;


	//short	sX,
	//		sY = (short)firstSector.y;

	// 출력할 Surface상의 위치	
	tilePoint.y = firstPointY;


	//--------------------------------------------------
	// TileSurface의 영역 : m_TileSurfaceFirstPixelXY 
	//                      + (TILESURFACE_WIDTH+ TILESURFACE_HEIGHT)
	//
	// 현재 화면의 영역   : m_FirstZonePixel + (800,600)
	//--------------------------------------------------
	//
	// 현재 화면 영역이 TileSurface에 속하면 
	// TileSurface의 일부를 그대로 이용하면 된다.
	//
	//
	//--------------------------------------------------
	__BEGIN_PROFILE("ReuseTileSurface")

	RECT rectTileSurface;
	RECT rectScreen;
	RECT	rectReuse;

	// TileSurface의 Zone에서의 영역
	rectTileSurface.left	= m_TileSurfaceFirstZonePixelX;
	rectTileSurface.top		= m_TileSurfaceFirstZonePixelY;
	rectTileSurface.right	= m_TileSurfaceFirstZonePixelX + TILESURFACE_WIDTH;
	rectTileSurface.bottom	= m_TileSurfaceFirstZonePixelY + TILESURFACE_HEIGHT;

	// 현재 화면의 영역
	rectScreen.left		= m_FirstZonePixel.x;
	rectScreen.top		= m_FirstZonePixel.y;
	rectScreen.right	= m_FirstZonePixel.x + CLIPSURFACE_WIDTH;
	rectScreen.bottom	= m_FirstZonePixel.y + CLIPSURFACE_HEIGHT;

	// 현재 화면 출력에 이용할 TileSurface의 영역을 정한다.
	// 화면의 (0,0)이 될 TileSurface에서의 첫 점
	rectReuse.left		= rectScreen.left - rectTileSurface.left;
	rectReuse.top		= rectScreen.top - rectTileSurface.top;
	rectReuse.right		= rectReuse.left + CLIPSURFACE_WIDTH;
	rectReuse.bottom	= rectReuse.top + CLIPSURFACE_HEIGHT;

	//----------------------------------------------------------------	
	// rectReuse를 제외한 부분은 검은색으로 칠해줘야 한다.
	//----------------------------------------------------------------
	/*
	// 위쪽
	if (rectReuse.top != 0)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = rectReuse.top;

		m_pSurface->FillRect(&rect, 0);
	}

	// 아래쪽
	if (rectReuse.bottom != SURFACE_HEIGHT)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = rectReuse.bottom;
		rect.bottom = SURFACE_HEIGHT;

		m_pSurface->FillRect(&rect, 0);
	}

	// 왼쪽
	if (rectReuse.left != 0)
	{
		rect.left = 0;
		rect.right = rectReuse.left;
		rect.top = rectReuse.top;
		rect.bottom = rectReuse.bottom;	

		m_pSurface->FillRect(&rect, 0);
	}

	// 오른쪽
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
	// TileSurface를 그대로 쓸 수 있는지 체크한다.
	//--------------------------------------------------
	// 부족한 부분 표시	
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
	// 현재 화면이 TileSurface에 속하는 경우
	//
	//---------------------------------------------------------------------
	if (bLack==0)
	{
		//....
		//---------------------------------------------------------------
		// 새로 나타나거나 화면에서 없어지는 ImageObject검색
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

	}
	/*
	//---------------------------------------------------------------------
	//
	// 너무 많이 갱신해야하는 경우
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
		
		// Tile전체를 다시 그려준다.
		DrawTileSurface();

		DetermineImageObject();		

		m_bFirstTileDraw = false;

		DEBUG_ADD("[DrawZone] Too Far Move - DrawTileSurface OK");		
	}
	*/
	//---------------------------------------------------------------------
	//
	// 현재 화면이 TileSurface에 속하지 않는 경우
	//
	//---------------------------------------------------------------------
	else
	{
		//---------------------------------------------------------------
		// 새로 나타나거나 화면에서 없어지는 ImageObject검색
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

		//----------------------------------------
		// 부분적으로만 갱신하는데 필요한 정보
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
			// LEFT + UP 부족
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = TILESURFACE_OUTLINE_UP;
				rect.left	= 0;
				rect.top	= 0;
				rect.right	= TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = TILESURFACE_OUTLINE_DOWN;
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------				

				changeCount = 2;

				//--------------------------------------------------
				// Left부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Up부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;


				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}			
			//--------------------------
			// LEFT + DOWN 부족
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = 0;
				rect.left	= 0;
				rect.top	= TILESURFACE_OUTLINE_UP;
				rect.right	= TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Left부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Down부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// LEFT 부족
			//--------------------------
			else
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = 0;
				rect.left	= 0;
				rect.top	= 0;
				rect.right	= TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
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
			// RIGHT + UP 부족
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = 0;
				point.y = TILESURFACE_OUTLINE_UP;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= 0;
				rect.right	= TILESURFACE_WIDTH;
				rect.bottom = TILESURFACE_OUTLINE_DOWN;
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Right부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// UP부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;

				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT + DOWN 부족
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= TILESURFACE_OUTLINE_UP;
				rect.right	= TILESURFACE_WIDTH;
				rect.bottom = TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Right부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Down부분
				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT 부족
			//--------------------------
			else
			{
				//--------------------------------------------------
				// 이전 Tile Surface에서
				// 다시 구성할려는 Tile Surface에서 사용할 수 
				// 있는 부분을 남겨둔다.			
				//--------------------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= 0;
				rect.right	= TILESURFACE_WIDTH;
				rect.bottom = TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 부족한 부분을 그려준다.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 첫번째 출력 좌표
				//--------------------------------------------------
				firstTilePoint[0].x = TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// 출력에 사용할 TileSurface 영역 다시 설정
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// 갱신된 TileSurface에 대한 좌표 설정
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
			}
		}
		//--------------------------
		// UP 부족
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_UP)
		{
			//--------------------------------------------------
			// 이전 Tile Surface에서
			// 다시 구성할려는 Tile Surface에서 사용할 수 
			// 있는 부분을 남겨둔다.			
			//--------------------------------------------------
			point.x = 0;
			point.y = TILESURFACE_OUTLINE_UP;
			rect.left	= 0;
			rect.top	= 0;
			rect.right	= TILESURFACE_WIDTH;
			rect.bottom = TILESURFACE_OUTLINE_DOWN;
			m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

			//--------------------------------------------------
			//
			// 부족한 부분을 그려준다.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
			sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// 첫번째 출력 좌표
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = 0;

			//--------------------------------------------------
			// 출력에 사용할 TileSurface 영역 다시 설정
			//--------------------------------------------------
			rectReuse.top += TILESURFACE_OUTLINE_UP;
			rectReuse.bottom += TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// 갱신된 TileSurface에 대한 좌표 설정
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
		}
		//--------------------------
		// DOWN 부족
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
		{
			//--------------------------------------------------
			// 이전 Tile Surface에서
			// 다시 구성할려는 Tile Surface에서 사용할 수 
			// 있는 부분을 남겨둔다.			
			//--------------------------------------------------
			point.x = 0;
			point.y = 0;
			rect.left	= 0;
			rect.top	= TILESURFACE_OUTLINE_UP;
			rect.right	= TILESURFACE_WIDTH;
			rect.bottom = TILESURFACE_HEIGHT;
			m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

			//--------------------------------------------------
			//
			// 부족한 부분을 그려준다.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// 출력할 sector (sX1,sY) ~ (sX2, sY2)까지 출력
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_HEIGHT;
			sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// 첫번째 출력 좌표
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = TILESURFACE_OUTLINE_DOWN;

			//--------------------------------------------------
			// 출력에 사용할 TileSurface 영역 다시 설정
			//--------------------------------------------------
			rectReuse.top -= TILESURFACE_OUTLINE_UP;
			rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// 갱신된 TileSurface에 대한 좌표 설정
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
		}


		//------------------------------------------------------
		//
		//   부분적으로 Tile들을 갱신하기..
		//
		//------------------------------------------------------
		register int n;
		for (n=0; n<changeCount; n++)
		{			
			POINT tilePointTemp;

			//------------------------------------------------------
			// Zone의 영역이 아닌 경우에 Skip...
			//------------------------------------------------------
			if (sX1[n] < 0) 
			{	
				// Zone외곽 부분은 검은색으로 칠한다.
				rect.left = firstTilePoint[n].x;
				rect.top = 0;	
				
				firstTilePoint[n].x += -sX1[n] * TILE_X;

				rect.right = firstTilePoint[n].x;
				rect.bottom = TILESURFACE_HEIGHT;				 

				m_pTileSurface->FillRect(&rect, 0);
				
				sX1[n] = 0;	
			}

			if (sX2[n] > m_pZone->GetWidth())
			{
				sX2[n] = m_pZone->GetWidth();//-1;				

				// Zone외곽 부분은 검은색으로 칠한다.
				rect.left = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.top = 0;					
				rect.right = TILESURFACE_WIDTH;
				rect.bottom = TILESURFACE_HEIGHT;

				m_pTileSurface->FillRect(&rect, 0);				
			}

			if (sY1[n] < 0)
			{
				// Zone외곽 부분은 검은색으로 칠한다.
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
				// Zone외곽 부분은 검은색으로 칠한다.
				rect.left = firstTilePoint[n].x;
				rect.top = firstTilePoint[n].y;					
				rect.right = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.bottom = TILESURFACE_HEIGHT;
				m_pTileSurface->FillRect(&rect, 0);

				sY2[n] = m_pZone->GetHeight();//-1;
			}
					
			// 첫번째 줄			
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
				// 한 줄의 첫번째 Sector					
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
						// ID가 spriteID인 Tile을 Load한다.
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
//							// Loading에 성공한 경우
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
//							// 실패한 경우 --> 이미 Loading하고 있는 경우이다.				
//							//--------------------------------------------------
//							/*
//							// 2001.8.20 주석처리
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
//								// file thread 순위를 높힌다.
//								//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//
//								MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_TileSPKI[spriteID]);
//								pNode->SetSPK( &m_TileSPK, FILE_SPRITE_TILE );
//								pNode->SetType( 1 );
//								g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//								g_pLoadingThread->AddFirst( pNode );
//
//								// Thread에서 Loading이 끝날때까지 기다린다.
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
//								// file thread 순위를 낮춘다.
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
							// 뜨악~~!!!!!!! 속도 잡아 먹는다~!!!
//							POINT pointTempTemp = point;
//							m_pTileSurface->BltSprite(&pointTempTemp, &m_EtcSPK[SPRITEID_TILE_NULL]);

							m_pTileSurface->BltSprite(&point, &sprite);
						//}
						//m_pTileSurface->BltSprite(&point, &m_SpritePack[ 3 ]);
						
						//sprintf(str, "(%d,%d)", x, y);			
						//m_pTileSurface->GDI_Text(point.x, point.y, str, 0);
					}
					

					// 출력하려는 좌표 이동
					tilePointTemp.x += TILE_X;
				}		
						
				// 다음 줄
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
			
			// TileTexture 설정
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
				// Light 설정
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


				// world matrix를 기울임..
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
	// 2004, 9, 3, sobeit add start - 타일 뒷쪽에 구름-_-;
//	event = g_pEventManager->GetEventByFlag(EVENTFLAG_CLOUD_BACKGROUND);
	if(g_pPlayer->IsShowAdamCloud())
	{
		CDirectDrawSurface *pCloudSurface = g_pEventManager->GetEventBackground(EVENTBACKGROUNDID_CLOUD);
		if(pCloudSurface != NULL)
		{
			int CloudPos = g_CurrentFrame % 800;
			POINT CloudPoint = {0,0};
			RECT CloudRect = { CloudPos, 0, 799, 599 };
			if(CloudPos != 799)
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSurface, &CloudRect);
			if(CloudPos != 0)
			{
				CloudPoint.x	= 799-CloudPos;
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
	// 2004, 9, 3, sobeit add end - 타일 뒷쪽에 구름-_-;
	if(bDrawBackGround)
		m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	

	__END_PROFILE("ReuseBltTileSurface")

	__END_PROFILE("ReuseTileSurface")
//#endif
		
	POINT tilePointTemp;
	int sX1, sX2, sY1, sY2;	

	//------------------------------------------------------
	// Object를 출력하기 위한 화면 좌표 설정
	//------------------------------------------------------
	sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	sY1 = firstSector.y + SECTOR_SKIP_UP;
	sX2 = firstSector.x + SECTOR_WIDTH;
	sY2 = firstSector.y + SECTOR_HEIGHT;

	// 출력할 Surface상의 위치
	tilePoint.x = firstPointX + TILE_X*SECTOR_SKIP_LEFT;
	tilePoint.y = firstPointY + TILE_Y*SECTOR_SKIP_UP;

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
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
	//                  Object 출력
	//
	//------------------------------------------------------

	//BYTE DarkBits;

	//---------------------------------------
	// LOCK
	//---------------------------------------
	if (!m_pSurface->Lock()) return;

// 2004, 03, 24, sobeit start
	//------------------------------------------------------
	// Creature OutputMap을 생성한다.
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
			// 출력 시점이 sY1보다 적은 경우..는 출력
			// 그림의 좌표를 현재 화면의 좌표에 맞추기								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// 밝기를 결정한다.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
			
			DrawCreature(&point, pCreature);//, DarkBits);
		}
	
		iCreatureOutput ++;
	}
// 2004, 03, 24, sobeit end
	/*
	//------------------------------------------------------
	// Sprite 하나 보기
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


	// Surface의 정보를 저장해둔다.
	//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());

	//------------------------------------------------------
	//
	//			바닥 오브젝트 출력
	//
	//------------------------------------------------------	
	//------------------------------------------------------
	// 출력대상이 되는 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput0 = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// 출력 시점이 0 즉 바닥 오브젝트인
	// ImageObject들을 출력한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput0 != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput0).second);

		// 출력 시점이 0인경우 경우..는 출력
		if (pImageObject->GetViewpoint() == 0)
		{
			// 그림의 좌표를 현재 화면의 좌표에 맞추기								
			point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
			point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
			DrawImageObject(&point, pImageObject);
		}
		// 아니면.. Object 출력할때 출력되는 경우이다.
//		else
//		{
//			break;
//		}

		iImageObjectOutput0 ++;
	}


	//------------------------------------------------------
	//
	//			Ground Effect 출력
	//
	//------------------------------------------------------	
	// player가 관속에 있는 경우 [새기술3]
	//------------------------------------------------------
	bool bPlayerInCasket = g_pPlayer->IsInCasket();
		
	if (bPlayerInCasket)
	{
		// [새기술3]
		// 화면 전체를 어둡게 표시한다. Tile만 - -;;
		rect.left = 0;
		rect.right = CLIPSURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = CLIPSURFACE_HEIGHT;

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
	// 선택된 Sector를 표시한다.
	//
	//------------------------------------------------------
	// 선택된 위치가 있고.. 선택된 Object가 없을 때...
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
				// 중심 좌표 보정
				CSprite* pSprite = &m_EtcSPK[ frameID[clickFrame] ];
				
				selectedPoint.x += TILE_X_HALF - (pSprite->GetWidth()>>1);
				selectedPoint.y += TILE_Y_HALF - (pSprite->GetHeight()>>1);
				
				
				m_pSurface->BltSprite(&selectedPoint, pSprite);
			}
			
			static DWORD lastTime = g_CurrentTime;
			if (g_CurrentTime - lastTime >= g_UpdateDelay)
			{
				if (++clickFrame==MaxClickFrame) clickFrame = 0;
				lastTime = g_CurrentTime;
			}
			
			
			// Player가 선택된 위치에 왔으면..
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
	//       ImageObject의 그림자 출력
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 출력대상이 되는 ImageObject Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawImageObjectShadow")

	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectShadowOutput = m_mapImageObject.begin();

	TYPE_SPRITEID	sprite;
	while (bDrawBackGround && iImageObjectShadowOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectShadowOutput).second);
		
		// 그림의 좌표를 현재 화면의 좌표에 맞추기								
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
	// 출력 시점보다 위쪽에 있는 ImageObject들을 출력한다.
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 출력대상이 되는 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// 출력 시점이 sY1보다 적은 경우의 
	// ImageObject들을 출력한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// 출력 시점이 sY1보다 적은 경우..는 출력
		if (pImageObject->GetViewpoint() < sY1)
		{
			if(pImageObject->GetViewpoint() != 0)
			{
				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
				point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
				point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
										
				DrawImageObject(&point, pImageObject);
			}
		}
		// 아니면.. Object 출력할때 출력되는 경우이다.
		else
		{
			break;
		}

		iImageObjectOutput ++;
	}

	// 애니메이션 오브젝트에서 Loop가 되는 에니메이션 오브젝트의 Frame을 맞추기 위해서
	if(g_bFrameChanged)
		MAnimationObject::NextLoopFrame();

/*	- 2004, 4, 24 sobeit block - ghost 때문에 위에서 처리 함
	//------------------------------------------------------
	// player가 관 속에 있는 경우가 아니면.. [새기술3]
	//------------------------------------------------------
	//------------------------------------------------------
	// Creature OutputMap을 생성한다.
	//------------------------------------------------------
	__BEGIN_PROFILE("AddOutputCreature")

	AddOutputCreatureAll();

	__END_PROFILE("AddOutputCreature")

	//------------------------------------------------------
	//
	//			모든 creature의 그림자를 출력한다.
	//
	//------------------------------------------------------
	// 모든 creature의 shadow를 출력한다.
	// (!) AddOutputCreatureAll()를 한 다음에 실행해야 한다.
	//
	// Lock된 상태에서 실행한다고 가정한다.
	//----------------------------------------------------
	// 3D가속할때는 Unlock으로 만든다.
	//----------------------------------------------------
	BOOL bUnlockStatus = CDirect3D::IsHAL() && g_pUserOption->BlendingShadow;
	if (bUnlockStatus)
	{	
		m_pSurface->Unlock();
	}
*/
	//------------------------------------------------------
	// 출력대상이 되는 Creature Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawCreatureShadowAll")

	//CREATURE_OUTPUT_MAP::const_iterator iCreatureOutput = m_mapCreature.begin();
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// 캐릭터 그림자를 모두 출력한다.
	//------------------------------------------------------
	// 타일을 체크해서 출력하는 경우
	//------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness랑 관계없이 보이는 경우. effect는 +2
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
							// player이거나 darkness랑 관계없이 보이는 경우
							if (pCreature==g_pPlayer
								|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
							{				
								// 그림의 좌표를 현재 화면의 좌표에 맞추기								
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

								// underground는 그림자가 없당.
								if (iCreature->first >= MSector::POSITION_GROUNDCREATURE)
								{
									pCreature = (MCreature*)iCreature->second;

									// player이거나 darkness랑 관계없이 보이는 경우
									if (
										(pCreature==g_pPlayer
										|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY())) &&
										!pCreature->IsFakeCreature()										
										)
									{				
										// 그림의 좌표를 현재 화면의 좌표에 맞추기								
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

		// 그림의 좌표를 현재 화면의 좌표에 맞추기								
		point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
		point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
								
		DrawCreatureShadow(&point, g_pPlayer);//, DarkBits);		
	}
	//------------------------------------------------------
	// 캐릭터 OutputMap을 이용하는 경우
	//------------------------------------------------------
	else
	{
		while (iCreatureOutput != m_mapCreature.end())
		{
			MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{
				// 출력 시점이 sY1보다 적은 경우..는 출력
				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// 밝기를 결정한다.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
				
				DrawCreatureShadow(&point, pCreature);//, DarkBits);
			}
		
			iCreatureOutput ++;
		}
	}

	__END_PROFILE("DrawCreatureShadowAll")

	//----------------------------------------------------
	// 원래의 Lock된 상태를 유지시켜 준다.
	//----------------------------------------------------
	if (bUnlockStatus)
	{	
		m_pSurface->Lock();
	}

	// Item 그림자 출력용
	//------------------------------------------------------
	// 화면의 Sector들을 모두 검색하면서
	// Sector에 존재하는 Object들을 출력한다.
	//------------------------------------------------------
	// 첫번째 줄	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tile을 검색해서 캐릭터를 출력하는 경우
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// 한 줄의 첫번째 Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------
				//
				//              Object 출력
				//
				//------------------------------------------------
					
				if (sector.IsExistObject())
				{				
			
					//----------------------------------------
					// Item일 경우
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// 시체인 경우
						// Effect를 출력하고 나서 출력한다.
						//
						// 2002.1.23
						// 언제 Effect출력코드가 빠진거지..
						// 암튼 시체 관련 코드는 뺀다. 
						//----------------------------------------
						//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
						{
						//	pCorpseItem = pItem;
						}
						//else
						{
							point.x = tilePointTemp.x;
							point.y = tilePointTemp.y;


							// 밝기를 결정한다.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
							DrawItemShadow(&point, pItem);//, DarkBits);
						}
					}
				}


				//------------------------------------			
				// 출력하려는 좌표 이동
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
					
			// 다음 줄
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tile을 검색하지 않고 OutputCreatureMap을 이용해서 캐릭터를 출력하는 경우
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// 관 속에 있을 때는 그냥 검색해서 출력하는 object는 없다.
			// AddOutputCreatureAll()에서 m_bTileSearchForCreature가 false로
			// 설정되므로... OutputCreatureMap을 이용하면 된다.
			if (!bPlayerInCasket)	// [새기술3]
			{			
				// 한 줄의 첫번째 Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------
					//
					//              Object 출력
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// Item일 경우
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// 시체인 경우
							// Effect를 출력하고 나서 출력한다.
							//
							// 2002.1.23
							// 언제 Effect출력코드가 빠진거지..
							// 암튼 시체 관련 코드는 뺀다. 
							//----------------------------------------
							//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
							//	pCorpseItem = pItem;
							}
							//else
							{
								point.x = tilePointTemp.x;
								point.y = tilePointTemp.y;

								// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
								DrawItemShadow(&point, pItem);//, DarkBits);
							}
						}
					}


					//------------------------------------			
					// 출력하려는 좌표 이동
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
					
			// 다음 줄
			tilePointTemp.y += TILE_Y;
		}
	}


	//------------------------------------------------------
	//
	//				실제 sprite 출력
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 출력대상이 되는 Creature Iterator
	//------------------------------------------------------
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// 출력 시점이 sY1보다 적은 경우의 
	// Creature들을 출력한다.
	// m_mapCreature에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost 일때 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost 일때 

		// 출력 시점이 sY1보다 적은 경우..는 출력
		if (pCreature->GetY() < sY1)
		{
			// player이거나 darkness랑 관계없이 보이는 경우
			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{					
				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// 밝기를 결정한다.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, pCreature);//, DarkBits);
			}
		}
		// 아니면.. 한 줄 출력이 끝나고 출력할때 출력되는 경우이다.
		else
		{
			break;
		}

		iCreatureOutput ++;
	}

	//------------------------------------------------------
	// 화면의 Sector들을 모두 검색하면서
	// Sector에 존재하는 Object들을 출력한다.
	//------------------------------------------------------
	// 첫번째 줄	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tile을 검색해서 캐릭터를 출력하는 경우
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// 한 줄의 첫번째 Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------------
				// 서버 block좌표에 사각형
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
				// 특정한 ImageObject의 ViewSector?에 "X"표하기
				//------------------------------------------------------
				#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						if (sector.IsExistImageObject())
						{
							OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

							// Sector에 있는 모든 ImageObject들을 검색한다.
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
				// 빛의 강도를 출력한다.
				//------------------------------------------------
				//if (x==g_SelectSector.x && y==g_SelectSector.y)
				//{
				//	char str[80];
				//	sprintf(str, "%d", sector.GetLight());
				//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
				//}

				//------------------------------------------------
				//
				//              Object 출력
				//
				//------------------------------------------------
				//MItem* pCorpseItem = NULL;
					
				if (sector.IsExistObject())
				{				
					//----------------------------------------
					// test code : Object 위치 표시
					//----------------------------------------
					//point = tilePointTemp;
					//m_pSurface->Copy(&point, m_pm_pSurface[0], 
					//				&m_SpriteSurfacePack[0][105]);
					//
					//----------------------------------------
			
					//----------------------------------------
					// Item일 경우
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// 시체인 경우
						// Effect를 출력하고 나서 출력한다.
						//
						// 2002.1.23
						// 언제 Effect출력코드가 빠진거지..
						// 암튼 시체 관련 코드는 뺀다. 
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
							// 울렁울렁~~
							int size = g_CurrentFrame & 0x00000007;

							int x0 = point.x + TILE_X_HALF;
							int y0 = point.y + TILE_Y_HALF;
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

							// 밝기를 결정한다.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
							DrawItem(&point, pItem);//, DarkBits);
						}
					}
				}

				//------------------------------------------------
				//
				//              Effect 출력
				//
				//------------------------------------------------
				//if (sector.IsExistEffect())
				//{				
				//	point = tilePointTemp;				
	//
	//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
	//			}

				//------------------------------------------------
				// 시체
				//------------------------------------------------
				// 음!! 꾸지한 코드의 지름길.. 음냐.. 나도 몰라
				//------------------------------------------------
				/*
				if (pCorpseItem)
				{
					point.x = tilePointTemp.x;
					point.y = tilePointTemp.y;

					// 밝기를 결정한다.
					//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
					
					// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
					DrawItem(&point, pItem);//, DarkBits);
				}
				*/

				//------------------------------------			
				// 출력하려는 좌표 이동
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
			//-----------------------------------------------------
			// 한 줄이 끝날때마다 
			// 출력해야할 그 줄의 ImageObject들을 출력한다.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// 출력 시점이 y와 같은 경우 출력
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// 그림의 좌표를 현재 화면의 좌표에 맞추기								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// 아니면.. 다른 줄이다.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// 한 줄이 끝날때마다 
			// 출력해야할 그 줄의 Creature들을 출력한다.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// 출력 시점이 sY1보다 적은 경우..는 출력
				if (pCreature->GetY() <= y)
				{
					// 2004, 04, 24 sobeit add start -ghost 일때 
					if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
					{
						iCreatureOutput ++;
						continue;
					}
					// 2004, 04, 24 sobeit add end -ghost 일때 

					// player이거나 darkness랑 관계없이 보이는 경우
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// 그림의 좌표를 현재 화면의 좌표에 맞추기								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// 밝기를 결정한다.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// 아니면.. 한 줄 출력이 끝나고 출력할때 출력되는 경우이다.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// 출력순서때문에.. 여기서 캐릭터를 체크한다.
			// Sector의 Effect 출력
			//------------------------------------------------------
			// 캐릭터보다 늦게 출력되어야 하므로...
			//------------------------------------------------------
			//------------------------------------------------------
			// Player는 Tile에 속하지 않기 때문에 따로 체크한다.
			//------------------------------------------------------
			if (g_pPlayer->GetY() == y)
			{
				// 그림의 좌표를 현재 화면의 좌표에 맞추기								
				point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
				point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
										
				// 밝기를 결정한다.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, g_pPlayer);//, DarkBits);
			}

			tilePointTemp.x = tilePoint.x;
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness랑 관계없이 보이는 경우. effect는 +2
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
						
								// 2004, 04, 24 sobeit add start -ghost 일때 
								if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
									continue;
								// 2004, 04, 24 sobeit add end -ghost 일때 

								// player이거나 darkness랑 관계없이 보이는 경우
								if (pCreature==g_pPlayer
									|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
								{				
									// 그림의 좌표를 현재 화면의 좌표에 맞추기								
									point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
									point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
															
									DrawCreature(&point, pCreature);//, DarkBits);
								}
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect 출력
					//
					//------------------------------------------------
					if (sector.IsExistEffect())
					{				
						point = tilePointTemp;				

	 					DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
					}
				}

				//------------------------------------			
				// 출력하려는 좌표 이동
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
					
			// 다음 줄
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tile을 검색하지 않고 OutputCreatureMap을 이용해서 캐릭터를 출력하는 경우
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// 관 속에 있을 때는 그냥 검색해서 출력하는 object는 없다.
			// AddOutputCreatureAll()에서 m_bTileSearchForCreature가 false로
			// 설정되므로... OutputCreatureMap을 이용하면 된다.
			if (!bPlayerInCasket)	// [새기술3]
			{			
				// 한 줄의 첫번째 Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------------
					// 서버 block좌표에 사각형
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
					// 특정한 ImageObject의 ViewSector?에 "X"표하기
					//------------------------------------------------------
					#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
						if (g_pDXInput->KeyDown(DIK_A) && 
							(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
						{
							if (sector.IsExistImageObject())
							{
								OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

								// Sector에 있는 모든 ImageObject들을 검색한다.
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
					// 빛의 강도를 출력한다.
					//------------------------------------------------
					//if (x==g_SelectSector.x && y==g_SelectSector.y)
					//{
					//	char str[80];
					//	sprintf(str, "%d", sector.GetLight());
					//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
					//}

					//------------------------------------------------
					//
					//              Object 출력
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// test code : Object 위치 표시
						//----------------------------------------
						//point = tilePointTemp;
						//m_pSurface->Copy(&point, m_pm_pSurface[0], 
						//				&m_SpriteSurfacePack[0][105]);
						//
						//----------------------------------------
				
						//----------------------------------------
						// Item일 경우
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// 시체인 경우
							// Effect를 출력하고 나서 출력한다.
							//
							// 2002.1.23
							// 언제 Effect출력코드가 빠진거지..
							// 암튼 시체 관련 코드는 뺀다. 
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
								// 울렁울렁~~
								int size = g_CurrentFrame & 0x00000007;

								int x0 = point.x + TILE_X_HALF;
								int y0 = point.y + TILE_Y_HALF;
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

								// 밝기를 결정한다.
								//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
								
								// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
								DrawItem(&point, pItem);//, DarkBits);
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect 출력
					//
					//------------------------------------------------
					//if (sector.IsExistEffect())
					//{				
					//	point = tilePointTemp;				
		//
		//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
		//			}

					//------------------------------------------------
					// 시체
					//------------------------------------------------
					// 음!! 꾸지한 코드의 지름길.. 음냐.. 나도 몰라
					//------------------------------------------------
					/*
					if (pCorpseItem)
					{
						point.x = tilePointTemp.x;
						point.y = tilePointTemp.y;

						// 밝기를 결정한다.
						//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
						
						// 나중에 Frame정보에서 cx,cy를 교정해야 한다.									
						DrawItem(&point, pItem);//, DarkBits);
					}
					*/

					//------------------------------------			
					// 출력하려는 좌표 이동
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
			
			//-----------------------------------------------------
			// 한 줄이 끝날때마다 
			// 출력해야할 그 줄의 ImageObject들을 출력한다.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// 출력 시점이 y와 같은 경우 출력
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// 그림의 좌표를 현재 화면의 좌표에 맞추기								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// 아니면.. 다른 줄이다.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// 한 줄이 끝날때마다 
			// 출력해야할 그 줄의 Creature들을 출력한다.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// 2004, 04, 24 sobeit add start -ghost 일때 
				if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
				{
					iCreatureOutput ++;
					continue;
				}
				// 2004, 04, 24 sobeit end start -ghost 일때 

				// 출력 시점이 sY1보다 적은 경우..는 출력
				if (pCreature->GetY() <= y)
				{
					// player이거나 darkness랑 관계없이 보이는 경우
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// 그림의 좌표를 현재 화면의 좌표에 맞추기								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// 밝기를 결정한다.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// 아니면.. 한 줄 출력이 끝나고 출력할때 출력되는 경우이다.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// Sector의 Effect 출력
			//------------------------------------------------------
			// 캐릭터보다 늦게 출력되어야 하므로...
			//------------------------------------------------------
			if (!bPlayerInCasket)	// [새기술3] 관 속에 있을때 Effect는 안 찍는다.
			{			
				tilePointTemp.x = tilePoint.x;
				for (x=sX1; x<=sX2; x++)
				{	
					int darknessCount = g_pPlayer->GetDarknessCount();
					
					// darkness랑 관계없이 보이는 경우. effect는 +2
					if (darknessCount < 0
						|| max(abs(g_pPlayer->GetX()-x), abs(g_pPlayer->GetY()-y)) <= darknessCount+2)
					//g_pPlayer->ShowInDarkness(x, y))
					{			
						const MSector& sector = m_pZone->GetSector(x,y);

						//------------------------------------------------
						//
						//              Effect 출력
						//
						//------------------------------------------------
						if (sector.IsExistEffect())
						{				
							point = tilePointTemp;				

	 						DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
						}
					}

					//------------------------------------			
					// 출력하려는 좌표 이동
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}
					
			// 다음 줄
			tilePointTemp.y += TILE_Y;
		}
	}

	
	//------------------------------------------------------
	// 출력 시점이 sY2보다 큰 경우의 
	// Creature들을 출력한다.
	// m_mapCreature에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost 일때 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost 일때 

		// player이거나 darkness랑 관계없이 보이는 경우
		if (pCreature==g_pPlayer
			|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
		{
			// 그림의 좌표를 현재 화면의 좌표에 맞추기								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// 밝기를 결정한다.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

			DrawCreature(&point, pCreature);//, DarkBits);		
		}

		iCreatureOutput ++;
	}


	//------------------------------------------------------
	// 출력 시점이 sY2보다 큰 경우의 
	// ImageObject들을 출력한다.
	// m_mapImageObject에는 Viewpoint가 key값이 되어서 Sort되어 있다.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// 그림의 좌표를 현재 화면의 좌표에 맞추기								
		point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
		point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
		DrawImageObject(&point, pImageObject);
		
		iImageObjectOutput ++;
	}

	//------------------------------------------------
	// mouse선택한 것의 외곽선만 다시 한 번 출력해준다.
	//------------------------------------------------
	if (m_SOM.IsInit())
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "m_SOM.Clear()" );
		#endif
		//m_pSurface->BltSpriteOutlineOnly( &m_SOM,  m_SOMOutlineColor );

		// 제거
		m_SOM.Clear();
	}
	
	//----------------------------------------------------------------
	// player가 관 속에 있는 경우가 아니면.. [새기술3]
	//----------------------------------------------------------------
	if (!bPlayerInCasket && bDrawBackGround)
	{			
		//----------------------------------------------------------------
		//
		//          Weather - 날씨 효과 출력
		//
		//----------------------------------------------------------------
		
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "Start DrawWeather" );
		#endif

		__BEGIN_PROFILE("DrawWeather")

		if (g_pWeather->IsActive())
		{
			// 가로 좌표 보정..
			// 현재 Player의 좌표와 비교한다.
			//int gapX = ((g_pPlayer->GetX() - g_pWeather->GetStartX()) * TILE_X + g_pPlayer->GetSX()) % SURFACE_WIDTH;
			//int gapY = ((g_pPlayer->GetY() - g_pWeather->GetStartY()) * TILE_Y + g_pPlayer->GetSY()) % SURFACE_HEIGHT;
			int gapX = (m_FirstZonePixel.x - g_pWeather->GetStartX()) % SURFACE_WIDTH;
			int gapY = (m_FirstZonePixel.y - g_pWeather->GetStartY()) % SURFACE_HEIGHT;

			for (i=0; i<g_pWeather->GetSize(); i++)
			{
				const MAP_EFFECT& MapEffect = (*g_pWeather)[i];

				// 멈추지 않은 것이면...
				if (MapEffect.IsActive())
				{
					point.x = MapEffect.GetX() - gapX;
					point.y = MapEffect.GetY() - gapY;

					if (point.x < 0) point.x += SURFACE_WIDTH;
					else if (point.x > SURFACE_WIDTH) point.x -= SURFACE_WIDTH;

					if (point.y < 0) point.y += SURFACE_HEIGHT;
					else if (point.y > SURFACE_HEIGHT) point.y -= SURFACE_HEIGHT;

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
				// player의 light 좌표 설정
				//----------------------------------------------------------------
				int pX = g_pPlayer->GetPixelX() - m_FirstZonePixel.x + TILE_X_HALF;
			int pY = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - TILE_Y;
			
			//----------------------------------------------------------------
			// 3D 
			//----------------------------------------------------------------
			// 박쥐인 경우 시야 +3	
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
					false,	// screenPixel좌표			
					true);	// 무조건 출력해야하는 빛		
				
				// 오토바이 불빛
				ADD_MOTORCYCLE_LIGHT_XY_3D( g_pPlayer, pX, pY, true );		
				
				//---------------------------------------
				// UNLOCK
				//---------------------------------------
				m_pSurface->Unlock();
				
				//---------------------------------------
				// 3D 시야처리 출력
				//---------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left = 0;
				rect.top = 0;
				rect.right = SURFACE_WIDTH;
				rect.bottom = SURFACE_HEIGHT;
				g_pBack->BltNoColorkey( &point, g_pLast, &rect );		

				g_pTopView->DrawLightBuffer3D();

				g_pLast->BltNoColorkey( &point, g_pBack, &rect );		

//				g_pLast->FillSurface(0x001F);

//				DrawLightBuffer3D();
				
				// Surface의 정보를 저장해둔다.
				//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());
			}
			//----------------------------------------------------------------
			// 2D
			//----------------------------------------------------------------
			else
			{
				// 박쥐인 경우 시야 +3		
				AddLightFilter2D( pX, 
					pY - g_pPlayer->GetZ(), 
					playerLight,  
					false,	// screenPixel좌표			
					true);	// 무조건 출력해야하는 빛
				
				// 오토바이 불빛
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
		// Unlock 상태이다..
		//----------------------------------------------------------------		

		//----------------------------------------------------------------
		// Mouse로 선택된 캐릭터이면.. 밝은 글자 출력
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
		// Item이름들을 출력하게 한다.
		//----------------------------------------------------------------
		if (m_bDrawItemNameList && !g_pPlayer->IsInDarkness())
		{
			__BEGIN_PROFILE("DrawItemNameList")

			// unlock상태에서 호출..
			DrawItemNameList();		

			__END_PROFILE("DrawItemNameList")
		}
	} // bPlayerInCasket
	else 
	{
		// [새기술3]
		m_pSurface->Unlock();
	}

	//----------------------------------------------------------------	
	// Lock
	//----------------------------------------------------------------	
	//m_pSurface->Lock();

	//----------------------------------------------------------------
	// 화면에 나오는 글자를 출력한다.
	//----------------------------------------------------------------
	__BEGIN_PROFILE("DrawTextList")

	DrawTextList();

	__END_PROFILE("DrawTextList")

	//-------------------------------------------------
	// 출력할 string에 기억된걸 다 없애준다.
	//-------------------------------------------------
	ClearTextList();

	//-------------------------------------------------
	// guild mark 출력 테스트
	//-------------------------------------------------
//	#ifdef _DEBUG
//
//		if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_G))
//		{		
//			int guildID = g_pPlayer->GetX()%10;
//
//			//-------------------------------------------------
//			// load되어 있는지 본다.
//			//-------------------------------------------------
//			CSprite* pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//			if (pSprite==NULL)
//			{		
//				//-------------------------------------------------
//				// file에 있는지 본다.
//				//-------------------------------------------------
//				g_pGuildMarkManager->LoadGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file에서 load되었는지 다시 체크
//				//-------------------------------------------------
//				pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file에도 없는 경우..
//				// guildMark관리서버?에서 받아온다.
//				//-------------------------------------------------
//				if (pSprite==NULL)
//				{
//					// 여기서는.. 임시로 화일에서 생성한다.
//					char str[256];
//					sprintf(str, "Data\\Guild\\Guild%d.bmp", guildID);
////					g_pGuildMarkManager->CreateGuildMark( guildID, str );
//				}
//			}
//			else
//			{			
//				// 작은거도 걍 함 찍어본다.
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
	// Light 출력
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
	// Object 출력	
	//----------------------------------------------------------------
	// Y좌표로 sort를 해서 출력해야 한다.
	//----------------------------------------------------------------
	// list대신에 priority queue를 사용하는게 낫지 싶다! sort자동~~
	// 그런데! list에 추가될때 순서가 있으니까... 관계가 없을 거 같다!
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
	// 그림자 Test		
	if (g_pDXInput->KeyDown(DIK_SPACE))
	{
		point.x = g_x;
		point.y = g_y;

		if (CDirect3D::IsHAL())
		{
			sprite = 0;//rand()%m_pImageObjectShadowManager->GetMaxIndex();

			m_pSurface->Unlock();
			// EffectTexture 설정
			//CDirect3D::GetDevice()->SetTexture(0, m_EffectTPK[sprite].GetSurface());
			CSpriteSurface* pSurface = m_pImageObjectShadowManager->GetTexture(sprite);		
			CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
			
			// 좌표 설정
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
			
			// 출력
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
	// (커서)
	// 움직이는 방향이 계속 바뀌고 있는 경우 --> 커서 표현	
	//
	//------------------------------------------------------
	/*
	if (!bPutSelectedSector && g_bLButtonDown)			
	{
		POINT cursorPoint;
		GetCursorPos(&cursorPoint);
		// 그리고, 현재 Player있는 위치를 선택하지 않은 경우이다.
		point = GetSelectedSector(cursorPoint.x, cursorPoint.y);
		if (g_pPlayer->GetX()!=point.x || g_pPlayer->GetY()!=point.y)
		{
			// player가 움직이는 방향으로 커서 모양 표시
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
	// Filter영역 사각형
	//---------------------------------------
	//rect.left = 410 - 100;
	//rect.right = 410 + 100;
	//rect.top = 270 - 100;
	//rect.bottom = 270 + 100;
	//m_pSurface->DrawRect(&rect, 0xFFFF);

	/*
	// Creature Sprite 보여주기
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
// - Player좌표를 중심으로 해서...
//   단지 Tile을 m_pTileSurface에 그린다.
//
// - 현재 화면의 Object들을 검색해서 출력에 이용할 수 있게 기억해둔다.
//----------------------------------------------------------------------
void	
MTopView::DrawTileSurface()
{
	//-------------------------------------------------
	//
	// Object를 출력하기 위한 정보
	//
	//-------------------------------------------------
	// sprite point
	POINT	tilePoint, point;
	RECT	rect;
	
	//---------------------------------------------------------------
	// Player를 중심에 두기 위한 첫번째 Sector좌표를 구한다.
	// 화면의 (0,0)이 나타내는 Zone의 Pixel좌표
	//---------------------------------------------------------------
	m_FirstZonePixel = MapToPixel(g_pPlayer->GetX()+SECTOR_SKIP_PLAYER_LEFT, 
									g_pPlayer->GetY()+SECTOR_SKIP_PLAYER_UP);
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
	// 화면의 (0,0)에 출력되는 Sector
	//---------------------------------------------------------------		
	POINT	firstSector;
	firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	m_FirstSector = firstSector;

	//---------------------------------------------------------------
	// TileSurface (0,0)에 출력되는 Sector좌표
	//---------------------------------------------------------------
	firstSector.x -= TILESURFACE_SECTOR_EDGE;
	firstSector.y -= TILESURFACE_SECTOR_EDGE;

	//---------------------------------------------------------------
	// TileSurface (0,0)의 Zone에서의 pixel좌표를 기억해둔다.
	//---------------------------------------------------------------
	m_TileSurfaceFirstSectorX		= firstSector.x;
	m_TileSurfaceFirstSectorY		= firstSector.y;
	
	point = MapToPixel(m_TileSurfaceFirstSectorX, m_TileSurfaceFirstSectorY);
	m_TileSurfaceFirstZonePixelX	= point.x;
	m_TileSurfaceFirstZonePixelY	= point.y;
	

	//----------------------------------------------------------------------
	//
	//                         Tile 출력
	//
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	// 무조건 최초 Sector가 TileSurface의 (0,0)에 오도록 해야한다.
	//----------------------------------------------------------------------
	int	sX1 = firstSector.x, 
			sX2 = firstSector.x + TILESURFACE_SECTOR_WIDTH,		// sX1 ~ sX2
			sY1 = firstSector.y, 
			sY2 = firstSector.y + TILESURFACE_SECTOR_HEIGHT;	// sY1 ~ sY2

	// 출력할 Surface상의 위치	
	tilePoint.x = 0;
	tilePoint.y = 0;
		
	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
	//------------------------------------------------------					
	if (sX1 < 0) 
	{			
		// Zone외곽 부분은 검은색으로 칠한다.
		rect.left = tilePoint.x;
		rect.top = 0;	
		
		tilePoint.x += -sX1 * TILE_X;

		rect.right = tilePoint.x;
		rect.bottom = TILESURFACE_HEIGHT;				 

		m_pTileSurface->FillRect(&rect, 0);
		
		sX1 = 0;	
	}

	if (sX2 > m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth();//-1;

		// Zone외곽 부분은 검은색으로 칠한다.
		rect.left = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.top = 0;					
		rect.right = TILESURFACE_WIDTH;
		rect.bottom = TILESURFACE_HEIGHT;

		m_pTileSurface->FillRect(&rect, 0);
	}

	if (sY1 < 0)
	{
		// Zone외곽 부분은 검은색으로 칠한다.
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
		// Zone외곽 부분은 검은색으로 칠한다.
		rect.left = tilePoint.x;
		rect.top = tilePoint.y;					
		rect.right = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.bottom = TILESURFACE_HEIGHT;
		m_pTileSurface->FillRect(&rect, 0);

		sY2 = m_pZone->GetHeight();//-1;
	}			
			
	//------------------------------------------------------
	// 일단 검은색으로 전체를 칠한다.
	//------------------------------------------------------
	/*
	rect.left = 0;
	rect.top = 0;
	rect.right = TILESURFACE_WIDTH;	
	rect.bottom = TILESURFACE_HEIGHT;

	m_pTileSurface->FillRect(&rect, 0);
	*/

	//------------------------------------------------------
	// 각 Sector출력
	//------------------------------------------------------	
	// 첫번째 줄
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
		// 한 줄의 첫번째 Sector					
		tilePointTemp.x = tilePoint.x;

		for (x=sX1; x<sX2; x++)
		{				
			point = tilePointTemp;

			// (sX,sY) Sector의 SpriteID를 읽어서 출력
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
				// ID가 spriteID인 Tile을 Load한다.
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
//					// 성공한 경우.. 
//					//--------------------------------------------------
//					if (m_TileSPK[spriteID].LoadFromFile( m_TileSPKFile ))
//					{
//						#ifdef	OUTPUT_DEBUG
//							strcat(str, "...OK");
//						#endif
//					}
//					//--------------------------------------------------
//					// 실패한 경우 --> 이미 Loading하고 있는 경우이다.				
//					//--------------------------------------------------
//					/*
//					// 2001.8.20 주석처리
//					else
//					{
//						#ifdef	OUTPUT_DEBUG
//							strcat(str, "...Fail & Wait Loading");
//						#endif
//
//						// file thread 순위를 높힌다.
//						//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//
//						// Thread에서 Loading이 끝날때까지 기다린다.
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
//						// file thread 순위를 낮춘다.
//						//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);					
//						g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//					}
//					*/
//
//					DEBUG_ADD( str );
//				}

				// 뜨악~~!!!!!!! 속도 잡아 먹는다~!!!
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
			

			// 출력하려는 좌표 이동
			tilePointTemp.x += TILE_X;
		}		
				
		// 다음 줄
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
	// HP계산
	//-----------------------------------------------------
	int currentHP		= pCreature->GetHP();
	int maxHP			= pCreature->GetMAX_HP();

	//-----------------------------------------------------
	// 땅속에 있는 애는 HP가 꽉 찬것처럼 보여준다.
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
	// 실제로 보여질 pixel계산
	//-----------------------------------------------------
	CSprite* pHPBackSprite	= &m_EtcSPK[SPRITEID_PARTY_HP_BACKGROUND];
	CSprite* pHPSprite		= &m_EtcSPK[SPRITEID_PARTY_HP];

	int maxPixels		= pHPBackSprite->GetWidth();
	int currentPixels	= (maxHP==0)? 0 : maxPixels * currentHP / maxHP;

	//-----------------------------------------------------
	// 출력
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
		// Unlock 상태로 만든다. 
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (!bLock)
		{
			m_pSurface->Lock();
		}

		MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );
		
		g_pTopView->DrawEffect(pPoint, g_pInventoryEffectManager->GetEffects(), g_pInventoryEffectManager->GetSize());

		//------------------------------------------------
		// 원래 lock 된 상태면 다시 되돌린다.
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
	// 약간 왼쪽에서 출력...
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
	//			Font설정
	//
	//---------------------------------------------------------	
	FONTID	font;
	//---------------------------------------------------------
	// Vampire와 Slayer에 따라서 Font를 다르게 해준다.
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
	//		제일 긴 string의 길이를 얻는다.
	//
	//---------------------------------------------------------
	int maxLen = 0;
	int numString = g_pClientConfig->MAX_CHATSTRING;
	int maxWidth = 0;
	for (register int i=g_pClientConfig->MAX_CHATSTRING_MINUS_1; i>=0; i--)
	{
		const char *str = pCreature->GetChatString(i);		

		//---------------------------------------------------------
		// 뭔가 있으면.. 출력해야한다.
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
			// 출력할게 없는 경우	
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
	//			출력 정보 설정
	//
	//---------------------------------------------------------	
	int firstY = y2 - numString * ChatFontHeight;

	int x = pPoint->x;
	int y = firstY;



	//---------------------------------------------------------
	// 화면 안에 글자가 들어오도록..
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
	
	if (x2 >= CLIPSURFACE_WIDTH)
	{
		x -= x2-CLIPSURFACE_WIDTH;
		x2 = CLIPSURFACE_WIDTH;
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
	//  각 string 출력...
	//---------------------------------------------------------
	int start = g_pClientConfig->MAX_CHATSTRING-numString; 
	for (i=start; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		const char *str = pCreature->GetChatString(i);
		
		// 뭔가 있으면.. 출력해야한다.
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
			// 33으로 한거는.. DWORD time에서 첫 bit가 필요없을 듯 하고
			// Creature Object ID로 캐릭터별로 순서에 맞춰서 출력하고 
			// 출력순서 i를 추가해서 순서대로 출력되게 한 것이다.
			//
			// 오래된거 < 캐릭터별로 < 줄순서
			//---------------------------------------------------------
			QWORD time = timeBase + m_pqDrawText.size();
			pNode->SetTextTime( time );
			
			//---------------------------------------------------------
			// 검은색 박스
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
				// 채팅에 외곽 박스 그리기
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
			// y줄 좌표 return
		//	return firstY;
		//}

		// 한줄씩 아래로..
		y += ChatFontHeight;
	}

	return firstY;
}

//----------------------------------------------------------------------
// Draw Item : Item 출력하기	
//----------------------------------------------------------------------
// pSurface의 pPoint에 pItem을 출력한다. 
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
	// Slayer인 경우 Darkness 안을 볼 수 없다.
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
	// 시체인 경우는 Creature를 출력한다.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		POINT orPoint = *pPoint;

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// PC Vampire인 경우는 '먼지'로 표현한다.
			// 
			// 틴버전인경우는 모두 '먼지'로 표현한다.
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
//				// 좌표 보정
//				pointTemp.x = pPoint->x - 5;
//				pointTemp.y = pPoint->y;
//					
//
//				// 선택된 것인 경우
//				// Creature를 선택한 것처럼해서 출력.. 사기당. - -;
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
			// 일반 캐릭터 죽은 모습..
			//---------------------------------------------------------
//			else
			{
				// 선택된 것인 경우
				// Creature를 선택한 것처럼해서 출력.. 사기당. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

//					if (
//							(
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 성물
//							|| pCreature->GetCreatureType() == 482	// 크리스마스 트리
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// 틴버전일 경우에는 슬레이어시체 그림자 출력해줘야함
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
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 성물
//							|| pCreature->GetCreatureType() == 482	// 크리스마스 트리
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// 틴버전일 경우에는 슬레이어시체 그림자 출력해줘야함
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
			// 죽은 캐릭터이면 선택 사각형을
			// 한 Tile 크기로 제한한다.
			//------------------------------------------------
			if(!(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673 ))
			{
				if(pCreature->GetCreatureType() == 482 || pCreature->GetCreatureType() == 650 )	// 트리는 select안되게
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
	// 아직 떨어지고 있는 중이면..
	//---------------------------------------------------------
	// m_ItemDropFPK와 m_ItemDropSPK를 이용..
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
		// 이거 고치고나서.. MZone::AddItem( ...) 에서
		// if (bDropping) 뒤의 주석부분을 살려야 한다..
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

		// 출력
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
	// 바닥에 있는 item
	//---------------------------------------------------------
	// m_ItemTileFPK와 m_ItemTileSPK를 이용
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

		// 좌표 보정
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
		// Item 선택 사각형 영역 설정
		// 아이템의 선택 영역 크기를 제한한다.
		//---------------------------------------- 	
		RECT rect;
		rect.left	= max(pointTemp.x, pPoint->x);
		rect.top	= max(pointTemp.y, pPoint->y);
		rect.right	= min(pointTemp.x + pSprite->GetWidth(), pPoint->x + TILE_X);
		rect.bottom = min(pointTemp.y + pSprite->GetHeight(), pPoint->y + TILE_Y);		

		pItem->SetScreenRect( &rect );

		// m_SpritePack에서 적절한 sprite를 골라서 출력해준다.

		//-------------------------------------------------------
		// Mouse가 가리키고 있는 Item
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
			// mouse가 가리키는 Item의 정보 표시
			//-------------------------------------
			/*
			const char* pItemName = pItem->GetName();

			// option이 붙은거는 파란색..
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

				// 화면 안에 글자가 들어오도록..
				if (x<0) x=0;
				if (y<0) y=0;

				//--------------------------------------------------
				// GetStringWidth에서는 unlock해야한다.
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


				// 검은색 박스			
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
			// 추적 중인 Item표시
			//-------------------------------------------------------
			//if (g_pPlayer->IsTraceItem() && g_pPlayer->GetTraceID()==pItem->GetID())
			{	
				// RED
			//	m_pSurface->BltSpriteColor(&pointTemp, &m_ItemTileSPK[ Frame.GetSpriteID() ], 0);
			}	
			//-------------------------------------------------------
			// 정상 출력
			//-------------------------------------------------------
			//else 
			{
				//if (CDirect3D::IsHAL() || DarkBits==0)
				//{			
					
				//-------------------------------------------------------
				// 설치된 지뢰이면..
				//-------------------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_MINE
					&& ((MMine*)pItem)->IsInstalled())
				{
					// 빨간색으로 출력한다.
					m_pSurface->BltIndexSpriteColor(&pointTemp, pSprite, 0);
				}
				//-------------------------------------------------------	
				// 일반적인 아이템
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
		// mouse가 가리키는 Item의 정보 표시
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
	// ItemNameList에 추가
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
// Draw ItemShadow : Item 그림자 출력하기	
//----------------------------------------------------------------------
// pSurface의 pPoint에 pItem을 출력한다.
//----------------------------------------------------------------------
void	
MTopView::DrawItemShadow(POINT* pPoint, MItem* pItem)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawItemShadow" );
	#endif

	__BEGIN_PROFILE("DrawItemShadow")

	//------------------------------------------------
	// Slayer인 경우 Darkness 안을 볼 수 없다.
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
	// 시체인 경우는 Creature를 출력한다.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// 일반 캐릭터 죽은 모습..
			//---------------------------------------------------------
			{
				// 선택된 것인 경우
				// Creature를 선택한 것처럼해서 출력.. 사기당. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

					if (
							(
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 성물
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// 성물
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// 성단
							|| pCreature->GetCreatureType() == 482	// 크리스마스 트리
							|| pCreature->GetCreatureType() == 650	// 게시판
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // 웨이 포인트1
							|| pCreature->GetCreatureType() == 731 // 웨이 포인트2
							|| pCreature->GetCreatureType() == 732 // 웨이 포인트3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// 틴버전일 경우에는 슬레이어시체 그림자 출력해줘야함
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
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 성물
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// 성물
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// 성물
							|| pCreature->GetCreatureType() == 482	// 크리스마스 트리
							|| pCreature->GetCreatureType() == 650  // 게시판
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // 웨이 포인트1
							|| pCreature->GetCreatureType() == 731 // 웨이 포인트2
							|| pCreature->GetCreatureType() == 732 // 웨이 포인트3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// 틴버전일 경우에는 슬레이어시체 그림자 출력해줘야함
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
// Draw ImageObject : 그림 출력하기	
//----------------------------------------------------------------------
// pSurface의 pPoint에 pImageObject을 출력한다.
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

	// m_SpritePack에서 적절한 sprite를 골라서 출력해준다.
	// Player를 가리는 그림일 경우 반투명 처리
	
	//if (pImageObject->GetImageObjectID() == m_BehindImageObjectID)

	//m_pSurface->HLine(410,310, 1, 0xFFFF);
	// (410,310)은 Player의 화면 상의 발위치..이다..
	// (g_x,g_y)는 Mouse좌표를 기준으로....

	//------------------------------------------------------
	// 1. ImageObject가 투명이 되는 것인가?
	// 2. Viewpoint가 Player위치보다 아래쪽에 있는가?
	// 3. Sprite가 Player를 가리는가?
	//------------------------------------------------------
	// 1,2,3이 모두 만족한 경우.. 반투명 처리한다.
	//------------------------------------------------------
	bool bTrans;

	// player좌표
	//POINT m_FilterPosition;// = MapToPixel(g_pPlayer->GetX(), g_pPlayer->GetY());
	//m_FilterPosition.x -= m_FirstZonePixel.x-m_PlusPoint.x;
	//m_FilterPosition.y -= m_FirstZonePixel.y-m_PlusPoint.y;

	// 그림의 좌표를 현재 화면의 좌표에 맞추기								
	//m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - filter.GetWidth();
	//m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - filter.GetHeight();
	//m_FilterPosition.x = 310;	// 410 - 100;
	//m_FilterPosition.y = 116;	// 204 - 100;
	
	//------------------------------------------------------
	// 전체적으로 어두운 값을 지정한다.
	// 단, ImageObject의 좌표(Zone에서의 Sector좌표)가 
	// 밝은 Tile이면(Light가 0이 아닌 경우) DarkBits=0으로 한다.
	//------------------------------------------------------
	//BYTE DarkBits;
	
	//------------------------------------------------------
	// [ TEST CODE ]
	//------------------------------------------------------
		// 이거는 필요없는 code이다.
		// MapEditor에서 잘 만들어주면 된다.
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
	
	// 3D 가속이 되는 경우.. 무조건 원래 색깔을 그대로 출력한다.
	if (CDirect3D::IsHAL())
	{
		DarkBits = 0;
	}
	*/

	//----------------------------------------------------				
	// 건물은 무조건 반투명
	//----------------------------------------------------				
	BOOL bBlendingShadow = 1;//g_pUserOption->BlendingShadow;
	
	//------------------------------------------------------
	//
	//  ShadowObject 출력
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
	//          ShadowAnimationObject 출력
	//
	//------------------------------------------------------
	else if (pImageObject->GetObjectType()==MObject::TYPE_SHADOWANIMATIONOBJECT)
	{
	}
	//------------------------------------------------------
	//
	//          InteractionObject 출력
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
//				// 좌표 보정
//				pPoint->x += Frame.GetCX();
//				pPoint->y += Frame.GetCY();
//
//				CSprite* pSprite = &m_InteractionObjectSPK[ sprite ];		
//				
//				//---------------------------------------- 		
//				// InteractionObject의 선택 사각형 영역 설정
//				//---------------------------------------- 	
//				RECT rect;
//				rect.left	= pPoint->x;
//				rect.top	= pPoint->y;
//				rect.right	= rect.left + pSprite->GetWidth();
//				rect.bottom = rect.top + pSprite->GetHeight();
//				pInteractionObject->SetScreenRect( &rect );
//
//
//				// 반투명 check				
//				bTrans = pImageObject->IsTrans();
//
//				//--------------------------------
//				// 선택된 InteractionObject일 경우...
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
//				// 투명 처리
//				//--------------------------------
//				else if (bTrans)
//				{
//					// 반투명
//					//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
//					// 그림에 맞춘 filter 좌표 보정
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
//				// 그냥 출력
//				//--------------------------------
//				else
//				{
//					//if (m_ImageObjectSPK[ sprite ].IsInit())
//					{
//						///*
//						// Player의 시야의 크기를 반영해서
//						// 시야 안에 있는 ImageObject들만 보이게 한다.
//						//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];
//
//						//int sight = g_pPlayer->GetMaxLightSight();
//						//int pX = sight * TILE_X;
//						//int pY = sight * TILE_Y;
//						//int sX = 410-(pX>>1) - pPoint->x;
//						//int sY = 270-(pY>>1) - pPoint->y;			
//
//						//if (DarkBits==0 //|| m_ImageObjectSPK[ pImageObject->GetSpriteID() ].IsIntersectFilter())
//							// 출력 시작위치(s_X,s_Y)가 Sprite의 끝점보다는 적을 경우
//						//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
//							// Filter의 끝점이 Sprite 첫점 보다 큰 경우
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
//				// 선택된 InteractionObject일 경우...
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
		//          ImageObject : Sprite 출력
		//
		//------------------------------------------------------
		int spriteID = pImageObject->GetSpriteID();
		if (spriteID != SPRITEID_NULL)	
		{			
			//---------------------------------------
			// ID가 sprite인 ImageObject를 Load한다.
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
//				// Loading에 성공한 경우
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
//				// 실패한 경우 --> 이미 Loading하고 있는 경우이다.				
//				//--------------------------------------------------
//				/*
//				// 2001.8.20 주석처리
//				else
//				{
//					#ifdef	OUTPUT_DEBUG
//						strcat(str, " ...Fail & Wait Loading");
//	
//						DEBUG_ADD( str );
//					#endif
//
//					// file thread 순위를 높힌다.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//					MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_ImageObjectSPKI[spriteID]);
//					pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
//					pNode->SetType( 1 );
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//					g_pLoadingThread->AddFirst( pNode );
//
//					// Thread에서 Loading이 끝날때까지 기다린다.
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
//					// file thread 순위를 낮춘다.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//
//				}
//				*/
//			}

			bTrans = pImageObject->IsTrans()
						&& pImageObject->GetViewpoint() >= g_pPlayer->GetY()
						// 2001.9.5 추가
						&& pImageObject->IsWallTransPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

						
						//&& m_ImageObjectSPK[ spriteID ].IsColorPixel(
						//	410 - pPoint->x,
						//	310 - pPoint->y);
		
			//--------------------------------
			// 투명 처리
			//--------------------------------
			if (bTrans)
			{	
				///*
				//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
			

				// 그림에 맞춘 filter 좌표 보정
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

						// Texture 설정
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());

							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );   
							
							// 좌표 설정
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
							// CTRL+A누르면 암거도 안 찍는다.
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

					// Texture만들어서 반투명 하기... 
					/*
					if (CDirect3D::IsHAL())
					{
						m_pSurface->Unlock();

						// Texture 설정
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);
						CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
				
						// 좌표 설정
						m_SpriteVertices[0].sx = pPoint->x;
						m_SpriteVertices[0].sy = pPoint->y;
						m_SpriteVertices[1].sx = pPoint->x,m_pImageObjectTextureManager->GetWidth(spriteID);
						m_SpriteVertices[1].sy = pPoint->y;
						m_SpriteVertices[2].sx = pPoint->x;
						m_SpriteVertices[2].sy = pPoint->y,m_pImageObjectTextureManager->GetHeight(spriteID);
						m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
						m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
						
						// 출력
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

						// Texture 설정
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
							
							// 좌표 설정
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
			// 그냥 출력
			//--------------------------------
			else
			{					
				//if (m_ImageObjectSPK[ spriteID ].IsInit())
				{
					///*
					// Player의 시야의 크기를 반영해서
					// 시야 안에 있는 ImageObject들만 보이게 한다.
					//CSprite& spriteRef = m_ImageObjectSPK[ spriteID ];

					//int sight = g_pPlayer->GetMaxLightSight();
					//int pX = sight * TILE_X;
					//int pY = sight * TILE_Y;
					//int sX = 410-(pX>>1) - pPoint->x;
					//int sY = 270-(pY>>1) - pPoint->y;			

					//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
						// 출력 시작위치(s_X,s_Y)가 Sprite의 끝점보다는 적을 경우
					//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
						// Filter의 끝점이 Sprite 첫점 보다 큰 경우
					//	 &&	sX,pX > 0 && sY,pY > 0))
						
					//{
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						// CTRL+A누르면 암거도 안 찍는다.
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
		//     AnimationObject인 경우엔 Frame도 출력해줘야 한다.
		//
		//------------------------------------------------------------
		if (pImageObject->IsAnimation())
		{
			MAnimationObject* pAnimationObject = (MAnimationObject*)pImageObject;

			TYPE_SPRITEID sprite;

			POINT pointTemp = *pPoint;

			/*******************그림자 출력 시작******************/ 
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
			/*******************그림자 출력 끝******************/ 

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
					// ID가 sprite인 ImageObject를 Load한다.
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
//						// Loading에 성공한 경우
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
//						// 실패한 경우 --> 이미 Loading하고 있는 경우이다.				
//						//--------------------------------------------------
//						
////						// 2001.8.20 주석처리
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
////							// file thread 순위를 높힌다.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
////							MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(sprite, m_ImageObjectSPKI[sprite]);
////							pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
////							pNode->SetType( 1 );
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
////							g_pLoadingThread->AddFirst( pNode );
////
////							// Thread에서 Loading이 끝날때까지 기다린다.
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
////							// file thread 순위를 낮춘다.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
////
////						}
//						
//					}
					// 좌표 보정
					pPoint->x += Frame.GetCX();
					pPoint->y += Frame.GetCY();

					// spriteID가 없는 경우는 반투명 다시 check				
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
					// 투명 처리
					//--------------------------------
					if (bTrans)
					{
						// 반투명
						//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
						// 그림에 맞춘 filter 좌표 보정
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
					// 그냥 출력
					//--------------------------------
					else
					{
						///*
						// Player의 시야의 크기를 반영해서
						// 시야 안에 있는 ImageObject들만 보이게 한다.
						//if (m_ImageObjectSPK[ sprite ].IsInit())
						{
							//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];

							//int sight = g_pPlayer->GetMaxLightSight();
							//int pX = sight * TILE_X;
							//int pY = sight * TILE_Y;
							//int sX = 410-(pX>>1) - pPoint->x;
							//int sY = 270-(pY>>1) - pPoint->y;			

							//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
								// 출력 시작위치(s_X,s_Y)가 Sprite의 끝점보다는 적을 경우
							//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
								// Filter의 끝점이 Sprite 첫점 보다 큰 경우
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
						
						// 좌표 보정
						pPoint->x += Frame.GetCX();
						pPoint->y += Frame.GetCY();
						
						DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
						//-------------------------------------------------------
						// H/W 가속이 되는 경우이면...
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
//								// 원래대로
//							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//							#endif
							
							
							//------------------------------------------------
							// LightFilter를 추가한다.
							//------------------------------------------------
							AddLightFilter3D( pPoint->x + TILE_X_HALF, 
								pPoint->y + TILE_Y_HALF, 
								Frame.GetLight(),			// 빛의 밝기
								false);		// false = screen좌표
							
						}
						//-------------------------------------------------------
						// 2D 출력
						//-------------------------------------------------------
						else
						{
//							DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
								AddLightFilter2D( pPoint->x + TILE_X_HALF, 
								pPoint->y + TILE_Y_HALF, 
								Frame.GetLight(),			// 빛의 밝기
								false);		// false = screen좌표
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
							// Frame 좌표 보정
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
							// H/W 가속이 되는 경우이면...
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
//									// 원래대로
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
								
								//------------------------------------------------
								// LightFilter를 추가한다.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);		// false = screen좌표
								
							}
							//-------------------------------------------------------
							// H/W 가속이 안되는 경우이면...
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
									
									AddLightFilter2D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);		// false = screen좌표
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
	// ImageObject ID 출력
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
// 3D이고 lock돼 있으면 Unlock으로...
// 3D아니고 lock안 돼 있으면 lock으로..
//
// return값은 원래 Lock상태
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
			// 3D이고 lock돼 있으면 Unlock으로...
			m_pSurface->Unlock();
		}
	}
	else
	{	
		if (!bLock)
		{
			// 3D아니고 lock안 돼 있으면 lock으로..
			m_pSurface->Lock();
		}
	}

	return bLock;
}
*/

//----------------------------------------------------------------
// SurfaceLock
//----------------------------------------------------------------
// return값은 원래의 lock상태
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
// 원래의 Lock상태로 돌려준다.
//----------------------------------------------------------------
/*
void
MTopView::SurfaceLockRestore( BOOL bOldLock )
{
	if (bOldLock)
	{
		if (!m_pSurface->IsLock())
		{
			// 원래 Lock이었는데 Lock이 아니면
			m_pSurface->Lock();
		}
	}
	else
	{
		if (m_pSurface->IsLock())
		{
			// 원래 Lock이 아니었는데 Lock이면..
			m_pSurface->Unlock();
		}
	}
}
*/

//----------------------------------------------------------------
// Effect 출력
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
		// Tile단위의 출력 좌표
		//------------------------------------------------------------------
		//if (pEffect->GetEffectType()==MEffect::EFFECT_SECTOR)
		//{			
		//	point = *pPoint;
		//}
		//------------------------------------------------------------------
		// Screen 출력 좌표
		//------------------------------------------------------------------
		//else 
		if (pEffect->GetEffectType()==MEffect::EFFECT_SCREEN)
		{			
			MScreenEffect* pScreenEffect = (MScreenEffect*)pEffect;
			//MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );	// 한번만 해주면 되는데.. 음냐.
			point.x = pScreenEffect->GetScreenX();
			point.y = pScreenEffect->GetScreenY();
		}
		//------------------------------------------------------------------
		// Pixel단위의 출력 좌표
		//------------------------------------------------------------------
		else
		{
			//MMovingEffect* pMovingEffect = (MMovingEffect* const)pEffect;
			
			// Pixel 단위의 좌표를 화면의 좌표로 바꿔준다.
			point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
			point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;
		}

		//------------------------------------------------------------------
		// 출력
		//------------------------------------------------------------------
		DrawEffect( &point, pEffect, pEffect->IsSelectable() );		

		// 다음 Effect
		iEffect++;
	}	
	
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawEffect" );
	#endif

	
}


//----------------------------------------------------------------
// Draw GroundEffect
//----------------------------------------------------------------
// Tile에 바로 붙어 있는 GroundEffect를 출력
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
		// Pixel 단위의 좌표를 화면의 좌표로 바꿔준다.
		// 중심점
		point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
		point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;			
		
		pointTemp = point;

		//------------------------------------------------------------------
		// 출력
		//------------------------------------------------------------------
		DrawEffect( &pointTemp, pEffect, pEffect->IsSelectable());
		
		//------------------------------------------------------------------
		// 선택되는 Effect인 경우
		//------------------------------------------------------------------
		if (pEffect->IsSelectable()
			&& m_SelectEffectID==pEffect->GetID())
		{
			MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();

			if (pEffectTarget!=NULL)
			{
				//------------------------------------------------------------------
				// Portal인 경우
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
		// 다음 Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawGroundEffect" );
	#endif

	
}

//----------------------------------------------------------------
// Draw AttachEffect
//----------------------------------------------------------------
// Effect 출력
// [하드코딩의 진수] - -;;
// type - 0 - 무조건 출력
//        1 - fore ground만 출력
//        2 - back ground만 출력
//----------------------------------------------------------------
// 일단..
// PairFrame은 AttachEffect에서만 출력한다.. 
// ActionFrame도 AttachEffect에서만 출력한다..
// NormalEffect에는 ActionFrame을 무시했다. 아직은 찍을일도 없고 귀찮고 해서 --;; 
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
		// Sprite로 표현하는 Effect인 경우만 출력한다.
		// Attach가 아니거나.. Attach이더라도 Sprite로 출력하는 경우
		//----------------------------------------------------------------		
		if (pEffect->GetEffectType()!=MEffect::EFFECT_ATTACH ||
			pEffect->GetEffectType()==MEffect::EFFECT_ATTACH && ((MAttachEffect*)pEffect)->IsEffectSprite()
			
		
			)
		{
			int direction = pEffect->GetDirection();
			int frameID = pEffect->GetFrameID();
			int frame = pEffect->GetFrame();


			/////////////////////////////////////////////////////////////////////
			// 2005. 11. 14. by chyaya
			// 할루시네이션에 걸렸을 경우 캐릭터 식별을 할 수 없게 하기 위해 
			// 마스터 이팩트를 제거한다

			if( g_pPlayer->GetID() != pCreature->GetID() &&
				g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION) &&
				 ( (frameID >= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_VAMPIRE_100		&&
				    frameID <= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_OUSTERS_150_NEW )		||
				   (frameID >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR		&&
				    frameID <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_HOPE )	||
				   (frameID >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_COLOR5	&&
				    frameID <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_ADVANCE_COLOR5)		||

					(frameID >= EFFECTSPRITETYPE_ADVANCEMENT_AURA_CRYSTAL_VAMPIRE	&&
				    frameID <= EFFECTSPRITETYPE_ADVANCEMENT_AURA_CRYSTAL_OUSTER)			||
					(frameID >= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK	&&
				    frameID <= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER)) ) 
			{
				iEffect++;
				continue;
			}

		

			//
			/////////////////////////////////////////////////////////////////////

			// 2004, 9, 14, sobeit add start - 인스톨 터렛일때 이펙트 안보여줌
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
			// 2004, 9, 14, sobeit add end - 인스톨 터렛일때 이펙트 안보여줌
//			if( pEffect->GetBltType() == BLT_EFFECT &&
//				frameID >= EFFECTSPRITETYPE_PROTECTION_FROM_BLOOD_ING &&
//				frameID <= EFFECTSPRITETYPE_PROTECTION_FROM_CURSE_ING			
//				)
//			{
//				// 해당 이펙트는 나 또는 우리 파티원만 보여야 한다.
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

					// action에 맞는 effectFrameID를 골라주는 거당.
					int aest = GET_ACTION_EFFECTSPRITETYPE(est);
					const bool bBack = (*g_pEffectSpriteTypeTable)[est].bPairFrameBack;
					int sest = GET_EFFECTSPRITETYPE_EFFECT( frameID );
					
					if (aest!=FRAMEID_NULL)
					{
						// FRAMEID_NULL인걸 체크한다. min( , ) -_-;
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

					// 지금은 하드코딩 되어 있지만, 따로 이펙트 스프라이트타입과 동작 프레임간의 프레임 동기화
					// 관련 기능이 추가되면, 하드코딩을 막을 수 있을 것이다. 
					// 동기화 기능이 추가된다면, 캐릭터에 해당 이펙트가 붙는 동작을 세팅할 수 있어야 겠지..
					
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
							// 다른 동작이면 잘 알아서 넘기자.
							iEffect++;
							continue;
						}
					}
					
					EFFECTFRAME_ARRAY &EFA = m_EffectAlphaFPK[frameID][direction];
					
					frame = min( frame, EFA.GetSize()-1 );					

					CEffectFrame &Frame = EFA[frame];

					bool bFrameBackground = Frame.IsBackground();
					AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);
				
					// 언제 출력되는건지 체크함 해준다.
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

							// 언제 출력되는건지 체크함 해준다.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 좌표 보정
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
									// H/W 가속이 되는 경우이면...
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
//											// 원래대로
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter를 추가한다.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + TILE_X_HALF, 
														pPoint->y + TILE_Y_HALF, 
														Frame.GetLight(),			// 빛의 밝기
														false);		// false = screen좌표
									
									}
									//-------------------------------------------------------
									// H/W 가속이 안되는 경우이면...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + TILE_X_HALF, 
													pPoint->y + TILE_Y_HALF, 
													Frame.GetLight(),			// 빛의 밝기
													false);		// false = screen좌표
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
							// Frame 좌표 보정
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
							// H/W 가속이 되는 경우이면...
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
//									// 원래대로
//									CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//								#endif
					

								//------------------------------------------------
								// LightFilter를 추가한다.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + TILE_X_HALF, 
												pPoint->y + TILE_Y_HALF, 
												Frame.GetLight(),			// 빛의 밝기
												false);		// false = screen좌표
							
							}
							//-------------------------------------------------------
							// H/W 가속이 안되는 경우이면...
							//-------------------------------------------------------
							else
							{
								DRAW_ALPHASPRITEPAL(&pointTemp, 
													sprite, 
													m_EffectAlphaSPK, 
													m_EffectAlphaPPK[frameID])
//													m_EffectAlphaSPKI, 
//													m_EffectAlphaSPKFile)

								AddLightFilter2D( pPoint->x + TILE_X_HALF, 
											pPoint->y + TILE_Y_HALF, 
											Frame.GetLight(),			// 빛의 밝기
											false);		// false = screen좌표
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame 출력
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

							// 언제 출력되는건지 체크함 해준다.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 좌표 보정
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
									// H/W 가속이 되는 경우이면...
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
//											// 원래대로
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter를 추가한다.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + TILE_X_HALF, 
														pPoint->y + TILE_Y_HALF, 
														Frame.GetLight(),			// 빛의 밝기
														false);		// false = screen좌표
									
									}
									//-------------------------------------------------------
									// H/W 가속이 안되는 경우이면...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + TILE_X_HALF, 
													pPoint->y + TILE_Y_HALF, 
													Frame.GetLight(),			// 빛의 밝기
													false);		// false = screen좌표
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

					// action에 맞는 effectFrameID를 골라주는 거당.
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

					// 언제 출력되는건지 체크함 해준다.
					if (type==0 
						|| type==1 && !bFrameBackground
						|| type==2 && bFrameBackground)
					{
						int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

						if (sprite!=SPRITEID_NULL)
						{
							// Frame 좌표 보정
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
							// H/W 가속이 되는 경우이면...
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
//								// 원래대로
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
								//------------------------------------------------
								// LightFilter를 추가한다.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + TILE_X_HALF, 
												pPoint->y + TILE_Y_HALF, 
												Frame.GetLight(),			// 빛의 밝기
												false);		// false = screen좌표
							
							}
							//-------------------------------------------------------
							// H/W 가속이 안되는 경우이면...
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

								AddLightFilter2D( pPoint->x + TILE_X_HALF, 
											pPoint->y + TILE_Y_HALF, 
											Frame.GetLight(),			// 빛의 밝기
											false);		// false = screen좌표
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame 출력
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

							// 언제 출력되는건지 체크함 해준다.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 좌표 보정
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
									// H/W 가속이 되는 경우이면...
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
//										// 원래대로
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
							
										//------------------------------------------------
										// LightFilter를 추가한다.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + TILE_X_HALF, 
														pPoint->y + TILE_Y_HALF, 
														Frame.GetLight(),			// 빛의 밝기
														false);		// false = screen좌표
									
									}
									//-------------------------------------------------------
									// H/W 가속이 안되는 경우이면...
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

										AddLightFilter2D( pPoint->x + TILE_X_HALF, 
													pPoint->y + TILE_Y_HALF, 
													Frame.GetLight(),			// 빛의 밝기
													false);		// false = screen좌표
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

		// 다음 Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawAttachEffect" );
	#endif

	
}

//----------------------------------------------------------------
// (x,y)에 m_pZone의 Minimap을 그린다.
//----------------------------------------------------------------
// Test Function
//----------------------------------------------------------------
void
MTopView::DrawMinimap(int x, int y, BYTE scale)
{
	
	//------------------------------------------------
	// vampire인 경우는 출력안한다.
	//------------------------------------------------
	//if (g_pPlayer->IsVampire())
	//{
	//	return;
	//}

	//------------------------------------------------
	// 3D 가속
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
//			// Texture 설정
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
//			// Player위치에 점찍기
//			//------------------------------------------------
////			if (!m_pSurface->Lock())
////				return;
////
////			POINT point = { x, y };
////
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// Player의 Minimap상의 좌표
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// 점 찍기
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
//			// Player위치에 점찍기
//			//------------------------------------------------	
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// Player의 Minimap상의 좌표
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// 점 찍기
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
	// 그림 출력
	m_pSurface->Lock();

	POINT point = { x, y };

	// Minimap 출력
	m_pSurface->BltSprite( &point, m_pMinimapSPR );

	int width = m_pMinimapSPR->GetWidth();
	int height = m_pMinimapSPR->GetHeight();

	WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
	long	lPitch		= m_pSurface->GetSurfacePitch();

	// Player의 Minimap상의 좌표
	point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
	point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;

	// 점 찍기
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
			// player위치
			if (pX==j && pY==i)
			{
				color = 0xFFFF;
			}			
			else
			{				
				// tile색깔별로..
				
				//switch (m_pZone->GetSector(j,i).GetSpriteID())
				//{
				//	case 0 :	color = 0x07E0;	break;	// G
				//	case 1 :	color = 0xC2E5;	break;	// ?
				//	case 2 :	color = 0x001F;	break;	// B
				//	default :	color = 0x18E3;	break;	// ,black
				//}
				
				 const MSector &sector = m_pZone->GetSector(j,i);
			
				// 그림
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
			
			// 다음 점
			lpSurfaceTemp = (WORD*)((BYTE*)lpSurfaceTemp + scale*4);
		}

		// 다음 줄
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
		MAX_GEAR에 -10 한건 core zap+bloodbible 갯수 만큼 뺀거..^^;
	*/
	// 부서질려는 item출력
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
	// 부서질려는 item이 있다면 출력한다.
	//----------------------------------------------------------------
	if (pGear->HasBrokenItem() )
	{
		//---------------------------------------------
		// Lock
		//---------------------------------------------
		m_pSurface->Lock();

		//---------------------------------------------
		// 기본 껍데기(-_-;) 출력
		//---------------------------------------------
		POINT pointBasis = { x+15, y };
		POINT point = { pointBasis.x, pointBasis.y };

		m_pSurface->BltSprite( &point, &m_ItemBrokenSPK[spriteID] );
		

		int size = pGear->GetSize();

		//---------------------------------------------
		// Gear의 모든 slot에 대해서 체크
		//---------------------------------------------
		for (int i=0; i<size-4-6; i++)
		{			
			const MItem* pItem = pGear->GetItem( (BYTE)i );	
			
			//---------------------------------------------
			// 뭔가 착용하고 있을때만 출력한다.
			//---------------------------------------------
			if (pItem!=NULL)
			{			
				MPlayerGear::ITEM_STATUS itemStatus = pGear->GetItemStatus( i );

				//---------------------------------------------
				// 무기인 경우.. 종류에 따라서 그림이 다르다.
				//---------------------------------------------
				int frameID;

				if (g_pPlayer->IsSlayer())
				{
					//---------------------------------------------
					// 왼손인데.
					//---------------------------------------------
					if (i==MSlayerGear::GEAR_SLAYER_LEFTHAND)
					{
						// 방패이면 출력
						if (pItem->GetItemClass()==ITEM_CLASS_SHIELD)
						{
							frameID = i;
						}
						// 방패가 아니면.. 출력 안한다.
						else
						{
							continue;
						}
					}
					//---------------------------------------------
					// 오른손은 무기에 따라서 출력
					//---------------------------------------------
					else if (i==MSlayerGear::GEAR_SLAYER_RIGHTHAND)
					{				
						// 총?				
						if (pItem->IsGunItem())
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER + 1 - 12;
						}
						// 십자가?
						else if (pItem->GetItemClass()==ITEM_CLASS_CROSS
								|| pItem->GetItemClass()==ITEM_CLASS_MACE)
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER -12;
						}
						// 아니믄.. 칼이나 도
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
				// 정상적이면 정상적인거 출력..
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
				// 부서져 가는 것이면.. 색깔~있게 출력..
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
// m_pSurface->IsLock() 상태에서 호출된다고 가정한다.
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
				// 이펙트 선택 사각형 영역 설정
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
				// 이펙트 선택 사각형 영역 설정
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
				// H/W 가속이 되는 경우이면...
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
//						// 원래대로
//						CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//					#endif
					
					//---------------------------------------- 		
					// 이펙트 선택 사각형 영역 설정
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
					// LightFilter를 추가한다.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);	// false = screen좌표					
				
				}
				//-------------------------------------------------------
				// H/W 가속이 안되는 경우이면...
				//-------------------------------------------------------
				else
				{
//					DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[pEffect->GetFrameID()])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)

//					//---------------------------------------- 		
//					// 이펙트 선택 사각형 영역 설정
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


					AddLightFilter2D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);		// false = screen좌표
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
				// 이펙트 선택 사각형 영역 설정
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
				// H/W 가속이 되는 경우이면...
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
//					// 원래대로
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
					//---------------------------------------- 		
					// 이펙트 선택 사각형 영역 설정
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
					// LightFilter를 추가한다.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);	// false = screen좌표					
				
				}
				//-------------------------------------------------------
				// H/W 가속이 안되는 경우이면...
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
//					// 이펙트 선택 사각형 영역 설정
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


					AddLightFilter2D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);		// false = screen좌표
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
				// H/W 가속이 되는 경우이면...
				//-------------------------------------------------------
				if (CDirect3D::IsHAL())
				{
//					DRAW_TEXTURE_SPRITE_LOCKED(point.x, point.y, spriteID, m_pEffectShadowManager)		

					//------------------------------------------------
					// LightFilter를 추가한다.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + TILE_X_HALF, 
									pPoint->y + TILE_Y_HALF, 
									Frame.GetLight(),			// 빛의 밝기
									false);	// false = screen좌표					
				
				}
				//-------------------------------------------------------
				// H/W 가속이 안되는 경우이면...
				//-------------------------------------------------------
				else
				{
//					CShadowSprite* pSprite = &m_EffectShadowSPK[spriteID];
//
//					if (pSprite->IsInit())
//					{
//						m_pSurface->BltShadowSpriteDarkness(&point, pSprite, 1 );
//						
						AddLightFilter2D( pPoint->x + TILE_X_HALF, 
										pPoint->y + TILE_Y_HALF, 
										Frame.GetLight(),			// 빛의 밝기
										false);		// false = screen좌표
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
		// 블러드 워록 그림.. 
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

	// spriteID가 sprite인 몬스터들 중에서 하나를 선택한다.
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
		
		g_PrintColorStrOut(point->x + TILE_X_HALF - g_GetStringWidth(str, gpC_base->m_chatting_pi.hfont)/2, py, str, gpC_base->m_chatting_pi, color, RGB_BLACK);

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
	- 공성전 시 캐릭터 위에 길드 마크를 보여준다.
	- 선택 여부와 상관없이 항상 보인다.
	- g_pPlayer가 공성전 참가중일 때만 공성전 참가중인 다른 크리쳐의 길드마크 표시
//----------------------------------------------------------------------*/
void		
MTopView::DrawGuildMarkInSiegeWar(MCreature* pCreature, int YPos)
{	
	// EFFECTSTATUS_GHOST 에 걸려있으면 남도 못보고 나도 못본다
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	//-----------------------------------------------------
	// Hallu에 걸리면 길드마크나 계급마크 안찍음
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
		// Guild가 있는 경우 - Guild Mark 출력
		//-----------------------------------------------------
		int guildID = pCreature->GetGuildNumber();
			
		if (guildID > 0)
		{
			//-------------------------------------------------
			// load되어 있는지 본다.
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
					// file에 있는지 본다.
					//-------------------------------------------------
					g_pGuildMarkManager->LoadGuildMark(guildID);
					
					//-------------------------------------------------
					// file에서 load되었는지 다시 체크
					//-------------------------------------------------
					pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
					
					//-------------------------------------------------
					// file에도 없는 경우..
					// guildMark관리서버?에서 받아온다.
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
// Unlock 상태에서 불려지는 함수이다.
//
// 선택된 캐릭터 이름을 출력한다.
// 이름에는 HP, 성향, 길드.. 등등의 정보가 포함된다.
// 서비스로(-_-;) RequestMode에 따른 icon도 출력한다.
//----------------------------------------------------------------------
void		
MTopView::DrawCreatureName(MCreature* pCreature)
{	
	// EFFECTSTATUS_GHOST 에 걸려있으면 남도 못보고 나도 못본다
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	// 2004, 08, 05 sobeit add start - 성문은 보여지면 이상하다?
	if( pCreature->GetCreatureType() >= 726 &&
		pCreature->GetCreatureType() <= 729)
		return;
	// 2004, 08, 05 sobeit add end - 성문 
	if(
		(g_pPlayer->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE) || 
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_FLARE )||
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_BLINDNESS))
		&& pCreature != g_pPlayer 
		)
	{
		// 거리에 따라서 출력 여부를 결정한다.
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
	// 땅 속에 있는 애는 이름도 없당. - -;
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
	// item 교환한다는 표시 출력
	//------------------------------------------------
	if (IsRequestMode())	
	{
		bool bRequest = false;

		if(pCreature->GetClassType() == MCreature::CLASS_FAKE)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)pCreature;
			if(pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// 펫이다
			{
				bRequest = true;
			}
		}
		//------------------------------------------------
		// 아직은 출력하면 안된다.
		//------------------------------------------------			
		else if (// NPC가 아니고
			!pCreature->IsNPC()
			// Player인 경우만
			//&& pCreature->GetCreatureType()<=CREATURETYPE_VAMPIRE_FEMALE
			//&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteType].IsPlayerOnlySprite()
			&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]].IsPlayerOnlySprite()
			// 관 속에 있는 경우
			&& !pCreature->IsInCasket()
			// 땅에 묻혀있을땐 거래 한다는 표시 안 나오게...
			//&& !pCreature->IsUndergroundCreature()
			// 살아있는.. - -;
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
			// trade 신청하는 아이콘
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
			// party 신청하는 아이콘
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
			// Info 신청하는 아이콘
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
	// 캐릭터 이름 출력
	//------------------------------------------------
	//gC_font.PrintStringNoConvert(&m_SurfaceInfo, pCreature->GetName(), pointTemp.x, pointTemp.y, 0xFFFF);	
	bool bMasterWords = pCreature->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
						|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR
						|| pCreature->GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR
						|| strncmp( pCreature->GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0;
//						|| (strstr(pCreature->GetName(), "운영자")!=NULL);

	bool bHalluName = (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION) || 
		!bMasterWords && 
		!g_pUserInformation->IsMaster && 
		(pCreature->GetClassType() == MCreature::CLASS_PLAYER || 
		pCreature->GetClassType() == MCreature::CLASS_CREATUREWEAR) && 
		g_pPlayer->GetRace() != pCreature->GetRace()) &&
		g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR;						// 챗 마스크를 씌울 경우에만 
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
	// NPC인 경우
	//-----------------------------------------------------
	if (pCreature->IsNPC())
	{
		// 바토리인 경우.. 하드코딩 - -;;
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
	// 운영자인 경우
	//-----------------------------------------------------
	else if (pCreature->GetCompetence()==0)
	{
		color = CDirectDraw::Color( 31, 23, 3 );	// 금색?
		font	= FONTID_NPC_NAME;
	}
	//-----------------------------------------------------
	// Hallu에 걸린 경우
	//-----------------------------------------------------
	else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
	{
		color	= m_ColorNameAlignment[1];
		font	= FONTID_VAMPIRE_NAME;
	}
	//-----------------------------------------------------
	// Slayer인 경우 
	//-----------------------------------------------------
	else if (g_pPlayer->IsSlayer())
	{
		//-----------------------------------------------------
		// 종족에 따라서 
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackTribe( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if (g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// 아니면.. 성향에 따른 표시
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
		// 아니면.. 성향에 따른 표시
		//-----------------------------------------------------
		else
		{
			int alignment = pCreature->GetAlignment();
			
			color = m_ColorNameAlignment[alignment];
			font	= FONTID_VAMPIRE_NAME;				
		}
	}
	//-----------------------------------------------------
	// 뱀파이어인 경우
	//-----------------------------------------------------
	else
	{
		//-----------------------------------------------------
		// 길드에 따라서
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackGuild( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if(g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// 아니면.. 성향에 따른 표시
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
		// Level Name이 있는 경우
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
		// 이름 밑에 깔릴 전체(이름) 크기의 박스
		//-----------------------------------------------------
		int rectLeft	= m_pointChatString.x + POSITION_HP_BAR -14 ;
		int rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
		int rectTop		= yPoint_20;
		int rectBottom	= yPoint_20 + g_pClientConfig->FONT_HEIGHT;

		//-----------------------------------------------------
		// Hallu에 걸리면 길드마크나 계급마크 안찍음
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
			// Guild가 있는 경우 - Guild Mark 출력
			//-----------------------------------------------------
			guildID = pCreature->GetGuildNumber();
			if(	g_pPlayer->GetRace() != pCreature->GetRace() && pCreature->IsPlayer() &&
				g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace == true
				)
				guildID = 0;
			
			// grade출력
			int gradeID = pCreature->GetGrade()-1;	// 1~50 으로 되있으므로 -1해서 0~49로 맞춘다
			// 플레이어만 계급 출력, 박쥐나 늑대는 안함
			if(!pCreature->IsPlayerOnly())
				gradeID = -1;
			
				/*
				if (guildID>=0)
				{
				// run-time loading도 괜찮을 듯..
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
				// load되어 있는지 본다.
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
						// file에 있는지 본다.
						//-------------------------------------------------
						g_pGuildMarkManager->LoadGuildMark(guildID);
						
						//-------------------------------------------------
						// file에서 load되었는지 다시 체크
						//-------------------------------------------------
						pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
						
						//-------------------------------------------------
						// file에도 없는 경우..
						// guildMark관리서버?에서 받아온다.
						//-------------------------------------------------
						//if (pSprite==NULL) {}
					}				
				}
			}
			
			// 계급이 있는경우 계급마크 출력
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
		// 펫인경우 레벨 마크 출력
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
		// HP계산
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
		// 땅속에 있는 애는 HP가 꽉 찬것처럼 보여준다.
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
		// 실제로 보여질 pixel계산
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP가 꽉 차게 표시되는 경우
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// 바탕색 깔 필요 없지만.. 이름 잘 보이게 할려고.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP 닳은 부분 표시
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP 만큼의 Box만 표시
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP 닳은 부분
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
		//		현재 HP만큼의 Box와 캐릭터 이름 출력
		//
		//-----------------------------------------------------
		// 현재 HP만큼의 Box와 캐릭터 이름 출력(까맣게)
		//-----------------------------------------------------	
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0이 아니면 box출력
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// 현재 HP에 대한 박스
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
				// 불투명한 HP bar를 출력할때
				static WORD notTransHPBar 
					= CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_R>>1,
											g_pClientConfig->COLOR_HP_BAR_G>>1,
											g_pClientConfig->COLOR_HP_BAR_B>>1);

				pNode->SetBox( rectHP, notTransHPBar, false );
			}			
		}

		//-----------------------------------------------------
		// 출력하게 설정..
		//-----------------------------------------------------
		AddText( pNode );			

		// 2004, 10, 28, sobeit add start -몬스터 킬 퀘스트 해당 몬스터에 표시.
//		if(pCreature->IsAlive() && UI_IsMonsterKillQuest_Monster((*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]))
//		{
//			WORD temp_color;
//			temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
//			color  = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
//		}
		// 2004, 10, 28, sobeit add end
		//-----------------------------------------------------
		// 이름 다시 찍기
		//-----------------------------------------------------
		DRAWTEXT_NODE* pNode2 = new DRAWTEXT_NODE (
									nameX,
									yPoint_20+4,
									pName,
									color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		AddText( pNode2 );
		// 2004, 6, 17, sobeit add start - about nick name - 같은 종족 일때만 닉네임 보이게..수정.
		if(pCreature->GetNickNameType() != NicknameInfo::NICK_NONE)// && (/*g_pPlayer->GetRace() == pCreature->GetRace()|| */pCreature->IsFakeCreature()))
		{
			if(yPoint-42<0) return; // 걍 찍지 말자..ㅎㅎ
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
				case NicknameInfo::NICK_QUEST_2:
					color = RGB(0,255,128);
					bgColor = CDirectDraw::Color(0,128,255);
					break;
				default:
					color = RGB_YELLOW;
					break;
				}
			//	if(guildID >0) // 길드마크가 안찍힐땐 닉네임을 hpbar+계급마크 중앙에 찍고
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // 그외엔 hpbar 중앙에 찍는다
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
					
					if(bType == NicknameInfo::NICK_QUEST_2 || bType == NicknameInfo::NICK_QUEST_2)
					{
						rectHP.left  -= 5 ;
						rectHP.right += 5 ;
					}

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
// 여기 있어야할 함수는 아니지만.. - -;
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
	// 고정된 burrow그림  -_-;; 
	//----------------------------------------
	POINT pointTemp;
	RECT rect;

	CSprite* pSprite = &m_EtcSPK[ SPRITEID_CREATURE_BURROW ];	
	
	//----------------------------------------
	// 좌표 보정
	//----------------------------------------
	int cx = 4;
	int cy = 4;
	pointTemp.x = pPoint->x + cx;
	pointTemp.y = pPoint->y + cy;
	
	//---------------------------------------- 		
	// 캐릭터 선택 사각형 영역 설정
	//---------------------------------------- 	
	rect.left	= pointTemp.x;
	rect.top	= pointTemp.y;
	rect.right	= rect.left + pSprite->GetWidth();
	rect.bottom = rect.top + pSprite->GetHeight();
	pCreature->SetScreenRect( &rect );				
	
	//---------------------------------------- 	
	// 선택된 경우
	//---------------------------------------- 	
	if (m_SelectCreatureID == pCreature->GetID() )
	{
		// SpriteOutlineManager에 추가
		
		// 땅속에 있는 애들은 무조건 vampire이다.
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
		// 이름 출력할 좌표 지정
		//---------------------------------------- 	
		const int FontHeight = g_pClientConfig->FONT_HEIGHT;
		const int FontHeight2 = FontHeight << 1;
		
		pointTemp.x = pPoint->x;			
		if (pointTemp.x<0) pointTemp.x=0;
		
		//---------------------------------------- 	
		// Level Name 찍을 위치도 계산
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
		// 그냥 이름만 찍을 때
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
	// 그냥 출력
	//----------------------------------------
	else
	{
		m_pSurface->BltSprite(&pointTemp, pSprite);
	}
	
	//----------------------------------------
	// 몸에 붙은 Effect 출력
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
	// EFFECTSTATUS_GHOST 에 걸려있으면 남도 못보고 나도 못본다
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
		// 이름 밑에 깔릴 전체(이름) 크기의 박스
		//-----------------------------------------------------
		{
			rectLeft	= g_pPlayer->GetPixelX() - m_FirstZonePixel.x + POSITION_HP_BAR - 14;
			rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
			rectTop		= yPoint_20;
			rectBottom	= yPoint_20+ g_pClientConfig->FONT_HEIGHT;
		}
		//-----------------------------------------------------
		// Hallu에 걸리면 길드마크나 계급마크 안찍음
		//-----------------------------------------------------
		if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
		{
			//-----------------------------------------------------
			// Guild가 있는 경우 - Guild Mark 출력
			//-----------------------------------------------------
			guildID = g_pPlayer->GetGuildNumber();

			gradeID = g_pPlayer->GetGrade()-1;	// 1~50 으로 되있으므로 -1해서 0~49로 맞춘다
			if(!g_pPlayer->IsPlayerOnly())
				gradeID = -1;
			
			if (guildID > 0)
			{
				//-------------------------------------------------
				// load되어 있는지 본다.
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
			
			// 계급이 있는경우 계급마크 출력
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
		// HP계산
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
		// 실제로 보여질 pixel계산
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP가 꽉 차게 표시되는 경우
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// 바탕색 깔 필요 없지만.. 이름 잘 보이게 할려고.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP 닳은 부분 표시
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP 만큼의 Box만 표시
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP 닳은 부분
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
		//		현재 HP만큼의 Box와 캐릭터 이름 출력
		//
		//-----------------------------------------------------
		// 현재 HP만큼의 Box와 캐릭터 이름 출력(까맣게)
		//-----------------------------------------------------			
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pCreatureName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0이 아니면 box출력
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// 현재 HP에 대한 박스
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
				// 불투명한 HP bar를 출력할때
				static WORD notTransHPBar 
					= CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_R>>1,
											g_pClientConfig->COLOR_HP_BAR_G>>1,
											g_pClientConfig->COLOR_HP_BAR_B>>1);

				pNode->SetBox( rectHP, notTransHPBar, false );
			}			
		}

		//-----------------------------------------------------
		// 출력하게 설정..
		//-----------------------------------------------------
		AddText( pNode );			


		//-----------------------------------------------------
		// 이름 다시 찍기
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
				case NicknameInfo::NICK_QUEST_2:
					color = RGB(0,255,128);
					bgColor = CDirectDraw::Color(0,128,255);
					break;
				default:
					color = RGB_YELLOW;
					break;
				}
			//	if(guildID >0) // 길드마크가 안찍힐땐 닉네임을 hpbar+계급마크 중앙에 찍고
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // 그외엔 hpbar 중앙에 찍는다
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

					if(bType == NicknameInfo::NICK_QUEST_2 || bType == NicknameInfo::NICK_QUEST_2)
					{
						rectHP.left  -= 5 ;
						rectHP.right += 5 ;
					}

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
	
	// 2005, 2, 18, sobeit add start - WebBrowser가 떠있을 때 처리
	if(UI_IsRunning_WebBrowser())
	{
		//bDrawBackGround = false; // 배경을 찍지 않는다. UI는 찍는다..
		int TempValue = UI_GetMouseCursorInfo_WebBrowser();

		if(TempValue == 1) // 커서가 웹 화면 안으로 들어옴
		{
			UI_ShowWindowCursor(); // 커서 보여주장..
		}
		else if( TempValue == -1) // 커서가 웹 화면 밖으로 나갔다.
		{
			UI_HiddenWindowCursor(); // 커서 안보여준다.
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
			ifstream	FinFile;
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
				
				const int scroll_x = 800/2-m_AdvacementQuestEnding[0].GetWidth()/2, scroll_y = 50;
				
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
				rt.right = min(rect.x+rect.w, 800-x);
				rt.bottom = min(rect.y+rect.h, 600-y);
				
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
		rect.right = CLIPSURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = CLIPSURFACE_HEIGHT;
		
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
			m_pSurface->FillSurface(0);	//	화면지우기
		}
		else if(event->parameter4 < EVENTBACKGROUNDID_MAX)
		{
//			AssertEventBackground(event->parameter4);
			
			POINT p = { 0, 0 };
			RECT r = {0, 0, 799, 599 };

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
			ifstream	FinFile;
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
					
					// Creature의 Action에 맞는 add-on을 출력한다.
					//action = pCreature->GetAction();
					
					WORD clothes;
					BYTE clothesType;

					for (int i=0; i<ADDON_MAX; i++)
					{
						// Creature의 현재 방향에 따라서...
						// 옷을 출력해주는 순서가 다를 수 있다.
						clothesType = MCreatureWear::s_AddonOrder[direction][i];
						
						// i번째 종류의 옷을 입고 있다면 출력해 준다.
						const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
						
						if (addonInfo.bAddon)
						{
							clothes = addonInfo.FrameID;
							
							FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
							
							// 있는 동작인 경우
							if (FA.GetSize() > frame)
							{
								CFrame &Frame = FA[frame];					
								int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
								int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
								int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
								
								
								CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
								
								// 복장Sprite가 초기화 되지 않은 경우
//									if (pSprite->IsNotInit())
//									{
//										LoadFromFileAddonSPK( clothes, action );
//									}
								
								
								POINT pointTemp;
								
								// 좌표 보정
								pointTemp.x = 384+cx;
								pointTemp.y = 312+cy;
								
								{
									int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
									if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
									{
										// 유니크 아이템이면
										//									if(colorSet2 == QUEST_ITEM_COLOR)
										//										colorSet2 = MItem::GetQuestItemColorset();
										//									else
										//										colorSet2 = MItem::GetUniqueItemColorset();										
										
										colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
										if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
											colorSet1 = colorSet2;
									}
									
									CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
									
									// 어둡게 찍기
									if (pCreature->IsFade())
									{
										m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
									}
									else
									{
										m_pSurface->BltIndexSprite(&pointTemp, pSprite);
									}
								}						
								
								// 정상적인 출력
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
						
						// 좌표 보정
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
				const int scroll_x = 800/2-m_OustersFinSPK[spriteID].GetWidth()/2, scroll_y = 50;
				
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
				rt.right = min(rect.x+rect.w, 800-x);
				rt.bottom = min(rect.y+rect.h, 600-y);
				
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
				rt.right = min(rect2.x+rect2.w, 800-x);
				rt.bottom = min(rect2.y+rect2.h, 600-y);
				
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
		rect.right = CLIPSURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = CLIPSURFACE_HEIGHT;
		
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