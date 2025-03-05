//----------------------------------------------------------------------
// AddonDef.h
//----------------------------------------------------------------------
// Addon.cfpk에 들어 있는 순서를 의미한다.
//----------------------------------------------------------------------

#ifndef	__ADDONDEF_H__
#define	__ADDONDEF_H__


//----------------------------------------------------------------------
// Addon종류
//----------------------------------------------------------------------
/*
enum ADDONID
{
	ADDONID_HAIR1,		// 머리1
	ADDONID_HAIR2,		// 머리2
	ADDONID_HAIR3,		// 머리3
	ADDONID_HELM1,		// 모자1
	ADDONID_HELM2,		// 모자2	
	ADDONID_COAT1,		// 상의1
	ADDONID_COAT2,		// 상의2		
	ADDONID_TROUSER1,	// 하의1
	ADDONID_TROUSER2,	// 하의2	
	ADDONID_SWORD,		// 검	
	ADDONID_BLADE,		// 도	
	ADDONID_GUN_TR,		// 총1 - 저격용총(TR)
	ADDONID_GUN_SG,		// 총2 - 샷건(SG)	
	ADDONID_GUN_AR,		// 총3 - 소총(AR)
	ADDONID_GUN_SMG,		// 총4 - 자동소총(SMG)	
	ADDONID_SHIELD,		// 방패	
	ADDONID_CROSS,		// 십자가
	ADDONID_MOTORCYCLE,	// 오토바이	
	MAX_ADDONID
};
*/
	

#ifdef __EXPO_CLIENT__
	//----------------------------------------------------------------------
	// 남자 Slayer
	//----------------------------------------------------------------------
	enum ADDONID_MALE
	{
		ADDONID_HELM1_MALE,			// 모자1
		ADDONID_COAT1_MALE,			// 상의1
		ADDONID_TROUSER1_MALE,		// 하의1
		ADDONID_GUN_SMG_MALE,			// 총4 - 자동소총(SMG)	
		ADDONID_BLADE_MALE,		// 도
		ADDONID_SHIELD1_MALE,		// 방패
		ADDONID_GUN_SG_MALE,			// 총2 - 샷건(SG)
		ADDONID_SWORD_MALE,		// 검	
		ADDONID_GUN_AR_MALE,			// 총3 - 소총(AR)
		ADDONID_HAIR1_MALE,			// 머리1
		ADDONID_HAIR2_MALE,			// 머리2
		ADDONID_HAIR3_MALE,			// 머리3
		ADDONID_HELM2_MALE,			// 모자2
		ADDONID_COAT2_MALE,			// 상의2
		ADDONID_TROUSER2_MALE,		// 하의2
		ADDONID_CROSS_MALE,			// 십자가
		ADDONID_GUN_TR_MALE,			// 총1 - 저격용총(TR)		
		ADDONID_MOTORCYCLE_MALE,	// 오토바이
		MAX_ADDONID_MALE
	};

	//----------------------------------------------------------------------
	// 여자 Slayer
	//----------------------------------------------------------------------
	enum ADDONID_FEMALE
	{
		ADDONID_HELM1_FEMALE,			// 모자1
		ADDONID_COAT1_FEMALE,			// 상의1
		ADDONID_TROUSER1_FEMALE,		// 하의1
		ADDONID_GUN_SMG_FEMALE,			// 총4 - 자동소총(SMG)	
		ADDONID_BLADE_FEMALE,		// 도
		ADDONID_SHIELD_FEMALE,		// 방패
		ADDONID_GUN_SG_FEMALE,			// 총2 - 샷건(SG)
		ADDONID_SWORD_FEMALE,		// 검	
		ADDONID_GUN_AR_FEMALE,			// 총3 - 소총(AR)
		ADDONID_HAIR1_FEMALE,			// 머리1
		ADDONID_HAIR2_FEMALE,			// 머리2
		ADDONID_HAIR3_FEMALE,			// 머리3
		ADDONID_HELM2_FEMALE,			// 모자2
		ADDONID_COAT2_FEMALE,			// 상의2
		ADDONID_TROUSER2_FEMALE,		// 하의2
		ADDONID_CROSS_FEMALE,			// 십자가
		ADDONID_GUN_TR_FEMALE,			// 총1 - 저격용총(TR)		
		ADDONID_MOTORCYCLE_FEMALE,	// 오토바이
		MAX_ADDONID_FEMALE
	};

#else

	//----------------------------------------------------------------------
	// 남자 Slayer
	//----------------------------------------------------------------------
	enum ADDONID_MALE
	{
		ADDONID_HELM1_MALE = 0,
		ADDONID_COAT1_MALE,
		ADDONID_TROUSER1_MALE,
		ADDONID_GUN_SMG_MALE,		// 자동소총(SMG)
		ADDONID_BLADE_MALE,			// 도
		ADDONID_SHIELD1_MALE,
		ADDONID_SHIELD2_MALE,		// 드래곤방패
		ADDONID_SHIELD3_MALE,		// 브론즈방패
		ADDONID_GUN_SG_MALE,
		ADDONID_SWORD_MALE,	// 검
		ADDONID_GUN_AR_MALE, 
		ADDONID_HAIR1_MALE,
		ADDONID_HAIR2_MALE,
		ADDONID_HAIR3_MALE,
		ADDONID_HELM2_MALE,
		ADDONID_COAT2_MALE,
		ADDONID_TROUSER2_MALE, 
		ADDONID_CROSS_MALE,
		ADDONID_GUN_SR_MALE,
		ADDONID_MOTORCYCLE_MALE,
		ADDONID_COAT0_MALE,
		ADDONID_TROUSER0_MALE,
		ADDONID_COAT3_MALE,
		ADDONID_TROUSER3_MALE,
		ADDONID_MACE_MALE,
	
		//----------------------------------------------------------------------
		// 여자 Slayer
		//----------------------------------------------------------------------
		ADDONID_HELM1_FEMALE,
		ADDONID_COAT1_FEMALE,
		ADDONID_TROUSER1_FEMALE,
		ADDONID_GUN_SMG_FEMALE,		// 자동소총(SMG)
		ADDONID_BLADE_FEMALE,			// 도
		ADDONID_SHIELD1_FEMALE,
		ADDONID_SHIELD2_FEMALE,		// 드래곤방패
		ADDONID_SHIELD3_FEMALE,		// 브론즈방패
		ADDONID_GUN_SG_FEMALE,
		ADDONID_SWORD_FEMALE,	// 검
		ADDONID_GUN_AR_FEMALE, 
		ADDONID_HAIR1_FEMALE,
		ADDONID_HAIR2_FEMALE,
		ADDONID_HAIR3_FEMALE,
		ADDONID_HELM2_FEMALE,
		ADDONID_COAT2_FEMALE,
		ADDONID_TROUSER2_FEMALE, 
		ADDONID_CROSS_FEMALE,
		ADDONID_GUN_SR_FEMALE,
		ADDONID_MOTORCYCLE_FEMALE,
		ADDONID_COAT0_FEMALE,
		ADDONID_TROUSER0_FEMALE,
		ADDONID_COAT3_FEMALE,
		ADDONID_TROUSER3_FEMALE,
		ADDONID_MACE_FEMALE,

		MAX_ADDONID_SLAYER
	};


#endif


#endif