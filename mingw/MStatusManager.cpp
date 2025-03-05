//----------------------------------------------------------------------
// MStatusManager.cpp
//----------------------------------------------------------------------

#include <Windows.h>
#include "MStatusManager.h"
#include "SkillDef.h"

//----------------------------------------------------------------------
// max값 정의
//----------------------------------------------------------------------
#define SLAYER_MAX_HP            500
#define SLAYER_MAX_MP            500
#define SLAYER_MAX_TOHIT         500
#define SLAYER_MAX_DEFENSE       500
#define SLAYER_MAX_PROTECTION    500
#define SLAYER_MAX_DAMAGE        500

#define VAMPIRE_MAX_HP           1000
#define VAMPIRE_MAX_TOHIT        1000
#define VAMPIRE_MAX_DEFENSE		 1000
#define VAMPIRE_MAX_PROTECTION   1000
#define VAMPIRE_MAX_DAMAGE       1000

#define OUSTERS_MAX_HP           1000
#define OUSTERS_MAX_TOHIT        1000
#define OUSTERS_MAX_DEFENSE		 1000
#define OUSTERS_MAX_PROTECTION   1000
#define OUSTERS_MAX_DAMAGE       1000

#define MONSTER_MAX_HP          20000
#define MONSTER_MAX_TOHIT        1000
#define MONSTER_MAX_DEFENSE      1000
#define MONSTER_MAX_PROTECTION   1000
#define MONSTER_MAX_DAMAGE       1000

#define SLAYER_MAX_ATTACK_SPEED   35
#define VAMPIRE_MAX_ATTACK_SPEED  30
#define OUSTERS_MAX_ATTACK_SPEED  35

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
__declspec(dllexport) MStatusManager		g_StatusManager;


//----------------------------------------------------------------------
// 
// constructor / destructor
//
//----------------------------------------------------------------------
MStatusManager::MStatusManager()
{
	m_STR = m_DEX = m_INT = 0;

	m_Domain = 0;
	m_DomainLevel = 0;

}

MStatusManager::~MStatusManager()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Get ToHit
//----------------------------------------------------------------------
//	Sword  Slayer : DEX + SwordDomainLevel/2
//	Blade  Slayer : DEX + BladeDomainLevel/2
//	Gun    Slayer : DEX + GunDomainLevel/2
//	Cleric Slayer : DEX
//	Vampire       : DEX + LEVEL/5
//----------------------------------------------------------------------
int 
MStatusManager::GetTOHIT()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
		case MAX_SKILLDOMAIN :
			value = min(m_DEX/2, SLAYER_MAX_TOHIT);
		break;

		//---------------------------------------------------
		// 검 / 도
		//---------------------------------------------------
		case SKILLDOMAIN_SWORD :
		case SKILLDOMAIN_BLADE :
		case SKILLDOMAIN_GUN :	
			value = (int)(m_DEX/2 + m_DomainLevel*1.5);
			value = min((int)value, SLAYER_MAX_TOHIT);
		break;		
		
		//---------------------------------------------------
		// 메이스 / 십자가
		//---------------------------------------------------
		case SKILLDOMAIN_HEAL :
		case SKILLDOMAIN_ENCHANT :
			value = (int)(m_DEX/2 + m_DomainLevel*1.5);
			value = min((int)value, SLAYER_MAX_TOHIT);
		break;

		//---------------------------------------------------
		// 아우스터즈
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS :
			value = (int)(m_DEX/2 + m_DomainLevel);
			value = min((int)value, OUSTERS_MAX_TOHIT);						
			break;
			
		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :
			value = (int)(m_DEX + m_DomainLevel/2.5);
			value = min((int)value, VAMPIRE_MAX_TOHIT);						
		break;

	}

	return value;
}

//----------------------------------------------------------------------
// Get Defense
//----------------------------------------------------------------------
// Slayer  : STR/15*2
// Vampire : DEX + LEVEL/5
//----------------------------------------------------------------------
int 
MStatusManager::GetDefense()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
//		case MAX_SKILLDOMAIN :
//			value = m_DEX/2;
//		break;

		//---------------------------------------------------
		// 아우스터즈
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS :
			value = (int)(m_DEX/2 + m_DomainLevel/5);
			value = min((int)value, OUSTERS_MAX_DEFENSE);						
			break;
			
		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :
			value = (int)(m_DEX/2 + m_DomainLevel/5);
			value = min((int)value, VAMPIRE_MAX_DEFENSE);						
		break;

		//---------------------------------------------------
		// 슬레이어
		//---------------------------------------------------
		default :
			value = min(m_DEX/2, SLAYER_MAX_DEFENSE);
	}

	return value;
}

//----------------------------------------------------------------------
// Get Protect
//----------------------------------------------------------------------
//	Slayer  : STR/15
//	Vampire : STR/5 + LEVEL/5
//----------------------------------------------------------------------
int 
MStatusManager::GetProtection()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
		case MAX_SKILLDOMAIN :
//			value = m_STR/15;
		break;

		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS :
			value = (int)(m_STR + m_DomainLevel/10);
			value = min((int)value, OUSTERS_MAX_PROTECTION);						
		break;
			
		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :
			value = (int)(m_STR + m_DomainLevel/5);
			value = min((int)value, VAMPIRE_MAX_PROTECTION);						
		break;

		//---------------------------------------------------
		// 슬레이어
		//---------------------------------------------------
		default :	
//			value = (int)(m_STR/15);
			value = min(m_STR, SLAYER_MAX_PROTECTION);
		
	}

	return value;
}


//----------------------------------------------------------------------
// Min Damage 계산
//----------------------------------------------------------------------
//	Sword  Slayer : STR/15
//	Blade  Slayer : STR/15
//	Gun    Slayer : 0
//	Cleric Slayer : STR/15
//	Vampire       : STR/10
//----------------------------------------------------------------------
int 
MStatusManager::GetMinDAM()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
		case MAX_SKILLDOMAIN :
			value = 1;
		break;

		//---------------------------------------------------
		// 아우스터즈
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS :
			value = max(1, (int)(m_STR/10 + m_DomainLevel/10));
			value = min((int)value, OUSTERS_MAX_DAMAGE);			
			break;
			
		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :
			value = max(1, (int)(m_STR/6 + m_DomainLevel/5));
			value = min((int)value, VAMPIRE_MAX_DAMAGE);			
		break;

		//---------------------------------------------------
		// 총
		//---------------------------------------------------
		case SKILLDOMAIN_GUN :			
			value = 1;
		break;

		//---------------------------------------------------
		// 검 / 도 / 십자가
		//---------------------------------------------------
		default :
//			value = (int)(m_STR/15);
			value = min(m_STR/15, SLAYER_MAX_DAMAGE);		


	}

	return value;
}

//----------------------------------------------------------------------
// Max Damage 계산
//----------------------------------------------------------------------
//	Sword Slayer  : STR/5
//	Blade Slayer  : STR/5
//	Gun   Slayer  : 0
//	Cleric Slayer : STR/5
//	Vampire       : STR/5
//----------------------------------------------------------------------
int 
MStatusManager::GetMaxDAM()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
		case MAX_SKILLDOMAIN :
//			value = m_STR/10;
			value = min(m_STR/10, SLAYER_MAX_DAMAGE);
		break;

		//---------------------------------------------------
		// 총
		//---------------------------------------------------
		case SKILLDOMAIN_GUN :			
			value = 2;
		break;
		
		//---------------------------------------------------
		// 아우스터즈
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS :			
			value = max(1, m_STR/6 + m_DomainLevel/6);
			value = min((int)value, OUSTERS_MAX_DAMAGE);
			break;
			
		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :			
			value = max(1, m_STR/4 + m_DomainLevel/5);
			value = min((int)value, VAMPIRE_MAX_DAMAGE);
		break;

		//---------------------------------------------------
		// 검 / 도 / 십자가
		//---------------------------------------------------
		default :
//			value = (int)(m_STR / 5);
			value = min(m_STR/10, SLAYER_MAX_DAMAGE);

	}

	return value;
}



//----------------------------------------------------------------------
// Attack Speed
//----------------------------------------------------------------------
//	Sword Slayer  : STR/10 + SwordDomainLevel/5
//	Blade Slayer  : STR/10 + BladeDomainLevel/5
//	Gun   Slayer  : DEX/10 + GunDomainLevel/5
//	Cleric Slayer : STR/10
//	Vampire       : DEX/10 + 10
//----------------------------------------------------------------------
int 
MStatusManager::GetAttackSpeed()
{
	int	value  = 0;

	switch (m_Domain)
	{
		//---------------------------------------------------
		// 맨손
		//---------------------------------------------------
		case MAX_SKILLDOMAIN :
//			value = m_STR/10;
			value = min(m_STR/10, SLAYER_MAX_ATTACK_SPEED);
		break;

		//---------------------------------------------------
		// 아우스터즈
		//---------------------------------------------------
		case SKILLDOMAIN_OUSTERS:
			value = (int)((m_DEX+m_DomainLevel)/10);
			value = min((int)value, OUSTERS_MAX_ATTACK_SPEED);
			break;

		//---------------------------------------------------
		// 뱀파이어
		//---------------------------------------------------
		case SKILLDOMAIN_VAMPIRE :
			value = (int)(m_DEX / 10 + 10);
			value = min((int)value, VAMPIRE_MAX_ATTACK_SPEED);
		break;

		//---------------------------------------------------
		// 메이스 / 십자가
		//---------------------------------------------------
		case SKILLDOMAIN_HEAL :
		case SKILLDOMAIN_ENCHANT :
//			value = (int)(m_STR / 10);
			value = min(m_STR/10, SLAYER_MAX_ATTACK_SPEED);
		break;

		//---------------------------------------------------
		// 총
		//---------------------------------------------------
		case SKILLDOMAIN_GUN :
//			value = (int)(m_DEX/10 + m_DomainLevel/5);
			value = min(m_DEX/10+m_DomainLevel/5, SLAYER_MAX_ATTACK_SPEED);
		break;

		//---------------------------------------------------
		// 검 / 도 
		//---------------------------------------------------
		default :
//			value = (int)(m_STR/10 + m_DomainLevel/5);
			value = min(m_STR/10+m_DomainLevel/5, SLAYER_MAX_ATTACK_SPEED);
		
	}

	return value;
}

/****************** Add by Sonic 2006.9.6 Start *****************/
void
MStatusManager::Set(int str, int dex, int intel)		
{

	m_STR = str;
	m_DEX = dex;
	m_INT = intel;
}


//--------------------------------------------------------------
// 현재 사용하는 무기의 domain level을 설정한다.
// Vampire는 domain level을 설정한다.
//--------------------------------------------------------------
void
MStatusManager::SetCurrentWeaponDomain(int domain, int level)
{
	m_Domain	 = domain;
	m_DomainLevel = level;
}
/****************** Add End by Sonic 2006.9.6 *******************/