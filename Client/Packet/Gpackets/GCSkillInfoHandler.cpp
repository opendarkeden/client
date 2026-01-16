//----------------------------------------------------------------------
//
// Filename    : GCSkillInfoHandler.cpp
// Written By  : elca
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCSkillInfo.h"
#include "ClientDef.h"
#include "MSkillManager.h"
#include "UserInformation.h"

//----------------------------------------------------------------------
// 클라이언트가 게임 서버로부터 GCSkillInfo 패킷을 받게 되면,
// 패킷 안의 데이터들을 클라이언트에 저장한 후, 데이터 로딩이
// 끝이 나면 게임 서버로 CGReady 패킷을 보내면 된다.
//----------------------------------------------------------------------
void GCSkillInfoHandler::execute ( GCSkillInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY


	g_pUserInformation->HasSkillRestore = false;
	g_pUserInformation->HasMagicGroundAttack = false;
	g_pUserInformation->HasMagicHallu = false;
	g_pUserInformation->HasMagicBloodyWarp = false;
	g_pUserInformation->HasMagicBloodySnake = false;
				
	
	//--------------------------------------------------
	// 각각에 domain에 대한 정보를 설정한다.
	//--------------------------------------------------
	int domainNum = pPacket->getListNum();

	int pcType = pPacket->getPCType();

	g_pSkillManager->InitSkillList();
	
	for (int d=0; d<domainNum; d++)
	{
		PCSkillInfo* pSkillInfo = pPacket->popFrontListElement();

		//--------------------------------------------------
		// 종족에 따라서...
		//--------------------------------------------------
		int i;

		if (pSkillInfo!=NULL)
		{
			switch (pcType)
			{
				//--------------------------------------------------
				//
				//						Slayer
				//
				//--------------------------------------------------
				case PC_SLAYER :
				{
					SlayerSkillInfo* pSlayerSkillInfo = (SlayerSkillInfo*)pSkillInfo;
		
					int domainType = pSlayerSkillInfo->getDomainiType();

					//--------------------------------------------------
					// 배운 skill들 체크..
					//--------------------------------------------------
					int num = pSlayerSkillInfo->getListNum();

					for (i=0; i<num; i++)
					{
						SubSlayerSkillInfo* pInfo = pSlayerSkillInfo->popFrontListElement();

						if (pInfo!=NULL)
						{
							int skillType	= pInfo->getSkillType();
							int skillExp	= pInfo->getSkillExp();
							int ExpLevel	= pInfo->getSkillExpLevel();
							DWORD delayTime = ConvertDurationToMillisecond( pInfo->getSkillTurn() );
							int currentDelay = ConvertDurationToMillisecond( pInfo->getCastingTime() );
							bool bEnable	= pInfo->getEnable();
							
//							if(skillType == SKILL_SOUL_CHAIN)
//							{
//								int a = 0;
//								bEnable = true;
//							}

							// Skill 배웠다는걸 체크한다.

							if((*g_pSkillInfoTable)[skillType].GetSkillStep() == SKILL_STEP_ETC)
							{
								(*g_pSkillManager)[SKILL_DOMAIN_BLADE].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_BLADE].LearnSkill( (ACTIONINFO)skillType );

								(*g_pSkillManager)[SKILL_DOMAIN_SWORD].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_SWORD].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_GUN].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_GUN].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_HEAL].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_HEAL].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].LearnSkill( (ACTIONINFO)skillType );

								(*g_pSkillManager)[SKILL_DOMAIN_ETC].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_ETC].LearnSkill( (ACTIONINFO)skillType );
								
							}
							else
							{
								(*g_pSkillManager)[domainType].SetNewSkill();
								(*g_pSkillManager)[domainType].LearnSkill( (ACTIONINFO)skillType );
							}

							(*g_pSkillInfoTable)[skillType].SetExpLevel( ExpLevel );
							(*g_pSkillInfoTable)[skillType].SetSkillExp( skillExp );

							(*g_pSkillInfoTable)[skillType].SetDelayTime( delayTime );
							(*g_pSkillInfoTable)[skillType].SetEnable( bEnable );

							(*g_pSkillInfoTable)[skillType].SetAvailableTime( currentDelay );

							switch (skillType)
							{
								case MAGIC_RESTORE :
									g_pUserInformation->HasSkillRestore = true;
								break;

								case SKILL_THROW_BOMB :
								{
									(*g_pSkillInfoTable)[BOMB_SPLINTER].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[BOMB_ACER].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[BOMB_BULLS].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[BOMB_STUN].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[BOMB_CROSSBOW].SetExpLevel( ExpLevel );
								}
								break;

								case SKILL_INSTALL_MINE :
									(*g_pSkillInfoTable)[MINE_ANKLE_KILLER].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[MINE_POMZ].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[MINE_AP_C1].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[MINE_DIAMONDBACK].SetExpLevel( ExpLevel );
									(*g_pSkillInfoTable)[MINE_SWIFT_EX].SetExpLevel( ExpLevel );
								break;
							}
							
							delete pInfo;
						}
					}

					//--------------------------------------------------
					// 새로 배울 수 있는 skill이 있나?
					//--------------------------------------------------
					if (pSlayerSkillInfo->isLearnNewSkill())
					{
						(*g_pSkillManager)[domainType].SetNewSkill();					
					}

				}
				break;

				//--------------------------------------------------
				//
				//						Vampire
				//
				//--------------------------------------------------
				case PC_VAMPIRE :
				{
					VampireSkillInfo* pVampireSkillInfo = (VampireSkillInfo*)pSkillInfo;

					int domainType = SKILLDOMAIN_VAMPIRE;

					//--------------------------------------------------
					// 배운 skill들 체크..
					//--------------------------------------------------
					int num = pVampireSkillInfo->getListNum();

					for (i=0; i<num; i++)
					{
						SubVampireSkillInfo* pInfo = pVampireSkillInfo->popFrontListElement();

						if (pInfo!=NULL)
						{
							int skillType	= pInfo->getSkillType();
							int Turn		= pInfo->getSkillTurn();
							DWORD delayTime = ConvertDurationToMillisecond( pInfo->getSkillTurn() );
							int currentDelay = ConvertDurationToMillisecond( pInfo->getCastingTime() );							
							
							// Skill 배웠다는걸 체크한다.
							if((*g_pSkillInfoTable)[skillType].GetSkillStep() == SKILL_STEP_ETC)
							{
								(*g_pSkillManager)[SKILL_DOMAIN_BLADE].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_BLADE].LearnSkill( (ACTIONINFO)skillType );

								(*g_pSkillManager)[SKILL_DOMAIN_SWORD].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_SWORD].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_GUN].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_GUN].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_HEAL].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_HEAL].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].LearnSkill( (ACTIONINFO)skillType );
								
								(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].LearnSkill( (ACTIONINFO)skillType );

								(*g_pSkillManager)[SKILL_DOMAIN_ETC].SetNewSkill();
								(*g_pSkillManager)[SKILL_DOMAIN_ETC].LearnSkill( (ACTIONINFO)skillType );
								
							}
							else
							{
								(*g_pSkillManager)[domainType].SetNewSkill();
								(*g_pSkillManager)[domainType].LearnSkill( (ACTIONINFO)skillType );							
							}
							
							(*g_pSkillInfoTable)[skillType].SetDelayTime( delayTime );							
							(*g_pSkillInfoTable)[skillType].SetAvailableTime( currentDelay );

							switch (skillType)
							{
								case MAGIC_GROUND_ATTACK :
									g_pUserInformation->HasMagicGroundAttack = true;
								break;

//								case MAGIC_HALLUCINATION :
//									g_pUserInformation->HasMagicHallu = true;
//								break;

								case MAGIC_BLOODY_SNAKE:
									g_pUserInformation->HasMagicBloodySnake = true;
									break;

								case MAGIC_BLOODY_WARP:
									g_pUserInformation->HasMagicBloodyWarp = true;
									break;
							}

							delete pInfo;
						}
					}

					//--------------------------------------------------
					// 새로 배울 수 있는 skill이 있나?
					//--------------------------------------------------
					if (pVampireSkillInfo->isLearnNewSkill())
					{
						(*g_pSkillManager)[domainType].SetNewSkill();					
					}
				}
				break;

				//--------------------------------------------------
				//
				//						Ousters
				//
				//--------------------------------------------------
				case PC_OUSTERS :
					{
						OustersSkillInfo* pOustersSkillInfo = (OustersSkillInfo*)pSkillInfo;
						
						int domainType = SKILLDOMAIN_OUSTERS;
						
						//--------------------------------------------------
						// 배운 skill들 체크..
						//--------------------------------------------------
						int num = pOustersSkillInfo->getListNum();
						
						for (i=0; i<num; i++)
						{
							SubOustersSkillInfo* pInfo = pOustersSkillInfo->popFrontListElement();
							
							if (pInfo!=NULL)
							{
								int skillType	= pInfo->getSkillType();
								int Turn		= pInfo->getSkillTurn();
								DWORD delayTime = ConvertDurationToMillisecond( pInfo->getSkillTurn() );
								int currentDelay = ConvertDurationToMillisecond( pInfo->getCastingTime() );							
								int	expLevel	= pInfo->getExpLevel();

								// Skill 배웠다는걸 체크한다.
								if((*g_pSkillInfoTable)[skillType].GetSkillStep() == SKILL_STEP_ETC)
								{
									(*g_pSkillManager)[SKILL_DOMAIN_BLADE].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_BLADE].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_SWORD].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_SWORD].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_GUN].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_GUN].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_HEAL].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_HEAL].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_ENCHANT].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_VAMPIRE].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_OUSTERS].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_OUSTERS].LearnSkill( (ACTIONINFO)skillType );
									
									(*g_pSkillManager)[SKILL_DOMAIN_ETC].SetNewSkill();
									(*g_pSkillManager)[SKILL_DOMAIN_ETC].LearnSkill( (ACTIONINFO)skillType );
									
								}
								else
								{
									(*g_pSkillManager)[domainType].SetNewSkill();
									(*g_pSkillManager)[domainType].LearnSkill( (ACTIONINFO)skillType );							
								}
								
								(*g_pSkillInfoTable)[skillType].SetExpLevel( expLevel );
								
								(*g_pSkillInfoTable)[skillType].SetDelayTime( delayTime );
								
								(*g_pSkillInfoTable)[skillType].SetAvailableTime( currentDelay );
								
								delete pInfo;
							}
						}
						
						//--------------------------------------------------
						// 새로 배울 수 있는 skill이 있나?
						//--------------------------------------------------
						if (pOustersSkillInfo->isLearnNewSkill())
						{
							(*g_pSkillManager)[domainType].SetNewSkill();					
						}
					}
					break;
			}
		}

		delete pSkillInfo;
	}

	//--------------------------------------------------
	// 홀리랜드 보너스 존이동 할때 reset
	//--------------------------------------------------
//	for(int i = 0; i < HOLYLAND_BONUS_MAX; i++)
//	{
//		g_abHolyLandBonusSkills[i] = false;
//	}
	
	for( int i = 0 ; i < SWEEPER_BONUS_MAX; i ++ )
		g_abSweeperBonusSkills[i] = false;

	//--------------------------------------------------
	// 현재 사용 가능한 skill들을 다시 체크한다.
	//--------------------------------------------------

	g_pSkillAvailable->SetAvailableSkills();

	__END_CATCH
}
