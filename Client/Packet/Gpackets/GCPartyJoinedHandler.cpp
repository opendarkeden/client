//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyJoinedHandler.cpp
// Written By  : 김성민
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyJoined.h"
#include "MParty.h"
#include "UserInformation.h"
#include "RequestClientPlayerManager.h"
#include "ProfileManager.h"
#include "RequestUserManager.h"
#include "TempInformation.h"
					
#include "..\Cpackets/CGRequestIP.h"
#include "ClientDef.h"
#include "UIFunction.h"

extern int	UI_GetFaceStyle(bool bMale, int faceStyle);
extern void	SendCharacterInfoToParty();
extern void	SendStatusInfoToParty();
extern void	SendPositionInfoToParty();

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;

void GCPartyJoinedHandler::execute (GCPartyJoined * pPacket , Player * pPlayer)
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	if (g_pPlayer==NULL
		|| g_pParty==NULL
		|| g_pUserInformation==NULL
		|| g_pTempInformation==NULL
		|| g_pZone==NULL
		|| g_pProfileManager==NULL)
	{
		DEBUG_ADD("GCPartyJoinedHandler Failed");
		return;
	}

	int previousSize = g_pParty->GetSize();
	
	//g_pParty->Release();
	g_pParty->UnSetPlayerParty();
	MParty* pOldParty = g_pParty;

	g_pParty = new MParty;
	g_pParty->SetJoinTime( pOldParty->GetJoinTime() );
	

	int count = pPacket->getMemberInfoCount();

	struct in_addr sa;
				
	for (int i=0; i<count; i++)
	{
		PARTY_MEMBER_INFO* pInfo = pPacket->popMemberInfo();

		if (pInfo!=NULL)
		{
			if (g_pUserInformation->CharacterID!=pInfo->name.c_str())
			{			
				PARTY_INFO* pNewInfo = new PARTY_INFO;

				pNewInfo->Name		= pInfo->name.c_str();
				pNewInfo->bMale		= (pInfo->sex==1);				
				pNewInfo->hairStyle	= UI_GetFaceStyle(pNewInfo->bMale, pInfo->hair_style);
				
				sa.S_un.S_addr = pInfo->ip;
				pNewInfo->IP		= inet_ntoa( sa );

				//---------------------------------------------------------
				// 얼굴 받아오기
				//---------------------------------------------------------
				const char* pName = pInfo->name.c_str();

				if (g_pProfileManager!=NULL 
					&& g_pRequestUserManager!=NULL
					&& g_pRequestClientPlayerManager!=NULL)
					//&& !g_pProfileManager->HasProfile(pName)
					//&& !g_pProfileManager->HasProfileNULL(pName)
					//&& !g_pRequestUserManager->HasRequestingUser(pName)
					//&& !g_pRequestClientPlayerManager->HasConnection(pName)
					//&& !g_pRequestClientPlayerManager->HasTryingConnection(pName))
				{
					g_pProfileManager->RequestProfile(pName);
				}

				//---------------------------------------------------------
				// 이전 파티 정보에 있는지 ..
				//---------------------------------------------------------
				PARTY_INFO* pOldInfo = pOldParty->GetMemberInfo( pInfo->name.c_str() );
					
				if (pOldInfo!=NULL)
				{
					pNewInfo->HP	= pOldInfo->HP;
					pNewInfo->MaxHP = pOldInfo->MaxHP;
					pNewInfo->zoneID = pOldInfo->zoneID;
					pNewInfo->zoneX = pOldInfo->zoneX;
					pNewInfo->zoneY = pOldInfo->zoneY;
					pNewInfo->guildID = pOldInfo->guildID;
				}			

				//---------------------------------------------------------
				// 현재 zone에 있는지 체크
				//---------------------------------------------------------
				if (g_pZone!=NULL)
				{
					int creatureID = g_pZone->GetCreatureID( pInfo->name.c_str(), 1 );

					if (creatureID==OBJECTID_NULL)
					{
						pNewInfo->bInSight = false;						
						pNewInfo->zoneID = 60002;
					}
					else
					{
						MCreature* pCreature = g_pZone->GetCreature( creatureID );
						
						if (pCreature!=NULL)
						{
							pNewInfo->bInSight = true;

							pNewInfo->HP = pCreature->GetHP();
							pNewInfo->MaxHP = pCreature->GetMAX_HP();
							pNewInfo->zoneID = (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);
							pNewInfo->zoneX = pCreature->GetServerX();
							pNewInfo->zoneY = pCreature->GetServerY();
							pNewInfo->guildID = pCreature->GetGuildNumber();

							pCreature->SetPlayerParty();
						}
						else
						{
							pNewInfo->bInSight = false;
						}
					}
				}
				else
				{
					pNewInfo->bInSight = false;
					pNewInfo->zoneID = 60002;
				}

				if (!g_pParty->AddMember( pNewInfo ))
				{
					delete pNewInfo;
				}
			}

			delete pInfo;
		}
	}

	UI_ClosePartyCancel();
	UI_ClosePartyAsk();
	UI_ClosePartyRequest();

	if (previousSize!=g_pParty->GetSize())
	{
		UI_RunParty();
	}	

	// 이전에 아무도 없었는데 파티원들이 생긴다면
	// 내가 파티에 들어간거다.
	if (previousSize==0)
	{
		g_pParty->SetJoinTime();
	}

	//---------------------------------------------------------------
	// 내 정보를 보내준다.
	//---------------------------------------------------------------
	SendCharacterInfoToParty();
	SendPositionInfoToParty();
	SendStatusInfoToParty();

	delete pOldParty;

	//---------------------------------------------------------------
	// 파티원들에 대한 접속을 체크한다.
	//---------------------------------------------------------------
	/*
	int num = g_pParty->GetSize();

	for (i=0; i<num; i++)
	{
		PARTY_INFO*	pInfo = g_pParty->GetMemberInfo(i);

		if (pInfo!=NULL)
		{
			// 상대에 대한 접속이 없다면 IP를 요청해야 한다.
			if (!g_pRequestClientPlayerManager->HasConnection( pInfo->Name.GetString() ))
			{
				CGRequestIP _CGRequestIP;

				_CGRequestIP.setName( pInfo->Name.GetString() );

//				pPlayer->sendPacket( &_CGRequestIP );
				#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
					if (g_pGameMessage!=NULL)
						g_pGameMessage->AddFormat("RequestIP for %s", pInfo->Name.GetString());
				#endif
			}
		}
	}
	*/
	
#endif

	__END_CATCH
}
