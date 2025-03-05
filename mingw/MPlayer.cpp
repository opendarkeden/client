//----------------------------------------------------------------------
// MPlayer.cpp
//----------------------------------------------------------------------
//
// m_nUsedActionInfo는 다음 turn에 바로 수행하게 되는 동작이고
//
// m_nNextUsedActionInfo는 
//          추적이 끝나고 나서야 수행하게 되는 동작이다.(buffer의 역할)
//
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <math.h>
#include "MZone.h"
#include "MTopView.h"
#include "MItem.h"
#include "MCreature.h"
#include "MInteractionObject.h"
#include "MLinearEffect.h"
#include "TempInformation.h"
#include "MParabolaEffect.h"
#include "MPlayer.h"
#include "MActionInfoTable.h"
#include "MEffectTarget.h"
#include "MEffectGeneratorTable.h"
#include "PacketDef.h" 
#include "ClientDef.h"
#include "MCreatureTable.h"
#include "MSoundTable.h"
#include "MItemTable.h"
#include "ClientConfig.h"
#include "MGameStringTable.h"
#include "MStatusManager.h"
#include "MItemOptionTable.h"
#include "TalkBox.h"
#include "UIDialog.h"
#include "MHelpManager.h"
#include "MPortal.h"
#include "PacketFunction.h"
#include "MGameTime.h"
#include "MObjectSelector.h"
#include "SoundDef.h"
#include "MItemFinder.h"
#include "packet\Cpackets\CGSelectTileEffect.h"
#include "packet\Cpackets\CGPartyInvite.h"
#include "EffectSpriteTypeDef.h"
#include "ModifyStatusManager.h"
#include "ClientCommunicationManager.h"
#include "MEventManager.h"
#include "MItemFinder.h"
#include "MZoneTable.h"

#include "Rpackets\RCPositionInfo.h"
#include "Rpackets\RCStatusHP.h"
#include "Rpackets\RCCharacterInfo.h"
#include "Cpackets\CGRequestInfo.h"
#include "Cpackets\CGUseItemFromGear.h"

#include "UserInformation.h"
#include "MParty.h"
#include "MJusticeAttackManager.h"
#include "RequestUserManager.h"
#include "MWarManager.h"

#include "MGuildType.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "MTestDef.h"
#include "RankBonusTable.h"
#include "RankBonusDef.h"
#include "MOustersGear.h"

#include "CPackets/CGAbsorbSoul.h"

#include "UIMessageManager.h"
#include "UIFunction.h"
#include "RequestClientPlayerManager.h"
#include "SystemAvailabilities.h"

#include "UserOption.h"
#include "VS_UI_GameCommon2.h"

#include "Cpackets\CGPartyPosition.h"

extern void UI_RunPetInfo(struct PETINFO *pPetInfo);
extern bool UpdateSocketOutput();
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
//#include "Client.h"
bool		GetMakeItemFitPosition(MItem* pItem, ITEM_CLASS itemClass, int itemType, POINT& fitPoint);

// [새기술]
void		SkillShadowDancing(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID);
bool			IsCreatureMove( MCreature *pCreature );
bool			IsCreatureActionAttack( MCreature *pCreature );
bool			HasEffectStatusSummonSylph( MCreature* pCreature );
void			RemoveEffectStatusSummonSylph( MCreature* pCreature );
BYTE GetCreatureActionCountMax( const MCreature* pCreature, int action );

// #ifndef CONNECT_SERVER
	#include "MFakeCreature.h"
// #endif


#include "MTestDef.h"

#ifdef __METROTECH_TEST__
//	#define OUTPUT_DEBUG_PLAYER_ACTION
#endif



#include "VS_UI.h" // KJTINC
#include "VS_UI_Mouse_pointer.h"
#include "CImm.h"

extern	DWORD	g_CurrentTime;
extern	int		g_x;
extern	int		g_y;
extern	bool	g_bNetStatusGood;
extern BOOL					g_bLButtonDown;
extern BOOL					g_bRButtonDown;
extern BOOL					g_bCButtonDown;
extern bool g_bPreviousMove;

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;
extern bool					g_bZoneSafe;
extern bool					g_bHolyLand;
extern POINT						g_SelectSector;

//#define	new			DEBUG_NEW

#define				TRADE_DISTANCE_NPC		4
#define				TRADE_DISTANCE_PC		2


		

//----------------------------------------------------------------------
//
//							Global Player
//
//----------------------------------------------------------------------
MPlayer*			g_pPlayer = NULL;

//----------------------------------------------------------------------
// define Functions
//----------------------------------------------------------------------
// 막 시체가 되는 순간...
/*
#define GET_DYING_CREATURE( pCreature, id )								\
		if (pCreature==NULL)											\
		{																\
			MItem* pItem = g_pZone->GetItem( id );						\
																		\
			if (pItem!=NULL												\
				&& pItem->GetItemClass()==ITEM_CLASS_CORPSE)		\
			{															\
				pCreature = ((MCorpse*)pItem)->GetCreature();			\
																		\
				if (pCreature->GetActionCount() >= pCreature->GetActionCountMax())	\
				{														\
					pCreature = NULL;									\
				}														\
			}															\
		}
*/

//----------------------------------------------------------------------
// CHECK_REPEAT_LIMIT_TO_RETURN_FALSE
//----------------------------------------------------------------------
#define CHECK_REPEAT_LIMIT_TO_RETURN_FALSE(nActionInfo, repeatCount)		\
		if (nActionInfo < g_pActionInfoTable->GetSize())	\
		{													\
			if (repeatCount >= (*g_pActionInfoTable)[nActionInfo].GetRepeatLimit())	\
			{												\
				UnSetRepeatAction();						\
				return false;								\
			}												\
		}

//----------------------------------------------------------------------
// 포션 자동 먹기
//----------------------------------------------------------------------
#ifdef __METROTECH_TEST__

	int  g_iAutoHealPotion = 1;
	bool g_bAutoManaPotion = true;
	int	 g_iAutoReload = 1;
	bool g_bLight = false;
	int	 g_iSpeed = 0;

	void
	AutoUseSkill(int skill)
	{
		if (g_pSkillInfoTable != NULL
			&& g_pInventory!=NULL 
			&& g_pPlayer!=NULL
			&& g_pPlayer->IsSlayer()
			&& g_pPlayer->IsItemCheckBufferNULL()
			&& !g_pPlayer->IsInSafeSector())
		{
			bool bUse = false;
			int skill_id = ACTIONINFO_NULL;

			if(skill == 0)	//healing
			{
				const int hp_gap = g_pPlayer->GetMAX_HP()-g_pPlayer->GetHP();
				
				if(hp_gap > 0)
				{
					if(g_iAutoHealPotion == 2)	// 에너지 달면 단만큼 채운다
					{
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_CRITICAL_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_CRITICAL_WOUNDS].IsEnable() && 
							(*g_pSkillInfoTable)[MAGIC_CURE_CRITICAL_WOUNDS].GetExpLevel()/2+30 <= hp_gap)
						{
							skill_id = MAGIC_CURE_CRITICAL_WOUNDS;
							bUse = true;
						}
						else
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_SERIOUS_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_SERIOUS_WOUNDS].IsEnable() && 
							(*g_pSkillInfoTable)[MAGIC_CURE_SERIOUS_WOUNDS].GetExpLevel()/4+30 <= hp_gap)
						{
							skill_id = MAGIC_CURE_SERIOUS_WOUNDS;
							bUse = true;
						}
						else
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_LIGHT_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_LIGHT_WOUNDS].IsEnable() && 
							(*g_pSkillInfoTable)[MAGIC_CURE_LIGHT_WOUNDS].GetExpLevel()/4+10 <= hp_gap)
						{
							skill_id = MAGIC_CURE_LIGHT_WOUNDS;
							bUse = true;
						}
					}
					else if(g_iAutoHealPotion == 3)	// 에너지 달면 채운다
					{
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_CRITICAL_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_CRITICAL_WOUNDS].IsEnable())
						{
							skill_id = MAGIC_CURE_CRITICAL_WOUNDS;
							bUse = true;
						}
						else
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_SERIOUS_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_SERIOUS_WOUNDS].IsEnable())
						{
							skill_id = MAGIC_CURE_SERIOUS_WOUNDS;
							bUse = true;
						}
						else
						if(g_pSkillAvailable->IsEnableSkill( MAGIC_CURE_LIGHT_WOUNDS ) &&
							(*g_pSkillInfoTable)[MAGIC_CURE_LIGHT_WOUNDS].IsEnable())
						{
							skill_id = MAGIC_CURE_LIGHT_WOUNDS;
							bUse = true;
						}
					}
				}
			}

			if(skill == 1)	// create holy water
			{
				if(g_pSkillAvailable->IsEnableSkill( MAGIC_CREATE_HOLY_WATER ) &&
					(*g_pSkillInfoTable)[MAGIC_CREATE_HOLY_WATER].IsEnable() &&
					(*g_pSkillInfoTable)[MAGIC_CREATE_HOLY_WATER].IsAvailableTime())
				{
					skill_id = MAGIC_CREATE_HOLY_WATER;
					bUse = true;
				}
			}

			if(bUse == true && skill_id != ACTIONINFO_NULL)
			{
				// 스킬사용
				g_pPlayer->UnSetRequestMode();
				
				TYPE_ACTIONINFO old_special = g_pPlayer->GetSpecialActionInfo();
				g_pPlayer->SetSpecialActionInfo(skill_id);
				g_pPlayer->SelfSpecialAction();
				g_pPlayer->SetSpecialActionInfo(old_special);
			
//				g_bPreviousMove = false;
			}
		}
	}

	//------------------------------------------------------------
	// Healing 먹기
	//------------------------------------------------------------
	void
	AutoUsePotion(int bHeal)	// 0:heal 1:mana 2:magazine
	{
		if (g_pInventory!=NULL 
			&& g_pPlayer!=NULL
			&& !g_pPlayer->IsVampire()
			&& g_pPlayer->IsItemCheckBufferNULL()
			&& !gbl_item_lock)
		{
			// Healing
			int badClass = -1;
			int badType = -1;

			int currentValue;
			int maxValue; 
			float modifier = 1.0f;

			switch(bHeal)
			{
			case 0:	// heal
				currentValue = g_pPlayer->GetHP();
				maxValue = g_pPlayer->GetMAX_HP();
				break;

			case 1: // mana
				currentValue = g_pPlayer->GetMP();
				maxValue = g_pPlayer->GetMAX_MP();

				modifier = 1.0f + g_pPlayer->GetINT() / 300.0f;
				break;

			case 2: // magazine
			case 3: // magazine
			case 4: // magazien
			case 5: // magazine
				if(g_pCurrentMagazine == NULL)
					return;

				currentValue = g_pCurrentMagazine->GetNumber();
				maxValue = 1;
				break;
			}

			if (maxValue<=0) return;

			int lost = maxValue - currentValue;
			int percent = currentValue * 100 / maxValue;

			bool forceUse = false;
			
			ITEM_CLASS itemClass = ITEM_CLASS_POTION;
			if(g_pPlayer->IsOusters())
				itemClass = ITEM_CLASS_PUPA;

			int itemType;
			int maxItemType = 4;
			
			if(bHeal >= 2)
			{
				if(g_pSlayerGear != NULL && !g_pSlayerGear->GetItem(MSlayerGear::GEAR_SLAYER_LEFTHAND)->IsGunItem())return;
//				badType = (g_pSlayerGear->GetItem(MSlayerGear::GEAR_SLAYER_LEFTHAND)->GetItemClass()-ITEM_CLASS_SG)*2-1;
				badType = (g_pCurrentMagazine->GetItemType() & ~1) -1;
				itemClass = ITEM_CLASS_MAGAZINE;
				
				if(currentValue > bHeal-2)
					return;

				itemType = badType+2;
			}
			else
			{
				if (lost >= 200.0f * modifier)		itemType = 4;
				else if (lost >= 100.0f * modifier)	itemType = 3;
				else if (lost >= 51.0f * modifier)	itemType = 2;
				else if (lost >= 24.0f * modifier)	itemType = 1;
				else if (lost >= 10.0f * modifier)	itemType = 0;
				else								itemType = badType;
			}

			switch(bHeal)
			{
			case 0:	// Heal
				// HP는 50% 이하일때 먹는다.
				forceUse = percent <= 50;
				break;

			case 1:	// Mana
				badType += 5;
				itemType += 5;
				maxItemType += 5;

				// Mana는 현재 사용중인 기술과 비교해서 먹는다.
				if (g_pSkillInfoTable!=NULL)
				{
					int skill = g_pPlayer->GetSpecialActionInfo();

					if (skill < g_pSkillInfoTable->GetSize())
					{
						forceUse = currentValue < 5
									|| (*g_pSkillInfoTable)[skill].GetMP()*2 >= currentValue;
					}
				}
				break;

			}
			
			if (itemType!=badType && itemClass!=badClass)
			{
				MItem* pItem = NULL;
				int type = itemType;

				do
				{
					pItem = g_pInventory->FindItem(itemClass , type );

					// reload일때는 은제총알을 찾아보자
					if(bHeal >= 2 && pItem == NULL) pItem = g_pInventory->FindItem(itemClass , type +8);

					if (pItem!=NULL) break;					
				}
				while (--type > badType);

				// 몇 percent 이하일때는 그냥 먹어야 한다. 총알은 빼구
				if (bHeal != 2 && pItem==NULL && forceUse)
				{
					type = badType + 1;

					do
					{
						pItem = g_pInventory->FindItem( ITEM_CLASS_POTION, type );

						if (pItem!=NULL) break;					
					}
					while (++type <= maxItemType);
				}
			
				if (pItem!=NULL && g_pPlayer->IsItemCheckBufferNULL())
				{
					if(bHeal >= 2)
					{
						CGReloadFromInventory _CGReloadFromInventory;
						_CGReloadFromInventory.setObjectID( pItem->GetID() );
						_CGReloadFromInventory.setX( pItem->GetGridX() );
						_CGReloadFromInventory.setY( pItem->GetGridY() );

						g_pSocket->sendPacket( &_CGReloadFromInventory );
					}
					else
					{
						CGUsePotionFromInventory _CGUsePotionFromInventory;
						_CGUsePotionFromInventory.setObjectID( pItem->GetID() );
						_CGUsePotionFromInventory.setX( pItem->GetGridX() );
						_CGUsePotionFromInventory.setY( pItem->GetGridY() );

						g_pSocket->sendPacket( &_CGUsePotionFromInventory );
					}

					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
				}
				else if (g_pQuickSlot!=NULL)
				{
					// 벨트에서 찾기
					type = itemType;

					do
					{
						MItemClassTypeFinder itemFinder( itemClass, type );

						pItem = g_pQuickSlot->FindItem( itemFinder );

						// reload일때는 은제총알을 찾아보자
						if(bHeal >= 2 && pItem == NULL)
						{
							MItemClassTypeFinder itemFinder( itemClass, type +8 );

							pItem = g_pQuickSlot->FindItem( itemFinder );
						}

						if (pItem!=NULL) break;					
					}
					while (--type > badType);

					// 몇 percent 이하일때는 그냥 먹어야 한다. 총알일 때는 빼구
					if (bHeal != 2 && pItem==NULL && forceUse)
					{
						type = badType + 1;

						do
						{
							MItemClassTypeFinder itemFinder( ITEM_CLASS_POTION, type );

							pItem = g_pQuickSlot->FindItem( itemFinder );

							if (pItem!=NULL) break;					
						}
						while (++type <= maxItemType);
					}

					if (pItem!=NULL && g_pPlayer->IsItemCheckBufferNULL() )
					{
						if(bHeal >= 2)
						{
							g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);

							CGReloadFromQuickSlot _CGReloadFromQuickSlot;
							_CGReloadFromQuickSlot.setObjectID( pItem->GetID() );
							_CGReloadFromQuickSlot.setSlotID( pItem->GetItemSlot() );							

							g_pSocket->sendPacket( &_CGReloadFromQuickSlot );
						}
						else
						{
							g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);

							CGUsePotionFromQuickSlot _CGUsePotionFromQuickSlot;
							_CGUsePotionFromQuickSlot.setObjectID( pItem->GetID() );
							_CGUsePotionFromQuickSlot.setSlotID( pItem->GetItemSlot() );							

							g_pSocket->sendPacket( &_CGUsePotionFromQuickSlot );
						}

						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_QUICKSLOT);

						UI_LockGear();
					}					
				}			
				
			}
		}
	}

#endif

bool	CanActionByZoneInfo()
{	
	if(g_pZone != NULL)
	{
		ZONETABLE_INFO *pZoneInfo = g_pZoneTable->Get( g_pZone->GetID() );
		
		if(pZoneInfo->CannotAttackInSafe == true)					// 안전지대 안에서 공격할 수 없으면
		{			
			if ( g_pZone->GetSector(g_pPlayer->GetX(),g_pPlayer->GetY()).IsSafeComplete() )
				return true;
			else
			{
				if(g_pPlayer->IsSlayer() &&	g_pZone->GetSector(g_pPlayer->GetX(),g_pPlayer->GetY()).IsSafeSlayer() )
					return true;				
				if(g_pPlayer->IsVampire() && g_pZone->GetSector(g_pPlayer->GetX(),g_pPlayer->GetY()).IsSafeVampire() ) 
					return true;
				if(g_pPlayer->IsOusters() && g_pZone->GetSector(g_pPlayer->GetX(),g_pPlayer->GetY()).IsSafeOusters() ) 
					return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
// 파티원들에게 내 좌표를 보낸다.
//----------------------------------------------------------------------
void
SendPositionInfoToParty()
{
	//------------------------------------------------------
	// 지속적으로 좌표를 보내는 경우
	//------------------------------------------------------		
	if (g_pParty->GetSize()!=0
		&& g_pClientCommunicationManager!=NULL)
	{
		static int oldX = -1;
		static int oldY = -1;
		static int oldZoneID = -1;

		int x		= g_pPlayer->GetX();
		int y		= g_pPlayer->GetY();
		int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

		// 좌표가 달라졌으면 보낸다.
		if (oldX!=x || oldY!=y || oldZoneID!=zoneID)			
		{
			if(g_pUserInformation->bKorean == true)
			{
				RCPositionInfo _RCPositionInfo;

				// 이름을 안 보내도 IP로 체크하기 때메 된다.
				//_RCPositionInfo.setName( g_pUserInformation->CharacterID.GetString() );
				_RCPositionInfo.setZoneX( x );
				_RCPositionInfo.setZoneY( y );
				_RCPositionInfo.setZoneID( zoneID );

				// 보낸 정보 기억
				oldX = x;
				oldY = y;
				oldZoneID = zoneID;

				for (int i=0; i<g_pParty->GetSize(); i++)
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( i );
					
					if (pInfo!=NULL
						// 시야에 없는 경우에만 위치 정보를 보낸다.
						// 의미가 없을지도.. - -;
						//&& !pInfo->bInSight
						)
					{
						int port = g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT;

						RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pInfo->Name.GetString() );

						if (pUserInfo!=NULL)
						{
							if (pUserInfo->UDPPort!=0)
								port = pUserInfo->UDPPort;
						}

						g_pClientCommunicationManager->sendPacket( pInfo->IP, 
																port, 
																&_RCPositionInfo );
					}
				}			
			}
			else	// 한국어가 아닐 경우
			{
				CGPartyPosition _CGPartyPosition;

				_CGPartyPosition.setZoneID ( zoneID );
				_CGPartyPosition.setXY( x, y );
				_CGPartyPosition.setHP( g_pPlayer->GetMAX_HP(), g_pPlayer->GetHP());

				g_pSocket->sendPacket( &_CGPartyPosition );
			}
		}
	}
}

//----------------------------------------------------------------------
// Send StatusInfo To Party
//----------------------------------------------------------------------
// 파티원들에게 내 상태 정보를 보낸다. 언제? - -;
//----------------------------------------------------------------------
void
SendStatusInfoToParty()
{
	static DWORD nextTime = g_CurrentTime;

	if (g_CurrentTime >= nextTime)
	{
		//------------------------------------------------------
		// 지속적으로 좌표를 보내는 경우
		//------------------------------------------------------		
		if (g_pParty->GetSize()!=0
			&& g_pClientCommunicationManager!=NULL)
		{
			static int oldHP = -1;
			static int oldMaxHP = -1;
			
			int HP		= g_pPlayer->GetHP();
			int MaxHP	= g_pPlayer->GetMAX_HP();
			
			// 좌표가 달라졌으면 보낸다.
			if (oldHP!=HP || oldMaxHP!=MaxHP)
			{
				if(g_pUserInformation->bKorean == true)
				{
					RCStatusHP _RCStatusHP;

					// 이름을 안 보내도 IP로 체크하기 때메 된다.
					//_RCStatusHP.setName( g_pUserInformation->CharacterID.GetString() );
					_RCStatusHP.setCurrentHP( HP );
					_RCStatusHP.setMaxHP( MaxHP );
					
					// 보낸 정보 기억
					oldHP = HP;
					oldMaxHP = MaxHP;
					
					for (int i=0; i<g_pParty->GetSize(); i++)
					{
						PARTY_INFO* pInfo = g_pParty->GetMemberInfo( i );
						
						if (pInfo!=NULL
							// 시야에 없는 경우에만 상태 정보를 보낸다.
							// 의미가 없을지도.. - -;
							//&& !pInfo->bInSight
							)
						{
							int port = g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT;

							RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pInfo->Name.GetString() );

							if (pUserInfo!=NULL)
							{
								if (pUserInfo->UDPPort!=0)
									port = pUserInfo->UDPPort;
							}

							g_pClientCommunicationManager->sendPacket( pInfo->IP, 
																	port, 
																	&_RCStatusHP );
						}
					}	
				}
				else // 한국어가 아닌 경우
				{
					int x		= g_pPlayer->GetX();
					int y		= g_pPlayer->GetY();
					int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

					CGPartyPosition _CGPartyPosition;

					_CGPartyPosition.setZoneID ( zoneID );
					_CGPartyPosition.setXY( x, y );
					_CGPartyPosition.setHP( g_pPlayer->GetMAX_HP(), g_pPlayer->GetHP());

					g_pSocket->sendPacket( &_CGPartyPosition );
				}
			}
		}

		// 시야에 없는 경우의 HP이기 때문에..
		// 5초에 한번 갱신해준다.
		nextTime = g_CurrentTime + g_pClientConfig->CLIENT_COMMUNICATION_STATUS_DELAY;
	}
}

//----------------------------------------------------------------------
// Send StatusInfo To Party
//----------------------------------------------------------------------
// 파티원들에게 내 상태 정보를 보낸다. 언제? - -;
//----------------------------------------------------------------------
void
SendCharacterInfoToParty()
{
	//------------------------------------------------------
	// 지속적으로 좌표를 보내는 경우
	//------------------------------------------------------		
	if (g_pParty->GetSize()!=0
		&& g_pClientCommunicationManager!=NULL)
	{
		int guildID	= g_pPlayer->GetGuildNumber();
		if (guildID>0)
		{
			RCCharacterInfo _RCCharacterInfo;

			// 이름을 안 보내도 IP로 체크하기 때메 된다.
			//_RCStatusHP.setName( g_pUserInformation->CharacterID.GetString() );
			_RCCharacterInfo.setGuildID( guildID );
			
			for (int i=0; i<g_pParty->GetSize(); i++)
			{
				PARTY_INFO* pInfo = g_pParty->GetMemberInfo( i );
				
				if (pInfo!=NULL
					// 시야에 없는 경우에만 상태 정보를 보낸다.
					// 의미가 없을지도.. - -;
					//&& !pInfo->bInSight
					)
				{
					int port = g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT;

					RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pInfo->Name.GetString() );

					if (pUserInfo!=NULL)
					{
						if (pUserInfo->UDPPort!=0)
							port = pUserInfo->UDPPort;
					}

					g_pClientCommunicationManager->sendPacket( pInfo->IP, 
															port, 
															&_RCCharacterInfo );
				}
			}
		}
	}
}

//----------------------------------------------------------------------
// Is Block All Direction
//----------------------------------------------------------------------
extern POINT g_DirectionValue[MAX_DIRECTION];
bool
IsBlockAllDirection()
{
	if (g_pPlayer!=NULL && g_pZone!=NULL)
	{
		int x = g_pPlayer->GetX();
		int y = g_pPlayer->GetY();

		for (int i=0; i<8; i++)
		{
			const POINT& pt = g_DirectionValue[i];

			int sx = x + pt.x;
			int sy = y + pt.y;

			if (g_pZone->CanMove( g_pPlayer->GetMoveType(), sx, sy ))
			{
				return false;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------
// 
// contructor/destructor
//
//----------------------------------------------------------------------
MPlayer::MPlayer()
{	
	// 추적 정보
	m_fTrace				= FLAG_TRACE_NULL;
	m_TraceDistance			= 0;
	m_TraceObjectAction		= 0;
	m_fTraceBuffer			= FLAG_TRACE_NULL;

	m_BasicAttackDistance	= 1;

	// 다음 추적 정보 
	m_fNextTrace			= FLAG_TRACE_NULL;
	m_NextTraceID			= OBJECTID_NULL;
	m_NextTraceX			= SECTORPOSITION_NULL;
	m_NextTraceY			= SECTORPOSITION_NULL;
	m_NextTraceZ			= 0;
	m_NextTraceObjectAction	= 0;

	
	// 목적지	
	m_DestX = SECTORPOSITION_NULL;
	m_DestY = SECTORPOSITION_NULL;

	// 다음 목적지
	m_NextDestX = SECTORPOSITION_NULL;
	m_NextDestY = SECTORPOSITION_NULL;

	// 못 갔던 곳..
	m_BlockDestX = SECTORPOSITION_NULL;
	m_BlockDestY = SECTORPOSITION_NULL;

	// Server로 보낸 메세지 개수
	m_SendMove = 0;

	// Delay시간
	m_DelayTime	= 0;
	
	// attack mode
	m_AttackMode = ATTACK_MODE_NORMAL;

	// 방금 주운 item
	m_ItemCheckBufferStatus = ITEM_CHECK_BUFFER_NULL;
	m_pItemCheckBuffer = NULL;
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		m_dwSubItemIDCheckBuffer = OBJECTID_NULL;
	#endif
	// 기술 사용의 목표
	m_pEffectTarget = NULL;

	// server에서 기다리기
	m_WaitVerify = WAIT_VERIFY_NULL;
	m_WaitVerifyActionInfo = ACTIONINFO_NULL;

	// 계속 추적하기..
	m_bKeepTraceCreature = true;

	// 행동 반복
	m_bRepeatAction		= FALSE;

	// 뱀파이어로 변하는 시간
	m_ConversionDelayTime = 0;

	m_nNoPacketUsedActionInfo = ACTIONINFO_NULL;

	m_bLockMode = false;

	//-------------------------------------------------------
	// 빛의 크기 시야
	//-------------------------------------------------------
	m_TimeLightSight = 0;
//	m_TimeLightSightX = SECTORPOSITION_NULL;
//	m_TimeLightSightY = SECTORPOSITION_NULL;	
	m_ItemLightSight = 0;
	m_LightSight = 0;

	// 캐릭터 시야
	m_Sight = 0;
	m_PetDelayTime = 0;

	m_SweepVice_Value = 0;
	m_TempSelectPosX = SECTORPOSITION_NULL;
	m_TempSelectPosY = SECTORPOSITION_NULL;
	m_SweepViceX = SECTORPOSITION_NULL;
	m_SweepViceY = SECTORPOSITION_NULL;
	m_bShowAdamCloud = false;

	m_ResurrectZoneID = 0;
}

MPlayer::~MPlayer()
{
	// 으헐~~ 필살의 코드 - -;;
	// Effect가 delete되면서
	// player의 EffectTarget을 제거할려고 g_pPlayer로 접근하는 바람에..
	// 구조를 좀 바꿔야될텐데.. T_T;; 고칠 시간이.. - -;	
	g_pUserInformation->bCompetence = false;
	g_pUserInformation->bCompetenceShape = false;
	g_pPlayer = NULL;

	// remove priority queue 
	RemoveNodes();

	// remove list
	m_listDirection.clear();
	m_listSendDirection.clear();

	// 기술 사용 목표
	if (m_pEffectTarget!=NULL)
	{
		MEffectTarget* pTempEffectTarget = m_pEffectTarget;
		m_pEffectTarget = NULL;

		delete pTempEffectTarget;
	}
}


//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// MPlayer가 존재하는 Zone을 설정한다.
//----------------------------------------------------------------------
void	
MPlayer::SetZone(MZone* pZone)
{
	// base class functions
	MCreature::SetZone(pZone);

}

//----------------------------------------------------------------------
// Player를 멈추게 한다.
//----------------------------------------------------------------------
void
MPlayer::SetStop()
{ 
	// 길찾기 제거
	// 2004, 9, 1, sobeit add start - 서서 죽는거 방지?ㅋㅋ
	m_bKnockBack = 0;

	// 2004, 9, 1, sobeit add end

	m_listDirection.clear(); 

	// 추적 중지	
	m_fTrace		= FLAG_TRACE_NULL;
	//m_fNextTrace	= FLAG_TRACE_NULL;

	// Action 중지
	m_sX=0; 
	m_sY=0;

	int tempAction = m_MoveAction==ACTION_SLAYER_MOTOR_MOVE? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
	if(IsOusters() && HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
		tempAction = ACTION_OUSTERS_FAST_MOVE_STAND;

	m_Action		= tempAction;
	// 2004, 11, 3, sobeit modify start - m_ActionCount를 바뀐 액션의 맥스 카운트로 세팅 했음 - 아우스터즈 고스트 버그 땜시..ㅡㅡ;
	m_ActionCount	= m_ActionCountMax; 
	//m_ActionCount	= (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	// 2004, 11, 3, sobeit modify end
	m_MoveCount		= m_MoveCountMax; 

	// 다음 동작도 없앰
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;
	
	//m_nNextUsedActionInfo = ACTIONINFO_NULL;
	//m_nUsedActionInfo = ACTIONINFO_NULL;

	
	// 목표 제거
	m_DestX			= SECTORPOSITION_NULL; 
	m_DestY			= SECTORPOSITION_NULL; 
	m_NextDestX		= SECTORPOSITION_NULL; 
	m_NextDestY		= SECTORPOSITION_NULL; 
}


//----------------------------------------------------------------------
// priority queue인 m_pqDNodes를 지운다.
//----------------------------------------------------------------------
void
MPlayer::RemoveNodes()
{
	while (!m_pqDNodes.empty())
	{
		//DNode* pDNode = m_pqDNodes.top();
		m_pqDNodes.pop();
		//delete pDNode;		
	}


	// temp를 지운다.
	DNODE_LIST::iterator iNode = m_listDNodes.begin();

	while (iNode != m_listDNodes.end())
	{
		DNode* pNode = *iNode;
		delete pNode;
		iNode++;		
	}

	m_listDNodes.clear();
}

//----------------------------------------------------------------------
// (x,y)로 부터 m_Dest(X,Y)까지의 거리를 구한다.
//----------------------------------------------------------------------
// 가로,세로,대각선 모두 1이므로 가장 큰 값이 거리이고
// 목표와 같은 좌표가 있는게 더 좋으므로 아닌 경우 +1이 되도록 한다.
//----------------------------------------------------------------------
int
MPlayer::CalculateDistance(int x, int y)
{
	//int xx = x-m_DestX,
	//	yy = y-m_DestY;

	//return abs(xx)+abs(yy);

	//return sqrt(xx*xx + yy*yy);
//	return xx*xx + yy*yy;

//	/*
	int xd = abs(x-m_DestX),
		yd = abs(y-m_DestY);	 

	int plus = !(m_DestX==x) + !(m_DestY==y);
		
	if (xd>yd)
	{
		return xd*xd + plus;
	}
	else return yd*yd + plus;

	return 0;
//	*/
}

//----------------------------------------------------------------------
// Get Destination()
//----------------------------------------------------------------------
void
MPlayer::GetDestination(POINT &dest)
{
	dest.x = m_DestX;
	dest.y = m_DestY;
}

//----------------------------------------------------------------------
// Set Move Position
//----------------------------------------------------------------------
bool	
MPlayer::SetMovePosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// 살아있지 않은 경우..
	if	(
		(
		!m_bAlive
		// 관속에 있는 경우
		|| m_bInCasket
		// 땅 속에 있는 경우
		|| IsUndergroundCreature()
		|| HasEffectStatus(EFFECTSTATUS_SLEEP)
		)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		return false;
	}

	// 다음에 사용해야할 기술이 있는 경우...
	//if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	if (m_ActionCount>=m_ActionCountMax)
	{
		m_fTraceBuffer = FLAG_TRACE_NULL;
		m_TraceIDBuffer = OBJECTID_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		//if (m_ActionCount==m_ActionCountMax)
		//{			
			m_nUsedActionInfo = ACTIONINFO_NULL;

			/*
			DEBUG_ADD("Clear m_nUsedActionInfo");
		}
		else
		{
			DEBUG_ADD_FORMAT("Not Clear : action=%d, %d/%d", (int)m_Action, (int)m_ActionCount, (int)m_ActionCountMax);
		}
		*/
		TraceNULL();
	}
	
	return SetNextDestination(sX, sY);
}

//----------------------------------------------------------------------
// Set Next Destination
//----------------------------------------------------------------------
// 다음에 이동할 목표 위치를 결정한다.
// 목표위치가 갈 수 없는 곳인 경우... 어떻게 할 것인가?
//----------------------------------------------------------------------
bool	
MPlayer::SetNextDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{ 
	//DEBUG_ADD_FORMAT("[Set] NextDestination (%d, %d) --> (%d, %d)", m_X, m_Y, sX, sY);

	// 살아있지 않은 경우..
	if	(
		(!m_bAlive || HasEffectStatus(EFFECTSTATUS_SLEEP))
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		return false;
	}

	// 다음에 사용해야할 기술이 있는 경우...
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		m_fTraceBuffer = FLAG_TRACE_NULL;
		m_TraceIDBuffer = OBJECTID_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		if (m_ActionCount>=m_ActionCountMax)
		{			
			m_nUsedActionInfo = ACTIONINFO_NULL;

			DEBUG_ADD("Clear m_nUsedActionInfo");			
		}
		else
		{
			DEBUG_ADD_FORMAT("Not Clear : action=%d, %d/%d", (int)m_Action, (int)m_ActionCount, (int)m_ActionCountMax);			
		}
		
		TraceNULL();
	}
	
	// 추적 중이 아니면서
	// 이미 목표위치에 있는 경우는 길찾기 필요없다.
	if (m_fTrace==FLAG_TRACE_NULL && sX==m_X && sY==m_Y)
	{
		return false;
	}	

	// 잘못된 값
	// 이전에 못 갔던 곳..
	if (sX==SECTORPOSITION_NULL || sY==SECTORPOSITION_NULL
		|| sX==m_BlockDestX && sY==m_BlockDestY)
	{
		return false;
	}

	//---------------------------------------------------
	// [방법 1]
	// 목표지점으로 갈 수 없다면
	// 목표 지점과 가장 가까운 곳으로 이동하게 한다.
	//---------------------------------------------------
	/*
	TYPE_SECTORPOSITION destX = sX;
	TYPE_SECTORPOSITION	destY = sY;	

	//---------------------------------------------------
	// 갈 수 있는 목표지점을 지정한다.
	//---------------------------------------------------
	while (!m_pZone->CanMove(m_MoveType, destX, destY))
	{
		// 목표(sX,sY)에서 현재 위치로의 방향
		BYTE direction = MTopView::GetDirectionToPosition(sX, sY, m_X, m_Y);

		GetPositionToDirection(destX, destY, direction);

		// 완전히(!) 갈 수 없는 곳이다.
		if (destX==m_X && destY==m_Y)
			return false;
	}
	m_NextDestX = destX; 
	m_NextDestY = destY; 
	*/

	if(m_fTrace==FLAG_TRACE_NULL)
	{
		BOOL bCanStand = m_pZone->CanMove(m_MoveType, sX,sY);
		if(!bCanStand)
		{
			extern POINT g_DirectionValue[MAX_DIRECTION];
			
			POINT MovePoint;

			if( sY > GetY() )
				MovePoint.y = -1;
			else if( sY < GetY() )
				MovePoint.y = 1;
			else
				MovePoint.y = 0;

			if( sX > GetX() )
				MovePoint.x = -1;
			else if( sX < GetX() )
				MovePoint.x = 1;
			else
				MovePoint.x = 0;

			int limit = 20;	// 유효값 20번내에 못찾으면 포기

			while( bCanStand == FALSE && !( GetX() == sX && GetY() == sY ) &&
				sX >= 0 && sY >= 0 
				&& sX < m_pZone->GetWidth() && sY < m_pZone->GetHeight()
				&& limit-- > 0
				)
			{
				if( GetX() != sX )sX += MovePoint.x;
				if( GetY() != sY )sY += MovePoint.y;
				bCanStand = m_pZone->CanMove( m_MoveType, sX,sY );
			};

			if( bCanStand == FALSE || GetX() == sX && GetY() == sY)
				return false;
		}
	}
//	else
	//---------------------------------------------------
	// [방법 2]
	// 현재 위치에서 목표 지점으로 한 칸 이동..
	//
	// 단, 추적 중인 경우는... 관계없고.. 
	// 이동 중에서만...
	//---------------------------------------------------
//	if (m_fTrace==FLAG_TRACE_NULL && !m_pZone->CanMove(m_MoveType, sX, sY))
//	{
//		BYTE direction = MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
//
//		sX = m_X;
//		sY = m_Y;
//
//		GetPositionToDirection(sX, sY, direction);
//
//		// 새로운 목표 지점으로 못 가는 경우..
//		if (!m_pZone->CanMove(m_MoveType, sX, sY))
//		{
//			return false;
//		}
//	}


	
	m_NextDestX = sX; 
	m_NextDestY = sY; 

	return true;
}

//----------------------------------------------------------------------
// Reset Send Move
//----------------------------------------------------------------------
// server에서 기다리는 messasge는 없다고 표시한다.
//----------------------------------------------------------------------
void	
MPlayer::ResetSendMove()	
{ 
	g_bNetStatusGood=true; 
	m_SendMove = 0; 
	m_listSendDirection.clear(); 
}

//----------------------------------------------------------------------
// Get DeadDelay Last
//----------------------------------------------------------------------
// Player가 죽고 난 후, 기다리는 시간..
//----------------------------------------------------------------------
DWORD			
MPlayer::GetDeadDelayLast() const
{
	int second = m_DeadDelayTime - g_CurrentTime;

	if (second < 0) 
	{
		return 0;
	}
	
	return second/1000;
}

//----------------------------------------------------------------------
// Get NextDestination()
//----------------------------------------------------------------------
void
MPlayer::GetNextDestination(POINT &dest)
{
	dest.x = m_NextDestX;
	dest.y = m_NextDestY;
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 길찾기에 의해서 정해진 길들 중에서
// 바로 다음에 가야할 Sector에 대한 좌표를 넘겨준다.
//
// '현재위치'에서 '다음 방향'에 대해 적절한 변화를 더해준다.
//----------------------------------------------------------------------
bool
MPlayer::GetNextPosition(POINT &next)
{
	// 갈 길이 정해지지 않은 경우
	if (m_listDirection.empty())
	{
		//next.x = SECTORPOSITION_NULL;
		//next.y = SECTORPOSITION_NULL;
		return false;
	}

	
	// 갈 길이 정해진 경우, 다음 좌표를 return한다.

	// 현재 위치에서 다음 방향에 대해서 목표 설정
	next.x = m_X;
	next.y = m_Y;
	
	switch (m_listDirection.front())
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;				break;
		case DIRECTION_DOWN			:			next.y++;	break;
		case DIRECTION_UP			:			next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;				break;
	}

	return true;
}


//----------------------------------------------------------------------
// Set Destination(sX, sY)
//----------------------------------------------------------------------
// 목표위치를 지정하는 순간에 목표위치까지의
// Best Path를 결정해서 m_listDirection에 저장해둔다.
//----------------------------------------------------------------------
bool
MPlayer::SetDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	if(HasEffectStatus(EFFECTSTATUS_SLEEP)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

	// message : find path
	//DEBUG_ADD_FORMAT("........Find Path........");				

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD_FORMAT("SetDest(%d, %d)", sX, sY);
	#endif

	//-------------------------------------------------------
	// 살아있지 않은 경우..
	//-------------------------------------------------------
	if (!m_bAlive)
	{
		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("dead");
		#endif
		return false;
	}

	//-------------------------------------------------------
	// if not empty, delete all elements
	//-------------------------------------------------------
	if (!m_listDirection.empty())
	{
		m_listDirection.clear();
	}

		// 성물을 들었을때는 자기편 안전지대는 못간다!
	bool bHasRelic = 
		( m_bEffectStatus[EFFECTSTATUS_HAS_VAMPIRE_RELIC] || m_bEffectStatus[EFFECTSTATUS_HAS_SLAYER_RELIC] ) && 
		(g_pZone->GetSector(sX, sY).IsSafeSlayer() && IsSlayer() ||
		g_pZone->GetSector(sX, sY).IsSafeVampire() && IsVampire() ||
		g_pZone->GetSector(sX, sY).IsSafeOusters() && IsOusters()
		);

	bool bHasBloodBible =
	(
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA] || m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE] ||
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO] ||m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_INI] ||
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI] ||m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA] ||
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS] ||m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL] ||
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE] ||m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA] ||
	m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA] ||m_bEffectStatus[EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA] ) && 
		(g_pZone->GetSector(sX, sY).IsSafeSlayer() && IsSlayer() ||
		g_pZone->GetSector(sX, sY).IsSafeOusters() && IsOusters() ||
		g_pZone->GetSector(sX, sY).IsSafeVampire() && IsVampire()
		);

	if(bHasBloodBible)
	{
		g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_BLOOD_BIBLE].GetString());
	}

	if(bHasRelic)
	{
		g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_RELIC].GetString());
	}

	if( HasEffectStatus( EFFECTSTATUS_HAS_FLAG ) &&
		(g_pZone->GetSector(sX, sY).IsSafeSlayer() && IsSlayer() ||
		g_pZone->GetSector(sX, sY).IsSafeVampire() && IsVampire() ||
		g_pZone->GetSector(sX, sY).IsSafeOusters() && IsOusters()
		) )
	{
		bHasRelic = true;
		g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_FLAG].GetString() );
	}

	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (sX<0 || sY<0 
		|| sX>=m_pZone->GetWidth() || sY>=m_pZone->GetHeight()
		// [새기술] Sanctuary 로는 못 움직인다.
		|| g_pZone->GetSector(m_X, m_Y).HasSanctuary()
		|| bHasRelic
		|| bHasBloodBible
		)
	{
		// Zone영역 밖이면 check 안 할 경우
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;
		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("outOfZone or Sanctuary");
		#endif

		return false;

		// 현재 진행 방향으로 이동하게 하는 경우
		/*
		POINT position = g_pTopView->GetSelectedSector(g_x, g_y);		
		BYTE direction = GetDirectionToPosition(position.x, position.y);		

		// 방향에 따른 위치를 결정한다.		
		MCreature::GetNextPosition(direction, position);
		
		sX = position.x;
		sY = position.y;
		
		// 현재 진행 방향이 Zone을 벗어나는 경우
		if (sX<0 || sY<0 
			|| sX>=m_pZone->GetWidth() || sY>=m_pZone->GetHeight()) 
		{
			return false;
		}
		*/
	}

	//-------------------------------------------------------
	// 추적 중일 경우, 이미 사정 거리에 있을 때...
	//-------------------------------------------------------
	if (m_fTrace!=FLAG_TRACE_NULL
		&& max(abs(m_TraceX-m_X), abs(m_TraceY-m_Y))<=m_TraceDistance)
	{
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;

		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;
	
		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("already in dist");
		#endif

		return false;
	}

	/*
	// priority queue test code
	ofstream file("test.txt", ios::app);
	CString str;

	m_pqDNodes.push( new DNode(0,0,0,1) );
	m_pqDNodes.push( new DNode(0,0,0,4) );
	m_pqDNodes.push( new DNode(0,0,0,3) );
	m_pqDNodes.push( new DNode(0,0,0,8) );
	m_pqDNodes.push( new DNode(0,0,0,2) );
	m_pqDNodes.push( new DNode(0,0,0,4) );
	m_pqDNodes.push( new DNode(0,0,0,7) );

	while (!m_pqDNodes.empty())
	{
		DNode*	pDNode = m_pqDNodes.top();
		m_pqDNodes.pop();

		str.Format("Pop : (%d,%d) , [%d], dist=%d", pDNode->x, pDNode->y, pDNode->direction, pDNode->distance);
		
		file << str << endl;
	}

	return;
	*/

	//--------------------------------------------------------------
	// 갈 수 있는 곳이면
	//--------------------------------------------------------------
	// Best First Search
	//--------------------------------------------------------------
	BOOL	bCanStand = m_pZone->CanMove(m_MoveType, sX,sY);

#ifdef __EXPO_CLIENT__
	bCanStand |= (BOOL)g_UserInformation.Invisible;
#endif

	//--------------------------------------------------------------
	// 갈 수 있는 곳이거나
	// 추적중이면 Best Search~로 길을 찾는다.
	//--------------------------------------------------------------
	if (bCanStand || m_fTrace!=FLAG_TRACE_NULL) 
	{		
		//-------------------------------------------------------
		// 목표위치를 저장
		//-------------------------------------------------------
		m_DestX = sX;
		m_DestY = sY;

		//-------------------------------------------------------
		// priority queue를 초기화한다.
		//-------------------------------------------------------
		RemoveNodes();	

		//-------------------------------------------------------
		// visited를 모두 false로 해두고
		// 현재위치부터 방문하지 않은 곳들을 방문하기 시작한다.
		//-------------------------------------------------------
		POINT firstSector = g_pTopView->GetFirstSector();
		//int x0 = m_DestX - g_pClientConfig->MAX_FINDPATH_WIDTH, 
		//	x1 = m_DestX + g_pClientConfig->MAX_FINDPATH_WIDTH+1, 
		//	y0 = m_DestY - g_pClientConfig->MAX_FINDPATH_HEIGHT,
		//	y1 = m_DestY + g_pClientConfig->MAX_FINDPATH_HEIGHT+1;
		int x0 = firstSector.x - 1, 
			x1 = firstSector.x + g_SECTOR_WIDTH + 5, 
			y0 = firstSector.y - 1,
			y1 = firstSector.y + g_SECTOR_HEIGHT + 5;

		if (x0 < 0)						
			x0 = 0;
		if (x1 > g_pZone->GetWidth())	
			x1 = g_pZone->GetWidth();
		
		if (y0 < 0)				
			y0 = 0;
		if (y1 > g_pZone->GetHeight())	
			y1 = g_pZone->GetHeight();
		
		int size = x1-x0;	// byte수
		for (int i=y0; i<y1; i++)
		{			
			for (int j=x0; j<x1; j++)
			{
				//m_ppVisited[i][j] = false;
				g_pZone->UnSetVisitedFlag(j, i);
			}
			//memset( m_ppVisited[i]+x0, false, size );
		}
			

		//-------------------------------------------------------
		//
		// 현재 위치
		//
		//-------------------------------------------------------

		//-------------------------------------------------------
		// 목적지를 향한 방향부터 길찾기를 한다.		
		//-------------------------------------------------------
		static BYTE FirstDirection = GetDirectionToPosition(m_DestX, m_DestY);
		static TYPE_SECTORPOSITION previousDestX = SECTORPOSITION_NULL;
		static TYPE_SECTORPOSITION previousDestY = SECTORPOSITION_NULL;

		//-------------------------------------------------------
		// 방금 전의 길찾기 했던거랑 목표 위치가 다른 경우에만 
		// 목적지를 향한 방향을 설정한다.
		//-------------------------------------------------------
		if (previousDestX!=m_DestX || previousDestY!=m_DestY)
		{
			FirstDirection = GetDirectionToPosition(m_DestX, m_DestY);
			previousDestX = m_DestX;
			previousDestY = m_DestY;
		}
		//-------------------------------------------------------
		// 같은 목표를 향해 나아가는 경우는 
		// 현재 Player의 방향부터 살펴본다.
		//-------------------------------------------------------
		else
		{
			FirstDirection = m_Direction;
		}
		
		DNode*	currentDNode = new DNode(
									m_X,
									m_Y,
									FirstDirection,
									CalculateDistance(m_X, m_Y),
									0
									);
	
		//-------------------------------------------------------
		// listDNodes에 넣어서 나중에 지워준다.
		//-------------------------------------------------------
		m_listDNodes.push_back( currentDNode );

		currentDNode->SetParent(NULL);
		m_pqDNodes.push(currentDNode);	
		
		POINT	next;
		int		dist;
		bool	bFound = false;		// 아직 도달하지 않았다는 의미

		while (!bFound && !m_pqDNodes.empty())
		{
			//-------------------------------------------------------
			// 제일 괜찮은(!) Node를 골라서(priority queue이다)
			// 어디로 가야할지에 대해서 계산해본다.
			//-------------------------------------------------------
			DNode* pDNode = m_pqDNodes.top();
			m_pqDNodes.pop();

			//-------------------------------------------------------
			// 완전히 도착한 경우
			//-------------------------------------------------------
			if (pDNode->distance==0) 
			{
				m_pqDNodes.push(pDNode);
				bFound = true;
				break;
			}
			
			//-------------------------------------------------------
			// 사정 거리 이내로 접근하기
			//-------------------------------------------------------
			if (m_fTrace!=FLAG_TRACE_NULL && 
				max(abs(pDNode->x-m_DestX), abs(pDNode->y-m_DestY))<=m_TraceDistance)
			{
				m_pqDNodes.push(pDNode);
				bFound = true;
				break;
			}			

			int Direction = pDNode->direction;

			//-------------------------------------------------------
			// 8방향을 모두 check
			//-------------------------------------------------------
			for (int i=0; i<8; Direction++, i++)
			{		
				if (Direction==8) Direction=0;

				next.x = pDNode->x;
				next.y = pDNode->y;

				switch (Direction)
				{
					case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
					case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
					case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
					case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
					case DIRECTION_LEFT			: next.x--;				break;
					case DIRECTION_DOWN			: next.y++;				break;
					case DIRECTION_UP			: next.y--;				break;
					case DIRECTION_RIGHT		: next.x++;				break;
				}

				//-------------------------------------------------------
				// Zone의 영역 밖이면 check 안한다.
				//-------------------------------------------------------
				if (next.x<0 || next.y<0 
					|| next.x>=m_pZone->GetWidth() || next.y>=m_pZone->GetHeight()) continue;

				//-------------------------------------------------------
				// 갔던 곳이면 안 간다.
				//-------------------------------------------------------
				if (g_pZone->IsVisitedFlag(next.x, next.y)) continue;

				// 한 화면을 넘어가는 경우는 check하지 않는다.				
				if (next.x<x0 || next.y<y0 
					|| next.x>=x1 || next.y>=y1) continue;

				//-------------------------------------------------------
				// 갈 수 있으면 pqueue에 넣는다.
				//-------------------------------------------------------
				if (m_pZone->CanMove(m_MoveType, next.x, next.y)
					&& !bHasRelic && !bHasBloodBible

#ifdef __EXPO_CLIENT__
	|| g_UserInformation.Invisible
#endif
					
					)
				{		
					g_pZone->SetVisitedFlag(next.x, next.y);
//					m_ppVisited[next.y][next.x] = true;

					//-------------------------------------------------------
					// Node를 만들어서 priority queue에 추가한다.
					//-------------------------------------------------------
					// 거리 계산
					dist = CalculateDistance(next.x, next.y);

					// Node를 만들어서 추가
					DNode*	nextDNode = new DNode(
											next.x, 
											next.y, 
											Direction,
											dist,
											pDNode->step + 1);
					
				
					//-------------------------------------------------------
					// listDNodes에 넣어서 나중에 지워준다.
					//-------------------------------------------------------
					m_listDNodes.push_back( nextDNode );
					
					//CString str;
					//str.Format("NewNode : (%d,%d)-->(%d,%d) , [%d], dist=%d", next.x, next.y, m_Dest.x, m_Dest.y, i, dist);

					nextDNode->SetParent(pDNode);
					m_pqDNodes.push(nextDNode);					
				}

			
			}
		}

		//-------------------------------------------------------
		// 찾았을 경우 길을 정해야 한다.
		//-------------------------------------------------------
		if (bFound)
		{				
			//-------------------------------------------------------
			// m_pqDNodes로부터 list를 채운다. (parent를 따라가야한다)
			//-------------------------------------------------------

			// 목표위치
			DNode* currentNode = m_pqDNodes.top();
			m_pqDNodes.pop();
			
			while (currentNode->pParent!=NULL)
			{
				m_listDirection.insert( m_listDirection.begin(), currentNode->direction);
				currentNode = currentNode->pParent;			
			}

			RemoveNodes();

			m_BlockDestX = SECTORPOSITION_NULL;
			m_BlockDestY = SECTORPOSITION_NULL;
		}
		//-------------------------------------------------------
		// 길이 없을 경우 정지한다.
		//-------------------------------------------------------
		else
		{
			// 추적 중지
			if (m_fTrace!=FLAG_TRACE_NULL)				
			{
				m_MoveCount = m_MoveCountMax;

				TraceNULL();

				// 2001.7.31 추가
				m_nUsedActionInfo = ACTIONINFO_NULL;
				m_nNextUsedActionInfo = ACTIONINFO_NULL;
			}

			m_BlockDestX = m_DestX;
			m_BlockDestY = m_DestY;

			m_DestX = SECTORPOSITION_NULL;
			m_DestY = SECTORPOSITION_NULL;			
		}
	}
	// 갈 수 없는 곳
	else 
	{
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;

		// 이번에 갈 수 없었던 곳 설정
		//m_BlockDestX = m_DestX;
		//m_BlockDestY = m_DestY;

		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;		

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("no way!");
		#endif

		return false;
	}

	/*
	else
	// 이 부분에 약간의 오류가 있는 듯 함!! -_-;;;;
	//--------------------------------------------------------------
	// 아예 갈 수 없는 곳이면..
	//--------------------------------------------------------------
	// Player위치에서 목표지점까지 직선?으로 걸어가다가 
	// 장애물이 생기면 멈춘다.
	// (m_X, m_Y)  --->  (sX, sY)
	//--------------------------------------------------------------		
	{
		int	stepX = sX - m_X,
			stepY = sY - m_Y,
			x = m_X,
			y = m_Y,
			signX, signY;

		//-------------------------------------------------------
		// if not empty, delete all elements
		//-------------------------------------------------------
		if (!m_listDirection.empty())
		{
			m_listDirection.clear();
		}
			
		
		while (1)
		{
			// 부호에 따라서..
			signX = (stepX==0)? 0 : (stepX<0)? -1 : 1;
			signY = (stepY==0)? 0 : (stepY<0)? -1 : 1;

			x += signX;
			y += signY;
				
			// 갈 수 있는 곳이면 list에 넣는다.
			if (m_pZone->CanMove(x,y))
			{				
				m_listDirection.insert( m_listDirection.end(), DetermineDirection(stepX, stepY));
			}
			else 
			{
				// x,y 한칸 전의 위치를 목표지점으로 정한다.
				m_DestX = x - signX;
				m_DestY = y - signY;
				break;
			}	
			
			// 한 칸 움직였음을 표시
			stepX -= signX;
			stepY -= signY;
		}				
		
		return;
	}
	*/

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("find way");
	#endif

	return true;
}

//----------------------------------------------------------------------
// Self Special Action
//----------------------------------------------------------------------
// 자신한테 특수기술 사용
//----------------------------------------------------------------------
bool	
MPlayer::SelfSpecialAction()
{
	if(HasEffectStatus(EFFECTSTATUS_SLEEP)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() || HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE ) )
	{
		return false;
	}

	// [새기술3] 관 속에 있는 경우는 Open casket만 사용할 수 있다.
	//if (IsInCasket())
//	{
//		m_nSpecialActionInfo = MAGIC_OPEN_CASKET;
//	}

	
	if (m_nSpecialActionInfo == ACTIONINFO_NULL
		// 오토바이를 타고 있으면 특수공격이 안된다.
		|| m_MoveDevice == MOVE_DEVICE_RIDE
		// 뭔가 기술 사용을 검정 받아야 하면 사용할 수 없다.
		//|| m_WaitVerify != WAIT_VERIFY_NULL)	// 2001.8.20 주석처리
		|| IsInSafeSector() == 2
		// 완전 안전지대에서는 기술을 사용할 수 없다.
		)
		return false;

	// 자신에게 사용할 수 있는 것들만 사용한다.
	if ((*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetSelf())
	{	
		
		//-------------------------------------------------------
		// 지뢰 만들기
		//-------------------------------------------------------
		// skillID            itemType
		// MINE_ANKLE_KILLER,	// 5
		// MINE_POMZ,			// 6
		// MINE_AP_C1,			// 7
		// MINE_DIAMONDBACK,	// 8
		// MINE_SWIFT_EX		// 9
		//
		// 하드하드...
		//			
		if (m_nSpecialActionInfo==SKILL_INSTALL_MINE
			|| m_nSpecialActionInfo>=MINE_ANKLE_KILLER 
				&& m_nSpecialActionInfo<=MINE_SWIFT_EX)
		{
			int					useSkill = SKILL_INSTALL_MINE;					

			//-------------------------------------------------------
			// 현재 사용할 수 있는 기술인지 체크..
			// Passive 스킬이면 사용 못하게..
			//-------------------------------------------------------
			if (g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)useSkill )
				&& (*g_pSkillInfoTable)[useSkill].IsEnable()
				&& (*g_pSkillInfoTable)[useSkill].IsAvailableTime()
				&& !(*g_pSkillInfoTable)[useSkill].IsPassive()
				&& IsSlayer()
				&& !IsWaitVerify()
				&& IsItemCheckBufferNULL())
			{					

				ITEM_CLASS	itemClass = ITEM_CLASS_MINE;

				MItem* pItem = NULL;

				if (m_nSpecialActionInfo!=SKILL_INSTALL_MINE)
				{
					// 특정 type의 지뢰를 찾는다.
					int itemType = m_nSpecialActionInfo-MINE_ANKLE_KILLER;

					MItemClassTypeFinder itemFinder(itemClass, itemType);
					pItem = ((MItemManager*)g_pInventory)->FindItem( itemFinder );
				}
				else
				{
					// 지뢰 암꺼나 찾는다.
					MItemClassFinder itemFinder(itemClass);
					pItem = ((MItemManager*)g_pInventory)->FindItem( itemFinder );
				}
				
				// 쓸려는 폭탄이 있는 경우
				if (pItem!=NULL)
				{
					// 지뢰 설치 준비..
					gC_vs_ui.StartInstallMineProgress( pItem->GetGridX(), pItem->GetGridY() );
				}									
			}

			// 반복동작을 설정하지 않는다.
			m_fNextTrace = FLAG_TRACE_NULL;
			UnSetRepeatAction();
			return false;	
		}

		//-------------------------------------------------------
		// 현재 사용할 수 있는 기술인지 체크..
		// Passive 스킬이면 사용 못하게..
		//-------------------------------------------------------
		if (!g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)m_nSpecialActionInfo )
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsEnable()
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsAvailableTime()
			|| (*g_pSkillInfoTable)[m_nSpecialActionInfo].IsPassive())
		{
			m_fNextTrace = FLAG_TRACE_NULL;

			UnSetRepeatAction();

			return false;
		}

		if (IsStop()) // || m_Action!=ACTION_ATTACK)
		{
			//-------------------------------------------------------
			// 특정한 기술인 경우의 체크
			//-------------------------------------------------------
			switch (m_nSpecialActionInfo)
			{
				//-------------------------------------------------------
				// 포탈
				//-------------------------------------------------------
				case MAGIC_BLOODY_MARK :
					if (IsVampire() && g_bHolyLand == false && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
					{
						//----------------------------------------------------
						// Server에 접속한 경우
						//----------------------------------------------------
							//----------------------------------------------------
							// 검증 받을게 없는 경우..
							//----------------------------------------------------
							if (!IsWaitVerify()
								&& IsItemCheckBufferNULL())
							{									
								// 사용안된(Marked=false) VampirePortalItem을 찾는다.

							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								MItem* pSubInventory = NULL;
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItemAll( MVampirePortalItemFinder( false ) , pSubInventory );
							#else
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( MVampirePortalItemFinder( false ) );
							#endif
								
								
								
								if (pItem!=NULL && pItem->GetNumber()>0 && pItem->IsAffectStatus())
								{	
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										DWORD SubInventoryItemID = 0;
									#endif
									//----------------------------------------------------
									// Server에 접속한 경우
									//----------------------------------------------------
										CGSkillToInventory _CGSkillToInventory;
										_CGSkillToInventory.setObjectID( pItem->GetID() );
										_CGSkillToInventory.setX( pItem->GetGridX() );
										_CGSkillToInventory.setY( pItem->GetGridY() );
										_CGSkillToInventory.setSkillType( MAGIC_BLOODY_MARK );
										//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;							
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										if(NULL != pSubInventory)
										{
											SubInventoryItemID = pSubInventory->GetID();
										}
										_CGSkillToInventory.setInventoryItemObjectID( SubInventoryItemID );
									#endif
										g_pSocket->sendPacket( &_CGSkillToInventory );

										//----------------------------------------------------
										// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
										//----------------------------------------------------
										SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS );

									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY, SubInventoryItemID);
									#else
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY);
									#endif


										(*g_pSkillInfoTable)[m_nSpecialActionInfo].SetNextAvailableTime();
								}
							}
					}						
					
					return false;
				break;

				//-------------------------------------------------------
				// 포탈
				//-------------------------------------------------------
				case MAGIC_BLOODY_TUNNEL :					
					if (IsVampire() && g_bHolyLand == false && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
					{
							//----------------------------------------------------
							// 검증 받을게 없는 경우..
							//----------------------------------------------------
							if (!IsWaitVerify()
								&& IsItemCheckBufferNULL())
							{		
								// 사용된(Marked=true) VampirePortalItem을 찾는다.
							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								MItem* pSubInventory = NULL;
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItemAll( MVampirePortalItemFinder( true ) , pSubInventory );
							#else
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( MVampirePortalItemFinder( true ) );
							#endif
								
								
								if (pItem!=NULL && pItem->GetNumber()>0 && pItem->IsAffectStatus())
								{				
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										DWORD SubInventoryItemID = 0;
									#endif
										CGSkillToInventory _CGSkillToInventory;
										_CGSkillToInventory.setObjectID( pItem->GetID() );
										_CGSkillToInventory.setX( pItem->GetGridX() );
										_CGSkillToInventory.setY( pItem->GetGridY() );
										_CGSkillToInventory.setSkillType( MAGIC_BLOODY_TUNNEL );
										//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;							
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										if(NULL != pSubInventory)
										{
											SubInventoryItemID = pSubInventory->GetID();
										}
										_CGSkillToInventory.setInventoryItemObjectID( SubInventoryItemID );
									#endif
										g_pSocket->sendPacket( &_CGSkillToInventory );

										//----------------------------------------------------
										// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
										//----------------------------------------------------
										SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );

									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY, SubInventoryItemID);
									#else
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY);
									#endif
										

										(*g_pSkillInfoTable)[m_nSpecialActionInfo].SetNextAvailableTime();

								}
							}
					}						
					
					return false;
				break;

				//-------------------------------------------------------
				// 포탈
				//-------------------------------------------------------
				case SUMMON_HELICOPTER :
					if (IsSlayer() && g_bHolyLand == false && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
					{
							//----------------------------------------------------
							// 검증 받을게 없는 경우..
							//----------------------------------------------------
							if (IsWaitVerifyNULL()
								&& IsItemCheckBufferNULL()
								&& g_pZone->GetHelicopter( GetID() )==NULL)				
							{		

							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								MItem* pSubInventory = NULL;
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItemAll( MSlayerPortalItemFinder() , pSubInventory);
							#else
								MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( MSlayerPortalItemFinder() );
							#endif
								
								
								if (pItem!=NULL && pItem->GetNumber()>0 && pItem->IsAffectStatus())
								{				
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										DWORD SubInventoryItemID = 0;
									#endif
										CGUseItemFromInventory _CGUseItemFromInventory;
										_CGUseItemFromInventory.setObjectID( pItem->GetID() );
										_CGUseItemFromInventory.setX( pItem->GetGridX() );
										_CGUseItemFromInventory.setY( pItem->GetGridY() );

									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										if(NULL != pSubInventory)
										{
											SubInventoryItemID = pSubInventory->GetID();
										}
										_CGUseItemFromInventory.setInventoryItemObjectID( SubInventoryItemID );
									#endif

										g_pSocket->sendPacket( &_CGUseItemFromInventory );

										//----------------------------------------------------
										// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
										//----------------------------------------------------
									#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY, SubInventoryItemID);
									#else
										SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
									#endif
										
								}
							}
					}						
					
					return false;
				break;

				//-------------------------------------------------------
				// 성수 만들기
				//-------------------------------------------------------
				case MAGIC_CREATE_HOLY_WATER :				
					if (
						IsSlayer()
						&& !IsWaitVerify()
						&& IsItemCheckBufferNULL())
					{					
						MItem* pItem = NULL;

						for( int i = 0; i <= 2; i++ )
						{
							pItem = g_pInventory->FindItem( ITEM_CLASS_WATER,  i);
							if( pItem != NULL )
								break;
						}

						if (pItem!=NULL)
						{
//							POINT fitPoint;			// 성수가 들어갈 자리

//							if (GetMakeItemFitPosition(pItem, 
//														ITEM_CLASS_HOLYWATER, 
//														pItem->GetItemType(), 
//														fitPoint))
							{
								CGSkillToInventory _CGSkillToInventory;
								_CGSkillToInventory.setObjectID( pItem->GetID() );
								_CGSkillToInventory.setX( pItem->GetGridX() );
								_CGSkillToInventory.setY( pItem->GetGridY() );
								_CGSkillToInventory.setTargetX( pItem->GetGridX() );
								_CGSkillToInventory.setTargetY( pItem->GetGridY() );
								_CGSkillToInventory.setSkillType( MAGIC_CREATE_HOLY_WATER );
								//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;							

								g_pSocket->sendPacket( &_CGSkillToInventory );

								
								// 일단(!) 그냥 없애고 본다.
								//(*g_pInventory).RemoveItem( pItem->GetID() );

								//----------------------------------------------------
								// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
								//----------------------------------------------------
								SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );
								SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY );

								(*g_pSkillInfoTable)[MAGIC_CREATE_HOLY_WATER].SetNextAvailableTime();

								//----------------------------------------------------
								// 기술 사용 시도 동작
								//----------------------------------------------------
								AddNewInventoryEffect( pItem->GetID(),
													MAGIC_CREATE_HOLY_WATER, //+ (*g_pActionInfoTable).GetMinResultActionInfo(),
													g_pClientConfig->FPS*3	// 3초
												);
								// 반복동작을 설정하지 않는다.
								m_fNextTrace = FLAG_TRACE_NULL;
								UnSetRepeatAction();
							}
//							else
//							{
//								// g_pGameMessage( "자리가 없따야~" );
//							}
						}						
					}
					
					return false;					
				break;
			case SKILL_CREATE_HOLY_POTION :				
					if (
						IsSlayer()
						&& !IsWaitVerify()
						&& IsItemCheckBufferNULL())
					{					
						
						MItem* pItem = NULL;

						for( int i = 3; i <= 6; i++ )
						{
							pItem = g_pInventory->FindItem( ITEM_CLASS_WATER,  i);
							if( pItem != NULL )
								break;
						}

						if (pItem!=NULL)
						{
							POINT fitPoint;			// 성수가 들어갈 자리

							if (GetMakeItemFitPosition(pItem, 
														ITEM_CLASS_POTION, 
														(*g_pItemTable)[pItem->GetItemClass()][pItem->GetItemType()].Value3, 
														fitPoint))
							{
								CGSkillToInventory _CGSkillToInventory;
								_CGSkillToInventory.setObjectID( pItem->GetID() );
								_CGSkillToInventory.setX( pItem->GetGridX() );
								_CGSkillToInventory.setY( pItem->GetGridY() );
								_CGSkillToInventory.setTargetX( fitPoint.x );
								_CGSkillToInventory.setTargetY( fitPoint.y );
								_CGSkillToInventory.setSkillType( SKILL_CREATE_HOLY_POTION );
								//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;							

								g_pSocket->sendPacket( &_CGSkillToInventory );

								
								// 일단(!) 그냥 없애고 본다.
								//(*g_pInventory).RemoveItem( pItem->GetID() );

								//----------------------------------------------------
								// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
								//----------------------------------------------------
								SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );
								SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY );

								(*g_pSkillInfoTable)[SKILL_CREATE_HOLY_POTION].SetNextAvailableTime();

								//----------------------------------------------------
								// 기술 사용 시도 동작
								//----------------------------------------------------
								AddNewInventoryEffect( pItem->GetID(),
													SKILL_CREATE_HOLY_POTION, //+ (*g_pActionInfoTable).GetMinResultActionInfo(),
													g_pClientConfig->FPS*3	// 3초
												);
								// 반복동작을 설정하지 않는다.
								m_fNextTrace = FLAG_TRACE_NULL;
								UnSetRepeatAction();
							}
							else
							{
								// g_pGameMessage( "자리가 없따야~" );
							}
						}						
					}
					
					return false;					
				break;

				//-------------------------------------------------------
				// 폭탄 만들기
				// 지뢰 만들기
				//-------------------------------------------------------
				case SKILL_MAKE_BOMB :
				case SKILL_MAKE_MINE :
					if (IsSlayer()
						&& !IsWaitVerify()
						&& IsItemCheckBufferNULL()
						&& !gC_vs_ui.IsInstallMineProgress())	// 지뢰 만드는 중이면..
						//|| gC_vs_ui.IsCreateMineProgress()	// 지뢰 만드는 중이면..
						//|| gC_vs_ui.IsCreateBombProgress()	// 지뢰 만드는 중이면..
					{					

						int					skillID;
						ITEM_CLASS	itemClass;
						int					minItemType;
						int					maxItemType;
						int					itemTypeModifier;
							
						// itemType이 0~4까지이면 폭탄이다.
						if (m_nSpecialActionInfo==SKILL_MAKE_BOMB)
						{
							skillID = SKILL_MAKE_BOMB;
							itemClass = ITEM_CLASS_BOMB;
							minItemType = 0;
							maxItemType = 4;
							itemTypeModifier = 0;
						}
						else
						{
							skillID = SKILL_MAKE_MINE;
							itemClass = ITEM_CLASS_MINE;
							minItemType = 5;
							maxItemType = 9;
							itemTypeModifier = -5;
						}

						MItemClassRangeTypeFinder itemFinder(ITEM_CLASS_BOMB_MATERIAL,
																minItemType, maxItemType);

						MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( itemFinder );

						if (pItem!=NULL)
						{
							POINT fitPoint;			// 들어갈 자리

							if (GetMakeItemFitPosition(pItem, 
														itemClass, 
														pItem->GetItemType() + itemTypeModifier, 
														fitPoint))
							{
								// 만들기 시작하는 순간...
								if (skillID==SKILL_MAKE_BOMB)
								{
									gC_vs_ui.StartCreateBombProgress( pItem->GetGridX(), pItem->GetGridY() );
								}
								else
								{
									gC_vs_ui.StartCreateMineProgress( pItem->GetGridX(), pItem->GetGridY() );
								}

								// UI에서 막대기 보여주고 나서
								// UI_ITEM_USE가 날아오는 부분에서 처리한다.
								
								// 연속적으로 사용하기 위해서
//								m_fNextTrace	= FLAG_TRACE_SELF;
//				
//								m_bRepeatAction = FALSE;
								
								// 반복동작을 설정하지 않는다.
								m_fNextTrace = FLAG_TRACE_NULL;
								UnSetRepeatAction();
								
								/*
								CGSkillToInventory _CGSkillToInventory;
								_CGSkillToInventory.setObjectID( pItem->GetID() );
								_CGSkillToInventory.setX( pItem->GetGridX() );
								_CGSkillToInventory.setY( pItem->GetGridY() );
								_CGSkillToInventory.setTargetX( fitPoint.x );
								_CGSkillToInventory.setTargetY( fitPoint.y );
								_CGSkillToInventory.setSkillType( skillID );
								//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;							

								g_pSocket->sendPacket( &_CGSkillToInventory );

								
								// 일단(!) 그냥 없애고 본다.
								//(*g_pInventory).RemoveItem( pItem->GetID() );

								//----------------------------------------------------
								// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
								//----------------------------------------------------
								SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );
								SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY );

								(*g_pSkillInfoTable)[skillID].SetNextAvailableTime();

								//----------------------------------------------------
								// 기술 사용 시도 동작
								//----------------------------------------------------
								AddNewInventoryEffect( pItem->GetID(),
													skillID, //+ (*g_pActionInfoTable).GetMinResultActionInfo(),
													g_pClientConfig->FPS*3	// 3초
												);
								*/
							}
							else
							{
								// g_pGameMessage( "자리가 없따야~" );
							}
						}						
					}
					
					return false;					
				break;

				//-------------------------------------------------------
				// 박쥐나 늑대 변신
				//-------------------------------------------------------
				case SKILL_TRANSFORM_TO_WERWOLF :
				case MAGIC_TRANSFORM_TO_WOLF :
				case MAGIC_TRANSFORM_TO_BAT :					
					//----------------------------------------------------
					//
					//					변신용 아이템 - vampire인 경우
					//
					//----------------------------------------------------
					if (IsVampire() && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
					{
							//----------------------------------------------------
							// 검증 받을게 없는 경우..
							//----------------------------------------------------
							if (!IsWaitVerify()
								&& IsItemCheckBufferNULL())
							{		
								MItem* pItem = NULL;

							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								MItem* pSubInventory = NULL;
							#endif

								if (m_nSpecialActionInfo==MAGIC_TRANSFORM_TO_WOLF)
								{
									// 늑대 발톱 찾기
									pItem = g_pInventory->FindItem(ITEM_CLASS_VAMPIRE_ETC, 0);
								}
								else if ( m_nSpecialActionInfo == SKILL_TRANSFORM_TO_WERWOLF )
								{
									pItem = g_pInventory->FindItem(ITEM_CLASS_SKULL, 39);
								}
								else
								{
								#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
									pItem = g_pInventory->FindItemAll( MItemClassTypeFinder(ITEM_CLASS_VAMPIRE_ETC , 1), pSubInventory );
								#else
									pItem = g_pInventory->FindItem(ITEM_CLASS_VAMPIRE_ETC, 1);
								#endif
									
								}

								if (pItem!=NULL)
								{			
								#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
									DWORD dwSubInventoryID = 0;
								#endif
									CGSkillToInventory _CGSkillToInventory;
									_CGSkillToInventory.setObjectID( pItem->GetID() );
									_CGSkillToInventory.setX( pItem->GetGridX() );
									_CGSkillToInventory.setY( pItem->GetGridY() );
									_CGSkillToInventory.setSkillType( m_nSpecialActionInfo );
									//_CGSkillToInventory.setCEffectID( 0 );	// -_-;;	
									
								#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
									if(NULL != pSubInventory)
										dwSubInventoryID = pSubInventory->GetID();

									_CGSkillToInventory.setInventoryItemObjectID(dwSubInventoryID);
								#endif

									g_pSocket->sendPacket( &_CGSkillToInventory );


									// 일단(!) 그냥 없애고 본다.
									//(*g_pInventory).RemoveItem( pItem->GetID() );

									//----------------------------------------------------
									// Inventory에서 item을 사용하는 걸 검증받기를 기다린다.
									//----------------------------------------------------
									SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );
									
								#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
									SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY , dwSubInventoryID);
								#else
									SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY );
								#endif
									

									(*g_pSkillInfoTable)[m_nSpecialActionInfo].SetNextAvailableTime();

									//----------------------------------------------------
									// 기술 사용 시도 동작
									//----------------------------------------------------
									AddNewInventoryEffect( pItem->GetID(),
														m_nSpecialActionInfo, //+ (*g_pActionInfoTable).GetMinResultActionInfo(),
														g_pClientConfig->FPS*3	// 3초
													);
								}
							}
					}						
					
					return false;
				break;

				//----------------------------------------------------
				//
				// 변신 풀기
				// 
				//----------------------------------------------------
				case MAGIC_UN_TRANSFORM :
					if (IsVampire())
					{
						const MSector& sector = m_pZone->GetSector(m_X, m_Y);
						
						if (sector.IsBlockGround())
						{
							m_fNextTrace = FLAG_TRACE_NULL;

							UnSetRepeatAction();

							return false;
						}						
					}
				break;
				
				case SKILL_SUMMON_SYLPH :
					if( IsOusters() && !HasEffectStatus( EFFECTSTATUS_HAS_FLAG )
						//----------------------------------------------------
						// 검증 받을게 없는 경우..
						//----------------------------------------------------
						&& IsWaitVerifyNULL()
						&& IsItemCheckBufferNULL()
						)
					{
						//MItem* pItem = g_pInventory->FindItem( ITEM_CLASS_OUSTERS_SUMMON_ITEM );

					#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
						MItem* pSubInventory = NULL;
						MItem* pItem = ((MItemManager*)g_pInventory)->FindItemAll( MOustersSummonGemItemFinder(), pSubInventory );
					#else
						MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( MOustersSummonGemItemFinder() );
					#endif
						
						if( pItem != NULL )
						{
							if( pItem->IsAffectStatus() && pItem->IsChargeItem() && pItem->GetNumber() > 0 )
							{
								
							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								DWORD SubInventoryItemID = 0;
							#endif

								CGUseItemFromInventory _CGUseItemFromInventory;
								_CGUseItemFromInventory.setObjectID( pItem->GetID() );
								_CGUseItemFromInventory.setX( pItem->GetGridX() );
								_CGUseItemFromInventory.setY( pItem->GetGridY() );

							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								if(NULL != pSubInventory)
								{
									SubInventoryItemID = pSubInventory->GetID();
								}

								_CGUseItemFromInventory.setInventoryItemObjectID( SubInventoryItemID );
							#endif

								g_pSocket->sendPacket( &_CGUseItemFromInventory );

							#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
								SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY, SubInventoryItemID);
							#else
								SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
							#endif

								(*g_pSkillInfoTable)[SKILL_SUMMON_SYLPH].SetAvailableTime( 4000 );
							}
						} else
						{
							
						}
					}
					return false;
				break;
				//-----------------------------------------------------
				//
				// 추적
				//
				//-----------------------------------------------------
				case SKILL_SOUL_CHAIN :
					// 뱀파이어 슬레이어 구분이 없다.

					if (!IsWaitVerify())
					{
						// UI 창을 띄워준다.
						// UI 창을 띄운 뒤 메시지가 날아오면 거기서 sendpacket 을 해준다.
						gC_vs_ui.RunTraceWindow();
						
//						int skillID = SKILL_SOUL_CHAIN;
//						
//						if (skillID < MIN_RESULT_ACTIONINFO)
//						{
//							(*g_pSkillInfoTable)[skillID].SetAvailableTime();
//						}
						
					}
					return false;
					
					break;

				case SKILL_LOVE_CHAIN :
					//----------------------------------------------------
					// 검증 받을게 없는 경우..
					//----------------------------------------------------
					if (!IsWaitVerify()
						&& IsItemCheckBufferNULL())
					{	
						ITEM_CLASS ItemClass;
						switch( GetRace() )
						{
						case RACE_SLAYER :
							ItemClass = ITEM_CLASS_COUPLE_RING;
							break;
						case RACE_VAMPIRE :
							ItemClass = ITEM_CLASS_VAMPIRE_COUPLE_RING;
							break;
						default :
							ItemClass = ITEM_CLASS_COUPLE_RING;
							break;
						}
						MPlayerGear* pGear = GetGear();
						MItemClassFinder itemFinder( ItemClass );
						
						MItem *pItem = pGear->FindItem( itemFinder );
						if(pItem == NULL || !pItem->IsAffectStatus())
							return false;

						CGUseItemFromGear _CGUseItemFromGear;

						_CGUseItemFromGear.setObjectID ( pItem->GetID() );
						_CGUseItemFromGear.setPart ( pItem->GetItemSlot() );
						
						g_pSocket->sendPacket( &_CGUseItemFromGear );

						SetItemCheckBuffer( pItem, ITEM_CHECK_BUFFER_USE_FROM_GEAR );
						return false;
					}
					break;
				// add by Coffee 2007-6-9 藤속賈痰劤세콘膠틔왱뇜
// 				case SKILL_BLLODY_SCARIFY:
// 				case SKILL_BLOOD_CURSE:
// 					if (IsVampire() && !IsWaitVerify() && IsItemCheckBufferNULL())
// 					{
// 						MItem* pItem = NULL;
// 
// 					}
// 					return false;
// 					break;
// 				case SKILL_SHINE_SWORD:
// 				case SKILL_BOMB_CRASH_WALK:
// 				case SKILL_SATELLITE_BOMB:
// 				case SKILL_ILLUSION_INVERSION:
// 				case SKILL_HEAVEN_GROUND:
// 				case SKILL_DUMMY_DRAKE:
// 				case SKILL_HYDRO_CONVERGENCE:
// 				case SKILL_SUMMON_CLAY:
// 				case SKILL_HETER_CHAKRAM:
// 					break;
				// end add By Coffee 2007-6-9
			}			

			if( (*g_pActionInfoTable)[m_nSpecialActionInfo].GetParentActionInfo() != ACTIONINFO_NULL )
				m_nSpecialActionInfo = (*g_pActionInfoTable)[m_nSpecialActionInfo].GetParentActionInfo();

			// 사용한 기술로 설정
			m_nUsedActionInfo	= m_nSpecialActionInfo;

			// 추적 정보 초기화
			SetTraceID( m_ID );
			m_fTrace	= FLAG_TRACE_SELF;
			m_TraceX	= m_X;//SECTORPOSITION_NULL;
			m_TraceY	= m_Y;//SECTORPOSITION_NULL;
			m_TraceZ	= 0;

			// 현재 추적하는 대상에 대해서 기억해둔다.
			m_fTraceBuffer	= m_fTrace;	

			//------------------------------------------------------------
			// BasicAction이 뭐냐에 따라서..
			// 현재 Player의 능력에 따라서 TraceDistance가 달라질 것이다.
			//------------------------------------------------------------
			
			m_TraceDistance		= 0;
			
			//------------------------------------------------------------
			// 기술 사용 Action표현
			SetNextAction( GetActionInfoAction(m_nUsedActionInfo, true) );

			// buffering을 없앤다.
			m_fNextTrace			= FLAG_TRACE_NULL;	

			//------------------------------------------------------------
			// message출력
			//------------------------------------------------------------
			DEBUG_ADD_FORMAT("Self SpecialAction");					
		}
		else
		{
			//------------------------------------------------------------
			// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
			//------------------------------------------------------------
			if (m_bRepeatAction)
			{
				m_fNextTrace	= FLAG_TRACE_NULL;

				return false;
			}
			else
			{
				m_fNextTrace	= FLAG_TRACE_SELF;
				
				m_bRepeatAction = FALSE;

				return false;
			}
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Keep Trace Creature
//----------------------------------------------------------------------
// 추적하던 Creature를 계속 추적한다.
//----------------------------------------------------------------------
void
MPlayer::KeepTraceCreature()
{
	if (IsInDarkness())
	{
		TraceNULL();			
		SetStop();			
		UnSetRepeatAction();
			
		return;			
	}

	// 추적하는 Creature의 정보를 읽어온다.
	MCreature*	pCreature = m_pZone->GetCreature( m_TraceID );

	//GET_DYING_CREATURE( pCreature, m_TraceID );

	bool	bTraceTimer = false;

	if( m_TraceTimer != 0 )
	{
		if( ( GetTickCount() - m_TraceTimer ) / 1000 > g_pClientConfig->TRACE_CHARACTER_LIMIT_TIME )
		{
			if( pCreature != NULL &&					// 동족일 경우만.
				(pCreature->IsSlayer() && IsSlayer()) || 
				(pCreature->IsVampire() && IsVampire()) ||
				(pCreature->IsOusters() && IsOusters())
				)
			{
				bTraceTimer = true;
				m_TraceTimer = 0;
			}			
		}
	}
	
	//-------------------------------------------------------
	// 추적하는 Creature가 사라졌을 경우 --> 추적 중지
	// 내가 Slayer인 경우는 Darkness안에 들어간 캐릭을 쫓아갈 수 없다.
	//-------------------------------------------------------
	if ((pCreature==NULL || 
		pCreature->IsInDarkness() && !pCreature->IsNPC() && 
		(!IsVampire() && !HasEffectStatus( EFFECTSTATUS_LIGHTNESS) ||
		IsVampire() && g_pZone->GetID() == 3001)
		||bTraceTimer
		|| pCreature->IsInGroundElemental() && pCreature->IsOusters() && !g_pPlayer->IsOusters())
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		// 추적 중지
		TraceNULL();
			
		// 멈춘다.
		SetStop();	
		
		UnSetRepeatAction();
		m_TraceTimer = 0;
			
		return;			
	}			
	
	if (max(abs(pCreature->GetX()-m_X), 
			abs(pCreature->GetY()-m_Y)) > 1)
	{		
		TraceCreatureToBasicAction( m_TraceID, false );	
		
		g_pTopView->SetSelectedSectorNULL();
	}	
}


//----------------------------------------------------------------------
// Get ActionInfo Range
//----------------------------------------------------------------------
// nActionInfo의 사용 가능 거리는?
//----------------------------------------------------------------------
int		
MPlayer::GetActionInfoRange(TYPE_ACTIONINFO nActionInfo)
{
	if( nActionInfo == SKILL_HEAD_SHOT )
		return 3;
	
	if( (*g_pActionInfoTable)[nActionInfo].GetParentActionInfo() != ACTIONINFO_NULL )
		nActionInfo = (*g_pActionInfoTable)[nActionInfo].GetParentActionInfo();

	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponRange())
	{
		return m_BasicAttackDistance;
		//nActionInfo = m_nBasicActionInfo;
	}
	
	SKILLINFO_NODE& skillInfo = (*g_pSkillInfoTable)[nActionInfo];	

	int minRange = skillInfo.minRange;
	int maxRange = skillInfo.maxRange;
	int skillLevel = skillInfo.GetExpLevel();
	
#ifdef __METROTECH_TEST__
	if(g_bLight)
		return maxRange;
#endif

	int maxLevel = 100;	// 슬레이어는 스킬레벨을 100까지 올린다.
	
	if(IsOusters())
		maxLevel = 30;	// 아우스터즈는 스킬레벨을 30까지 올린다.

	// 특정 스킬에 대한 Range 계산
	
	switch( nActionInfo )
	{
	case MAGIC_RAPID_GLIDING :
		return ( min(6, (2+ (GetDEX() / 50)) ) );
		break;
	
	case MAGIC_BLOODY_ZENITH :
		return min(6, 2 + GetSTR()/80 + GetDEX()/120 + GetINT()/240 );
		break;	
	
	case SKILL_SOUL_REBIRTH:
		return min(5, 2+(*g_pSkillInfoTable)[SKILL_SOUL_REBIRTH_MASTERY].GetExpLevel()/10);
		break;
	}
	
	return (int)(minRange + (maxRange - minRange) * skillLevel/maxLevel);

}

//----------------------------------------------------------------------
// Trace Creature To BasicAction
//----------------------------------------------------------------------
// 기본공격으로(L-Click) 한 명의 Creature를 추적하여 공격한다.
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
bool
MPlayer::TraceCreatureToBasicAction(TYPE_OBJECTID id, bool bForceAttack, bool bClick)
{
//	if(bForceAttack && !g_pDXInput->KeyDown(DIK_LSHIFT))
//	{
//		bForceAttack = false;
//	}
	
	if((HasEffectStatus(EFFECTSTATUS_SLEEP) || HasEffectStatus( EFFECTSTATUS_BLOCK_HEAD ) )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() 
		|| IsInCasket()	// [새기술3]
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInDarkness() && 
		!HasEffectStatus( EFFECTSTATUS_LIGHTNESS )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		 )
	{
		return false;
	}

	if (m_nBasicActionInfo == ACTIONINFO_NULL)
		return false;

	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		return false;

	if (IsStop()) // || m_Action!=ACTION_ATTACK)
	{		
		// Zone에 존재하는 Creature인지 check한다.
		MCreature*	pCreature = m_pZone->GetCreature(id);
		if(pCreature == NULL)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)m_pZone->GetFakeCreature(id);
			if(pFakeCreature != NULL && pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// 펫이당
			{
				MPetItem *pPetItem = pFakeCreature->GetPetItem();
				if(pPetItem != NULL)
				{
					PETINFO petInfo;
					petInfo.HP = pPetItem->GetCurrentDurability();
					petInfo.ATTR = pPetItem->GetEnchantLevel();
					petInfo.ATTR_VALUE = pPetItem->GetSilver();
					petInfo.LEVEL = pPetItem->GetNumber();
					petInfo.FOODTYPE = pPetItem->GetPetFoodType();
					petInfo.EXP_REMAIN = pPetItem->GetPetExpRemain();
					petInfo.CUT_HEAD = pPetItem->IsCanCutHead();
					petInfo.GAMBLE = pPetItem->IsCanGamble();
					if(pPetItem->GetItemOptionListCount() == 0)
						petInfo.OPTION = -1;
					else
						petInfo.OPTION = pPetItem->GetItemOptionList().front();
					petInfo.ITEM_TYPE = pPetItem->GetItemType();
					petInfo.NAME = pPetItem->GetPetName();
					petInfo.ITEM_ID = pPetItem->GetID();
					petInfo.ATTACK = pPetItem->IsCanAttack();
					petInfo.NICK_NAME = pFakeCreature->GetNickName();
					if(pFakeCreature->GetOwnerID() == g_pPlayer->GetID())
						petInfo.ChangeAbleNickName = true;
					else
						petInfo.ChangeAbleNickName = false;
					UI_RunPetInfo(&petInfo);
				}
				return false;
			}
		}
		// 2004,5, 28 sobeit add start - 다른 캐릭터 정보
		if(g_pTopView->GetRequestMode() == MRequestMode::REQUEST_INFO)
		{
			SetTraceID( id );
			SetRequestMode( g_pTopView->GetRequestMode() );
			BasicActionToCreature();				
			SetTraceID( NULL );
			return false;	
		}	
		// 2004,5, 28 sobeit add end
		//GET_DYING_CREATURE( pCreature, id );

		// creature가 없는 경우
		// creature가 이미 죽은 경우
		// 내가 Slayer인 경우는 Darkness안에 들어간 캐릭을 쫓아갈 수 없다.	
		
		if( pCreature != NULL )
		{
			if( pCreature->GetObjectType() == MObject::TYPE_CREATURE )
			{
				if( IsSlayer() && g_pZone->GetSector( GetX(), GetY() ).IsSafeSlayer() && 
					pCreature->IsSlayer() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeSlayer() && pCreature->IsPlayerOnly() ||
					IsVampire() && g_pZone->GetSector( GetX(), GetY() ).IsSafeVampire() && 
					pCreature->IsVampire() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeVampire() && pCreature->IsPlayerOnly() ||
					IsOusters() && g_pZone->GetSector( GetX(), GetY() ).IsSafeOusters() && 
					pCreature->IsOusters() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeOusters() && pCreature->IsPlayerOnly()
					)
				{
					//						UnSetLockMode();
					UnSetRepeatAction();
					//						g_pTopView->SetSelectedNULL();
					return false;
				}
			}
		}
		
//		bool bOusters = IsOusters();
//		MItem *pOustersItem = g_pOustersGear->GetItem(MOustersGear::GEAR_OUSTERS_RIGHTHAND);
		if (pCreature==NULL
			|| pCreature->IsDead()
			|| pCreature->IsInDarkness() && !pCreature->IsNPC() && 
			( !IsVampire() && !HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) ||
			IsVampire() &&g_pZone->GetID() == 3001)
			|| pCreature->IsInGroundElemental() && pCreature->IsOusters() && !g_pPlayer->IsOusters()
			&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
			&& !g_bLight
#endif
			)
		{
			UnSetRepeatAction();
			
			m_fTraceBuffer	= FLAG_TRACE_NULL;	
			m_fNextTrace	= FLAG_TRACE_NULL;
			return false;
		}


		bool	bTraceCreatureToForceAttack;
		bool	bKeepTraceCreature;

		bool bOusters = IsOusters();
		MItem *pOustersItem = g_pOustersGear->GetItem(MOustersGear::GEAR_OUSTERS_RIGHTHAND);

		//------------------------------------------------
		// Request할려고 쫓아가는 경우
		//------------------------------------------------
		if (g_pTopView->IsRequestMode() 
			// Player만 된다.
			//&& pCreature->GetCreatureType()<=CREATURETYPE_VAMPIRE_FEMALE
			&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[m_CreatureType].SpriteTypes[0]].IsPlayerOnlySprite()
			// 관 속에 있는 경우는 안된다.
			&& !pCreature->IsInCasket()
			// 숨어 있는 경우는 안된다.
			&& !pCreature->IsUndergroundCreature()
			&& pCreature->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR

			// 내가 박쥐나 늑대가 아닌 경우
			//&& m_CreatureType!=CREATURETYPE_BAT
			//&& m_CreatureType!=CREATURETYPE_WOLF
			)
		{
			bTraceCreatureToForceAttack = false;
			bKeepTraceCreature			= false;
			m_bNextForceAttack			= false;
			
			SetRequestMode( g_pTopView->GetRequestMode() );
		}
		//------------------------------------------------
		// 오토바이를 타고 있으면 공격이 안된다.
		// 박쥐인 경우 공격이 안된다.
		//------------------------------------------------
		else if (m_MoveDevice == MOVE_DEVICE_RIDE 
				|| m_MoveDevice == MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) )
				|| m_CreatureType==CREATURETYPE_BAT
				|| !(bOusters && pOustersItem!=NULL && pOustersItem->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM || !bOusters)
				|| CanActionByZoneInfo()
				)
		{
			bTraceCreatureToForceAttack = false;
			bKeepTraceCreature			= true;	
			
			UnSetRequestMode();
		}
		//------------------------------------------------
		// 강제 공격인가?
		//------------------------------------------------		
		else if (bForceAttack)
		{
//			if(! (IsSlayer() && g_pZone->GetSector( GetX(), GetY() ).IsSafeSlayer() && pCreature->IsSlayer() && pCreature->IsPlayerOnly()|| 
//				IsVampire() && g_pZone->GetSector( GetX(), GetY() ).IsSafeVampire() && pCreature->IsVampire() && pCreature->IsPlayerOnly()) )
				bTraceCreatureToForceAttack = true;

			bKeepTraceCreature			= false;			

			UnSetRequestMode();
		}
		//------------------------------------------------
		// 아니면.. 종족에 따라서..
		//------------------------------------------------
		else
		{
			bTraceCreatureToForceAttack = CanAttackTribe( pCreature )
										// 뱀파이어인 경우에는 Guild에 따라서 공격할 수 있거나..
										|| IsVampire() && CanAttackGuild( pCreature )
										|| g_pObjectSelector->IsWarEnemy( pCreature );
									//	|| m_pZone->IsFreePKZone();			// -_- 바나툴


			// 정당방위가 되는가?
			bTraceCreatureToForceAttack |= g_pJusticeAttackManager->HasCreature( pCreature->GetName() );

			// 계속 쫓아가는가? - 공격이 아니라면..
			bKeepTraceCreature			= !bTraceCreatureToForceAttack;	
		}

		// 2005, 1, 8, sobeit add start - 승직 슬래이어 중 힐직 인챈은 일반 공격을 할 수 없다. 동작이 없다..-_-
		if(IsAdvancementClass())
		{
			const MCreatureWear::ADDON_INFO& TempaddonInfo = GetAddonInfo(ADDON_RIGHTHAND);
			MCreature*	pCreature = m_pZone->GetCreature(id);
			if(pCreature != NULL)
			{
				if(!pCreature->IsNPC() && IsSlayer() && (!TempaddonInfo.bAddon || TempaddonInfo.ItemClass == ITEM_CLASS_CROSS || TempaddonInfo.ItemClass == ITEM_CLASS_MACE)
					&& bTraceCreatureToForceAttack)
					return false;
			}
		}
		// 2005, 1, 8, sobeit add end
		//------------------------------------------------
		// 총 공격일 경우 
		// --> 탄창이 설정되어 있는 경우를 체크하면 된다.
		//------------------------------------------------
		if (bKeepTraceCreature)
		{
			// 추적하는 경우 체크할 것...
		}
		// NPC가 아니라면 공격할려는건데... 총알 체크해야 한다.
		else if (!pCreature->IsNPC())
		{
			if(!IsRequestMode() && IsInSafeSector() == 2)
				return false;
			
			// g_pCurrentMagazine을 확실히 NULL로 설정하면 된다.
			//if (IsSlayer())
			{
			//	const MItem* pWeapon = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );
				
				//------------------------------------------------
				// 총알 체크
				//------------------------------------------------
			//	if (pWeapon!=NULL					// 무기가 있고
			//		&& pWeapon->IsGunItem()			// 총이고
				//	&&	

				if (
					(IsOusters() && 
					g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_RIGHTHAND ) != NULL &&
					g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_RIGHTHAND )->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM || 
					!IsOusters() ) 
					&& 	(*g_pActionInfoTable)[m_nBasicActionInfo].IsWeaponTypeGunAny() 
					)
				{
					if (!IsRequestMode() 
						
						&& g_pCurrentMagazine!=NULL)		// 총알이 필요 없는 경우
					{
						if (g_pCurrentMagazine->GetNumber()==0)	// 총알이 없는 경우
						{
							// 총을 사용하는데 탄창이 없는 경우
							PlaySound( SOUND_ITEM_NO_MAGAZINE );

							m_fNextTrace	= FLAG_TRACE_NULL;

							// 탄창 없을때 delay
							m_DelayTime	= g_CurrentTime + GetActionInfoDelay(m_nBasicActionInfo);

							// [도움말] 총알 다 썼을 때
//							__BEGIN_HELP_EVENT
//								ExecuteHelpEvent( HE_ITEM_EMPTY_BULLET );
//							__END_HELP_EVENT

							m_fNextTrace = FLAG_TRACE_NULL;

							UnSetRepeatAction();

						
							return false;
						}
					}
				}
			}
			
			// 사용한 기술로 설정
			m_nNextUsedActionInfo	= GetBasicActionInfo(); //m_nBasicActionInfo;
		}
		
		// 추적 정보 설정
		SetTraceID( id );

		if( !bForceAttack && bClick )
		{
			m_TraceTimer = GetTickCount();
		}
		
		m_fTrace	= FLAG_TRACE_CREATURE_BASIC;
		m_TraceX	= pCreature->GetX();
		m_TraceY	= pCreature->GetY();
		m_TraceZ	= pCreature->GetZ();
		
		//------------------------------------------------------------
		// 강제 공격인가?
		// 계속 추적하는가?
		//------------------------------------------------------------
		m_bTraceCreatureToForceAttack	= bTraceCreatureToForceAttack;
		m_bKeepTraceCreature			= bKeepTraceCreature;

		//------------------------------------------------------------
		// 추적 목표 거리..
		//------------------------------------------------------------
		// BasicAction이 뭐냐에 따라서..
		// 현재 Player의 능력에 따라서 TraceDistance가 달라질 것이다.
		//
		// 따라가는 것이면 1이고
		// 아니면 기본 공격에 따라서..
		//
		// NPC인 경우는 거리가 달라질 수 있다.
		//------------------------------------------------------------
		if (m_bKeepTraceCreature)
		{
			m_TraceDistance = 1;
		}
		else
		{
			m_TraceDistance		= m_BasicAttackDistance;	
		}

		// NPC 
		if (pCreature->IsNPC())
		{
			if(pCreature->GetCreatureType() == 659)	// 대지정령의 뿔은 한칸
				m_TraceDistance		= 1;	
			else
				m_TraceDistance		= TRADE_DISTANCE_NPC;	
		}
		// Trade할려고 쫓아가는 경우
		else if (IsRequestMode())
		{
			m_TraceDistance		= TRADE_DISTANCE_PC;	
		}

		// 현재 추적하는 대상에 대해서 기억해둔다.
		m_fTraceBuffer	= m_fTrace;	
		m_TraceIDBuffer = id;

		//------------------------------------------------------------

		if( HasEffectStatus( EFFECTSTATUS_BLOCK_HEAD ) )
			m_bTraceCreatureToForceAttack = false;


		// 추적하는 길 설정
		SetAction( m_MoveAction );
		SetNextDestination(m_TraceX, m_TraceY);

		// buffering을 없앤다.
		m_fNextTrace			= FLAG_TRACE_NULL;	

		//------------------------------------------------------------
		// message출력
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("Trace Creature To BasicAction : ID=%d, (%d, %d)", id, m_TraceX, m_TraceY);				

		//------------------------------------------------------------
		// return false하면 반복 action을 안하게 된다.
		//------------------------------------------------------------
		if (m_bKeepTraceCreature)// || m_bTraceCreatureToForceAttack)
		{
			return false;
		}

		// 선택된 sector 없애기
		g_pTopView->SetSelectedSectorNULL();
			

		return true;
	}
	else
	{
		//------------------------------------------------------------
		// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
		//------------------------------------------------------------
		if (m_bRepeatAction)
		{
			m_fNextTrace	= FLAG_TRACE_NULL;
			
			return false;
		}
		else
		{
			m_fNextTrace	= FLAG_TRACE_CREATURE_BASIC;
			m_NextTraceID	= id;
			m_bNextForceAttack = bForceAttack;

			m_TraceIDBuffer = id;

			m_bRepeatAction = FALSE;

			m_fTraceBuffer = FLAG_TRACE_NULL;

			return true;
		}
	}

	return true;
}


//----------------------------------------------------------------------
// Trace Creature To SpecialAction
//----------------------------------------------------------------------
// 특수공격으로(R-Click) 한 명의 Creature를 추적하여 공격한다.
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
// [return값]
//
//  다른 Creature에게 사용하지 못하는 경우만 false를 return한다. 
//
//----------------------------------------------------------------------
bool	
MPlayer::TraceCreatureToSpecialAction(TYPE_OBJECTID id, bool bForceAttack)
{
	SetActionDelay( 0 );

	if((HasEffectStatus(EFFECTSTATUS_SLEEP) || HasEffectStatus( EFFECTSTATUS_BLOCK_HEAD ) )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

//	if(bForceAttack && !g_pDXInput->KeyDown(DIK_LSHIFT))
//	{
//		bForceAttack = false;
//	}
		
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() 
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket()	// [새기술3]
		|| IsInDarkness() && (!HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) || g_pZone->GetID() == 3001)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		return false;
	}

	if (m_nSpecialActionInfo == ACTIONINFO_NULL
		// 오토바이를 타고 있으면 특수공격이 안된다.
		|| m_MoveDevice == MOVE_DEVICE_RIDE
		// 뭔가 기술 사용을 검정 받아야 하면 사용할 수 없다.
		//|| m_WaitVerify != WAIT_VERIFY_NULL)	// 2001.8.20 주석처리
		|| IsInSafeSector() == 2
		// 완전 안전지대에서는 기술을 사용할 수 없다.
		|| CanActionByZoneInfo()
		|| HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) )				// 달라댕기면 못한당~ 
		)
	{
		m_fNextTrace = FLAG_TRACE_NULL;

		UnSetRepeatAction();

		return false;
	}

		
	// 다른 기술을 사용중인 경우
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		return false;

	//-------------------------------------------------------
	// 타인에게 사용할 수 있는 것들만 사용한다.
	//-------------------------------------------------------
	
	if ((*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetOther())
	{
		int useSkill = m_nSpecialActionInfo;
		int originalSkill = m_nSpecialActionInfo;

//		// 2004, 6, 9 sobeit add start - 테스트, 흡혈 성공 메세지 온담에 동작하게..-_-
//		if(m_nSpecialActionInfo == SKILL_BLOOD_DRAIN)
//		{
//			m_fNextTrace = FLAG_TRACE_NULL;
//			//UnSetRepeatAction();
//			if(g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)m_nSpecialActionInfo ) && m_TraceID != -1)
//			{
//				CGBloodDrain _CGBloodDrain;
//				_CGBloodDrain.setObjectID( m_TraceID );
//				g_pSocket->sendPacket( &_CGBloodDrain );
//				DEBUG_ADD("흡혈 메세지 보냄");
//			}
//			return false;
//		}
//		// 2004, 6, 9 sobeit add end - 테스트, 흡혈 성공 메세지 온담에 동작하게..-_-
	
		if( m_nSpecialActionInfo == SKILL_BLITZ_SLIDING_ATTACK ||
			m_nSpecialActionInfo == SKILL_BLAZE_WALK_ATTACK 
			)
		{
			m_fNextTrace = FLAG_TRACE_NULL;
			UnSetRepeatAction();

			return false;
		}

		int grade = 0;
		
		if( IsOusters() )
		{
			grade = (*g_pSkillInfoTable)[originalSkill].GetExpLevel() / 15;
			DEBUG_ADD_FORMAT("OUSTERS LEVEL : %d %d",originalSkill,(*g_pSkillInfoTable)[originalSkill].GetExpLevel());
		}

		if( grade>0 && (*g_pActionInfoTable)[originalSkill].IsUseActionStep() )
			useSkill = (*g_pActionInfoTable)[originalSkill].GetActionStep( grade - 1 );
		// 2004, 9, 20, sobeit add start
		if((*g_pActionInfoTable)[useSkill].IsMasterySkillStep())
		{
			int TempSkill = (*g_pActionInfoTable)[useSkill].GetActionStep( (*g_pActionInfoTable)[useSkill].GetMasterySkillStep() );
			if(g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)TempSkill ))
				useSkill = TempSkill;
		}
		// 2004, 9, 20, sobeit add end
		if( (*g_pActionInfoTable)[useSkill].GetParentActionInfo() != ACTIONINFO_NULL )
			originalSkill = (*g_pActionInfoTable)[useSkill].GetParentActionInfo();

		DEBUG_ADD_FORMAT("SKILL : %d %d", originalSkill, useSkill );

		
		//-------------------------------------------------------
		// 현재 사용할 수 있는 기술인지 체크..
		// Passive 스킬이면 사용 못하게..
		//-------------------------------------------------------
			if (!g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)originalSkill )
				|| !(*g_pSkillInfoTable)[originalSkill].IsEnable()
				|| !(*g_pSkillInfoTable)[originalSkill].IsAvailableTime()
				|| (*g_pSkillInfoTable)[originalSkill].IsPassive())
			{
				m_fNextTrace = FLAG_TRACE_NULL;

				UnSetRepeatAction();

				return false;
			}
		
		if (IsStop()) // || m_Action!=ACTION_ATTACK)
		{
			// Zone에 존재하는 Creature인지 check한다.
			MCreature*	pCreature = m_pZone->GetCreature(id);

			//GET_DYING_CREATURE( pCreature, id );
			
			// creature가 없는 경우
			// 내가 Slayer인 경우는 Darkness안에 들어간 캐릭을 쫓아갈 수 없다.
			if (pCreature==NULL 
				// 시체한테 쓸 수 있는 스킬 정보를 추가해야한다.
				|| pCreature->IsDead() && originalSkill!=MAGIC_RESURRECT && originalSkill!=SKILL_TRANSFUSION  && originalSkill!=SKILL_SOUL_REBIRTH 
				|| pCreature->IsInGroundElemental() && pCreature->IsOusters() && !g_pPlayer->IsOusters()
				// 2004, 10, 14, sobiet modify start - 라이트니스걸렸을때 다크니스 안에 녀석 공격이 안된다고 해서 수정..
//				|| pCreature->IsInDarkness() && !pCreature->IsNPC() && 
//				!(IsVampire() && !HasEffectStatus( EFFECTSTATUS_LIGHTNESS )  ||
//				g_pZone->GetID() != 3001 && IsVampire() )
//				&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST ) 
				|| pCreature->IsInDarkness() && !pCreature->IsNPC() && !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST ) &&
				(!IsVampire() && !HasEffectStatus( EFFECTSTATUS_LIGHTNESS )  ||	g_pZone->GetID() == 3001 && IsVampire() )
				// 2004, 10, 14, sobiet modify end- 라이트니스걸렸을때 다크니스 안에 녀석 공격이 안된다고 해서 수정..
#ifdef __METROTECH_TEST__
				&& !g_bLight
#endif
				|| (*g_pActionInfoTable)[originalSkill].GetUser() == FLAG_ACTIONINFO_USER_SLAYER && (!pCreature->IsSlayer() || !pCreature->IsPlayerOnly())
				|| (*g_pActionInfoTable)[originalSkill].GetUser() == FLAG_ACTIONINFO_USER_VAMPIRE && (!pCreature->IsVampire() || !pCreature->IsPlayerOnly())
				|| (*g_pActionInfoTable)[originalSkill].GetUser() == FLAG_ACTIONINFO_USER_OUSTERS && (!pCreature->IsOusters() || !pCreature->IsPlayerOnly())
				|| (*g_pActionInfoTable)[originalSkill].GetUser() == FLAG_ACTIONINFO_USER_MONSTER && (!pCreature->IsVampire() || pCreature->IsPlayerOnly())

				|| IsSlayer() && !(*g_pActionInfoTable)[originalSkill].IsUserSlayer() && pCreature->IsSlayer() && pCreature->IsPlayerOnly() && !bForceAttack
				|| IsVampire() && !(*g_pActionInfoTable)[originalSkill].IsUserVampire() && pCreature->IsVampire() && pCreature->IsPlayerOnly() && !bForceAttack				
				|| IsOusters() && !(*g_pActionInfoTable)[originalSkill].IsUserOusters() && pCreature->IsOusters() && pCreature->IsPlayerOnly() && !bForceAttack				
				) 
			{
				// 여기까지 오면 ACTIONINFO_NULL인데..
				// UnSetRepeatAction()에서 제대로 처리하기 위해서.. - -;;
				// 암튼 콩가루다.. - -;;
				if (IsRepeatAction())
				{
					m_nUsedActionInfo = originalSkill;

					UnSetRepeatAction();

					m_nUsedActionInfo = ACTIONINFO_NULL;
				}
				else
				{
					UnSetRepeatAction();
				}

				m_fNextTrace = FLAG_TRACE_NULL; 
				m_fTraceBuffer	= FLAG_TRACE_NULL;	
				return false;
			}

			if( pCreature != NULL )
			{
				if( pCreature->GetObjectType() == MObject::TYPE_CREATURE )
				{
					if( IsSlayer() && g_pZone->GetSector( GetX(), GetY() ).IsSafeSlayer() && 
						pCreature->IsSlayer() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeSlayer() && pCreature->IsPlayerOnly() ||
						IsVampire() && g_pZone->GetSector( GetX(), GetY() ).IsSafeVampire() && 
						pCreature->IsVampire() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeVampire() && pCreature->IsPlayerOnly() ||
						IsOusters() && g_pZone->GetSector( GetX(), GetY() ).IsSafeOusters() && 
						pCreature->IsOusters() && !g_pZone->GetSector( pCreature->GetX(), pCreature->GetY() ).IsSafeOusters() && pCreature->IsPlayerOnly())
					{
//						UnSetLockMode();
						UnSetRepeatAction();
//						g_pTopView->SetSelectedNULL();
						return false;
					}
				}
			}

			bool bTraceCreatureToForceAttack = true;
			bool bKeepTraceCreature			= false;			

			//-------------------------------------------------------
			// NPC인 경우.. 그냥 추적하게 한다.
			//-------------------------------------------------------
			if (pCreature->IsNPC())
			{
				TraceCreatureToBasicAction(id, false);				

				return false;
			}
			else
				if (bForceAttack)
				{
					bTraceCreatureToForceAttack = true;
					bKeepTraceCreature			= false;			
					
					UnSetRequestMode();
				}


			//-------------------------------------------------------
			// 특정 기술에 대한 체크 - 따로 빼야되는데. 늘 그렇듯이.. --;
			//-------------------------------------------------------
			switch (originalSkill)
			{
				//-------------------------------------------------------
				// SKILL_TRANSFUSION
				//-------------------------------------------------------
				case SKILL_TRANSFUSION :

					//--------------------------------------------------
					// 사용할 수 없는 경우
					//--------------------------------------------------
					// 내 HP가 30보다 적은 경우
					// 다른 길드인 경우
					// 상대의 HP가 full인 경우
					if (GetHP() < 30
						|| pCreature->IsSlayer()
							//|| GetGuildNumber()!=pCreature->GetGuildNumber()	
							// vampire의 default 길드
							//&& GetGuildNumber()!=0
						//	&& pCreature->GetGuildNumber()!=0
						|| pCreature->GetHP()==pCreature->GetMAX_HP()
						|| pCreature->IsUndergroundCreature()
						|| pCreature->IsFlyingCreature() )
					{
						m_fNextTrace = FLAG_TRACE_NULL;

						UnSetRepeatAction();

						return false;
					}
				break;

				//-------------------------------------------------------
				// SKILL_THROW_BOMB
				//-------------------------------------------------------
				case SKILL_THROW_BOMB :
				{
					MItem* pItem = g_pInventory->FindItem( ITEM_CLASS_BOMB );
								
					if (pItem==NULL)
					{
						m_fNextTrace = FLAG_TRACE_NULL;
						UnSetRepeatAction();
						return false;
					}

					useSkill = pItem->GetUseActionInfo();
				}
				break;
			}

			//-------------------------------------------------------
			// 특정한 상황 체크
			//-------------------------------------------------------
			if ((*g_pActionInfoTable)[originalSkill].HasOption())
			{
				//-------------------------------------------------------
				// bless 걸려야 사용하는 기술
				//-------------------------------------------------------
				if ((*g_pActionInfoTable)[originalSkill].IsOptionUseWithBless())
				{
					if (!m_bEffectStatus[EFFECTSTATUS_BLESS])
					{
							m_fNextTrace = FLAG_TRACE_NULL;

							UnSetRepeatAction();

							return false;
					}
				}
			}

			//-------------------------------------------------------
			// item을 사용하는 기술이면 item을 체크해야 한다.
			//-------------------------------------------------------
			WORD fWeaponType = (*g_pActionInfoTable)[originalSkill].GetWeaponType();
			if (fWeaponType	& FLAG_ACTIONINFO_WEAPON_HOLY_WATER)
			{
				MItem* pItem = g_pInventory->FindItem( ITEM_CLASS_HOLYWATER );

				if (pItem==NULL)
				{
					m_fNextTrace = FLAG_TRACE_NULL;

					UnSetRepeatAction();

					return false;
				}

				// holy water가 있는 경우라면..
				// 딴짓 못하도록... item 고정..
				// 기술 끝나고 풀어야 한다.
				//SetItemCheckBuffer( pItem, ITEM_CHECK_BUFFER_SKILL_FROM_ITEM );
			}

			//-------------------------------------------------------
			// item을 사용하는 기술이면 item을 체크해야 한다.
			//-------------------------------------------------------
			if (fWeaponType & FLAG_ACTIONINFO_WEAPON_BOMB)
			{
				MItem* pItem = g_pInventory->FindItem( ITEM_CLASS_BOMB );

				if (pItem==NULL)
				{
					m_fNextTrace = FLAG_TRACE_NULL;

					UnSetRepeatAction();

					return false;
				}

				// holy water가 있는 경우라면..
				// 딴짓 못하도록... item 고정..
				// 기술 끝나고 풀어야 한다.
				//SetItemCheckBuffer( pItem, ITEM_CHECK_BUFFER_SKILL_FROM_ITEM );
			}

			//-------------------------------------------------------
			// 총을 사용하는 기술이면 
			// 총알을 체크해야 한다.
			//-------------------------------------------------------
			if ((*g_pActionInfoTable)[originalSkill].IsWeaponTypeGunAny())
			{
					if (g_pCurrentMagazine==NULL)
					{
						// 총을 사용하는데 탄창이 없는 경우
						PlaySound( SOUND_ITEM_NO_MAGAZINE );

						// 탄창 없을때 delay
							m_DelayTime	= g_CurrentTime + GetActionInfoDelay(m_nBasicActionInfo);

						m_fNextTrace	= FLAG_TRACE_NULL;
						return false;					
					}
					else
					{
						if (g_pCurrentMagazine->GetNumber()==0)
						{
							// 총을 사용하는데 탄창이 없는 경우
							PlaySound( SOUND_ITEM_NO_MAGAZINE );

							// 탄창 없을때 delay
							m_DelayTime	= g_CurrentTime + GetActionInfoDelay(m_nBasicActionInfo);

							if (IsRepeatAction())
							{
								m_nUsedActionInfo = originalSkill;

								UnSetRepeatAction();

								m_nUsedActionInfo = ACTIONINFO_NULL;
							}
							else
							{
								UnSetRepeatAction();
							}

							m_fNextTrace	= FLAG_TRACE_NULL;

							// [도움말] 총알 다 썼을 때
//							__BEGIN_HELP_EVENT
//								ExecuteHelpEvent( HE_ITEM_EMPTY_BULLET );
//							__END_HELP_EVENT
							

							return false;
						}
					}
			}

			// creature가 이미 죽은 경우
			//if (pCreature->IsDead()) return;

			// 사용한 기술로 설정
			m_nNextUsedActionInfo	= useSkill;

			// 추적 정보 설정
			SetTraceID( id );
			m_fTrace	= FLAG_TRACE_CREATURE_SPECIAL;		
			m_TraceX	= pCreature->GetX();
			m_TraceY	= pCreature->GetY();
			m_TraceZ	= pCreature->GetZ();

			// 추적대상이 되는 캐릭터의 종족
			//m_TraceCreatureTribe = pCreature->GetCreatureTribe();

			// 현재 추적하는 대상에 대해서 기억해둔다.
			m_fTraceBuffer	= m_fTrace;	
			m_TraceIDBuffer = id;

			//------------------------------------------------------------
			// SpecialAction이 뭐냐에 따라서.. 
			// 현재 Player의 능력에 따라서 TraceDistance가 달라질 것이다.
			//------------------------------------------------------------
			//------------------------------------------------------------
			// 강제 공격인가?
			// 계속 추적하는가?
			//------------------------------------------------------------
			m_bTraceCreatureToForceAttack	= bTraceCreatureToForceAttack;
			m_bKeepTraceCreature			= bKeepTraceCreature;
			
			m_TraceDistance		= GetActionInfoRange(m_nNextUsedActionInfo);
			
			//------------------------------------------------------------

			// 추적하는 길 설정
			SetAction( m_MoveAction );
			SetNextDestination(m_TraceX, m_TraceY);

			// buffering을 없앤다.
			m_fNextTrace			= FLAG_TRACE_NULL;

			// 계속 쫓아가는 것을 중지
			m_bKeepTraceCreature	= false;
		
			//------------------------------------------------------------
			// message출력
			//------------------------------------------------------------
			DEBUG_ADD_FORMAT("Trace Creature To SpecialAction: ID=%d, (%d, %d)", id, m_TraceX, m_TraceY);					

			// 선택된 sector 없애기
			g_pTopView->SetSelectedSectorNULL();
		}
		else
		{
			//------------------------------------------------------------
			// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
			//------------------------------------------------------------
			if (m_bRepeatAction)
			{
				m_fNextTrace	= FLAG_TRACE_NULL;

				return false;
			}
			else
			{
				m_fNextTrace	= FLAG_TRACE_CREATURE_SPECIAL;
				m_bNextForceAttack = bForceAttack;
				m_NextTraceID	= id;

				m_TraceIDBuffer = id;

				m_bRepeatAction = FALSE;

				m_fTraceBuffer = FLAG_TRACE_NULL;

				return true;//false;
			}
		}

		return true;
	}
	
	// Target to creature가 아니라는 의미.
	return false;
}


//----------------------------------------------------------------------
// Trace Sector To BasicAction
//----------------------------------------------------------------------
// 기본공격으로(L-Click)  Sector(sX,sY)를 공격한다.
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
bool	
MPlayer::TraceSectorToBasicAction(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() 
		|| HasEffectStatus ( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket()	// [새기술3]
		|| g_bZoneSafe)
	{
		return false;
	}

	// 좌표가 잘못 입력된 경우
	if (sX==SECTORPOSITION_NULL || sY==SECTORPOSITION_NULL)
		return false;
	
	if (m_nBasicActionInfo == ACTIONINFO_NULL
		// 오토바이를 타고 있으면 공격 안됨
		|| m_MoveDevice == MOVE_DEVICE_RIDE
		|| CanActionByZoneInfo())
	{
		return false;
	}
		
	if (IsStop()) // || m_Action!=ACTION_ATTACK)
	{		
		// 사용한 기술로 설정
		m_nNextUsedActionInfo	= GetBasicActionInfo(); //m_nBasicActionInfo;
		
		// 추적 정보 설정
		m_fTrace	= FLAG_TRACE_SECTOR_BASIC;
		m_TraceX	= sX;
		m_TraceY	= sY;
		m_TraceZ	= 0;
		
		// 현재 추적하는 대상에 대해서 기억해둔다.
		m_fTraceBuffer	= m_fTrace;	

		//------------------------------------------------------------
		// BasicAction이 뭐냐에 따라서..
		// 현재 Player의 능력에 따라서 TraceDistance가 달라질 것이다.
		//------------------------------------------------------------
		
		m_TraceDistance		= GetActionInfoRange(m_nNextUsedActionInfo);
		
		//------------------------------------------------------------		

		// 추적하는 길 설정
		SetAction( m_MoveAction );
		SetNextDestination(m_TraceX, m_TraceY);

		// buffering을 없앤다.
		m_fNextTrace			= FLAG_TRACE_NULL;
		
		// 계속 쫓아가는 것을 중지
		m_bKeepTraceCreature	= false;

		//------------------------------------------------------------
		// message출력
		//------------------------------------------------------------
		#ifdef	OUTPUT_DEBUG
			DEBUG_ADD_FORMAT("Trace Sector To BasicAction: (%d, %d)", sX, sY);				
		#endif
	}
	else
	{
		//------------------------------------------------------------
		// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
		//------------------------------------------------------------
		if (m_bRepeatAction)
		{
			m_fNextTrace	= FLAG_TRACE_NULL;

			return false;
		}
		else
		{
			m_fNextTrace	= FLAG_TRACE_SECTOR_BASIC;
			m_NextTraceX	= sX;
			m_NextTraceY	= sY;

			m_bRepeatAction = FALSE;

			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------
// Trace Sector To SpecialAction
//----------------------------------------------------------------------
// 특수공격으로(R-Click)  Sector(sX,sY)를 공격한다.
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
// [return값]
//
//  Zone에 사용하지 못하는 경우만 false를 return한다. 
//
//----------------------------------------------------------------------
bool	
MPlayer::TraceSectorToSpecialAction(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	if((HasEffectStatus(EFFECTSTATUS_SLEEP) || HasEffectStatus( EFFECTSTATUS_BLOCK_HEAD ) )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() 
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket()	// [새기술3]
		|| g_bZoneSafe
		|| CanActionByZoneInfo()
		)
	{
		return false;
	}

	// 좌표가 잘못 입력된 경우
	if (sX==SECTORPOSITION_NULL || sY==SECTORPOSITION_NULL)
		return false;
	
	if (m_nSpecialActionInfo == ACTIONINFO_NULL		
		// 오토바이를 타고 있으면 기술을 사용할 수 없다.
		|| m_MoveDevice == MOVE_DEVICE_RIDE
		// 뭔가 기술 사용을 검정 받아야 하면 사용할 수 없다.
		//|| m_WaitVerify != WAIT_VERIFY_NULL)	// 2001.8.20 주석처리
		|| IsInSafeSector() == 2
		// 완전 안전지대에서는 기술을 사용할 수 없다.		
		)
	{
		m_fNextTrace = FLAG_TRACE_NULL;

		UnSetRepeatAction();

		return false;
	}
	// 2004, 9, 18, sobeit add start - 터렛 파이어는 사정거리 벗어나믄 무시 ㅜ.ㅜ 
	if (m_nSpecialActionInfo==SKILL_TURRET_FIRE)
	{
		m_TraceDistance		= GetActionInfoRange(m_nSpecialActionInfo);
		if (max(abs(m_X-sX), abs(m_Y-sY)) > m_TraceDistance || !g_pSkillAvailable->IsEnableSkill( SKILL_TURRET_FIRE ))
		{
			m_fNextTrace = FLAG_TRACE_NULL;
			UnSetRepeatAction();
			InstallTurretStopAttack();
			return false;
		}
	}
	// 2004, 9, 18, sobeit add end

	// 다른 기술을 사용중인 경우
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		return false;

	// 2004, 9, 21, sobeit add start - 타겟 크리쳐 스킬이 타겟 존 스킬로 사용할 수 있게
	bool bAbleTargetSector = false;
//	if(m_nSpecialActionInfo == SKILL_DESTRUCTION_SPEAR || m_nSpecialActionInfo == SKILL_DESTRUCTION_SPEAR_MASTERY)
//	{
//		if(g_pSkillAvailable->IsEnableSkill( SKILL_DESTRUCTION_SPEAR_MASTERY ))
//			bAbleTargetSector = true;
//	}
//	else 
	if(m_nSpecialActionInfo == SKILL_ICE_LANCE || m_nSpecialActionInfo == SKILL_ICE_LANCE_MASTERY)
	{
		if(g_pSkillAvailable->IsEnableSkill( SKILL_ICE_LANCE_MASTERY ))
			bAbleTargetSector = true;
	}
	// 2004, 9, 21, sobeit add end
	// Zone에 사용할 수 있는 것들만 사용한다.
	if ((*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetZone() || bAbleTargetSector)
	{
		int grade = 0;
		int		useSkill = m_nSpecialActionInfo;

		if( IsOusters() )
		{
			grade = (*g_pSkillInfoTable)[useSkill].GetExpLevel() / 15;
		}
		
		if( (*g_pActionInfoTable)[useSkill].IsUseActionStep() && grade > 0 )
			useSkill = (*g_pActionInfoTable)[useSkill].GetActionStep( grade - 1 );			
			
		// 2004, 9, 20, sobeit add start
		if((*g_pActionInfoTable)[useSkill].IsMasterySkillStep())
		{
			int TempSkill = (*g_pActionInfoTable)[useSkill].GetActionStep( (*g_pActionInfoTable)[useSkill].GetMasterySkillStep() );
			if(g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)TempSkill ))
				useSkill = TempSkill;
		}
		// 2004, 9, 20, sobeit add end
		
//		if (useSkill==SKILL_TURRET_FIRE)
//		{
//			if (max(abs(m_X-sX), abs(m_Y-sY)) 
//				> m_TraceDistance)
//			{
//				m_fNextTrace = FLAG_TRACE_NULL;
//				UnSetRepeatAction();
//				return false;
//			}
//		}
		//-------------------------------------------------------
		// 폭탄 골라서 던지기
		//-------------------------------------------------------
		if (useSkill==SKILL_THROW_BOMB)
		{
			//-------------------------------------------------------
			// 현재 사용할 수 있는 기술인지 체크..
			// Passive 스킬이면 사용 못하게..
			//-------------------------------------------------------
			if (IsSlayer()
				&& !IsWaitVerify()
				&& IsItemCheckBufferNULL())
			{											
				ITEM_CLASS	itemClass = ITEM_CLASS_BOMB;

				MItemClassFinder itemFinder(itemClass);
				MItem* pItem = ((MItemManager*)g_pInventory)->FindItem( itemFinder );
							
				if (pItem==NULL)
				{
					m_fNextTrace = FLAG_TRACE_NULL;
					UnSetRepeatAction();
					return false;
				}

				useSkill = pItem->GetUseActionInfo();				
			}
		}

		
		//-------------------------------------------------------
		// 현재 사용할 수 있는 기술인지 체크..
		// Passive 스킬이면 사용 못하게..
		//-------------------------------------------------------
		if (
			(!g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)m_nSpecialActionInfo ) && m_nSpecialActionInfo != SKILL_TURRET_FIRE)
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsEnable()
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsAvailableTime()
			|| (*g_pSkillInfoTable)[m_nSpecialActionInfo].IsPassive())
		{
			m_fNextTrace = FLAG_TRACE_NULL;

			UnSetRepeatAction();

			return false;
		}

		if (IsStop()) // || m_Action!=ACTION_ATTACK)
		{	
#ifdef __METROTECH_TEST__
			if (IsOusters())
			{
				bool bAbsorbSoul = true;
				
				MItem* pItem = g_pZone->GetItem( g_pZone->GetItemID( sX, sY ) );
				
				if (pItem==NULL 
					|| pItem->GetItemClass()!=ITEM_CLASS_CORPSE)
				{
					bAbsorbSoul = false;
					
					if(pItem == NULL)
					{
						MCreature *pCreature = g_pZone->GetCreature(g_pZone->GetCreatureID(sX, sY));
						
						if(pCreature != NULL && pCreature->HasEffectStatus(EFFECTSTATUS_COMA))
						{
							bAbsorbSoul = true;
						}
					}
				}
				
				if(pItem != NULL)
				{
					// 성물은 안먹게 하기
					MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();
					if(pCreature != NULL)
					{
						if(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 670 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673)
							bAbsorbSoul = false;
					}
				}
				if(bAbsorbSoul)
					useSkill = SKILL_ABSORB_SOUL;
			}
#endif
			
			int targetX	= sX;
			int targetY = sY;

			//-------------------------------------------------------
			// 특정한 기술인 경우의 체크
			//-------------------------------------------------------
			switch (useSkill)
			{
				// 트랜스 퓨전은 타일기술로 체크되있지만 실제로 사용할 수 는 없다
				case SKILL_TRANSFUSION:
					return false;
					break;
					
				case SKILL_WILD_TYPHOON :
					return false;
					break;

				//-------------------------------------------------------
				// 흡영
				//-------------------------------------------------------
				// 바닥에 시체가 없는 경우는 기술 사용 안한다.
				//-------------------------------------------------------
				case SKILL_ABSORB_SOUL :
					if (IsOusters())
					{
						MItem* pItem = g_pZone->GetItem( g_pZone->GetItemID( sX, sY ) );
						
						if (pItem==NULL 
							|| pItem->GetItemClass()!=ITEM_CLASS_CORPSE)
						{
							if(pItem == NULL)
							{
								MCreature *pCreature = g_pZone->GetCreature(g_pZone->GetCreatureID(sX, sY));

								if(pCreature != NULL && pCreature->HasEffectStatus(EFFECTSTATUS_COMA))
									break;
							}

							m_fNextTrace = FLAG_TRACE_NULL;
							
							UnSetRepeatAction();
							
							return false;
						}
						
						// 성물은 안먹게 하기
						MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();
						if(pCreature != NULL)
						{
							if(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 670 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673)
								return false;
						}
						
					}
					else
					{
						m_fNextTrace = FLAG_TRACE_NULL;
						
						UnSetRepeatAction();
						
						return false;
					}
					break;

				//-------------------------------------------------------
				// 늑대인 경우 시체 먹기
				//-------------------------------------------------------
				// 바닥에 시체가 없는 경우는 기술 사용 안한다.
				//-------------------------------------------------------
				case SKILL_BITE_OF_DEATH :
					if (m_CreatureType != CREATURETYPE_WER_WOLF )
					{
						return false;
					}
					break;
				case MAGIC_EAT_CORPSE :
					if (m_CreatureType==CREATURETYPE_WOLF)
					{
						MItem* pItem = g_pZone->GetItem( g_pZone->GetItemID( sX, sY ) );

						if (pItem==NULL 
							|| pItem->GetItemClass()!=ITEM_CLASS_CORPSE)
						{
							m_fNextTrace = FLAG_TRACE_NULL;

							UnSetRepeatAction();

							return false;
						}

						// 성물은 안먹게 하기
						MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();
						if(pCreature != NULL)
						{
							if(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 670 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673)
								return false;
						}
					
					}
					else
					{
						m_fNextTrace = FLAG_TRACE_NULL;

						UnSetRepeatAction();

						return false;
					}
				break;
//				// 2004, 8, 30, sobeit add start - sweep vice test
//				case SKILL_SWEEP_VICE_1:
//					if(!GetSweepViewValue()) //스윕바이스를 아직 사용중이 아닐때
//					{
//						SetSweepViewValue(1);
//						g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
//						m_TraceX	= 	g_SelectSector.x;
//						m_TraceY	=	g_SelectSector.y;
//						ExecuteActionInfoFromMainNode(SKILL_CLIENT_SWEEP_VICE,g_SelectSector.x, g_SelectSector.y, 0,g_pPlayer->GetDirection(),0	,	
//							g_SelectSector.x, g_SelectSector.y, 0, 0xffff, NULL, false);
//						UnSetRepeatAction();
//					//	return false ;
//					}
//					else
//					{
//						if(!g_bRButtonDown)
//						{
//							if(2 == GetSweepViewValue())
//								useSkill = SKILL_SWEEP_VICE_3;
//							else if(3 == GetSweepViewValue())
//								useSkill = SKILL_SWEEP_VICE_5;
//						}
//						else 
//							return false;
//					}
//					break;
				case SKILL_TURRET_FIRE :
					if (!HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
						return false;
					break;
			}

	

// 2004, 8, 30, sob
	

			WORD fWeaponType = (*g_pActionInfoTable)[useSkill].GetWeaponType();
			
			//------------------------------------------------
			// 폭탄인 경우.. 레벨에 따라서 좌표 어긋나게
			//------------------------------------------------
			if (fWeaponType	& FLAG_ACTIONINFO_WEAPON_BOMB)
			{
				int skillLevel = (*g_pSkillInfoTable)[SKILL_THROW_BOMB].GetExpLevel();

				int hitRoll = 50 + skillLevel/2;

				if (rand()%100 < hitRoll)
				{
					// 제대로 맞은 경우
				}
				else
				{
					if (rand()%100 < 25
						&& skillLevel < 30)		// 2타일 근처
					{
						// 빚맞아야 한다.
						targetX += rand()%5 - 2;
						targetY += rand()%5 - 2;
					}
					else						// 1타일 근처
					{
						// 빚맞아야 한다.
						targetX += rand()%3 - 1;
						targetY += rand()%3 - 1;
					}
				}
			}

			//------------------------------------------------
			// 총알 체크
			//------------------------------------------------
			if ((*g_pActionInfoTable)[useSkill].IsWeaponTypeGunAny())
			{
				if (!IsRequestMode() && g_pCurrentMagazine!=NULL)		// 총알이 필요 없는 경우
				{
					if (g_pCurrentMagazine->GetNumber()==0)	// 총알이 없는 경우
					{
						// 총을 사용하는데 탄창이 없는 경우
						PlaySound( SOUND_ITEM_NO_MAGAZINE );

						m_fNextTrace	= FLAG_TRACE_NULL;

						

						// [도움말] 총알 다 썼을 때
//						__BEGIN_HELP_EVENT
//							ExecuteHelpEvent( HE_ITEM_EMPTY_BULLET );
//						__END_HELP_EVENT

						m_fNextTrace = FLAG_TRACE_NULL;

						UnSetRepeatAction();
						// 2004, 9, 23, sobeit add start - 터렛 공격 끝나는 동작
						if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
						{
							InstallTurretStopAttack();
						}
						else
							// 탄창 없을때 delay
						m_DelayTime	= g_CurrentTime + GetActionInfoDelay(m_nBasicActionInfo);
						// 2004, 9, 23, sobeit add end
						return false;
					}
				}
			}

			// 사용한 기술로 설정
			m_nNextUsedActionInfo	= useSkill;

			// 추적 정보 설정
			m_fTrace	= FLAG_TRACE_SECTOR_SPECIAL;
			m_TraceX	= targetX;
			m_TraceY	= targetY;
			m_TraceZ	= 0;

			// 현재 추적하는 대상에 대해서 기억해둔다.
			m_fTraceBuffer	= m_fTrace;	
			
			//------------------------------------------------------------
			// BasicAction이 뭐냐에 따라서..
			// 현재 Player의 능력에 따라서 TraceDistance가 달라질 것이다.
			//------------------------------------------------------------
			
			m_TraceDistance		= GetActionInfoRange(m_nNextUsedActionInfo);
			
			//------------------------------------------------------------		

			// 추적하는 길 설정
			SetAction( m_MoveAction );
			SetNextDestination(m_TraceX, m_TraceY);

			// buffering을 없앤다.
			m_fNextTrace			= FLAG_TRACE_NULL;

			// 계속 쫓아가는 것을 중지
			m_bKeepTraceCreature	= false;
			
			//------------------------------------------------------------
			// message출력
			//------------------------------------------------------------
			DEBUG_ADD_FORMAT("Trace Sector To SpecialAction: (%d, %d)", sX, sY);					
			if(useSkill == SKILL_TELEPORT)
			{
				return false;
			}
		}
		else
		{

			//------------------------------------------------------------
			// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
			//------------------------------------------------------------
			if (m_bRepeatAction)
			{
				m_fNextTrace	= FLAG_TRACE_NULL;

				return false;
			}
			else
			{
				DEBUG_ADD_FORMAT("Trace Sector To SpecialAction: Buffering");					
				
				m_fNextTrace	= FLAG_TRACE_SECTOR_SPECIAL;
				m_NextTraceX	= sX;
				m_NextTraceY	= sY;

				m_bRepeatAction = FALSE;

				// 200411, 12, 21, sobeit modify start - 이동중 스킬 사용시 반복이 안되는 현상 수정
				//return false;
				return true;
				// 200411, 12, 21, sobeit modify end
			}
		}

		return true;
	}

	// Zone에 사용하는게 아니라는 의미
	return false;
}

//----------------------------------------------------------------------
// Trace Item
//----------------------------------------------------------------------
// Item으로 다가가서 주워야 한다.
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
bool	
MPlayer::TraceItem(TYPE_OBJECTID id)
{
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay() 
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket()	// [새기술3]
		|| IsInDarkness() && (!HasEffectStatus( EFFECTSTATUS_LIGHTNESS )||g_pZone->GetID() == 3001)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		return false;
	}

	if (IsStop()) // || m_Action!=ACTION_ATTACK)
	{
		// Zone에 존재하는 Item인지 check한다.
		MItem*	pItem = m_pZone->GetItem(id);

		// item이 zone에 없는 경우
		if (pItem==NULL
			|| g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() && 
			( !IsVampire() && (!HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) || 
			IsVampire() && g_pZone->GetID() == 3001) )
			&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
			&& !g_bLight
#endif
			) 
		{
			return false;
		}
	
		//-------------------------------------------------------------
		// Item 줍기 가능 거리에 이미 있다면..
		//-------------------------------------------------------------
		if (max(abs(pItem->GetX()-m_X), abs(pItem->GetY()-m_Y)) <= 1)
		{
			DEBUG_ADD("Pickup Item in Trace Dist.");

			TraceNULL();
			
			// 추적을 완료했으므로 Item을 줍는다.
			PickupItem( pItem );	
			
			return true;
		}


		// 추적 정보 설정
		SetTraceID( id );
		m_fTrace	= FLAG_TRACE_ITEM;
		m_TraceX	= pItem->GetX();
		m_TraceY	= pItem->GetY();
		m_TraceZ	= 0;

		// 현재 추적하는 대상에 대해서 기억해둔다.
		m_fTraceBuffer	= m_fTrace;	
		

		//------------------------------------------------------------		
		// 현재 Player의 능력에 따라서 TraceDistance가 달라질까?
		//------------------------------------------------------------
		
		m_TraceDistance		= 1;
		
		//------------------------------------------------------------

		// 추적하는 길 설정
		SetAction( m_MoveAction );
		SetNextDestination(m_TraceX, m_TraceY);

		// buffering을 없앤다.
		m_fNextTrace			= FLAG_TRACE_NULL;
	
		// 계속 쫓아가는 것을 중지
		m_bKeepTraceCreature	= false;

		//------------------------------------------------------------
		// message출력
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("Trace IteM: %d", id);				
	}
	else
	{
		//------------------------------------------------------------
		// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
		//------------------------------------------------------------
		if (m_bRepeatAction)
		{
			m_fNextTrace	= FLAG_TRACE_NULL;

			return false;
		}
		else
		{
			m_fNextTrace	= FLAG_TRACE_ITEM;
			m_NextTraceID	= id;

			m_bRepeatAction = FALSE;

			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------
// Trace InteractionObject
//----------------------------------------------------------------------
// InteractionObject로 다가가서 currentAction을 변경시킨다.
// --> 다가갔을때, currentAction이 
//     "이미" 변경되어있으면 변경하지 않아야 한다.
//
// 멈춰있는 상태가 아니면.. Next에 설정해둔다.
//----------------------------------------------------------------------
bool	
MPlayer::TraceInteractionObject(TYPE_OBJECTID id, BYTE currentAction)
{
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay()
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket())	// [새기술3]		
	{
		return false;
	}

	//------------------------------------------------
	// 오토바이를 타고 있으면 문을 열 수 없다.
	//------------------------------------------------
	if (m_MoveDevice == MOVE_DEVICE_RIDE)
	{
		return false;
	}

	if (IsStop()) // || m_Action!=ACTION_ATTACK)
	{
		// Zone에 존재하는 Item인지 check한다.
		MImageObject*	pImageObject = m_pZone->GetImageObject(id);

		// ImageObject가 없으면 return
		if (pImageObject==NULL) return false;

		// InteractionObject가 아니면 return
		if (pImageObject->GetObjectType()!=MObject::TYPE_INTERACTIONOBJECT)
			return false;	 	

		// 추적 정보 설정
		SetTraceID( id );
		m_fTrace			= FLAG_TRACE_INTERACTIONOBJECT;
		m_TraceObjectAction	= currentAction;
		m_TraceX			= pImageObject->GetX();
		m_TraceY			= pImageObject->GetY();
		m_TraceZ			= 0;

		// 현재 추적하는 대상에 대해서 기억해둔다.
		m_fTraceBuffer	= m_fTrace;	

		//------------------------------------------------------------		
		// 현재 Player의 능력에 따라서 TraceDistance가 달라질까?
		//------------------------------------------------------------
		
		m_TraceDistance		= 1;
		
		//------------------------------------------------------------

		// 추적하는 길 설정
		SetAction( m_MoveAction );
		SetNextDestination(m_TraceX, m_TraceY);

		// buffering을 없앤다.
		m_fNextTrace			= FLAG_TRACE_NULL;
	
		// 계속 쫓아가는 것을 중지
		m_bKeepTraceCreature	= false;

		//------------------------------------------------------------
		// message출력
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("Trace InteractionObject: %d", id);				
	}
	else
	{
		//------------------------------------------------------------
		// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
		//------------------------------------------------------------
		if (m_bRepeatAction)
		{
			m_fNextTrace	= FLAG_TRACE_NULL;

			return false;
		}
		else
		{
			m_fNextTrace			= FLAG_TRACE_INTERACTIONOBJECT;
			m_NextTraceID			= id;
			m_NextTraceObjectAction	= currentAction;

			m_bRepeatAction = FALSE;

			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------
// Trace Effect
//----------------------------------------------------------------------
// SelectableEffect에 접근한다.
//----------------------------------------------------------------------
bool	
MPlayer::TraceEffect(TYPE_OBJECTID id)
{
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay()
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE )
		|| IsInCasket())	// [새기술3]
	{
		return false;
	}

	//------------------------------------------------
	// 오토바이를 타고 있으면 문을 열 수 없다.
	//------------------------------------------------
	if (m_MoveDevice == MOVE_DEVICE_RIDE)
	{
		return false;
	}

	if (IsStop()) // || m_Action!=ACTION_ATTACK)
	{
		MEffect*	pEffect = m_pZone->GetEffect( id );

		if (pEffect==NULL)
		{
			pEffect = m_pZone->GetGroundEffect( id );

			if (pEffect==NULL)
			{
				return false;
			}
		}
		
		if (!pEffect->IsSelectable())
			return false;	 	

		// 추적 정보 설정
		SetTraceID( id );
		m_fTrace			= FLAG_TRACE_EFFECT;
		//m_TraceObjectAction	= currentAction;
		m_TraceX			= pEffect->GetX();
		m_TraceY			= pEffect->GetY();
		m_TraceZ			= 0;

		// 현재 추적하는 대상에 대해서 기억해둔다.
		m_fTraceBuffer	= m_fTrace;	

		//------------------------------------------------------------		
		// 현재 Player의 능력에 따라서 TraceDistance가 달라질까?
		//------------------------------------------------------------
		
		m_TraceDistance		= 1;
		
		//------------------------------------------------------------

		// 추적하는 길 설정
		SetAction( m_MoveAction );
		SetNextDestination(m_TraceX, m_TraceY);

		// buffering을 없앤다.
		m_fNextTrace			= FLAG_TRACE_NULL;
	
		// 계속 쫓아가는 것을 중지
		m_bKeepTraceCreature	= false;

		//------------------------------------------------------------
		// message출력
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("Trace Effect: %d", id);				
	}
	else
	{
		//------------------------------------------------------------
		// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
		//------------------------------------------------------------
		if (m_bRepeatAction)
		{
			m_fNextTrace	= FLAG_TRACE_NULL;

			return false;
		}
		else
		{
			m_fNextTrace			= FLAG_TRACE_EFFECT;
			m_NextTraceID			= id;
			//m_NextTraceObjectAction	= currentAction;

			m_bRepeatAction = FALSE;

			return false;
		}
	}

	return true;
}



//----------------------------------------------------------------------
// Set Trace Position
//----------------------------------------------------------------------
// Creature가 있는 sector는 이미 Creature가 있기 때문에 갈 수 없다.
// 그러므로, Creature에 가장 근접하면서 현재 Player에서 가까운 
// Sector를 선택해야 한다.
//----------------------------------------------------------------------
/*
void
MPlayer::SetTraceDestination()
{
	// check하는 방향의 순서를 둔다.
	int		stepX = m_X - m_TraceX,
			stepY = m_Y - m_TraceY;
	int		signX = (stepX==0)? 0 : (stepX<0)? -1 : 1,
			signY = (stepY==0)? 0 : (stepY<0)? -1 : 1;

	int		check[8];

	// signX,signY로 
	switch (DetermineDirection(signX, signY))
	{
		case RIGHT :
			
		break;
	}
	

	//-------------------------------------------------------
	// 8방향을 모두 check
	//-------------------------------------------------------
	for (int i=0; i<8; i++)
	{		
		switch (i)
		{
			case DIRECTION_LEFT			: next.x--;	next.y++;	break;
			case DIRECTION_RIGHT		: next.x++;	next.y--;	break;
			case DIRECTION_UP			: next.x--;	next.y--;	break;
			case DIRECTION_DOWN			: next.x++;	next.y++;	break;
			case DIRECTION_LEFTUP		: next.x--;				break;
			case DIRECTION_LEFTDOWN		: next.y++;				break;
			case DIRECTION_RIGHTUP		: next.y--;				break;
			case DIRECTION_RIGHTDOWN	: next.x++;				break;
		}

		//-------------------------------------------------------
		// Zone의 영역 밖이면 check 안한다.
		//-------------------------------------------------------
		if (next.x<0 || next.y<0 
					|| next.x>=m_pZone->GetWidth() || next.y>=m_pZone->GetHeight()) continue;

		//-------------------------------------------------------
		// 갈 수 있으면 선택한다.
		//-------------------------------------------------------
		if (m_pZone->CanMove(next.x, next.y))
		{				
		}
	}
}
*/			

//----------------------------------------------------------------------
// Can Attack Tribe
//----------------------------------------------------------------------
// 어떤 종족을 공격할 수 있는 AttackMode인지를 알아본다.
//----------------------------------------------------------------------
bool
MPlayer::CanAttackTribe( enum CREATURETRIBE to ) const
{
	switch ( m_AttackMode )
	{
		//--------------------------------------------
		// 평화상태 : 아무도 공격 안 한다.
		//--------------------------------------------
		case ATTACK_MODE_PEACE :
			return false;
		break;

		//--------------------------------------------
		// 공격상태 : 아무나 공격 한다.
		//--------------------------------------------
		case ATTACK_MODE_AGGRESS :
			return true;
		break;

		//--------------------------------------------
		// 보통 : 적만 공격한다.
		//--------------------------------------------
		// 적 = Tribe가 같지 않은 경우..
		// NPC는 공격 못한다.
		case ATTACK_MODE_NORMAL :
			return GetCreatureTribe()!=to;// && to!=CREATURETRIBE_NPC;
		break;		
	}

	return false;
}

//----------------------------------------------------------------------
// Can Attack Guild
//----------------------------------------------------------------------
// 어떤 guild를 공격할 수 있는 AttackMode인지를 알아본다.
//----------------------------------------------------------------------
bool	
MPlayer::CanAttackGuild( const MCreature* pCreature ) const
{
	switch ( m_AttackMode )
	{
		//--------------------------------------------
		// 평화상태 : 아무도 공격 안 한다.
		//--------------------------------------------
		case ATTACK_MODE_PEACE :
			return false;
		break;

		//--------------------------------------------
		// 공격상태 : 아무나 공격 한다.
		//--------------------------------------------
		case ATTACK_MODE_AGGRESS :
			return true;
		break;

		//--------------------------------------------
		// 보통 : 적만 공격한다.
		//--------------------------------------------
		// 적 = Guild가 같지 않은 경우..
		case ATTACK_MODE_NORMAL :
		{
			int gn = pCreature->GetGuildNumber();

			return m_GuildNumber!=gn
					// 현재는 사용자끼리는 공격하면 안되기 때문에..
					// 사용자 길드가 아닌 경우만 공격할 수 있다.
					// 나중에는 이거 빠져야되지 않을까?

					// vampire인 경우. 일단 vampire만 AttackByGuild를 사용하기 때문
					&& gn!=GUILDID_VAMPIRE_DEFAULT && gn<MIN_USER_GUILDID
					|| GetCreatureTribe()!=pCreature->GetCreatureTribe();
					// && to!=CREATURETRIBE_NPC;
		}
		break;		
	}

	return false;
}

//----------------------------------------------------------------------
// Action In Trace Distance
//----------------------------------------------------------------------
// 행동이 가능한 거리에 있을 때의 행동 처리
//
// return true : 거리에 있을 때
//		  false : 거리 밖.
//----------------------------------------------------------------------
bool
MPlayer::ActionInTraceDistance()
{
	int nextAction;
	if (m_nUsedActionInfo==ACTIONINFO_NULL)
	{
		if (m_MoveDevice==MOVE_DEVICE_RIDE)
		{
			if (IsSlayer())
			{
				nextAction = ACTION_SLAYER_MOTOR_STAND;				
			}
			else
			{
				// 콩가루~~..
				//nextAction = ACTION_STAND;
			}
		}
		//-----------------------------------------------------
		// 걸어다닐때
		//-----------------------------------------------------
		else if( m_MoveDevice == MOVE_DEVICE_SUMMON_SYLPH )
		{
			if( IsOusters() )
			{
				nextAction = ACTION_OUSTERS_FAST_MOVE_STAND;
			}
		}
		else
		{
			nextAction = ACTION_STAND;
		}
	}
	else
	{
		nextAction = GetActionInfoAction(m_nUsedActionInfo);
	}

	//-------------------------------------------------------
	//
	// [ 추적 완료 ] 
	//
	// : 사정 거리 이내인 경우 --> 추적 중지 --> 추적 Action
	//
	//-------------------------------------------------------			
	if (max(abs(m_X-m_TraceX), abs(m_Y-m_TraceY)) 
		<= m_TraceDistance)
	{	
		//----------------------------------------------------
		//
		// 사정거리에 접근했으므로 Action Mode전환				
		//
		//----------------------------------------------------
				
		//----------------------------------------------------
		// 추적이 완료됐으므로 적절한 행동을 취하게 한다.
		// m_NextAction에 적절한 행동을 설정..
		//----------------------------------------------------
		//---------------------
		// 자신에게 행동
		//---------------------
		if (m_fTrace & FLAG_TRACE_SELF)
		{
			//---------------------------------------------------------------
			// 현재 선택된 특수기술에 따라서..
			//---------------------------------------------------------------					
			//m_ActionCount = m_ActionCountMax;
			m_MoveCount = m_MoveCountMax;					
			SetNextAction(nextAction);

			//--------------------
			// message
			//--------------------
			DEBUG_ADD("Do Self SpecialAction");							
			
			return true;
		}
		//----------------------------------------------------
		//
		//					기본 행동
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_CREATURE_BASIC)
		{
			//---------------------------------------------------------------
			// 현재 사용중인 무기에 따라서...
			//---------------------------------------------------------------
			// pCreature
			// m_TraceX, m_TraceY가 목적지
			//---------------------------------------------------------------
			//m_ActionCount = m_ActionCountMax;
			m_MoveCount = m_MoveCountMax;

			BasicActionToCreature();			// by viva	

			return true;
		}
		//----------------------------------------------------
		//
		//						특수 행동
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_CREATURE_SPECIAL)
		{
			//---------------------------------------------------------------
			// Slot에 설정된 특수공격(공격이 아닐수도 있겠지)에 따라서..
			//---------------------------------------------------------------
			// pCreature
			// m_TraceX, m_TraceY가 목적지
			//---------------------------------------------------------------
			//m_ActionCount = m_ActionCountMax;
			m_MoveCount = m_MoveCountMax;
			SetNextAction(nextAction);

			/*
			#ifdef	CONNECT_SERVER
				CGAttackNormal _CGAttackNormal;
				_CGAttackNormal.setX( m_X );
				_CGAttackNormal.setY( m_Y );
				_CGAttackNormal.setDir( m_Direction );
				_CGAttackNormal.setObjectID( m_TraceID );
				g_pSocket->sendPacket( &_CGAttackNormal );

				#ifdef	OUTPUT_DEBUG
					DEBUG_ADD( _CGAttackNormal.toString().c_str() );							
				#endif
			#endif
			*/

			//--------------------
			// message
			//--------------------
			DEBUG_ADD_FORMAT("Special Attack : %d", m_TraceID);							
			
			return true;
		}
		//----------------------------------------------------
		//
		//				기본 행동 -> Sector
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_SECTOR_BASIC)
		{
			//---------------------------------------------------------------
			// 현재 사용중인 무기에 따라서...
			//---------------------------------------------------------------
			// pCreature
			// m_TraceX, m_TraceY가 목적지
			//---------------------------------------------------------------
			//m_ActionCount = m_ActionCountMax;
			m_MoveCount = m_MoveCountMax;
			SetNextAction(nextAction);

			/*
			#ifdef	CONNECT_SERVER
				CGAttackNormal _CGAttackNormal;
				_CGAttackNormal.setX( m_X );
				_CGAttackNormal.setY( m_Y );
				_CGAttackNormal.setDir( m_Direction );
				_CGAttackNormal.setObjectID( m_TraceID );
				g_pSocket->sendPacket( &_CGAttackNormal );

				#ifdef	OUTPUT_DEBUG
					DEBUG_ADD( _CGAttackNormal.toString().c_str() );							
				#endif
			#endif
			*/

			//--------------------
			// message
			//--------------------
			DEBUG_ADD_FORMAT("Basic Attack Sector : (%d, %d)", m_TraceX, m_TraceY);
			
			return true;
		}
		//----------------------------------------------------
		//
		//				특수 행동 -> Sector
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_SECTOR_SPECIAL)
		{
			//---------------------------------------------------------------
			// Slot에 설정된 특수공격(공격이 아닐수도 있겠지)에 따라서..
			//---------------------------------------------------------------
			// pCreature
			// m_TraceX, m_TraceY가 목적지
			//---------------------------------------------------------------
			//m_NextAction = ACTION_SHOOT;
			//m_ActionCount = m_ActionCountMax; 
			m_MoveCount = m_MoveCountMax;
			SetNextAction(nextAction);

			/*
			#ifdef	CONNECT_SERVER
				CGAttackNormal _CGAttackNormal;
				_CGAttackNormal.setX( m_X );
				_CGAttackNormal.setY( m_Y );
				_CGAttackNormal.setDir( m_Direction );
				_CGAttackNormal.setObjectID( m_TraceID );
				g_pSocket->sendPacket( &_CGAttackNormal );

				#ifdef	OUTPUT_DEBUG
					DEBUG_ADD( _CGAttackNormal.toString().c_str() );							
				#endif
			#endif
			*/

			//--------------------
			// message
			//--------------------
			DEBUG_ADD_FORMAT("Special Attack Sector : (%d, %d)", m_TraceX, m_TraceY);							
			//*/

			return true;
		}
		//----------------------------------------------------
		//
		//					Item 추적
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_ITEM)
		{
			MItem*	pItem = m_pZone->GetItem( m_TraceID );

			// 추적을 완료했으므로 Item을 줍는다.
			if (pItem!=NULL
				&& (!g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() || IsVampire() && g_pZone->GetID() != 3001 || 
				!IsVampire() && HasEffectStatus( EFFECTSTATUS_LIGHTNESS )
				&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
				&& !g_bLight
#endif
				||!IsOusters())
				)
			{
				PickupItem( pItem );
			}

			return true;
		}
		//----------------------------------------------------
		//
		//			Inventory의 Item에 기술 사용
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_INVENTORY)
		{
			MItem* pItem = g_pInventory->GetItemToModify( m_TraceID );

			// 추적을 완료했으므로 Item을 줍는다.
			if (pItem!=NULL)
			{
				// ActionEffect()를 호출하기 위해서...
				m_MoveCount = m_MoveCountMax;
				SetNextAction(nextAction);
			}
			else
			{
				DEBUG_ADD_FORMAT("No such Item in Inventory: id=%d", m_TraceID);
			}
			
			return true;
		}
		//----------------------------------------------------
		//
		//			Effect추적
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_EFFECT)
		{
			MEffect* pEffect = m_pZone->GetEffect( m_TraceID );

			if (pEffect==NULL)
			{
				pEffect = m_pZone->GetGroundEffect( m_TraceID );

				if (pEffect==NULL)
				{
					return true;
				}
			}

			if (pEffect->IsSelectable())
			{		
					MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();

					if (pEffectTarget!=NULL)
					{
						bool bSendPacket = true;
						if (pEffectTarget->GetEffectTargetType()==MEffectTarget::EFFECT_TARGET_PORTAL)
						{					
							if (IsSlayer())
							{
								bSendPacket = false;
							}
						}						

						if (bSendPacket)
						{
							int serverID = pEffectTarget->GetServerID();

							CGSelectTileEffect _CGSelectTileEffect;
							_CGSelectTileEffect.setEffectObjectID( serverID );
							
							g_pSocket->sendPacket( &_CGSelectTileEffect );
						}
					}
			}

			return true;
		}	
		//----------------------------------------------------
		//
		//			InteractionObject추적
		//
		//----------------------------------------------------
		else if (m_fTrace & FLAG_TRACE_INTERACTIONOBJECT)
		{
			MImageObject* pImageObject = m_pZone->GetImageObject( m_TraceID );

			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
			{
				MInteractionObject*	pInteractionObject = (MInteractionObject*)pImageObject;
				pInteractionObject->SetNextAction( m_TraceObjectAction );
				
			}

			return true;
		}				

		return false;
	}
	//-------------------------------------------------------
	// 더 추적해야 하는 경우
	//-------------------------------------------------------
	else
	{
		//------------------------------------------
		// 추적할 길이 없는 경우 --> 길찾기
		//------------------------------------------
		if (m_listDirection.empty())
		{
			//---------------------
			// 기본 행동
			//---------------------
			if (m_fTrace & FLAG_TRACE_CREATURE_BASIC)
			{
				TraceCreatureToBasicAction( m_TraceID, m_bTraceCreatureToForceAttack );
			}
			//---------------------
			// 특수 행동
			//---------------------
			else  if (m_fTrace & FLAG_TRACE_CREATURE_SPECIAL)
			{
				TraceCreatureToSpecialAction( m_TraceID, m_bTraceCreatureToForceAttack );
			}
			//---------------------
			// 기본 행동 -> Sector
			//---------------------
			else if (m_fTrace & FLAG_TRACE_SECTOR_BASIC)
			{
				TraceSectorToBasicAction( m_TraceX, m_TraceY );
			}
			//---------------------
			// 특수 행동 -> Sector
			//---------------------
			else if (m_fTrace & FLAG_TRACE_SECTOR_SPECIAL)
			{
				// 2004, 9, 18, sobeit add start - 터렛 파이어는 사정거리 벗어나믄 무시 ㅜ.ㅜ 
				if (m_nUsedActionInfo==SKILL_TURRET_FIRE)
				{
					m_fNextTrace = FLAG_TRACE_NULL;
					UnSetRepeatAction();
					InstallTurretStopAttack();
					//SetAction(ACTION_STAND);
					return false;
				}
				// 2004, 9, 18, sobeit add end
				TraceSectorToSpecialAction( m_TraceX, m_TraceY );
			}
			//---------------------
			// Item 추적
			//---------------------
			else if (m_fTrace & FLAG_TRACE_ITEM)
			{
				TraceItem( m_TraceID );
			}
			//---------------------
			// Inventory에 기술 사용
			//---------------------
			else if (m_fTrace & FLAG_TRACE_INVENTORY)
			{
				TraceInventoryItem( m_TraceID );
			}
			//---------------------
			// Effect추적
			//---------------------
			else if (m_fTrace & FLAG_TRACE_EFFECT)
			{
				TraceEffect( m_TraceID );
			}
			//---------------------
			// InteractionObject추적
			//---------------------
			else if (m_fTrace & FLAG_TRACE_INTERACTIONOBJECT)
			{
				TraceInteractionObject( m_TraceID, m_TraceObjectAction );
			}

			// 이동~하면서 추적..
			//SetAction( m_MoveAction );
		}


		//m_NextAction = m_MoveAction;
		SetNextAction( m_MoveAction );
	}

	return false;
}

//----------------------------------------------------------------------
// Move
//----------------------------------------------------------------------
// MCreature::Move와 비슷하지만 MPlayer::Move는 
//    m_listDirection에서 방향을 하나씩 읽어서 움직이게 된다.
// 
// 단, 길을 가다가 막히면 다시 길을 정해야한다(SetDest...)
//----------------------------------------------------------------------
void	
MPlayer::ActionMove()
{
	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("ActionMove");
	#endif

	//----------------------------------------------------
	//
	// 다음 Sector에 도달한 경우	
	//
	//----------------------------------------------------
	// m_listDirection에서 방향을 하나 읽어와서 
	// check해보고 움직인다.
	//----------------------------------------------------
	if( HasEffectStatus( EFFECTSTATUS_TRYING ) )
	{
		MZone::EFFECT_MAP::const_iterator itr = g_pZone->GetGroundEffects();
		bool bFind = false;

		for(int i = 0; i < g_pZone->GetGroundEffectCount(); itr++,i++ )
		{
			const MEffect* pEffect = itr->second;
			
			if( GetX() == pEffect->GetX() &&
				GetY() == pEffect->GetY() &&
				pEffect->GetFrameID() == EFFECTSPRITETYPE_REGEN_TOWER_GROUND )
			{
				bFind = true;
				break;
			}
		}
		if( bFind == false )
			RemoveEffectStatus( EFFECTSTATUS_TRYING );
	}

	if (m_MoveCount==0)//m_sX==0 && m_sY==0)
	{
		TYPE_SECTORPOSITION	x, y;

		//-------------------------------------------------------------
		// fast move인 경우는 nextX, nextY에 좌표가 있다.
		//-------------------------------------------------------------
		if (m_bFastMove)
		{
			x = m_NextX;
			y = m_NextY;

			m_NextX = SECTORPOSITION_NULL;					

			//m_CurrentDirection = 
			m_Direction = GetDirectionToPosition( x, y );

		}
		//-------------------------------------------------------------
		// 일반적인 움직임
		//-------------------------------------------------------------
		else
		{
			//----------------------------------------------------
			// 목표위치가 없으면 움직이지 않는다.
			//----------------------------------------------------
			if (m_DestX==SECTORPOSITION_NULL && m_NextDestX==SECTORPOSITION_NULL) 
			{
				DEBUG_ADD("### Don't need to move any more - Move Stop! ###");
				
				m_MoveCount = m_MoveCountMax;
				return;
			}

				//sprintf(g_pDebugMessage->GetCurrent(), "ListSize = %d", m_listDirection.size());
			
			//----------------------------------------------------
			// 
			// 1. 길찾기를 해서 이동하게 하고..
			// 2. Server에서 검증 받기 위해 Send..
			//
			//----------------------------------------------------
			// Server로부터 기다리는 message의 개수 출력		
			if (m_SendMove != 0  && g_bNetStatusGood)
			{
				static int lastSendCount = 0;

				if (lastSendCount != m_SendMove)
				{
					DEBUG_ADD_FORMAT("Waiting MoveOK = %d", m_SendMove);						
					
					lastSendCount = m_SendMove;
				}
			}

			//----------------------------------------------------
			//
			//
			//                 추적 중이면..
			//
			//
			//----------------------------------------------------		
			if (m_fTrace != FLAG_TRACE_NULL)
			{				
				//----------------------------------------------------	
				// Creature 추적 중이면..
				// Creature가 사라지거나
				// 추적 목표좌표가 바뀔 수 있으므로 check해준다.
				//----------------------------------------------------	
				if ((m_fTrace & FLAG_TRACE_CREATURE_BASIC) 
					|| (m_fTrace & FLAG_TRACE_CREATURE_SPECIAL))
				{
					// 추적하는 Creature의 정보를 읽어온다.
					MCreature*	pCreature = m_pZone->GetCreature( m_TraceID );				

					//GET_DYING_CREATURE( pCreature, m_TraceID );

					//-------------------------------------------------------
					// 추적하는 Creature가 사라졌을 경우 --> 추적 중지
					// 내가 Slayer인 경우는 Darkness안에 들어간 캐릭을 쫓아갈 수 없다.					
					//-------------------------------------------------------								
					if (pCreature==NULL 
						|| pCreature->IsInDarkness() && !pCreature->IsNPC() && 
						(!IsVampire() && !HasEffectStatus( EFFECTSTATUS_LIGHTNESS )  || 
						g_pZone->GetID() == 3001 && IsVampire() ||
						pCreature->IsOusters() && pCreature->IsInGroundElemental() && !g_pPlayer->IsOusters())
						&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )						
#ifdef __METROTECH_TEST__
						&& !g_bLight
#endif
						) 
					{
						if (IsRepeatAction())
						{
							m_fNextTrace = FLAG_TRACE_NULL;

							// 버튼을 떼었으므로 행동 반복을 취소한다.
							UnSetRepeatAction();
							//TraceNextNULL();
						}
						else
						{
							// 추적 중지
							TraceNULL();
								
							// 멈춘다.
							SetStop();					
						}

						#ifdef OUTPUT_DEBUG_PLAYER_ACTION
							DEBUG_ADD("creature lost");
						#endif
							
						return;			
					}
				
					//-------------------------------------------------------			
					// 추적할려는 Creature의 좌표가 달라졌을 경우
					//-------------------------------------------------------			
					if (pCreature->GetX()!=m_TraceX ||
						pCreature->GetY()!=m_TraceY)
					{										
						m_NextDestX = pCreature->GetX();
						m_NextDestY = pCreature->GetY();
						// Z는 관계없다.
					}

					m_TraceX	= pCreature->GetX();
					m_TraceY	= pCreature->GetY();
					m_TraceZ	= pCreature->GetZ();
				}
				//----------------------------------------------------	
				// Item 추적 중이면..
				// Item이 사라질 수 있으므로 check해준다.
				//----------------------------------------------------
				else if (m_fTrace & FLAG_TRACE_ITEM)
				{
					// 추적하는 Creature의 정보를 읽어온다.
					MItem*	pItem = m_pZone->GetItem( m_TraceID );

					//-------------------------------------------------------
					// 추적하는 Item이 사라졌을 경우 --> 추적 중지
					//-------------------------------------------------------
					if (pItem==NULL
						|| g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness()&& 
						(!IsVampire() && (!HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) ||
						IsVampire() &&g_pZone->GetID() == 3001) )
						&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
						&& !g_bLight
#endif
						) 
					{
						// 추적 중지
						TraceNULL();
							
						// 멈춘다.
						SetStop();					
							
						#ifdef OUTPUT_DEBUG_PLAYER_ACTION
							DEBUG_ADD("item lost");
						#endif

						return;			
					}
				}

				//-------------------------------------------------------
				// 행동 가능 거리에 있는지 판단한다.
				//-------------------------------------------------------
				if (ActionInTraceDistance())
				{
					// 기술 사용~~
					if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
					{						
						m_nUsedActionInfo = m_nNextUsedActionInfo;
						m_nNextUsedActionInfo = ACTIONINFO_NULL;

						// 다음 action설정
						m_bNextAction = true;
						// 2004, 11, 23, sobeit add start - 아..감기걸려서..눈에 암것두 안들어온다..ㅜ.ㅜ
						if(m_nUsedActionInfo == SKILL_SET_AFIRE)
						{
							if(max(abs(m_X-m_TraceX), abs(m_Y-m_TraceY))<2)
								SetNextAction(ACTION_ATTACK);
							else
								SetNextAction(ACTION_STAND);
						}
						else
						// 2004, 11, 23, sobeit add end
							SetNextAction(GetActionInfoAction( m_nUsedActionInfo ));
					}

					// 더 이상 갈 필요가 없다.
					m_listDirection.clear();

					// Action중지
					//m_ActionCount = m_ActionCountMax;
					m_MoveCount = m_MoveCountMax;

					// Player가 바라보는 방향이 현재 적이 있는 방향이 되도록 한다.				
					SetDirectionToPosition( m_TraceX, m_TraceY );
					
							
					//----------------------------------------------------			
					// 계속 추적하는 경우가 아니면 추적 중지
					//----------------------------------------------------
					if (m_bKeepTraceCreature)
					{
						int fTrace = m_fTrace;
						
						// 정지
						SetStop();

						m_fTrace = fTrace;
					}	
					else
					{
						TraceNULL();

						// 정지
						SetStop();
					}
				}
			
			}

			//----------------------------------------------------
			// 길찾기 Buffering했던걸 설정한다.
			//----------------------------------------------------
			if (m_NextDestX!=SECTORPOSITION_NULL)
			{
				// 길찾기
				//SetDestination(m_NextDestX, m_NextDestY);
				m_DestX		= m_NextDestX;	// TEST
				m_DestY		= m_NextDestY;	// TEST
				m_NextDestX = SECTORPOSITION_NULL;
				m_NextDestY = SECTORPOSITION_NULL;			
			}

			//----------------------------------------------------
			// 길찾기 : 한 Tile 이동시마다 길찾기를 한다.
			//----------------------------------------------------
			if (m_DestX!=SECTORPOSITION_NULL)
				SetDestination(m_DestX, m_DestY);

			//----------------------------------------------------
			// Server에서 검증받기 위해서 Send
			//----------------------------------------------------
			//SetAction( m_MoveAction );

			
			//----------------------------------------------------
			// 검증받지 않은 움직임이 
			// 정해진 숫자 이하일 경우에만 움직이게 한다.
			//----------------------------------------------------
			if (m_SendMove > g_pClientConfig->MAX_CLIENT_MOVE)	
			{			
				g_bNetStatusGood = false;

				#ifdef OUTPUT_DEBUG_PLAYER_ACTION
					DEBUG_ADD("max client move");
				#endif
				
				return;
			}
			else
			{
				g_bNetStatusGood = true;
			}

			//----------------------------------------------------
			// 갈 길이 없는 경우
			//----------------------------------------------------
			if (m_listDirection.empty())
			{
				//--------------------------------------------
				// 목적지에 도달한 경우..
				//--------------------------------------------
				if (m_DestX==m_X && m_DestY==m_Y)
				{
					m_DestX = SECTORPOSITION_NULL;
					m_DestY = SECTORPOSITION_NULL;

					#ifdef OUTPUT_DEBUG_PLAYER_ACTION
						DEBUG_ADD("dest reached");
					#endif

					return;
				}

				#ifdef OUTPUT_DEBUG_PLAYER_ACTION
					DEBUG_ADD("no way");
				#endif

				return;
			}		

			
			//----------------------------------------------------	
			//
			// 방향대로 움직인다.
			//
			//----------------------------------------------------			
			//----------------------------------------------------
			// m_listDirection에서 방향을 하나 읽어온다.
			//----------------------------------------------------
			m_Direction = m_listDirection.front();
			m_listDirection.pop_front();	
				
			//----------------------------------------------------
			// 다음에 길찾기를 해야할 경우...
			//----------------------------------------------------
			if (m_NextDestX!=SECTORPOSITION_NULL)
			{
				//m_NextAction = m_MoveAction;
				SetNextAction( m_MoveAction );
			}

			//----------------------------------------------------
			// 읽어온 방향으로 한 Sector를 진행하고
			// cX,cY, sX,sY를 다시 지정해줘야 한다.
			//----------------------------------------------------

			//----------------------------------------------------
			// 이동 가능한지 check
			//----------------------------------------------------
			x = m_X;
			y = m_Y;

			switch (m_Direction)
			{
				case DIRECTION_LEFTDOWN		: x--;	y++;	break;
				case DIRECTION_RIGHTUP		: x++;	y--;	break;
				case DIRECTION_LEFTUP		: x--;	y--;	break;
				case DIRECTION_RIGHTDOWN	: x++;	y++;	break;
				case DIRECTION_LEFT			: x--;			break;
				case DIRECTION_DOWN			:		y++;	break;
				case DIRECTION_UP			:		y--;	break;
				case DIRECTION_RIGHT		: x++;			break;
			}
		}

		//--------------------------------------------------------------------
		// 움직일 수 있을 경우		
		//--------------------------------------------------------------------
		if (m_pZone->CanMove(m_MoveType, x,y)
			|| m_bFastMove		// Fast Move인 경우는 무조건 이동 가능..
			
#ifdef __EXPO_CLIENT__
			|| g_UserInformation.Invisible
#endif
			)
		{		
			//---------------------------------------------------------------
			// Fast Move인 경우..
			//---------------------------------------------------------------
			if (m_bFastMove)
			{
				// nothing
			}
			//---------------------------------------------------------------
			// 일반적인 움직임 --> 검증 필요
			//---------------------------------------------------------------
			else
			{
				//----------------------------------------------------
				// 지뢰 설치중이면 취소하기
				//----------------------------------------------------
				if (gC_vs_ui.IsInstallMineProgress())
				{
					gC_vs_ui.EndInstallMineProgress();
				}

				CGMove _CGMove;
				_CGMove.setX( m_X );
				_CGMove.setY( m_Y );
				_CGMove.setDir( m_Direction );
				g_pSocket->sendPacket( &_CGMove );

				//yckou
//				UpdateSocketOutput();
				// Server로 보낸 방향을 기억해둔다.
				m_listSendDirection.push_back( m_Direction );
				m_SendMove++;
				

				//----------------------------------------------------
				// NPC에게 말 건 상태에서 움직이는 경우..			
				//----------------------------------------------------
				if (m_WaitVerifyActionInfo==WAIT_VERIFY_NPC_ASK)
				{
					MCreature* pCreature = g_pZone->GetCreature( g_pPCTalkBox->GetNPCID() );

					if (pCreature!=NULL)
					{
						//----------------------------------------------------
						// 거리가 멀어지면 대화 중지당.
						//----------------------------------------------------
						if ((max(abs(m_X-pCreature->GetX()), abs(m_Y-pCreature->GetY()))) > TRADE_DISTANCE_NPC)
						{
							// 검증 제거
							SetWaitVerifyNULL();

							// Dialog 꺼준다.
							g_pUIDialog->ClosePCTalkDlg();
						}
					}
				}
			}

			//-----------------------
			// 시야 바꿔주기
			//-----------------------
			//m_pZone->MoveLightSight(m_X, m_Y, m_LightSight, m_Direction);
			//m_pZone->UnSetLightSight(m_X, m_Y, m_LightSight);
			//m_pZone->SetLightSight(x, y, m_LightSight);

			// 이전 좌표 기억
			TYPE_SECTORPOSITION ox = m_X;
			TYPE_SECTORPOSITION oy = m_Y;

			// zone의 sector의 정보를 바꿔준다.
			MovePosition( x, y );
			
			//CheckInDarkness();

			//--------------------------------------------
			//
			// 이동한 곳이 '포탈'인 경우
			//
			//--------------------------------------------
			if (IsSlayer())
			{
				const MSector& sector = m_pZone->GetSector(x, y);
				if (sector.IsPortal())
				{
					int numPortal = sector.GetPortalSize();		
					
					if (numPortal!=0)
					{
						PORTAL_LIST::const_iterator	iPortal = sector.GetPortalBegin();

						PORTAL_INFO portalInfo = *iPortal;			

						//--------------------------------------------
						// 멀티 포탈이면 elevator를 띄운다.
						//--------------------------------------------
						if (portalInfo.Type==MPortal::TYPE_MULTI_PORTAL)
						{
							DEBUG_ADD("[MPlayer] Run Elevator");
							
							gC_vs_ui.RunElevator();

							// 목적지 표시를 없앤다.
							g_pTopView->SetSelectedSectorNULL();

							// 목적지 없앰..
							m_DestX		= SECTORPOSITION_NULL;
							m_DestY		= SECTORPOSITION_NULL;	
							m_NextDestX = SECTORPOSITION_NULL;
							m_NextDestY = SECTORPOSITION_NULL;	
						}

						iPortal++;
					}
				}
			}

			//--------------------------------------------
			// 5 Frame이 있다고 할 경우
			//--------------------------------------------
			//
			// [1] UP,DOWN,LEFT,RIGHT일 때,
			//
			//     X변화 : 0 16 32 48 64   (+-16)
			//     Y변화 : 0  8 16 24 32   (+-8)
			// 
			//
			// [2] 대각선(LEFTUP,LEFTDOWN,RIGHTUP,RIGHTDOWN)으로 움직일때,
			//
			//     X변화 : 0  8 16 24 32   (+-8)
			//     Y변화 : 0  4  8 12 16   (+-4)
			//
			//--------------------------------------------
			// sX,sY : 움직여야할 전체 pixel(한 TILE)
			// cX,cY : 이동하는 단위 pixel
			//--------------------------------------------		
			//--------------------------------------------
			// Fast Move 인 경우
			//--------------------------------------------
			if (m_bFastMove)
			{
				// 직선 거리로 움직일때의 pixel거리
				// ex) 왼쪽 2 --> 1 : ( 2 - 1 ) * TILE_X
				//     위쪽 2 --> 1 : ( 2 - 1 ) * TILE_Y
				m_sX = (ox - m_X) * TILE_X;
				m_sY = (oy - m_Y) * TILE_Y;
					
				//------------------------------------------------
				// 실제 한 타일 이동 속도의 반의 속도에 목적지까지 이동한다.
				//------------------------------------------------
				// 한 타일 이동할때의 Frame 수
				int moveTimes_div_2 = (*g_pCreatureTable)[m_CreatureType].MoveTimes >> 1;
				// 2005, 1, 5, sobeit add start
				if(HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
				{
//					moveTimes_div_2 = moveTimes_div_2/2;
					m_MoveCountMax = m_MoveCountMax*2;
				}
				// 2005, 1, 5, sobeit add end
				m_cX = -m_sX / moveTimes_div_2;
				m_cY = -m_sY / moveTimes_div_2;

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}
			}
			//--------------------------------------------	
			// 정상 이동
			//--------------------------------------------
			else
			{	
				m_DirectionMoved = m_Direction;

				m_sX = m_sXTable[m_DirectionMoved];
				m_sY = m_sYTable[m_DirectionMoved];

				//????????????????????????????????????????????????????????
				//???                                                  ???
				//???   한 Frame을 이동...해야하는가?? 말아야 하는가   ???
				//???                                                  ???
				//????????????????????????????????????????????????????????
				int moveTimes_1;
				
				#ifdef OUTPUT_DEBUG
					if (m_CreatureType >= g_pCreatureTable->GetSize())
					{
						DEBUG_ADD_FORMAT("[Error] CreatureType is exceed MAX. id=%d, type=%d", m_ID, m_CreatureType);
						
						SetStop();

						return;
					}
				#endif

				if (m_MoveDevice==MOVE_DEVICE_WALK)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_RIDE)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else
				{					
					DEBUG_ADD_FORMAT("[Error] What is MoveType of this Player??? id=%d", m_ID);							
					
					SetStop();

					return;
				}

				m_MoveTableCount = 0;
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
			}

			m_sX += m_cX;
			m_sY += m_cY;

			m_MoveCount++;

			// 다음에 이동할 count를 지정한다.
			m_NextMoveCount = (*g_pCreatureTable)[m_CreatureType].MoveRatio;

			//------------------------------------------------
			// 캐릭터의 MoveAction에 맞는 Sound를 출력해준다.
			//------------------------------------------------
			TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( m_MoveAction );

			if (soundID!=SOUNDID_NULL)
			{
				PlaySound( soundID,
							false,
							m_X,
							m_Y);
			}
		}	
		//--------------------------------------------
		// 움직일 수 없을 경우		
		//--------------------------------------------
		// 길찾기를 다시 해준다.
		else
		{
			m_NextDestX = m_DestX;
			m_NextDestY = m_DestY;
			m_listDirection.empty();


			DEBUG_ADD("다른 캐릭터에 의해서 Block됐음.");					
		}

		// 다음 Sector까지 움직인다.
		if (!m_listDirection.empty())
		{
			//m_NextAction = m_MoveAction;
			SetNextAction( m_MoveAction );		
		}
	}
	//----------------------------------------------------
	//
	// 아직 다음 Sector까지 덜 도달한 경우
	//
	//----------------------------------------------------
	else
	{
		// 한 Frame을 이동한다.
		/*
		switch (m_MoveDevice)
		{
			case MOVE_DEVICE_NULL :			
				m_sX += m_cXTableDeviceNull[m_Direction][m_ActionCount];
				m_sY += m_cYTableDeviceNull[m_Direction][m_ActionCount];
			break;

			case MOVE_DEVICE_MOTOR1 :						
				m_sX += m_cXTableDeviceMotor1[m_Direction][m_ActionCount];
				m_sY += m_cYTableDeviceMotor1[m_Direction][m_ActionCount];
			break;
		}
		*/

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("going");
		#endif

		// 이동할 count가 됐을때만 이동한다.
		if (m_MoveCount>=m_NextMoveCount)
		{			
			//--------------------------------------------
			// Fast Move 인 경우
			//--------------------------------------------
			if (m_bFastMove)
			{
				// 2001.8.22 추가 - 냠냠
				if (m_sX!=0)	m_sX += m_cX;
				if (m_sY!=0)	m_sY += m_cY;		

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}

				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				if (m_MoveCount+1 >= m_MoveCountMax
					&& m_nNoPacketUsedActionInfo==ACTIONINFO_NULL)
				{
					//----------------------------------------------------------
					// 으으흠.. 테스트 코드.
					//----------------------------------------------------------
					// fast move 후에 한번 때리기.. - -
					// Flash Sliding
					//----------------------------------------------------------
					//m_TraceID	= id;		
					MCreature* pCreature = g_pZone->GetCreature( m_TraceID );

					//GET_DYING_CREATURE( pCreature, m_TraceID );

					if (pCreature!=NULL 
						&& m_nSpecialActionInfo != MAGIC_RAPID_GLIDING 
						&& m_nSpecialActionInfo != SKILL_TELEPORT
						&& !HasEffectStatus(EFFECTSTATUS_BIKE_CRASH)
						)		//-_- 현재 선택된게...로..
					{
//						_MinTrace(" SpecialActionInfo : %d UsedAction : %d NextUsedActionInfo :%d\n",
//							m_nSpecialActionInfo, m_nUsedActionInfo, m_nNextUsedActionInfo);
						#ifdef OUTPUT_DEBUG
							DEBUG_ADD("[FastMovePosition] set next action to attack");
						#endif

						UnSetRequestMode();

						// 사용한 기술로 설정
						
						//---------------------------------------------------
						// 우후훗..
						//---------------------------------------------------
						MActionResult* pResult = new MActionResult;

						// 결과 표현
						int ActionInfo = GetBasicActionInfo() ; //m_nBasicActionInfo;
						if( m_nSpecialActionInfo == SKILL_BLITZ_SLIDING || m_nSpecialActionInfo == SKILL_BLAZE_WALK )
						{
							m_fTrace	= FLAG_TRACE_CREATURE_SPECIAL;
							if(m_nSpecialActionInfo == SKILL_BLITZ_SLIDING )
								m_nNextUsedActionInfo	= SKILL_BLITZ_SLIDING_ATTACK;
							else if( m_nSpecialActionInfo == SKILL_BLAZE_WALK )
								m_nNextUsedActionInfo	= SKILL_BLAZE_WALK_ATTACK;
							else
								return;
							m_TraceX	= pCreature->GetX();
							m_TraceY	= pCreature->GetY();
							m_TraceZ	= pCreature->GetZ();
							
							m_bTraceCreatureToForceAttack	= false;
							m_bKeepTraceCreature			= false;
							m_TraceDistance		= m_BasicAttackDistance;
							
							m_bNextAction = true;
							SetNextAction(GetActionInfoAction( m_nNextUsedActionInfo ));
							SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );
							return;
						}
						if(m_nSpecialActionInfo == SKILL_SET_AFIRE)
						{
							m_nNextUsedActionInfo = SKILL_SET_AFIRE;
							m_nNoPacketUsedActionInfo = m_nNextUsedActionInfo;
							m_NextAction = ACTION_ATTACK;
							
							return;
						}
//						if(m_nSpecialActionInfo == MAGIC_BLOODY_ZENITH)
//						{
//							SetBloodyZenith(16);
//						}

						m_nNextUsedActionInfo	= GetBasicActionInfo() ; //m_nBasicActionInfo;

						pResult->Add( new MActionResultNodeActionInfo(
														ActionInfo,
														m_ID,
														pCreature->GetID(), 
														pCreature->GetX(),
														pCreature->GetY(),
														0 ) 
										);	

						// Player의 현재 진행 중인 기술의 결과로 추가시킨다.
						if( m_nUsedActionInfo == SKILL_BLITZ_SLIDING_ATTACK ||
							m_nUsedActionInfo == SKILL_BLAZE_WALK_ATTACK 
							)
							m_nUsedActionInfo = GetBasicActionInfo(); //m_nBasicActionInfo;
						
						int tempActionInfo = m_nUsedActionInfo;
						m_nUsedActionInfo = GetBasicActionInfo(); //m_nBasicActionInfo;	// 막가는 코드 - -;
						
						// 막간다 ~~  --;
						if (m_listEffectTarget.size()==0)
						{
							m_pEffectTarget = new MEffectTarget( 0 );
							m_pEffectTarget->NewEffectID();

							m_listEffectTarget.push_back( m_pEffectTarget );							
						}

						PacketAddActionResult( 0, pResult );

						m_nUsedActionInfo = tempActionInfo;
						//---------------------------------------------------
						// 우후후훗... - -; 2001.12.10에 추가.. ㅋㅋ.
						//---------------------------------------------------
						
						// 추적 정보 설정						
						m_fTrace	= FLAG_TRACE_CREATURE_BASIC;
						m_TraceX	= pCreature->GetX();
						m_TraceY	= pCreature->GetY();
						m_TraceZ	= pCreature->GetZ();

						m_bTraceCreatureToForceAttack	= true;
						m_bKeepTraceCreature			= false;
						m_TraceDistance		= m_BasicAttackDistance;
						
						m_bNextAction = true;
						SetNextAction(GetActionInfoAction( m_nNextUsedActionInfo ));
						SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );

						// 다음에 한 번 패킷을 안 보낸다.
						m_nNoPacketUsedActionInfo = m_nNextUsedActionInfo;
					}
					else
					{
						#ifdef OUTPUT_DEBUG
							DEBUG_ADD_FORMAT("[FastMovePosition] there is no such creature.id=%d", m_TraceID);
						#endif
					}
				}
	
			}
			//--------------------------------------------
			// 일반적인 이동의 경우
			//--------------------------------------------
			else
			{
				int moveTimes_1;
							
				if (m_MoveDevice==MOVE_DEVICE_WALK)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_RIDE)
				{	
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this)))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] What is MoveType of this Creature??? id=%d", m_ID);					
					SetStop();					
					return;
				}

				m_MoveTableCount++;
				// 2004, 12, 22, sobeit add start - max 값 넘어가면 엉뚱한 좌표가 들어간다.
				if(m_MoveTableCount > moveTimes_1)
					m_MoveTableCount = moveTimes_1;
				// 2004, 12, 22, sobeit add end
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];

				// 2001.8.22 추가 - 냠냠
				if (m_sX!=0)	m_sX += m_cX;
				if (m_sY!=0)	m_sY += m_cY;		
			
				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				m_MoveCount++;
			}
		}	
		else
		{
			m_MoveCount++;
		}
		
		//m_ActionCount++;
		// frame을 바꿔준다. 마지막 Frame까지 갔으면 0번째 Frame으로 바꾼다.
		//if (++m_ActionCount == (*m_pFrames)[m_Action][m_Direction].GetCount())
		//	m_ActionCount = 0;

		//if (m_ActionCount == (m_ActionCountMax>>1))
		//if (m_MoveCount == (m_MoveCountMax>>1))
		//{				
		//	SetLightSight( m_LightSight );
		//}
	}

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("moveok");
	#endif
}

//----------------------------------------------------------------------
// Attach CastingEffect
//----------------------------------------------------------------------
void	
MPlayer::AttachCastingEffect(TYPE_ACTIONINFO nUsedActionInfo, BOOL bForceAttach)
{
	MCreature::AttachCastingEffect(nUsedActionInfo, bForceAttach);

	// 켁..
}

//----------------------------------------------------------------------
// Affect UsedActionInfo
//----------------------------------------------------------------------
// m_nUsedActionInfo를 실행시킨다.
//----------------------------------------------------------------------
void
MPlayer::AffectUsedActionInfo(TYPE_ACTIONINFO nUsedActionInfo)
{
	if (nUsedActionInfo==ACTIONINFO_NULL)
	{
		return;
	}

	switch(nUsedActionInfo)
	{
	case SKILL_ACID_STORM:
		{
			if((*g_pRankBonusTable)[RANK_BONUS_WIDE_STORM].GetStatus() == RankBonusInfo::STATUS_LEARNED)
				nUsedActionInfo = SKILL_ACID_STORM_WIDE;
		}
		break;
		
	case SKILL_POISON_STORM:
		{
			if((*g_pRankBonusTable)[RANK_BONUS_WIDE_STORM].GetStatus() == RankBonusInfo::STATUS_LEARNED)
				nUsedActionInfo = SKILL_POISON_STORM_WIDE;
		}
		break;

	case SKILL_KASAS_ARROW:
		break;

	case SKILL_LARSLASH :
		
		break;					

	case SKILL_SWEEP_VICE_1:
	case SKILL_SWEEP_VICE_3:
	case SKILL_SWEEP_VICE_5:
		if(g_pSkillAvailable->IsEnableSkill( SKILL_SWEEP_VICE_1 ))
		{
		}
		else
			return;
		break;
		//by viva
//	case SKILL_SLAYER_HEAL_SACRED_STAMP:
//		return;
	}
	POINT point;	
#ifdef __METROTECH_TEST__
//	if(g_bLight)
//	{
//		int plusX = GetX()-m_TraceX;
//		int plusY = GetY()-m_TraceY;
//		int moveX = 0, moveY = 0;
//		if(plusX != 0)
//			moveX = plusX/abs(plusX);
//		if(plusY != 0)
//			moveY = plusY/abs(plusY);
//		
//		g_pPlayer->UnSetRequestMode();
//		
//		g_SelectSector.x = GetX()+moveX;
//		g_SelectSector.y = GetY()+moveY;
//		if(abs(g_SelectSector.x - m_TraceX) > GetActionInfoRange(nUsedActionInfo))
//			g_SelectSector.x = GetX();
//		if(abs(g_SelectSector.y - m_TraceY) > GetActionInfoRange(nUsedActionInfo))
//			g_SelectSector.y = GetY();
//		
//		// 다음 목표위치로 설정한다
//		g_pPlayer->TraceNULL();
//		
//		if (g_pPlayer->SetMovePosition(g_SelectSector.x, g_SelectSector.y))
//		{
//			g_pPlayer->SetNextActionToMove();
//		}
//		
//		// 다음 갈 곳이 없으면
//		POINT point;
//		g_pPlayer->GetNextDestination( point );		
//		if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
//		{
//			// 현재 가고 있는 곳이 없으면
//			g_pPlayer->GetDestination( point );
//			if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
//			{
//				//-_-;;
//			}
//			else
//			{
//				g_pTopView->SetSelectedSector( point );
//			}
//		}
//		else
//		{
//			g_pTopView->SetSelectedSector( point );
//		}		
//	}
#endif

	//-----------------------------------------------------
	// 이 기술에서 사용할 EffectTarget
	//-----------------------------------------------------
	MEffectTarget* pEffectTarget = NULL;

	DEBUG_ADD_FORMAT("[MPlayer-StartEffect] ActionInfo=%d, ActionCount=%d/%d", nUsedActionInfo, m_ActionCount, m_ActionCountMax);
	
	//--------------------------------------------------------
	// Casting ActionInfo인가?
	//--------------------------------------------------------
	BOOL bCastingAction = (*g_pActionInfoTable)[nUsedActionInfo].IsCastingAction();

	//--------------------------------------------------------
	// 목표 위치 Pixel좌표
	//--------------------------------------------------------
	point = MTopView::MapToPixel(m_TraceX, m_TraceY);
	//point.x += m_sX;
	//point.y += m_sY;
	//point.x += TILE_X_HALF;
	
	//--------------------------------------------------------
	//
	//					Effect 목표 설정
	//Z
	//--------------------------------------------------------
	//MEffectTarget* m_pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
	// 
	//--------------------------------------------------------
	// casting action인 경우
	//--------------------------------------------------------
	if (bCastingAction)
	{
		// casting용 EffectTarget을 생성한다.
		pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
		pEffectTarget->NewEffectID();

		if( nUsedActionInfo == CASTING_FIRE || nUsedActionInfo == CASTING_WATER ||
			nUsedActionInfo == CASTING_GROUND || nUsedActionInfo == CASTING_FIRE2 ||
			nUsedActionInfo == CASTING_GROUND2 || nUsedActionInfo == CASTING_WATER2 )
			pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );
		else
			pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );

		//--------------------------------------------------------
		// castingAction에서도 delay가 적용되지만,
		// castingAction인 경우는 delay를 제거하면 안된다.
		//--------------------------------------------------------
		// 이거 검증할 방법이 없다.
		//if (m_DelayActionInfo==nUsedActionInfo) - -;
		{
			pEffectTarget->SetDelayFrame( m_EffectDelayFrame );
			m_DelayActionInfo	= ACTIONINFO_NULL;
			m_EffectDelayFrame	= 0;
		}

	}
	//--------------------------------------------------------
	// casting action이 아닌.. 실제 기술인 경우..
	//--------------------------------------------------------
	else
	{
		//--------------------------------------------------------
		// 시작 기술이면.. 이미 new를 했으므로
		// 결과 기술인 경우에만 EffectTarget을 생성한다.
		//--------------------------------------------------------
		if (nUsedActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo())
		{
		}
		else
		{
			if (m_pEffectTarget != NULL)
			{
				DEBUG_ADD_FORMAT("RemoveEffectTarget: action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, nUsedActionInfo, m_pEffectTarget);
				
				RemoveEffectTarget( m_pEffectTarget->GetEffectID() );
				MEffectTarget* pTempEffectTarget = m_pEffectTarget;
				m_pEffectTarget = NULL;
				
				// delete되면서 m_pEffectTarget에 접근이 된다 - -;
				delete pTempEffectTarget;				
			}

			//----------------------------------------------------------
			// 결과를 하나 생성시켜 둔다.
			//----------------------------------------------------------
			m_pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			m_pEffectTarget->NewEffectID();

			// 나중에 지워주기 위해서..
			AddEffectTarget( m_pEffectTarget );
			//m_listEffectTarget.push_back( pEffectTarget );

			DEBUG_ADD_FORMAT("NewEffectTarget: action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, nUsedActionInfo, m_pEffectTarget);
		}
		

		if (m_pEffectTarget==NULL)
		{	
			DEBUG_ADD_FORMAT("EffectTarget is NULL, newEffectTarget : %x", m_pEffectTarget);
			
			m_pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			m_pEffectTarget->NewEffectID();

			DEBUG_ADD("new EffectTarget OK");
		}

//		if( nUsedActionInfo == MAGIC_ACID_TOUCH || nUsedActionInfo == MAGIC_BLOODY_NAIL ||
//			nUsedActionInfo == SKILL_FLOURISH || nUsedActionInfo == SKILL_ABSORB_SOUL)
		if(nUsedActionInfo != ACTIONINFO_NULL && (*g_pActionInfoTable)[nUsedActionInfo].IsAttachSelf() )
		{
			//m_pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
			m_pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );
		} else
		{
			m_pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
		}

		//--------------------------------------------------------
		// Action에 맞는 Sound 출력
		//--------------------------------------------------------
		// 결과인 경우만 여기서 사운드 출력한다.
		if (nUsedActionInfo >= g_pActionInfoTable->GetMinResultActionInfo())
		{
			int soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( 
									GetActionInfoAction(nUsedActionInfo) );

			PlaySound( soundID,	false, m_X, m_Y	);

			//-------------------------------------------------
			// Force Feel
			//-------------------------------------------------
			if (g_pUserOption->UseForceFeel && gpC_Imm!=NULL && gpC_Imm->IsDevice()
				&& soundID < g_pSoundTable->GetSize())
			{
				DEBUG_ADD_FORMAT("ForceAction-AffectUsedActionInfo(%d, %s)", soundID, strrchr((*g_pSoundTable)[soundID].Filename.GetString(), '\\'));
		
				gpC_Imm->ForceAction( soundID );
			}
		}

		//--------------------------------------------------------
		// 지속 시간 설정
		//--------------------------------------------------------
		if (m_DelayActionInfo==nUsedActionInfo)
		{
			m_pEffectTarget->SetDelayFrame( m_EffectDelayFrame );
			m_DelayActionInfo	= ACTIONINFO_NULL;
			m_EffectDelayFrame	= 0;
		}

		// 저장해두고
		pEffectTarget = m_pEffectTarget;
		
		// 지움.. 
		m_pEffectTarget = NULL;
	}

	// 진행중인 Effect로서 기억해둔다.		
	//AddEffectTarget( m_pEffectTarget );
	
	// 결과에 대한 정보를 설정 - delete는 결과를 적용할때..
	/*
	MActionResult* pResult = new MActionResult;
	pResult->Add( new MActionResultNodeActionInfo( m_ID, m_TraceID, nUsedActionInfo ) );
	
	m_pEffectTarget->SetResult( pResult );
	*/
	

	//--------------------------------------------------------
	//
	//					시작 위치를 결정한다.
	//
	//--------------------------------------------------------
	int x,y,z, direction;

	//--------------------------------------------------------
	// User 위치에서 시작하는 경우
	//--------------------------------------------------------
	if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartUser())
	{
		point = MTopView::MapToPixel(m_X, m_Y);
		point.x += m_sX;
		point.y += m_sY;
		//point.x += TILE_X_HALF;

		x			= point.x;
		y			= point.y;
		z			= m_Z+1;//+60;			
	}
	//--------------------------------------------------------
	// Target 위치에서 시작하는 경우
	//--------------------------------------------------------
	else if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartTarget())
	{
		x			= point.x;
		y			= point.y;
		z			= m_Z;//+60;			
	}

	//--------------------------------------------------------
	// 공중에서 시작하는 경우
	//--------------------------------------------------------
	if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartSky())
	{
		z	= m_Z + (*g_pActionInfoTable)[nUsedActionInfo].GetValue();

		//direction	= DIRECTION_DOWN;
	}
	//--------------------------------------------------------
	// 지상에서 시작하는 경우
	//--------------------------------------------------------
	//else
	//{
	if( nUsedActionInfo == SKILL_ABSORB_SOUL )
	{
		// m_TraceID 는 존의 시체이므로 아이템이다.		
		direction = g_pTopView->GetDirectionToPosition( 
			GetX(), GetY(),m_TraceX, m_TraceY);
	}
	else
		direction	= m_Direction;
	//}

	DEBUG_ADD_FORMAT("[MPlayer-StartEffect] Before Generate");
	
	//--------------------------------------------------------
	//
	//                   Effect생성		
	//
	//--------------------------------------------------------
	g_pEffectGeneratorTable->Generate(
			x,y,z,				// 시작 위치
			direction, 		// 방향
			1,					// power
			nUsedActionInfo,		//	ActionInfoTable종류,
			pEffectTarget		// 목표 정보
			, GetActionGrade()
	);
	ClearActionGrade();

	DEBUG_ADD_FORMAT("[MPlayer-StartEffect] AffectUsedActionInfo OK");
}

//----------------------------------------------------------------------
// Action to Send Packet
//----------------------------------------------------------------------
// packet을 보낸당...
//----------------------------------------------------------------------
void
MPlayer::ActionToSendPacket()
{
	//--------------------------------------------------------
	// 결과 행동이 아닌 경우만 packet을 보낸다.
	//--------------------------------------------------------
	if (m_nUsedActionInfo >= g_pActionInfoTable->GetMinResultActionInfo() || m_nUsedActionInfo == ACTIONINFO_NULL)
	{
		return;
	}

	switch(m_nUsedActionInfo)
	{
	case SKILL_ACID_STORM_WIDE:
		m_nUsedActionInfo = SKILL_ACID_STORM;
		break;

	case SKILL_POISON_STORM_WIDE:
		m_nUsedActionInfo = SKILL_POISON_STORM;
		break;
	case SKILL_BURNING_SOUL_CHARGING:
		if(IsBurningSol()) 
			return;
		break;
	case SKILL_SWEEP_VICE_1:
	case SKILL_SWEEP_VICE_3:
	case SKILL_SWEEP_VICE_5:
//		if(!GetSweepViewValue()) //스윕바이스를 아직 사용중이 아닐때
//		{
//			SetSweepViewValue(1);
//			g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
//			ExecuteActionInfoFromMainNode(SKILL_CLIENT_SWEEP_VICE,g_SelectSector.x, g_SelectSector.y, 0,g_pPlayer->GetDirection(),0	,	
//				g_SelectSector.x, g_SelectSector.y, 0, 0xffff, NULL, false);
//			return;
//		}
		if(g_bRButtonDown/*||!GetSweepViewValue()*/) 
			return;
			
		if(2 == GetSweepViewValue() || 3 == GetSweepViewValue())
			m_fTraceBuffer = FLAG_TRACE_SECTOR_SPECIAL;
		break;
	case SKILL_PLASMA_ROCKET_LAUNCHER:
		if(g_pCurrentMagazine != NULL)
		 {
			 int MagazineCount = g_pCurrentMagazine->GetNumber();
			 if(MagazineCount>0)
			 {
				 g_pCurrentMagazine->SetNumber(MagazineCount-1);
			 }
			 else
				 return;
		 }
		else return;
		break;
	// add by Coffee	2007-6-9
		
	case SKILL_BLLODY_SCARIFY :
	case SKILL_BLOOD_CURSE :
	case SKILL_VAMPIRE_INNATE_DEADLY_CLAW://by viva
	case SKILL_VAMPIRE_POISON_VOODOO_RING://by viva
/*		if (IsVampire() && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
		{
			if (!IsWaitVerify()
				&& IsItemCheckBufferNULL())
			{
				MItem* pItem = NULL;
				pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 6);
				if (pItem!=NULL)
				{
					CGUseItemFromInventory _CGUseItemFromInventory;
					_CGUseItemFromInventory.setObjectID( pItem->GetID() );
					_CGUseItemFromInventory.setX( pItem->GetGridX() );
					_CGUseItemFromInventory.setY( pItem->GetGridY() );
					g_pSocket->sendPacket( &_CGUseItemFromInventory );
					SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
				}
				else
				{
					g_pGameMessage->AddFormat("[瓊刻] %s",(*g_pGameStringTable)[STRING_STATUS_NOT_FIND_SKILL_CRAD].GetString());
					return;
				}
			}
		}else return;
*/		break;
	case SKILL_SHINE_SWORD :
	case SKILL_BOMB_CRASH_WALK :
	case SKILL_SATELLITE_BOMB :
	case SKILL_ILLUSION_INVERSION :
	case SKILL_HEAVEN_GROUND :
	case SKILL_SLAYER_SWORD_SKY_FIRE://by viva
	case SKILL_SLAYER_BLADE_CUT_STORM://by viva
	case SKILL_SLAYER_GUN_BOMB_XRL_MISSILE://by viva
	case SKILL_SLAYER_HEAL_SACRED_STAMP://by viva
	case SKILL_SLAYER_ENCHANT_BRAMBLE_HALO://by viva

/*		if (IsSlayer() && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
		{
			if (!IsWaitVerify()
				&& IsItemCheckBufferNULL())
			{
				MItem* pItem = NULL;
				pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 5);
				if (pItem!=NULL)
				{
					CGUseItemFromInventory _CGUseItemFromInventory;
					_CGUseItemFromInventory.setObjectID( pItem->GetID() );
					_CGUseItemFromInventory.setX( pItem->GetGridX() );
					_CGUseItemFromInventory.setY( pItem->GetGridY() );
					g_pSocket->sendPacket( &_CGUseItemFromInventory );
					SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
				}
				else
				{
					g_pGameMessage->AddFormat("[瓊刻] %s",(*g_pGameStringTable)[STRING_STATUS_NOT_FIND_SKILL_CRAD].GetString());
					return;
				}
			}
		}else return;
*/		break;
	case SKILL_DUMMY_DRAKE :
	case SKILL_HYDRO_CONVERGENCE :
	case SKILL_SUMMON_CLAY :
	case SKILL_HETER_CHAKRAM :
/*		if (IsOusters() && g_pZoneTable->Get( m_pZone->GetID() )->CannotUseSpecialItem == false)
		{
			if (!IsWaitVerify()
				&& IsItemCheckBufferNULL())
			{
				MItem* pItem = NULL;
				pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 7);
				if (pItem!=NULL)
				{
					CGUseItemFromInventory _CGUseItemFromInventory;
					_CGUseItemFromInventory.setObjectID( pItem->GetID() );
					_CGUseItemFromInventory.setX( pItem->GetGridX() );
					_CGUseItemFromInventory.setY( pItem->GetGridY() );
					g_pSocket->sendPacket( &_CGUseItemFromInventory );
					SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
				}
				else
				{
					g_pGameMessage->AddFormat("[瓊刻] %s",(*g_pGameStringTable)[STRING_STATUS_NOT_FIND_SKILL_CRAD].GetString());
					return;
				}
			}
		}else return;
*/		break;
	// add end by Coffee	
	}

	/*
	// [적절한 타이밍]에 .. 
	BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND);
	int		actionCountMax_1 = m_ActionCountMax-1;
	BOOL	bEndAction		= (m_ActionCount==actionCountMax_1);// || bActionStand;
	int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

	int		packetSendTiming = min(actionCountMax_1, StartFrame);
	BOOL	bStartAction	= (m_ActionCount==packetSendTiming);	// packetSendTiming이 원래는 0이었다.
	
	if (bActionStand)
	{
		bEndAction = bStartAction;
	}
	*/

	BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND || IsOusters() && m_Action == ACTION_OUSTERS_FAST_MOVE_STAND);
	int		actionCountMax_1 = m_ActionCountMax-1;
	BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
	int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

	BOOL	bStartAction	= (m_ActionCount==0);	
	

	// Effect가 시작되는 경우는..
	// (1) StartFrame인 경우
	// (2) 마지막 ActionFrame인 경우
	BOOL	bStartEffect = m_ActionCount==StartFrame || 
							StartFrame >= m_ActionCountMax && bEndAction;
	
	//BOOL	bCastingEffect	= GetActionInfoCastingStartFrame(m_nUsedActionInfo)==m_ActionCount;
		// 반복 action의 시작 frame
	bStartAction |= m_bRepeatAction 
					&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
					&& m_RepeatCount!=0
					&& m_ActionCount == GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

	//----------------------------------------------------------------------
	// [ TEST CODE ]
	//----------------------------------------------------------------------
	// 상당히 위험한 짓이지만.. - -;;
	// 크게 문제는 없을 듯...
	//----------------------------------------------------------------------
	// EffectTarget은 casting동작이 끝난 뒤에 생성되는데,
	// EffectTarget의 ID를 casting시작때에 server로 보내야 하기 때문에
	// 임시로... EffectTarget을 먼저 생성시켜두고 사용했다.
	//----------------------------------------------------------------------
	// 음. data member로 바꿨다. - -;;
	//static MEffectTarget* 

	
	#ifdef OUTPUT_DEBUG
			//DEBUG_ADD_FORMAT("action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, m_nUsedActionInfo, m_pEffectTarget);
	#endif

	//----------------------------------------------------------
	//
	//					동작 시작 시점..
	//
	//----------------------------------------------------------
	if (//bStartEffect)
		bStartAction || m_nUsedActionInfo == SKILL_BURNING_SOUL_LAUNCH)
	{			
		//----------------------------------------------------------
		// Packet을 안 보내는 경우이다.
		//----------------------------------------------------------
		if (m_nNoPacketUsedActionInfo==m_nUsedActionInfo)
		{
			m_nNoPacketUsedActionInfo = ACTIONINFO_NULL;

			int soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( 
											GetActionInfoAction(m_nUsedActionInfo) );

			//------------------------------------------------
			// 캐릭터의 Action에 맞는 Sound를 출력해준다.
			//------------------------------------------------
			PlaySound( soundID,	false, m_X, m_Y	);

			//-------------------------------------------------
			// Force Feel
			//-------------------------------------------------
			if (g_pUserOption->UseForceFeel && gpC_Imm!=NULL && gpC_Imm->IsDevice())				
			{				
				if ((*g_pActionInfoTable)[m_nUsedActionInfo].IsWeaponTypeGunAny())
				{
					soundID = SOUND_SLAYER_ATTACK_AR;
				}

				if (soundID < g_pSoundTable->GetSize())
				{
					DEBUG_ADD_FORMAT("ForceAction-ActionToSendPacket(%d, %s)", soundID, strrchr((*g_pSoundTable)[soundID].Filename.GetString(), '\\'));
					gpC_Imm->ForceAction( soundID );
				}
			}

			return;
		}

		//----------------------------------------------------------------------
		// 이 단계에서 Packet을 보내야 한다.
		//----------------------------------------------------------------------
		// 단, ActionEffect()가 호출됐다고 해서 모든 경우에 
		// 다 Packet전송이 일어나는 것은 아니다.
		//
		// Server에서 받아서 Player가 어떤 Action을 취할 경우도 있기 때문이다.
		//
		// ActionEffect()가 불리기 전에..
		// Packet을 보낼지 말지를 결정하는 변수가 필요할 것이다.
		//
		// (!) Packet에는 사용한 ActionInfo에 대한 Instance ID를 붙여줘야 한다.
		//     결과를 받았을 때 check를 위해서이다.
		//     그리고, 가장 최근의 Instance ID를 하나만 기억한다.		
		//----------------------------------------------------------------------

		// 결과를 보여주는 Action이 아니고
		// 실제로 Player가 사용한 Action일 경우에만 Packet을 보낸다.
		if (m_nUsedActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo())
		{
			//--------------------------------------------------------
			// 기술 사용 시간 Delay를 설정해준다.
			// 시작 기술인 경우에만..
			//--------------------------------------------------------
			if (m_Action==ACTION_STAND)
			{
				// 정지 동작 기술은 0.3초의 delay를 가진다.
				m_DelayTime	= g_CurrentTime + 300;
			}
			else if (m_bRepeatAction)
			{				
			}
			else
			{
				m_DelayTime	= g_CurrentTime 
								+ GetActionInfoDelay(m_nUsedActionInfo);
								// [적절한 타이밍]에 보낼때는 - 해야 한다.
								// - (m_ActionCount<<6);	// 지나간 ActionCount만큼 delay를 빼준다.
			}

		//----------------------------------------------------------
		//
		//			서버에 접속해 있는 경우..
		//
		//----------------------------------------------------------

			// 
			if (m_pEffectTarget != NULL)
			{
				DEBUG_ADD_FORMAT("RemoveEffectTarget: action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, m_nUsedActionInfo, m_pEffectTarget);
				
				RemoveEffectTarget( m_pEffectTarget->GetEffectID() );

				MEffectTarget* pTempEffectTarget = m_pEffectTarget;
				m_pEffectTarget = NULL;
				
				delete pTempEffectTarget;			
				pTempEffectTarget = NULL;
			}

			//----------------------------------------------------------
			// 결과를 하나 생성시켜 둔다.
			//----------------------------------------------------------
			m_pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[m_nUsedActionInfo].GetSize() );
			
			DEBUG_ADD_FORMAT("NewEffectTarget: action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, m_nUsedActionInfo, m_pEffectTarget);
			
			//---------------------------------------------------------
			// 진행중인 결과로서 추가한다.
			//----------------------------------------------------------
			AddEffectTarget( m_pEffectTarget );

			//----------------------------------------------------------
			// 새로운 EffectID를 발급?받는다.
			//----------------------------------------------------------
			m_pEffectTarget->NewEffectID();

			
			if (m_fTraceBuffer!=FLAG_TRACE_NULL)
			{
				//----------------------------------------------------------
				// 다음에 이 기술을 사용할 수 있는 시간 설정
				//----------------------------------------------------------
				int originalActionInfo = m_nUsedActionInfo;
				if( (*g_pActionInfoTable)[m_nUsedActionInfo].GetParentActionInfo() != ACTIONINFO_NULL )
					originalActionInfo = (*g_pActionInfoTable)[m_nUsedActionInfo].GetParentActionInfo();
				(*g_pSkillInfoTable)[originalActionInfo].SetNextAvailableTime();

				//----------------------------------------------------------
				// 보내는 packet 종류 설정
				//----------------------------------------------------------
				ACTIONINFO_PACKET	packetType		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetPacketType();
				

				//----------------------------------------------------------
				//
				//			상대에게 사용하는 것이면..
				//
				//----------------------------------------------------------
				// 기본 공격
				//----------------------------------------------------------
				if (m_fTraceBuffer & FLAG_TRACE_CREATURE_BASIC) 
				{
					CGAttack _CGAttack;
					_CGAttack.setX( m_X );
					_CGAttack.setY( m_Y );
					_CGAttack.setDir( m_Direction );
					_CGAttack.setObjectID( m_TraceID );
					g_pSocket->sendPacket( &_CGAttack );

					// [ TEST CODE ] - -;;
					// [ TEST CODE ] - -;;
					// [ TEST CODE ] - -;;
					/*
					#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__) && defined(_DEBUG)
						if (g_DXInput.KeyDown(DIK_LMENU))
						{
							for (int i=0; i<200; i++)
							{
								g_pSocket->sendPacket( &_CGAttack );
							}
						}
					#endif
					*/

				}
				//----------------------------------------------------------
				//
				// Blood drain인 경우
				//
				//----------------------------------------------------------
				else if (packetType==ACTIONINFO_PACKET_BLOOD_DRAIN)
				{
					CGBloodDrain _CGBloodDrain;
					_CGBloodDrain.setObjectID( m_TraceID );
					g_pSocket->sendPacket( &_CGBloodDrain );
			//		DEBUG_ADD("흡혈 메세지 보냄");									
					// 2001.8.20 주석처리
//					SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
				}
				//----------------------------------------------------------
				//
				// 흡영인 경우
				//
				//----------------------------------------------------------
				else if (packetType==ACTIONINFO_PACKET_ABSORB_SOUL)
				{
					CGAbsorbSoul _CGAbsorbSoul;
					_CGAbsorbSoul.setObjectID( g_pZone->GetItemID( m_TraceX, m_TraceY ) );
					_CGAbsorbSoul.setTargetZoneX( m_TraceX );
					_CGAbsorbSoul.setTargetZoneY( m_TraceY );

					int larvaSize = (*g_pItemTable)[ITEM_CLASS_LARVA].GetSize();
					MItem* pLarvaItem = NULL;
					for(int i = 0; i < larvaSize; i++)
					{
						 pLarvaItem= g_pInventory->FindItem( ITEM_CLASS_LARVA, i );
						if(pLarvaItem != NULL)
							break;
					}

					if(pLarvaItem == NULL)
					{
						_CGAbsorbSoul.setInvenObjectID(OBJECTID_NULL);
						_CGAbsorbSoul.setInvenX(255);
						_CGAbsorbSoul.setInvenY(255);
						_CGAbsorbSoul.setTargetInvenX(255);
						_CGAbsorbSoul.setTargetInvenY(255);
					}
					else
					{
						_CGAbsorbSoul.setInvenObjectID(pLarvaItem->GetID());
						_CGAbsorbSoul.setInvenX(pLarvaItem->GetGridX());
						_CGAbsorbSoul.setInvenY(pLarvaItem->GetGridY());

						MOustersPupa pupa;

						// 라바와 같은 타입으로
						pupa.SetItemType(pLarvaItem->GetItemType());

						POINT p;
						g_pInventory->GetFitPosition(&pupa, p);
						_CGAbsorbSoul.setTargetInvenX(p.x);
						_CGAbsorbSoul.setTargetInvenY(p.y);

						// (!!!) 검증 packet을 받을때까지 item을 못 움직이도록 해야한다.
						// Item관련 행위 중단
						SetItemCheckBuffer(pLarvaItem, ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY);
						
						// 기술 사용 중단
						// 2001.8.20 주석처리
						// 2001.10.26 주석해제 - Item은 검증되어야 한다.
						SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
					}

					g_pSocket->sendPacket( &_CGAbsorbSoul );

					
					// 2001.8.20 주석처리
					SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
				}
				//----------------------------------------------------------
				//
				// 다른 사람에게 사용하는 packet을 보내는 경우
				//
				//----------------------------------------------------------
				else if	(packetType==ACTIONINFO_PACKET_OTHER
						||	(m_fTraceBuffer & FLAG_TRACE_CREATURE_SPECIAL))
				{
					#ifndef __EXPO_CLIENT__

						//-----------------------------------------------
						// item을 사용해서 skill을 사용하는 경우
						//-----------------------------------------------					
						if ((*g_pActionInfoTable)[m_nUsedActionInfo].GetWeaponType()
							& FLAG_ACTIONINFO_WEAPON_HOLY_WATER)
						{						
							MItem* pUsingItem = g_pInventory->FindItem( ITEM_CLASS_HOLYWATER );
							
							//-----------------------------------------------
							// 사용할 item이 없는 경우
							//-----------------------------------------------
							if (pUsingItem==NULL)
							{
								//-------------------------------------------------
								// Skill Icon 체크 - 필요할려나.. - -;
								//-------------------------------------------------
								g_pSkillAvailable->SetAvailableSkills();

								// 추적 중지
								TraceNULL();

								//------------------------------------------
								// 제자리에서 흔들거리는 모습
								//------------------------------------------
								SetAction( ACTION_STAND );

								m_ActionCount++;

								m_nUsedActionInfo = ACTIONINFO_NULL;
	
								return;
							}

							CGThrowItem	_CGThrowItem;
							_CGThrowItem.setObjectID( pUsingItem->GetID() );	// item id
							_CGThrowItem.setTargetObjectID( m_TraceID );	//
							_CGThrowItem.setX( pUsingItem->GetGridX() );
							_CGThrowItem.setY( pUsingItem->GetGridY() );

							g_pSocket->sendPacket( &_CGThrowItem );						
							
							//--------------------------------------------------
							// 1개 이상인 경우에는 개수만 줄인다.
							//--------------------------------------------------
							if (pUsingItem->GetNumber()>1)
							{
								pUsingItem->SetNumber( pUsingItem->GetNumber() - 1 );
							}
							//--------------------------------------------------
							// 1개만 남은 경우는 Item을 완전히 제거한다
							//--------------------------------------------------
							else
							{
								MItem* pRemovedItem = g_pInventory->RemoveItem( pUsingItem->GetGridX(), pUsingItem->GetGridY() );

								if (pRemovedItem!=NULL)
								{
									// item정보 제거
									UI_RemoveDescriptor( (void*)pRemovedItem );

									delete pRemovedItem;								
								}
								else
								{
									DEBUG_ADD_FORMAT("[Error] Cannot Remove Item to use Skill. id=%d", pUsingItem->GetID());
								}

								//-------------------------------------------------
								// Skill Icon 체크
								//-------------------------------------------------
								g_pSkillAvailable->SetAvailableSkills();
							}
						}
						//-----------------------------------------------
						// 폭탄을 사용해서 skill을 사용하는 경우
						//-----------------------------------------------					
						if ((*g_pActionInfoTable)[m_nUsedActionInfo].GetWeaponType()
							& FLAG_ACTIONINFO_WEAPON_BOMB)
						{		
							int itemType;

							MItem* pUsingItem = NULL;

							if (m_nUsedActionInfo==SKILL_THROW_BOMB)
							{
								pUsingItem = g_pInventory->FindItem( ITEM_CLASS_BOMB );
							}
							else
							{
								itemType = m_nUsedActionInfo - BOMB_SPLINTER;

								pUsingItem = g_pInventory->FindItem( ITEM_CLASS_BOMB, itemType );
							}
							
							//-----------------------------------------------
							// 사용할 item이 없는 경우
							//-----------------------------------------------
							if (pUsingItem==NULL)
							{
								//-------------------------------------------------
								// Skill Icon 체크 - 필요할려나.. - -;
								//-------------------------------------------------
								g_pSkillAvailable->SetAvailableSkills();

								// 추적 중지
								TraceNULL();

								//------------------------------------------
								// 제자리에서 흔들거리는 모습
								//------------------------------------------
								SetAction( ACTION_STAND );

								m_ActionCount++;

								m_nUsedActionInfo = ACTIONINFO_NULL;
	
								return;
							}

							CGThrowBomb	cgThrowBomb;

							cgThrowBomb.setZoneX( m_TraceX );
							cgThrowBomb.setZoneY( m_TraceY );
							cgThrowBomb.setBombX( pUsingItem->GetGridX() );
							cgThrowBomb.setBombY( pUsingItem->GetGridY() );
							cgThrowBomb.setAttackSlayerFlag( m_TraceID!=OBJECTID_NULL );

							g_pSocket->sendPacket( &cgThrowBomb );						
							
							//--------------------------------------------------
							// 1개 이상인 경우에는 개수만 줄인다.
							//--------------------------------------------------
							if (pUsingItem->GetNumber()>1)
							{
								pUsingItem->SetNumber( pUsingItem->GetNumber() - 1 );
							}
							//--------------------------------------------------
							// 1개만 남은 경우는 Item을 완전히 제거한다
							//--------------------------------------------------
							else
							{
								MItem* pRemovedItem = g_pInventory->RemoveItem( pUsingItem->GetGridX(), pUsingItem->GetGridY() );

								if (pRemovedItem!=NULL)
								{
									// item정보 제거
									UI_RemoveDescriptor( (void*)pRemovedItem );

									delete pRemovedItem;								
								}
								else
								{
									DEBUG_ADD_FORMAT("[Error] Cannot Remove Item to use Skill. id=%d", pUsingItem->GetID());
								}

								//-------------------------------------------------
								// Skill Icon 체크
								//-------------------------------------------------
								g_pSkillAvailable->SetAvailableSkills();
							}
						}
						else
						{
							int SkillType = m_nUsedActionInfo;
							if( (*g_pActionInfoTable)[SkillType].GetParentActionInfo() != ACTIONINFO_NULL )
								SkillType = (*g_pActionInfoTable)[SkillType].GetParentActionInfo();

							if(SkillType == SKILL_TELEPORT || SkillType == SKILL_CHARGING_ATTACK)
							{
								g_pPlayer->SetDelay(1000);
							}
							CGSkillToObject _CGSkillToObject;
							_CGSkillToObject.setSkillType( SkillType );
							_CGSkillToObject.setCEffectID( m_pEffectTarget->GetEffectID() );			
							_CGSkillToObject.setTargetObjectID( m_TraceID );
							g_pSocket->sendPacket( &_CGSkillToObject );						
						}

					#else
					// 임시로..
					// BOMB기술은 target이 object와 tile이므로..
					// 무조건 tile로 보내준다.
						if (m_nUsedActionInfo==BOMB_TWISTER)
						{
							int SkillType = m_nUsedActionInfo;
							if( (*g_pActionInfoTable)[SkillType].GetParentActionInfo() != ACTIONINFO_NULL )
								SkillType = (*g_pActionInfoTable)[SkillType].GetParentActionInfo();

							CGSkillToTile _CGSkillToTile;
							_CGSkillToTile.setSkillType( SkillType );
							_CGSkillToTile.setCEffectID( m_pEffectTarget->GetEffectID() );
							_CGSkillToTile.setX( m_TraceX );
							_CGSkillToTile.setY( m_TraceY );
							
							g_pSocket->sendPacket( &_CGSkillToTile );
						}
						else
						{
							int SkillType = m_nUsedActionInfo;
							if( (*g_pActionInfoTable)[SkillType].GetParentActionInfo() != ACTIONINFO_NULL )
								SkillType = (*g_pActionInfoTable)[SkillType].GetParentActionInfo();

							if(SkillType == SKILL_TELEPORT || SkillType == SKILL_CHARGING_ATTACK)
							{
								g_pPlayer->SetDelay(1000);
							}
							CGSkillToObject _CGSkillToObject;
							_CGSkillToObject.setSkillType( SkillType );
							_CGSkillToObject.setCEffectID( m_pEffectTarget->GetEffectID() );			
							_CGSkillToObject.setTargetObjectID( m_TraceID );
							
							g_pSocket->sendPacket( &_CGSkillToObject );
						}
					#endif
						
					// 2001.8.20 주석처리
					SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
				}
				//----------------------------------------------------------
				//
				//			자신에게 사용하는 것이면..
				//
				//----------------------------------------------------------
				else if (packetType==ACTIONINFO_PACKET_SELF
						|| (m_fTraceBuffer & FLAG_TRACE_SELF))
				{
					//----------------------------------------------------------
					// OLD_PACKET
					//----------------------------------------------------------
					///*
					int originalActionInfo = m_nUsedActionInfo;
					if( (*g_pActionInfoTable)[originalActionInfo].GetParentActionInfo() != ACTIONINFO_NULL )
						originalActionInfo = (*g_pActionInfoTable)[originalActionInfo].GetParentActionInfo();
					CGSkillToSelf _CGSkillToSelf;
					_CGSkillToSelf.setSkillType( originalActionInfo );
					_CGSkillToSelf.setCEffectID( m_pEffectTarget->GetEffectID() );
					g_pSocket->sendPacket( &_CGSkillToSelf );

					// 2001.8.20 주석처리
					SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
					//*/
				}
				//----------------------------------------------------------
				//
				//				Tile에 사용하는 것이면..
				//
				//----------------------------------------------------------
				else if (packetType==ACTIONINFO_PACKET_ZONE
						|| (m_fTraceBuffer & FLAG_TRACE_SECTOR_BASIC) 
						|| (m_fTraceBuffer & FLAG_TRACE_SECTOR_SPECIAL))
				{
					//----------------------------------------------------------
					// OLD_PACKET
					//----------------------------------------------------------
					///*
					int SkillType = m_nUsedActionInfo;
					if( (*g_pActionInfoTable)[SkillType].GetParentActionInfo() != ACTIONINFO_NULL )
						SkillType = (*g_pActionInfoTable)[SkillType].GetParentActionInfo();

					if(SkillType == SKILL_TELEPORT || SkillType == SKILL_CHARGING_ATTACK)
					{
						g_pPlayer->SetDelay(1000);
					}
					else if(SkillType == SKILL_SWEEP_VICE_1)
					{
						if(1 == GetSweepViewValue())
							SkillType = SKILL_SWEEP_VICE_1;
						else if(2 == GetSweepViewValue())
							SkillType = SKILL_SWEEP_VICE_3;
						else if(3 == GetSweepViewValue())
							SkillType = SKILL_SWEEP_VICE_5;
						m_TraceX = m_SweepViceX;
						m_TraceY = m_SweepViceY;
					}
					else 	if(SkillType == SKILL_BURNING_SOUL_LAUNCH)
					{
						m_TraceX = m_TempSelectPosX;
						m_TraceY = m_TempSelectPosY;
					}
					
					CGSkillToTile _CGSkillToTile;
					_CGSkillToTile.setSkillType( SkillType );
					_CGSkillToTile.setCEffectID( m_pEffectTarget->GetEffectID() );
					_CGSkillToTile.setX( m_TraceX );
					_CGSkillToTile.setY( m_TraceY );
					g_pSocket->sendPacket( &_CGSkillToTile );

					// 2001.8.20 주석처리
					SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
					//*/
				}
				//----------------------------------------------------------
				//
				//				Inventory에 사용하는 것이면..
				//
				//----------------------------------------------------------
				else if (packetType==ACTIONINFO_PACKET_ITEM
						|| (m_fTraceBuffer & FLAG_TRACE_INVENTORY))
				{
					MItem* pItem = g_pInventory->GetItemToModify( m_TraceID );

					if (pItem!=NULL && IsItemCheckBufferNULL())
					{
						// inventory의 Item에 기술을 사용한다고 packet처리를 해주면 된다.
						CGSkillToInventory _CGSkillToInventory;
						
						_CGSkillToInventory.setSkillType( m_nUsedActionInfo );
						//_CGSkillToInventory.setCEffectID( m_pEffectTarget->GetEffectID() );
						_CGSkillToInventory.setObjectID( m_TraceID );
						_CGSkillToInventory.setX( pItem->GetGridX() );
						_CGSkillToInventory.setY( pItem->GetGridY() );

							
						// (!!!) 검증 packet을 받을때까지 item을 못 움직이도록 해야한다.
						// Item관련 행위 중단
						SetItemCheckBuffer(pItem, ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY);
						
						// 기술 사용 중단
						// 2001.8.20 주석처리
						// 2001.10.26 주석해제 - Item은 검증되어야 한다.
						SetWaitVerify( WAIT_VERIFY_SKILL_SUCCESS, m_nUsedActionInfo );
					}
					
				}

				
			}		
		}


		//----------------------------------------------------------------------
		// Packet 결과를 받을 경우에..
		//----------------------------------------------------------------------
		// Client에서 보낸 Instance ID를 같이 받아서..
		// 그 ID가 현재 저장된 InstanceID와 같으면 .. 아직 그 ActionInfo가
		// 진행중이라는 의미이다. Player의 ActionInfo가 끝날때는
		// Instance ID를 0으로? 해야하지 않을까..
		//
		// 같으면.. Server에서 날아온 ActionInfo의 결과를 기억시켜두고  
		//          나중에~~~ 출력하고..(ActionInfo가 끝난후에)
		// 다르면.. 즉시 출력하면 된다.
		//
		// 이렇게 강제로 시간차를 두어서.. Packet처리에 따른 흐름을 만들어야 한다.
		//----------------------------------------------------------------------

		// casting동작이 원래는 여기서 표현됐다. - -;

		//------------------------------------------------
		// 기술의 동작에 맞는 sound를 출력해준다.
		//------------------------------------------------
		//g_Sound.Play( g_SoundTable[(*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID()].pDSBuffer );
		//PlaySound( (*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID() ,
		//			false,
		//			m_X, m_Y);	

		//------------------------------------------------
		// 캐릭터의 Action에 맞는 Sound를 출력해준다.
		//------------------------------------------------
		int soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( 
										GetActionInfoAction(m_nUsedActionInfo) );
		if(!HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET) && !m_bBurningSol)
			PlaySound( soundID,	false, m_X, m_Y	);

		//-------------------------------------------------
		// Force Feel
		//-------------------------------------------------
		if (g_pUserOption->UseForceFeel && gpC_Imm!=NULL && gpC_Imm->IsDevice())				
		{				
			if ((*g_pActionInfoTable)[m_nUsedActionInfo].IsWeaponTypeGunAny())
			{
				soundID = SOUND_SLAYER_ATTACK_AR;
			}

			if (soundID < g_pSoundTable->GetSize())
			{
				DEBUG_ADD_FORMAT("ForceAction-ActionToSendPacket(%d, %s)", soundID, strrchr((*g_pSoundTable)[soundID].Filename.GetString(), '\\'));
				gpC_Imm->ForceAction( soundID );
			}
		}
	}
}

//----------------------------------------------------------------------
// Set ServerPosition
//----------------------------------------------------------------------
void		
MPlayer::SetServerPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	//--------------------------------------------------------
	// 모두 제거..
	//--------------------------------------------------------
	m_listSendDirection.clear();

	m_SendMove = 0;
	
	g_bNetStatusGood = true;

	//--------------------------------------------------------
	// Server에서 검증된 위치를 기억해둔다.
	//--------------------------------------------------------
	m_ServerX	= sX;
	m_ServerY	= sY;

	//--------------------------------------------------------
	// (m_ServerX, m_ServerY)에서부터 
	// Player의 시야에 포함되지 않는 Creature를 Zone에서 제거시킨다.
	//--------------------------------------------------------
	if (m_pZone!=NULL)
	{
		m_pZone->KeepObjectInSight(m_ServerX, m_ServerY, m_Sight);
	}

	SendPositionInfoToParty();
}

//----------------------------------------------------------------------
// Action Effect
//----------------------------------------------------------------------
// 동작이 모두 끝날때에 Effect효과를 수행한다.
//----------------------------------------------------------------------
void	
MPlayer::ActionEffect()
{
	/*
	// [적절한 타이밍]에 패킷을 보내는 코드
	// 
	BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND);
	int		actionCountMax_1 = m_ActionCountMax-1;
	BOOL	bEndAction		= (m_ActionCount==actionCountMax_1);// || bActionStand;
	int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );
	
	// 동작 끝부분에서 패킷 보낸다. 2001.8.1
	int		packetSendTiming = min(actionCountMax_1, StartFrame);
	BOOL	bStartAction	= (m_ActionCount==packetSendTiming);	// packetSendTiming이 원래는 0이었다.
	
	if (bActionStand)
	{
		bEndAction = bStartAction;
	}
	*/

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("ActionEffect");
	#endif

	// 2002.3.12
	// packet을 보내야하는 시작 동작의 기술이면..
	if (m_nUsedActionInfo < g_pActionInfoTable->GetMinResultActionInfo())
	{
		// 거리가 되는 경우만 ActionEffect를 수행한다.		
		if (max(abs(m_X-m_TraceX), abs(m_Y-m_TraceY)) 
			<= m_TraceDistance)
		{
			#ifdef OUTPUT_DEBUG_PLAYER_ACTION
				DEBUG_ADD("inDist");
			#endif
		}
		else
		{
			#ifdef OUTPUT_DEBUG_PLAYER_ACTION
				DEBUG_ADD("not inDist");
			#endif

			// 거리가 안 되면? 
			//if (m_listDirection.empty())
			{
				if (m_DestX != SECTORPOSITION_NULL
					&& m_DestY != SECTORPOSITION_NULL)
				{
					// 현재 가고 있는 목표가 있는 경우
				}
				else if (abs((int)m_X-m_TraceX) < g_SECTOR_WIDTH
						&& abs((int)m_Y-m_TraceY) < g_SECTOR_HEIGHT)
				{
					// 추적하는 경우
					m_DestX = m_TraceX;
					m_DestY = m_TraceY;
				}
				
				if (SetDestination(m_TraceX, m_TraceY))
				{
					#ifdef OUTPUT_DEBUG_PLAYER_ACTION
						DEBUG_ADD("NewWayOK");
					#endif

					// 갈 길이 있는 경우
					m_nNextUsedActionInfo = m_nUsedActionInfo;
					m_nUsedActionInfo = ACTIONINFO_NULL;
					
					SetAction( ACTION_STAND );
				}
				else
				{				
					// 갈길이 없는 경우
					// 또 이런 코드가 늘었다. 쩝.. 싹 뜯어고쳐야되는데 --;
					SetAction(((m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND));
					SetNextAction(ACTION_STAND);
					m_fTraceBuffer = 0;
					m_fNextTrace = 0;
					m_bTraceCreatureToForceAttack = false;
					SetStop();
					UnSetRepeatAction();
					TraceNULL();
					m_nUsedActionInfo = ACTIONINFO_NULL;
					m_nNextUsedActionInfo = ACTIONINFO_NULL;

					#ifdef OUTPUT_DEBUG_PLAYER_ACTION
						DEBUG_ADD("Stop!");
					#endif

					return;
				}

				return;
			}
		}
	}

	BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND);
	int		actionCountMax_1 = m_ActionCountMax-1;
	BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
	int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );
	
	BOOL	bStartAction	= (m_ActionCount==0 && !IsGunShotGuidance());
	
	// Effect가 시작되는 경우는..
	// (1) StartFrame인 경우
	// (2) 마지막 ActionFrame인 경우
	BOOL	bStartEffect = m_ActionCount==StartFrame || 
							StartFrame >= m_ActionCountMax && bEndAction;

	// 반복 action의 시작 frame
	bStartAction |= m_bRepeatAction 
					&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
					&& m_RepeatCount!=0
					&& m_ActionCount == GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

	//BOOL	bCastingEffect	= GetActionInfoCastingStartFrame(m_nUsedActionInfo)==m_ActionCount;
	

	//----------------------------------------------------------------------
	// [ TEST CODE ]
	//----------------------------------------------------------------------
	// 상당히 위험한 짓이지만.. - -;;
	// 크게 문제는 없을 듯...
	//----------------------------------------------------------------------
	// EffectTarget은 casting동작이 끝난 뒤에 생성되는데,
	// EffectTarget의 ID를 casting시작때에 server로 보내야 하기 때문에
	// 임시로... EffectTarget을 먼저 생성시켜두고 사용했다.
	//----------------------------------------------------------------------
	// 음. data member로 바꿨다. - -;;
	//static MEffectTarget* 

	
	#ifdef OUTPUT_DEBUG
			//DEBUG_ADD_FORMAT("action=(%d/%d) ActionInfo=%d, pEffectTarget=%x", m_ActionCount, m_ActionCountMax, m_nUsedActionInfo, m_pEffectTarget);
	#endif

	int SkillInfo = m_nUsedActionInfo;
	if(SkillInfo == SKILL_SWEEP_VICE_1)
	{
		if(!GetSweepViewValue() && g_bRButtonDown) //스윕바이스를 아직 사용중이 아닐때
		{
			if(g_pSkillAvailable->IsEnableSkill( SKILL_SWEEP_VICE_1)
				&&!IsDead()
				&&!(*g_pSkillInfoTable)[SKILL_SWEEP_VICE_1].GetAvailableTimeLeft()
				)
			{
				SetSweepViewValue(1);
				g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

				m_TempSelectPosX = g_SelectSector.x;
				m_TempSelectPosY = g_SelectSector.y;
				m_SweepViceX = m_TraceX;
				m_SweepViceY = m_TraceY;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_SWEEP_VICE,m_SweepViceX, m_SweepViceY, 0,g_pPlayer->GetDirection(),0	,	
						m_SweepViceX, m_SweepViceY, 0, 0xffff, NULL, false);
			}
			return;
		}
		else if(g_bRButtonDown)
			return;

	//	bStartEffect = true;

		if(1 == GetSweepViewValue())
			SkillInfo = SKILL_SWEEP_VICE_1;
		else if(2 == GetSweepViewValue())
			SkillInfo = SKILL_SWEEP_VICE_3;
		else if(3 == GetSweepViewValue())
			SkillInfo = SKILL_SWEEP_VICE_5;
		else
		{
			SkillInfo = SKILL_SWEEP_VICE_1;
			SetSweepViewValue(0);
		}
	}
	
	//----------------------------------------------------------
	//
	//					동작 시작 시점..
	//
	//----------------------------------------------------------
	if (//bStartEffect)		// Effect가 붙을때 packet을 보낸다.
		bStartAction)		// 동작 시작할때 packet을 보낸다.
	{	
		if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
		{
			 if(bStartEffect && g_pCurrentMagazine != NULL)
			 {
				 int MagazineCount = g_pCurrentMagazine->GetNumber();
				 if(MagazineCount>0)
				 {
					 g_pCurrentMagazine->SetNumber(MagazineCount-1);
					 ActionToSendPacket();
				 }
			 }
		}
		else
			ActionToSendPacket();
	}

	//-------------------------------------------------------------
	//
	// Casting effect
	//
	//-------------------------------------------------------------
	AttachCastingEffect( SkillInfo );

	//-------------------------------------------------------------
	// 정지 동작이면..
	// 바로 effect를 보여준다.
	//-------------------------------------------------------------

	// [적절한 타이밍] 맞게 보낼려면 이거 주석처리 해야한다.
	if (bActionStand)
	{
		m_ActionCount = actionCountMax_1;
	}

	//----------------------------------------------------------
	// action count 증가
	//----------------------------------------------------------
	// 왜 여기서 증가하냐면?
	// 밑에서.. actionCount가 바뀔 수 있기 때문에..
	//----------------------------------------------------------
	m_ActionCount++; 


	//--------------------------------------------------------	
	// 임시로 흡혈동작 중지하는거..
	//--------------------------------------------------------	
	if (m_bStopBloodDrain)
	{
		StopBloodDrain();
	}
	if (m_bStopAbsorbSoul)
	{
		StopAbsorbSoul();
	}

		
	//----------------------------------------------------------
	//
	//					Effect 시작
	//
	//----------------------------------------------------------
	// affect 후에 이게 바뀔 수도 있다 T_T;
	TYPE_ACTIONINFO currentUsedActionInfo = SkillInfo;

	if (bStartEffect)
	{
		AffectUsedActionInfo(SkillInfo);
	}
	




	//----------------------------------------------------------
	//
	//					Action동작이 끝날 때
	//
	//----------------------------------------------------------
	if (bEndAction)
		//&& (*g_pActionInfoTable)[ m_nUsedActionInfo ].GetSize()!=0)		
	{
		//--------------------------------------------------------
		// 추적 정보를 없애준다.
		//--------------------------------------------------------
		//m_TraceID	= OBJECTID_NULL;
		//m_Action	= ACTION_STAND;

		//--------------------------------------------------------
		// 기술 사용 시간 Delay를 설정해준다.
		// 시작 기술인 경우에만..
		//--------------------------------------------------------
		// [ TEST CODE ]
		//if (//currentUsedActionInfo!=ACTIONINFO_NULL && 
		//	currentUsedActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo())
		//{
		//	m_DelayTime	= g_CurrentTime + GetActionInfoDelay(currentUsedActionInfo);
		//}

		//m_bAffectUsedActionInfo = true;


		//------------------------------------------------------------
		// 반복 행동 체크..
		//------------------------------------------------------------
			///*
		if (CheckRepeatAction())
		{
			CheckBufferAction();
		}

		// 2002.3.24 checkRepeatAction() 위에 있던걸 여기로 내렸다.
		m_nUsedActionInfo = ACTIONINFO_NULL;		

		//*/
	}	

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("AEEnd");
	#endif
}


//----------------------------------------------------------------------
// 시야 수정
//----------------------------------------------------------------------
//void	
//MPlayer::SetLightSight(char LightSight)
//{
//	// 시야가 설정되어 있을 때, 시야 제거..
////	if (m_TimeLightSightX != SECTORPOSITION_NULL)
////	{
//////		m_pZone->UnSetLight(m_LightSightX, m_LightSightY, m_LightSight);	
////	}
//
//	// 설정
//	m_TimeLightSight = LightSight;
//
//	// 설정된 시야에 대한 좌표값을 Set..	
////	m_TimeLightSightX = m_X;
////	m_TimeLightSightY = m_Y;
//
//	// 새로운 시야 범위 지정.		
////	m_pZone->SetLight(m_X, m_Y, m_LightSight);			
//}

//----------------------------------------------------------------------
// 시야 삭제
//----------------------------------------------------------------------
//void	
//MPlayer::UnSetLightSight()
//{
//	// 시야가 설정되어 있을 때, 시야 제거..
////	if (m_LightSightX != SECTORPOSITION_NULL)
////	{
////		m_pZone->UnSetLight(m_LightSightX, m_LightSightY, m_LightSight);	
////	}
//	
//	//m_LightSight = 0;
//
////	m_LightSightX = SECTORPOSITION_NULL;
////	m_LightSightY = SECTORPOSITION_NULL;
//}
//
//----------------------------------------------------------------------
// Set ItemCheckBuffer
//----------------------------------------------------------------------
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
void MPlayer::SetItemCheckBuffer(MItem* pItem, enum ITEM_CHECK_BUFFER status, TYPE_OBJECTID SubItem)
	#else
void MPlayer::SetItemCheckBuffer(MItem* pItem, enum ITEM_CHECK_BUFFER status)
	#endif

{
	m_pItemCheckBuffer = pItem;

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		m_dwSubItemIDCheckBuffer = SubItem;
	#endif

	m_ItemCheckBufferStatus = status;	

	#ifdef OUTPUT_DEBUG
	DEBUG_ADD_FORMAT("[SetItemCheckBuffer] status %d", status);
//		switch (status)
//		{
//			case ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY :
//				DEBUG_ADD("ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY");
//			break;
//
//			case ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE :
//				DEBUG_ADD("ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE");
//			break;
//
//			case ITEM_CHECK_BUFFER_DROP_TO_ZONE :
//				DEBUG_ADD("ITEM_CHECK_BUFFER_DROP_TO_ZONE");
//			break;
//		}
	#endif

	UI_LockItem();
	UI_LockGear();
	UI_LockItemTrade();
}

//----------------------------------------------------------------------
// Clear ItemCheckBuffer
//----------------------------------------------------------------------
void
MPlayer::ClearItemCheckBuffer()
{
	DEBUG_ADD("Set Item Check Buffer to NULL");
	
	m_pItemCheckBuffer = NULL;
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		m_dwSubItemIDCheckBuffer = OBJECTID_NULL;
	#endif
	m_ItemCheckBufferStatus = ITEM_CHECK_BUFFER_NULL;

	// 2004, 8, 27, sobeit add start - 관소환 상태에서 인벤아이템 사용후 착용아이템 벗었다 입었다 하면 
	// 클라이언트 쪽에서 관상태가 풀린다.-_- 따라서 관 소환일땐 UI_UnlockGear()을 호출하지 않는다.
	if(!IsInCasket())
		UI_UnlockGear();
	// 2004, 8, 27, sobeit add end - 
	UI_UnlockItem();
	UI_UnlockItemTrade();
}

//----------------------------------------------------------------------
// 행동 결정
//----------------------------------------------------------------------
void	
MPlayer::SetAction(BYTE action)
{
//	if ( m_nSpecialActionInfo == MAGIC_BLOODY_ZENITH && m_Action == ACTION_ATTACK &&
//		m_ActionCount >= m_ActionCountMax && GetActionDelay() > 0)
//	{
//		SetActionDelay( GetActionDelay() - 1 );
//		m_ActionCount = m_ActionCountMax - 2;
//		return;
//	}	
	// 2004, 10, 6, sobeit add start
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET) && (action == ACTION_DAMAGED || action == ACTION_DRAINED))
		return;
	// 2004, 10, 6, sobeit add end
	if(HasEffectStatus(EFFECTSTATUS_SLEEP)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
//		return;
		action = ACTION_STAND;
	}

	// 무조건 꺼준다.
	m_bFastMove = false;
	//m_bFastMove = false;
	
	//-----------------------------------------------------------------
	// 현재 모습의 action에 맞는 경우. 2001.10.5
	//-----------------------------------------------------------------
	if (action < GetActionMax())
	{
		//-----------------------------------------------------
		// 오토바이 타고 있을때
		//-----------------------------------------------------
		if (m_MoveDevice==MOVE_DEVICE_RIDE)
		{
			if (IsSlayer())
			{
				if (action==ACTION_STAND)
				{
					action = ACTION_SLAYER_MOTOR_STAND;
				}
				else if (action==ACTION_MOVE)
				{
					action = ACTION_SLAYER_MOTOR_MOVE;
				}
			}
		}
		//-----------------------------------------------------
		// 걸어다닐때
		//-----------------------------------------------------
		else
		{
			if (IsSlayer())
			{
				if (action==ACTION_SLAYER_MOTOR_STAND)
				{
					action = ACTION_STAND;
				}
				else if (action==ACTION_SLAYER_MOTOR_MOVE)
				{
					action = ACTION_MOVE;
				}
			}
		}

		if( IsOusters() && (HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ) || m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH))
		{
			if (action==ACTION_STAND)
			{
				action = ACTION_OUSTERS_FAST_MOVE_STAND;
			} 
			else if( action == ACTION_MOVE )
			{
				action = ACTION_OUSTERS_FAST_MOVE_MOVE;
			}
		}

		#ifdef OUTPUT_DEBUG
			if (IsSlayer())
			{
				// slayer인 경우
				if (action >= GetActionMax())
				{
					DEBUG_ADD_FORMAT("[Error] Slayer's Action exceed MAX : id=%d, action=%d", m_ID, action);
					
					return;
				}
			}
			else if (IsVampire())
			{
				if (action >= GetActionMax())
				{
					DEBUG_ADD_FORMAT("[Error] Vampire's Action exceed MAX : id=%d, action=%d", m_ID, action);
					
					return;
				}
			}
			else if (IsOusters())
			{
				if (action >= GetActionMax())
				{
					DEBUG_ADD_FORMAT("[Error] Ousters's Action exceed MAX : id=%d, action=%d", m_ID, action);
					
					return;
				}
			}
			else if (IsNPC())
			{
				if (action >= GetActionMax())
				{
					DEBUG_ADD_FORMAT("[Error] NPC's Action exceed MAX : id=%d, action=%d", m_ID, action);
					
					return;
				}
			}
			else
			{
				// - -;;
				DEBUG_ADD("[Error] What is this Creature????");
				
				return;
			}
		#endif

		m_bNextAction = false;

		//------------------------------------------------
		// Move인 경우
		//------------------------------------------------
		if (action==m_MoveAction)
		{
			// 그냥 서있는 경우면... 끝동작으로 만든다.
			// Action이 끝났다고 표시해주기 위해서..
			if (m_Action==ACTION_STAND 
				|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND
				|| IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND)
			{			
				m_ActionCount = 0;
				m_ActionCountMax = 0;			
			}

			//------------------------------------------------
			// 정지된 경우만 움직인다.
			//------------------------------------------------
			if (IsStop())
			{	
				m_Action = action;
				//SetAction( action );

				SetNextAction(ACTION_STAND);

				// Move 첫 동작
				m_MoveCount = 0;
				m_NextMoveCount = 0;
				
				// 정지  :  m_sX = 0; m_sY = 0;
				// 움직이는 종류에 따라서..
				//m_MoveCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( action );
				m_MoveCountMax = GetCreatureActionCountMax( this, action );
					//g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
					//									[action][m_Direction].GetSize();		
			
				//---------------------------------------------
				// UI에서 gear의 item 이동을 못하게 한다. 
				//---------------------------------------------
				//gC_vs_ui.LockItem();
				gC_vs_ui.LockGear();				
			}
			//------------------------------------------------
			// 움직이고 있는 도중이면 Buffering한다.
			//------------------------------------------------
			else
			{
				//---------------------------------------------
				// UI에서 gear item 이동을 할 수 있도록 한다.
				//---------------------------------------------
				//gC_vs_ui.UnlockItem();
				gC_vs_ui.UnlockGear();

				// buffering
				SetNextAction(action);
			}
		}
		//------------------------------------------------
		// Action인 경우
		//------------------------------------------------
		else
		{
			//------------------------------------------------
			// extreme은 특정 동작에서 풀린다.
			//------------------------------------------------
//			if (HasEffectStatus(EFFECTSTATUS_EXTREME))
//			{
//				if (action==ACTION_MAGIC
//					|| action==ACTION_DIE
//					|| action==ACTION_VAMPIRE_DRAIN
//					|| action==ACTION_DRAINED)
//				{
//					RemoveEffectStatus(EFFECTSTATUS_EXTREME);
//				}
//			}

			if (// 오토바이 타고 있을때는 action 안 보여준다.
				m_MoveDevice==MOVE_DEVICE_RIDE && action!=ACTION_SLAYER_MOTOR_STAND
				// damaged인 경우..
				|| action==ACTION_DAMAGED 						
						&& (// 공격받는 중이 아니거나 정지 동작이 아니면 공격 받는 모습 표현 안 한다
							m_Action!=ACTION_DAMAGED && m_Action!=ACTION_STAND && m_Action!=ACTION_MOVE

							// 흡혈 당하는 동작 중이거나 
	//						m_Action==ACTION_DRAINED
							// 뱀파이어일때, 흡혈하는 동작 중에는 damaged를 안 보여준다.
	//						|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							)
				)
			{
				SetNextAction(ACTION_STAND);
				return;
			}

			//---------------------------------------------
			// UI에서 gear item 이동을 할 수 있도록 한다.
			//---------------------------------------------
			//gC_vs_ui.UnlockItem();
			if(!IsInCasket())
				gC_vs_ui.UnlockGear();		

			// 동작 설정		
			SetNextAction(ACTION_STAND);

			//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( action );
			m_ActionCountMax = GetCreatureActionCountMax( this, action );

			if (m_ActionCountMax==0)
			{
				// 동작이 없는 경우
				m_Action = ((m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND);
				//SetAction( ((m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND) );
				m_ActionCount = 0;		
				m_RepeatCount = 0;
				//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
				m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );
			}
			else
			{
				//---------------------------------------------
				// action을 반복하는 경우
				//---------------------------------------------
				if (m_nUsedActionInfo!=ACTIONINFO_NULL 
					&& m_bRepeatAction 
					&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame())// m_WeaponSpeed ))
				{
					m_Action = action; 
					//SetAction( action );

					// 반복회수 제한이 걸리면..
					if (m_RepeatCount >= (*g_pActionInfoTable)[m_nUsedActionInfo].GetRepeatLimit())
					{		
						m_RepeatCount = 0;

						// 무조건 첫 동작부터..
						m_ActionCount = 0;
					}
					else
					{
						//---------------------------------------------
						// 계속 반복하는 경우
						//---------------------------------------------
						BOOL bSlayer = IsSlayer();

						if (m_RepeatCount!=0 
							
							&& action!=ACTION_STAND 
							&& (!bSlayer || bSlayer && action!=ACTION_SLAYER_MOTOR_STAND))
						{
							// count 설정..
							//m_ActionCount = (*g_pActionInfoTable)[m_nUsedActionInfo].GetRepeatStartFrame( m_WeaponSpeed );
							m_ActionCount = GetActionInfoRepeatStartFrame( m_nUsedActionInfo );
						}
						//---------------------------------------------
						// 처음 반복을 시작할려는 경우
						//---------------------------------------------
						else
						{
							m_ActionCount = 0;				
						}
					}

					//m_ActionCountMax = (*g_pActionInfoTable)[m_nUsedActionInfo].GetRepeatEndFrame( m_WeaponSpeed );			
					m_ActionCountMax = GetActionInfoRepeatEndFrame( m_nUsedActionInfo );
				}
				//---------------------------------------------
				// 일반적인 경우..
				//---------------------------------------------
				else
				{			
					if (m_ActionCount!=m_ActionCountMax
						&& (action==ACTION_STAND && m_Action==ACTION_STAND
							|| IsSlayer() && action==ACTION_SLAYER_MOTOR_STAND && m_Action==ACTION_SLAYER_MOTOR_STAND
							|| IsOusters() && action==ACTION_OUSTERS_FAST_MOVE_STAND && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND)
							)
					{
						// 같은 정지 동작이면 설정하지 않는다.
						//m_ActionCount = 0;
						//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
					}
					else
					{
						m_Action = action;
						//SetAction( action );

						// count 설정..
						m_ActionCount = 0;
						
							//g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
							//												[m_Action][m_Direction].GetSize();
					}			
				}			
			}
		}		
	}


	// 동작을 시작할때 부가적으로 붙는 Effect 들.
	// 어딘가로 빼야 하는데.-_-
	if( HasEffectStatus( EFFECTSTATUS_REDIANCE ) )
	{
		if( 
			( m_Action == ACTION_SLAYER_SWORD_2 || m_Action == ACTION_SLAYER_SWORD_2_SLOW || m_Action == ACTION_SLAYER_SWORD_2_FAST ) 
			)
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		} 
		else if(
			( m_Action == ACTION_SLAYER_SWORD || m_Action == ACTION_SLAYER_SWORD_FAST || m_Action == ACTION_SLAYER_SWORD_SLOW ) )
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL_ATTACK_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		}				
	}

	//------------------------------
	// 정지한 상태이면
	//------------------------------
	/*
	if (IsStop())
	{	
		m_Action = action; 
		m_NextAction = ACTION_STAND;

		// Creature의 종류에 따라 Action에 대한 frame수가 달라야 하나?
		switch (m_Action)
		{
			case ACTION_MOVE :
				switch (m_MoveDevice)
				{
					case MOVE_DEVICE_NULL :			
						m_ActionCountMax = ACTIONCOUNT_MOVE;
					break;

					case MOVE_DEVICE_MOTOR1 :						
						m_ActionCountMax = ACTIONCOUNT_MOVE_HALF;
					break;
				}
			break;

			default :
				m_ActionCountMax = g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
															[m_Action][m_Direction].GetSize();
		}

		m_ActionCount = 0;
	}
	//------------------------------
	// 움직이고 있는 상태이면
	//------------------------------
	else
	{
		// buffering
		m_NextAction = action;
	}	
	*/
}

//----------------------------------------------------------------------
// Set AddonItem
//----------------------------------------------------------------------
// AddonItem을 장착한다.
//----------------------------------------------------------------------
bool
MPlayer::SetAddonItem( MItem* pItem )
{
	DEBUG_ADD("MPlayer::SetAddonItem");
	
	//-------------------------------------------------
	// 제대로 장착한 경우
	//-------------------------------------------------
	if (MCreatureWear::SetAddonItem( pItem ))
	{
		//-------------------------------------------------
		// 장착한 Item이 
		// 기본공격용 아이템이라면...
		// 기본공격ActionInfo가 바뀌어야 한다.
		//-------------------------------------------------
		if (pItem->IsBasicWeapon())
		{
			//-------------------------------------------------
			// 공격 가능거리 설정
			// 총인 경우만 다르고.. 
			// 나머지는 모두 근접공격이므로 1이다.							
			//-------------------------------------------------
			//if (pItem->IsGunItem())
			//{
				SetBasicAttackDistance( pItem->GetReach() );
			//}
			//else
			//{								
			//	SetBasicAttackDistance( 1 );
			//}

			//-------------------------------------------------
			// Skill Icon 체크
			//-------------------------------------------------
			//(*g_pSkillAvailable).SetAvailableSkills();
		}

		return true;
	}

	return false;	
}

//----------------------------------------------------------------------
// Remove AddonItem
//----------------------------------------------------------------------
// AddonItem을 장착해제한다.
//----------------------------------------------------------------------
bool
MPlayer::RemoveAddonItem( MItem* pItem )
{
	DEBUG_ADD("MCreatureWear::RemoveAddonItem");
	
	//-------------------------------------------------
	// 제대로 해제된 경우
	//-------------------------------------------------
	if (MCreatureWear::RemoveAddonItem( pItem ))
	{
		//-------------------------------------------------
		// 해제한 Item이 
		// 기본공격용 아이템이라면...
		// 기본공격ActionInfo가 맨손 공격으로 바뀌어야 한다.
		//-------------------------------------------------
		if (pItem->IsBasicWeapon())
		{
			// 기본 맨손 공격 동작으로 전환한다.
			SetBasicActionInfo( SKILL_ATTACK_MELEE );


			// 공격 가능거리 설정
			SetBasicAttackDistance( 1 );

			//-------------------------------------------------
			// Skill Icon 체크
			//-------------------------------------------------
			//(*g_pSkillAvailable).SetAvailableSkills();
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Set Next Action
//----------------------------------------------------------------------
// 다음에 할 행동 설정.. 현재 그냥 서있으면(STAND) 바로 action 설정
// 아니면, buffering
//----------------------------------------------------------------------
void	
MPlayer::SetNextAction(BYTE action)
{ 	
	if(HasEffectStatus(EFFECTSTATUS_SLEEP)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
//		return;
		action = ACTION_STAND;
	}

	if(action == ACTION_STAND)
	{
		action = ((m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
		if(IsOusters() && HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
			action = ACTION_OUSTERS_FAST_MOVE_STAND;
	}
	else if(action == ACTION_MOVE)
		if(IsOusters() && HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
			action = ACTION_OUSTERS_FAST_MOVE_MOVE;

	if (m_Action==ACTION_STAND || IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND) 
	{
		//SetAction( action );
		m_ActionCount = m_ActionCountMax;
	}
	//else
	//{
		m_NextAction = action;
	//}
}

//----------------------------------------------------------------------
// Set NextAction To Move
//----------------------------------------------------------------------
// 다음 action에는 이동한다.
//----------------------------------------------------------------------
void	
MPlayer::SetNextActionToMove()
{ 	
	if (m_Action==ACTION_STAND || IsOusters() && m_Action == ACTION_OUSTERS_FAST_MOVE_STAND
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND) 
	{
		//SetAction( action );
		m_ActionCount = m_ActionCountMax;
	}
	//else
	//{
		SetNextAction(m_MoveAction);
	//}
}

//----------------------------------------------------------------------
// Set Alive
//----------------------------------------------------------------------
// 살았다~ 
//----------------------------------------------------------------------
void	
MPlayer::SetAlive()
{
	m_bAlive = true;

	// item 이동 해제
	UI_UnlockItem();
	UI_UnlockGear();
	UI_UnlockItemTrade();

	SetAction( ACTION_STAND );

	//-------------------------------------------------------
	// Effect상태들 제거
	//-------------------------------------------------------
	/*
	if (g_pEffectStatusTable!=NULL)
	{
		for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
		{
			RemoveEffectStatus( (EFFECTSTATUS)e );
		}
	}
	*/

	if (g_pTopView!=NULL)
	{
		g_pTopView->SetFadeEnd();
	}

	if (m_MoveType == CREATURE_FLYING)
	{
		m_Z=(*g_pCreatureTable)[m_CreatureType].FlyingHeight;
	}
}

//----------------------------------------------------------------------
// Set Dead
//----------------------------------------------------------------------
// 죽었다~ 
// ServerPosition으로 가서 죽자..
//----------------------------------------------------------------------
void	
MPlayer::SetDead()
{
	// 살아 있는 경우만 죽인다. -_-;;
	if (m_bAlive)
	{
		if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
		{
			RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
			SetAction( ACTION_STAND );
		}
		if(HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ) )
		{
			RemoveEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) );
			SetAction( ACTION_STAND );
		}
		if(HasEffectStatus(EFFECTSTATUS_GLACIER))
		{ 
			RemoveEffectStatus( EFFECTSTATUS_GLACIER );
			SetAction(ACTION_STAND);
			ClearAttachEffect();
		}
		g_pUIMessageManager->Execute(UI_FINISH_LEVELUP_BUTTON, 0, 0, NULL);
		
		StopDrain();
		StopAbsorb();

		// item 이동 못하게..
		UI_LockItem();
		UI_LockGear();
		UI_LockItemTrade();

		SetStop();	
//		StopCauseCriticalWounds();

		MovePosition( m_ServerX, m_ServerY );

		DEBUG_ADD_FORMAT("PlayerDeadPosition(%d, %d)", m_ServerX, m_ServerY);

		// 정지
		TraceNULL();
		
		// 죽을 때의 ActionInfo
		m_nUsedActionInfo	= (*g_pCreatureTable)[m_CreatureType].DeadActionInfo;
		
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			// 죽을 때의 동작
			SetAction( GetActionInfoAction(m_nUsedActionInfo) );

			// 나한테 하는 동작
			m_NextTraceID			= m_ID;
			m_NextTraceX			= m_X;
			m_NextTraceY			= m_Y;
			m_NextTraceZ			= 0;	

			SetTraceID( m_ID );
			m_TraceX			= m_X;
			m_TraceY			= m_Y;
			m_TraceZ			= 0;	
		}

		// Delay 시간
		m_DeadDelayTime	= g_CurrentTime + g_pClientConfig->DELAY_PLAYER_DEAD;


		//-------------------------------------------------------
		// 보이게 하기
		//-------------------------------------------------------
		SetVisibleSoon();

		// 죽을때 벌떡 서는거 없애기 위해서.. 확인사살.. - -; 2001.7.23
		m_bNextAction = false;
		m_bKeepTraceCreature = false;
		SetNextAction(ACTION_STAND);
		UnSetRepeatAction();

		// HP
		StopRecoveryHP();
		StopRecoveryMP();
		SetStatus( MODIFY_CURRENT_HP, 0 );

		// LockMode 끄기
		UnSetLockMode();

		// 음악을 느리게...
		//g_DXMusic.SetCurrentTempo( g_DXMusic.GetOriginalTempo() * 0.8f );

		m_Z = 0;
		// 확인.. 
		UI_CloseSelectWayPoint();

		// Logout시간 제거
		g_pUserInformation->LogoutTime = 0;	

		SetWaitVerifyNULL();

		g_pEventManager->RemoveEvent(EVENTID_LOGOUT);
		g_pEventManager->RemoveEvent(EVENTID_LOVECHAIN);

		// 죽었다.
		m_bAlive = false;
		m_bInCasket = false;
	
		//-------------------------------------------------------
		// Effect상태들 제거
		//-------------------------------------------------------
		if (g_pEffectStatusTable!=NULL)
		{
			for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
			{
				// 흡혈은 제거하지 않는다.
				if (e!=EFFECTSTATUS_BLOOD_DRAIN && 
					// 2004, 11, 16, sobeit add start - 공성전 공성측 녀석들 상태가 필요해서 삭제 안함..
					e!=EFFECTSTATUS_SIEGE_ATTACKER_1 &&	
					e!=EFFECTSTATUS_SIEGE_ATTACKER_2 &&
					e!=EFFECTSTATUS_SIEGE_ATTACKER_3 &&
					e!=EFFECTSTATUS_SIEGE_ATTACKER_4 &&
					e!=EFFECTSTATUS_SIEGE_ATTACKER_5 &&
					// 2004, 11, 16, sobeit add end
					HasEffectStatus((EFFECTSTATUS)e))
				{
					RemoveEffectStatus( (EFFECTSTATUS)e );
				}
			}
		}
		ClearAttachEffect();

//		g_pTempInformation->SetMode( TempInformation::MODE_NULL );

		//---------------------------------------------------------------
		// 내 정보를 파티에게 보내준다.
		//---------------------------------------------------------------
		SendStatusInfoToParty();	

		// 2004, 5, 7 sobeit add start - 죽었을 때 도움말
		ExecuteHelpEvent(HELP_EVENT_DIE);
		// 2004, 5, 7 sobeit add end
	}
}

//----------------------------------------------------------------------
// Check Repeat Action
//----------------------------------------------------------------------
bool
MPlayer::CheckRepeatAction()
{
	//----------------------------------------------------------------------
	// 반복 action이 안되는 경우
	//----------------------------------------------------------------------
	if (m_bKeepTraceCreature || IsRequestMode() || IsInDarkness())// || m_bTraceCreatureToForceAttack)
	{
		UnSetRepeatAction();
		return false;
	}

	//----------------------------------------------------------------------
	// action의 마지막 동작이거나 정지동작인 경우에 반복 action을 적용한다.
	//----------------------------------------------------------------------
	if (m_ActionCount>=m_ActionCountMax-1 
			|| m_Action==ACTION_STAND 
			|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		static DWORD repeatFrameIncTurn = 0;

		//------------------------------------------------------------
		// 반복 행동이 설정된 경우
		//------------------------------------------------------------
		if (m_bRepeatAction)				
		{
			//------------------------------------------------------------
			// 자신에게 특수 기술
			//------------------------------------------------------------
			if (m_fTraceBuffer & FLAG_TRACE_SELF)
			{
				//SelfSpecialAction();
				m_fNextTrace = m_fTraceBuffer;

				if (repeatFrameIncTurn!=g_CurrentFrame)
				{
					repeatFrameIncTurn = g_CurrentFrame;
					m_RepeatCount ++;
				}

				//CHECK_REPEAT_LIMIT_TO_RETURN_FALSE( m_nUsedActionInfo, m_RepeatCount )

				return true;
			}				
			//------------------------------------------------------------
			// 기본 행동			
			//------------------------------------------------------------
			else if (m_fTraceBuffer & FLAG_TRACE_CREATURE_BASIC)
			{	
				//TraceCreatureToBasicAction( m_TraceID, m_bTraceCreatureToForceAttack );
				m_fNextTrace = m_fTraceBuffer;

				m_NextTraceID  = m_TraceIDBuffer;				
				
				m_bNextForceAttack = m_bTraceCreatureToForceAttack;

				if (repeatFrameIncTurn!=g_CurrentFrame)
				{
					repeatFrameIncTurn = g_CurrentFrame;
					m_RepeatCount ++;
				}

				//CHECK_REPEAT_LIMIT_TO_RETURN_FALSE( m_nUsedActionInfo, m_RepeatCount )

				return true;
			}				
			//------------------------------------------------------------
			// 특수 행동
			//------------------------------------------------------------
			else  if (m_fTraceBuffer & FLAG_TRACE_CREATURE_SPECIAL)
			{
				//TraceCreatureToSpecialAction( m_TraceID );
				m_fNextTrace = m_fTraceBuffer;

				m_NextTraceID = m_TraceIDBuffer;

				m_bNextForceAttack = m_bTraceCreatureToForceAttack;

				if (repeatFrameIncTurn!=g_CurrentFrame)
				{
					repeatFrameIncTurn = g_CurrentFrame;
					m_RepeatCount ++;
				}
				
				//CHECK_REPEAT_LIMIT_TO_RETURN_FALSE( m_nUsedActionInfo, m_RepeatCount )				
				if (m_nUsedActionInfo < g_pActionInfoTable->GetSize())
				{
					// 반복회수 제한에 걸리면..
					if (m_RepeatCount >= (*g_pActionInfoTable)[m_nUsedActionInfo].GetRepeatLimit())
					{		
						// 동작 끝으로 보낸다.
						//m_RepeatCount = 0;

						BOOL bSlayer = IsSlayer();

						if ((*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame() //m_WeaponSpeed )
							&& m_Action!=ACTION_STAND 
							&& (!bSlayer || bSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
							&& m_ActionCountMax!=0 )	// 이걸로 될려나.. - -;
						{	
							//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
							m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );

							DEBUG_ADD_FORMAT("unsetrepeat: actionCount=%d / %d", m_ActionCount, m_ActionCountMax);		
						}						
					}												
				}
				
				return true;
			}
			//------------------------------------------------------------
			// 특수 행동 -> Sector
			//------------------------------------------------------------
			else if (m_fTraceBuffer & FLAG_TRACE_SECTOR_SPECIAL)
			{
				//TraceSectorToSpecialAction( m_TraceX, m_TraceY );
				m_fNextTrace = m_fTraceBuffer;

				//m_NextTraceX = m_TraceX;
				//m_NextTraceY = m_TraceY;

				//------------------------------------------------------------
				// 현재의 mouse 좌표를 읽어서 map의 좌표로 바꾼다.
				//------------------------------------------------------------
				POINT temp = g_pTopView->ScreenToPixel(g_x, g_y);
				temp = MTopView::PixelToMap( temp.x, temp.y );
				
				m_NextTraceX = temp.x;
				m_NextTraceY = temp.y;

				if (repeatFrameIncTurn!=g_CurrentFrame)
				{
					repeatFrameIncTurn = g_CurrentFrame;
					m_RepeatCount ++;
				}
				
				//CHECK_REPEAT_LIMIT_TO_RETURN_FALSE( m_nUsedActionInfo, m_RepeatCount )

				return true;
			}		
		}
		//------------------------------------------------------------
		// 반복 행동이 설정 안 된 경우
		//------------------------------------------------------------
		else
		{
			//------------------------------------------------------------
			// buffering된 다음 행동이 있다면..
			//------------------------------------------------------------
			if (m_fNextTrace!=FLAG_TRACE_NULL)
			{
				//m_fTraceBuffer = m_fNextTrace;
				return true;
			}			
		}
	}
	else
	{
	}

	return false;
}

//----------------------------------------------------------------------
// Check BufferAction
//----------------------------------------------------------------------
// 다음에 처리해야할 buffering된 action이 있는지를 알아본다.
// 있을 경우는 return true
// 없을 경우는 return false
//----------------------------------------------------------------------
bool
MPlayer::CheckBufferAction()
{
	//DEBUG_ADD("Before CheckBufferAction");
	
	//------------------------------------------		
	//     추적해야 하는 경우..		
	//------------------------------------------
	if (m_fNextTrace != FLAG_TRACE_NULL)
	{			
		//SetAction( (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );

		//---------------------
		// 자신에게 특수 기술
		//---------------------
		if (m_fNextTrace & FLAG_TRACE_SELF)
		{
			SelfSpecialAction();
		}				
		//---------------------
		// 기본 행동			
		//---------------------
		if (m_fNextTrace & FLAG_TRACE_CREATURE_BASIC)
		{
			TraceCreatureToBasicAction( m_NextTraceID, m_bNextForceAttack );
		}				
		//---------------------
		// 특수 행동
		//---------------------
		else  if (m_fNextTrace & FLAG_TRACE_CREATURE_SPECIAL)
		{
			TraceCreatureToSpecialAction( m_NextTraceID, m_bNextForceAttack );
		}
		//---------------------
		// 기본 행동 -> Sector
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_SECTOR_BASIC)
		{
			TraceSectorToBasicAction( m_NextTraceX, m_NextTraceY );
		}
		//---------------------
		// 특수 행동 -> Sector
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_SECTOR_SPECIAL)
		{
			TraceSectorToSpecialAction( m_NextTraceX, m_NextTraceY );
		}
		//---------------------
		// Item 추적
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_ITEM)
		{
			TraceItem( m_NextTraceID );
		}
		//---------------------
		// Inventory의 Item 에 기술 사용
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_INVENTORY)
		{
			TraceInventoryItem( m_NextTraceID );
		}
		//---------------------
		// Effect추적
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_EFFECT)
		{
			TraceEffect( m_NextTraceID );
		}
		//---------------------
		// InteractionObject추적
		//---------------------
		else if (m_fNextTrace & FLAG_TRACE_INTERACTIONOBJECT)
		{
			TraceInteractionObject( m_NextTraceID, m_NextTraceObjectAction );
		}

		//DEBUG_ADD("After CheckBufferAction : true");
		
		return true;
	}

	//------------------------------------------		
	//     Buffering된 다음 행동
	//------------------------------------------
	BOOL bSlayer = IsSlayer();

	if (m_bNextAction || 
		m_NextAction!=ACTION_STAND 
		&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)// && m_fNextTrace==FLAG_TRACE_NULL)
		&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))// && m_fNextTrace==FLAG_TRACE_NULL)
	{
		SetAction( m_NextAction );	

		DEBUG_ADD("After CheckBufferAction : true - SetAction");
		
		return true;
	}	

	//DEBUG_ADD("After CheckBufferAction - false");
	
	return false;
}

//----------------------------------------------------------------------
// UnSet Repeat Action
//----------------------------------------------------------------------
void			
MPlayer::UnSetRepeatAction()				
{ 
	// 반복이 중지되면 다음 buffering행동도 없애야 될까?

	m_bRepeatAction = FALSE; 

	m_RepeatCount = 0;

	m_nNextUsedActionInfo = ACTIONINFO_NULL;

	BOOL bSlayer = IsSlayer();

	//m_fTraceBuffer = FLAG_TRACE_NULL;
	/*
	#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
			{
				if (m_nUsedActionInfo != ACTIONINFO_NULL)
				{
					DEBUG_ADD_FORMAT("UnSetRepeatAction:ai=%d, ir=%d, at=%d, sm=%d, acm=%d",
			
					(int)m_nUsedActionInfo, 
					(int)(*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame(),
					(int)m_Action!=ACTION_STAND,
					(int)(!bSlayer || bSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND),
					(int)m_ActionCountMax!=0);
				}
				else
				{
					DEBUG_ADD("UnSetRepeatAction: m_nUsedActionInfo is ACTIONINFO_NULL");
				}
			}
	#endif
	*/

	if (m_nUsedActionInfo!=ACTIONINFO_NULL 
		&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame() //m_WeaponSpeed )
		&& m_Action!=ACTION_STAND 
		&& (!bSlayer || bSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
		&& m_ActionCountMax!=0 )	// 이걸로 될려나.. - -;
	{	
		//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );		
		m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );

		DEBUG_ADD_FORMAT("unsetrepeat: actionCount=%d / %d", m_ActionCount, m_ActionCountMax);		
	}

	//m_fNextTrace = FLAG_TRACE_NULL; 
}

//----------------------------------------------------------------------
// Add EffectStatus
//----------------------------------------------------------------------
bool		
MPlayer::AddEffectStatus(EFFECTSTATUS status, DWORD delayFrame)
{	
	__BEGIN_HELP_EVENT
		switch (status)
		{		
		case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
		case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
		case EFFECTSTATUS_SUMMON_SYLPH_RED :
		case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
		case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
		case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
		case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
		case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
		case EFFECTSTATUS_SUMMON_SYLPH:
			SetMoveDevice(MOVE_DEVICE_SUMMON_SYLPH);
			break;

			//--------------------------------------------------
			// Bless
			//--------------------------------------------------
			case EFFECTSTATUS_BLESS :
				// [도움말] striking
//				ExecuteHelpEvent( HE_EFFECT_BLESS );
			break;

			//--------------------------------------------------
			// Striking
			//--------------------------------------------------
			case EFFECTSTATUS_STRIKING :
				// [도움말] striking
//				ExecuteHelpEvent( HE_EFFECT_STRIKING );
			break;

			//--------------------------------------------------
			// Green Poison
			//--------------------------------------------------
			case EFFECTSTATUS_GREEN_POISON :
			case EFFECTSTATUS_POISON :
				// [도움말] green poison
//				ExecuteHelpEvent( HE_EFFECT_GREEN_POISON );
			break;

			//--------------------------------------------------
			// Purple Wall
			//--------------------------------------------------
			//case EFFECTSTATUS_PURPLE_WALL :
			case EFFECTSTATUS_CURSE_PARALYSIS :
				// [도움말] purple wall
//				ExecuteHelpEvent( HE_EFFECT_PURPLE_WALL );
			break;

			//--------------------------------------------------
			// Yellow Poison
			//--------------------------------------------------
			case EFFECTSTATUS_YELLOW_POISON :
			case EFFECTSTATUS_YELLOW_POISON_TO_CREATURE :
				// [도움말] yellow poison
//				ExecuteHelpEvent( HE_EFFECT_YELLOW_POISON );
			break;
		
			//--------------------------------------------------
			// BLOOD DRAIN 
			//--------------------------------------------------
			case EFFECTSTATUS_BLOOD_DRAIN :
			{
				// 2004, 5, 6 sobeit add start [도움말] 흡혈당했을때
				ExecuteHelpEvent( HELP_EVENT_DRAIN_BLOOD );				
				// 2004, 5, 6 sobeit add end
			}
			break;		

			//--------------------------------------------------
			// OBSERVING_EYE [새기술6]
			//--------------------------------------------------
			case EFFECTSTATUS_OBSERVING_EYE :
				// 아직 안 걸려있는 경우
				if (!m_bEffectStatus[EFFECTSTATUS_OBSERVING_EYE])
				{
					CalculateLightSight();
				}
			break;

			case EFFECTSTATUS_GHOST:
				SetFlyingCreature();
//				g_pUserInformation->bCompetence = true;
			break;

		}
	__END_HELP_EVENT

	//--------------------------------------------------
	// 뱀파이어로 변하는 effect이면.. 시간 설정을 해둔다.
	//--------------------------------------------------
	switch (status)
	{
		//--------------------------------------------------
		// 흡혈
		//--------------------------------------------------
		case EFFECTSTATUS_BLOOD_DRAIN :
		{
			DWORD conversionTime = delayFrame * 1000 / 16;// / g_pClientConfig->FPS;

			SetConversionDelay( conversionTime );

			DEBUG_ADD_FORMAT( "[BloodDrained] %ld --> %ld", g_CurrentTime, m_ConversionDelayTime );

//			delayFrame *= 16;
//			UI_SetHP( GetHP(), GetMAX_HP() );
		}
		break;

		//--------------------------------------------------
		// hallu
		//--------------------------------------------------
		case EFFECTSTATUS_HALLUCINATION :
			g_pZone->ChangeToHalluCreature();			
		break;

		case EFFECTSTATUS_LIGHTNESS :
		case EFFECTSTATUS_YELLOW_POISON :
		case EFFECTSTATUS_FLARE :
		case EFFECTSTATUS_YELLOW_POISON_TO_CREATURE :
//		case EFFECTSTATUS_BLINDNESS:
			CalculateSight();
			break;
		case EFFECTSTATUS_CAN_ENTER_GDR_LAIR:
			delayFrame = delayFrame*10;
			break;
		case EFFECTSTATUS_INSTALL_TURRET:
//			g_pSkillAvailable->SetAvailableSkills();
			SetDelay(1000);
			gC_vs_ui.UnselectSkill();
			//gC_vs_ui.SelectSkill( SKILL_TURRET_FIRE );
			break;

	}

	bool re = MCreature::AddEffectStatus( status, delayFrame );
	//--------------------------------------------------
	// UI에 알린다. 왜 이걸 re로 판단하면 삑살나지-_-; 2002.5.5 쑥갓
	//--------------------------------------------------
	if(!(IsSlayer() && status == EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS))
		UI_AddEffectStatus( status, delayFrame );
	
	if(status == EFFECTSTATUS_INSTALL_TURRET)
	{
		g_pSkillAvailable->SetAvailableSkills();
		gC_vs_ui.SelectSkill( SKILL_TURRET_FIRE );
	}
	CheckRegen();

	return re; 
}

//----------------------------------------------------------------------
// Remove EffectStatus
//----------------------------------------------------------------------
bool		
MPlayer::RemoveEffectStatus(EFFECTSTATUS status)
{
	bool bOK = MCreatureWear::RemoveEffectStatus( status );

	//--------------------------------------------------
	// UI에 알린다.
	//--------------------------------------------------
	UI_RemoveEffectStatus( status );

	switch (status)
	{		
	case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
	case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
	case EFFECTSTATUS_SUMMON_SYLPH_RED :
	case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
	case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
	case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
	case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
	case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
	case EFFECTSTATUS_SUMMON_SYLPH:
		SetMoveDevice(MOVE_DEVICE_WALK);
		if(GetWaitVerify() == MPlayer::WAIT_VERIFY_SYLPH_SUMMON_GETOFF)
			SetWaitVerifyNULL();
		
		/*  add by sonic 錦攣침쥣랗瘻빈賈痰루쑹쥣꼇콘긴쀼覩近BUG 2006.10.4 */
		if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
		{
				m_bEffectStatus[status]=true;
				bool re = MCreature::RemoveEffectStatus( status );
		}
		break;
		/*   end    */
		
		//--------------------------------------------------
		// hallu
		//--------------------------------------------------
		case EFFECTSTATUS_HALLUCINATION :
			g_pZone->RemoveHalluCreature();			
		break;

		//--------------------------------------------------
		// 뱀파이어로 변하는 effect이면.. 시간 설정 제거
		//--------------------------------------------------
		case EFFECTSTATUS_BLOOD_DRAIN :
			UnSetConversionDelay();

			UI_SetHP( GetHP(), GetMAX_HP() );		
		break;

		//--------------------------------------------------
		// 죽다가 살아난 경우
		//--------------------------------------------------		
		case EFFECTSTATUS_COMA :
			// 검증 제거
			if (g_pTempInformation->GetMode()==TempInformation::MODE_WAIT_RESURRECT)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}

			if (GetWaitVerify()==WAIT_VERIFY_RESURRECT)
			{
				SetWaitVerifyNULL();
			}

			gC_vs_ui.FinishRequestDie();
			gC_vs_ui.FinishRequestResurrect();
			gC_vs_ui.CloseRequestShrineMinimap();
		break;

		//--------------------------------------------------
		// OBSERVING_EYE [새기술6]
		//--------------------------------------------------
		case EFFECTSTATUS_OBSERVING_EYE :
			CalculateStatus();
		break;

		case EFFECTSTATUS_GHOST:
			if (!(*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 박쥐인 경우
				SetGroundCreature();
			break;
		case EFFECTSTATUS_WILL_OF_LIFE :
			if(IsVampire() )
				MCreature::CheckRegen();
			break;
		case EFFECTSTATUS_FLARE :
		case EFFECTSTATUS_YELLOW_POISON :
		case EFFECTSTATUS_YELLOW_POISON_TO_CREATURE :
		case EFFECTSTATUS_LIGHTNESS :
			// 2004, 6, 21 sobeit add start - about blindness
//		case EFFECTSTATUS_BLINDNESS:
			// 2004, 6, 21 sobeit add end - about blindness
			if( bOK )
				CalculateSight();
			break;
//		case EFFECTSTATUS_GLACIER:
//			if (m_bAlive)
//			{
//				SetStop();
//				StopCauseCriticalWounds();
//			}			
//		break;
		case EFFECTSTATUS_INSTALL_TURRET:
			g_pSkillAvailable->SetAvailableSkills();
			ExecuteActionInfoFromMainNode(RESULT_SKILL_INSTALL_TURRET,GetX(), GetY(), 0,0,GetID()	,	
				GetX(), GetY(), 0, 0, NULL, false);
			SetDelay(1000);
			break;
	}
	
	return bOK;
}

//----------------------------------------------------------------------
// Set CreatureType
//----------------------------------------------------------------------
void	
MPlayer::SetCreatureType(TYPE_CREATURETYPE type)
{
	//add by viva 
//	type = 808; 
	MCreatureWear::SetCreatureType(type);

	//---------------------------------------------------
	// Skill Icon을 다시 체크한다.
	//---------------------------------------------------
	if (g_pSkillAvailable!=NULL)
	{
		g_pSkillAvailable->SetAvailableSkills();		
	}	
	if(type==CREATURETYPE_SLAYER_OPERATOR
		|| type==CREATURETYPE_VAMPIRE_OPERATOR
		|| type==CREATURETYPE_OUSTERS_OPERATOR)
		g_pUserInformation->bCompetenceShape = true;
	else
		g_pUserInformation->bCompetenceShape = false;
}

//----------------------------------------------------------------------
// UpdateConversionTime
//----------------------------------------------------------------------
void
MPlayer::UpdateConversionTime()
{
	//--------------------------------------------------------
	// 뱀파이어로 변하는 Effect체크
	// 코드 마구 추가하기.. 카카...- -;
	//--------------------------------------------------------

	// 전쟁 남은 시간 체크
	
	if (IsSlayer())
	{
		if ((int)m_ConversionDelayTime > (int)g_CurrentTime)
		{
			//--------------------------------------------------------
			// 뱀파이어로 변하는 남은 시간을 UI에 설정한다.
			//--------------------------------------------------------
			// msec --> min으로 바꾼다.   * GetTimeRatio / 1000 / 60 
			int changeTime = ((int)m_ConversionDelayTime - (int)g_CurrentTime)/* * g_pGameTime->GetTimeRatio()*/ / 60000 ;
			g_char_slot_ingame.CHANGE_VAMPIRE = changeTime;

			static DWORD enableBlinkTime = 0;	// 깜빡거려도 되는 시간
			
			//const DWORD sixHour = 6*60*60*1000;

			// { 남은시간, 깜빡이는 시간 }
			const int maxBlink = 12;
			const DWORD blinkValue[maxBlink][2] = 
			{
				{ 4*60*60*1000, 6*60*1000 },	// 4시간 이상 --> 5분마다			
				{ 3*60*60*1000, 4*60*1000 },	// 3시간 이상 --> 4분마다			
				{ 2*60*60*1000, 3*60*1000 },	// 2시간 이상 --> 3분마다			
				{ 1*60*60*1000, 2*60*1000 },	// 1시간 이상 --> 2분마다
				{ 1*60*60*1000, 1*60*1000 },	// 10분 이상 --> 1분
				{ 3*60*1000, 30*1000 },			// 3분 이상 --> 30초
				{ 2*60*1000, 10*1000 },			// 2분 이상 --> 10초
				{ 1*60*1000, 5*1000 },			// 1분 이상 --> 5초
				{ 30*60*1000, 3*1000 },			// 30초 이상 --> 3초
				{ 10*1000, 2*1000 },			// 10초 이상 --> 2초
				{ 10*1000, 2*1000 },			// 5초 이상 --> 1초
				{ 0*1000, 300 }					// 0초 이상 --> 0.3초
			};

			//--------------------------------------------------------
			// 마지막에 깜빡이고 나서.. 
			// 다시 깜빡일 수 있는 시간인지 체크한다.
			//--------------------------------------------------------
			if (g_CurrentTime > enableBlinkTime)
			{
				// 변하기까지 남은 시간.. /1000하면 '초'로 나온다.
				DWORD timeGap = m_ConversionDelayTime - g_CurrentTime;
				
				// 무조건 출력. 6시간이 안 남은 경우에만 effect출력
				if (1)//timeGap < sixHour)
				{
					BOOL bBlink = FALSE;

					for (int i=0; i<maxBlink; i++)
					{
						if (timeGap > blinkValue[i][0])
						{
							g_pTopView->SetFadeStart(25, 31, 2, 31,0,0);

							// 다음에 깜빡일 수 있는 시간을 설정한다.
							enableBlinkTime = g_CurrentTime + blinkValue[i][1];

							bBlink = TRUE;

							break;
						}
					}

					if (!bBlink)
					{
						UnSetConversionDelay();
					}
				}
			}
		}
		else
		{
			m_ConversionDelayTime = 0;

			//--------------------------------------------------------
			// 뱀파이어로 변하는 남은 시간을 UI에 설정한다.
			//--------------------------------------------------------
			g_char_slot_ingame.CHANGE_VAMPIRE = 0;
		}
	}
}

//----------------------------------------------------------------------
// 다음 동작을 취한다.
//----------------------------------------------------------------------
void
MPlayer::Action()
{	
	if( !HasEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL ) && !HasEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL ) && GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}

	// 펫이 있고 펫 동작한지 1초 넘었을때
	if( GetPetID() != OBJECTID_NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetPetID());
		if(pFakeCreature != NULL)
		{
			MPetItem *pPetItem = pFakeCreature->GetPetItem();
			if(pPetItem != NULL)
			{
				const int petLevelMax = 50;
				int petLevel = pPetItem->GetNumber();
				int petDelayTime = 0;	
				TYPE_ITEMTYPE tmpItemType = pPetItem->GetItemType();
				// add by svi 2009-07-01 藤속劤녘膠穀庫돨醵똑
				if(tmpItemType == 6 || tmpItemType == 7 || tmpItemType == 8)
				{
					petDelayTime = 1200 + ((petLevelMax-petLevel)*120)/petLevelMax;
				}
				// end
				else if(pPetItem->GetItemType()>2)// 2 차 펫이면
					petDelayTime = 3200+((petLevelMax-petLevel)*2800)/petLevelMax;
				else
					petDelayTime = 1200+((petLevelMax-petLevel)*2800)/petLevelMax;

				if(m_PetDelayTime+petDelayTime < GetTickCount() )
				{
					int itemLimit = 1;	// 머리는 빼고

					if(pPetItem->IsCanCutHead())	// 머리따기 기능이 있으면 머리 포함해서
						itemLimit = 0;

					MItem *pItem = g_pZone->GetCorpseKilledByMe(itemLimit);

					if(pItem != NULL)
					{
						CGDissectionCorpse _CGDissectionCorpse;
						_CGDissectionCorpse.setX( pItem->GetX() );
						_CGDissectionCorpse.setY( pItem->GetY() );
						_CGDissectionCorpse.setPet( true );
						_CGDissectionCorpse.setObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGDissectionCorpse );

						// 한꺼번에 따진다 limit로 세팅해주쟈
						DEBUG_ADD_FORMAT("[PETPET] itemLimit %d", itemLimit);
						pItem->SetNumber( itemLimit );
					}
					
					m_PetDelayTime = GetTickCount();
				}
			}
		}
	}

	// 너무 오래 반복 시키지 않게
	if( m_nUsedActionInfo == SKILL_BLITZ_SLIDING_ATTACK || m_nUsedActionInfo == SKILL_BLAZE_WALK_ATTACK )
		UnSetRepeatAction();

	if(IsRepeatAction())
	{
		if(m_RepeatTimer + g_pClientConfig->REPEAT_TIME < GetTickCount())
			UnSetRepeatAction();
	}
	// 너무 오래 반복 시키지 않게
	if(IsLockMode())
	{
		if(m_LockTimer + g_pClientConfig->LOCK_TIME < GetTickCount())
			UnSetLockMode();
	}
	//if (m_ActionCount==m_ActionCountMax)
	//{
	//	int a =0;
	//}
	
	//----------------------------------------------------------------------
	// 파티원들에게 나의 상태정보를 보낸다.
	//----------------------------------------------------------------------
	SendStatusInfoToParty();
	
#ifdef OUTPUT_DEBUG_PLAYER_ACTION
	DEBUG_ADD_FORMAT("[MPLayer::Action] Action[%d]=%d/%d. Move=%d/%d"
					, m_Action, m_ActionCount, m_ActionCountMax, m_MoveCount, m_MoveCountMax);	
#endif
	
#ifdef __METROTECH_TEST__
	if(!IsVampire())
	{
		if (g_iAutoHealPotion == 1 && m_RecoveryHPTimes==0)	AutoUsePotion(0);		// HP
		if (g_bAutoManaPotion && m_RecoveryMPTimes==0)	AutoUsePotion(1);		// MP
		if (g_iAutoReload)	AutoUsePotion(2+g_iAutoReload-1);					// Magazine
//		if (g_iAutoHealPotion == 2 || g_iAutoHealPotion == 3)  AutoUseSkill(0);									// Healing
//		if (g_bHolyWater)  AutoUseSkill(1);									// CreateHolyWater
	}
#endif
	/*
	// [ TEST CODE ]
	// EffectTarget에 결과 추가하기 Test
	if (m_listEffectTarget.size()!=0 && m_ActionCount==m_ActionCountMax)
	{
		MActionResult* pResult = new MActionResult;
		pResult->Add( new MActionResultNodeActionInfo( 1, 1001, GUIDANCE_FIRE ) );
		
		// 결과와 관련된 Effect가 없는 경우..
		EFFECTTARGET_LIST::iterator iEffectTarget = m_listEffectTarget.begin();
		MEffectTarget* pEffectTarget = *iEffectTarget;
		
		BYTE id = pEffectTarget->GetEffectID();

		if (!AddActionResult(id, pResult))
		{
			// 결과를 바로 실행하고..
			// 메모리에서 지운다.
			delete pResult;
		}
		
	}
	*/

	#ifdef OUTPUT_DEBUG
		if (m_ActionCount > m_ActionCountMax)
		{
			DEBUG_ADD("So Terrible!");
		}
	#endif

	//--------------------------------------------------------
	// 도움말 --;
	//--------------------------------------------------------
//	__BEGIN_HELP_EVENT
//		static DWORD nextHelpTime = g_CurrentTime+20*1000;	// 20초 뒤 처음으로 메세지 출력
//		if (g_CurrentTime > nextHelpTime)
//		{
//			ExecuteHelpEvent( HE_TIME_PASSED );
//
//			nextHelpTime = g_CurrentTime + 20*1000;	// 20초
//		}
//	__END_HELP_EVENT

	//--------------------------------------------------------
	// Status 상태 변화	- HP, MP 변화
	//--------------------------------------------------------
	UpdateStatus();	
	
	//--------------------------------------------------------
	// 뱀파이어로 변하는 Effect체크
	//--------------------------------------------------------
	UpdateConversionTime();
	
	//--------------------------------------------------------
	// 채팅 String Scroll 시킬 시간
	//--------------------------------------------------------
	if (m_NextChatFadeTime < g_CurrentTime)
	{
		FadeChatString();
	}

	//--------------------------------------------------------
	// 특수한 효과들
	//--------------------------------------------------------
	if(UpDateInstallTurret())
		return;
	UpdateInvisible();
	UpdateCasket();
	UpdateCutHeight();
	UpdateTurning();
	UpdateCauseCriticalWounds();
//	UpdateBloodyZenith();
	UpdateGunShotGuidance();
	UpdateFakeDie();
	UpdateGhost();

	UpdateDarkness();

	UpdateSweepVice();
	UpdateBurningSol();		

	UpdateBikeCrash();
	//--------------------------------------------------------
	// 방향 전환을 자연스럽게
	//--------------------------------------------------------
	ChangeNearDirection();
	//m_CurrentDirection = m_Direction;

	//--------------------------------------------------------
	// 기술에 따른 결과 표현 체크
	//--------------------------------------------------------
	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("beforeCheckListEffect");
	#endif
	
	if (m_nUsedActionInfo < g_pActionInfoTable->GetMinResultActionInfo())
	{		
		int numTarget = m_listEffectTarget.size();

		if (numTarget!=0)
		{
			//------------------------------------------------------------
			// 뭔가 기술을 사용하고 있다면 
			// 그 기술의 결과 시점에서 결과를 보여주게 할려고...
			//------------------------------------------------------------
			BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND);
			//int		actionCountMax_1 = m_ActionCountMax-1;
			//BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
			int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

			BOOL	bStartResult	= bActionStand 
										|| (m_ActionCount >= StartFrame)
										|| m_ActionCount >= m_ActionCountMax;

			bStartResult |= m_bRepeatAction 
							&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
							&& m_RepeatCount!=0
							&& m_ActionCount >= GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

			// 결과를 전부 실행해버린다.
			if (bStartResult)
			{
				EFFECTTARGET_LIST::iterator iEffectTarget = m_listEffectTarget.begin();
		
				while (iEffectTarget != m_listEffectTarget.end())
				{
					MEffectTarget* pEffectTarget = *iEffectTarget;

					if (pEffectTarget->IsResultTime())// || pEffectTarget->IsEnd())
					{
						MActionResult* pResult = pEffectTarget->GetResult();

						if (pResult!=NULL)
						{
							// 실행한 결과는 없앤다.
							int numTargetBeforeExecute = m_listEffectTarget.size();

							pEffectTarget->SetResultNULL();

							pResult->Execute();

							delete pResult;

							int numTargetAfterExecute = m_listEffectTarget.size();
							
							// pResult->Execute()에서 m_listEffectTarget이 제거될 수가 있다 - -;
							// 콩가루 구조... 수정이 필요하다 - -;;
							if (numTargetBeforeExecute!=numTargetAfterExecute)
							{
								// 처음부터 다시 체크
								iEffectTarget = m_listEffectTarget.begin();
								continue;
							}

							// delete는 외부에서 한다...
							// 아마(-_-;;) Effect에 붙여서 Effect 제거될때 지우는 듯..
							// 그러나.. 별로 기억이 안나는데.. ㅡ.ㅡ;;
							// 일단 지우면 다운되는데. 안 지우면 다운안되는걸보면.. 맞는듯.. - -;;
							//delete pEffectTarget;
						}
					}

					iEffectTarget++;
				}		

				m_listEffectTarget.clear();
			}
		}
	}

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("bcleok");
	#endif

	//--------------------------------------------------------
	// 기술 사용후의 delay가 모두 끝난 상태이면...
	//--------------------------------------------------------	
	if (!m_bTurning	// [새기술]
		&& !HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
		&& !IsCauseCriticalWounds()
//		&& !IsBloodyZenith()
		&& !IsGunShotGuidance()
		&& !HasEffectStatus(EFFECTSTATUS_SOUL_CHAIN)
		&& !HasEffectStatus(EFFECTSTATUS_TRAP_TRIGGERED)
		&& !HasEffectStatus(EFFECTSTATUS_TRAPPED) 
//		&& g_pUserInformation->LogoutTime == 0
		|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		|| g_bLight
#endif
		)// && !HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS))	//IsNotDelay())
	{
		//--------------------------------------------------------
		// 다음 buffering된 actioninfo
		//--------------------------------------------------------
		/*
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL
			&& m_nUsedActionInfo==ACTIONINFO_NULL)
		{
			m_nUsedActionInfo = m_nNextUsedActionInfo;
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
		}
		*/
		//--------------------------------------------------------
		// 무슨 effect가 걸려있다면 2배 느리게 움직인다.
		//--------------------------------------------------------
		if (HasEffectStatus( EFFECTSTATUS_HAS_SLAYER_RELIC )       || HasEffectStatus( EFFECTSTATUS_HAS_VAMPIRE_RELIC ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI )|| HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA )  || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_INI )    || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO )   || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL ) ||
			HasEffectStatus( EFFECTSTATUS_AMBER_OF_GUARD ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_AVENGER ) ||
			HasEffectStatus( EFFECTSTATUS_AMBER_OF_IMMORTAL ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_CURSE ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_6 ) ||
			IsCreatureMove( dynamic_cast<MCreature*>(this) ) && HasEffectStatus( EFFECTSTATUS_ICE_FIELD_TO_CREATURE ) ||
			IsCreatureMove( dynamic_cast<MCreature*>(this) ) && HasEffectStatus( EFFECTSTATUS_FROZEN_ARMOR_TO_ENEMY ) ||
			IsCreatureActionAttack( this ) && (	HasEffectStatus( EFFECTSTATUS_ICE_OF_SOUL_STONE ) || (IsInFuryOfGnome() && !IsOusters()/*(IsSlayer() || IsVampire())*/)	) || 
			IsCreatureMove( dynamic_cast<MCreature*>(this) ) && HasEffectStatus( EFFECTSTATUS_JABBING_VEIN ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_2 ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_3 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_4 ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_6 ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_7 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_8 ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_9 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_10 ) ||
			HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_11 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_12 )
			&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )			
#ifdef __METROTECH_TEST__
			&& !g_bLight
#endif
			)			
		{
			if (g_CurrentFrame & 0x01)
			{
				return;
			}
		}

		//--------------------------------------------------------
		// KnockBack 처리 2001.10.9
		//--------------------------------------------------------
		if (m_bKnockBack > 0)
		{
			m_sX += m_cX;
			m_sY += m_cY;

			// knockBack 다x 됐으면 정지동작.
			if (--m_bKnockBack==0)
			{
				m_sX = 0;
				m_sY = 0;

				SetAction( ACTION_STAND );

				return;
			}			
		}
		//--------------------------------------------------------
		// FastMove중에는 action을 취하지 않는다. 2001.8.10
		//--------------------------------------------------------
		else if (!m_bFastMove)
		{
			//--------------------------------------------------------
			// Lock Mode 사용
			//--------------------------------------------------------
			if (m_bLockMode && !IsInDarkness())
			{
				int creatureID = FindEnemy();

				//--------------------------------------------------------
				// 사정거리 이내에 적이 없는 경우
				//--------------------------------------------------------
				if (creatureID == OBJECTID_NULL)
				{
					m_listDirection.clear();

					UnSetRepeatAction();
					TraceNULL();
					m_fTraceBuffer	= FLAG_TRACE_NULL;	
					m_fNextTrace	= FLAG_TRACE_NULL;

					// 길찾기 했던걸 없애준다.
					m_DestX		= SECTORPOSITION_NULL;
					m_DestY		= SECTORPOSITION_NULL;
					m_NextDestX	= SECTORPOSITION_NULL;
					m_NextDestY	= SECTORPOSITION_NULL;

					g_pTopView->SetSelectedNULL();
				}
				//--------------------------------------------------------
				// 적이 있는 경우
				//--------------------------------------------------------
				else
				{
					int bBasicAction = g_bLButtonDown;
					int bSpecialAction = g_bRButtonDown;

					//---------------------------------------------------------------
					// 기본 행동
					//-------------------------------------------------------------
					if (bBasicAction)
					{
						if (TraceCreatureToBasicAction( 
									creatureID, 
									true ))		// 강제 공격
						{
							//----------------------------------
							// 반복 action인 경우만 반복 설정
							//----------------------------------
							//if (m_nBasicActionInfo!=ACTIONINFO_NULL 
							//	&& (*g_pActionInfoTable)[m_nBasicActionInfo].IsUseRepeatFrame()
							//	&& !IsRepeatAction())
							//{
							//	SetRepeatAction();
							//}

							g_pTopView->SetSelectedCreature( creatureID );			
							gpC_mouse_pointer->SetCursorAttack();					

							//g_bPreviousMove = false;
						}
					}
					//-------------------------------------------------------------
					// 특수 행동
					//-------------------------------------------------------------
					else if (bSpecialAction)
					{
						if (TraceCreatureToSpecialAction( creatureID, true ))
						{
							//----------------------------------
							// 반복 action인 경우만 반복 설정
							//----------------------------------
							if (m_nSpecialActionInfo!=ACTIONINFO_NULL 
								&& (*g_pActionInfoTable)[m_nSpecialActionInfo].IsUseRepeatFrame()
								&& !IsRepeatAction()
								)
							{
								SetRepeatAction();
							}							
							
							g_pTopView->SetSelectedCreature( creatureID );			
							gpC_mouse_pointer->SetCursorAttack();					

							//g_bPreviousMove = false;
						}
					}					

					// 목적지 표시를 없앤다.
					g_pTopView->SetSelectedSectorNULL();
				}
				
			}

			//--------------------------------------------------------
			// 피 흘리는거 체크
			//--------------------------------------------------------
			if (m_bAlive)
			{
				CheckDropBlood();
			}

			// delayTime은 없는 걸로 setting한다.
			//m_DelayTime	= 0;
			//----------------------------------------------------
			// 계속 추적하는 경우
			// 음.. 지저분한 코드가 늘어난다... - -;;
			//----------------------------------------------------
			if (m_bKeepTraceCreature && (m_fTrace & FLAG_TRACE_CREATURE_BASIC))
			{							
				KeepTraceCreature();		
			}
			
			//------------------------------------------		
			//     Buffering된 다음 행동
			//------------------------------------------
			BOOL bSlayer = IsSlayer();
			if (m_bNextAction ||
				(!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND )
				&& m_NextAction!=ACTION_STAND 
				&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND)
				&& m_NextAction!=m_MoveAction)
			{
				SetAction( m_NextAction );
				
				#ifdef OUTPUT_DEBUG
					if (m_ActionCount > m_ActionCountMax)
					{
						DEBUG_ADD("So Terrible! return1");
					}
				#endif

				return;
			}

			//------------------------------------------
			// [ TEST CODE ]
			//------------------------------------------
			// 움직이는 도중에..
			// 추적하는 경우 추적목표거리가 되는 경우에는..
			// 바로 이번에.. action을 수행하게 해야한다.
			// 그래서, return false
			if (m_NextAction==m_MoveAction &&
				m_fTrace != FLAG_TRACE_NULL &&
				(max(abs(m_X-m_TraceX), abs(m_Y-m_TraceY))) <= m_TraceDistance)
			{
				SetAction( m_NextAction );
			}

			//------------------------------------------
			// Action 수행.. 다음 Frame으로...
			//------------------------------------------
			if (m_ActionCount < m_ActionCountMax)
			{		
				if (m_nUsedActionInfo!=ACTIONINFO_NULL)
				{
					ActionEffect();
				}
				else
				{
					//m_ActionCount--;

					// 적절한 Action 수행
					//switch (m_Action)
					//

					BOOL bSlayer = IsSlayer();

						// 정지 동작
					if (m_Action==ACTION_STAND || IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_MOVE
						|| bSlayer && m_Action==ACTION_SLAYER_MOTOR_STAND)
					{
												
						m_ActionCount++;						

						//------------------------------------------------------------
						// 반복 행동 체크..
						//------------------------------------------------------------
						if (CheckRepeatAction())
						{
							if (CheckBufferAction())
							{
								if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
								{
									// 아.. 헷갈려라.. - -;
									m_nUsedActionInfo = m_nNextUsedActionInfo;
									m_nNextUsedActionInfo = ACTIONINFO_NULL;							

									if (ActionInTraceDistance())
									{
										if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
										{
											// 아.. 헷갈려라.. - -;
											m_nUsedActionInfo = m_nNextUsedActionInfo;
											m_nNextUsedActionInfo = ACTIONINFO_NULL;
										}
										else
										{
											// 더 이상 갈 필요가 없다.
											m_listDirection.clear();

											// 이동 중지
											m_MoveCount = m_MoveCountMax;

											// Player가 바라보는 방향이 현재 적이 있는 방향이 되도록 한다.				
											SetDirectionToPosition( m_TraceX, m_TraceY );

											SetAction( m_NextAction );

											//ActionToSendPacket();
										}
									}
								}
								#ifdef OUTPUT_DEBUG
									if (m_ActionCount > m_ActionCountMax)
									{
										DEBUG_ADD("So Terrible! return2");
									}
								#endif

								return;
							}
						}					

						/*
						else if (m_Action==ACTION_MOVE 
								|| bSlayer && ACTION_SLAYER_MOTOR_MOVE)
						{
							// Move는 따로 처리한다.
							//ActionMove();					
						}
						*/					
					}
					else
					{
					//default :
						if (m_nUsedActionInfo==ACTIONINFO_NULL)
						{	
							m_ActionCount++;

							if (m_bStopBloodDrain)
							{
								StopBloodDrain();
							}							
							if (m_bStopAbsorbSoul)
							{
								StopAbsorbSoul();
							}							
						}
						else
						{
							ActionEffect();
						}
					}
				}
			}
		}
		


		//------------------------------------------
		// Action이 끝난 상태이면
		//------------------------------------------
		if (m_bAlive)
		{
			// 부등호 추가 2001.10.7
			if (m_ActionCount>=m_ActionCountMax) //if (IsStop())
			{
				//--------------------------------------------------------
				// 목표 타일에 도착한 상태에서..
				// 계속 서 있거나.. 다 걸은 경우는.. 
				// 제자리에서 흔들거리는 모습을 표현해준다.
				//--------------------------------------------------------
				// 부등호 추가 2001.10.7
				if (m_MoveCount>=m_MoveCountMax)
				{					
					//if (m_bRepeatAction && m_RepeatCount!=0 && (*g_pActionInfoTable)[m_nUsedActionInfo].IsRepeatFrame())
					//{						
					//	int a =0;
					//}
					//------------------------------------------------------------
					// 반복 행동 체크..
					//------------------------------------------------------------
					//if (
						bool bCheckRepeat = CheckRepeatAction();
						//)
					//{
					//}

					if ((!m_bRepeatAction || !bCheckRepeat)
						&& m_fNextTrace != FLAG_TRACE_NULL)
					{			
						if (m_ActionCount>=m_ActionCountMax)
						{
							SetAction( ACTION_STAND );
						}
					}

					if (CheckBufferAction()) 
					{
						/*
						if (m_NextAction==m_MoveAction &&
							//m_fTrace != FLAG_TRACE_NULL &&
							(max(abs(m_X-m_TraceX), abs(m_Y-m_TraceY))) <= m_TraceDistance)
						{
							SetAction( m_NextAction );
						}
						*/
						if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
						{
							// 아.. 헷갈려라.. - -;
							m_nUsedActionInfo = m_nNextUsedActionInfo;
							m_nNextUsedActionInfo = ACTIONINFO_NULL;							

							if (ActionInTraceDistance())
							{
								if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
								{
									// 아.. 헷갈려라.. - -;
									m_nUsedActionInfo = m_nNextUsedActionInfo;
									m_nNextUsedActionInfo = ACTIONINFO_NULL;
								}
								else
								{
									// 더 이상 갈 필요가 없다.
									m_listDirection.clear();

									// 이동 중지
									m_MoveCount = m_MoveCountMax;

									// Player가 바라보는 방향이 현재 적이 있는 방향이 되도록 한다.				
									SetDirectionToPosition( m_TraceX, m_TraceY );

									SetAction( m_NextAction );

									//ActionToSendPacket();
								}
							}							
						}

						#ifdef OUTPUT_DEBUG
							if (m_ActionCount > m_ActionCountMax)
							{
								DEBUG_ADD("So Terrible! return3");
							}
						#endif

						return;
					}

					//------------------------------------------
					// 제자리에서 흔들거리는 모습
					//------------------------------------------
					if (m_bRepeatAction)// && m_RepeatCount!=0)// && (*g_pActionInfoTable)[m_nUsedActionInfo].IsRepeatFrame())
					{						
					}
					else
					{
						SetAction( ACTION_STAND );
					}
					
				}
				//--------------------------------------------------------
				// 걷다가 다른 action을 보여줬거나 계속 걷던 중이다.
				// 다시 걷는다.
				//--------------------------------------------------------					
				else
				{			
					m_Action = m_MoveAction;
					ActionMove();

					//----------------------------------------------------
					// 이동이 모두 끝났으면..
					// 제자리에서 흔들거리는 모습 
					//----------------------------------------------------
					if (m_MoveCount==m_MoveCountMax)
					{		
						if (m_fNextTrace != FLAG_TRACE_NULL)
						{		
							if (m_ActionCount>=m_ActionCountMax)
							{
								SetAction( ACTION_STAND );
							}
						}

						if (CheckBufferAction()) 
						{
							if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
							{
								// 아.. 헷갈려라.. - -;
								m_nUsedActionInfo = m_nNextUsedActionInfo;
								m_nNextUsedActionInfo = ACTIONINFO_NULL;							

								if (ActionInTraceDistance())
								{
									if (m_nNextUsedActionInfo != ACTIONINFO_NULL)
									{
										// 아.. 헷갈려라.. - -;
										m_nUsedActionInfo = m_nNextUsedActionInfo;
										m_nNextUsedActionInfo = ACTIONINFO_NULL;
									}
									else
									{
										// 더 이상 갈 필요가 없다.
										m_listDirection.clear();

										// 이동 중지
										m_MoveCount = m_MoveCountMax;

										// Player가 바라보는 방향이 현재 적이 있는 방향이 되도록 한다.				
										SetDirectionToPosition( m_TraceX, m_TraceY );

										SetAction( m_NextAction );

										//ActionToSendPacket();
									}
								}
							}

							#ifdef OUTPUT_DEBUG
								if (m_ActionCount > m_ActionCountMax)
								{
									DEBUG_ADD("So Terrible! return4");
								}
							#endif

							return;
						}

						//------------------------------------------
						// 제자리에서 흔들거리는 모습
						//------------------------------------------				
						if (m_ActionCount>=m_ActionCountMax)
						{
							SetAction( ACTION_STAND );
						}
					}
				}			
			}	
		}
	}

	
	//--------------------------------------------------------
	// Delay가 있는 경우...  
	// 살아 있는 경우에만 --> 정지동작을 보여준다.
	//--------------------------------------------------------
	/*
	else if (m_bAlive)
	{
		if (m_ActionCount < m_ActionCountMax)
		{	
			if (m_Action==ACTION_STAND 
				|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
			{
				m_ActionCount++;
			}		

			// action이 끝났으면..
			if (m_ActionCount==m_ActionCountMax) 
			{
				// 다시 정지 동작
				SetAction( (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );
			}		hflh
		}
		else
		{
			SetAction( (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );
		}
	}
	*/
	
}

//----------------------------------------------------------------------
// Add EffectTarget
//----------------------------------------------------------------------
// 진행중인 EffectTarget을 추가한다.
//----------------------------------------------------------------------
void
MPlayer::AddEffectTarget(MEffectTarget* pEffectTarget)
{
	// 기존에꺼 다 지워버린다. ㅋㅋ
	m_listEffectTarget.clear();

	// 새거 하나 추가
	m_listEffectTarget.push_back( pEffectTarget );
}

//----------------------------------------------------------------------
// Remove EffectTarget
//----------------------------------------------------------------------
// 진행중이던 Effect가 끝난 경우...
// 그와 관련있는 Target을 제거한다.
//----------------------------------------------------------------------
void
MPlayer::RemoveEffectTarget(BYTE id)
{
	EFFECTTARGET_LIST::iterator iEffectTarget = m_listEffectTarget.begin();

	// id가 같은 경우를 찾아서 제거한다.
	// list이기 때문에.. 다 비교해야 하지만...
	// 실제로 Player가 사용중인 기술이 몇개~ 밖에 안되기 때문에...
	while (iEffectTarget != m_listEffectTarget.end())
	{
		// id가 같은 경우
		if ((*iEffectTarget)->GetEffectID() == id)
		{
			// 메모리에서 제거는 외부에서 한다.
			//delete (*iEffectTarget);

			// list에서 제거한다.			
			m_listEffectTarget.erase( iEffectTarget );

			break;
		}

		iEffectTarget++;
	}
}

//----------------------------------------------------------------------
// Packet Back
//----------------------------------------------------------------------
// 이동 중이면..
// 바로 전의 칸으로 Back한다.
// 공격 당했을때의 처리이다.
// 해킹의 위험이 있지 않을까.. 흠흠.. - -;;
//----------------------------------------------------------------------
/*
void	
MPlayer::PacketBack()
{
	// 정지된 상태이면 필요가 없다.
	if (m_sX==0 && m_sY==0)
		return;

	// 반대방향.. 	
	//m_sX = -m_sX;
	//m_sY = -m_sY;
	//m_cX = -m_cX;
	//m_cY = -m_cY;
	//m_ActionCount = m_ActionCountMax - m_ActionCount;
	

	// 정지된 상태가 아니면.. 정지 시킨다.
	SetStop();

	// 바로 전의 위치는...
	// 현재 위치에서 진행방향의 반대방향으로 되돌아 가면 된다.
	m_Direction = GetCounterDirection( m_Direction );

	// 방향으로 움직인 좌표를 얻는다.
	GetPositionToDirection(m_X, m_Y, m_Direction);

	// Back이동을 했다가 Server로 보낸다.
	#ifdef	CONNECT_SERVER
		CGMove _CGMove;
		_CGMove.setX( m_X );
		_CGMove.setY( m_Y );
		_CGMove.setDir( m_Direction );
		g_pSocket->sendPacket( &_CGMove );

		#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
				sprintf(g_pDebugMessage->GetCurrent(), "[Send] [Back]%s", _CGMove.toString().c_str());
				g_pDebugMessage->Next();
		#endif

		// Server로 보낸 방향을 기억해둔다.
		m_listSendDirection.push_back( m_Direction );
		m_SendMove++;
		
	#endif
}
*/

//----------------------------------------------------------------------
// Packet MoveOK
//----------------------------------------------------------------------
// Server에서 MOVE_OK를 받은 경우
//
// 현재위치에서 direction방향인 (x,y) sector로 이동가능하다는 의미이다.
//----------------------------------------------------------------------
void
MPlayer::PacketMoveOK(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE direction)
{
	// 2004, 9, 16, sobeit add start - 터렛 방향 바꾸기, 뺄까..ㅡㅡ;
//	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
//	{
//		SetCurrentDirection(direction);
//		return;
//	}
	// 2004, 9, 16, sobeit add end
	// 2004, 9, 21, sobeit add start - 아담 북 구름
	m_bShowAdamCloud = false;
	if(NULL != g_pZone && g_pZone->GetID() == 74)
	{
		if(sY<60)
			m_bShowAdamCloud = true;
	}
	// 2004, 9, 21, sobeit add end
	//--------------------------------------------------------
	// 기억해둔 것 중에서..
	// 검증받은 방향을 제거한다.
	//--------------------------------------------------------
	if (!m_listSendDirection.empty())
	{
		m_listSendDirection.pop_front();
	}

	//--------------------------------------------------------
	// MoveError다음에 MoveOK가 바로 날아오는 경우
	//--------------------------------------------------------
	if (m_SendMove==0)
	{
		//--------------------------------------------------------
		// MoveError다음에 바로 MoveOK가 날아오는 경우가 있었다.
		// 이 경우.. (아마 - -;) 좌표가 삐끗~해서 
		// 계속 다음부터 좌표 검증을 못받는거 같다.
		// 그래서.. 임시(?) 대책으로... Server에서 검증된 좌표로 바로 이동시킨다.
		//--------------------------------------------------------
		MovePosition( sX, sY );

		// 이동을 중지한다.
		m_sX = 0;	
		m_sY = 0;

		m_MoveCount		= m_MoveCountMax;
		m_ActionCount	= m_ActionCountMax; 

		// 길찾기 제거
		m_listDirection.empty();			

		//----------------------------------------------------
		// Creature를 계속 추적하던 중이면..
		//----------------------------------------------------
		if (m_bKeepTraceCreature						// 계속 추적하는 경우
			&& (m_fTrace & FLAG_TRACE_CREATURE_BASIC)	// creature를 추적하는 경우
			&& !m_bTraceCreatureToForceAttack)			// 그냥 따라가는 경우
		{							
			KeepTraceCreature();		
		}
		//----------------------------------------------------
		// 보통 이동시에 중지된 경우
		//----------------------------------------------------
		else
		{		
			TraceNULL();	

			// 길찾기 했던걸 없애준다.
			m_DestX		= SECTORPOSITION_NULL;
			m_DestY		= SECTORPOSITION_NULL;
			m_NextDestX	= SECTORPOSITION_NULL;
			m_NextDestY	= SECTORPOSITION_NULL;
		}
	}
	//--------------------------------------------------------
	// 정상적인 MoveOK인 경우
	//--------------------------------------------------------
	else if (m_SendMove>0)
	{
		m_SendMove--;		
	}

	g_bNetStatusGood = true;

	//--------------------------------------------------------
	// Server에서 검증된 위치를 기억해둔다.
	//--------------------------------------------------------
	//POINT nextPoint;
	//MCreature::GetNextPosition(direction, nextPoint);
//	m_ServerX	= nextPoint.x;
//	m_ServerY	= nextPoint.y;

	m_ServerX	= sX;
	m_ServerY	= sY;


	//--------------------------------------------------------
	// (m_ServerX, m_ServerY)에서부터 
	// Player의 시야에 포함되지 않는 Creature를 Zone에서 제거시킨다.
	//--------------------------------------------------------
	if (m_pZone!=NULL)
	{
		m_pZone->KeepObjectInSight(m_ServerX, m_ServerY, m_Sight);
	}

	SendPositionInfoToParty();

	return;
}

//----------------------------------------------------------------------
// Packet MoveNO
//----------------------------------------------------------------------
// Server에서 MOVE_NO를 받은 경우
//----------------------------------------------------------------------
void	
MPlayer::PacketMoveNO()	
{
	//m_listSendDirection.front();
	if (!m_listSendDirection.empty())
	{
		m_listSendDirection.pop_front();
	}

	m_SendMove = 0;

	//------------------------------------------------------------
	// 현재 위치로 올 수 있는 길이 있는지 살펴본다.
	//
	// 길이 없는 경우이거나 길이 넘 긴 경우?는 
	//		최근의 검증받은 위치(ServerX,Y)로 이동한다.
	//
	// 길이 있다면, 찾은 위치까지의 이동 packet을 보낸다.
	//------------------------------------------------------------
	//if (!SetDestination( m_X, m_Y ) || m_listDirection.empty()	// 길이 없는 경우
		//|| m_SendMove > g_pClientConfig->MAX_CLIENT_MOVE)		// 길이 넘 긴 경우
	{
		// Zone에서 현재 있는 Sector에서 제거하고 
		// Server에서 검증된 최근의 Sector로 이동한다.
		MovePosition( m_ServerX, m_ServerY );
		

		// 이동을 중지한다.
		m_sX = 0;	
		m_sY = 0;
		
		m_MoveCount		= m_MoveCountMax;
		//m_ActionCount	= m_ActionCountMax; 

		//----------------------------------------------------
		// Creature를 계속 추적하던 중이면..
		//----------------------------------------------------
		if (m_bKeepTraceCreature						// 계속 추적하는 경우
			&& (m_fTrace & FLAG_TRACE_CREATURE_BASIC)	// creature를 추적하는 경우
			&& !m_bTraceCreatureToForceAttack			// 그냥 따라가는 경우
			&& m_ActionCount>=m_ActionCountMax)			// 특별한 동작없이 움직이고 있던 경우
		{							
			KeepTraceCreature();		
		}
		//----------------------------------------------------
		// 보통 이동시에 중지된 경우
		//----------------------------------------------------
		else
		{		
			// 길찾기 했던걸 없애준다.
			m_listDirection.empty();

			/*
			// 다 주석처리하면 왠지 될거같다... - -;  2001.8.1		
			m_DestX		= SECTORPOSITION_NULL;
			m_DestY		= SECTORPOSITION_NULL;
			m_NextDestX	= SECTORPOSITION_NULL;
			m_NextDestY	= SECTORPOSITION_NULL;

			TraceNULL();	

			// 2001.7.27 추가
			UnSetRepeatAction();	
			*/
		}

		// 2001.8.22
		if (m_Action==m_MoveAction)
		{
			// 이동 중이면 그냥 정지
			/*
			SetStop();

			m_DestX		= SECTORPOSITION_NULL;
			m_DestY		= SECTORPOSITION_NULL;
			m_NextDestX	= SECTORPOSITION_NULL;
			m_NextDestY	= SECTORPOSITION_NULL;

			TraceNULL();	

			UnSetRepeatAction();			
			*/
			// 길찾기 제거
			m_listDirection.clear(); 

			// 추적 중지	
			//m_fTrace		= FLAG_TRACE_NULL;
			//m_fNextTrace	= FLAG_TRACE_NULL;

			// Action 중지
			//m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
			//m_ActionCount	= m_ActionCountMax; 	
			m_MoveCount		= m_MoveCountMax-1; 
			m_NextMoveCount = m_MoveCount;

			// 다음 동작도 없앰
			m_bNextAction = false;
			m_NextX = SECTORPOSITION_NULL;
			m_NextY = SECTORPOSITION_NULL;
			
			//m_nNextUsedActionInfo = ACTIONINFO_NULL;
			//m_nUsedActionInfo = ACTIONINFO_NULL;

			
			// 목표 제거
			m_DestX			= SECTORPOSITION_NULL; 
			m_DestY			= SECTORPOSITION_NULL; 
			m_NextDestX		= SECTORPOSITION_NULL; 
			m_NextDestY		= SECTORPOSITION_NULL; 
		}
		// 2001.8.6
		// 임시 대책.. - 움직이다가 부딪혀셔 튕기는 경우..
		else if (m_ActionCount>=m_ActionCountMax)
		{
			if (m_Action==m_MoveAction)
			{
				/*
				SetStop();

				m_DestX		= SECTORPOSITION_NULL;
				m_DestY		= SECTORPOSITION_NULL;
				m_NextDestX	= SECTORPOSITION_NULL;
				m_NextDestY	= SECTORPOSITION_NULL;

				TraceNULL();	

				UnSetRepeatAction();
				*/
				m_sX = 0;//-m_cX;
				m_sY = 0;//-m_cY;

				m_MoveCount = m_MoveCountMax - 1;
				m_NextMoveCount = m_MoveCount;
			}
		}

		//m_SendMove = 0;
		// 여기서 뭔가 SX, SY값이 이상하게 되는데~~!!!!!!!!!!!!!!!!
	}
	/*
	// 한칸 차이로 대체로 막혀버리기 때문에
	// 별로 의미가 없어서 주석처리해뒀다.
	// 그리고.. 테스트도 한번도 못해본 코드이다. - -;
	else
	{
		DEBUG_ADD_FORMAT("[MoveNo] Find New Way. num=%d", m_listDirection.size());

		//----------------------------------------------------	
		//
		// (m_ServerX, m_ServerY)에서 
		//	(m_X, m_Y)까지 방향대로 움직인다.
		//
		//----------------------------------------------------
		int x = m_ServerX;
		int y = m_ServerY;

		while (!m_listDirection.empty())
		{
			//----------------------------------------------------
			// m_listDirection에서 방향을 하나 읽어온다.
			//----------------------------------------------------
			int direction = m_listDirection.front();
			m_listDirection.pop_front();	
		
			switch (direction)
			{
				case DIRECTION_LEFTDOWN		: x--;	y++;	break;
				case DIRECTION_RIGHTUP		: x++;	y--;	break;
				case DIRECTION_LEFTUP		: x--;	y--;	break;
				case DIRECTION_RIGHTDOWN	: x++;	y++;	break;
				case DIRECTION_LEFT			: x--;			break;
				case DIRECTION_DOWN			:		y++;	break;
				case DIRECTION_UP			:		y--;	break;
				case DIRECTION_RIGHT		: x++;			break;
			}

			//----------------------------------------------------
			// Server에서 검증을 받아야 한다.
			//----------------------------------------------------
			#ifdef	CONNECT_SERVER
				CGMove _CGMove;
				_CGMove.setX( x );
				_CGMove.setY( y );
				_CGMove.setDir( direction );
				g_pSocket->sendPacket( &_CGMove );
				
				// Server로 보낸 방향을 기억해둔다.
				m_listSendDirection.push_back( direction );
				m_SendMove++;				
			#endif
		}
	}
	*/
}


//----------------------------------------------------------------------
// Packet SpecialAction Result
//----------------------------------------------------------------------
// this Creature는 Damage를 받았으므로
// 이동 중이면 방금 전의 Tile로 Back시켜야 한다.
//
// 0. 모든 동작을 중지하고..
// 1. Action --> Damage받는 동작
// 2. Damage 표시
//----------------------------------------------------------------------
void		
MPlayer::PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive)
		return;

	//----------------------------------------------------------------------
	// 결과 action이 아닌 경우..  - 의미가 있나?? 흠..
	//----------------------------------------------------------------------
	if (nResultActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo()
		// fast move중에는 action을 보여주지 않는다... // 2001.8.10
		|| m_bFastMove || m_bKnockBack)
	{
		return;
	}

	//----------------------------------------------------------------------
	// 방어막 효과.. - -;
	//----------------------------------------------------------------------
	int actionInfoAction = GetActionInfoAction(nResultActionInfo);
	
	if (actionInfoAction==ACTION_DAMAGED)
	{
		//if (HasEffectStatus(EFFECTSTATUS_AURA_PRISM))
		{
		//	nResultActionInfo = REFLECT_AURA_PRISM;
		//	actionInfoAction = GetActionInfoAction(nResultActionInfo);
		}
		//else 
			if (HasEffectStatus(EFFECTSTATUS_AURA_SHIELD))
		{
			nResultActionInfo = REFLECT_AURA_PRISM;//REFLECT_AURA_SHIELD;
			actionInfoAction = GetActionInfoAction(nResultActionInfo);
		}

		//-------------------------------------------------
		// Force Feel
		//-------------------------------------------------
//		if (gpC_Imm!=NULL && gpC_Imm->IsDevice())
//		{
//			gpC_Imm->ForceAction( CImm::FORCE_ACTION_DAMAGED );
//		}
	}

	//-------------------------------------------------
	// 반복 action 때문에 기억해둔다.
	//-------------------------------------------------
	int oldTraceID = m_TraceID;

	//----------------------------------------------------------------------
	// 기존에 사용할려는 것이 있었다면...
	//----------------------------------------------------------------------
	///*
	m_nNextUsedActionInfo = ACTIONINFO_NULL;
	TraceNextNULL();

	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		BOOL	bStartActionInfo = m_nUsedActionInfo < g_pActionInfoTable->GetMinResultActionInfo();
		BOOL	bSentPacket		= (m_ActionCount>0);
		int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

		// Effect가 시작되는 경우는..
		// (1) StartFrame인 경우
		// (2) 마지막 ActionFrame인 경우
		BOOL	bStartedEffect = m_ActionCount>=StartFrame;

		//--------------------------------------------------
		// 이미 effect가 붙은게 아니라면 붙인다.
		//--------------------------------------------------		
		if (!bStartActionInfo						// 결과 패킷이거나..
			|| (bSentPacket && !bStartedEffect))	// 패킷을 보냈는데 Effect가 안 붙은 경우..
		{
			TYPE_ACTIONINFO oldUsedActionInfo = m_nUsedActionInfo;

			m_nUsedActionInfo = ACTIONINFO_NULL;

			AffectUsedActionInfo(oldUsedActionInfo);
			
			// AffectUsedActionInfo에 의해서 생긴 결과 표현..
			if (m_nUsedActionInfo != ACTIONINFO_NULL)
			{
				AttachCastingEffect( m_nUsedActionInfo, TRUE );
				AffectUsedActionInfo(m_nUsedActionInfo);
			}
		}			
		//--------------------------------------------------		
		// 바로 이전의 Effect를 출력 안 하는 경우.
		//--------------------------------------------------		
		else
		{
			m_nUsedActionInfo = ACTIONINFO_NULL;			
		}
		
		//-------------------------------------------------------------
		// castingEffect가 아직 출력 안됐으면 출력시킨다.
		//-------------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL
			&& GetActionInfoCastingStartFrame(m_nUsedActionInfo) >= m_ActionCount)
		{
			AttachCastingEffect( m_nUsedActionInfo, TRUE );		
		}
	
	}
	//*/

	// 이동을 중지한다.
	//m_sX = 0;	
	//m_sY = 0;
	// Back시킨다.	
	////SetStop();
	//PacketBack();		
	
	//m_ActionCount = m_ActionCountMax; 

	// 추적 중지
	//-------------------------------------------------
	// 2000.09.22 에 m_fTrace만 NULL로 바꿈..
	//-------------------------------------------------
	//TraceNULL();
	m_fTrace = FLAG_TRACE_NULL;


	SetTraceID( id );
	m_TraceX	= sX;
	m_TraceY	= sY;

	// 길찾기 했던걸 없애준다.
	m_listDirection.empty();
	m_DestX		= m_X;//SECTORPOSITION_NULL;
	m_DestY		= m_Y;//SECTORPOSITION_NULL;
	m_NextDestX	= SECTORPOSITION_NULL;
	m_NextDestY	= SECTORPOSITION_NULL;

	// nResultActionInfo에 해당하는 ActionInfo를 찾아야 한다.
	// 원래ActionInfo + MIN_RESULT_ACTIONINFO를 하면 된다.
//	m_nUsedActionInfo	= nResultActionInfo;	// + (*g_pActionInfoTable).GetMinResultActionInfo()
	//SetAction( (*g_pActionInfoTable)[m_nUsedActionInfo].GetAction() );

	// 이렇게 해버리면..
	// 현재 진행하는 Tile까지 가서.. 공격을 받기 때문에
	// 한 칸 떨어져서 공격받는 경우가 발생한다.
	// 어떻게 해야할까??
	//m_NextAction = (*g_pActionInfoTable)[m_nUsedActionInfo].GetAction();
	
	//SetNextAction( GetActionInfoAction(m_nUsedActionInfo) );
	//m_bNextAction = true;

	// 2001.05.21 추가
	// 기술 동작에서 ACTION_STAND는 보여주지 않는다.
	if (actionInfoAction!=ACTION_STAND)
	{
		SetAction( actionInfoAction );
	}

	if( temp != 0)
		SetActionGrade( temp );

	// 바로 적용
	//-------------------------------------------------------------
	//
	// Casting effect
	//
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	// Casting effect
	//-------------------------------------------------------------
	AttachCastingEffect( nResultActionInfo, TRUE );		
	AffectUsedActionInfo( nResultActionInfo );


	//-------------------------------------------------
	// 반복 action 때문에...
	//-------------------------------------------------
	SetTraceID( oldTraceID );

	// 자신에게 사용?..
	//-------------------------------------------------
	// 2000.09.22 에 주석 처리.. -_-;
	//-------------------------------------------------
	//m_TraceID = m_ID;

	// Test Code
	/*
	if (rand()%2)
		SetChatString( "Shit!" );
	else
		SetChatString( "Oops!" );
	*/
}

//----------------------------------------------------------------------
// Packet Add ActionResult
//----------------------------------------------------------------------
// Player가 사용한 Action과 관련된 결과를 Server로 부터 받아서
// 외부에서 ActionResult로 생성한 것을 
// Player의 ActionResultList에 추가한다.
//
// 즉, InstanceID가 id인 MEffectTarget의 Result를 설정한다.
//----------------------------------------------------------------------
// return값이 
//     true이면 추가되었다는 의미
//     false이면 관련된 EffectTarget이 없으므로 
//               결과를 바로 실행해줘야 한다.
//----------------------------------------------------------------------
bool		
MPlayer::PacketAddActionResult(WORD effectID, MActionResult* pActionResult)
{
	/*
	MActionResult* pResult = new MActionResult;
	pResult->Add( new MActionResultNodeActionInfo( m_ID, m_TraceID, m_nUsedActionInfo ) );
		
	pEffectTarget->SetResult( pResult );
	*/
	
	// 처리할 결과가 없는 경우
	if (pActionResult==NULL)
	{
		return false;
	}


	if (m_nUsedActionInfo < g_pActionInfoTable->GetMinResultActionInfo())
	{
		//------------------------------------------------------------
		// 진행중인 Effect의 EffectTarget이 있을 경우에
		//------------------------------------------------------------
		if (m_listEffectTarget.size()!=0)
		{
			// 마지막 EffectTarget의 결과를 확인해본다..
			MEffectTarget* pEffectTarget = m_listEffectTarget.back();

			// 이미 결과가 있는 경우라면..
			MActionResult* pResult = pEffectTarget->GetResult();

			if (pResult!=NULL)
			{
				pEffectTarget->SetResultNULL();
			}

			// 새로운 결과를 대입해준다.
			pEffectTarget->SetResult( pActionResult );

			if (pResult!=NULL)
			{
				// 기존의 결과를 실행시켜 버린다.
				pResult->Execute();

				delete pResult;
			}			

			return true;

			/*
			EFFECTTARGET_LIST::iterator iEffectTarget = m_listEffectTarget.begin();

			while (iEffectTarget != m_listEffectTarget.end())
			{
				//------------------------------------------------------------
				// id가 같은 경우
				//------------------------------------------------------------
				// EffectID를 체크해야하지만,
				// 현재(!)는 .. 기술 하나 쓰고 하나 검증받기 때문에
				// EffectID를 체크 안해도 된다.
				//------------------------------------------------------------
				// 무조건 이번 행동에 맞춰서 결과를 표현해줄 예정이므로
				// 결과를 넣어둔다.
				//if ((*iEffectTarget)->GetEffectID() == effectID)
				{
					// EffectTarget의 결과로 추가시킨다.
					// 이미 결과가 있는 경우..(?)
					// 그 전의 결과를 실행시켜버린다.
					if ((*iEffectTarget)->IsExistResult())
					{
						(*iEffectTarget)->GetResult()->Execute();
						//break; // 원래는.. - -;
					}

					// 새로운걸 set하면 기존에건 delete된다.
					(*iEffectTarget)->SetResult( pActionResult );

					// 제대로 설정 된 경우
					return true;
				}

				iEffectTarget++;
			}
			*/
		}
	}

	//------------------------------------------------------------
	// 진행중인 Effect의 EffectTarget이 없을 경우에
	//------------------------------------------------------------			
	// 결과를 바로 실행하고..
	// 메모리에서 지운다.
	// (!) 결과 실행을 위한 Manager class가 필요하다.
	pActionResult->Execute();

	delete pActionResult;
	
	return false;	
}

//----------------------------------------------------------------------
// On Attacking
//----------------------------------------------------------------------
// 반복 행동 중에 공격하는 경우..
//----------------------------------------------------------------------
bool			
MPlayer::OnAttacking() const				
{ 
	return m_bRepeatAction && m_bTraceCreatureToForceAttack; 
}

//----------------------------------------------------------------------
// Basic Action To Creature
//----------------------------------------------------------------------
// 추적을 완료한 경우.. Creature에게 기본action을 취한다.
//----------------------------------------------------------------------
void
MPlayer::BasicActionToCreature()
{
	if (IsInDarkness())
	{
		return;
	}

	MCreature* pCreature = g_pZone->GetCreature( m_TraceID );

	if (pCreature==NULL)// || pCreature->IsDead())
	{
		return;
	}
	if(pCreature->CurPernalShop() == 1) // by csm 12/4
		return;

	//---------------------------------------------------------------
	// 대상이 NPC인 경우..
	//---------------------------------------------------------------
	if (pCreature->IsNPC())
	{
		// 박쥐나 늑대가 아닌 경우.. 거래 가능하다.
		if (//m_CreatureType!=CREATURETYPE_BAT
			//&& m_CreatureType!=CREATURETYPE_WOLF)
			(*g_pCreatureSpriteTable)[(*g_pCreatureTable)[m_CreatureType].SpriteTypes[0]].IsPlayerOnlySprite())
		{		
			if(pCreature->GetCreatureType() == 659)	// 대지정령의 뿔이면 UI띄운다
				UI_RunHorn(g_pZone->GetID());
			else
				
				if (IsWaitVerifyNULL())
				{
					// NPC 처리 packet을 보낸다.
					CGNPCTalk _CGNPCTalk;
					_CGNPCTalk.setObjectID( m_TraceID );

					g_pSocket->sendPacket( &_CGNPCTalk );

					SetWaitVerify( WAIT_VERIFY_NPC_ASK );
				}
		}

		// 필요하려나.. - -;
		m_bTraceCreatureToForceAttack = false;
		m_bKeepTraceCreature = false;

		// 정지한다.
		SetNextAction(ACTION_STAND);
		m_nUsedActionInfo = ACTIONINFO_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
		m_fNextTrace = FLAG_TRACE_NULL;
		UnSetRepeatAction();
	}
	//---------------------------------------------------------------
	// Request할려는 경우
	//---------------------------------------------------------------
	else if (IsRequestMode())
	{
		switch (m_RequestMode)
		{
			//---------------------------------------------------------------
			// Trade 할려는 경우
			//---------------------------------------------------------------
			case REQUEST_TRADE :
			{
				if (IsWaitVerifyNULL() 
					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
					&& (IsSlayer() && pCreature->IsSlayer() || 
					IsVampire() && pCreature->IsVampire() || 
					IsOusters() && pCreature->IsOusters())
					)
				{
						// 교환 신청
						CGTradePrepare _CGTradePrepare;
						_CGTradePrepare.setTargetObjectID( m_TraceID );
						_CGTradePrepare.setCode( CG_TRADE_PREPARE_CODE_REQUEST );

						g_pSocket->sendPacket( &_CGTradePrepare );

					SetWaitVerify( WAIT_VERIFY_TRADE );
					
					// temp information도 설정.. (음..약간 허접하당.- -;)
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_REQUEST);
					g_pTempInformation->Value1 = m_TraceID;

					//-----------------------------------------------------
					// 교환 취소할까?
					//-----------------------------------------------------
					UI_RunExchangeCancel( pCreature->GetName() );
				}				
			}
			break;

			//---------------------------------------------------------------
			// Party 할려는 경우
			//---------------------------------------------------------------
			case REQUEST_PARTY :
			{
				if (IsWaitVerifyNULL() 
					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
					&& g_pParty!=NULL
					// 내 파티원이 아닌 경우에만 신청 가능하다.
					&& !g_pParty->HasMember( pCreature->GetName() )
					&& (IsSlayer() && pCreature->IsSlayer() || IsVampire() && pCreature->IsVampire() || IsOusters() && pCreature->IsOusters())
					&& g_pSystemAvailableManager->IsAvailablePartySystem()
					)
				{
						// 파티 신청
						CGPartyInvite _CGPartyInvite;
						_CGPartyInvite.setTargetObjectID( m_TraceID );
						_CGPartyInvite.setCode( CG_PARTY_INVITE_REQUEST );

						g_pSocket->sendPacket( &_CGPartyInvite );

					SetWaitVerify( WAIT_VERIFY_PARTY );

					// temp information도 설정.. (음..약간 허접하당.- -;)
					g_pTempInformation->SetMode(TempInformation::MODE_PARTY_REQUEST);
					g_pTempInformation->Value1 = m_TraceID;

					//-----------------------------------------------------
					// 교환 취소할까?
					//-----------------------------------------------------
					UI_RunPartyCancel( pCreature->GetName() );
				}				
			}
			break;

			//---------------------------------------------------------------
			// OtherInfo 볼려는 경우
			//---------------------------------------------------------------
			case REQUEST_INFO:
			{
//				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(m_TraceID);
//				if(pFakeCreature != NULL && pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// 펫이당
//				{
//					UI_RunPetInfo(pFakeCreature->GetPetItem());
//				}
//				else
				if (IsWaitVerifyNULL() 
					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
					&& (IsSlayer() && pCreature->IsSlayer() || 
					IsVampire() && pCreature->IsVampire() || 
					IsOusters() && pCreature->IsOusters() ||
					GetCreatureType() == CREATURETYPE_SLAYER_OPERATOR ||
					GetCreatureType() == CREATURETYPE_VAMPIRE_OPERATOR ||
					GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR ||
					GetCompetence() == 0 )
					)
				{
						const char* str = gC_vs_ui.GetInputString();
						if(strlen(str)>0)
						{
							if(str[0] == '*')
							{
								gC_vs_ui.AddInputString(pCreature->GetName());
								break;

							}
						}
						// 정보 신청
						CGRequestInfo _CGRequestInfo;
						_CGRequestInfo.setValue( m_TraceID );
						_CGRequestInfo.setCode( CGRequestInfo::REQUEST_CHARACTER_INFO );

						g_pSocket->sendPacket( &_CGRequestInfo );

						MCreature *pCreature = g_pZone->GetCreature(m_TraceID);
						
						if(pCreature != NULL)
						{
							UI_RunOtherInfo(pCreature);

							if (g_pProfileManager!=NULL 
								&& g_pRequestUserManager!=NULL
								&& g_pRequestClientPlayerManager!=NULL)
								//&& !g_pProfileManager->HasProfile(pName)
								//&& !g_pProfileManager->HasProfileNULL(pName)
								//&& !g_pRequestUserManager->HasRequestingUser(pName)
								//&& !g_pRequestClientPlayerManager->HasConnection(pName)
								//&& !g_pRequestClientPlayerManager->HasTryingConnection(pName))
							{
								g_pProfileManager->RequestProfile(pCreature->GetName());
							}
						}


				}
			}
			break;
	
		}

		// 필요하려나.. - -;
		m_bTraceCreatureToForceAttack = false;
		m_bKeepTraceCreature = false;

		// 정지한다.
		SetNextAction(ACTION_STAND);
		m_nUsedActionInfo = ACTIONINFO_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
		m_fNextTrace = FLAG_TRACE_NULL;
		UnSetRepeatAction();

		UnSetRequestMode();
	}	
	//---------------------------------------------------------------
	// 아니면.. 공격이거나 멈춤..
	//---------------------------------------------------------------
	else
	{
		//---------------------------------------------------------------
		// 강제 attack이거나..
		// 추적대상이 되는 캐릭터의 종족을 보고 공격할지를 결정한다.
		//---------------------------------------------------------------
		bool bOusters = IsOusters();
		MItem *pOustersItem = NULL;
		if( bOusters) 
			pOustersItem = g_pOustersGear->GetItem(MOustersGear::GEAR_OUSTERS_RIGHTHAND);

		if (m_bTraceCreatureToForceAttack 
			&& (bOusters && pOustersItem != NULL && pOustersItem->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM
				|| !bOusters)
			)
		{
			SetNextAction(GetActionInfoAction(m_nBasicActionInfo));

			//--------------------
			// message
			//--------------------
			DEBUG_ADD_FORMAT("Basic Action : %d", m_TraceID);					
			
			// 
			//m_bTraceCreatureToForceAttack = false;
		}
		else
		{
			DEBUG_ADD_FORMAT("Basic Action : %d - But don't attack", m_TraceID);					
			
			// 공격 안한다.
			m_nUsedActionInfo = ACTIONINFO_NULL;
			
			//---------------------------------
			// 계속 추적해야 하면..
			//---------------------------------
			if (m_bKeepTraceCreature)
			{
				return;
			}
		}
	}
	/*
	#ifdef	CONNECT_SERVER
		CGAttackNormal _CGAttackNormal;
		_CGAttackNormal.setX( m_X );
		_CGAttackNormal.setY( m_Y );
		_CGAttackNormal.setDir( m_Direction );
		_CGAttackNormal.setObjectID( m_TraceID );
		g_pSocket->sendPacket( &_CGAttackNormal );

		#ifdef	OUTPUT_DEBUG
			DEBUG_ADD( _CGAttackNormal.toString().c_str() );							
		#endif
	#endif
	*/	
}
//void
//MPlayer::BasicActionToCreature()
//{
//	if (IsInDarkness())
//	{
//		return;
//	}
//
//	MCreature* pCreature = g_pZone->GetCreature( m_TraceID );
//
//	if (pCreature==NULL)// || pCreature->IsDead())
//	{
//		return;
//	}
//
//	//---------------------------------------------------------------
//	// 대상이 NPC인 경우..
//	//---------------------------------------------------------------
//	if (pCreature->IsNPC())
//	{
//		// 박쥐나 늑대가 아닌 경우.. 거래 가능하다.
//		if (//m_CreatureType!=CREATURETYPE_BAT
//			//&& m_CreatureType!=CREATURETYPE_WOLF)
//			(*g_pCreatureSpriteTable)[(*g_pCreatureTable)[m_CreatureType].SpriteType].IsPlayerOnlySprite())
//		{		
//			if(pCreature->GetCreatureType() == 659)	// 대지정령의 뿔이면 UI띄운다
//				UI_RunHorn(g_pZone->GetID());
//			else
//				
//				if (IsWaitVerifyNULL())
//				{
//					// NPC 처리 packet을 보낸다.
//					CGNPCTalk _CGNPCTalk;
//					_CGNPCTalk.setObjectID( m_TraceID );
//
//					g_pSocket->sendPacket( &_CGNPCTalk );
//
//					SetWaitVerify( WAIT_VERIFY_NPC_ASK );
//				}
//		}
//
//		// 필요하려나.. - -;
//		m_bTraceCreatureToForceAttack = false;
//		m_bKeepTraceCreature = false;
//
//		// 정지한다.
//		SetNextAction(ACTION_STAND);
//		m_nUsedActionInfo = ACTIONINFO_NULL;
//		m_nNextUsedActionInfo = ACTIONINFO_NULL;
//		m_fNextTrace = FLAG_TRACE_NULL;
//		UnSetRepeatAction();
//	}
//	//---------------------------------------------------------------
//	// Request할려는 경우
//	//---------------------------------------------------------------
//	else if (IsRequestMode())
//	{
//		switch (m_RequestMode)
//		{
//			//---------------------------------------------------------------
//			// Trade 할려는 경우
//			//---------------------------------------------------------------
//			case REQUEST_TRADE :
//			{
//				if (IsWaitVerifyNULL() 
//					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
//					&& (IsSlayer() && pCreature->IsSlayer() || 
//					IsVampire() && pCreature->IsVampire() || 
//					IsOusters() && pCreature->IsOusters())
//					)
//				{
//						// 교환 신청
//						CGTradePrepare _CGTradePrepare;
//						_CGTradePrepare.setTargetObjectID( m_TraceID );
//						_CGTradePrepare.setCode( CG_TRADE_PREPARE_CODE_REQUEST );
//
//						g_pSocket->sendPacket( &_CGTradePrepare );
//
//					SetWaitVerify( WAIT_VERIFY_TRADE );
//					
//					// temp information도 설정.. (음..약간 허접하당.- -;)
//					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_REQUEST);
//					g_pTempInformation->Value1 = m_TraceID;
//
//					//-----------------------------------------------------
//					// 교환 취소할까?
//					//-----------------------------------------------------
//					UI_RunExchangeCancel( pCreature->GetName() );
//				}				
//			}
//			break;
//
//			//---------------------------------------------------------------
//			// Party 할려는 경우
//			//---------------------------------------------------------------
//			case REQUEST_PARTY :
//			{
//				if (IsWaitVerifyNULL() 
//					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
//					&& g_pParty!=NULL
//					// 내 파티원이 아닌 경우에만 신청 가능하다.
//					&& !g_pParty->HasMember( pCreature->GetName() )
//					&& (IsSlayer() && pCreature->IsSlayer() || IsVampire() && pCreature->IsVampire() || IsOusters() && pCreature->IsOusters())
//					&& g_pSystemAvailableManager->IsAvailablePartySystem()
//					)
//				{
//						// 파티 신청
//						CGPartyInvite _CGPartyInvite;
//						_CGPartyInvite.setTargetObjectID( m_TraceID );
//						_CGPartyInvite.setCode( CG_PARTY_INVITE_REQUEST );
//
//						g_pSocket->sendPacket( &_CGPartyInvite );
//
//					SetWaitVerify( WAIT_VERIFY_PARTY );
//
//					// temp information도 설정.. (음..약간 허접하당.- -;)
//					g_pTempInformation->SetMode(TempInformation::MODE_PARTY_REQUEST);
//					g_pTempInformation->Value1 = m_TraceID;
//
//					//-----------------------------------------------------
//					// 교환 취소할까?
//					//-----------------------------------------------------
//					UI_RunPartyCancel( pCreature->GetName() );
//				}				
//			}
//			break;
//
//			//---------------------------------------------------------------
//			// OtherInfo 볼려는 경우
//			//---------------------------------------------------------------
//			case REQUEST_INFO:
//			{
////				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(m_TraceID);
////				if(pFakeCreature != NULL && pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// 펫이당
////				{
////					UI_RunPetInfo(pFakeCreature->GetPetItem());
////				}
////				else
//				if (IsWaitVerifyNULL() 
//					&& g_pTempInformation->GetMode() == TempInformation::MODE_NULL
//					&& (IsSlayer() && pCreature->IsSlayer() || 
//					IsVampire() && pCreature->IsVampire() || 
//					IsOusters() && pCreature->IsOusters() ||
//					GetCreatureType() == CREATURETYPE_SLAYER_OPERATOR ||
//					GetCreatureType() == CREATURETYPE_VAMPIRE_OPERATOR ||
//					GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR ||
//					GetCompetence() == 0 )
//					)
//				{
//						const char* str = gC_vs_ui.GetInputString();
//						if(strlen(str)>0)
//						{
//							if(str[0] == '*')
//							{
//								gC_vs_ui.AddInputString(pCreature->GetName());
//								break;
//
//							}
//						}
//						// 정보 신청
//						CGRequestInfo _CGRequestInfo;
//						_CGRequestInfo.setValue( m_TraceID );
//						_CGRequestInfo.setCode( CGRequestInfo::REQUEST_CHARACTER_INFO );
//
//						g_pSocket->sendPacket( &_CGRequestInfo );
//
//						MCreature *pCreature = g_pZone->GetCreature(m_TraceID);
//						
//						if(pCreature != NULL)
//						{
//							UI_RunOtherInfo(pCreature);
//
//							if (g_pProfileManager!=NULL 
//								&& g_pRequestUserManager!=NULL
//								&& g_pRequestClientPlayerManager!=NULL)
//								//&& !g_pProfileManager->HasProfile(pName)
//								//&& !g_pProfileManager->HasProfileNULL(pName)
//								//&& !g_pRequestUserManager->HasRequestingUser(pName)
//								//&& !g_pRequestClientPlayerManager->HasConnection(pName)
//								//&& !g_pRequestClientPlayerManager->HasTryingConnection(pName))
//							{
//								g_pProfileManager->RequestProfile(pCreature->GetName());
//							}
//						}
//
//
//				}
//			}
//			break;
//	
//		}
//
//		// 필요하려나.. - -;
//		m_bTraceCreatureToForceAttack = false;
//		m_bKeepTraceCreature = false;
//
//		// 정지한다.
//		SetNextAction(ACTION_STAND);
//		m_nUsedActionInfo = ACTIONINFO_NULL;
//		m_nNextUsedActionInfo = ACTIONINFO_NULL;
//		m_fNextTrace = FLAG_TRACE_NULL;
//		UnSetRepeatAction();
//
//		UnSetRequestMode();
//	}	
//	//---------------------------------------------------------------
//	// 아니면.. 공격이거나 멈춤..
//	//---------------------------------------------------------------
//	else
//	{
//		//---------------------------------------------------------------
//		// 강제 attack이거나..
//		// 추적대상이 되는 캐릭터의 종족을 보고 공격할지를 결정한다.
//		//---------------------------------------------------------------
//		bool bOusters = IsOusters();
//		MItem *pOustersItem = NULL;
//		if( bOusters) 
//			pOustersItem = g_pOustersGear->GetItem(MOustersGear::GEAR_OUSTERS_RIGHTHAND);
//
//		if (m_bTraceCreatureToForceAttack 
//			&& (bOusters && pOustersItem != NULL && pOustersItem->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM
//				|| !bOusters)
//			)
//		{
//			SetNextAction(GetActionInfoAction(m_nBasicActionInfo));
//
//			//--------------------
//			// message
//			//--------------------
//			DEBUG_ADD_FORMAT("Basic Action : %d", m_TraceID);					
//			
//			// 
//			//m_bTraceCreatureToForceAttack = false;
//		}
//		else
//		{
//			DEBUG_ADD_FORMAT("Basic Action : %d - But don't attack", m_TraceID);					
//			
//			// 공격 안한다.
//			m_nUsedActionInfo = ACTIONINFO_NULL;
//			
//			//---------------------------------
//			// 계속 추적해야 하면..
//			//---------------------------------
//			if (m_bKeepTraceCreature)
//			{
//				return;
//			}
//		}
//	}
//	/*
//	#ifdef	CONNECT_SERVER
//		CGAttackNormal _CGAttackNormal;
//		_CGAttackNormal.setX( m_X );
//		_CGAttackNormal.setY( m_Y );
//		_CGAttackNormal.setDir( m_Direction );
//		_CGAttackNormal.setObjectID( m_TraceID );
//		g_pSocket->sendPacket( &_CGAttackNormal );
//
//		#ifdef	OUTPUT_DEBUG
//			DEBUG_ADD( _CGAttackNormal.toString().c_str() );							
//		#endif
//	#endif
//	*/	
//}

//----------------------------------------------------------------------
// Pickup Item
//----------------------------------------------------------------------
// Item줍기를 수행한다.
//----------------------------------------------------------------------
void
MPlayer::PickupItem(MItem* pItem)
{
	//----------------------------------------------------
	// 검증 받을 아이템이 없고..
	// 현재 들고 있는 Item이 없는 경우
	//----------------------------------------------------

	if( HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ) )
		return;

	if (IsItemCheckBufferNULL() 
		&& gpC_mouse_pointer->GetPickUpItem()==NULL)
	{
		//----------------------------------------------------------------
		// 위치 검증이 된 상태
		//----------------------------------------------------------------
		//if (m_SendMove==0)
		{
			// [ TEST CODE ]
			// Item 정보를 읽어와서 Item줍기할때 Sound를 낸다.
			// 이 부분은 나중에.. 
			// Server Packet에서 확실히 "주웠다"라고 할 때
			// 하는 것이 좋을 것 같기도 하다.
			//MItem*	pItem = m_pZone->GetItem( m_TraceID );

			//g_Sound.Play( g_SoundTable[ g_ItemTable[pItem->GetItemType()].PickupSoundID ].pDSBuffer );
			//PlaySound( g_ItemTable[pItem->GetItemClass()][pItem->GetItemType()].TileSoundID ,
			//			false,
			//			m_X, m_Y);

			// item class 비교하지 말고..
			// item class에다가 virtual pickup()을 두는게 낫지 싶은데.
			// 귀찮어랑.. -_-;;

			//----------------------------------------------------------------
			//
			//	 Motorcycle인 경우 --> 타기
			//
			//----------------------------------------------------------------
			if (pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
			{	
				if (IsSlayer()
					&& m_CreatureType!=CREATURETYPE_SLAYER_OPERATOR)
				{
					RideMotorcycle( (MMotorcycle*)pItem );
				}
			}	
			
			//----------------------------------------------------------------
			//
			// 시체인 경우 --> Looting
			//
			//----------------------------------------------------------------
			else if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
			{				
				//----------------------------------------------------
				// Server에 접속해 있을 때,
				//----------------------------------------------------
					MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();

					if ( pCreature != NULL &&
						(
						pCreature->GetActionCount() >= pCreature->GetActionCountMax() || 
						pCreature->GetCreatureType() == 672 
						)
						)
					{	
						if (pItem->GetNumber()>0 || 
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || 
							pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || 
							pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 ||
							pCreature->GetCreatureType() == 670 ||			// 깃대면
							pCreature->GetCreatureType() == 672	||			// 스위퍼 보관대이면
							pCreature->GetCreatureType() == 673
							)
						{
							// 박쥐가 아닌 경우에만 보낸다.
							// 오토바이를 타지 않은 보통 걸음인 경우에만
							// 석화 안걸렸을때
							if (m_CreatureType!=CREATURETYPE_BAT
								&& m_MoveDevice==MOVE_DEVICE_WALK
								&& !m_bEffectStatus[EFFECTSTATUS_CURSE_PARALYSIS])
							{
								CGDissectionCorpse _CGDissectionCorpse;
								_CGDissectionCorpse.setX( pItem->GetX() );
								_CGDissectionCorpse.setY( pItem->GetY() );
								_CGDissectionCorpse.setObjectID( pItem->GetID() );

								g_pSocket->sendPacket( &_CGDissectionCorpse );

								if(pItem->GetNumber() > 0)
									pItem->SetNumber( pItem->GetNumber()-1 );
							}
						}
						// item이 없는 경우엔..
						else
						{
							// 늑대라면 시체 먹기
							// 성물이 아닌 경우에만, 수호성단이 아닌 경우에만.
							if ( m_CreatureType==CREATURETYPE_WOLF && 
								!(
								pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || 
								pCreature->GetCreatureType() == 670	||			// 깃대면
								pCreature->GetCreatureType() == 672 ||			// 스위퍼 보관대이면
								pCreature->GetCreatureType() == 673 ||
								pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || 
								pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563
								)								
							   )
							{
								// 사용한 기술로 설정
								m_nNextUsedActionInfo	= MAGIC_EAT_CORPSE;

								// 추적 정보 설정
								m_fTraceBuffer = m_fTrace = FLAG_TRACE_SECTOR_SPECIAL;
								m_TraceX	= pItem->GetX();
								m_TraceY	= pItem->GetY();
								m_TraceZ	= 0;

								m_TraceDistance		= GetActionInfoRange(m_nNextUsedActionInfo);

								SetAction( m_MoveAction );
								SetNextDestination(m_TraceX, m_TraceY);

								m_fNextTrace			= FLAG_TRACE_NULL;
								
								m_bKeepTraceCreature	= false;
							}							
						}						
					}
			}

			//----------------------------------------------------------------
			//
			// 돈인 경우 --> 무조건 inventory로..
			//
			//----------------------------------------------------------------
			else if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
			{
				PickupMoney( (MMoney*)pItem );	
			}
			
			else //if (m_CreatureType!=CREATURETYPE_BAT || m_MoveDevice!=MOVE_DEVICE_WALK)
			{
				//----------------------------------------------------------------
				// 설치된 지뢰인 경우는 주을 수 없다.
				//----------------------------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_MINE)
				{
					if (((MMine*)pItem)->IsInstalled())
					{
						return;
					}
				}				
				//----------------------------------------------------------------
				//
				//	 Event 아이템인 경우
				//
				//----------------------------------------------------------------
//				else if (pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX)
//				{
//					// 같은 type의 아이템이 있는지 체크한다.
//					if (NULL != g_pInventory->FindItem( ITEM_CLASS_EVENT_GIFT_BOX, pItem->GetItemType() ))
//					{
//						// 있다면 못 줍는다.
//						return;
//					}
//				}

				//----------------------------------------------------------------
				//
				//	Gear에 들어갈 수 있는 Item인 경우..
				//
				//----------------------------------------------------------------
				if (pItem->IsGearItem())
				{					
					//------------------------------------------
					// gear나 Inventory가 열려있다면 --> mouse로
					//------------------------------------------
					if (gC_vs_ui.GetGearOpenState() ||
						gC_vs_ui.GetInventoryOpenState())
					{							
						PickupItemToMouse( pItem );
					}
					//------------------------------------------
					// 아니면 
					//------------------------------------------
					else
					{
						// gear에 넣어보고 들어갈 수 없다면
						// inventory에 넣어본다.

						// 둘 다 안 들어가면 못 집는다.

						// 일단은.. --;
						PickupItemToInventory( pItem );									
					}
				}
				//----------------------------------------------------------------
				//
				//	Inventory에 들어갈 수 있는 Item인 경우..
				//
				//----------------------------------------------------------------
				else if (pItem->IsInventoryItem())
				{												
					//----------------------------------------------------------------
					// inventory가 열려있다면 --> mouse로
					//----------------------------------------------------------------
					if (gC_vs_ui.GetInventoryOpenState())
					{							
						PickupItemToMouse( pItem );
					}
					//----------------------------------------------------------------
					// 아니면 그냥 inventory로
					//----------------------------------------------------------------
					else
					{
						if (pItem->IsQuickItem()
							&& PickupItemToQuickslot( pItem ))
						{
							// quickslot에 들어갈 수 있어서 들어가도 되는 경우다.
							// 냥냥..
						}
						else
						{
							PickupItemToInventory( pItem );
						}
					}	
				}
			}
		}
		//----------------------------------------------------------------
		// 위치 검증이 안된 상태
		//----------------------------------------------------------------
		//DEBUG_ADD_FORMAT("Cannot Pickup item (Position not verified) sendMove=%d", m_SendMove);		
	}
	#ifdef OUTPUT_DEBUG
	else
	{
		if (gpC_mouse_pointer->GetPickUpItem()!=NULL)
		{
			DEBUG_ADD_FORMAT("[UI] Already Handle a Item id=%d", gpC_mouse_pointer->GetPickUpItem()->GetID());
		}
	}
	#endif
	
}

//----------------------------------------------------------------------
// Pickup Item to Inventory
//----------------------------------------------------------------------
// Zone의 Item을 주워서 inventory에 넣는다.
//----------------------------------------------------------------------
void
MPlayer::PickupItemToInventory(MItem* pItem)
{
	//------------------------------------------
	// Inventory에 들어갈 자리가 있는 경우
	// : Server에서 검증받기 위한 packet을 보낸다.
	//------------------------------------------
		POINT fitPoint;
		//------------------------------------------
		// 이전에 처리할려는 item이 없는 경우..
		//------------------------------------------
		if (IsItemCheckBufferNULL())
		{
			if (g_pInventory->GetFitPosition(pItem, fitPoint) && 
				(pItem->IsSlayerItem() && IsSlayer() || 
				pItem->IsVampireItem() && IsVampire() || 
				pItem->IsOustersItem() && IsOusters()))
			{
				CGAddZoneToInventory _CGAddZoneToInventory;
				
				_CGAddZoneToInventory.setObjectID( pItem->GetID() );
				_CGAddZoneToInventory.setZoneX( pItem->GetX() );
				_CGAddZoneToInventory.setZoneY( pItem->GetY() );
				_CGAddZoneToInventory.setInvenX( fitPoint.x );
				_CGAddZoneToInventory.setInvenY( fitPoint.y );

				g_pSocket->sendPacket( &_CGAddZoneToInventory );

				// 주울려는 item을 기억한다.
				pItem->SetGridXY( fitPoint.x, fitPoint.y );
				SetItemCheckBuffer(pItem, ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY);											
			}
			else
			{
				//------------------------------------------------------------
				// inventory에 적절한 공간이 없어서 주울 수 없는 경우이다.				
				//------------------------------------------------------------
				pItem->SetDropping();

				// 2004, 5, 7 , sobeit add start - 인벤에 자리가 없으면 보관함 사라고 도움말 보여줌
				ExecuteHelpEvent( HELP_EVENT_STORAGE_BUY );
				// 2004, 5, 6, sobeit add end
			}
		}
		//------------------------------------------
		// 아직 다른 item을 처리 중이기 때문에
		// item을 줍지 못하는 경우이다.
		//------------------------------------------
		else
		{
			// message출력
			// "아직 item을 주울 수 없습니다."
		}
}

//----------------------------------------------------------------------
// Pickup Money
//----------------------------------------------------------------------
// Zone의 Item을 줍는다.
//----------------------------------------------------------------------
void
MPlayer::PickupMoney(MMoney* pItem)
{
		//------------------------------------------
		// 이전에 처리할려는 item이 없는 경우..
		//------------------------------------------
		if (IsItemCheckBufferNULL())
		{		
			CGPickupMoney _CGPickupMoney;
			
			_CGPickupMoney.setObjectID( pItem->GetID() );
			_CGPickupMoney.setZoneX( pItem->GetX() );
			_CGPickupMoney.setZoneY( pItem->GetY() );
		
			g_pSocket->sendPacket( &_CGPickupMoney );

			// 주울려는 item을 기억한다.
			SetItemCheckBuffer(pItem, ITEM_CHECK_BUFFER_PICKUP_MONEY);											
		
		}
		//------------------------------------------
		// 아직 다른 item을 처리 중이기 때문에
		// item을 줍지 못하는 경우이다.
		//------------------------------------------
		else
		{
			// message출력
			// "아직 item을 주울 수 없습니다."
		}
}

//----------------------------------------------------------------------
// Pickup Item to Mouse
//----------------------------------------------------------------------
// Zone의 Item을 주워서 Mouse에 붙인다. - -
//----------------------------------------------------------------------
void
MPlayer::PickupItemToMouse(MItem* pItem)
{
	//------------------------------------------
	// Server에 접속된 경우는..
	//------------------------------------------
	// item을 주울 수 있는지 server에서 
	// 검증을 받아야 한다.
	//------------------------------------------
		//------------------------------------------
		// 이전에 처리할려는 item이 없는 경우..
		//------------------------------------------
		if (IsItemCheckBufferNULL())
		{
			if(pItem->IsSlayerItem() && IsSlayer() || 
				pItem->IsVampireItem() && IsVampire() ||
				pItem->IsOustersItem() && IsOusters())
			{
				CGAddZoneToMouse _CGAddZoneToMouse;
				
				_CGAddZoneToMouse.setObjectID( pItem->GetID() );
				_CGAddZoneToMouse.setZoneX( pItem->GetX() );
				_CGAddZoneToMouse.setZoneY( pItem->GetY() );
				
				g_pSocket->sendPacket( &_CGAddZoneToMouse );
				
				// 주울려는 item을 기억한다.
				SetItemCheckBuffer(pItem, ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE);
			}
		}								
		//------------------------------------------
		// item을 줍지 못하는 경우이다.
		//------------------------------------------
		else
		{
			// message출력
			// "아직 item을 주울 수 없습니다."
		}
}

//----------------------------------------------------------------------
// Pickup Item to Quickslot
//----------------------------------------------------------------------
// Zone의 Item을 주워서 Mouse에 붙인다. - -
//----------------------------------------------------------------------
bool
MPlayer::PickupItemToQuickslot(MItem* pItem)
{
	//------------------------------------------------------------------
	// 일단 QuickSlot에 들어갈 수 있는지를 확인해야 한다.
	//------------------------------------------------------------------
	if ( (g_pQuickSlot!=NULL&&IsSlayer()) || ((g_pArmsBand1!=NULL||g_pArmsBand2!=NULL)&&IsOusters())
		&& IsItemCheckBufferNULL()
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL
		&& (pItem->IsSlayerItem() && IsSlayer() || 
		pItem->IsVampireItem() && IsVampire() || 
		pItem->IsOustersItem() && IsOusters()))
	{
		BOOL FirstArmsband = TRUE;
		int slot;

		//---------------------------------------------------------
		// QuickSlot의 어디에 들어갈 수 있을까?
		//---------------------------------------------------------
		if (IsSlayer() && !g_pQuickSlot->FindSlotToAddItem(pItem, slot))
		{
			return false;
		} 
		else if( IsOusters() )
		{
			// 첫번째꺼먼저 뒤지고 없으면 두번째껄 뒤진다.
			
			if( g_pArmsBand1 == NULL ||  !g_pArmsBand1->FindSlotToAddItem( pItem, slot ) )
			{
				if( g_pArmsBand2 == NULL || !g_pArmsBand2->FindSlotToAddItem( pItem,slot ) )
					return false;
				else FirstArmsband = FALSE;
			} else
			{
				FirstArmsband = TRUE;
			}
		}

			//---------------------------------------------------------
			// 일단 마우스로 갈 수 있는걸 검증받아야 한다...
			//---------------------------------------------------------
			CGAddZoneToMouse _CGAddZoneToMouse;

			_CGAddZoneToMouse.setObjectID( pItem->GetID() );
			_CGAddZoneToMouse.setZoneX( pItem->GetX() );
			_CGAddZoneToMouse.setZoneY( pItem->GetY() );

			g_pSocket->sendPacket( &_CGAddZoneToMouse );

			//---------------------------------------------------------
			// slot에 들어갈 수 있다.
			//---------------------------------------------------------
			g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_PICKUP_TO_QUICKSLOT);
			g_pTempInformation->Value1 = FirstArmsband ? slot : slot + 3;
			g_pTempInformation->pValue = (void*)pItem;

			SetItemCheckBuffer(pItem, ITEM_CHECK_BUFFER_PICKUP_TO_QUICKSLOT);


		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Pickup Item to Mouse
//----------------------------------------------------------------------
// Zone의 Item을 주워서 Mouse에 붙인다. - -
//----------------------------------------------------------------------
void
MPlayer::RideMotorcycle(MMotorcycle* pMotorcycle)
{
	if (pMotorcycle==NULL)
	{
		DEBUG_ADD("[Error] Player::Motorcycle to ride is NULL");
		
		return;
	}

	//------------------------------------------
	// 걷고 있는 중인 경우 오토바이에 탈 수 있다.
	//------------------------------------------
	if (m_MoveDevice==MOVE_DEVICE_WALK)
	{
		CGRideMotorCycle _CGRideMotorCycle;

		_CGRideMotorCycle.setObjectID( pMotorcycle->GetID() );
		_CGRideMotorCycle.setX( pMotorcycle->GetX() );
		_CGRideMotorCycle.setY( pMotorcycle->GetY() );

		g_pSocket->sendPacket( &_CGRideMotorCycle );

		// Motorcycle을 타기 위해 검증받기를 기다린다.
		SetWaitVerify( WAIT_VERIFY_MOTORCYCLE_GETON );
	}
	//------------------------------------------
	// 이미 오토바이를 타고 있는 경우이다.
	//------------------------------------------
	else
	{
		// 오토바이 탈 수 없다. 
	}
}

//----------------------------------------------------------------------
// Skill To Inventory Item
//----------------------------------------------------------------------
// Inventory에 있는 item에 기술을 사용한다.
// 여기서는 casting동작만 시작하게 하면 된다.
//
// 결과는 packet을 받아서 처리..
// 결과는 (global) AddNewInventoryEffect(...)에서 보여지게 하면 된다.
//----------------------------------------------------------------------
bool
MPlayer::TraceInventoryItem(TYPE_OBJECTID id)
{
	//-----------------------------------------------------
	// delay가 있으면 안된다.
	//-----------------------------------------------------
	if (!IsNotDelay()
		|| HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE ))
	{
		return false;
	}

	//--------------------------------------------------------
	// 사용할 수 없는 경우
	//--------------------------------------------------------
	if (m_nSpecialActionInfo == ACTIONINFO_NULL
		// 오토바이를 타고 있으면 특수공격이 안된다.
		|| m_MoveDevice == MOVE_DEVICE_RIDE
		// 뭔가 기술 사용을 검정 받아야 하면 사용할 수 없다.
		//|| m_WaitVerify != WAIT_VERIFY_NULL)	// 2001.8.20 주석처리
		)
	{
		return false;
	}
	
	//--------------------------------------------------------
	// 다른 기술을 사용중인 경우
	//--------------------------------------------------------
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		return false;

	

	//--------------------------------------------------------
	// 현재 기술이 item에 사용하는게 맞나?
	//--------------------------------------------------------
	if ((*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetItem())
	{
		//-------------------------------------------------------
		// 현재 사용할 수 있는 기술인지 체크..
		// Passive 스킬이면 사용 못하게..
		//-------------------------------------------------------
		if (!g_pSkillAvailable->IsEnableSkill( (ACTIONINFO)m_nSpecialActionInfo )
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsEnable()
			|| !(*g_pSkillInfoTable)[m_nSpecialActionInfo].IsAvailableTime()
			|| (*g_pSkillInfoTable)[m_nSpecialActionInfo].IsPassive())
		{
			m_fNextTrace = FLAG_TRACE_NULL;

			UnSetRepeatAction();

			return false;
		}

		// 정지 상태에서만 사용할 수 있다.
		if (IsStop() && IsItemCheckBufferNULL()) // || m_Action!=ACTION_ATTACK)
		{
			//--------------------------------------------------------
			// pItem이 inventory에 있는 item인지 체크한다.
			//--------------------------------------------------------
			const MItem* pItem = ((MItemManager)(*g_pInventory)).GetItem( id );
			
			if (pItem==NULL)
			{
				return false;
			}

			// 사용한 기술로 설정
			m_nNextUsedActionInfo	= m_nSpecialActionInfo;

			// 추적 정보 설정
			SetTraceID( id );
			m_fTrace	= FLAG_TRACE_INVENTORY;
			m_TraceX	= pItem->GetGridX();
			m_TraceY	= pItem->GetGridY();
			m_TraceZ	= 0;

			// 현재 추적하는 대상에 대해서 기억해둔다.
			m_fTraceBuffer	= m_fTrace;	
			

			//------------------------------------------------------------		
			// Inventory Item에 대해서는 의미 없다. - -;
			//------------------------------------------------------------	
			m_TraceDistance		= 1;		//흔벎굳錦맣槨40앎홍팁숄膠	
			SetAction( m_MoveAction );
			SetNextDestination(m_X, m_Y);

			// buffering을 없앤다.
			m_fNextTrace			= FLAG_TRACE_NULL;
		
			// 계속 쫓아가는 것을 중지
			m_bKeepTraceCreature	= false;

			//------------------------------------------------------------
			// message출력
			//------------------------------------------------------------
			DEBUG_ADD_FORMAT("Trace IteMM: %d", id);				
		}
		else
		{
			//------------------------------------------------------------
			// 반복 action하는 중이 아닌 경우에 다음 동작 buffering
			//------------------------------------------------------------
			if (m_bRepeatAction)
			{
				m_fNextTrace	= FLAG_TRACE_NULL;

				return false;
			}
			else
			{
				m_fNextTrace	= FLAG_TRACE_INVENTORY;
				m_NextTraceID	= id;

				m_bRepeatAction = FALSE;

				return false;
			}
		}

		return true;
	}

	// item에 사용하는 기술이 아니라는 의미
	return false;
}

//----------------------------------------------------------------------
// Change To Slayer
//----------------------------------------------------------------------
// slayer로 변신하는데..
// 이미 slayer이면.. return false
//----------------------------------------------------------------------
bool	
MPlayer::ChangeToSlayer()
{
	if (MCreature::ChangeToSlayer())
	{
		m_ConversionDelayTime = 0;

		g_pCurrentMagazine = NULL;
		
		//-----------------------------------------------------
		// 보관함도 날려준다.
		//-----------------------------------------------------
		if (g_pStorage!=NULL)
		{
			delete g_pStorage;
			g_pStorage = NULL;
		}

		//-----------------------------------------------------
		// 번쩍
		//-----------------------------------------------------
		g_pTopView->SetFadeStart(1, 31, 6,  5,5,31);

		UI_ChangeInterfaceRace(RACE_SLAYER);

		SetWaitVerifyNULL();

		ResetSendMove();
		
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Change To Vampire
//----------------------------------------------------------------------
// vampire로 변신하는데..
// 이미 vampire이면.. return false
//----------------------------------------------------------------------
bool
MPlayer::ChangeToVampire()
{
	if (MCreature::ChangeToVampire())
	{
		m_ConversionDelayTime = 0;

		g_pCurrentMagazine = NULL;
		

		//-----------------------------------------------------
		// 보관함도 날려준다.
		//-----------------------------------------------------
		if (g_pStorage!=NULL)
		{
			delete g_pStorage;
			g_pStorage = NULL;
		}

		g_pTopView->SetFadeStart(1, 31, 6,  31,0,0);

		UI_ChangeInterfaceRace(RACE_VAMPIRE);

		SetWaitVerifyNULL();

		ResetSendMove();

		DEBUG_ADD("MPlayer::Change to Vampire OK");
		
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Set Status
//----------------------------------------------------------------------
// 캐릭터의 상태값을 바꾼다.
//----------------------------------------------------------------------
void	
MPlayer::SetStatus(DWORD n, DWORD value)
{ 
	if (n >= MODIFY_MAX)
	{
		DEBUG_ADD_FORMAT("[Error] Modify Part is Wrong : part=%d, value=%d", n, value);
		
		return;
	}

	MODIFY_VALUE modifyValue;
	modifyValue.oldValue = m_Status[n];
	modifyValue.newValue = value;

	// 일단 값 변경
	m_Status[n] = value; 
	
	// 변경된 값과 관련된 처리
	g_pModifyStatusManager->Execute( n, (void*)&modifyValue );

	// 다시.. - -;
	m_Status[n] = modifyValue.newValue;
}

//----------------------------------------------------------------------
// Check Status
//----------------------------------------------------------------------
// 수치 변하는거 체크
//----------------------------------------------------------------------
void	
MPlayer::CalculateStatus()
{
	//-----------------------------------------------------------------
	// item 착용한거에 따라서...
	//-----------------------------------------------------------------
	MItemManager* pGear;

	int weaponSpeed = 0;
	int weaponTohit = 0;

	switch(GetRace())
	{
	case RACE_SLAYER:
		{
			pGear = g_pSlayerGear;
			
			const MItem* pWeapon = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );
			
			SKILLDOMAIN domain = MAX_SKILLDOMAIN;
			int domainLevel = 0;
			
			//------------------------------------------------------
			// 총들었는지 체크
			//------------------------------------------------------
			if (pWeapon!=NULL && pWeapon->IsAffectStatus())
			{
				if (pWeapon->IsGunItem())
				{
					domain = SKILLDOMAIN_GUN;
					
					weaponTohit = pWeapon->GetToHit();
				}
				else if (pWeapon->GetItemClass()==ITEM_CLASS_SWORD)
				{
					domain = SKILLDOMAIN_SWORD;
				}
				else if (pWeapon->GetItemClass()==ITEM_CLASS_BLADE)
				{
					domain = SKILLDOMAIN_BLADE;
				}
				else if (pWeapon->GetItemClass()==ITEM_CLASS_CROSS)
				{
					domain = SKILLDOMAIN_HEAL;
				}
				else if (pWeapon->GetItemClass()==ITEM_CLASS_MACE)
				{
					domain = SKILLDOMAIN_ENCHANT;
				}
				
				if (domain < MAX_SKILLDOMAIN)
				{
					domainLevel = (*g_pSkillManager)[domain].GetDomainLevel();
				}
				
				
				//----------------------------------------------------
				// 무기의 기본 속도
				//----------------------------------------------------			
				if(pWeapon->IsAffectStatus())
					weaponSpeed = pWeapon->GetOriginalSpeed();
			}
			
			g_StatusManager.SetCurrentWeaponDomain( domain, domainLevel );
		}
		break;

	case RACE_VAMPIRE:
		{
			pGear = g_pVampireGear;
			
			const MItem* pWeapon = g_pVampireGear->GetItem( MVampireGear::GEAR_VAMPIRE_RIGHTHAND );
			
			if (pWeapon!=NULL)
			{
				//----------------------------------------------------
				// 무기의 기본 속도
				//----------------------------------------------------			
				if(pWeapon->IsAffectStatus())
					weaponSpeed = pWeapon->GetOriginalSpeed();
			}
			g_StatusManager.SetCurrentWeaponDomain( SKILLDOMAIN_VAMPIRE, GetLEVEL() );
		}
		break;

	case RACE_OUSTERS:
		{
			pGear = g_pOustersGear;
			
			const MItem* pWeapon = g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_RIGHTHAND );
			
			if (pWeapon!=NULL)
			{
				//----------------------------------------------------
				// 무기의 기본 속도
				//----------------------------------------------------			
				if(pWeapon->IsAffectStatus())
					weaponSpeed = pWeapon->GetOriginalSpeed();
			}
			g_StatusManager.SetCurrentWeaponDomain( SKILLDOMAIN_OUSTERS, GetLEVEL() );
		}
		break;
	}

	
	//-----------------------------------------------------------------
	// Gear의 모든 item에 대해서..
	//
	// DAM, AC, TOHIT, CC option만 적용시킨다.
	//-----------------------------------------------------------------
	g_StatusManager.Set(GetSTR(), GetDEX(), GetINT());

	int DV		= g_StatusManager.GetDefense();
	int PV		= g_StatusManager.GetProtection();
	int TOHIT	= g_StatusManager.GetTOHIT() + weaponTohit;
	//int CC		= g_StatusManager.GetCC();
	int MinDAM	= m_Status[MODIFY_MIN_DAMAGE];	//g_StatusManager.GetMinDAM();
	int MaxDAM	= m_Status[MODIFY_MAX_DAMAGE];	//g_StatusManager.GetMaxDAM();
	int AttackSpeed = g_StatusManager.GetAttackSpeed() + weaponSpeed;

	if((*g_pRankBonusTable)[RANK_BONUS_HAWK_WING].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		AttackSpeed += (*g_pRankBonusTable)[RANK_BONUS_HAWK_WING].GetPoint();

	if((*g_pRankBonusTable)[RANK_BONUS_CROW_WING].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		AttackSpeed += (*g_pRankBonusTable)[RANK_BONUS_CROW_WING].GetPoint();
	
	if((*g_pRankBonusTable)[RANK_BONUS_SWIFT_ARM].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		AttackSpeed += (*g_pRankBonusTable)[RANK_BONUS_SWIFT_ARM].GetPoint();

	// Pet Bonus
	if( GetPetID() != OBJECTID_NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetPetID());
		if(pFakeCreature != NULL)
		{
			MPetItem *pPetItem = pFakeCreature->GetPetItem();
			if(pPetItem != NULL)
			{
				ITEMOPTION_TABLE::ITEMOPTION_PART attr = (ITEMOPTION_TABLE::ITEMOPTION_PART)pPetItem->GetEnchantLevel();
				if(attr == ITEMOPTION_TABLE::PART_ATTACK_SPEED)
				{
					AttackSpeed += pPetItem->GetSilver();
				}
			}
		}
	}

	int	ElementalFire = 0;
	int	ElementalWater = 0;
	int	ElementalEarth = 0;
	int	ElementalWind = 0;

	//-----------------------------------------------------------------
	// 아이템에 의해서 시야가 밝아지는거 체크.
	//-----------------------------------------------------------------
	SetItemLightSight(0);

	pGear->SetBegin();

	while (pGear->IsNotEnd())
	{
		const MItem* pItem = pGear->Get();

		//----------------------------------------------------------
		// 수치가 적용되는 item인 경우만..
		//----------------------------------------------------------
		if (pItem != NULL && pItem->IsAffectStatus())
		{
			if(pItem->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET || pItem->GetItemClass() == ITEM_CLASS_OUSTERS_STONE)
			{
				ITEMTABLE_INFO iInfo = (*g_pItemTable)[pItem->GetItemClass()][pItem->GetItemType()];
				if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE)
					ElementalFire += iInfo.Elemental;
				else if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER)
					ElementalWater += iInfo.Elemental;
				else if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH)
					ElementalEarth += iInfo.Elemental;
				else if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND)
					ElementalWind += iInfo.Elemental;

			}
			//---------------------------------------------------
			// 기본적인 방어수치 증가
			//---------------------------------------------------
			int def = pItem->GetDefenseValue();
			
			if (def != -1)
			{
				DV += def;
			}

			int pro = pItem->GetProtectionValue();
			
			if (pro != -1)
			{
				PV += pro;
			}

			const std::list<TYPE_ITEM_OPTION>& optionList = pItem->GetItemOptionList();

			std::list<TYPE_ITEM_OPTION>::const_iterator optionListItr = optionList.begin();

			while(optionListItr != optionList.end())
			{
				
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[*optionListItr];
				//---------------------------------------------------
				// 부가적인 Option
				//---------------------------------------------------
				switch ((ITEMOPTION_TABLE::ITEMOPTION_PART)optionInfo.Part)
				{
				case ITEMOPTION_TABLE::PART_DEFENSE		: DV += optionInfo.PlusPoint; break;
				case ITEMOPTION_TABLE::PART_TOHIT		: TOHIT += optionInfo.PlusPoint; break;
				case ITEMOPTION_TABLE::PART_PROTECTION	: PV += optionInfo.PlusPoint; break;
					//case ITEMOPTION_INFO::PART_DAMAGE		: MinDAM += optionInfo.PlusPoint; 
					//											MaxDAM += optionInfo.PlusPoint; 
					//break;
					
				case ITEMOPTION_TABLE::PART_ATTACK_SPEED : AttackSpeed += optionInfo.PlusPoint; break;
					
				case ITEMOPTION_TABLE::PART_VISION :	SetItemLightSight(GetItemLightSight() + optionInfo.PlusPoint); break;
				}
				optionListItr++;
			}

			std::list<TYPE_ITEM_OPTION>::const_iterator itr = pItem->GetItemDefaultOptionList().begin();

			while(itr != pItem->GetItemDefaultOptionList().end())
			{
				TYPE_ITEM_OPTION option = *itr;

				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[option];
				//---------------------------------------------------
				// 부가적인 Option
				//---------------------------------------------------
				switch ((ITEMOPTION_TABLE::ITEMOPTION_PART)optionInfo.Part)
				{
				case ITEMOPTION_TABLE::PART_DEFENSE		: DV += optionInfo.PlusPoint; break;
				case ITEMOPTION_TABLE::PART_TOHIT		: TOHIT += optionInfo.PlusPoint; break;
				case ITEMOPTION_TABLE::PART_PROTECTION	: PV += optionInfo.PlusPoint; break;
					//case ITEMOPTION_INFO::PART_DAMAGE		: MinDAM += optionInfo.PlusPoint; 
					//											MaxDAM += optionInfo.PlusPoint; 
					//break;
					
				case ITEMOPTION_TABLE::PART_ATTACK_SPEED : AttackSpeed += optionInfo.PlusPoint; break;
					
				case ITEMOPTION_TABLE::PART_VISION :	SetItemLightSight(GetItemLightSight() + optionInfo.PlusPoint); break;
				}

				itr++;
			}
		}

		pGear->Next();
	}	

	// 2004, 9, 20, sobeit add start - 정령 레벨 올려주는 계급스킬이 있네..
	if((*g_pRankBonusTable)[RANK_BONUS_SALAMANDERS_KNOWLEDGE].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		ElementalFire ++;
	if((*g_pRankBonusTable)[RANK_BONUS_UNDINES_KNOWLEDGE].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		ElementalWater ++;
	if((*g_pRankBonusTable)[RANK_BONUS_GNOMES_KNOWLEDGE].GetStatus() == RankBonusInfo::STATUS_LEARNED)
		ElementalEarth ++;
	// 2004, 9, 20, sobeit add end
	// 정령 수치 지정
	SetStatus(MODIFY_ELEMENTAL_FIRE, ElementalFire);
	SetStatus(MODIFY_ELEMENTAL_WATER, ElementalWater);
	SetStatus(MODIFY_ELEMENTAL_EARTH, ElementalEarth);
	SetStatus(MODIFY_ELEMENTAL_WIND, ElementalWind);
	if(IsOusters())
		g_pSkillAvailable->SetAvailableSkills();

	//-----------------------------------------------------------------
	// 무기 속도 지정
	//-----------------------------------------------------------------
	SetStatus(MODIFY_ATTACK_SPEED, AttackSpeed);

	//-----------------------------------------------------------------
	// DAM, AC, TOHI, CC option만 적용시킨다.
	//-----------------------------------------------------------------
	// 2001.9.28 제거
	//SetStatus(MODIFY_DEFENSE, DV);
	//SetStatus(MODIFY_PROTECTION, PV);
	//SetStatus(MODIFY_TOHIT, TOHIT);
//	SetStatus(MODIFY_CARRYWEIGHT, CC);			

	//-----------------------------------------------------------------
	// DAM만 따로 체크.. 냠냠..
	//-----------------------------------------------------------------	
//	m_Status[MODIFY_MIN_DAMAGE] = MinDAM; 
//	m_Status[MODIFY_MAX_DAMAGE] = MaxDAM; 
	int maxDAM, minDAM;
	int maxSilverDAM = 0, minSilverDAM = 0;

	//-----------------------------------------------------------------
	// Slayer인 경우 --> 무기 damage 적용
	//-----------------------------------------------------------------
	switch(GetRace())
	{
		case RACE_SLAYER:
		{
			const MItem* pWeapon = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );

			if (pWeapon==NULL || !pWeapon->IsAffectStatus())
			{
				minDAM = MinDAM;
				maxDAM = MaxDAM;
			}
			else //if (pWeapon->IsAffectStatus())
			{
				minDAM = MinDAM + pWeapon->GetMinDamage();
				maxDAM = MaxDAM + pWeapon->GetMaxDamage();		

				//-----------------------------------------------------------------
				// 십자가나 메이스인 경우  : 은데미지 +10%
				//-----------------------------------------------------------------
				if (pWeapon->GetItemClass()==ITEM_CLASS_MACE
					|| pWeapon->GetItemClass()==ITEM_CLASS_CROSS)
				{
					minSilverDAM = pWeapon->GetMinDamage() / 10;
					maxSilverDAM = pWeapon->GetMaxDamage() / 10;
				}

				//-----------------------------------------------------------------
				// 은도금이 되어 있는 경우 : 은데미지 +10%
				// 총인 경우 은총알 체크
				//-----------------------------------------------------------------
				if (!pWeapon->IsGunItem() && pWeapon->GetSilver() > 0
					|| pWeapon->IsGunItem() 
						&& g_pCurrentMagazine!=NULL 
						&& g_pCurrentMagazine->GetItemType()>=8
						&& g_pCurrentMagazine->GetItemType()<=15)
				{
					minSilverDAM += pWeapon->GetMinDamage() / 10;
					maxSilverDAM += pWeapon->GetMaxDamage() / 10;
				}
			}
		}
	break;

	case RACE_VAMPIRE:
		//-----------------------------------------------------------------
		// 뱀파이어..
		//-----------------------------------------------------------------
		{
			const MItem* pWeapon = g_pVampireGear->GetItem( MVampireGear::GEAR_VAMPIRE_RIGHTHAND );

			if (pWeapon==NULL || !pWeapon->IsAffectStatus())
			{
				minDAM = MinDAM;
				maxDAM = MaxDAM;
			}
			else //if (pWeapon->IsAffectStatus())
			{
				minDAM = MinDAM + pWeapon->GetMinDamage();
				maxDAM = MaxDAM + pWeapon->GetMaxDamage();		

			}
		}
		break;

	case RACE_OUSTERS:
		//-----------------------------------------------------------------
		// 아우스터즈..
		//-----------------------------------------------------------------
		{
			const MItem* pWeapon = g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_RIGHTHAND );
			
			if (pWeapon==NULL || !pWeapon->IsAffectStatus() || pWeapon->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET)
			{
				minDAM = MinDAM;
				maxDAM = MaxDAM;
			}
			else //if (pWeapon->IsAffectStatus())
			{
				minDAM = MinDAM + pWeapon->GetMinDamage();
				maxDAM = MaxDAM + pWeapon->GetMaxDamage();		
				
			}
		}
		break;
	}

	UI_SetCharInfoDAM( maxDAM, minDAM );
	
	g_char_slot_ingame.SILVER_DAM2	= minSilverDAM;
	g_char_slot_ingame.SILVER_DAM	= maxSilverDAM;

	// 게임 중에는 서버에서 보내주기 때문에..
	if (g_Mode == MODE_WAIT_PCLIST)
	{
		UI_SetCharInfoDefense( DV );	
		UI_SetCharInfoProtection( PV );	
		UI_SetCharInfoTOHIT( TOHIT );
	}
	else
	{
		UI_SetCharInfoDefense( GetDefense() );	
		UI_SetCharInfoProtection( GetProtection() );	
		UI_SetCharInfoTOHIT( GetTOHIT() );
	}
}

//----------------------------------------------------------------------
// Fast Move Position
//----------------------------------------------------------------------
// (m_X, m_Y) --> (x, y)
//----------------------------------------------------------------------
bool
MPlayer::FastMovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, bool server)
{	
	if (g_pTopView!=NULL)
	{
		g_pTopView->SetSelectedSectorNULL();
	}

	if (MCreature::FastMovePosition( x, y ))
	{
		if(server == true)
			SetServerPosition( x, y);
		// 2001.8.8  계속 추적하게 해보기 위한 주석처리
		// 그러나.. 뭔가 문제가 있어서.. 다시... - -;;
		//m_nUsedActionInfo = ACTIONINFO_NULL;
		//m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;

		///*
		m_fNextTrace = FLAG_TRACE_NULL;

		m_bKeepTraceCreature = false;
		m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;

		UnSetRepeatAction();
		//*/

		// 다 검증됐다고 본다.
		ResetSendMove();

		// 2001.8.10에 추가
		m_nUsedActionInfo = ACTIONINFO_NULL;

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// KnockBackPosition
//----------------------------------------------------------------------
bool	
MPlayer::KnockBackPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	if (MCreature::KnockBackPosition( x, y ))
	{
		// 2001.8.8  계속 추적하게 해보기 위한 주석처리
		// 그러나.. 뭔가 문제가 있어서.. 다시... - -;;
		//m_nUsedActionInfo = ACTIONINFO_NULL;
		//m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;

		///*
		//m_fNextTrace = FLAG_TRACE_NULL;

		//m_bKeepTraceCreature = false;
		//m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;

		//UnSetRepeatAction();
		//*/

		// 다 검증됐다고 본다.
		ResetSendMove();

		// 2001.8.10에 추가
		m_nUsedActionInfo = ACTIONINFO_NULL;

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Find Enemy
//----------------------------------------------------------------------
// 근처의 적을 찾는다.
// 적이 없다면 OBJECTID_NULL을 return한다.
//----------------------------------------------------------------------
int		
MPlayer::FindEnemy()
{
	//---------------------------------------------------------------
	// 현재 L, R Button이 눌린 상태로 LockMode의 행동을 판단한다.
	// 별루 안 깔끔하지만.. ㅋㅋ..
	//---------------------------------------------------------------
	int bBasicAction = g_bLButtonDown;
	int bSpecialAction = g_bRButtonDown;
	int actionDistance = 0;

	//---------------------------------------------------------------
	// 기본 행동
	//-------------------------------------------------------------
	if (bBasicAction)
	{
		actionDistance = m_BasicAttackDistance;		
	}
	//-------------------------------------------------------------
	// 특수 행동
	//-------------------------------------------------------------
	else if (bSpecialAction)
	{
		// 특수 기술이 설정되지 않은 경우
		if (m_nSpecialActionInfo==ACTIONINFO_NULL)
		{
			return NULL;
		}

		actionDistance = GetActionInfoRange( m_nSpecialActionInfo );
	}

	//-------------------------------------------------------------
	// Zone의 모든 Creature를 검색한다.
	//-------------------------------------------------------------
	int	numCreature = g_pZone->GetCreatureNumber();

	MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();
		
	int targetCreatureID = OBJECTID_NULL;

	for (int i=0; i<numCreature; i++)
	{
		MCreature* pCreature = iCreature->second;

		//--------------------------------------------------
		// NPC도 아니고, 내가 아닌 경우..
		//--------------------------------------------------
		if (!pCreature->IsNPC() 
			&& pCreature->GetID()!=m_ID)
		{
			int tx = pCreature->GetServerX();
			int ty = pCreature->GetServerY();

			//--------------------------------------------------
			// 적이고..
			// 행동 가능 거리이면...
			//--------------------------------------------------
			if (g_pObjectSelector->CanAttack(pCreature)
				&& max(abs(tx-m_X), abs(ty-m_Y)) <= actionDistance)
				//&& g_pObjectSelector->IsAblePKbyZone(pCreature))					
			{
				//--------------------------------------------------
				// 처음 선택된 캐릭터인 경우
				//--------------------------------------------------
				if (targetCreatureID==OBJECTID_NULL)
				{
					targetCreatureID = pCreature->GetID();
				}
				//--------------------------------------------------
				// 처음 선택된 캐릭터가 아닌 경우
				//--------------------------------------------------
				else
				{
					//--------------------------------------------------
					// 반반의 확률로 목표를 다시 설정한다.
					//--------------------------------------------------
					if (rand()%2)
					{
						targetCreatureID = pCreature->GetID();	
					}
					//--------------------------------------------------
					// 다시 설정되지 않은 경우
					// 기존에 설정된 캐릭터를 선택한다.
					//--------------------------------------------------
					else
					{
						return targetCreatureID;
					}
				}
			}
		}

		iCreature++;
	}	

	return targetCreatureID;
}

//----------------------------------------------------------------------
// Stop Blood Drain
//----------------------------------------------------------------------
// m_bStopBloodDrain이면 이걸 호출해야 한다.
//----------------------------------------------------------------------
void	
MPlayer::StopBloodDrain()
{
	if (m_Action==ACTION_VAMPIRE_DRAIN
		&& IsVampire()
		&& m_ActionCountMax>60)
	{
		if (m_ActionCount>9 && m_ActionCount<60)
		{
			m_ActionCount = 60;

			// delay 다시 설정
			//m_DelayTime	= g_CurrentTime + (m_ActionCountMax-m_ActionCount)*60;		
			
			UnSetStopBloodDrain();
//			char TempBuffer[128];
//			sprintf(TempBuffer, "StopBD: D:%d, m:%d, C:%d", m_DelayTime,m_bStopBloodDrain, m_ActionCount);
//			g_pSystemMessage->Add(TempBuffer);

			//DEBUG_ADD_FORMAT("[Player] StopBloodDrain1(%d)", m_bStopBloodDrain);

		}	
	}
	else
	{
		//UnSetStopBloodDrain();
		//DEBUG_ADD_FORMAT("[Player] StopBloodDrain2(%d), action=%d(%d)", m_bStopBloodDrain, m_Action, m_ActionCountMax);
	}
}

//----------------------------------------------------------------------
// Stop Absorb Soul
//----------------------------------------------------------------------
// m_bStopAbsorbSoul이면 이걸 호출해야 한다.
//----------------------------------------------------------------------
void	
MPlayer::StopAbsorbSoul()
{
	if (m_Action==ACTION_OUSTERS_DRAIN
		&& IsOusters()
		&& m_ActionCountMax>30)
	{
		if (m_ActionCount>9 && m_ActionCount<30)
		{
			m_ActionCount = 30;
			
			ATTACHEFFECT_LIST::iterator itr = m_listEffect.begin();
			ATTACHEFFECT_LIST::iterator endItr = m_listEffect.end();
			
			while(itr != endItr)
			{
				MAttachEffect*	pEffect = *itr;
				
				if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_ABSORB_SOUL)
				{
					bool bUseEffectSprite = pEffect->IsEffectSprite();
					
					// flag제거
					if (bUseEffectSprite)
					{
						m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
					}
					
					delete pEffect;
					m_listEffect.erase(itr);
					break;
				}
				itr++;
			}
			
			UnSetStopAbsorbSoul();
		}	
	}
	else
	{
		//UnSetStopBloodDrain();
	}
}

//----------------------------------------------------------------------
// Check In Darkness
//----------------------------------------------------------------------
// darkness가 나타나고/사라지는 순간
//            player가 움직이는 순간을 체크하면 된다.
//----------------------------------------------------------------------
void
MPlayer::CheckInDarkness()
{
	if (g_pZone!=NULL && 
		( !IsVampire() && (!HasEffectStatus( EFFECTSTATUS_LIGHTNESS)|| g_pZone->GetID() == 3001 ) || 
		g_pZone->GetID() == 3001 && IsVampire())
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		&& !HasEffectStatus( EFFECTSTATUS_INSTALL_TURRET)
		)
	{
		//-------------------------------------------------------
		// Darkness에 들어왔는지 체크
		//-------------------------------------------------------
		// 서버 좌표보다는 현재 좌표가 보기에 좋다.. ㅋㅋ..
		const MSector& sector = g_pZone->GetSector( m_X, m_Y );

		if (sector.HasDarkness())
		{
			// darkness에 있는 경우
			PlaceInDarkness();
		}
		else
		{		
			// darkness는 사라졌다.
			PlaceNotInDarkness();
		}
	}
	else
	{
		PlaceNotInDarkness();
	}
}

//----------------------------------------------------------------------
// Set SpecialActionInfo
//----------------------------------------------------------------------
void			
MPlayer::SetSpecialActionInfo( TYPE_ACTIONINFO n )	
{ 
	if (g_pActionInfoTable!=NULL 
		&& n<g_pActionInfoTable->GetMinResultActionInfo())
	{
		m_nSpecialActionInfo = n; 

		// 반복 중에는..
		// 기술의 Target이 바뀌는 경우가 있다.
		if (IsRepeatAction())
		{
			if (m_fTraceBuffer != FLAG_TRACE_NULL
				// 기본 동작 중에는 관계없다.
				&& !(m_fTraceBuffer & FLAG_TRACE_CREATURE_BASIC)
				&& !(m_fTraceBuffer & FLAG_TRACE_SECTOR_BASIC)
				&& !(m_fTraceBuffer & FLAG_TRACE_ITEM)
				&& !(m_fTraceBuffer & FLAG_TRACE_INTERACTIONOBJECT)
				&& !(m_fTraceBuffer & FLAG_TRACE_EFFECT)
				)
			{			
				// 기술 사용중인 경우..
				BOOL bOther = ((*g_pActionInfoTable)[n].IsTargetOther());
				BOOL bSelf = ((*g_pActionInfoTable)[n].IsTargetSelf());
				BOOL bZone = ((*g_pActionInfoTable)[n].IsTargetZone());
				BOOL bItem = ((*g_pActionInfoTable)[n].IsTargetItem());

				// 정상적인 경우...
				if (((m_fTraceBuffer & FLAG_TRACE_SELF) && bSelf)
					|| ((m_fTraceBuffer & FLAG_TRACE_CREATURE_SPECIAL) && bOther)
					|| ((m_fTraceBuffer & FLAG_TRACE_SECTOR_SPECIAL) && bZone)
					|| ((m_fTraceBuffer & FLAG_TRACE_INVENTORY) && bItem)
					)
				{				
				}
				// 비정상적인 경우.. target 설정을 바꿔야 한다.
				else
				{
					if (bOther)
					{
						m_fTraceBuffer = FLAG_TRACE_CREATURE_SPECIAL;
					}
					else if (bSelf)
					{
						m_fTraceBuffer = FLAG_TRACE_SELF;
					}
					else if (bZone)
					{
						m_fTraceBuffer = FLAG_TRACE_SECTOR_SPECIAL;
					}
					else if (bItem)
					{
						m_fTraceBuffer = FLAG_TRACE_INVENTORY;
					}
				}
			}		
		}
	}
	
}

//----------------------------------------------------------------------
// Is In SafeSector
//----------------------------------------------------------------------
BOOL
MPlayer::IsInSafeSector() const
{
	if(g_pWarManager->IsExist(m_pZone->GetID()))
		return FALSE;

	if (m_pZone!=NULL)
	{
		if(m_pZone->GetSector(m_X, m_Y).IsSafeComplete() || g_bZoneSafe)
			return 2;

		if (IsSlayer())
		{
			return (m_pZone->GetSector(m_X, m_Y).IsSafeSlayer())?1:0;
		}
		else
		if (IsVampire())
		{
			return (m_pZone->GetSector(m_X, m_Y).IsSafeVampire())?1:0;
		} else
		if (IsOusters() )
		{
			return (m_pZone->GetSector(m_X, m_Y).IsSafeOusters())?1:0;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
// Soul Chain File
//-------------------------------------------------------------------------
void	
MPlayer::SetCannotTrace()
{
	gC_vs_ui.SetCannotTrace();
}

void	MPlayer::SetTimeLightSight(char s)
{
	m_TimeLightSight = s; 
	CalculateLightSight();
}

void	MPlayer::SetItemLightSight(int s)
{
	m_ItemLightSight = s; 
	CalculateLightSight();
}

void	MPlayer::CalculateLightSight()
{
//	CalculateSight();
	m_LightSight = GetTimeLightSight() + GetItemLightSight();

	// [새기술6] observing eye가 걸려있으면 시야가 넓어진다.
	if (m_bEffectStatus[EFFECTSTATUS_OBSERVING_EYE])
	{
		m_LightSight +=  1 + (*g_pSkillInfoTable)[SKILL_OBSERVING_EYE].GetLevel() / 50;
	}
	// Pet Bonus
	if( GetPetID() != OBJECTID_NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetPetID());
		if(pFakeCreature != NULL)
		{
			MPetItem *pPetItem = pFakeCreature->GetPetItem();
			if(pPetItem != NULL)
			{
				ITEMOPTION_TABLE::ITEMOPTION_PART attr = (ITEMOPTION_TABLE::ITEMOPTION_PART)pPetItem->GetEnchantLevel();
				if(attr == ITEMOPTION_TABLE::PART_VISION)
				{
					m_LightSight += pPetItem->GetSilver();
				}
			}
		}
	}
	
	if(g_pSkillAvailable->IsEnableSkill(SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI))
	{
		m_LightSight += 5;
	}
	
	if(g_pSkillAvailable->IsEnableSkill(SKILL_SWEEPER_BONUS_12))
	{
		m_LightSight += 5;
	}

	if(IsFlyingCreature())
	{
		m_LightSight += 3;
	}
	// 2004, 6, 24, sobeit add start
	if(HasEffectStatus(EFFECTSTATUS_BLINDNESS))
	{
		//m_LightSight = 1;
		PlaceInDarkness(true);
	}
	// 2004, 6, 24, sobeit add end
}

void	MPlayer::SetUndergroundCreature()
{
	MCreature::SetUndergroundCreature();
	CalculateLightSight();
}

void	MPlayer::SetGroundCreature()
{
	MCreature::SetGroundCreature();
	CalculateLightSight();
}

void	MPlayer::SetFlyingCreature()
{
	MCreature::SetFlyingCreature();
	CalculateLightSight();
}

void	MPlayer::CalculateSight()
{
	// -_----; 시야 세팅 ㅋㅋㅋ 버그가 많아서 걍 맨들자

	//-----------------------------------------------------------------
	// item 착용한거에 따라서...
	//-----------------------------------------------------------------
	MItemManager* pGear = GetGear();

	//-----------------------------------------------------------------
	// Slayer인 경우
	//-----------------------------------------------------------------	
	if(pGear == NULL )
		return;

	SetItemLightSight(0);

	pGear->SetBegin();

	while (pGear->IsNotEnd())
	{
		const MItem* pItem = pGear->Get();

		//----------------------------------------------------------
		// 수치가 적용되는 item인 경우만..
		//----------------------------------------------------------
		if (pItem->IsAffectStatus())
		{	
			const std::list<TYPE_ITEM_OPTION>& optionList = pItem->GetItemOptionList();
			std::list<TYPE_ITEM_OPTION>::const_iterator optionListItr = optionList.begin();
			while(optionListItr != optionList.end())
			{				
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[*optionListItr];
				//---------------------------------------------------
				// 부가적인 Option
				//---------------------------------------------------
				switch ((ITEMOPTION_TABLE::ITEMOPTION_PART)optionInfo.Part)
				{
				case ITEMOPTION_TABLE::PART_VISION :	
					SetItemLightSight(GetItemLightSight() + optionInfo.PlusPoint); 
					break;
				}
				optionListItr++;
			}

			std::list<TYPE_ITEM_OPTION>::const_iterator itr = pItem->GetItemDefaultOptionList().begin();

			while(itr != pItem->GetItemDefaultOptionList().end())
			{
				TYPE_ITEM_OPTION option = *itr;
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[option];
				//---------------------------------------------------
				// 부가적인 Option
				//---------------------------------------------------
				switch ((ITEMOPTION_TABLE::ITEMOPTION_PART)optionInfo.Part)
				{
				case ITEMOPTION_TABLE::PART_VISION :	
					SetItemLightSight(GetItemLightSight() + optionInfo.PlusPoint); 
					break;
				}
				itr++;
			}
		}
		pGear->Next();
	}
	if( HasEffectStatus(EFFECTSTATUS_LIGHTNESS) 
		|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		|| g_bLight
#endif
		)
	{
		// 최강~~
		SetItemLightSight( GetItemLightSight() + 15 );
	} else
	{
		if( HasEffectStatus( EFFECTSTATUS_FLARE ) && IsVampire() || 
			HasEffectStatus( EFFECTSTATUS_YELLOW_POISON ) && IsSlayer() ||
			HasEffectStatus( EFFECTSTATUS_YELLOW_POISON_TO_CREATURE ) )//||
//			HasEffectStatus( EFFECTSTATUS_BLINDNESS ))
			SetItemLightSight( 1 );
	}
}
// 2004, 8, 30, sobeit add start - sweep vice test
void
MPlayer::UpdateSweepVice()
{
	if(GetSweepViewValue())	// 스윕 바이스 중일떄
	{
		//if(IsDead() || m_nUsedActionInfo != SKILL_SWEEP_VICE_1 || m_SweepViceX == SECTORPOSITION_NULL ||	m_SweepViceY == SECTORPOSITION_NULL	)
		if(IsDead() ||!IsSlayer() || m_SweepViceX == SECTORPOSITION_NULL ||	m_SweepViceY == SECTORPOSITION_NULL	)
		{
			g_pZone->RemoveSwapViceType();
			SetSweepViewValue(0);
			return;
		}
		POINT CurrentPos = g_pTopView->GetSelectedSector(g_x, g_y);
		int TempVal = 0;
		int Distance = max(abs(m_TempSelectPosX - CurrentPos.x),abs(m_TempSelectPosY - CurrentPos.y));

		if(Distance<2)
		{
			TempVal = 1;
			if(GetSweepViewValue()!=TempVal)
			{
				g_pZone->ChangeSwapViceType(m_SweepViceX, m_SweepViceY, EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_1);
				PlaySound( SOUND_SLAYER_SWEEPVICE_RESIZE );
			}
				
		}
		else if(Distance<3)
		{
			TempVal = 2;
			if(GetSweepViewValue()!=TempVal)
			{
				g_pZone->ChangeSwapViceType(m_SweepViceX, m_SweepViceY, EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_2);
				PlaySound( SOUND_SLAYER_SWEEPVICE_RESIZE );
			}
		}
		else
		{
			TempVal = 3;
			if(GetSweepViewValue()!=TempVal)
			{
				g_pZone->ChangeSwapViceType(m_SweepViceX, m_SweepViceY, EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_3);
				PlaySound( SOUND_SLAYER_SWEEPVICE_RESIZE );
			}
		}
		SetSweepViewValue(TempVal);
	}
}

bool 
MPlayer::CheckRbuttonUpSkill()
{
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
	{
		m_fNextTrace = FLAG_TRACE_NULL;
		UnSetRepeatAction();
		InstallTurretStopAttack();
	}
	if(GetSweepViewValue())
	{
		g_pZone->RemoveSwapViceType();
		m_nUsedActionInfo = SKILL_SWEEP_VICE_1;
		ActionEffect();
		SetSweepViewValue(0);
		m_nUsedActionInfo = SKILL_SWEEP_VICE_1;
		m_TempSelectPosX = SECTORPOSITION_NULL;
		m_TempSelectPosY = SECTORPOSITION_NULL;
		m_SweepViceX = SECTORPOSITION_NULL;
		m_SweepViceY = SECTORPOSITION_NULL;

		return true;
	}
//	else if(IsBurningSol())
//	{
//		return true;
//	}
	return false;
}
// 2004, 8, 30, sobeit add end - sweep vice test



bool MPlayer::UpdateBurningSol()
{
	if(m_bBurningSol)
	{
		if(IsDead()||!IsSlayer())
		{
			SetBurningSol(0);
			StopBurningSol();
			SetStop();
			m_nUsedActionInfo = ACTIONINFO_NULL;
			m_fNextTrace = FLAG_TRACE_NULL;
			UnSetRepeatAction();
		//	m_ActionCount = m_ActionCountMax;
			SetAction(ACTION_DIE);
			return false ;
		}
		else if(HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
			|| m_MoveDevice == MOVE_DEVICE_RIDE)
		{
			SetBurningSol(0);
			StopBurningSol();
			//SetStop();
			m_nUsedActionInfo = ACTIONINFO_NULL;
			m_fNextTrace = FLAG_TRACE_NULL;
			UnSetRepeatAction();
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4);
			return false;
		}
		if(m_TempSelectPosX == SECTORPOSITION_NULL || m_TempSelectPosY == SECTORPOSITION_NULL)
		{
			//SetBurningSol(0);
			StopBurningSol();
			m_nUsedActionInfo = ACTIONINFO_NULL;
			m_fNextTrace = FLAG_TRACE_NULL;
//			UnSetRepeatAction();
			//m_ActionCount = m_ActionCountMax;
			SetAction(ACTION_STAND);
			return false;
		}
		int bCheckFrame = 0;
		if(!IsAdvancementClass())
			bCheckFrame = IsMale()?7:11;
		else
			bCheckFrame = 7;

		m_bBurningSolCount++;

		//if(g_bRButtonDown)
		if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4))
		{
			if(m_bBurningSolCount>=bCheckFrame)
				m_bBurningSolCount = bCheckFrame;
		}
		POINT  TempPos = g_pTopView->GetSelectedSector(g_x, g_y);
		m_TempSelectPosX = TempPos.x;
		m_TempSelectPosY = TempPos.y;
		m_Direction = MTopView::GetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY(),m_TempSelectPosX, m_TempSelectPosY);
		ChangeNearDirection();
		//else
		if(!g_bRButtonDown)
		{
			if(m_bBurningSolCount>=bCheckFrame  )
			{
//				POINT  TempPos = g_pTopView->GetSelectedSector(g_x, g_y);
//				m_TempSelectPosX = TempPos.x;
//				m_TempSelectPosY = TempPos.y;
//				m_CurrentDirection = MTopView::GetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY(),m_TempSelectPosX, m_TempSelectPosY);
//				

				bool ShowLunchEffect = true;
				int TempDistance = GetActionInfoRange(SKILL_BURNING_SOUL_LAUNCH);
				if (max(abs(m_X-m_TempSelectPosX), abs(m_Y-m_TempSelectPosY)) > TempDistance)
				{
					ShowLunchEffect = false;
				}
				
				if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
				{
					if(ShowLunchEffect)
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_BURNING_SOL_1,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
								m_TempSelectPosX,m_TempSelectPosY, 0, 10, NULL, false);	
					RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
				}
				else if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
				{
					if(ShowLunchEffect)
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_BURNING_SOL_2,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
								m_TempSelectPosX, m_TempSelectPosY, 0, 10, NULL, false);	
					RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
				}
				else if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
				{
					if(ShowLunchEffect)
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_BURNING_SOL_3,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
								m_TempSelectPosX, m_TempSelectPosY, 0, 10, NULL, false);	
					RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
				}
				else if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4))
				{
					if(ShowLunchEffect)
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_BURNING_SOL_4,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
								m_TempSelectPosX, m_TempSelectPosY, 0, 10, NULL, false);	
					RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4);
				}

				m_nUsedActionInfo = SKILL_BURNING_SOUL_LAUNCH;
				m_fTraceBuffer = FLAG_TRACE_SECTOR_SPECIAL;
				ActionToSendPacket();
				m_nNextUsedActionInfo = ACTIONINFO_NULL;
				//SetBurningSol(0);
				StopBurningSol();
				
				UnSetRepeatAction();
				m_TempSelectPosX = SECTORPOSITION_NULL;
				m_TempSelectPosY = SECTORPOSITION_NULL;
			}
		}
		m_ActionCount = m_bBurningSolCount;
			
		return true;
	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateBurningSol] ActionCount  : %d", m_bBurningSolCount);
	#endif
	}
	return false;
}


bool MPlayer::UpDateInstallTurret()
{
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
	{
		if(!IsNotDelay())
		{
			m_Action = ACTION_STAND;
			//m_CurrentDirection = 2; 
			return true;
		}
		if(IsAlive() && (m_Action == ACTION_STAND))
		{
			if (m_bInstallTurretDirect != m_CurrentDirection)
			{
				if( ((m_bInstallTurretDirect - m_CurrentDirection + 8)%8) < 4 )
				{
					m_bInstallTurretCount = ((m_bInstallTurretCount-1)+16)%16;
				}
				else
				{
					m_bInstallTurretCount = (m_bInstallTurretCount+1)%16;
				}

				if(!(m_bInstallTurretCount&0x01))
					m_bInstallTurretDirect = ((m_bInstallTurretCount /2) +2 )%8;
			}
			return true;
		}

		
		SetInstallTurretDirect(m_CurrentDirection);
		SetInstallTurretCount(((m_CurrentDirection+6)%8)*2);
	}
	return false;
}
// 2005, 1, 4, sobeit add start
void	
MPlayer::UseWildWolf_Corpse(MItem *pItem)
{
	if(pItem == NULL) return;
	m_TraceDistance = GetActionInfoRange(SKILL_WILD_WOLF);
	if (max(abs(m_X-pItem->GetX()), abs(m_Y-pItem->GetY())) <= m_TraceDistance && g_pSkillAvailable->IsEnableSkill( SKILL_WILD_WOLF ))
	{
		SetAction( ACTION_MAGIC );
		CGSkillToObject _CGSkillToObject;
		_CGSkillToObject.setSkillType( SKILL_WILD_WOLF );
		_CGSkillToObject.setCEffectID( 0 );			
		_CGSkillToObject.setTargetObjectID( pItem->GetID() );
		g_pSocket->sendPacket( &_CGSkillToObject );				
	}
}
// 2005, 1, 4, sobeit add end
