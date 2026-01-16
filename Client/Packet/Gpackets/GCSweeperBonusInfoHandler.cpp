//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSweeperBonusInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSweeperBonusInfo.h"
#include "ClientDef.h"
#include "MSkillManager.h"
#include "VS_UI.h" 


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void GCSweeperBonusInfoHandler::execute ( GCSweeperBonusInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	if( g_pPlayer == NULL || g_pSkillAvailable == NULL )
		return;
	
	SweeperBonusInfo *pInfo = pPacket->popFrontSweeperBonusInfoList();

	int i=0;

	while( pInfo != NULL )
	{
		if( g_pPlayer->GetRace() == pInfo->getRace() )
		{			
			switch( g_pPlayer->GetRace() )
			{
			case RACE_SLAYER :
				{
					int allstatsum = g_char_slot_ingame.STR_PURE + g_char_slot_ingame.DEX_PURE + g_char_slot_ingame.INT_PURE;
					
					switch( i )
					{
					case 0 :
					case 1 :
					case 2 :
						if( allstatsum >= 1 && allstatsum <= 150 )
							g_abSweeperBonusSkills[i] = true;							
						break;

					case 3 :
					case 4 :
					case 5 :
						if( allstatsum >= 151 && allstatsum <= 210 )
							g_abSweeperBonusSkills[i] = true;
						break;

					case 6 :
					case 7 :
					case 8 :
						if( allstatsum >= 211 && allstatsum <= 260 )
							g_abSweeperBonusSkills[i] = true;
						break;

					case 9 :
					case 10 :
					case 11 :
						if (allstatsum >= 261 && allstatsum <= 300 )
							g_abSweeperBonusSkills[i] = true;
						break;
					}
				}
				break;
			
			case RACE_VAMPIRE :
			case RACE_OUSTERS :
				{
					int myLevel = g_pPlayer->GetLEVEL();
					
					switch( i )
					{
					case 0 :
					case 1 :
					case 2 :
						if( myLevel >= 1 && myLevel <= 30 )
							g_abSweeperBonusSkills[i] = true;
						break;

					case 3 :
					case 4 :
					case 5 :
						if( myLevel >= 31 && myLevel <= 50 )
							g_abSweeperBonusSkills[i] = true;
						break;

					case 6 :
					case 7 :
					case 8 :
						if( myLevel >= 51 && myLevel <= 70 )
							g_abSweeperBonusSkills[i] = true;
						break;

					case 9 :
					case 10 :
					case 11 :
						if( myLevel >= 71 && myLevel <= 90 )
							g_abSweeperBonusSkills[i] = true;
						break;
					}
				}
				break;	
			
			default :
				g_abSweeperBonusSkills[i] = false;
				break;
			}			
		}
		else
			g_abSweeperBonusSkills[i] = false;
		
		delete pInfo;

		pInfo = pPacket->popFrontSweeperBonusInfoList();		
		i++;
	}

	g_pSkillAvailable->SetAvailableSkills();
}

