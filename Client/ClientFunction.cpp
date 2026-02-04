//---------------------------------------------------------------------------
// ClientFunction.cpp
//---------------------------------------------------------------------------
#include "Client_PCH.h"
#include "ClientFunction.h"
#include "SkillDef.h"
#include "MItemOptionTable.h"

// Forward declarations (common to all builds)
extern RECT g_GameRect;

#ifdef __GAME_CLIENT__
	#include "MTopView.h"
	#include "MInventory.h"
	#include "MEffectGeneratorTable.h"
	#include "MActionInfoTable.h"
	#include "MScreenEffectManager.h"
	#include "MScreenEffect.h"
	#include "UserInformation.h"
	#include "UIFunction.h"
	#include "DebugInfo.h"
	#include "MPlayer.h"
	#include "SpriteLib/SpriteLibBackend.h"  // For spritectl API
	#include "../VS_UI/src/header/VS_UI_Base.h"  // For gpC_base
	
	extern MScreenEffectManager* g_pInventoryEffectManager;

	extern bool   g_bZoneSafe;
	extern BOOL g_MyFull;
	extern RECT g_GameRect;
#else
	#include "VS_UI_Base.h"
	#include "CSoundPartManager.h"
	#include "MSoundTable.h"

	CSoundPartManager*		g_pSoundManager = NULL;

	extern HWND	g_hWnd;
#endif

#if !defined(_LIB) && !defined(__GAME_CLIENT__)
	
//-----------------------------------------------------------------------------
// 초기화 할 때,
//-----------------------------------------------------------------------------
void
InitSound()
{
	if (g_pSoundTable==NULL)
	{
		g_pSoundTable = new SOUND_TABLE;
	}

	//------------------------------------------------
	// Load SoundTable
	//------------------------------------------------
	std::ifstream soundTable("Data/Info/Sound.inf", std::ios::binary);
	g_pSoundTable->LoadFromFile(soundTable);
	soundTable.close();


	//------------------------------------------------
	// DXSound & SoundManager 초기화
	//------------------------------------------------
	if (g_SDLAudio.Init(g_hWnd))
	{
		g_pSoundManager = new CSoundPartManager;
		g_pSoundManager->Init( g_pSoundTable->GetSize(), 50 );	// 50개의 wav만 loading한다는 의미
	}
}

//-----------------------------------------------------------------------------
// UnInit Sound
//-----------------------------------------------------------------------------
void
UnInitSound()
{
	//-------------------------------------
	// UnInit DirectSound
	//-------------------------------------
	g_pSoundManager->Release();	

	g_SDLAudio.Release();
}

	//---------------------------------------------------------------------------
	// PlaySound
	//---------------------------------------------------------------------------
	// Client가 아닌 경우에 사용..
	//---------------------------------------------------------------------------	
	void		
	PlaySound(TYPE_SOUNDID soundID)
	{
		//-----------------------------------------------------------
		// 정의되지 않는 sound ID일 경우..
		//-----------------------------------------------------------
		if (!g_SDLAudio.IsInit() || soundID >= g_pSoundTable->GetSize())
			return;

		//-----------------------------------------------------------
		// 없으면 --> Load & Play
		//-----------------------------------------------------------
		if (g_pSoundManager->IsDataNULL(soundID))
		{
			// 다시 load						
			const char* filename = (*g_pSoundTable)[soundID].Filename.GetString();
 			LPDIRECTSOUNDBUFFER pBuffer = g_SDLAudio.LoadWav( (char*)filename );

			//-----------------------------------------------------------
			// Loading 실패
			//-----------------------------------------------------------
			if (pBuffer==NULL)
			{
			}
			//-----------------------------------------------------------
			// Load에 성공 했으면...
			//-----------------------------------------------------------
			else		
			{
				// Replace됐으면 원래것을 메모리에서 지운다.
#ifdef PLATFORM_WINDOWS
				LPDIRECTSOUNDBUFFER pOld;
				if (g_pSoundManager->SetData( soundID, pBuffer, pOld )!=0xFFFF)
				{
					pOld->Release();
				}
#else
				// SDL backend: SetData manages buffer lifecycle internally
				g_pSoundManager->SetData( soundID, pBuffer);
#endif

				// Play
				g_SDLAudio.Play( pBuffer, false );				
			}
		}
		//-----------------------------------------------------------
		// 있는 경우 --> Play
		//-----------------------------------------------------------
		else
		{
			LPDIRECTSOUNDBUFFER pBuffer;
			if (g_pSoundManager->GetData(soundID, pBuffer))
			{			
				g_SDLAudio.Play( pBuffer, false );
			}
		}
	}

	const char*
	GetWhisperID()
	{
		return NULL;
	}

	BOOL
	IsPlayerInSafePosition()
	{
		return FALSE;
	}

	bool
	IsPlayerInSafeZone()
	{
		return false;
	}
#else
	//---------------------------------------------------------------------------
	// Get Whisper ID
	//---------------------------------------------------------------------------
	const char*
	GetWhisperID()
	{
		return g_pUserInformation->WhisperID.GetString();
	}

	BOOL
	IsPlayerInSafePosition()
	{
		if (g_pPlayer!=NULL)
		{
			return g_pPlayer->IsInSafeSector();
		}
		
		return false;
	}

	bool
	IsPlayerInSafeZone()
	{
		return g_bZoneSafe;
	}
#endif

//---------------------------------------------------------------------------
// DrawInventoryEffect
//---------------------------------------------------------------------------
// InventoryEffect들을 그려준다.
//---------------------------------------------------------------------------
void
DrawInventoryEffect()
{
	#ifdef __GAME_CLIENT__
		// 현재 inventory의 첫 좌표			
		POINT point = UI_GetInventoryPosition();
		
		// TODO: [SDL_BACKEND] DrawInventoryEffect not implemented for SDL backend
		// g_pTopView->DrawInventoryEffect(&point);

	#endif
}

void 
DrawTitleEffect()
{
	#ifdef __GAME_CLIENT__
		// 현재 inventory의 첫 좌표			
		POINT point = 	{400,528};

		g_pTopView->DrawTitleEffect(&point);

	#endif
}

// 2004, 11, 22, sobeit add start
//--------------------------------------------------------------------
//	기어 창에 피의 성서 아이템 뒤에 뿌린다..
//  DrawInventoryEffect 처럼 generator 만들수도 있지만..특수한 경우기 때문에 무시
//-------------------------------------------------------------------
void 
DrawBloodBibleEffect_InGear(int X, int Y)
{
	#ifdef __GAME_CLIENT__
		// 현재 inventory의 첫 좌표			
		POINT point = 	{X,Y};

		g_pTopView->DrawBloodBibleEffect_InGear(&point);

	#endif
}
// 2004, 11, 22, sobeit add end
//---------------------------------------------------------------------------
// Add New_Inventory_Effect
//---------------------------------------------------------------------------
// 화면좌표(x,y)
//---------------------------------------------------------------------------
void
AddNewInventoryEffect(TYPE_OBJECTID id, TYPE_ACTIONINFO ai, DWORD delayFrame, DWORD value)
{
	#ifdef __GAME_CLIENT__
	
		DEBUG_ADD("AddNewInventoryEffect");

		//---------------------------------------------------------
		// 기술 설정이 잘못된 경우
		//---------------------------------------------------------
		if (ai>=g_pActionInfoTable->GetSize())
		{
			return;
		}

		// item의 grid 좌표
		int x, y;

		const MItem* pItem = g_pInventory->GetItemToModify( id );
		/*
		for (int i=0; i<8; i++)
		{
			const MItem* pCheckItem = g_pInventory->GetItem(i, 0);

			if (pCheckItem!=NULL)
			{
				if (pCheckItem->GetID()==id)
				{
					x = pCheckItem->GetGridX();
					y = pCheckItem->GetGridY();			

					break;
				}
			}
		}
		*/

		//---------------------------------------------------------
		// item이 없는 경우
		//---------------------------------------------------------
		if (pItem==NULL || !(*g_pActionInfoTable)[ai].IsTargetItem())
		{
			DEBUG_ADD("No such item or Not Target Item");
			return;
		}

		x = pItem->GetGridX();
		y = pItem->GetGridY();

		//---------------------------------------------------------
		// 기술의 연결 동작?들이 있는 경우에만...
		//---------------------------------------------------------
		if ((*g_pActionInfoTable)[ai].GetSize()!=0)
		{
			MEffectTarget* pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[ai].GetSize() );

			pEffectTarget->Set( x, y, 0, id );

			pEffectTarget->SetDelayFrame( delayFrame );

			//--------------------------------------------------------
			//
			//                   Effect생성		
			//
			//--------------------------------------------------------
			g_pEffectGeneratorTable->Generate(
					x,y,0,		// 시작 위치
					0, 			// 방향
					1,			// power
					ai,			//	ActionInfoTable종류,
					pEffectTarget		// 목표 정보
			);

			//------------------------------------------------------
			// 결과 생성
			//------------------------------------------------------
			MActionResult* pResult = new MActionResult;
			MActionResultNode* pActionResultNode = NULL;

			switch(ai)
			{
			case MAGIC_ENCHANT_OPTION_PLUS:
				{
					MItem* pInvenItem = g_pInventory->GetItem(id);

					if(pInvenItem != NULL)
					{
						pActionResultNode = new MActionResultNodeChangeItemOptionInInventory(id, value);
					}
				}
				break;

			case MAGIC_ENCHANT_OPTION_NULL:
				{
					MItem* pInvenItem = g_pInventory->GetItem(id);

					// 대상 아이템이 펫아이템인경우는 처리 안함
					if(pInvenItem != NULL)
					{
						if(pInvenItem->GetItemClass() != ITEM_CLASS_PET_ITEM)
						{
							pActionResultNode = new MActionResultNodeChangeItemOptionInInventory(id, value);
						}
						else
						{
							if(g_pPlayer->IsItemCheckBufferItemToItem())
							{
								MItem *pMouseItem = g_pPlayer->GetItemCheckBuffer();

								g_pPlayer->ClearItemCheckBuffer();

								if(pMouseItem != NULL)
									delete pMouseItem;
							}
						}
					}

				}
				break;

			case MAGIC_ENCHANT_REMOVE_ITEM:
				pActionResultNode = new MActionResultNodeRemoveItemInInventory(id);
				break;
			case MAGIC_TRANS_ITEM_OK :
				pActionResultNode = new MActionResultNodeChangeItemGenderInInventory( id );
				break;
			}

			if (pActionResultNode!=NULL)
			{
				pResult->Add( pActionResultNode );
				pEffectTarget->SetResult(pResult);
			}

			DEBUG_ADD_FORMAT("[AddNewInventoryEffect] ai=%d, item id=%d", ai, id);
		}
		//---------------------------------------------------------
		// 기술의 연결 동작이 없는 경우
		//---------------------------------------------------------
		else
		{
			//------------------------------------------------------------
			// 결과를 처리해야하는 시점인가? - 당연하다고 본다 *_*;
			//------------------------------------------------------------				
			// 음.. 결과는 어딨지.. - -;;
		}
	#endif
}


//---------------------------------------------------------------------------
// Draw AlphaBox (pRect,  (r,g,b),  alpha)
//---------------------------------------------------------------------------
// alpha가 커지면.. 더 진해지게 되어 있음..
//---------------------------------------------------------------------------
void
DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	// 클리핑
	RECT rect = *pRect;
	pRect = &rect;
	// add by Sonic 2006.9.26
//	if(g_MyFull)
//	{
//		if(pRect->left < 0)pRect->left = 0;
//		if(pRect->right > 1024-1)pRect->right = 1024-1;
//		if(pRect->top < 0)pRect->top = 0;
//		if(pRect->bottom > 768-1)pRect->bottom = 768-1;

//	}
//	else
//	{
		if(pRect->left < 0)pRect->left = 0;
		if(pRect->right > g_GameRect.right-1)pRect->right = g_GameRect.right-1;
		if(pRect->top < 0)pRect->top = 0;
		if(pRect->bottom > g_GameRect.bottom -1)pRect->bottom = g_GameRect.bottom-1;
//	}
	// end
	if(pRect->left >= pRect->right || pRect->top >= pRect->bottom)return;

	#ifdef __GAME_CLIENT__
		int reverseAlpha = 31-alpha;

		if (alpha<0) alpha = 0;

#ifdef PLATFORM_WINDOWS
		WORD color;
		//------------------------------------------------
		// Lock 상태로 만든다.
		//------------------------------------------------
		BOOL bUnlock = !gpC_base->m_p_DDSurface_back->IsLock();
		if (bUnlock)
		{
			gpC_base->m_p_DDSurface_back->Lock();
		}

		color = CSDLGraphics::Color(r,g,b);

		//-------------------------------------------------
		// 검정색이면.. 쉽게 된다~
		//-------------------------------------------------
		if (color==0)
		{
			// 2D 5:6:5
			if (CSDLGraphics::Is565())
			{
				gpC_base->m_p_DDSurface_back->GammaBox565(pRect, reverseAlpha);
			}
			// 2D 5:5:5
			else
			{
				gpC_base->m_p_DDSurface_back->GammaBox555(pRect, reverseAlpha);
			}
		}
		//-------------------------------------------------
		// 아니면...
		//-------------------------------------------------
		else
		{
			gpC_base->m_p_DDSurface_back->BltColorAlpha(pRect, color, reverseAlpha);
		}

		//------------------------------------------------
		// 원래의 Lock 상태로 되돌린다.
		//------------------------------------------------
		if (bUnlock)
		{
			gpC_base->m_p_DDSurface_back->Unlock();
		}
#else
		// SDL backend implementation for __GAME_CLIENT__
		// Need to use spritectl API directly since Lock() is a stub in SDL backend

		// Get the SDL surface
		spritectl_surface_t sdl_surface = (spritectl_surface_t)gpC_base->m_p_DDSurface_back->GetBackendSurface();
		if (!sdl_surface || sdl_surface == SPRITECTL_INVALID_SURFACE) {
			return;  // Invalid surface
		}

		// Lock the surface to get pixel access
		spritectl_surface_info_t surface_info;
		if (spritectl_lock_surface(sdl_surface, &surface_info) != 0) {
			return;  // Failed to lock
		}

		// Check if color is black (0,0,0) -> use gamma correction like Windows version
		WORD color = CSDLGraphics::Color(r, g, b);

		// Calculate clipping
		int startX = (pRect->left > 0) ? pRect->left : 0;
		int endX = (pRect->right < surface_info.width) ? pRect->right : surface_info.width;
		int startY = (pRect->top > 0) ? pRect->top : 0;
		int endY = (pRect->bottom < surface_info.height) ? pRect->bottom : surface_info.height;

		if (color == 0)
		{
			// Black color: use gamma correction (darken existing pixels)
			// Same algorithm as Windows GammaBox565

			// Gamma correction: multiply by reverseAlpha then shift right by 5
			// This darkens the area
			for (int y = startY; y < endY; y++)
			{
				WORD* pDest = (WORD*)((BYTE*)surface_info.pixels + y * surface_info.pitch) + startX;
				for (int x = startX; x < endX; x++)
				{
					WORD pixel = *pDest;

					// RGB565 format: RRRR RGGG GGGB BBBB
					// Apply gamma correction separately to each channel
					WORD r = (pixel >> 11) & 0x1F;
					WORD g = (pixel >> 5) & 0x3F;
					WORD b = pixel & 0x1F;

					// Multiply by reverseAlpha and shift right by 5
					// This is equivalent to: value = value * reverseAlpha / 32
					r = (r * reverseAlpha) >> 5;
					g = (g * reverseAlpha) >> 5;
					b = (b * reverseAlpha) >> 5;

					// Clamp and recombine
					if (r > 31) r = 31;
					if (g > 63) g = 63;
					if (b > 31) b = 31;

					*pDest++ = (r << 11) | (g << 5) | b;
				}
			}
		}
		else
		{
			// Non-black color: use alpha blending
			BYTE r565 = (color >> 11) & 0x1F;  // 5-bit red
			BYTE g565 = (color >> 5) & 0x3F;   // 6-bit green
			BYTE b565 = color & 0x1F;          // 5-bit blue

			// Convert alpha from 5-bit (0-31) to 8-bit (0-255)
			BYTE alpha8 = (31 - reverseAlpha) * 255 / 31;

			// Draw alpha-blended rectangle
			for (int y = startY; y < endY; y++)
			{
				WORD* pDest = (WORD*)((BYTE*)surface_info.pixels + y * surface_info.pitch) + startX;
				for (int x = startX; x < endX; x++)
				{
					// Get destination pixel
					WORD destPixel = *pDest;
					BYTE destR = (destPixel >> 11) & 0x1F;
					BYTE destG = (destPixel >> 5) & 0x3F;
					BYTE destB = destPixel & 0x1F;

					// Alpha blend: src * alpha + dest * (1 - alpha)
					BYTE srcR = r565;
					BYTE srcG = g565;
					BYTE srcB = b565;

					BYTE resultR = (srcR * alpha8 + destR * (255 - alpha8)) / 255;
					BYTE resultG = (srcG * alpha8 + destG * (255 - alpha8)) / 255;
					BYTE resultB = (srcB * alpha8 + destB * (255 - alpha8)) / 255;

					// Clamp to valid range
					if (resultR > 31) resultR = 31;
					if (resultG > 63) resultG = 63;
					if (resultB > 31) resultB = 31;

					// Write back
					*pDest++ = (resultR << 11) | (resultG << 5) | resultB;
				}
			}
		}

		// Unlock the surface
		spritectl_unlock_surface(sdl_surface);
#endif
	#else
		// Non-GAME_CLIENT build (editor/tools)
		int reverseAlpha = 31-alpha;
		// No-op for non-game builds
	#endif
}


//---------------------------------------------------------------------------
// InitMinimap
//---------------------------------------------------------------------------
//void
//InitMinimap(CSpriteSurface *minimap_surface)
//{
//	#ifdef __GAME_CLIENT__
//		g_pTopView->InitMinimapTexture(minimap_surface);
//	#endif
//}
//
//
////---------------------------------------------------------------------------
//// DrawMinimap
////---------------------------------------------------------------------------
//void
//DrawMinimapAlpha(int x, int y, CSpriteSurface *minimap_surface)
//{
////	#ifdef __GAME_CLIENT__
////		g_pTopView->DrawMinimap(x, y, 1);
////	#else
//		RECT rt = {0,0,minimap_surface->GetWidth(),minimap_surface->GetHeight()};
//		POINT map_point = {x, y};
//
//		gpC_base->m_p_DDSurface_back->Blt(&map_point, minimap_surface, &rt);
//// 	#endif
//}

