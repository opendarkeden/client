#include "Client_PCH.h"
#define MOTORCYCLE_LIGHT		2

//----------------------------------------------------------------------
//
// Define Functionㄴ
//
//----------------------------------------------------------------------
// 처절.. - -;;
//----------------------------------------------------------------------
// DRAW_ALPHASPRITE
//----------------------------------------------------------------------
#define DRAW_ALPHASPRITE(pPoint, spriteID, ASPK, ASPKI, ASPKFile)	\
		{																\
			CAlphaSprite* pSprite = &ASPK[ spriteID ];					\
																		\
			if (pSprite->IsNotInit())									\
			{															\
				ASPKFile.seekg( ASPKI[spriteID] );						\
				pSprite->LoadFromFile( ASPKFile );						\
			}															\
																		\
			m_pSurface->BltAlphaSprite(pPoint, pSprite);				\
		}

//----------------------------------------------------------------------
// DRAW_ALPHASPRITE
//----------------------------------------------------------------------
#define DRAW_ALPHASPRITEPAL(pPoint, spriteID, ASPK, PAL)	\
		{																\
			CAlphaSpritePal* pSprite = &ASPK[ spriteID ];					\
																		\
			m_pSurface->BltAlphaSpritePal(pPoint, pSprite, PAL);		\
		}

//----------------------------------------------------------------------
// DRAW_NORMALSPRITE
//----------------------------------------------------------------------
#define DRAW_NORMALSPRITE(pPoint, spriteID, SPK, SPKI, SPKFile)			\
		{																\
			CSprite* pSprite = &ASPK[ spriteID ];						\
																		\
			if (pSprite->IsNotInit())									\
			{															\
				SPKFile.seekg( SPKI[spriteID] );						\
				pSprite->LoadFromFile( SPKFile );						\
			}															\
																		\
			m_pSurface->BltSprite(pPoint, pSprite);						\
		}

//----------------------------------------------------------------------
// DRAW_NORMALSPRITE_EFFECT
//----------------------------------------------------------------------
#define DRAW_NORMALSPRITE_EFFECT(pPoint, spriteID, SPK, SPKI, SPKFile, EffectType)	\
		{																\
			CSprite* pSprite = &SPK[ spriteID ];						\
																		\
			if (pSprite->IsNotInit())									\
			{															\
				SPKFile.seekg( SPKI[spriteID] );						\
				pSprite->LoadFromFile( SPKFile );						\
			}															\
																		\
			CSpriteSurface::SetEffect( EffectType );					\
			m_pSurface->BltSpriteEffect(pPoint, pSprite);				\
		}

//----------------------------------------------------------------------
// DRAW_NORMALSPRITEPAL_EFFECT
//----------------------------------------------------------------------
#define DRAW_NORMALSPRITEPAL_EFFECT(pPoint, spriteID, SPK, pal, EffectType)	\
		{																\
			CSpritePal* pSprite = &SPK[ spriteID ];						\
																		\
			CSpriteSurface::SetPalEffect( EffectType );					\
			m_pSurface->BltSpritePalEffect(pPoint, pSprite, pal);		\
		}

//----------------------------------------------------------------------
// DRAW_TEXTURE_SPRITE
//----------------------------------------------------------------------
#define DRAW_TEXTURE_SPRITE(x, y, spriteID, pTextureManager)					\
		{																		\
			CSpriteSurface* pSurface = pTextureManager->GetTexture(spriteID);	\
																				\
			if (pSurface!=NULL)													\
			{																	\
				if (CDirect3D::GetDevice()->BeginScene()==D3D_OK)				\
				{																\
					CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());	\
																				\
					RECT rect =													\
					{															\
						x,														\
						y,														\
						x + pTextureManager->GetWidth(spriteID),				\
						y + pTextureManager->GetHeight(spriteID)				\
					};															\
																				\
					m_TextureEffect.DrawEffect2D( &rect );						\
																				\
					CDirect3D::GetDevice()->EndScene();							\
																				\
					CDirect3D::GetDevice()->SetTexture(0, NULL);				\
				}																\
			}																	\
		}

//----------------------------------------------------------------------
// DRAW_TEXTURE_SPRITE_LOCKED
//----------------------------------------------------------------------
#define DRAW_TEXTURE_SPRITE_LOCKED(x, y, spriteID, pTextureManager)				\
			m_pSurface->Unlock();												\
																				\
			DRAW_TEXTURE_SPRITE(x, y, spriteID, pTextureManager)				\
																				\
			m_pSurface->Lock();

////----------------------------------------------------------------------
//// DRAW_TEXTURE_SPRITE
////----------------------------------------------------------------------
//#define DRAW_TEXTURE_SPRITEPAL(x, y, spriteID, pTextureManager, pal)				\
//		{																			\
//			CSpriteSurface* pSurface = pTextureManager->GetTexture(spriteID, pal);	\
//																				\
//			if (pSurface!=NULL)													\
//			{																	\
//				if (CDirect3D::GetDevice()->BeginScene()==D3D_OK)				\
//				{																\
//					CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());	\
//																				\
//					RECT rect =													\
//					{															\
//						x,														\
//						y,														\
//						x + pTextureManager->GetWidth(spriteID),				\
//						y + pTextureManager->GetHeight(spriteID)				\
//					};															\
//																				\
//					m_TextureEffect.DrawEffect2D( &rect );						\
//																				\
//					CDirect3D::GetDevice()->EndScene();							\
//																				\
//					CDirect3D::GetDevice()->SetTexture(0, NULL);				\
//				}																\
//			}																	\
//		}
//
////----------------------------------------------------------------------
//// DRAW_TEXTURE_SPRITE_LOCKED
////----------------------------------------------------------------------
//#define DRAW_TEXTURE_SPRITEPAL_LOCKED(x, y, spriteID, pTextureManager, pal)		\
//			m_pSurface->Unlock();												\
//																				\
//			DRAW_TEXTURE_SPRITEPAL(x, y, spriteID, pTextureManager, pal)			\
//																				\
//			m_pSurface->Lock();

//----------------------------------------------------------------------
// DRAW_TEXTURE_SURFACE
//----------------------------------------------------------------------
#define	DRAW_TEXTURE_SURFACE( pTextureSurface, pRect )							\
			if (CDirect3D::GetDevice()->BeginScene()==D3D_OK)					\
			{																	\
				CDirect3D::GetDevice()->SetTexture(0, pTextureSurface->GetSurface());	\
																				\
				m_TextureEffect.DrawEffect2D( pRect );							\
																				\
				CDirect3D::GetDevice()->EndScene();								\
																				\
				CDirect3D::GetDevice()->SetTexture(0, NULL);					\
			}

//----------------------------------------------------------------------
// GET_EFFECTSPRITETYPE_xxxxx
//----------------------------------------------------------------------
#define	GET_EFFECTSPRITETYPE_SCREEN( frameID )			\
		(frameID + MAX_EFFECTSPRITETYPE_ALPHAEFFECT)

#define	GET_EFFECTSPRITETYPE_EFFECT( frameID )			\
		(frameID)

#define	GET_EFFECTSPRITETYPE_NORMAL( frameID )			\
		(frameID + MAX_EFFECTSPRITETYPE_SCREENEFFECT)

#define	GET_EFFECTSPRITETYPE_SHADOW( frameID )			\
		(frameID + MAX_EFFECTSPRITETYPE_NORMALEFFECT)

//----------------------------------------------------------------------
// DRAW_SPRITE_WITH_EFFECTFRAME
//----------------------------------------------------------------------
#define	DRAW_SPRITE_WITH_EFFECTFRAME( pPoint, EffectSPK, EffectFPK, frameID, direction, frame )	\
		{																						\
			CFrame& Frame = EffectFPK[frameID][direction][frame];								\
			int sprite = Frame.GetSpriteID();													\
																								\
			if (sprite < EffectSPK.GetSize())													\
			{																					\
				POINT pointTemp = *pPoint;														\
				pointTemp.x += Frame.GetCX();													\
				pointTemp.y += Frame.GetCY();													\
																								\
				m_pSurface->BltSprite(&pointTemp, &EffectSPK[ sprite ]);						\
			}																					\
		}

//----------------------------------------------------------------------
// HAS_PAIR_EFFECTSPRITETYPE
//----------------------------------------------------------------------
#define	HAS_PAIR_EFFECTSPRITETYPE(est)	((*g_pEffectSpriteTypeTable)[est].PairFrameIDList.size())

//----------------------------------------------------------------------
// HAS_ACTION_EFFECTSPRITETYPE
//----------------------------------------------------------------------
#define	HAS_ACTION_EFFECTSPRITETYPE(est)	((*g_pEffectSpriteTypeTable)[est].ActionEffectFrameID!=FRAMEID_NULL)

//----------------------------------------------------------------------
// GET_ACTION_EFFECTSPRITETYPE
//----------------------------------------------------------------------
#define	GET_ACTION_EFFECTSPRITETYPE(est)	((*g_pEffectSpriteTypeTable)[est].ActionEffectFrameID)

//----------------------------------------------------------------------
// GET_ACTION_EFFECTSPRITETYPE_FRAMEID
//----------------------------------------------------------------------
#define	GET_ACTION_EFFECTSPRITETYPE_FRAMEID(aest, action)	((*g_pActionEffectSpriteTypeTable)[aest][action].FrameID)

//----------------------------------------------------------------------
// DRAW_SPRITE_WITH_EFFECTFRAME_PAIR
//----------------------------------------------------------------------
#define DRAW_SPRITE_WITH_EFFECTFRAME_PAIR( est, pPoint, EffectSPK, EffectFPK, direction, frame )	\
		if (HAS_PAIR_EFFECTSPRITETYPE(est))															\
		{																							\
			EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST& idList = (*g_pEffectSpriteTypeTable)[est].PairFrameIDList;	\
																									\
			EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST::iterator iID = idList.begin();				\
																									\
			while (iID != idList.end())																\
			{																						\
				int frameID = *iID;																	\
																									\
				DRAW_SPRITE_WITH_EFFECTFRAME(pPoint, EffectSPK, EffectFPK, frameID, direction, frame )	\
																									\
				iID++;																				\
			}																						\
		}
	
//----------------------------------------------------------------------
// ADD_MOTORCYCLE_LIGHT_2D
//----------------------------------------------------------------------
#define ADD_MOTORCYCLE_LIGHT_2D( pCreature, bForceLight )						\
		if (pCreature->GetMoveDevice()==MCreature::MOVE_DEVICE_RIDE				\
			&& !pCreature->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE))	\
		{																		\
			int pX = pCreature->GetPixelX() - m_FirstZonePixel.x + TILE_X_HALF;	\
			int pY = pCreature->GetPixelY() - m_FirstZonePixel.y - TILE_Y;		\
																				\
			POINT pt = GetChangeValueToDirection(pCreature->GetDirection());	\
																				\
			AddLightFilter2D( pX + pt.x*(TILE_X<<2) + TILE_X_HALF,				\
								pY - pCreature->GetZ() + pt.y*(TILE_Y<<2) + TILE_Y_HALF,		\
								MOTORCYCLE_LIGHT,								\
								false,											\
								bForceLight);									\
		}

//----------------------------------------------------------------------
// ADD_MOTORCYCLE_LIGHT_3D
//----------------------------------------------------------------------
#define ADD_MOTORCYCLE_LIGHT_3D( pCreature, bForceLight )						\
		if (pCreature->GetMoveDevice()==MCreature::MOVE_DEVICE_RIDE				\
			&& !pCreature->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE))	\
		{																		\
			int pX = pCreature->GetPixelX() - m_FirstZonePixel.x + TILE_X_HALF;	\
			int pY = pCreature->GetPixelY() - m_FirstZonePixel.y - TILE_Y;		\
																				\
			POINT pt = GetChangeValueToDirection(pCreature->GetDirection());	\
																				\
			AddLightFilter3D( pX + pt.x*(TILE_X<<2) + TILE_X_HALF,				\
								pY - pCreature->GetZ() + pt.y*(TILE_Y<<2) + TILE_Y_HALF,		\
								MOTORCYCLE_LIGHT,								\
								false,											\
								bForceLight);									\
		}

//----------------------------------------------------------------------
// ADD_MOTORCYCLE_LIGHT_XY_2D
//----------------------------------------------------------------------
#define ADD_MOTORCYCLE_LIGHT_XY_2D( pCreature, pX, pY, bForceLight )			\
		if (pCreature->GetMoveDevice()==MCreature::MOVE_DEVICE_RIDE				\
			&& !pCreature->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE))	\
		{																		\
			POINT pt = GetChangeValueToDirection(pCreature->GetDirection());	\
																				\
			AddLightFilter2D( pX + pt.x*(TILE_X<<2) + TILE_X_HALF,				\
								pY - pCreature->GetZ() + pt.y*(TILE_Y<<2) + TILE_Y_HALF,		\
								MOTORCYCLE_LIGHT,								\
								false,											\
								bForceLight);									\
		}

//----------------------------------------------------------------------
// ADD_MOTORCYCLE_LIGHT_XY_3D
//----------------------------------------------------------------------
#define ADD_MOTORCYCLE_LIGHT_XY_3D( pCreature, pX, pY, bForceLight )			\
		if (pCreature->GetMoveDevice()==MCreature::MOVE_DEVICE_RIDE				\
			&& !pCreature->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE))	\
		{																		\
			POINT pt = GetChangeValueToDirection(pCreature->GetDirection());	\
																				\
			AddLightFilter3D( pX + pt.x*(TILE_X<<2) + TILE_X_HALF,							\
								pY - pCreature->GetZ() + pt.y*(TILE_Y<<2) + TILE_Y_HALF,		\
								MOTORCYCLE_LIGHT,								\
								false,											\
								bForceLight);									\
		}


//----------------------------------------------------------------------
// DRAW_ALPHA_BOX_2D
//----------------------------------------------------------------------
#define DRAW_ALPHA_BOX_2D( rect, boxColor )						\
		if (boxColor==0)										\
		{														\
			if (CDirectDraw::Is565())							\
			{													\
				m_pSurface->GammaBox565(&rect, 15);				\
			}													\
			else												\
			{													\
				m_pSurface->GammaBox555(&rect, 15);				\
			}													\
		}														\
		else													\
		{														\
			m_pSurface->BltColorAlpha(&rect, boxColor, 15);		\
		}

//----------------------------------------------------------------------
// DRAW_CHAT_BOX_OUTLINE
//----------------------------------------------------------------------
#define	MIN_CHAT_BOX_WIDTH	60
#define CHAT_BOX_TAIL_X		29				// 채팅 꼬리표 붙는 x위치
#define MIN_CHAT_LARGE_BOX_WIDTH	90

#define DRAW_CHAT_BOX_OUTLINE( rect, color )					\
		{														\
			int left = rect.left+1;								\
			int top = rect.top+1;								\
																\
			int width = abs(rect.right - rect.left);			\
			int height = abs(rect.bottom - rect.top);			\
																\
			if (rect.top-1 > 0)									\
			{													\
				m_pSurface->HLine(left-1, rect.top-1, width, color);	\
			}															\
																		\
			if (rect.bottom < CLIPSURFACE_HEIGHT)						\
			{															\
				m_pSurface->HLine(left-1, rect.bottom, CHAT_BOX_TAIL_X, color);		\
				m_pSurface->HLine(left+CHAT_BOX_TAIL_X-1+6, rect.bottom, width-CHAT_BOX_TAIL_X-6, color);	\
			}																\
																			\
			if (rect.left >= 0)												\
			{																\
				m_pSurface->VLine(rect.left-1, top-1, height, color);		\
			}																\
																			\
			if (rect.right < CLIPSURFACE_WIDTH)								\
			{																\
				m_pSurface->VLine(rect.right, top-1, height, color);		\
			}																\
		}


//----------------------------------------------------------------------
// DRAW_CHAT_BOX_TAIL ( 원래 박스, 색깔 )
//----------------------------------------------------------------------
#define DRAW_CHAT_BOX_TAIL_LOCKED( rect, color )					\
		{															\
			WORD *lpSurface, pitch_2;								\
			lpSurface = (WORD*)m_pSurface->GetSurfacePointer();		\
			pitch_2 = m_pSurface->GetSurfacePitch() >> 1;			\
																	\
			int x = rect.left + CHAT_BOX_TAIL_X;					\
			int y = rect.bottom;									\
																	\
			const int ptNum = 13;									\
			POINT pt[ptNum] =										\
			{														\
				{ x, y+1 },											\
				{ x+5, y+1 },										\
				{ x, y+2 },											\
				{ x+4, y+2 },										\
				{ x+1, y+3 },										\
				{ x+4, y+3 },										\
				{ x+1, y+4 },										\
				{ x+3, y+4 },										\
				{ x+1, y+5 },										\
				{ x+3, y+5 },										\
				{ x+2, y+6 },										\
				{ x+2, y+7 },										\
				{ x+2, y+8 }										\
			};														\
																	\
			if (x>=0 && y+1>=0										\
					&& x+5<CLIPSURFACE_WIDTH						\
					&& y+8<CLIPSURFACE_HEIGHT)						\
			{														\
				for (int i=0; i<ptNum; i++)							\
				{													\
					*(lpSurface + pitch_2*pt[i].y + pt[i].x) = color;	\
				}														\
			}															\
			else														\
			{															\
				for (int i=0; i<ptNum; i++)								\
				{														\
					int x = pt[i].x;									\
					int y = pt[i].y;									\
																		\
					if (x>=0 && y>=0									\
						&& x<CLIPSURFACE_WIDTH							\
						&& y<CLIPSURFACE_HEIGHT)						\
					{													\
						*(lpSurface + pitch_2*y + x) = color;			\
					}													\
				}														\
			}															\
		}

//----------------------------------------------------------------------
// Affect Orbit Position
//----------------------------------------------------------------------
// pEffect가 AttachOrbitEffect라면..
// point에 orbitPosition을 더해준다.
//----------------------------------------------------------------------
#define AFFECT_ORBIT_EFFECT_POSITION( pEffect, point )					\
		if (pEffect->GetEffectType()==MEffect::EFFECT_ATTACH_ORBIT)		\
		{																\
			const POINT& pt = ((MAttachOrbitEffect*)pEffect)->GetOrbitPosition();	\
			point.x += pt.x;											\
			point.y += pt.y;											\
		}

//----------------------------------------------------------------------
// Is BackGround Orbit Position
//----------------------------------------------------------------------
// pEffect가 AttachOrbitEffect라면..
// point의 y좌표에 따라 Back, Fore를 결정한다.
//----------------------------------------------------------------------
#define AFFECT_ORBIT_EFFECT_BACKGROUND( pEffect, bBackground )					\
		if (pEffect->GetEffectType()==MEffect::EFFECT_ATTACH_ORBIT)		\
		{																\
			const POINT& pt = ((MAttachOrbitEffect*)pEffect)->GetOrbitPosition();	\
			bBackground = (pt.y < 0); \
		}
