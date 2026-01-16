//----------------------------------------------------------------------
// MSkillInfoTable.h
//----------------------------------------------------------------------
// ´ÙÀ½ Ç×¸ñÀº SkillInfo.inf¸¦ ÅëÇØ¼­ ¼­¹ö¿¡¼­ ¹Þ´Â °ÍÀÌ¹Ç·Î
// ½Å°æ¾µ ÇÊ¿ä°¡ ¾ø´Ù.
//		Name(±â¼ú ÀÌ¸§)
//		LearnLevel(±â¼ú ¹è¿ì´Â ·¹º§)
//		MP(»ç¿ë MP)
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MSkillManager.h"

#ifdef __INIT_INFO__
	#include "MActionInfoTable.h"
#endif


#ifndef __NEW_SKILL__
	#define __NEW_SKILL__
	//#undef  __NEW_SKILL__
#endif
//----------------------------------------------------------------------
//
//						MSkillInfoTable
//
//----------------------------------------------------------------------
MSkillInfoTable::MSkillInfoTable()
{
	// ±â¼úÀÇ °³¼ö¸¸Å­..
	CTypeTable<SKILLINFO_NODE>::Init( MIN_RESULT_ACTIONINFO );

#ifndef __GAME_CLIENT__

	//----------------------------------------------------------------------
	// ÀÏ´Ü ¸ðµç skill¿¡ ´ëÇÑ defaultÁ¤º¸ ¼³Á¤
	//----------------------------------------------------------------------
	for (int i=0; i<MIN_RESULT_ACTIONINFO; i++)
	{
		#ifdef __INIT_INFO__
			m_pTypeInfo[i].Set(0, (*g_pActionInfoTable)[i].GetName(), 0, 0, 0, "Not Defined");
		#else
			m_pTypeInfo[i].Set(0, "Not defined", 0, 0, 0,"Î´¶¨Òå");
		#endif
		m_pTypeInfo[i].UnSetPassive();		
		m_pTypeInfo[i].UnSetActive();
		m_pTypeInfo[i].SetSkillStep( SKILL_STEP_NULL );
		m_pTypeInfo[i].SetLearnLevel( -1 );	// ÃÊ±âÈ­
		m_pTypeInfo[i].SetSlayerSkill();		
	}

	int sid = 0;

	//----------------------------------------------------------------------
	// °Ë°è¿­
	//----------------------------------------------------------------------
	m_pTypeInfo[SKILL_DOUBLE_IMPACT].Set(sid++, "Double Impact", 400, 100, 0,"Ë«ÖØ´ò»÷");
	m_pTypeInfo[SKILL_DOUBLE_IMPACT].SetMP( 5 );	
	m_pTypeInfo[SKILL_DOUBLE_IMPACT].SetLearnLevel( 5 );
	m_pTypeInfo[SKILL_DOUBLE_IMPACT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DOUBLE_IMPACT].AddNextSkill( SKILL_TRIPLE_SLASHER );
	
	m_pTypeInfo[SKILL_TRIPLE_SLASHER].Set(sid++, "Triple Slasher", 200, 130, 1,"ÈýÖØ´ò»÷");		
	m_pTypeInfo[SKILL_TRIPLE_SLASHER].SetMP( 8 );
	m_pTypeInfo[SKILL_TRIPLE_SLASHER].SetLearnLevel( 10 );
	m_pTypeInfo[SKILL_TRIPLE_SLASHER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TRIPLE_SLASHER].AddNextSkill( SKILL_SWORD_MASTERY );

	m_pTypeInfo[SKILL_SWORD_MASTERY].Set(sid++, "Sword Mastery", 200, 130, 154,"½£·¨ÊìÁ·¶È");
	m_pTypeInfo[SKILL_SWORD_MASTERY].SetMP( 8 );
	m_pTypeInfo[SKILL_SWORD_MASTERY].SetPassive();
	m_pTypeInfo[SKILL_SWORD_MASTERY].SetLearnLevel( 15 );
	m_pTypeInfo[SKILL_SWORD_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SWORD_MASTERY].AddNextSkill( SKILL_DANCING_SWORD );

	m_pTypeInfo[SKILL_DANCING_SWORD].Set(sid++, "Dancing Sword", 700, 190, 4,"·ÉÎèÖ®½£");
	m_pTypeInfo[SKILL_DANCING_SWORD].SetMP( 12 );
	m_pTypeInfo[SKILL_DANCING_SWORD].SetLearnLevel( 20 );
	m_pTypeInfo[SKILL_DANCING_SWORD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DANCING_SWORD].AddNextSkill( SKILL_FABULOUS_SOUL );	
//	m_pTypeInfo[SKILL_DANCING_SWORD].AddNextSkill( SKILL_THUNDER_SPARK );	

	//2004,4,30,sobeit add start
	m_pTypeInfo[SKILL_FABULOUS_SOUL].Set(sid++, "Fabulous Soul", 600, 160, 382,"Ææ¼£Ö®»ê");
	m_pTypeInfo[SKILL_FABULOUS_SOUL].SetMP( 0 );
	m_pTypeInfo[SKILL_FABULOUS_SOUL].SetPassive();
	m_pTypeInfo[SKILL_FABULOUS_SOUL].SetLearnLevel( 23 );
	m_pTypeInfo[SKILL_FABULOUS_SOUL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_FABULOUS_SOUL].AddNextSkill( SKILL_THUNDER_SPARK );
	//2004,4,30,sobeit add end

	m_pTypeInfo[SKILL_THUNDER_SPARK].Set(sid++, "Thunder Spark", 600, 160, 2,"À×¹âÉÁµç");
	m_pTypeInfo[SKILL_THUNDER_SPARK].SetMP( 10 );
	m_pTypeInfo[SKILL_THUNDER_SPARK].SetLearnLevel( 25 );
	m_pTypeInfo[SKILL_THUNDER_SPARK].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_THUNDER_SPARK].AddNextSkill( SKILL_SHIELD_MASTERY );

	m_pTypeInfo[SKILL_SHIELD_MASTERY].Set(sid++, "Shield Mastery", 200, 130, 155,"¶ÜÅÆÊìÁ·¶È");
	m_pTypeInfo[SKILL_SHIELD_MASTERY].SetMP( 8 );
	m_pTypeInfo[SKILL_SHIELD_MASTERY].SetPassive();
	m_pTypeInfo[SKILL_SHIELD_MASTERY].SetLearnLevel( 30 );
	m_pTypeInfo[SKILL_SHIELD_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SHIELD_MASTERY].AddNextSkill( SKILL_CROSS_COUNTER );

	m_pTypeInfo[SKILL_CROSS_COUNTER].Set(sid++, "Cross Counter", 400, 210, 5,"·´»÷");
	m_pTypeInfo[SKILL_CROSS_COUNTER].SetMP( 15 );
	m_pTypeInfo[SKILL_CROSS_COUNTER].SetLearnLevel( 35 );
	m_pTypeInfo[SKILL_CROSS_COUNTER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_CROSS_COUNTER].AddNextSkill( SKILL_RAINBOW_SLASHER );
	
	m_pTypeInfo[SKILL_RAINBOW_SLASHER].Set(sid++, "Rainbow Slasher", 200, 160, 3,"²Êºç¹¥»÷");
	m_pTypeInfo[SKILL_RAINBOW_SLASHER].SetMP( 10 );
	m_pTypeInfo[SKILL_RAINBOW_SLASHER].SetLearnLevel( 40 );
	m_pTypeInfo[SKILL_RAINBOW_SLASHER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_RAINBOW_SLASHER].AddNextSkill( SKILL_FLASH_SLIDING );

	m_pTypeInfo[SKILL_FLASH_SLIDING].Set(sid++, "Flash Sliding", 300, 240, 6,"ÉÁµç»¬ÐÐ");
	m_pTypeInfo[SKILL_FLASH_SLIDING].SetMP( 20 );
	m_pTypeInfo[SKILL_FLASH_SLIDING].SetLearnLevel( 45 );
	m_pTypeInfo[SKILL_FLASH_SLIDING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_FLASH_SLIDING].AddNextSkill( SKILL_SNAKE_COMBO );

	m_pTypeInfo[SKILL_SNAKE_COMBO].Set(sid++, "Snake Combo", 700, 190, 8,"ÁéÉßÉÁ¶¯");
	m_pTypeInfo[SKILL_SNAKE_COMBO].SetMP( 12 );
	m_pTypeInfo[SKILL_SNAKE_COMBO].SetLearnLevel( 50 );
	m_pTypeInfo[SKILL_SNAKE_COMBO].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SNAKE_COMBO].AddNextSkill( SKILL_SWORD_WAVE );	

	m_pTypeInfo[SKILL_SWORD_WAVE].Set(sid++, "Sword Wave", 500, 240,  9,"²¨ÀËÖ®½£");
	m_pTypeInfo[SKILL_SWORD_WAVE].SetMP( 20 );
	m_pTypeInfo[SKILL_SWORD_WAVE].SetLearnLevel( 55 );
	m_pTypeInfo[SKILL_SWORD_WAVE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SWORD_WAVE].AddNextSkill( SKILL_SWORD_RAY );

	m_pTypeInfo[SKILL_SWORD_RAY].Set(sid++, "Sword Ray", 0, 0, 9 ,"½£Æø");
	m_pTypeInfo[SKILL_SWORD_RAY].SetMP( 25 );
	m_pTypeInfo[SKILL_SWORD_RAY].SetLearnLevel( 61 );
	m_pTypeInfo[SKILL_SWORD_RAY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SWORD_RAY].AddNextSkill( SKILL_LIGHTNING_HAND );

	m_pTypeInfo[SKILL_LIGHTNING_HAND].Set(sid++, "Lightning Hand", 300, 270, 7,"µç¹âÌúÊÖ");
	m_pTypeInfo[SKILL_LIGHTNING_HAND].SetMP( 25 );
	m_pTypeInfo[SKILL_LIGHTNING_HAND].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_LIGHTNING_HAND].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_LIGHTNING_HAND].AddNextSkill( SKILL_REDIANCE );
	
	m_pTypeInfo[SKILL_REDIANCE].Set(sid++, "Rediance", 300, 270, 322,"ËÙ¶È¼¤·¢");
	m_pTypeInfo[SKILL_REDIANCE].SetMP( 25 );
	m_pTypeInfo[SKILL_REDIANCE].SetLearnLevel( 68 );
	m_pTypeInfo[SKILL_REDIANCE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_REDIANCE].AddNextSkill( SKILL_WIND_DIVIDER );

	m_pTypeInfo[SKILL_WIND_DIVIDER].Set(sid++, "Wind Divider", 500, 240,  121,"ËºÁÑÖ®·ç");
	m_pTypeInfo[SKILL_WIND_DIVIDER].SetMP( 20 );
	m_pTypeInfo[SKILL_WIND_DIVIDER].SetLearnLevel( 70 );
	m_pTypeInfo[SKILL_WIND_DIVIDER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WIND_DIVIDER].AddNextSkill( SKILL_EXPANSION );

	m_pTypeInfo[SKILL_EXPANSION].Set(sid++, "Expansion", 700, 270,  11,"ÌåÁ¦Ôö·ù");
	m_pTypeInfo[SKILL_EXPANSION].SetMP( 25 );
	m_pTypeInfo[SKILL_EXPANSION].SetLearnLevel( 75 );
	m_pTypeInfo[SKILL_EXPANSION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_EXPANSION].AddNextSkill( SKILL_THUNDER_BOLT );

	m_pTypeInfo[SKILL_THUNDER_BOLT].Set(sid++, "Thunder Bolt", 700, 270,  10,"À×Ãù½£");
	m_pTypeInfo[SKILL_THUNDER_BOLT].SetMP( 25 );
	m_pTypeInfo[SKILL_THUNDER_BOLT].SetLearnLevel( 80 );
	m_pTypeInfo[SKILL_THUNDER_BOLT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_THUNDER_BOLT].AddNextSkill( SKILL_MENTAL_SWORD );

	m_pTypeInfo[SKILL_MENTAL_SWORD].Set(sid++, "Mental Sword", 700, 270,  156,"ÄýÐÄ½£");
	m_pTypeInfo[SKILL_MENTAL_SWORD].SetMP( 25 );
	m_pTypeInfo[SKILL_MENTAL_SWORD].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_MENTAL_SWORD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MENTAL_SWORD].AddNextSkill( SKILL_LARSLASH );

	m_pTypeInfo[SKILL_LARSLASH].Set(sid++, "Lar Slash", 700, 270,  323,"ÁÒÑ×½£");
	m_pTypeInfo[SKILL_LARSLASH].SetMP( 25 );
	m_pTypeInfo[SKILL_LARSLASH].SetLearnLevel( 88 );
	m_pTypeInfo[SKILL_LARSLASH].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_LARSLASH].AddNextSkill( SKILL_THUNDER_STORM );

	m_pTypeInfo[SKILL_THUNDER_STORM].Set(sid++, "Thunder Storm", 700, 270,  157,"À×Ãù·ç±©");
	m_pTypeInfo[SKILL_THUNDER_STORM].SetMP( 25 );
	m_pTypeInfo[SKILL_THUNDER_STORM].SetLearnLevel( 90 );
	m_pTypeInfo[SKILL_THUNDER_STORM].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_THUNDER_STORM].AddNextSkill( SKILL_SHARP_SHIELD );

	m_pTypeInfo[SKILL_SHARP_SHIELD].Set(sid++, "Sharp Shield", 700, 270,  216,"¼â´ÌÖ®¶Ü");
	m_pTypeInfo[SKILL_SHARP_SHIELD].SetMP( 25 );
	m_pTypeInfo[SKILL_SHARP_SHIELD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SHARP_SHIELD].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_SHARP_SHIELD].AddNextSkill( SKILL_WIDE_LIGHTNING );

	m_pTypeInfo[SKILL_WIDE_LIGHTNING].Set(sid++, "Wide Lightning", 700, 270,  217,"´óÉÁµç");
	m_pTypeInfo[SKILL_WIDE_LIGHTNING].SetMP( 25 );
	m_pTypeInfo[SKILL_WIDE_LIGHTNING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WIDE_LIGHTNING].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_WIDE_LIGHTNING].AddNextSkill( SKILL_BLITZ_SLIDING );
	
	m_pTypeInfo[SKILL_BLITZ_SLIDING].Set(sid++, "Blitz Sliding", 700, 270,  313,"ÃðÈÕÐÞÂÞÕ¶");
	m_pTypeInfo[SKILL_BLITZ_SLIDING].SetMP( 25 );
	m_pTypeInfo[SKILL_BLITZ_SLIDING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_BLITZ_SLIDING].SetLearnLevel( 110 );
	m_pTypeInfo[SKILL_BLITZ_SLIDING].AddNextSkill( SKILL_HIT_CONVERT );

	m_pTypeInfo[SKILL_HIT_CONVERT].Set(sid++, "Hit Convert", 700, 270,  254,"¹¥»÷×ª»»");
	m_pTypeInfo[SKILL_HIT_CONVERT].SetMP( 40 );
	m_pTypeInfo[SKILL_HIT_CONVERT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HIT_CONVERT].SetLearnLevel( 120 );
	m_pTypeInfo[SKILL_HIT_CONVERT].AddNextSkill( SKILL_SWORD_OF_THOR );

	// 2004, 9, 8, sobeit add start - °Ë 130 ½ºÅ³
	m_pTypeInfo[SKILL_SWORD_OF_THOR].Set(sid++, "Sword of Thor", 700, 270,  387,"À×ÉñÖ®½£");
	m_pTypeInfo[SKILL_SWORD_OF_THOR].SetMP( 40 );
	m_pTypeInfo[SKILL_SWORD_OF_THOR].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SWORD_OF_THOR].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_SWORD_OF_THOR].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_SWORD_OF_THOR].AddNextSkill( SKILL_INFINITY_THUNDERBOLT );
	// 2004, 9, 8, sobeit add end - °Ë 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - °Ë 140 ½ºÅ³
	m_pTypeInfo[SKILL_INFINITY_THUNDERBOLT].Set(sid++, "Infinity Thunderbolt", 700, 270,  458,"ÎÞÏÞÉÁµç");
	m_pTypeInfo[SKILL_INFINITY_THUNDERBOLT].SetMP( 40 );
	m_pTypeInfo[SKILL_INFINITY_THUNDERBOLT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_INFINITY_THUNDERBOLT].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_INFINITY_THUNDERBOLT].AddNextSkill( SKILL_BIKE_CRASH );
	// 2004, 11, 11, sobeit add end - °Ë 140 ½ºÅ³

	m_pTypeInfo[SKILL_BIKE_CRASH].Set(sid++, "Bike Crash", 700, 270,  470,"·É³µÒ»»÷");
	m_pTypeInfo[SKILL_BIKE_CRASH].SetMP( 40 );
	m_pTypeInfo[SKILL_BIKE_CRASH].SetSkillStep( SKILL_STEP_SLAYER_SWORD_ADVANCEMENT );
	m_pTypeInfo[SKILL_BIKE_CRASH].SetLearnLevel( 150 );

#ifdef __NEW_SKILL__

	m_pTypeInfo[SKILL_BIKE_CRASH].AddNextSkill( SKILL_SHINE_SWORD );
	// add by Coffee 2007-3-2  ÈËÀàÐÂ¼¼ÄÜ   
	m_pTypeInfo[SKILL_SHINE_SWORD].Set(sid++, "Shine Sword", 700, 270,  497,"ÉÁÒ«Ö®½£");
	m_pTypeInfo[SKILL_SHINE_SWORD].SetMP( 40 );
	m_pTypeInfo[SKILL_SHINE_SWORD].SetSkillStep( SKILL_STEP_SLAYER_SWORD_ADVANCEMENT );
	m_pTypeInfo[SKILL_SHINE_SWORD].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_SHINE_SWORD].AddNextSkill( SKILL_SOUL_CHAIN );
	// end 2007-3-2
#else
	m_pTypeInfo[SKILL_BIKE_CRASH].AddNextSkill( SKILL_SOUL_CHAIN );
#endif
	//----------------------------------------------------------------------
	// µµ °è¿­
	//----------------------------------------------------------------------
	sid = 0;
	m_pTypeInfo[SKILL_SINGLE_BLOW].Set(sid++, "Single Blow", 0, 0,  13,"¾«×¼´ò»÷");
	m_pTypeInfo[SKILL_SINGLE_BLOW].SetMP( 8 );
	m_pTypeInfo[SKILL_SINGLE_BLOW].SetLearnLevel( 5 );
	m_pTypeInfo[SKILL_SINGLE_BLOW].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SINGLE_BLOW].AddNextSkill( SKILL_SPIRAL_SLAY );

	m_pTypeInfo[SKILL_SPIRAL_SLAY].Set(sid++, "Spiral Slay", 0, 0,  14,"ÂÞÐýÒ»»÷");
	m_pTypeInfo[SKILL_SPIRAL_SLAY].SetMP( 10 );
	m_pTypeInfo[SKILL_SPIRAL_SLAY].SetLearnLevel( 10 );
	m_pTypeInfo[SKILL_SPIRAL_SLAY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SPIRAL_SLAY].AddNextSkill( SKILL_CONCENTRATION );
	
	m_pTypeInfo[SKILL_CONCENTRATION].Set(sid++, "Concentration", 200, 130, 150,"ÐÄÉñÄý¾Û");	
	m_pTypeInfo[SKILL_CONCENTRATION].SetMP( 8 );
	m_pTypeInfo[SKILL_CONCENTRATION].SetPassive();
	m_pTypeInfo[SKILL_CONCENTRATION].SetLearnLevel( 15 );
	m_pTypeInfo[SKILL_CONCENTRATION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_CONCENTRATION].AddNextSkill( SKILL_GHOST_BLADE );

	m_pTypeInfo[SKILL_GHOST_BLADE].Set(sid++, "Ghost Blade", 0, 0, 17,"ÓÄÁéÖ®ÈÐ");
	m_pTypeInfo[SKILL_GHOST_BLADE].SetMP( 12 );
	m_pTypeInfo[SKILL_GHOST_BLADE].SetLearnLevel( 20 );
	m_pTypeInfo[SKILL_GHOST_BLADE].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_GHOST_BLADE].AddNextSkill( SKILL_TRIPLE_BREAK );
	m_pTypeInfo[SKILL_GHOST_BLADE].AddNextSkill( SKILL_WILL_OF_IRON );

	//2004,4,30,sobeit add start
	m_pTypeInfo[SKILL_WILL_OF_IRON].Set(sid++, "Will Of Iron", 600, 160, 382,"¸ÖÌúÒâÖ¾");
	m_pTypeInfo[SKILL_WILL_OF_IRON].SetMP( 0 );
	m_pTypeInfo[SKILL_WILL_OF_IRON].SetPassive();
	m_pTypeInfo[SKILL_WILL_OF_IRON].SetLearnLevel( 23 );
	m_pTypeInfo[SKILL_WILL_OF_IRON].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WILL_OF_IRON].AddNextSkill( SKILL_TRIPLE_BREAK );
	//2004,4,30,sobeit add end
	
	m_pTypeInfo[SKILL_TRIPLE_BREAK].Set(sid++, "Triple Break", 0, 0, 15,"Èý±¶±¬»÷");
	m_pTypeInfo[SKILL_TRIPLE_BREAK].SetMP( 12 );
	m_pTypeInfo[SKILL_TRIPLE_BREAK].SetLearnLevel( 25 );
	m_pTypeInfo[SKILL_TRIPLE_BREAK].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TRIPLE_BREAK].AddNextSkill( SKILL_EVASION );
	
	m_pTypeInfo[SKILL_EVASION].Set(sid++, "ÀÌº£ÀÌ¼Ç", 200, 130, 151,"ÌÓÍÑÊõ");	
	m_pTypeInfo[SKILL_EVASION].SetMP( 8 );
	m_pTypeInfo[SKILL_EVASION].SetPassive();
	m_pTypeInfo[SKILL_EVASION].SetLearnLevel( 30 );
	m_pTypeInfo[SKILL_EVASION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_EVASION].AddNextSkill( SKILL_WILD_SMASH );

	m_pTypeInfo[SKILL_WILD_SMASH].Set(sid++, "Wild Smash", 0, 0, 16,"Ò°Âù±¬»÷");
	m_pTypeInfo[SKILL_WILD_SMASH].SetMP( 12 );
	m_pTypeInfo[SKILL_WILD_SMASH].SetLearnLevel( 35 );
	m_pTypeInfo[SKILL_WILD_SMASH].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WILD_SMASH].AddNextSkill( SKILL_SHADOW_WALK );

	m_pTypeInfo[SKILL_SHADOW_WALK].Set(sid++, "Shadow Walk", 0, 0, 19,"Î²ÐÐÖ®Êõ");
	m_pTypeInfo[SKILL_SHADOW_WALK].SetMP( 20 );
	m_pTypeInfo[SKILL_SHADOW_WALK].SetLearnLevel( 45 );
	m_pTypeInfo[SKILL_SHADOW_WALK].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SHADOW_WALK].AddNextSkill( SKILL_POTENTIAL_EXPLOSION );
	
	m_pTypeInfo[SKILL_POTENTIAL_EXPLOSION].Set(sid++, "Potential Explosion", 0, 0, 18,"Ç±Á¦±¬Õ¨");
	m_pTypeInfo[SKILL_POTENTIAL_EXPLOSION].SetMP( 25 );
	m_pTypeInfo[SKILL_POTENTIAL_EXPLOSION].SetLearnLevel( 50 );
	m_pTypeInfo[SKILL_POTENTIAL_EXPLOSION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_POTENTIAL_EXPLOSION].AddNextSkill( SKILL_HURRICANE_COMBO );

	m_pTypeInfo[SKILL_HURRICANE_COMBO].Set(sid++, "Hurricane Combo", 0, 0, 21,"¿ñ·çÖè»÷");
	m_pTypeInfo[SKILL_HURRICANE_COMBO].SetMP( 15 );
	m_pTypeInfo[SKILL_HURRICANE_COMBO].SetLearnLevel( 55 );
	m_pTypeInfo[SKILL_HURRICANE_COMBO].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HURRICANE_COMBO].AddNextSkill( SKILL_TORNADO_SEVER );
	
	m_pTypeInfo[SKILL_TORNADO_SEVER].Set(sid++, "Tornado Sever", 0, 0, 22,"¶Ï¿ÕÕ¶");
	m_pTypeInfo[SKILL_TORNADO_SEVER].SetMP( 20 );
	m_pTypeInfo[SKILL_TORNADO_SEVER].SetLearnLevel( 60 );
	m_pTypeInfo[SKILL_TORNADO_SEVER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TORNADO_SEVER].AddNextSkill( SKILL_MULTI_AMPUTATE );

	m_pTypeInfo[SKILL_MULTI_AMPUTATE].Set(sid++, "Multi Amputate", 0, 0, 247,"Á¬ÐøÇÐ¸î");
	m_pTypeInfo[SKILL_MULTI_AMPUTATE].SetMP( 20 );
	m_pTypeInfo[SKILL_MULTI_AMPUTATE].SetLearnLevel( 61 );
	m_pTypeInfo[SKILL_MULTI_AMPUTATE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MULTI_AMPUTATE].AddNextSkill( SKILL_CHARGING_POWER );

	m_pTypeInfo[SKILL_CHARGING_POWER].Set(sid++, "Charging Power", 0, 0, 20,"ÄÜÁ¿²¹³ä");
	m_pTypeInfo[SKILL_CHARGING_POWER].SetMP( 20 );
	m_pTypeInfo[SKILL_CHARGING_POWER].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_CHARGING_POWER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_CHARGING_POWER].AddNextSkill( SKILL_MOONLIGHT_SEVER );
	
	m_pTypeInfo[SKILL_MOONLIGHT_SEVER].Set(sid++, "Moonlight Sever", 0, 0, 24,"¶ÏÔÂÕ¶");
	m_pTypeInfo[SKILL_MOONLIGHT_SEVER].SetMP( 25 );
	m_pTypeInfo[SKILL_MOONLIGHT_SEVER].SetLearnLevel( 70 );
	m_pTypeInfo[SKILL_MOONLIGHT_SEVER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MOONLIGHT_SEVER].AddNextSkill( SKILL_EARTHQUAKE );

	m_pTypeInfo[SKILL_EARTHQUAKE].Set(sid++, "Earthquake", 0, 0, 118,"µØÕðÊõ");
	m_pTypeInfo[SKILL_EARTHQUAKE].SetMP( 20 );
	m_pTypeInfo[SKILL_EARTHQUAKE].SetLearnLevel( 75 );
	m_pTypeInfo[SKILL_EARTHQUAKE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_EARTHQUAKE].AddNextSkill( SKILL_BERSERKER );
	
	m_pTypeInfo[SKILL_BERSERKER].Set(sid++, "Berserker", 0, 0,  23,"¿ñ±©");
	m_pTypeInfo[SKILL_BERSERKER].SetMP( 25 );
	m_pTypeInfo[SKILL_BERSERKER].SetLearnLevel( 80 );
	m_pTypeInfo[SKILL_BERSERKER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_BERSERKER].AddNextSkill( SKILL_SHADOW_DANCING );
		
	m_pTypeInfo[SKILL_SHADOW_DANCING].Set(sid++, "Shadow Dancing", 0, 0,  152,"°µÓ°ÎèÕß");
	m_pTypeInfo[SKILL_SHADOW_DANCING].SetMP( 35 );
	m_pTypeInfo[SKILL_SHADOW_DANCING].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_SHADOW_DANCING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SHADOW_DANCING].AddNextSkill( SKILL_TYPHOON );

	m_pTypeInfo[SKILL_TYPHOON].Set(sid++, "Typhoon", 0, 0, 153,"Ì¨·çÇ¿»÷");
	m_pTypeInfo[SKILL_TYPHOON].SetMP( 35 );
	m_pTypeInfo[SKILL_TYPHOON].SetLearnLevel( 90 );
	m_pTypeInfo[SKILL_TYPHOON].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TYPHOON].AddNextSkill( SKILL_AIR_SHIELD );

	m_pTypeInfo[SKILL_AIR_SHIELD].Set(sid++, "Air Shield", 0, 0,  218,"ÔªÆø¶Ü");
	m_pTypeInfo[SKILL_AIR_SHIELD].SetMP( 35 );
	m_pTypeInfo[SKILL_AIR_SHIELD].SetLearnLevel( 95 );
	m_pTypeInfo[SKILL_AIR_SHIELD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_AIR_SHIELD].AddNextSkill( SKILL_POWER_OF_LAND );

	m_pTypeInfo[SKILL_POWER_OF_LAND].Set(sid++, "Power of Land", 0, 0,  219,"´óµØÄÜÁ¿");
	m_pTypeInfo[SKILL_POWER_OF_LAND].SetMP( 35 );
	m_pTypeInfo[SKILL_POWER_OF_LAND].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_POWER_OF_LAND].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_POWER_OF_LAND].AddNextSkill( SKILL_BLAZE_WALK );

	m_pTypeInfo[SKILL_BLAZE_WALK].Set(sid++, "Blaze Walk", 0, 0,  314,"»ðÑæÖ®²½");
	m_pTypeInfo[SKILL_BLAZE_WALK].SetMP( 35 );
	m_pTypeInfo[SKILL_BLAZE_WALK].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_BLAZE_WALK].SetLearnLevel( 110 );
	m_pTypeInfo[SKILL_BLAZE_WALK].AddNextSkill( SKILL_WILD_TYPHOON );

	m_pTypeInfo[SKILL_WILD_TYPHOON].Set(sid++, "Wild Typhoon", 0, 0,  255,"¿ñÒ°Ì¨·ç");
	m_pTypeInfo[SKILL_WILD_TYPHOON].SetMP( 40 );
	m_pTypeInfo[SKILL_WILD_TYPHOON].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WILD_TYPHOON].SetLearnLevel( 120 );
	m_pTypeInfo[SKILL_WILD_TYPHOON].AddNextSkill( SKILL_BURNING_SOUL_CHARGING );

	// 2004, 9, 8, sobeit add start - µµ 130 ½ºÅ³
	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].Set(sid++, "Burning Sol", 700, 270,  388,"È¼ÉÕÖ®»ê");
	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].SetMP( 40 );
	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_BURNING_SOUL_CHARGING].AddNextSkill( SKILL_SPIT_STREAM );
	// 2004, 9, 8, sobeit add end - µµ 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - µµ 140 ½ºÅ³
	m_pTypeInfo[SKILL_SPIT_STREAM].Set(sid++, "Spit Stream", 700, 270,  459,"ÅçÁ÷");
	m_pTypeInfo[SKILL_SPIT_STREAM].SetMP( 40 );
	m_pTypeInfo[SKILL_SPIT_STREAM].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SPIT_STREAM].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_SPIT_STREAM].AddNextSkill( SKILL_DRAGON_TORNADO );
	// 2004, 11, 11, sobeit add end - µµ 130 ½ºÅ³

	// by csm ÃÖ¼®¹Î 
	m_pTypeInfo[SKILL_DRAGON_TORNADO].Set(sid++, "Dragon Tornado", 700, 270,  469,"¾ÞÁú¾í·ç");
	m_pTypeInfo[SKILL_DRAGON_TORNADO].SetMP( 40 );
	m_pTypeInfo[SKILL_DRAGON_TORNADO].SetSkillStep( SKILL_STEP_SLAYER_BLADE_ADVANCEMENT );
	m_pTypeInfo[SKILL_DRAGON_TORNADO].SetLearnLevel( 150 );
	
#ifdef __NEW_SKILL__
	m_pTypeInfo[SKILL_DRAGON_TORNADO].AddNextSkill( SKILL_BOMB_CRASH_WALK );

	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].Set(sid++, "Bomb Crash Walk", 700, 270,  499,"¾ÞÅÚºäÕ¨");
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].SetMP( 40 );
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].SetSkillStep( SKILL_STEP_SLAYER_BLADE_ADVANCEMENT );
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].AddNextSkill( SKILL_SOUL_CHAIN );
#else
	m_pTypeInfo[SKILL_DRAGON_TORNADO].AddNextSkill( SKILL_SOUL_CHAIN );
#endif
	//----------------------------------------------------------------------     
	// ÃÑ
	//----------------------------------------------------------------------
	sid = 0;
	m_pTypeInfo[SKILL_FAST_RELOAD].Set(sid++, "Fast Reload", 0, 0,  26,"¿ìËÙ×°Ìî");
	m_pTypeInfo[SKILL_FAST_RELOAD].SetMP( 0 );
	m_pTypeInfo[SKILL_FAST_RELOAD].SetLearnLevel( 5 );
	m_pTypeInfo[SKILL_FAST_RELOAD].SetPassive();
	m_pTypeInfo[SKILL_FAST_RELOAD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_FAST_RELOAD].AddNextSkill( SKILL_DOUBLE_SHOT );

	m_pTypeInfo[SKILL_DOUBLE_SHOT].Set(sid++, "Double Shot", 0, 0,  27,"¶þÁ¬Éä");
	m_pTypeInfo[SKILL_DOUBLE_SHOT].SetMP( 0 );
	m_pTypeInfo[SKILL_DOUBLE_SHOT].SetLearnLevel( 10 );
	m_pTypeInfo[SKILL_DOUBLE_SHOT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DOUBLE_SHOT].AddNextSkill( SKILL_HEART_CATALYST );

//	m_pTypeInfo[SKILL_SMG_MASTERY].Set(sid++, "SMG Mastery", 0, 0, 28,"SMG¸¶½ºÅÍ¸®");
//	m_pTypeInfo[SKILL_SMG_MASTERY].SetMP( 0 );
//	m_pTypeInfo[SKILL_SMG_MASTERY].SetLearnLevel( 15 );
//	m_pTypeInfo[SKILL_SMG_MASTERY].SetPassive();
//	m_pTypeInfo[SKILL_SMG_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_SMG_MASTERY].AddNextSkill( SKILL_TRIPLE_SHOT );

	m_pTypeInfo[SKILL_HEART_CATALYST].Set(sid++, "Heart Catalyst", 0, 0, 329,"¿ìËÙ»Ö¸´");
	m_pTypeInfo[SKILL_HEART_CATALYST].SetMP( 0 );
	m_pTypeInfo[SKILL_HEART_CATALYST].SetLearnLevel( 15 );
	m_pTypeInfo[SKILL_HEART_CATALYST].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HEART_CATALYST].AddNextSkill( SKILL_TRIPLE_SHOT );

	m_pTypeInfo[SKILL_TRIPLE_SHOT].Set(sid++, "Triple Shot", 0, 0,  27,"ÈýÁ¬Éä");
	m_pTypeInfo[SKILL_TRIPLE_SHOT].SetMP( 0 );
	m_pTypeInfo[SKILL_TRIPLE_SHOT].SetLearnLevel( 20 );
	m_pTypeInfo[SKILL_TRIPLE_SHOT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TRIPLE_SHOT].AddNextSkill( SKILL_MULTI_SHOT );

	m_pTypeInfo[SKILL_MULTI_SHOT].Set(sid++, "Multi Shot", 0, 0,  29,"É¢Éä");
	m_pTypeInfo[SKILL_MULTI_SHOT].SetMP( 0 );
	m_pTypeInfo[SKILL_MULTI_SHOT].SetLearnLevel( 25 );
	m_pTypeInfo[SKILL_MULTI_SHOT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MULTI_SHOT].AddNextSkill( SKILL_HEAD_SHOT );

	m_pTypeInfo[SKILL_HEAD_SHOT].Set(sid++, "Head Shot", 0, 0,  30,"±¬Í·");
	m_pTypeInfo[SKILL_HEAD_SHOT].SetMP( 0 );
	m_pTypeInfo[SKILL_HEAD_SHOT].SetLearnLevel( 30 );
	m_pTypeInfo[SKILL_HEAD_SHOT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HEAD_SHOT].AddNextSkill( SKILL_ARMS_MASTERY1 );

	m_pTypeInfo[SKILL_ARMS_MASTERY1].Set(sid++, "Arms Mastery 1", 0, 0, 325,"Ç¹ÐµÊìÁ·¶È 1");
	m_pTypeInfo[SKILL_ARMS_MASTERY1].SetMP( 0 );
	m_pTypeInfo[SKILL_ARMS_MASTERY1].SetLearnLevel( 35 );
	m_pTypeInfo[SKILL_ARMS_MASTERY1].SetPassive();
	m_pTypeInfo[SKILL_ARMS_MASTERY1].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ARMS_MASTERY1].AddNextSkill( SKILL_QUICK_FIRE );

//	m_pTypeInfo[SKILL_AR_MASTERY].Set(sid++, "AR Mastery", 0, 0, 31,"AR¸¶½ºÅÍ¸®");
//	m_pTypeInfo[SKILL_AR_MASTERY].SetMP( 0 );
//	m_pTypeInfo[SKILL_AR_MASTERY].SetLearnLevel( 35 );
//	m_pTypeInfo[SKILL_AR_MASTERY].SetPassive();
//	m_pTypeInfo[SKILL_AR_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_AR_MASTERY].AddNextSkill( SKILL_QUICK_FIRE );
	
	m_pTypeInfo[SKILL_QUICK_FIRE].Set(sid++, "Quick Fire", 0, 0,  27,"¼±ËÙ»ðÑæ");
	m_pTypeInfo[SKILL_QUICK_FIRE].SetMP( 0 );
	m_pTypeInfo[SKILL_QUICK_FIRE].SetLearnLevel( 40 );
	m_pTypeInfo[SKILL_QUICK_FIRE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_QUICK_FIRE].AddNextSkill( SKILL_SNIPPING );

	m_pTypeInfo[SKILL_SNIPPING].Set(sid++, "Snipping", 0, 0, 33,"¾Ñ»÷");
	m_pTypeInfo[SKILL_SNIPPING].SetMP( 0 );
	m_pTypeInfo[SKILL_SNIPPING].SetLearnLevel( 45 );
	m_pTypeInfo[SKILL_SNIPPING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SNIPPING].AddNextSkill( SKILL_LIVENESS );

	m_pTypeInfo[SKILL_LIVENESS].Set(sid++, "Liveness", 0, 0, 176,"Ãð¾ø");
	m_pTypeInfo[SKILL_LIVENESS].SetPassive();
	m_pTypeInfo[SKILL_LIVENESS].SetLearnLevel( 45 );
	m_pTypeInfo[SKILL_LIVENESS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_LIVENESS].AddNextSkill( SKILL_PIERCING );

	m_pTypeInfo[SKILL_PIERCING].Set(sid++, "Piercing", 0, 0, 32,"´©´ÌÉä»÷");
	m_pTypeInfo[SKILL_PIERCING].SetMP( 0 );
	m_pTypeInfo[SKILL_PIERCING].SetLearnLevel( 47 );
	m_pTypeInfo[SKILL_PIERCING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_PIERCING].AddNextSkill( SKILL_VIVID_MAGAZINE );

	m_pTypeInfo[SKILL_VIVID_MAGAZINE].Set(sid++, "Vivid Magazine", 0, 0, 335,"´óÈÝÁ¿Ìî×°");
	m_pTypeInfo[SKILL_VIVID_MAGAZINE].SetMP( 0 );
	m_pTypeInfo[SKILL_VIVID_MAGAZINE].SetLearnLevel( 50 );
	m_pTypeInfo[SKILL_VIVID_MAGAZINE].SetPassive();
	m_pTypeInfo[SKILL_VIVID_MAGAZINE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_VIVID_MAGAZINE].AddNextSkill( SKILL_THROW_BOMB );

//	m_pTypeInfo[SKILL_SG_MASTERY].Set(sid++, "SG Mastery", 0, 0, 34,"SG¸¶½ºÅÍ¸®");
//	m_pTypeInfo[SKILL_SG_MASTERY].SetMP( 0 );
//	m_pTypeInfo[SKILL_SG_MASTERY].SetLearnLevel( 50 );
//	m_pTypeInfo[SKILL_SG_MASTERY].SetPassive();
//	m_pTypeInfo[SKILL_SG_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_SG_MASTERY].AddNextSkill( SKILL_THROW_BOMB );

	m_pTypeInfo[SKILL_THROW_BOMB].Set(sid++, "Throw Bomb", 0, 0, 36,"Õ¨µ¯Í¶ÖÀ");
	m_pTypeInfo[SKILL_THROW_BOMB].SetMP( 10 );	
	m_pTypeInfo[SKILL_THROW_BOMB].SetLearnLevel( 53 );
	//m_pTypeInfo[SKILL_THROW_BOMB].SetPassive();
	m_pTypeInfo[SKILL_THROW_BOMB].SetActive();
	m_pTypeInfo[SKILL_THROW_BOMB].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_THROW_BOMB].AddNextSkill( SKILL_CONCEALMENT );	

	m_pTypeInfo[SKILL_CONCEALMENT].Set(sid++, "Concealment", 0, 0, 249,"ÉñÊ¥Ö®¶Ü");
	m_pTypeInfo[SKILL_CONCEALMENT].SetMP( 10 );
	m_pTypeInfo[SKILL_CONCEALMENT].SetLearnLevel( 55 );
	m_pTypeInfo[SKILL_CONCEALMENT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_CONCEALMENT].AddNextSkill(SKILL_DETECT_MINE );

	m_pTypeInfo[SKILL_DETECT_MINE].Set(sid++, "Revealer", 0, 0, 35,"ÆôÊ¾Õß");
	m_pTypeInfo[SKILL_DETECT_MINE].SetMP( 20 );
	m_pTypeInfo[SKILL_DETECT_MINE].SetLearnLevel( 57 );
	m_pTypeInfo[SKILL_DETECT_MINE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DETECT_MINE].AddNextSkill( SKILL_TRIDENT );
	m_pTypeInfo[SKILL_DETECT_MINE].SetActive();	

	m_pTypeInfo[SKILL_TRIDENT].Set(sid++, "Trident", 0, 0, 321,"±¬ÁÑÉä»÷");
	m_pTypeInfo[SKILL_TRIDENT].SetMP( 10 );	
	m_pTypeInfo[SKILL_TRIDENT].SetLearnLevel( 60 );
	m_pTypeInfo[SKILL_TRIDENT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TRIDENT].AddNextSkill( SKILL_ARMS_MASTERY2 );
	
//	m_pTypeInfo[SKILL_MAKE_BOMB].Set(sid++, "Make Bomb", 0, 0, 37,"Å©¸®¿¡ÀÌÆ®ºÕ");
//	m_pTypeInfo[SKILL_MAKE_BOMB].SetMP( 10 );	
//	m_pTypeInfo[SKILL_MAKE_BOMB].SetLearnLevel( 60 );
//	m_pTypeInfo[SKILL_MAKE_BOMB].SetActive();
//	m_pTypeInfo[SKILL_MAKE_BOMB].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_MAKE_BOMB].AddNextSkill( SKILL_ARMS_MASTERY2 );

	m_pTypeInfo[SKILL_ARMS_MASTERY2].Set(sid++, "Arms Mastery 2", 0, 0, 324,"Ç¹ÐµÊìÁ·¶È 2");
	m_pTypeInfo[SKILL_ARMS_MASTERY2].SetMP( 0 );
	m_pTypeInfo[SKILL_ARMS_MASTERY2].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_ARMS_MASTERY2].SetPassive();
	m_pTypeInfo[SKILL_ARMS_MASTERY2].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ARMS_MASTERY2].AddNextSkill( SKILL_INSTALL_MINE );
		
//	m_pTypeInfo[SKILL_SR_MASTERY].Set(sid++, "SR Mastery", 0, 0, 38,"SR¸¶½ºÅÍ¸®");
//	m_pTypeInfo[SKILL_SR_MASTERY].SetMP( 0 );
//	m_pTypeInfo[SKILL_SR_MASTERY].SetLearnLevel( 65 );
//	m_pTypeInfo[SKILL_SR_MASTERY].SetPassive();
//	m_pTypeInfo[SKILL_SR_MASTERY].SetSkillStep( SKILL_STEP_APPRENTICE );
//	m_pTypeInfo[SKILL_SR_MASTERY].AddNextSkill( SKILL_INSTALL_MINE );
	
	m_pTypeInfo[SKILL_INSTALL_MINE].Set(sid++, "Install Mine", 0, 0, 39,"µØÀ×°²×°");
	m_pTypeInfo[SKILL_INSTALL_MINE].SetMP( 20 );
	m_pTypeInfo[SKILL_INSTALL_MINE].SetLearnLevel( 70 );
   	m_pTypeInfo[SKILL_INSTALL_MINE].SetActive();
	m_pTypeInfo[SKILL_INSTALL_MINE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_INSTALL_MINE].AddNextSkill( SKILL_MAKE_MINE );

	m_pTypeInfo[SKILL_MAKE_MINE].Set(sid++, "Create Mine", 0, 0, 40,"µØÀ×ÖÆÔì");
	m_pTypeInfo[SKILL_MAKE_MINE].SetMP( 20 );
	m_pTypeInfo[SKILL_MAKE_MINE].SetLearnLevel( 75 );
   	m_pTypeInfo[SKILL_MAKE_MINE].SetActive();
	m_pTypeInfo[SKILL_MAKE_MINE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MAKE_MINE].AddNextSkill( SKILL_INSTALL_TRAP );

	m_pTypeInfo[SKILL_INSTALL_TRAP].Set(sid++, "Install Trap", 0, 0, 331,"·ÅÖÃÏÝÚå");
	m_pTypeInfo[SKILL_INSTALL_TRAP].SetMP( 20 );
	m_pTypeInfo[SKILL_INSTALL_TRAP].SetLearnLevel( 75 );
   	m_pTypeInfo[SKILL_INSTALL_TRAP].SetActive();
	m_pTypeInfo[SKILL_INSTALL_TRAP].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_INSTALL_TRAP].AddNextSkill( SKILL_OBSERVING_EYE );

	m_pTypeInfo[SKILL_OBSERVING_EYE].Set(sid++, "Observing Eye", 0, 0, 159,"×¨×¢Ö®ÑÛ");
	m_pTypeInfo[SKILL_OBSERVING_EYE].SetMP( 20 );
	m_pTypeInfo[SKILL_OBSERVING_EYE].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_OBSERVING_EYE].SetSkillStep( SKILL_STEP_APPRENTICE );
   	m_pTypeInfo[SKILL_OBSERVING_EYE].AddNextSkill( SKILL_MOLE_SHOT );

	m_pTypeInfo[SKILL_MOLE_SHOT].Set(sid++, "Mole Shot", 0, 0, 333,"Ä¦¶ûÉ¢Éä");
	m_pTypeInfo[SKILL_MOLE_SHOT].SetMP( 20 );
	m_pTypeInfo[SKILL_MOLE_SHOT].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_MOLE_SHOT].SetSkillStep( SKILL_STEP_APPRENTICE );
   	m_pTypeInfo[SKILL_MOLE_SHOT].AddNextSkill( SKILL_BULLET_OF_LIGHT );

	m_pTypeInfo[SKILL_BULLET_OF_LIGHT].Set(sid++, "Bullet of Light", 0, 0, 224,"¹âÖ®×Óµ¯");
	m_pTypeInfo[SKILL_BULLET_OF_LIGHT].SetMP( 20 );
	m_pTypeInfo[SKILL_BULLET_OF_LIGHT].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_BULLET_OF_LIGHT].SetSkillStep( SKILL_STEP_APPRENTICE );
   	m_pTypeInfo[SKILL_BULLET_OF_LIGHT].AddNextSkill( SKILL_GUN_SHOT_GUIDANCE );

	m_pTypeInfo[SKILL_GUN_SHOT_GUIDANCE].Set(sid++, "Gunshot Guidance", 0, 0, 225,"ÅÚ»ðÖ§Ô®");
	m_pTypeInfo[SKILL_GUN_SHOT_GUIDANCE].SetMP( 20 );
	m_pTypeInfo[SKILL_GUN_SHOT_GUIDANCE].SetLearnLevel( 95 );
	m_pTypeInfo[SKILL_GUN_SHOT_GUIDANCE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_GUN_SHOT_GUIDANCE].AddNextSkill( SKILL_JABBING_VEIN );

	m_pTypeInfo[SKILL_JABBING_VEIN].Set(sid++, "Jabbing Vein", 0, 0, 315,"·ÅÑªÊõ");
	m_pTypeInfo[SKILL_JABBING_VEIN].SetMP( 20 );
	m_pTypeInfo[SKILL_JABBING_VEIN].SetLearnLevel( 110 );
	m_pTypeInfo[SKILL_JABBING_VEIN].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_JABBING_VEIN].AddNextSkill( SKILL_ULTIMATE_BLOW );

	m_pTypeInfo[SKILL_ULTIMATE_BLOW].Set(sid++, "Ultimate Blow", 0, 0, 256,"ÖÕ¼«Í»Ï®");
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].SetMP( 25 );
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].SetLearnLevel( 120 );
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].AddNextSkill( SKILL_INSTALL_TURRET );
	

	// 2004, 9, 8, sobeit add start - ÃÑ 130 ½ºÅ³
	m_pTypeInfo[SKILL_INSTALL_TURRET].Set(sid++, "Install Turret", 700, 270,  389,"°²×°ÅÚËþ");
	m_pTypeInfo[SKILL_INSTALL_TURRET].SetMP( 40 );
	m_pTypeInfo[SKILL_INSTALL_TURRET].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_INSTALL_TURRET].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_INSTALL_TURRET].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_INSTALL_TURRET].AddNextSkill( SKILL_PLASMA_ROCKET_LAUNCHER );
	// 2004, 9, 8, sobeit add end - ÃÑ 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - ÃÑ 140 ½ºÅ³
	m_pTypeInfo[SKILL_PLASMA_ROCKET_LAUNCHER].Set(sid++, "Plasma Rocket Luncher", 700, 270,  460,"Àë×Ó»ð¼ý·¢ÉäÆ÷");
	m_pTypeInfo[SKILL_PLASMA_ROCKET_LAUNCHER].SetMP( 40 );
	m_pTypeInfo[SKILL_PLASMA_ROCKET_LAUNCHER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_PLASMA_ROCKET_LAUNCHER].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_PLASMA_ROCKET_LAUNCHER].AddNextSkill( SKILL_HARPOON_BOMB );
	// 2004, 11, 11, sobeit add end - ÃÑ 140 ½ºÅ³

	m_pTypeInfo[SKILL_HARPOON_BOMB].Set(sid++, "Harpoon Bomb", 700, 270,  471,"Óã²æÕ¨µ¯");
	m_pTypeInfo[SKILL_HARPOON_BOMB].SetMP( 40 );
	m_pTypeInfo[SKILL_HARPOON_BOMB].SetSkillStep( SKILL_STEP_SLAYER_GUN_BOMB_ADVANCEMENT );
	m_pTypeInfo[SKILL_HARPOON_BOMB].SetLearnLevel( 150 );
	
#ifdef __NEW_SKILL__
	// add by Coffee  ÈËÀàÐÂ¼¼ÄÜ
	m_pTypeInfo[SKILL_HARPOON_BOMB].AddNextSkill( SKILL_SATELLITE_BOMB );
	m_pTypeInfo[SKILL_SATELLITE_BOMB].Set(sid++, "Satellite Bomb", 0, 0, 500,"ÎÀÐÇºä»÷");
	m_pTypeInfo[SKILL_SATELLITE_BOMB].SetMP( 60 );
	m_pTypeInfo[SKILL_SATELLITE_BOMB].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_SATELLITE_BOMB].SetSkillStep( SKILL_STEP_SLAYER_GUN_BOMB_ADVANCEMENT );
	m_pTypeInfo[SKILL_SATELLITE_BOMB].AddNextSkill( SKILL_SOUL_CHAIN );
	// end 
#else
	m_pTypeInfo[SKILL_HARPOON_BOMB].AddNextSkill( SKILL_SOUL_CHAIN );
#endif
	
	//----------------------------------------------------------------------     
	// Heal
	//----------------------------------------------------------------------	
	sid = 0;
	m_pTypeInfo[MAGIC_CURE_LIGHT_WOUNDS].Set(sid++, "Cure Light Wounds", 0, 0, 41,"ÖÎÁÆÊõ");
	m_pTypeInfo[MAGIC_CURE_LIGHT_WOUNDS].SetMP( 6 );
	m_pTypeInfo[MAGIC_CURE_LIGHT_WOUNDS].SetLearnLevel( 5 );
	m_pTypeInfo[MAGIC_CURE_LIGHT_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CURE_LIGHT_WOUNDS].AddNextSkill( SKILL_HOLY_ARROW );	
	
	m_pTypeInfo[SKILL_HOLY_ARROW].Set(sid++, "HOLY_ARROW", 0, 0, 214,"ÉñÊ¥¼ýÊ¸");
	m_pTypeInfo[SKILL_HOLY_ARROW].SetMP( 4 );
	m_pTypeInfo[SKILL_HOLY_ARROW].SetLearnLevel( 8 );
	m_pTypeInfo[SKILL_HOLY_ARROW].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HOLY_ARROW].AddNextSkill( MAGIC_CURE_POISON );	

	m_pTypeInfo[MAGIC_CURE_POISON].Set(sid++, "Cure Poison", 0, 0, 42,"½â¶¾");
	m_pTypeInfo[MAGIC_CURE_POISON].SetMP( 4 );
	m_pTypeInfo[MAGIC_CURE_POISON].SetLearnLevel( 10 );
	m_pTypeInfo[MAGIC_CURE_POISON].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CURE_POISON].AddNextSkill( MAGIC_PROTECTION_FROM_POISON );	

	m_pTypeInfo[MAGIC_PROTECTION_FROM_POISON].Set(sid++, "Protection From Poison", 0, 0, 43,"¶¾ÒºµÖ¿¹");
	m_pTypeInfo[MAGIC_PROTECTION_FROM_POISON].SetMP( 6 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_POISON].SetLearnLevel( 15 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_POISON].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_POISON].AddNextSkill( MAGIC_CAUSE_LIGHT_WOUNDS );
		
	m_pTypeInfo[MAGIC_CAUSE_LIGHT_WOUNDS].Set(sid++, "Cause Light Wounds", 0, 0, 44,"¹â»÷Êõ");
	m_pTypeInfo[MAGIC_CAUSE_LIGHT_WOUNDS].SetMP( 4 );
	m_pTypeInfo[MAGIC_CAUSE_LIGHT_WOUNDS].SetLearnLevel( 20 );
	m_pTypeInfo[MAGIC_CAUSE_LIGHT_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CAUSE_LIGHT_WOUNDS].AddNextSkill( MAGIC_CURE_SERIOUS_WOUNDS );	
	
	m_pTypeInfo[MAGIC_CURE_SERIOUS_WOUNDS].Set(sid++, "Cure Serious Wounds", 0, 0, 45,"ÉËº¦ÖÎÁÆÊõ");
	m_pTypeInfo[MAGIC_CURE_SERIOUS_WOUNDS].SetMP( 4 );
	m_pTypeInfo[MAGIC_CURE_SERIOUS_WOUNDS].SetLearnLevel( 25 );
	m_pTypeInfo[MAGIC_CURE_SERIOUS_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CURE_SERIOUS_WOUNDS].AddNextSkill( MAGIC_REMOVE_CURSE );	
	
	m_pTypeInfo[MAGIC_REMOVE_CURSE].Set(sid++, "Remove Curse", 0, 0, 46,"½â³ý×çÖä");
	m_pTypeInfo[MAGIC_REMOVE_CURSE].SetMP( 4 );
	m_pTypeInfo[MAGIC_REMOVE_CURSE].SetLearnLevel( 30 );
	m_pTypeInfo[MAGIC_REMOVE_CURSE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_REMOVE_CURSE].AddNextSkill( MAGIC_PROTECTION_FROM_CURSE );	
		
	m_pTypeInfo[MAGIC_PROTECTION_FROM_CURSE].Set(sid++, "Protection From Curse", 0, 0, 47,"×çÖä¿¹Á¦");//207);
	m_pTypeInfo[MAGIC_PROTECTION_FROM_CURSE].SetMP( 12 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_CURSE].SetLearnLevel( 35 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_CURSE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_CURSE].AddNextSkill( MAGIC_RESURRECT );	
	
	m_pTypeInfo[MAGIC_RESURRECT].Set(sid++, "Resurrect", 0, 0, 126,"¸´»î");
	m_pTypeInfo[MAGIC_RESURRECT].SetMP( 4 );
	m_pTypeInfo[MAGIC_RESURRECT].SetLearnLevel( 40 );
	m_pTypeInfo[MAGIC_RESURRECT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_RESURRECT].AddNextSkill( MAGIC_CAUSE_SERIOUS_WOUNDS );	

	m_pTypeInfo[MAGIC_CAUSE_SERIOUS_WOUNDS].Set(sid++, "Cause Serious Wounds", 0, 0, 48,"¸ß¼¶¹â»÷Êõ");
	m_pTypeInfo[MAGIC_CAUSE_SERIOUS_WOUNDS].SetMP( 4 );
	m_pTypeInfo[MAGIC_CAUSE_SERIOUS_WOUNDS].SetLearnLevel( 45 );
	m_pTypeInfo[MAGIC_CAUSE_SERIOUS_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CAUSE_SERIOUS_WOUNDS].AddNextSkill( SKILL_CREATE_HOLY_POTION );	

	m_pTypeInfo[SKILL_CREATE_HOLY_POTION].Set(sid++, "Create Holy Potion", 0, 0, 327,"Ê¥Ë®ÖÆÔì");
	m_pTypeInfo[SKILL_CREATE_HOLY_POTION].SetMP( 4 );
	m_pTypeInfo[SKILL_CREATE_HOLY_POTION].SetLearnLevel( 47 );
	m_pTypeInfo[SKILL_CREATE_HOLY_POTION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_CREATE_HOLY_POTION].AddNextSkill( MAGIC_CURE_CRITICAL_WOUNDS );	
	
	m_pTypeInfo[MAGIC_CURE_CRITICAL_WOUNDS].Set(sid++, "Cure Critical Wounds", 0, 0, 49,"¸ß¼¶ÖÎÁÆÊõ");
	m_pTypeInfo[MAGIC_CURE_CRITICAL_WOUNDS].SetMP( 4 );
	m_pTypeInfo[MAGIC_CURE_CRITICAL_WOUNDS].SetLearnLevel( 50 );
	m_pTypeInfo[MAGIC_CURE_CRITICAL_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CURE_CRITICAL_WOUNDS].AddNextSkill( MAGIC_PRAYER );	
	
	m_pTypeInfo[MAGIC_PRAYER].Set(sid++, "Prayer", 0, 0, 125,"Æíµ»");
	m_pTypeInfo[MAGIC_PRAYER].SetMP( 12 );
	m_pTypeInfo[MAGIC_PRAYER].SetLearnLevel( 55 );
	m_pTypeInfo[MAGIC_PRAYER].SetPassive();
	m_pTypeInfo[MAGIC_PRAYER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_PRAYER].AddNextSkill( SKILL_DENIAL_MAGIC );		
	
	m_pTypeInfo[SKILL_DENIAL_MAGIC].Set(sid++, "Denial Magic", 0, 0, 248 ,"Ä§·¨ÃâÒß");
	m_pTypeInfo[SKILL_DENIAL_MAGIC].SetMP( 12 );
	m_pTypeInfo[SKILL_DENIAL_MAGIC].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_DENIAL_MAGIC].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DENIAL_MAGIC].AddNextSkill( MAGIC_PROTECTION_FROM_ACID );

	m_pTypeInfo[MAGIC_PROTECTION_FROM_ACID].Set(sid++, "Protection From Acid", 0, 0, 50,"ËáÐÔ·À»¤");
	m_pTypeInfo[MAGIC_PROTECTION_FROM_ACID].SetMP( 12 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_ACID].SetLearnLevel( 60 );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_ACID].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_PROTECTION_FROM_ACID].AddNextSkill( MAGIC_SACRIFICE );		
		
	m_pTypeInfo[MAGIC_SACRIFICE].Set(sid++, "Sacrifice", 0, 0, 51,"ÎþÉü");
	m_pTypeInfo[MAGIC_SACRIFICE].SetMP( 12 );
	m_pTypeInfo[MAGIC_SACRIFICE].SetLearnLevel( 65 );
	m_pTypeInfo[MAGIC_SACRIFICE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_SACRIFICE].AddNextSkill( MAGIC_CAUSE_CRITICAL_WOUNDS );		
	
	m_pTypeInfo[MAGIC_CAUSE_CRITICAL_WOUNDS].Set(sid++, "Cause Critical Wounds", 0, 0, 52,"²Ã¾öÖ®¼ý");
	m_pTypeInfo[MAGIC_CAUSE_CRITICAL_WOUNDS].SetMP( 4 );
	m_pTypeInfo[MAGIC_CAUSE_CRITICAL_WOUNDS].SetLearnLevel( 70 );
	m_pTypeInfo[MAGIC_CAUSE_CRITICAL_WOUNDS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CAUSE_CRITICAL_WOUNDS].AddNextSkill( SKILL_PROTECTION_FROM_BLOOD );	

	m_pTypeInfo[SKILL_PROTECTION_FROM_BLOOD].Set(sid++, "Protection From Blood", 0, 0, 334,"Ñª¶Ü");
	m_pTypeInfo[SKILL_PROTECTION_FROM_BLOOD].SetMP( 4 );
	m_pTypeInfo[SKILL_PROTECTION_FROM_BLOOD].SetLearnLevel( 73 );
	m_pTypeInfo[SKILL_PROTECTION_FROM_BLOOD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_PROTECTION_FROM_BLOOD].AddNextSkill( MAGIC_CURE_ALL );	

	m_pTypeInfo[MAGIC_CURE_ALL].Set(sid++, "Cure All", 0, 0, 53,"È«ÌåÖÎÁÆ");
	m_pTypeInfo[MAGIC_CURE_ALL].SetMP( 30 );
	m_pTypeInfo[MAGIC_CURE_ALL].SetLearnLevel( 75 );
	m_pTypeInfo[MAGIC_CURE_ALL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CURE_ALL].AddNextSkill( SKILL_VIGOR_DROP );

	m_pTypeInfo[SKILL_VIGOR_DROP].Set(sid++, "Vigor Drop", 0, 0, 54,"Ê¥¹âÓê");
	m_pTypeInfo[SKILL_VIGOR_DROP].SetMP( 30 );
	m_pTypeInfo[SKILL_VIGOR_DROP].SetLearnLevel( 80 );
	m_pTypeInfo[SKILL_VIGOR_DROP].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_VIGOR_DROP].AddNextSkill( SKILL_PEACE );

	m_pTypeInfo[SKILL_PEACE].Set(sid++, "Peace", 0, 0, 160,"Ã÷¾µÖ¹Ë®");
	m_pTypeInfo[SKILL_PEACE].SetMP( 30 );
	m_pTypeInfo[SKILL_PEACE].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_PEACE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_PEACE].AddNextSkill( SKILL_ETERNITY );

	m_pTypeInfo[SKILL_ETERNITY].Set(sid++, "Eternity", 0, 0, 328,"×ªÉúÊõ");
	m_pTypeInfo[SKILL_ETERNITY].SetMP( 30 );
	m_pTypeInfo[SKILL_ETERNITY].SetLearnLevel( 88 );
	m_pTypeInfo[SKILL_ETERNITY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ETERNITY].AddNextSkill( SKILL_ACTIVATION );
	
	m_pTypeInfo[SKILL_ACTIVATION].Set(sid++, "Activation", 0, 0, 162,"ÌìÊ¹Ö®´Í¸£");
	m_pTypeInfo[SKILL_ACTIVATION].SetMP( 25 );
	m_pTypeInfo[SKILL_ACTIVATION].SetLearnLevel( 90 );
	m_pTypeInfo[SKILL_ACTIVATION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ACTIVATION].AddNextSkill( MAGIC_REGENERATION );

	m_pTypeInfo[MAGIC_REGENERATION].Set(sid++, "Regeneration", 0, 0, 220,"¹â»ÔÁìÓò");
	m_pTypeInfo[MAGIC_REGENERATION].SetMP( 30 );
	m_pTypeInfo[MAGIC_REGENERATION].SetLearnLevel( 95 );
	m_pTypeInfo[MAGIC_REGENERATION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_REGENERATION].AddNextSkill( SKILL_TURN_UNDEAD );

	m_pTypeInfo[SKILL_TURN_UNDEAD].Set(sid++, "Turn Undead", 0, 0, 221,"²»ËÀ×ª»»");
	m_pTypeInfo[SKILL_TURN_UNDEAD].SetMP( 30 );
	m_pTypeInfo[SKILL_TURN_UNDEAD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_TURN_UNDEAD].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_TURN_UNDEAD].AddNextSkill( SKILL_GREAT_HEAL );

	m_pTypeInfo[SKILL_GREAT_HEAL].Set(sid++, "Great Heal", 0, 0, 316,"Ç¿Á¦ÖÎÓúÊõ");
	m_pTypeInfo[SKILL_GREAT_HEAL].SetMP( 30 );
	m_pTypeInfo[SKILL_GREAT_HEAL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_GREAT_HEAL].SetLearnLevel( 110 );
	m_pTypeInfo[SKILL_GREAT_HEAL].AddNextSkill( SKILL_ILLENDUE );

	m_pTypeInfo[SKILL_ILLENDUE].Set(sid++, "Illendue", 0, 0, 257,"ÐÞÂÞ-ÆÆ");
	m_pTypeInfo[SKILL_ILLENDUE].SetMP( 40 );
	m_pTypeInfo[SKILL_ILLENDUE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ILLENDUE].SetLearnLevel( 120 );
	m_pTypeInfo[SKILL_ILLENDUE].AddNextSkill( SKILL_WHITSUNTIDE );
	
	// 2004, 9, 8, sobeit add start - Èú 130 ½ºÅ³
	m_pTypeInfo[SKILL_WHITSUNTIDE].Set(sid++, "whitsuntide", 700, 270,  390,"Ì«Ñô·ç");
	m_pTypeInfo[SKILL_WHITSUNTIDE].SetMP( 40 );
	m_pTypeInfo[SKILL_WHITSUNTIDE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_WHITSUNTIDE].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_WHITSUNTIDE].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_WHITSUNTIDE].AddNextSkill( SKILL_BOMBING_STAR );
	// 2004, 9, 8, sobeit add end - Èú 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - Èú 140 ½ºÅ³
	m_pTypeInfo[SKILL_BOMBING_STAR].Set(sid++, "Bombing Star", 700, 270,  461,"±¬ÆÆÖ®ÐÇ");
	m_pTypeInfo[SKILL_BOMBING_STAR].SetMP( 40 );
	m_pTypeInfo[SKILL_BOMBING_STAR].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_BOMBING_STAR].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_BOMBING_STAR].AddNextSkill( SKILL_PASSING_HEAL );
	// 2004, 11, 11, sobeit add end - Èú 140 ½ºÅ³

	m_pTypeInfo[SKILL_PASSING_HEAL].Set(sid++, "Passing Heal", 700, 270,  472,"´«µÝÖÎÁÆ");
	m_pTypeInfo[SKILL_PASSING_HEAL].SetMP( 40 );
	m_pTypeInfo[SKILL_PASSING_HEAL].SetSkillStep( SKILL_STEP_SLAYER_HEAL_ADVANCEMENT );
	m_pTypeInfo[SKILL_PASSING_HEAL].SetLearnLevel( 150 );
	
#ifdef __NEW_SKILL__
	m_pTypeInfo[SKILL_PASSING_HEAL].AddNextSkill( SKILL_ILLUSION_INVERSION );
// add by Coffee 2007-3-25
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].Set(sid++, "Illusion Inversion", 700, 270,  501,"¿Ö²À»Ã¾õ");
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].SetMP( 40 );
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].SetSkillStep( SKILL_STEP_SLAYER_HEAL_ADVANCEMENT );
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].AddNextSkill( SKILL_SOUL_CHAIN );
// end 

#else
	m_pTypeInfo[SKILL_PASSING_HEAL].AddNextSkill( SKILL_SOUL_CHAIN );
#endif


	
	//----------------------------------------------------------------------     
	// Enchant
	//----------------------------------------------------------------------	
	sid = 0;
	m_pTypeInfo[MAGIC_CREATE_HOLY_WATER].Set(sid++, "Create Holy Water", 0, 0, 57,"Ê¥Ë®ÖÆÔì");
	m_pTypeInfo[MAGIC_CREATE_HOLY_WATER].SetMP( 6 );
	m_pTypeInfo[MAGIC_CREATE_HOLY_WATER].SetLearnLevel( 5 );
	m_pTypeInfo[MAGIC_CREATE_HOLY_WATER].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CREATE_HOLY_WATER].AddNextSkill( SKILL_LIGHT_BALL );
	
	m_pTypeInfo[SKILL_LIGHT_BALL].Set(sid++, "Light Ball", 0, 0, 215,"¹âÇò");
	m_pTypeInfo[SKILL_LIGHT_BALL].SetMP( 6 );
	m_pTypeInfo[SKILL_LIGHT_BALL].SetLearnLevel( 8 );
	m_pTypeInfo[SKILL_LIGHT_BALL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_LIGHT_BALL].AddNextSkill( MAGIC_LIGHT );
	
	m_pTypeInfo[MAGIC_LIGHT].Set(sid++, "Light", 0, 0, 58,"ÕÕÃ÷Êõ");
	m_pTypeInfo[MAGIC_LIGHT].SetMP( 4 );
	m_pTypeInfo[MAGIC_LIGHT].SetLearnLevel( 10 );
	m_pTypeInfo[MAGIC_LIGHT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_LIGHT].AddNextSkill( MAGIC_DETECT_HIDDEN );

	m_pTypeInfo[MAGIC_DETECT_HIDDEN].Set(sid++, "Detect Hidden", 0, 0, 59,"ÕÕÃ÷Êõ");
	m_pTypeInfo[MAGIC_DETECT_HIDDEN].SetMP( 4 );
	m_pTypeInfo[MAGIC_DETECT_HIDDEN].SetLearnLevel( 15 );
	m_pTypeInfo[MAGIC_DETECT_HIDDEN].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_DETECT_HIDDEN].AddNextSkill( MAGIC_AURA_BALL );
	
	m_pTypeInfo[MAGIC_AURA_BALL].Set(sid++, "Aura Ball", 0, 0, 60,"Ì½²âÊõ");
	m_pTypeInfo[MAGIC_AURA_BALL].SetMP( 6 );
	m_pTypeInfo[MAGIC_AURA_BALL].SetLearnLevel( 20 );
	m_pTypeInfo[MAGIC_AURA_BALL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_AURA_BALL].AddNextSkill( MAGIC_BLESS );

	m_pTypeInfo[MAGIC_BLESS].Set(sid++, "Bless", 0, 0, 61,"´óµØ×£¸£");
	m_pTypeInfo[MAGIC_BLESS].SetMP( 6 );
	m_pTypeInfo[MAGIC_BLESS].SetLearnLevel( 25 );
	m_pTypeInfo[MAGIC_BLESS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_BLESS].AddNextSkill( MAGIC_CONTINUAL_LIGHT );
			
	m_pTypeInfo[MAGIC_CONTINUAL_LIGHT].Set(sid++, "Continual Light", 0, 0, 62,"¸ß¼¶ÕÕÃ÷Êõ");
	m_pTypeInfo[MAGIC_CONTINUAL_LIGHT].SetMP( 10 );
	m_pTypeInfo[MAGIC_CONTINUAL_LIGHT].SetLearnLevel( 30 );
	m_pTypeInfo[MAGIC_CONTINUAL_LIGHT].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_CONTINUAL_LIGHT].AddNextSkill( MAGIC_FLARE );
		
	m_pTypeInfo[MAGIC_FLARE].Set(sid++, "Flare", 0, 0, 63,"¹âÒ«Ö®»·");
	m_pTypeInfo[MAGIC_FLARE].SetMP( 10 );
	m_pTypeInfo[MAGIC_FLARE].SetLearnLevel( 35 );
	m_pTypeInfo[MAGIC_FLARE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_FLARE].AddNextSkill( MAGIC_PURIFY );

	m_pTypeInfo[MAGIC_PURIFY].Set(sid++, "Purify", 0, 0, 64,"¾»»¯Êõ");
	m_pTypeInfo[MAGIC_PURIFY].SetMP( 10 );
	m_pTypeInfo[MAGIC_PURIFY].SetLearnLevel( 40 );
	m_pTypeInfo[MAGIC_PURIFY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_PURIFY].AddNextSkill( SKILL_MERCY_GROUND );

	m_pTypeInfo[SKILL_MERCY_GROUND].Set(sid++, "Mercy Ground", 0, 0, 332,"ÈÊ´È´óµØ");
	m_pTypeInfo[SKILL_MERCY_GROUND].SetMP( 14 );
	m_pTypeInfo[SKILL_MERCY_GROUND].SetLearnLevel( 40 );
	m_pTypeInfo[SKILL_MERCY_GROUND].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_MERCY_GROUND].AddNextSkill( MAGIC_AURA_RING );

	m_pTypeInfo[MAGIC_AURA_RING].Set(sid++, "Aura Ring", 0, 0, 65,"ÉñÊ¥¹â»·");//212);
	m_pTypeInfo[MAGIC_AURA_RING].SetMP( 4 );
	m_pTypeInfo[MAGIC_AURA_RING].SetLearnLevel( 45 );
	m_pTypeInfo[MAGIC_AURA_RING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_AURA_RING].AddNextSkill( MAGIC_STRIKING );

	m_pTypeInfo[MAGIC_STRIKING].Set(sid++, "Striking", 0, 0, 66,"Í»»÷");
	m_pTypeInfo[MAGIC_STRIKING].SetMP( 6 );
	m_pTypeInfo[MAGIC_STRIKING].SetLearnLevel( 50 );
	m_pTypeInfo[MAGIC_STRIKING].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_STRIKING].AddNextSkill( MAGIC_MEDITATION );
	
	m_pTypeInfo[MAGIC_MEDITATION].Set(sid++, "Meditation", 0, 0, 124,"Ú¤Ïë");
	m_pTypeInfo[MAGIC_MEDITATION].SetMP( 10 );
	m_pTypeInfo[MAGIC_MEDITATION].SetLearnLevel( 55 );
	m_pTypeInfo[MAGIC_MEDITATION].SetPassive();
	m_pTypeInfo[MAGIC_MEDITATION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_MEDITATION].AddNextSkill( MAGIC_DETECT_INVISIBILITY );

	m_pTypeInfo[MAGIC_DETECT_INVISIBILITY].Set(sid++, "Detect Invisibility", 0, 0, 67,"Õì²âÒþÐÎ");
	m_pTypeInfo[MAGIC_DETECT_INVISIBILITY].SetMP( 10 );
	m_pTypeInfo[MAGIC_DETECT_INVISIBILITY].SetLearnLevel( 60 );
	m_pTypeInfo[MAGIC_DETECT_INVISIBILITY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_DETECT_INVISIBILITY].AddNextSkill( SKILL_REQUITAL );

	m_pTypeInfo[SKILL_REQUITAL].Set(sid++,"Requital", 0, 0, 251 ,"¹¥»÷·´Éä");
	m_pTypeInfo[SKILL_REQUITAL].SetMP( 10 );
	m_pTypeInfo[SKILL_REQUITAL].SetLearnLevel( 61 );
	m_pTypeInfo[SKILL_REQUITAL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_REQUITAL].AddNextSkill(MAGIC_AURA_SHIELD );

	m_pTypeInfo[MAGIC_AURA_SHIELD].Set(sid++, "Aura Shield", 0, 0, 69,"Ê¥¹â¶Ü");
	m_pTypeInfo[MAGIC_AURA_SHIELD].SetMP( 12 );
	m_pTypeInfo[MAGIC_AURA_SHIELD].SetLearnLevel( 65 );
	m_pTypeInfo[MAGIC_AURA_SHIELD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_AURA_SHIELD].AddNextSkill( MAGIC_VISIBLE );

	m_pTypeInfo[MAGIC_VISIBLE].Set(sid++, "Visible", 0, 0, 71,"Ã÷Ä¿Êõ");
	m_pTypeInfo[MAGIC_VISIBLE].SetMP( 12 );
	m_pTypeInfo[MAGIC_VISIBLE].SetLearnLevel( 70 );
	m_pTypeInfo[MAGIC_VISIBLE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[MAGIC_VISIBLE].AddNextSkill( SKILL_ENERGY_DROP );

	m_pTypeInfo[SKILL_ENERGY_DROP].Set(sid++, "Aura Storm", 0, 0, 54,"Ê¥¹â·ç±©");
	m_pTypeInfo[SKILL_ENERGY_DROP].SetMP( 12 );
	m_pTypeInfo[SKILL_ENERGY_DROP].SetLearnLevel( 75 );
	m_pTypeInfo[SKILL_ENERGY_DROP].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_ENERGY_DROP].AddNextSkill( SKILL_SANCTUARY );

	m_pTypeInfo[SKILL_SANCTUARY].Set(sid++, "Sanctuary", 0, 0, 163,"±ÜÄÑËù");
	m_pTypeInfo[SKILL_SANCTUARY].SetMP( 12 );
	m_pTypeInfo[SKILL_SANCTUARY].SetLearnLevel( 85 );
	m_pTypeInfo[SKILL_SANCTUARY].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SANCTUARY].AddNextSkill( SKILL_HOLY_ARMOR );	

	m_pTypeInfo[SKILL_HOLY_ARMOR].Set(sid++, "Holy Armor", 0, 0, 330,"ÉñÊ¥×°¼×");
	m_pTypeInfo[SKILL_HOLY_ARMOR].SetMP( 38 );
	m_pTypeInfo[SKILL_HOLY_ARMOR].SetLearnLevel( 90 );
	m_pTypeInfo[SKILL_HOLY_ARMOR].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_HOLY_ARMOR].AddNextSkill( SKILL_REFLECTION );	
	
	m_pTypeInfo[SKILL_REFLECTION].Set(sid++, "Reflection", 0, 0, 165,"·´µ¯");
	m_pTypeInfo[SKILL_REFLECTION].SetMP( 12 );
	m_pTypeInfo[SKILL_REFLECTION].SetLearnLevel( 90 );
	m_pTypeInfo[SKILL_REFLECTION].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_REFLECTION].AddNextSkill( SKILL_REBUKE );
	
	m_pTypeInfo[SKILL_REBUKE].Set(sid++, "Rebuke", 0, 0, 222,"ÈºÌå´ßÃß");
	m_pTypeInfo[SKILL_REBUKE].SetMP( 12 );
	m_pTypeInfo[SKILL_REBUKE].SetLearnLevel( 95 );	
	m_pTypeInfo[SKILL_REBUKE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_REBUKE].AddNextSkill( SKILL_SPIRIT_GUARD );
	
	m_pTypeInfo[SKILL_SPIRIT_GUARD].Set(sid++, "Spirit Guard", 0, 0, 223,"¾«Áé»¤ÎÀ");
	m_pTypeInfo[SKILL_SPIRIT_GUARD].SetMP( 12 );
	m_pTypeInfo[SKILL_SPIRIT_GUARD].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_SPIRIT_GUARD].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SPIRIT_GUARD].AddNextSkill( SKILL_DIVINE_GUIDANCE );

	m_pTypeInfo[SKILL_DIVINE_GUIDANCE].Set(sid++, "Divine Guidance", 0, 0, 317,"ÉñÖ®×çÖä");
	m_pTypeInfo[SKILL_DIVINE_GUIDANCE].SetMP( 12 );
	m_pTypeInfo[SKILL_DIVINE_GUIDANCE].SetLearnLevel( 110 );
	m_pTypeInfo[SKILL_DIVINE_GUIDANCE].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_DIVINE_GUIDANCE].AddNextSkill( SKILL_LIGHTNESS );

	m_pTypeInfo[SKILL_LIGHTNESS].Set(sid++, "Lightness", 0, 0, 258,"ÖÕ¼«ÕÕÃ÷Êõ");
	m_pTypeInfo[SKILL_LIGHTNESS].SetMP( 40 );
	m_pTypeInfo[SKILL_LIGHTNESS].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_LIGHTNESS].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_LIGHTNESS].AddNextSkill( SKILL_SWEEP_VICE_1 );

	// 2004, 9, 8, sobeit add start - ÀÎÃ¦ 130 ½ºÅ³
	m_pTypeInfo[SKILL_SWEEP_VICE_1].Set(sid++, "Sweep vice", 700, 270,  391,"»ÃÃðÖ®Ç¯");
	m_pTypeInfo[SKILL_SWEEP_VICE_1].SetMP( 40 );
	m_pTypeInfo[SKILL_SWEEP_VICE_1].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_SWEEP_VICE_1].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_SWEEP_VICE_1].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_SWEEP_VICE_1].AddNextSkill( SKILL_INTIMATE_GRAIL );
	// 2004, 9, 8, sobeit add end - ÀÎÃ¦ 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - ÀÎÃ¦ 140 ½ºÅ³
	m_pTypeInfo[SKILL_INTIMATE_GRAIL].Set(sid++, "Intimate Grail", 700, 270,  462,"Ç×ÃÜÊ¥±­");
	m_pTypeInfo[SKILL_INTIMATE_GRAIL].SetMP( 40 );
	m_pTypeInfo[SKILL_INTIMATE_GRAIL].SetSkillStep( SKILL_STEP_APPRENTICE );
	m_pTypeInfo[SKILL_INTIMATE_GRAIL].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_INTIMATE_GRAIL].AddNextSkill( SKILL_ROTTEN_APPLE );
	// 2004, 11, 11, sobeit add end - ÀÎÃ¦ 140 ½ºÅ³

	m_pTypeInfo[SKILL_ROTTEN_APPLE].Set(sid++, "Rotten Apple", 700, 270,  473,"ÀÃÆ»¹û");
	m_pTypeInfo[SKILL_ROTTEN_APPLE].SetMP( 40 );
	m_pTypeInfo[SKILL_ROTTEN_APPLE].SetSkillStep( SKILL_STEP_SLAYER_ENCHANT_ADVANCEMENT );
	m_pTypeInfo[SKILL_ROTTEN_APPLE].SetLearnLevel( 150 );

#ifdef __NEW_SKILL__
	m_pTypeInfo[SKILL_ROTTEN_APPLE].AddNextSkill( SKILL_HEAVEN_GROUND );
	// 2005, 1, 3, sobeit add end - 
	// add by Coffee 2007-3-25
	m_pTypeInfo[SKILL_HEAVEN_GROUND].Set(sid++, "Heaven Ground", 700, 270,  502,"ÌìÉñ½µÁÙ");
	m_pTypeInfo[SKILL_HEAVEN_GROUND].SetMP( 40 );
	m_pTypeInfo[SKILL_HEAVEN_GROUND].SetSkillStep( SKILL_STEP_SLAYER_ENCHANT_ADVANCEMENT );
	m_pTypeInfo[SKILL_HEAVEN_GROUND].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_HEAVEN_GROUND].AddNextSkill( SKILL_SOUL_CHAIN );
	// end
#else
	m_pTypeInfo[SKILL_ROTTEN_APPLE].AddNextSkill( SKILL_SOUL_CHAIN );
#endif
	//----------------------------------------------------------------------
	// ¹ìÇÁ¸¶¹ý
	//----------------------------------------------------------------------
	sid = 0;
	m_pTypeInfo[MAGIC_HIDE].Set(sid++, "Hide", 0, 0, 96,"Òþ±Î");
	m_pTypeInfo[MAGIC_HIDE].SetVampireSkill();	
	m_pTypeInfo[MAGIC_HIDE].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_HIDE].SetMP( 8 );
	m_pTypeInfo[MAGIC_HIDE].SetLearnLevel( 3 );
	m_pTypeInfo[MAGIC_HIDE].AddNextSkill( SKILL_HANDS_OF_WISDOM );

	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].Set(sid++, "Hands of Wisdom", 0, 0, 213,"ÖÇ»ÛÖ®´¥");
	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].SetVampireSkill();
	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].SetMP( 15 );
	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].SetLearnLevel( 5 );
	m_pTypeInfo[SKILL_HANDS_OF_WISDOM].AddNextSkill( MAGIC_POISONOUS_HANDS );

	m_pTypeInfo[MAGIC_POISONOUS_HANDS].Set(sid++, "Poisonous Hands", 0, 0, 74,"¾ç¶¾Ö®´¥");
	m_pTypeInfo[MAGIC_POISONOUS_HANDS].SetVampireSkill();
	m_pTypeInfo[MAGIC_POISONOUS_HANDS].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[MAGIC_POISONOUS_HANDS].SetMP( 15 );
	m_pTypeInfo[MAGIC_POISONOUS_HANDS].SetLearnLevel( 6 );
	m_pTypeInfo[MAGIC_POISONOUS_HANDS].AddNextSkill( MAGIC_ACID_TOUCH );

	m_pTypeInfo[MAGIC_ACID_TOUCH].Set(sid++, "Acid Touch", 0, 0, 79,"ËáÐÔÖ®´¥");
	m_pTypeInfo[MAGIC_ACID_TOUCH].SetVampireSkill();
	m_pTypeInfo[MAGIC_ACID_TOUCH].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[MAGIC_ACID_TOUCH].SetMP( 20 );
	m_pTypeInfo[MAGIC_ACID_TOUCH].SetLearnLevel( 9 );
	m_pTypeInfo[MAGIC_ACID_TOUCH].AddNextSkill( MAGIC_GREEN_POISON );
	
	m_pTypeInfo[MAGIC_GREEN_POISON].Set(sid++, "Green Poison", 0, 0, 75,"Ó«ÂÌÖ®¶¾");
	m_pTypeInfo[MAGIC_GREEN_POISON].SetVampireSkill();
	m_pTypeInfo[MAGIC_GREEN_POISON].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[MAGIC_GREEN_POISON].SetMP( 25 );
	m_pTypeInfo[MAGIC_GREEN_POISON].SetLearnLevel( 12 );
	m_pTypeInfo[MAGIC_GREEN_POISON].AddNextSkill( MAGIC_DARKNESS );

	m_pTypeInfo[MAGIC_DARKNESS].Set(sid++, "Darkness", 0, 0, 97,"ºÚ°µÉñÁ¦");
	m_pTypeInfo[MAGIC_DARKNESS].SetVampireSkill();
	m_pTypeInfo[MAGIC_DARKNESS].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_DARKNESS].SetMP( 10 );
	m_pTypeInfo[MAGIC_DARKNESS].SetLearnLevel( 15 );
	m_pTypeInfo[MAGIC_DARKNESS].AddNextSkill( MAGIC_YELLOW_POISON );

	m_pTypeInfo[MAGIC_YELLOW_POISON].Set(sid++, "Yellow Poison", 0, 0, 76,"ÑÞ»ÆÖ®¶¾");
	m_pTypeInfo[MAGIC_YELLOW_POISON].SetVampireSkill();
	m_pTypeInfo[MAGIC_YELLOW_POISON].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[MAGIC_YELLOW_POISON].SetMP( 30 );
	m_pTypeInfo[MAGIC_YELLOW_POISON].SetLearnLevel( 18 );
	m_pTypeInfo[MAGIC_YELLOW_POISON].AddNextSkill( MAGIC_TRANSFORM_TO_BAT );

	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].Set(sid++, "Transform to Bat", 0, 0, 99,"±äÉí-òùòð");
	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].SetVampireSkill();
	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].SetMP( 45 );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].SetLearnLevel( 45 );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_BAT].AddNextSkill( MAGIC_ACID_BOLT );

	m_pTypeInfo[MAGIC_ACID_BOLT].Set(sid++, "Acid Bolt", 0, 0, 80,"ËáÊ¸");
	m_pTypeInfo[MAGIC_ACID_BOLT].SetVampireSkill();
	m_pTypeInfo[MAGIC_ACID_BOLT].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[MAGIC_ACID_BOLT].SetMP( 30 );
	m_pTypeInfo[MAGIC_ACID_BOLT].SetLearnLevel( 21 );
	m_pTypeInfo[MAGIC_ACID_BOLT].AddNextSkill( MAGIC_GREEN_STALKER );
	
	m_pTypeInfo[MAGIC_GREEN_STALKER].Set(sid++, "Green Stalker", 0, 0, 77,"¶¾Îí");
	m_pTypeInfo[MAGIC_GREEN_STALKER].SetVampireSkill();
	m_pTypeInfo[MAGIC_GREEN_STALKER].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[MAGIC_GREEN_STALKER].SetMP( 35 );
	m_pTypeInfo[MAGIC_GREEN_STALKER].SetLearnLevel( 24 );
	m_pTypeInfo[MAGIC_GREEN_STALKER].AddNextSkill( MAGIC_BLOODY_TUNNEL );

	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].Set(sid++, "Bloody Tunnel", 0, 0,  117,"ÑªÖ®äöÎÐ");
	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].SetMP( 0 );
	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].SetLearnLevel( 31 );
	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].AddNextSkill( MAGIC_PARALYZE );


	m_pTypeInfo[MAGIC_PARALYZE].Set(sid++, "Paralyze", 0, 0, 83,"Âé±Ô");
	m_pTypeInfo[MAGIC_PARALYZE].SetVampireSkill();
	m_pTypeInfo[MAGIC_PARALYZE].SetSkillStep( SKILL_STEP_VAMPIRE_CURSE );
	m_pTypeInfo[MAGIC_PARALYZE].SetMP( 30 );
	m_pTypeInfo[MAGIC_PARALYZE].SetLearnLevel( 27 );	
	m_pTypeInfo[MAGIC_PARALYZE].AddNextSkill( MAGIC_BLOODY_MARK );

	
	m_pTypeInfo[MAGIC_BLOODY_MARK].Set(sid++, "Bloody Mark", 0, 0,  116,"ÑªÖ®Ó¡¼Ç");
	m_pTypeInfo[MAGIC_BLOODY_MARK].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_MARK].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_MARK].SetMP( 0 );
	m_pTypeInfo[MAGIC_BLOODY_MARK].SetLearnLevel( 34 );
	m_pTypeInfo[MAGIC_BLOODY_MARK].AddNextSkill( MAGIC_DARKBLUE_POISON );


	m_pTypeInfo[MAGIC_DARKBLUE_POISON].Set(sid++, "DarkBlue Poison", 0, 0, 78,"ÓÄ°µÖ®¶¾");
	m_pTypeInfo[MAGIC_DARKBLUE_POISON].SetVampireSkill();
	m_pTypeInfo[MAGIC_DARKBLUE_POISON].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[MAGIC_DARKBLUE_POISON].SetMP( 43 );
	m_pTypeInfo[MAGIC_DARKBLUE_POISON].SetLearnLevel( 30 );
	m_pTypeInfo[MAGIC_DARKBLUE_POISON].AddNextSkill( MAGIC_TRANSFORM_TO_WOLF );

	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].Set(sid++, "Transform to Wolf", 0, 0, 98,"±äÉí-ÀÇ");
	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].SetVampireSkill();
	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].SetMP( 45 );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].SetLearnLevel( 39 );
	m_pTypeInfo[MAGIC_TRANSFORM_TO_WOLF].AddNextSkill( SKILL_POISON_STRIKE );

	m_pTypeInfo[SKILL_POISON_STRIKE].Set(sid++, "Poison Strike", 0, 0, 130,"¶¾ÐÔ¹¥»÷");
	m_pTypeInfo[SKILL_POISON_STRIKE].SetVampireSkill();
	m_pTypeInfo[SKILL_POISON_STRIKE].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[SKILL_POISON_STRIKE].SetMP( 30 );
	m_pTypeInfo[SKILL_POISON_STRIKE].SetLearnLevel( 36 );
	m_pTypeInfo[SKILL_POISON_STRIKE].AddNextSkill( MAGIC_DOOM );

	m_pTypeInfo[MAGIC_DOOM].Set(sid++, "Doom", 0, 0, 84,"»ÙÃð");
	m_pTypeInfo[MAGIC_DOOM].SetVampireSkill();
	m_pTypeInfo[MAGIC_DOOM].SetSkillStep( SKILL_STEP_VAMPIRE_CURSE );
	m_pTypeInfo[MAGIC_DOOM].SetMP( 30 );
	m_pTypeInfo[MAGIC_DOOM].SetLearnLevel( 36 );
	m_pTypeInfo[MAGIC_DOOM].AddNextSkill( MAGIC_ACID_BALL );
	
	m_pTypeInfo[MAGIC_ACID_BALL].Set(sid++, "Acid Ball", 0, 0, 81,"ËáÇò");	
	m_pTypeInfo[MAGIC_ACID_BALL].SetVampireSkill();
	m_pTypeInfo[MAGIC_ACID_BALL].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[MAGIC_ACID_BALL].SetMP( 45 );
	m_pTypeInfo[MAGIC_ACID_BALL].SetLearnLevel( 42 );
	m_pTypeInfo[MAGIC_ACID_BALL].AddNextSkill( MAGIC_INVISIBILITY );
	
	m_pTypeInfo[MAGIC_INVISIBILITY].Set(sid++, "Invisibllity", 0, 0, 100,"Õì²âÒþÐÎ");
	m_pTypeInfo[MAGIC_INVISIBILITY].SetVampireSkill();
	m_pTypeInfo[MAGIC_INVISIBILITY].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_INVISIBILITY].SetMP( 45 );
	m_pTypeInfo[MAGIC_INVISIBILITY].SetLearnLevel( 42 );
	m_pTypeInfo[MAGIC_INVISIBILITY].AddNextSkill( MAGIC_RAPID_GLIDING );

	m_pTypeInfo[MAGIC_RAPID_GLIDING].Set(sid++, "Rapid Gliding", 0, 0, 244,"¿ìËÙ»¬ÐÐ");	
	m_pTypeInfo[MAGIC_RAPID_GLIDING].SetVampireSkill();
	m_pTypeInfo[MAGIC_RAPID_GLIDING].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_RAPID_GLIDING].SetMP( 33 );
	m_pTypeInfo[MAGIC_RAPID_GLIDING].SetLearnLevel( 47 );
	m_pTypeInfo[MAGIC_RAPID_GLIDING].AddNextSkill( SKILL_NAIL_MASTERY );

	m_pTypeInfo[SKILL_NAIL_MASTERY].Set(sid++,"Nail Mastery", 0, 0, 246,"Àû×¦ÊìÁ·¶È");
	m_pTypeInfo[SKILL_NAIL_MASTERY].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_NAIL_MASTERY].SetVampireSkill();
	m_pTypeInfo[SKILL_NAIL_MASTERY].SetPassive();
	m_pTypeInfo[SKILL_NAIL_MASTERY].SetLearnLevel( 57 );
	m_pTypeInfo[SKILL_NAIL_MASTERY].AddNextSkill( SKILL_TRANSFUSION );

	m_pTypeInfo[SKILL_TRANSFUSION].Set(sid++, "Transfusion", 0, 0, 167,"ÑªÒº»¥²¹");
	m_pTypeInfo[SKILL_TRANSFUSION].SetVampireSkill();
	m_pTypeInfo[SKILL_TRANSFUSION].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[SKILL_TRANSFUSION].SetMP( 55 );
	m_pTypeInfo[SKILL_TRANSFUSION].SetLearnLevel( 50 );
	m_pTypeInfo[SKILL_TRANSFUSION].AddNextSkill( SKILL_ACID_STRIKE );

	m_pTypeInfo[SKILL_ACID_STRIKE].Set(sid++, "Acid Strike", 0, 0, 132,"ËáÐÔ¹¥»÷");
	m_pTypeInfo[SKILL_ACID_STRIKE].SetVampireSkill();
	m_pTypeInfo[SKILL_ACID_STRIKE].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[SKILL_ACID_STRIKE].SetMP( 55 );
	m_pTypeInfo[SKILL_ACID_STRIKE].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_ACID_STRIKE].AddNextSkill( MAGIC_ACID_SWAMP );

	m_pTypeInfo[MAGIC_ACID_SWAMP].Set(sid++, "Acid Swamp", 0, 0, 82,"ËáÐÔÕÓÔó");
	m_pTypeInfo[MAGIC_ACID_SWAMP].SetVampireSkill();
	m_pTypeInfo[MAGIC_ACID_SWAMP].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[MAGIC_ACID_SWAMP].SetMP( 55 );
	m_pTypeInfo[MAGIC_ACID_SWAMP].SetLearnLevel( 65 );
	m_pTypeInfo[MAGIC_ACID_SWAMP].AddNextSkill( SKILL_POISON_STORM );

	m_pTypeInfo[SKILL_POISON_STORM].Set(sid++, "Poison Storm", 0, 0, 131,"¶¾Òº·ç±©");
	m_pTypeInfo[SKILL_POISON_STORM].SetVampireSkill();
	m_pTypeInfo[SKILL_POISON_STORM].SetSkillStep( SKILL_STEP_VAMPIRE_POISON );
	m_pTypeInfo[SKILL_POISON_STORM].SetMP( 30 );
	m_pTypeInfo[SKILL_POISON_STORM].SetLearnLevel( 36 );
	m_pTypeInfo[SKILL_POISON_STORM].AddNextSkill( MAGIC_SEDUCTION );

	m_pTypeInfo[MAGIC_SEDUCTION].Set(sid++, "Seduction", 0, 0, 119,"ÓÕ»ó");
	m_pTypeInfo[MAGIC_SEDUCTION].SetVampireSkill();
	m_pTypeInfo[MAGIC_SEDUCTION].SetSkillStep( SKILL_STEP_VAMPIRE_CURSE );
	m_pTypeInfo[MAGIC_SEDUCTION].SetMP( 55 );
	m_pTypeInfo[MAGIC_SEDUCTION].SetLearnLevel( 48 );
	m_pTypeInfo[MAGIC_SEDUCTION].AddNextSkill( MAGIC_BLOODY_NAIL );

	m_pTypeInfo[SKILL_BLOODY_WAVE].Set(sid++, "Bloody Wave", 0, 0, 174,"ÑªÀË");
	m_pTypeInfo[SKILL_BLOODY_WAVE].SetVampireSkill();
	m_pTypeInfo[SKILL_BLOODY_WAVE].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[SKILL_BLOODY_WAVE].SetMP( 90 );
	m_pTypeInfo[SKILL_BLOODY_WAVE].SetLearnLevel( 64 );
	m_pTypeInfo[SKILL_BLOODY_WAVE].AddNextSkill( SKILL_BLOODY_STRIKE );

	m_pTypeInfo[SKILL_BLOODY_STRIKE].Set(sid++, "Bloody Strike", 0, 0, 250,"ÑªÐÈ¹¥»÷");
	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetVampireSkill();
	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetMP( 76 );
	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetLearnLevel( 66 );
	m_pTypeInfo[SKILL_BLOODY_STRIKE].AddNextSkill( SKILL_ACID_STORM );
		
	m_pTypeInfo[MAGIC_BLOODY_NAIL].Set(sid++, "Bloody Nail", 0, 0, 127,"Ñª×¦");
	m_pTypeInfo[MAGIC_BLOODY_NAIL].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_NAIL].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_NAIL].SetMP( 60 );
	m_pTypeInfo[MAGIC_BLOODY_NAIL].SetLearnLevel( 75 );
	m_pTypeInfo[MAGIC_BLOODY_NAIL].AddNextSkill( MAGIC_BLOODY_KNIFE  );

	m_pTypeInfo[SKILL_ACID_STORM].Set(sid++, "Acid Storm", 0, 0, 133,"ËáÐÔ·ç±©");
	m_pTypeInfo[SKILL_ACID_STORM].SetVampireSkill();
	m_pTypeInfo[SKILL_ACID_STORM].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[SKILL_ACID_STORM].SetMP( 55 );
	m_pTypeInfo[SKILL_ACID_STORM].SetLearnLevel( 65 );
	m_pTypeInfo[SKILL_ACID_STORM].AddNextSkill( SKILL_EXTREME );

	m_pTypeInfo[MAGIC_BLOODY_KNIFE].Set(sid++, "Bloody Knife", 0, 0, 128,"ÑªÈÐ");
	m_pTypeInfo[MAGIC_BLOODY_KNIFE].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_KNIFE].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_KNIFE].SetMP( 63 );
	m_pTypeInfo[MAGIC_BLOODY_KNIFE].SetLearnLevel( 58 );
	m_pTypeInfo[MAGIC_BLOODY_KNIFE].AddNextSkill( SKILL_BLOODY_WAVE );

	m_pTypeInfo[SKILL_EXTREME].Set(sid++, "Dark Revenge", 0, 0, 134,"ºÚ°µ¸´³ðÕß");
	m_pTypeInfo[SKILL_EXTREME].SetVampireSkill();
	m_pTypeInfo[SKILL_EXTREME].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_EXTREME].SetMP( 63 );
	m_pTypeInfo[SKILL_EXTREME].SetLearnLevel( 63 );
	m_pTypeInfo[SKILL_EXTREME].AddNextSkill( MAGIC_BLOODY_BALL );

	m_pTypeInfo[MAGIC_BLOODY_BALL].Set(sid++, "Bloody Ball", 0, 0, 129,"ÑªÇò");
	m_pTypeInfo[MAGIC_BLOODY_BALL].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_BALL].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_BALL].SetMP( 65 );
	m_pTypeInfo[MAGIC_BLOODY_BALL].SetLearnLevel( 66 );
	m_pTypeInfo[MAGIC_BLOODY_BALL].AddNextSkill( MAGIC_SUMMON_CASKET );

	m_pTypeInfo[MAGIC_SUMMON_CASKET].Set(sid++, "Summon Casket", 0, 0, 120,"ÕÙ»½-¹×²Ä");
	m_pTypeInfo[MAGIC_SUMMON_CASKET].SetVampireSkill();
	m_pTypeInfo[MAGIC_SUMMON_CASKET].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
	m_pTypeInfo[MAGIC_SUMMON_CASKET].SetMP( 70 );
	m_pTypeInfo[MAGIC_SUMMON_CASKET].SetLearnLevel( 55 );
	m_pTypeInfo[MAGIC_SUMMON_CASKET].AddNextSkill( SKILL_STONE_SKIN );

//	m_pTypeInfo[SKILL_CHARM].Set(sid++, "Kidnap", 0, 0, 135,"Kidnap");
//	m_pTypeInfo[SKILL_CHARM].SetVampireSkill();
//	m_pTypeInfo[SKILL_CHARM].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
//	m_pTypeInfo[SKILL_CHARM].SetMP( 30 );
//	m_pTypeInfo[SKILL_CHARM].SetLearnLevel( 73 );
//	m_pTypeInfo[SKILL_CHARM].AddNextSkill( MAGIC_DEATH );	

	m_pTypeInfo[SKILL_STONE_SKIN].Set(sid++, "Stone Skin", 0, 0, 338,"Ê¯Ö®¼¡·ô");
	m_pTypeInfo[SKILL_STONE_SKIN].SetVampireSkill();
	m_pTypeInfo[SKILL_STONE_SKIN].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_STONE_SKIN].SetMP( 10 );
	m_pTypeInfo[SKILL_STONE_SKIN].SetLearnLevel( 68 );
	m_pTypeInfo[SKILL_STONE_SKIN].AddNextSkill( SKILL_TRANSFORM_TO_WERWOLF );

	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].Set(sid++, "Transform To Werwolf", 0, 0, 336,"±äÉí-ÀÇÈË");
	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].SetVampireSkill();
	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].SetMP( 10 );
	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].SetLearnLevel( 72 );
	m_pTypeInfo[SKILL_TRANSFORM_TO_WERWOLF].AddNextSkill( SKILL_GRAY_DARKNESS );
		
	m_pTypeInfo[SKILL_GRAY_DARKNESS].Set(sid++, "Gray Darkness", 0, 0, 339,"ºÚ°µÉñÁ¦");
	m_pTypeInfo[SKILL_GRAY_DARKNESS].SetVampireSkill();
	m_pTypeInfo[SKILL_GRAY_DARKNESS].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_GRAY_DARKNESS].SetMP( 10 );
	m_pTypeInfo[SKILL_GRAY_DARKNESS].SetLearnLevel( 78 );
	m_pTypeInfo[SKILL_GRAY_DARKNESS].AddNextSkill( SKILL_ACID_ERUPTION );
	
	m_pTypeInfo[SKILL_ACID_ERUPTION].Set(sid++, "Acid Eruption", 0, 0, 340,"ËáÐÔÅç·¢");
	m_pTypeInfo[SKILL_ACID_ERUPTION].SetVampireSkill();
	m_pTypeInfo[SKILL_ACID_ERUPTION].SetSkillStep( SKILL_STEP_VAMPIRE_ACID );
	m_pTypeInfo[SKILL_ACID_ERUPTION].SetMP( 60 );
	m_pTypeInfo[SKILL_ACID_ERUPTION].SetLearnLevel( 80 );
	m_pTypeInfo[SKILL_ACID_ERUPTION].AddNextSkill( SKILL_TALON_OF_CROW );

	m_pTypeInfo[SKILL_TALON_OF_CROW].Set(sid++, "Talon Of Crow", 0, 0, 337,"ÎüÑªÉñÑ»");
	m_pTypeInfo[SKILL_TALON_OF_CROW].SetVampireSkill();
	m_pTypeInfo[SKILL_TALON_OF_CROW].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_TALON_OF_CROW].SetMP( 20 );
	m_pTypeInfo[SKILL_TALON_OF_CROW].SetLearnLevel( 86 );
	m_pTypeInfo[SKILL_TALON_OF_CROW].AddNextSkill( MAGIC_HOWL );

	m_pTypeInfo[MAGIC_HOWL].Set(sid++, "Howl", 0, 0, 122,"Õ½ÕùÅØÏø");
	m_pTypeInfo[MAGIC_HOWL].SetVampireSkill();
	m_pTypeInfo[MAGIC_HOWL].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_HOWL].SetMP( 10 );
	m_pTypeInfo[MAGIC_HOWL].SetLearnLevel( 40 );
	m_pTypeInfo[MAGIC_HOWL].AddNextSkill( MAGIC_DEATH );
	
	m_pTypeInfo[MAGIC_DEATH].Set(sid++, "Death", 0, 0, 86,"ÖÂËÀÊõ");
	m_pTypeInfo[MAGIC_DEATH].SetVampireSkill();
	m_pTypeInfo[MAGIC_DEATH].SetSkillStep( SKILL_STEP_VAMPIRE_CURSE );
	m_pTypeInfo[MAGIC_DEATH].SetMP( 55 );
	m_pTypeInfo[MAGIC_DEATH].SetLearnLevel( 79 );
	m_pTypeInfo[MAGIC_DEATH].AddNextSkill( MAGIC_HALLUCINATION );

	m_pTypeInfo[MAGIC_HALLUCINATION].Set(sid++, "Hallucination", 0, 0, 85,"ÃÔ»ÃÊõ");
	m_pTypeInfo[MAGIC_HALLUCINATION].SetVampireSkill();
	m_pTypeInfo[MAGIC_HALLUCINATION].SetSkillStep( SKILL_STEP_VAMPIRE_CURSE );
	m_pTypeInfo[MAGIC_HALLUCINATION].SetMP( 40 );
	m_pTypeInfo[MAGIC_HALLUCINATION].SetLearnLevel( 120 );
	m_pTypeInfo[MAGIC_HALLUCINATION].AddNextSkill( MAGIC_BLOODY_SPEAR );
	
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].Set(sid++, "Bloody Spear", 0, 0, 169,"ÑªÃ¬");
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].SetMP( 90 );
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].SetLearnLevel( 94 );
	m_pTypeInfo[MAGIC_BLOODY_SPEAR].AddNextSkill( MAGIC_BLOODY_WALL );

	m_pTypeInfo[MAGIC_BLOODY_WALL].Set(sid++, "Bloody Wall", 0, 0, 168,"ÑªÇ½");
	m_pTypeInfo[MAGIC_BLOODY_WALL].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_WALL].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_WALL].SetMP( 75 );
	m_pTypeInfo[MAGIC_BLOODY_WALL].SetLearnLevel( 95 );
	m_pTypeInfo[MAGIC_BLOODY_WALL].AddNextSkill( SKILL_MEPHISTO );	
	
	m_pTypeInfo[SKILL_MEPHISTO].Set(sid++, "Mephistopheles", 0, 0, 171,"ËÀÍöÆõÔ¼");
	m_pTypeInfo[SKILL_MEPHISTO].SetVampireSkill();
	m_pTypeInfo[SKILL_MEPHISTO].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
	m_pTypeInfo[SKILL_MEPHISTO].SetMP( 90 );
	m_pTypeInfo[SKILL_MEPHISTO].SetLearnLevel( 96 );
	m_pTypeInfo[SKILL_MEPHISTO].AddNextSkill( MAGIC_BLOODY_BREAKER );

	m_pTypeInfo[MAGIC_BLOODY_BREAKER].Set(0, "Bloody Breaker", 0, 0, 243,"ÑªÐÈÆÆ»µ");
	m_pTypeInfo[MAGIC_BLOODY_BREAKER].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_BREAKER].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_BREAKER].SetMP( 60 );
	m_pTypeInfo[MAGIC_BLOODY_BREAKER].SetLearnLevel( 99 );
//	m_pTypeInfo[MAGIC_BLOODY_BREAKER].AddNextSkill( SKILL_BLOODY_STRIKE );
	m_pTypeInfo[MAGIC_BLOODY_BREAKER].AddNextSkill( MAGIC_BLOODY_ZENITH );

	m_pTypeInfo[MAGIC_BLOODY_ZENITH].Set(0, "Bloody Zenith", 0, 0, 318,"ÑªÐÈáÛ·å");
	m_pTypeInfo[MAGIC_BLOODY_ZENITH].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
	m_pTypeInfo[MAGIC_BLOODY_ZENITH].SetVampireSkill();
	m_pTypeInfo[MAGIC_BLOODY_ZENITH].SetMP( 60 );
	m_pTypeInfo[MAGIC_BLOODY_ZENITH].SetLearnLevel( 110 );
	m_pTypeInfo[MAGIC_BLOODY_ZENITH].AddNextSkill( MAGIC_METEOR );

//	m_pTypeInfo[SKILL_BLOODY_STRIKE].Set(sid++, "Bloody Strike", 0, 0, 136,"ºí·¯µð ½ºÆ®¶óÀÌÅ©");
//	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetVampireSkill();
//	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
//	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetMP( 76 );
//	m_pTypeInfo[SKILL_BLOODY_STRIKE].SetLearnLevel( 100 );
//	m_pTypeInfo[SKILL_BLOODY_STRIKE].AddNextSkill( MAGIC_METEOR );

//	m_pTypeInfo[MAGIC_RAISING_DEAD].Set(sid++, "Raising Dead", 0, 0, 173,"RaisingDead");
//	m_pTypeInfo[MAGIC_RAISING_DEAD].SetVampireSkill();
//	m_pTypeInfo[MAGIC_RAISING_DEAD].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
//	m_pTypeInfo[MAGIC_RAISING_DEAD].SetMP( 55 );
//	m_pTypeInfo[MAGIC_RAISING_DEAD].SetLearnLevel( 100 );
//	m_pTypeInfo[MAGIC_RAISING_DEAD].AddNextSkill( SKILL_SUMMON_BAT );

//	m_pTypeInfo[SKILL_SUMMON_BAT].Set(sid++, "Summon Bat", 0, 0, 87,"SummonBat");//324);
//	m_pTypeInfo[SKILL_SUMMON_BAT].SetVampireSkill();
//	m_pTypeInfo[SKILL_SUMMON_BAT].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
//	m_pTypeInfo[SKILL_SUMMON_BAT].SetMP( 90 );
//	m_pTypeInfo[SKILL_SUMMON_BAT].SetLearnLevel( 100 );
//	m_pTypeInfo[SKILL_SUMMON_BAT].AddNextSkill( MAGIC_SUMMON_SERVANT );
//	
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].Set(sid++, "Summon Servant", 0, 0, 170,"SummonServant");
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].SetVampireSkill();
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].SetMP( 93 );
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].SetLearnLevel( 100 );
//	m_pTypeInfo[MAGIC_SUMMON_SERVANT].AddNextSkill( MAGIC_METEOR );	

	m_pTypeInfo[MAGIC_METEOR].Set(sid++, "Meteor Strike", 0, 0, 242,"ÔÉÊ¯Êõ");
	m_pTypeInfo[MAGIC_METEOR].SetVampireSkill();
	m_pTypeInfo[MAGIC_METEOR].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[MAGIC_METEOR].SetMP( 70 );
	m_pTypeInfo[MAGIC_METEOR].SetLearnLevel( 100 );
	m_pTypeInfo[MAGIC_METEOR].AddNextSkill( SKILL_VIOLENT_PHANTOM );
	
//	m_pTypeInfo[SKILL_ARMAGEDDON].Set(sid++, "Armageddon", 0, 0, 172,"Armageddon");
//	m_pTypeInfo[SKILL_ARMAGEDDON].SetVampireSkill();
//	m_pTypeInfo[SKILL_ARMAGEDDON].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
//	m_pTypeInfo[SKILL_ARMAGEDDON].SetMP( 75 );
//	m_pTypeInfo[SKILL_ARMAGEDDON].SetLearnLevel( 99 );
//	m_pTypeInfo[SKILL_ARMAGEDDON].AddNextSkill( SKILL_BLOODY_STORM );	
		
//	m_pTypeInfo[SKILL_BLOODY_STORM].Set(sid++, "Bloody Storm", 0, 0, 93,"BloodyStorm");
//	m_pTypeInfo[SKILL_BLOODY_STORM].SetVampireSkill();
//	m_pTypeInfo[SKILL_BLOODY_STORM].SetSkillStep( SKILL_STEP_VAMPIRE_BLOOD );
//	m_pTypeInfo[SKILL_BLOODY_STORM].SetMP( 75 );
//	m_pTypeInfo[SKILL_BLOODY_STORM].SetLearnLevel( 100 );
//	m_pTypeInfo[SKILL_BLOODY_STORM].AddNextSkill( SKILL_SOUL_CHAIN );

	// 2004, 9, 8, sobeit add start - ¹ìÆÄÀÌ¾î 130 ½ºÅ³
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].Set(sid++, "Violent phantom", 0, 0, 392,"±©Á¦»ÃÓ°");
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].SetVampireSkill();
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].SetMP( 20 );
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].SetLearnLevel( 130 );
	m_pTypeInfo[SKILL_VIOLENT_PHANTOM].AddNextSkill( SKILL_SUMMON_GORE_GLAND );

	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].Set(sid++, "Summon Gore gland", 0, 0, 393,"ÕÐ»½-ÑªÐÈÂö¹Ü");
	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].SetVampireSkill();
	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].SetMP( 20 );
	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].SetLearnLevel( 130 );
//	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[SKILL_SUMMON_GORE_GLAND].AddNextSkill( SKILL_SET_AFIRE );
	// 2004, 9, 8, sobeit add end - ¹ìÆÄÀÌ¾î 130 ½ºÅ³

	// 2004, 11, 11, sobeit add start - ¹ìÆÄÀÌ¾î 140 ½ºÅ³
	m_pTypeInfo[SKILL_SET_AFIRE].Set(sid++, "Set Afire", 0, 0, 463,"µãÈ¼");
	m_pTypeInfo[SKILL_SET_AFIRE].SetVampireSkill();
	m_pTypeInfo[SKILL_SET_AFIRE].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_SET_AFIRE].SetMP( 20 );
	m_pTypeInfo[SKILL_SET_AFIRE].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_SET_AFIRE].AddNextSkill( SKILL_NOOSE_OF_WRAITH );

	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].Set(sid++, "Noose of Wraith", 0, 0, 464, "¸¿»êÊõ");
	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].SetVampireSkill();
	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].SetSkillStep( SKILL_STEP_VAMPIRE_SUMMON );
	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].SetMP( 20 );
	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].SetLearnLevel( 140 );
	m_pTypeInfo[SKILL_NOOSE_OF_WRAITH].AddNextSkill( SKILL_ABERRATION );
	// 2004, 11, 11, sobeit add end - ¹ìÆÄÀÌ¾î 140 ½ºÅ³

	//2005.1.5 by csm  2Â÷ ÀüÁ÷ ¹ìÆÄÀÌ¾î ½ºÅ³Ãß°¡ 
	m_pTypeInfo[SKILL_ABERRATION].Set(sid++, "Aberration", 0, 0, 475, "ÐÄÌ¬Ê§³£");
	m_pTypeInfo[SKILL_ABERRATION].SetVampireSkill();
	m_pTypeInfo[SKILL_ABERRATION].SetSkillStep( SKILL_STEP_VAMPIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_ABERRATION].SetMP( 20 );
	m_pTypeInfo[SKILL_ABERRATION].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_ABERRATION].AddNextSkill( SKILL_WILD_WOLF );

	m_pTypeInfo[SKILL_WILD_WOLF].Set(sid++, "Wild Wolf", 0, 0, 474, "Ò°ÀÇ");
	m_pTypeInfo[SKILL_WILD_WOLF].SetVampireSkill();
	m_pTypeInfo[SKILL_WILD_WOLF].SetSkillStep( SKILL_STEP_VAMPIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_WILD_WOLF].SetMP( 20 );
	m_pTypeInfo[SKILL_WILD_WOLF].SetLearnLevel( 150 );
	

#ifdef __NEW_SKILL__
	// 2007-2-15 add by coffee start
	m_pTypeInfo[SKILL_WILD_WOLF].AddNextSkill( SKILL_BLLODY_SCARIFY );

	m_pTypeInfo[SKILL_BLLODY_SCARIFY].Set(150, "Bloody Scarify", 0, 0, 505, "ÑªÖ®ÀÓÓ¡");
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetVampireSkill();
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetSkillStep( SKILL_STEP_VAMPIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetMP( 120 );
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].AddNextSkill( SKILL_BLOOD_CURSE );
#else
	m_pTypeInfo[SKILL_WILD_WOLF].AddNextSkill( SKILL_SOUL_CHAIN );
#endif
	//SKILL_BLOOD_CURSE
#ifdef __NEW_SKILL__
	// 2007-2-15 add by coffee end
	// 2007-2-25  add by coffee start
	m_pTypeInfo[SKILL_BLOOD_CURSE].Set(150, "Blood Curse", 0, 0, 506, "ÑªÖ®×çÖä");
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetVampireSkill();
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetSkillStep( SKILL_STEP_VAMPIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetMP( 70 );
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_BLOOD_CURSE].AddNextSkill( SKILL_SOUL_CHAIN );

#endif
	// end

	//----------------------------------------------------------------------
	// ¾Æ¿ì½ºÅÍÁî
	//----------------------------------------------------------------------
	sid = 0;
	int skillID;

	int NumSkillIcon = 259;
	skillID = SKILL_FLOURISH;
	m_pTypeInfo[skillID].Set(sid++, "FLOURISH", 7, 21, NumSkillIcon++,"»îÔ¾¹¥»÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_EVADE );
	
	skillID = SKILL_EVADE;
	m_pTypeInfo[skillID].Set(sid++, "EVADE", 22, 84, NumSkillIcon++,"»Ø±ÜÊõ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SHARP_ROUND);
	
	skillID = SKILL_SHARP_ROUND;
	m_pTypeInfo[skillID].Set(sid++, "SHARP_ROUND", 151, 81, NumSkillIcon++,"Ðý·çÁÒÕ¶");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_HIDE_SIGHT);
	
	skillID = SKILL_HIDE_SIGHT;
	m_pTypeInfo[skillID].Set(sid++, "HIDE_SIGHT", 168, 272, NumSkillIcon++,"¾«È·´ò»÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BACK_STAB);
	
	skillID = SKILL_BACK_STAB;
	m_pTypeInfo[skillID].Set(sid++, "BACK_STAB", 17, 144, NumSkillIcon++,"±³´Ì");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BLUNTING);
	
	skillID = SKILL_BLUNTING;
	m_pTypeInfo[skillID].Set(sid++, "BLUNTING", 213, 95, NumSkillIcon++,"·ÀÓù±À»µ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_GAMMA_CHOP);
	
	skillID = SKILL_GAMMA_CHOP;
	m_pTypeInfo[skillID].Set(sid++, "GAMMA_CHOP", 271, 219, NumSkillIcon++,"Ù¤ÂêÕ¶");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_CROSS_GUARD);
	
	skillID = SKILL_CROSS_GUARD;
	m_pTypeInfo[skillID].Set(sid++, "CROSS_GUARD", 71, 199, NumSkillIcon++,"Ê®×Ö»¤ÎÀ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_KASAS_ARROW);

	//////////////////////////////////////////////////////////////////////////
	// elemental
	//////////////////////////////////////////////////////////////////////////
	
	skillID = SKILL_KASAS_ARROW;
	m_pTypeInfo[skillID].Set(sid++, "KASAS_ARROW", 298,	34, NumSkillIcon++,"¿¨ÈøÖ®¼ý");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_HANDS_OF_FIRE);
	
	skillID = SKILL_HANDS_OF_FIRE;
	m_pTypeInfo[skillID].Set(sid++, "HANDS_OF_FIRE", 403, 153, NumSkillIcon++,"»ðÑæÖ®´¥");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_PROMINENCE);
	
	skillID = SKILL_PROMINENCE;
	m_pTypeInfo[skillID].Set(sid++, "PROMINENCE", 166, 285, NumSkillIcon++,"µØÓü»ðº£");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_RING_OF_FLARE);
	
	skillID = SKILL_RING_OF_FLARE;
	m_pTypeInfo[skillID].Set(sid++, "RING_OF_FLARE", 190, 345, NumSkillIcon++,"¹âÒ«Ö®»·");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BLAZE_BOLT);
	
	skillID = SKILL_BLAZE_BOLT;
	m_pTypeInfo[skillID].Set(sid++, "BLAZE_BOLT", 288, 424, NumSkillIcon++,"ÁÒÑæÖ®Ê¸");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ICE_FIELD);
	
	skillID = SKILL_ICE_FIELD;
	m_pTypeInfo[skillID].Set(sid++, "ICE_FIELD", 129, 157, NumSkillIcon++,"±ùÖ®ÁìÓò");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_WATER_BARRIER);
	
	skillID = SKILL_WATER_BARRIER;
	m_pTypeInfo[skillID].Set(sid++, "WATER_BARRIER", 112, 223, NumSkillIcon++,"Ë®Ö®±ÚÕÏ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_HANDS_OF_NIZIE);
	
	skillID = SKILL_HANDS_OF_NIZIE;
	m_pTypeInfo[skillID].Set(sid++, "HANDS_OF_NIZIE", 357, 257, NumSkillIcon++,"Ë®ÉñÖ®´¥");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_NYMPH_RECOVERY);
	
	skillID = SKILL_NYMPH_RECOVERY;
	m_pTypeInfo[skillID].Set(sid++, "NYMPH_RECOVERY", 438, 289, NumSkillIcon++,"ÖÎÓúÅ®Éñ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_LIBERTY);
	
	skillID = SKILL_LIBERTY;
	m_pTypeInfo[skillID].Set(sid++, "LIBERTY", 201, 407, NumSkillIcon++,"×çÖä¾»»¯");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_TENDRIL);
	
	skillID = SKILL_TENDRIL;
	m_pTypeInfo[skillID].Set(sid++, "TENDRIL", 304, 104, NumSkillIcon++,"ÌÙÂû");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_GNOMES_WHISPER);
	
	skillID = SKILL_GNOMES_WHISPER;
	m_pTypeInfo[skillID].Set(sid++, "GNOMES_WHISPER", 222, 160, NumSkillIcon++,"ÕæÊµÊÓÒ°");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_STONE_AUGER);
	
	skillID = SKILL_STONE_AUGER;
	m_pTypeInfo[skillID].Set(sid++, "STONE_AUGER", 308, 161, NumSkillIcon++,"Ê¯×ê³å»÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_REFUSAL_ETHER);
	
	skillID = SKILL_REFUSAL_ETHER;
	m_pTypeInfo[skillID].Set(sid++, "REFUSAL_ETHER", 245, 296, NumSkillIcon++,"¾»»¯Ö®Ï¢");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_EARTHS_TEETH);
	
	skillID = SKILL_EARTHS_TEETH;
	m_pTypeInfo[skillID].Set(sid++, "EARTHS_TEETH", 376, 425, NumSkillIcon++,"´óµØÖ®ÑÀ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ABSORB_SOUL);
	
	skillID = SKILL_ABSORB_SOUL;
	m_pTypeInfo[skillID].Set(sid++, "ABSORB_SOUL", MAKELONG(159, 13), MAKELONG(17, 14), NumSkillIcon++,"Áé»êÎüÊÕ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ETC );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_SYLPH);
	
	skillID = SKILL_SUMMON_SYLPH;
	m_pTypeInfo[skillID].Set(sid++, "SUMMON_SYLPH", MAKELONG(76, 148), MAKELONG(17, 30), NumSkillIcon++,"ÕÙ»½-¾«Áé");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ETC );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DRIFTING_SOUL);
	
	skillID = SKILL_DRIFTING_SOUL;
	m_pTypeInfo[skillID].Set(sid++, "DRIFTING_SOUL", 266, 232, NumSkillIcon++,"Áé»êÆ¯Á÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_CRITICAL_MAGIC);
	
	skillID = SKILL_CRITICAL_MAGIC;
	m_pTypeInfo[skillID].Set(sid++, "CRITICAL_MAGIC", 240, 492, NumSkillIcon++,"Ä§·¨±¬»÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_EMISSION_WATER);

	skillID = SKILL_EMISSION_WATER;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_EMISSION_WATER", 71, 94, 309,"ÒºÌåÅç·¢");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BEAT_HEAD);

	skillID = SKILL_BEAT_HEAD;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_BEAT_HEAD", 229, 84, 310,"ÖÂÃü´ò»÷");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );	
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FIRE_OF_SOUL_STONE);

	skillID = SKILL_FIRE_OF_SOUL_STONE;
	m_pTypeInfo[skillID].Set(sid++, "FIRE_OF_SOUL_STONE", 110, 138, NumSkillIcon++,"»ðÖ®ÁéÊ¯");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ICE_OF_SOUL_STONE);
	
	skillID = SKILL_ICE_OF_SOUL_STONE;
	m_pTypeInfo[skillID].Set(sid++, "ICE_OF_SOUL_STONE", 94, 258, NumSkillIcon++,"±ùÖ®ÁéÊ¯");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SAND_OF_SOUL_STONE);
	
	skillID = SKILL_SAND_OF_SOUL_STONE;
	m_pTypeInfo[skillID].Set(sid++, "SAND_OF_SOUL_STONE", 204, 213,   NumSkillIcon++,"É³Ö®ÁéÊ¯");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BLOCK_HEAD);
	
	skillID = SKILL_BLOCK_HEAD;
	m_pTypeInfo[skillID].Set(sid++, "BLOCK_HEAD", 226, 156, NumSkillIcon++,"Ñ£ÔÎ");
	m_pTypeInfo[skillID].SetOustersSkill();	
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DIVINE_SPIRITS );
	
	skillID = SKILL_DIVINE_SPIRITS;
	m_pTypeInfo[skillID].Set(sid++, "DIVINE_SPIRITS", MAKELONG(85, 154), MAKELONG(78, 98),  311,"ÉñÖ®¾«Áé");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ETC );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_TELEPORT );

	skillID = SKILL_TELEPORT;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_TELEPORT",  MAKELONG(141, 271),MAKELONG(195, 357) , 357,"Ë²¼äÒÆ¶¯");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ETC );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SOUL_REBIRTH );

	skillID = SKILL_SOUL_REBIRTH;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SOUL_REBIRTH", 368, 347, 358,"¸´»î");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ICE_LANCE );

	skillID = SKILL_ICE_LANCE;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_ICE_LANCE", 337, 515, 359,"¼²·ç±ùÃ¬");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ICE_LANCE_MASTERY );

	skillID = SKILL_ICE_LANCE_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_ICE_LANCE_MASTERY", 337, 800, 359,"¼²·ç±ùÃ¬ÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_EXPLOSION_WATER );

	skillID = SKILL_EXPLOSION_WATER;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_EXPLOSION_WATER", 455, 621, 360,"ÖÆÔ¼ÅçÈª");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FROZEN_ARMOR );

	skillID = SKILL_FROZEN_ARMOR;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_FROZEN_ARMOR", 399, 561, 361,"º®±ù»¤¼×");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FIRE_PIERCING );

	skillID = SKILL_FIRE_PIERCING;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_FIRE_PIERCING", 233, 555, 362,"ÁÒÑæÖ¸");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_OF_FIRE_ELEMENTAL );

	skillID = SKILL_SUMMON_OF_FIRE_ELEMENTAL;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SUMMON_OF_FIRE_ELEMENTAL", 199, 617, 363,"ÕÙ»½-»ðÔªËØ");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_MAGNUM_SPEAR );

	skillID = SKILL_MAGNUM_SPEAR;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_MAGNUM_SPEAR",294, 591, 364,"³àÓêÁ÷Ñæ");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_MAGNUM_SPEAR_MASTERY );

	skillID = SKILL_MAGNUM_SPEAR_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_MAGNUM_SPEAR_MASTERY",263, 800, 364,"³àÓêÁ÷ÑæÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_HELLFIRE );

	skillID = SKILL_HELLFIRE;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_HELLFIRE", 204, 702, 365,"µØÓü»ð");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
//	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_OF_WATER_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_MIGA );

	skillID = SKILL_SUMMON_MIGA;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SUMMON_MIGA", 204, 860, 466,"ÕÐ»½-Miga");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_OF_WATER_ELEMENTAL );

	skillID = SKILL_SUMMON_OF_WATER_ELEMENTAL;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SUMMON_OF_WATER_ELEMENTAL", 378, 621, 366,"ÕÙ»½-Ë®ÔªËØ");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SOUL_REBIRTH_MASTERY );

	skillID = SKILL_SOUL_REBIRTH_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SOUL_REBIRTH_MASTERY", 416, 688, 367,"¸´»îÇ¿»¯ÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
//	m_pTypeInfo[skillID].AddNextSkill( SKILL_REACTIVE_ARMOR );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_ICE_HORIZON );

	skillID = SKILL_ICE_HORIZON;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_ICE_HORIZON", 416, 860, 467,"±ù¶³µØÆ½Ïß");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_REACTIVE_ARMOR );

	skillID = SKILL_REACTIVE_ARMOR;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_REACTIVE_ARMOR", 139, 521, 368,"´óµØ»¤¼×");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_REACTIVE_ARMOR_MASTERY );

	skillID = SKILL_REACTIVE_ARMOR_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_REACTIVE_ARMOR_MASTERY", 139, 800, 368,"´óµØ»¤¼×ÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_GROUND_BLESS );

	skillID = SKILL_GROUND_BLESS;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_GROUND_BLESS", 85 , 583, 369,"´óµØ×£¸£");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SUMMON_OF_GROUND_ELEMENTAL );

	skillID = SKILL_SUMMON_OF_GROUND_ELEMENTAL;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SUMMON_OF_GROUND_ELEMENTAL", 37 ,650, 370,"ÉñÒ«Ó¡¼Ç");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_METEOR_STORM );

	skillID = SKILL_METEOR_STORM;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_METEOR_STORM", 100, 702, 371,"Á÷ÐÇÓê");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
//	m_pTypeInfo[skillID].AddNextSkill( SKILL_SHARP_CHAKRAM );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FURY_OF_GNOME );

	skillID = SKILL_FURY_OF_GNOME;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_FURY_OF_GNOME", 100, 860, 468,"´óµØÖ®Å­");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_ELEMENTAL );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SHARP_CHAKRAM );

	skillID = SKILL_SHARP_CHAKRAM;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SHARP_CHAKRAM",34, 291, 372,"ÈñÀûÖ®ÂÖ");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SHIFT_BREAK );

	skillID = SKILL_SHIFT_BREAK;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SHIFT_BREAK", 154,331 , 373,"»ØÐýÕ¶");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_WATER_SHIELD );

	skillID = SKILL_WATER_SHIELD;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_WATER_SHIELD", 263, 287, 374,"Ë®Ö®Æíµ»");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DESTRUCTION_SPEAR );

	skillID = SKILL_DESTRUCTION_SPEAR;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_DESTRUCTION_SPEAR", 84, 352, 375,"ÖÂÃü±¬·¢");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DESTRUCTION_SPEAR_MASTERY );
	
	skillID = SKILL_DESTRUCTION_SPEAR_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_DESTRUCTION_SPEAR_MASTERY", 84, 510, 375,"ÖÂÃü±¬·¢ÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_BLESS_FIRE );

	skillID = SKILL_BLESS_FIRE;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_BLESS_FIRE", 210, 380, 376,"»ðÖ®Æíµ»");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FATAL_SNICK );

	skillID = SKILL_FATAL_SNICK;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_FATAL_SNICK", 340, 343, 377,"ÖÂ²ÐÖ®Éß");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_FATAL_SNICK_MASTERY );

	skillID = SKILL_FATAL_SNICK_MASTERY;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_FATAL_SNICK_MASTERY", 340, 510, 377,"ÖÂ²ÐÖ®ÉßÊìÁ·¶È");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SAND_CROSS );

	skillID = SKILL_SAND_CROSS;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SAND_CROSS", 47, 413, 378,"É³Ö®Æíµ»");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetPassive();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DUCKING_WALLOP );

	skillID = SKILL_DUCKING_WALLOP;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_DUCKING_WALLOP", 152, 412, 379,"¹âËÙ³å»÷");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_CHARGING_ATTACK );

	skillID = SKILL_CHARGING_ATTACK;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_CHARGING_ATTACK", 285, 409, 380,"¾«ÁéÖ®Å­");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_DISTANCE_BLITZ );

	skillID = SKILL_DISTANCE_BLITZ;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_DISTANCE_BLITZ", 224, 450, 381,"À×ÉñÕ¶");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
//	m_pTypeInfo[skillID].AddNextSkill( SKILL_SOUL_CHAIN );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SHARP_HAIL );

	skillID = SKILL_SHARP_HAIL;
	m_pTypeInfo[skillID].Set(sid++, "SKILL_SHARP_HAIL", 224, 565, 465,"¼âÈñ±ù±¢");
	m_pTypeInfo[skillID].SetOustersSkill();
	m_pTypeInfo[skillID].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT );
	m_pTypeInfo[skillID].AddNextSkill( SKILL_SOUL_CHAIN );

	//±âÅ¸ °è¿­
	m_pTypeInfo[SKILL_SOUL_CHAIN].Set(sid++, "Soul Chain", MAKELONG(250, 400), MAKELONG(20,40), 175,"Áé»êËøÁ´");
//	m_pTypeInfo[SKILL_SOUL_CHAIN].SetVampireSkill();
	m_pTypeInfo[SKILL_SOUL_CHAIN].SetSkillStep( SKILL_STEP_ETC );
//	m_pTypeInfo[SKILL_SOUL_CHAIN].SetSkillStep( SKILL_STEP_VAMPIRE_INNATE );
	m_pTypeInfo[SKILL_SOUL_CHAIN].SetMP( 75 );
	m_pTypeInfo[SKILL_SOUL_CHAIN].SetLearnLevel( 100 );
	m_pTypeInfo[SKILL_SOUL_CHAIN].AddNextSkill( SKILL_MAGIC_ELUSION );
	
	
	
	m_pTypeInfo[SKILL_MAGIC_ELUSION].Set(sid++, "Magic Elusion", 0, 0, 226,"Ä§·¨»Ø±Ü");
	m_pTypeInfo[SKILL_MAGIC_ELUSION].SetSlayerSkill();
	m_pTypeInfo[SKILL_MAGIC_ELUSION].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_MAGIC_ELUSION].SetMP( 50 );
	m_pTypeInfo[SKILL_MAGIC_ELUSION].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_MAGIC_ELUSION].AddNextSkill( SKILL_ILLUSION_OF_AVENGE );

	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].Set(sid++, "Illusion Of Avenge", 0, 0, 227,"¸´³ð»ÃÏë");
	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].SetSlayerSkill();
	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].SetMP( 50 );
	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_ILLUSION_OF_AVENGE].AddNextSkill( SKILL_POISON_MESH );


	m_pTypeInfo[SKILL_POISON_MESH].Set(sid++, "Poison Mesh", 0, 0, 228,"¶¾Íø");
	m_pTypeInfo[SKILL_POISON_MESH].SetSlayerSkill();
	m_pTypeInfo[SKILL_POISON_MESH].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_POISON_MESH].SetMP( 50 );
	m_pTypeInfo[SKILL_POISON_MESH].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_POISON_MESH].AddNextSkill( SKILL_WILL_OF_LIFE );

	m_pTypeInfo[SKILL_WILL_OF_LIFE].Set(sid++, "Will Of Life", 0, 0, 229,"ÉúÃüÒâÖ¾");
	m_pTypeInfo[SKILL_WILL_OF_LIFE].SetSlayerSkill();
	m_pTypeInfo[SKILL_WILL_OF_LIFE].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_WILL_OF_LIFE].SetMP( 50 );
	m_pTypeInfo[SKILL_WILL_OF_LIFE].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_WILL_OF_LIFE].AddNextSkill( SKILL_DELEO_EFFICIO );

	// 2004, 9, 4, sobeit add start - ¾Æ¿ì½ºÅÍÁî °ø¼ºÀü ½ºÅ³
	m_pTypeInfo[SKILL_DELEO_EFFICIO].Set(sid++, "Deleo Efficio", 0, 0, 385,"Deleo Efficio"); // 385
	m_pTypeInfo[SKILL_DELEO_EFFICIO].SetOustersSkill();
	m_pTypeInfo[SKILL_DELEO_EFFICIO].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_DELEO_EFFICIO].SetMP( 50 );
	m_pTypeInfo[SKILL_DELEO_EFFICIO].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_DELEO_EFFICIO].AddNextSkill( SKILL_REPUTO_FACTUM );

	m_pTypeInfo[SKILL_REPUTO_FACTUM].Set(sid++, "Reputo Factum", 0, 0, 386,"Reputo Factum"); // 386
	m_pTypeInfo[SKILL_REPUTO_FACTUM].SetOustersSkill();
	m_pTypeInfo[SKILL_REPUTO_FACTUM].SetSkillStep( SKILL_STEP_ETC );
	m_pTypeInfo[SKILL_REPUTO_FACTUM].SetMP( 50 );
	m_pTypeInfo[SKILL_REPUTO_FACTUM].SetLearnLevel( 0 );
	m_pTypeInfo[SKILL_REPUTO_FACTUM].AddNextSkill( SKILL_HALO );
	//m_pTypeInfo[SKILL_REPUTO_FACTUM].AddNextSkill( SKILL_WILL_OF_LIFE );
	// 2004, 9, 4, sobeit add end - ¾Æ¿ì½ºÅÍÁî °ø¼ºÀü ½ºÅ³
	
	// ¾Æ¿ì½ºÅÍÁî ½ÂÁ÷ ½ºÅ³
	// By csm ¾Æ¿ì½ºÅÍÁî ½ÂÁ÷ °øÅë ½ºÅ³ 
	m_pTypeInfo[SKILL_HALO].Set(sid++, "halo", 0, 0, 476,"¹âÔÎ"); 
	m_pTypeInfo[SKILL_HALO].SetOustersSkill();
	m_pTypeInfo[SKILL_HALO].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT_ADVANCEMENT );
	m_pTypeInfo[SKILL_HALO].SetMP( 50 );
	m_pTypeInfo[SKILL_HALO].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_HALO].AddNextSkill( SKILL_DESTINIES );

	m_pTypeInfo[SKILL_DESTINIES].Set(sid++, "Destinies", 0, 0, 477,"ÃüÔË"); 
	m_pTypeInfo[SKILL_DESTINIES].SetOustersSkill();
	m_pTypeInfo[SKILL_DESTINIES].SetSkillStep( SKILL_STEP_OUSTERS_EARTH_ADVANCEMENT );
	m_pTypeInfo[SKILL_DESTINIES].SetMP( 50 );
	m_pTypeInfo[SKILL_DESTINIES].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_DESTINIES].AddNextSkill( SKILL_FIERCE_FLAME );

	m_pTypeInfo[SKILL_FIERCE_FLAME].Set(sid++, "Fierce Flame", 0, 0, 478,"ÁÒ»ð"); 
	m_pTypeInfo[SKILL_FIERCE_FLAME].SetOustersSkill();
	m_pTypeInfo[SKILL_FIERCE_FLAME].SetSkillStep( SKILL_STEP_OUSTERS_FIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_FIERCE_FLAME].SetMP( 50 );
	m_pTypeInfo[SKILL_FIERCE_FLAME].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_FIERCE_FLAME].AddNextSkill( SKILL_SHADOW_OF_STORM );

	m_pTypeInfo[SKILL_SHADOW_OF_STORM].Set(sid++, "Shadow Of Storm", 0, 0, 479,"·ç±©Ö®Ó°"); 
	m_pTypeInfo[SKILL_SHADOW_OF_STORM].SetOustersSkill();
	m_pTypeInfo[SKILL_SHADOW_OF_STORM].SetSkillStep( SKILL_STEP_OUSTERS_WATER_ADVANCEMENT );
	m_pTypeInfo[SKILL_SHADOW_OF_STORM].SetMP( 50 );
	m_pTypeInfo[SKILL_SHADOW_OF_STORM].SetLearnLevel( 150 );

	// add by Coffee 2007-5-1 Ä§Áé»ð·¨ÐÂ¼¼ÄÜ
	m_pTypeInfo[SKILL_SHADOW_OF_STORM].AddNextSkill( SKILL_DUMMY_DRAKE );

	m_pTypeInfo[SKILL_DUMMY_DRAKE].Set(sid++, "Dummy Drake", 0, 0, 508,"µÂÀ×¿Ë¿þÀÜ"); 
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetOustersSkill();
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetSkillStep( SKILL_STEP_OUSTERS_FIRE_ADVANCEMENT );
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetMP( 100 );
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_DUMMY_DRAKE].AddNextSkill( SKILL_HYDRO_CONVERGENCE );
	// Ë®·¨
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].Set(sid++, "Hydro Convergence", 0, 0, 509,"¸´ºÏË®ÁÆ"); 
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetOustersSkill();
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetSkillStep( SKILL_STEP_OUSTERS_WATER_ADVANCEMENT );
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetMP( 100 );
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].AddNextSkill( SKILL_SUMMON_CLAY );
	// ÍÁ·¨
	m_pTypeInfo[SKILL_SUMMON_CLAY].Set(sid++, "Summon Clay", 0 ,0, 510,"Õ³ÍÁÕÙ»½");
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetOustersSkill();
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetSkillStep( SKILL_STEP_OUSTERS_EARTH_ADVANCEMENT );
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetMP( 100 );
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_SUMMON_CLAY].AddNextSkill( SKILL_HETER_CHAKRAM );
	// ÃôÕ½
	m_pTypeInfo[SKILL_HETER_CHAKRAM].Set(sid++, "Heter Chakram", 0 ,0, 507,"ÏÄ²¼Àû»ùÒò");
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetOustersSkill();
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetSkillStep( SKILL_STEP_OUSTERS_COMBAT_ADVANCEMENT );
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetMP( 75 );
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetLearnLevel( 150 );
	m_pTypeInfo[SKILL_HETER_CHAKRAM].AddNextSkill( SKILL_DRAGON_TORNADO );
	//m_pTypeInfo[SKILL_SHADOW_OF_STORM].AddNextSkill( SKILL_DRAGON_TORNADO );

		
	
	//----------------------------------------------------------------------
	//
	//							±âÅ¸ ±â¼úµé
	//
	//----------------------------------------------------------------------
	// SkillTree¿¡´Â ¾È ¾øÁö¸¸, IconÀ» ¼±ÅÃÇØ¼­ »ç¿ëÇÒ ¼ö ÀÖ´Â ±â¼úµé
	//----------------------------------------------------------------------
	
	//----------------------------------------------------------------------
	// ¼º¼ö ´øÁö±â
	//----------------------------------------------------------------------
	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].Set(0, "Throw Holy Water", 0, 0,  101,"Ê¥Ë®Í¶ÖÀ");
	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].SetMP( 0 );

	// ÅÍ·¿ °ø°Ý
	m_pTypeInfo[SKILL_TURRET_FIRE].Set(0, "Turret Fire", 0, 0, 389,"ÅÚËþ»ðÁ¦");
	m_pTypeInfo[SKILL_TURRET_FIRE].SetMP( 0 );
	//----------------------------------------------------------------------
	// Æ÷Å»
	//----------------------------------------------------------------------
	m_pTypeInfo[SUMMON_HELICOPTER].Set(0, "Call Helicopter", 0, 0,  115,"ÕÐ»½-Ö±Éý»ú");
	m_pTypeInfo[SUMMON_HELICOPTER].SetMP( 0 );

	//----------------------------------------------------------------------
	// ÈíÇ÷
	//----------------------------------------------------------------------
	m_pTypeInfo[SKILL_BLOOD_DRAIN].Set(0, "Blood Drain", 0, 0,  102,"ÎüÑª");
	m_pTypeInfo[SKILL_BLOOD_DRAIN].SetVampireSkill();
	m_pTypeInfo[SKILL_BLOOD_DRAIN].SetMP( 0 );

	//----------------------------------------------------------------------
	// ÆøÅº 
	//----------------------------------------------------------------------	
	m_pTypeInfo[BOMB_SPLINTER].Set(0, "Bomb-Splinter", 0, 0, 137,"×ÓÄ¸Õ¨µ¯");
	m_pTypeInfo[BOMB_SPLINTER].SetMP( 0 );

	m_pTypeInfo[BOMB_ACER].Set(0, "Bomb-Acer", 0, 0, 138,"¾«Ó¢Õ¨µ¯");
	m_pTypeInfo[BOMB_ACER].SetMP( 0 );

	m_pTypeInfo[BOMB_BULLS].Set(0, "Bomb-Bulls", 0, 0, 139,"¹«Å£Õ¨µ¯");
	m_pTypeInfo[BOMB_BULLS].SetMP( 0 );

	m_pTypeInfo[BOMB_STUN].Set(0, "Bomb-Stun", 0, 0, 140,"ÖÂÃ¤Õ¨µ¯");
	m_pTypeInfo[BOMB_STUN].SetMP( 0 );

	m_pTypeInfo[BOMB_CROSSBOW].Set(0, "Bomb-Crossbow", 0, 0, 141,"åó¹­Õ¨µ¯");
	m_pTypeInfo[BOMB_CROSSBOW].SetMP( 0 );

	m_pTypeInfo[BOMB_TWISTER].Set(0, "Bomb-Twister", 0, 0, 142,"Å¤ÇúÕ¨µ¯");
	m_pTypeInfo[BOMB_TWISTER].SetMP( 0 );

	//----------------------------------------------------------------------
	// Áö·Ú
	//----------------------------------------------------------------------
	m_pTypeInfo[MINE_ANKLE_KILLER].Set(0, "Mine-AnkleKiller", 0, 0, 143,"µØÀ×-AnkleKiller");
	m_pTypeInfo[MINE_ANKLE_KILLER].SetMP( 0 );
	
	m_pTypeInfo[MINE_POMZ].Set(0, "Mine-Pomz", 0, 0, 144,"µØÀ×Pomz");
	m_pTypeInfo[MINE_POMZ].SetMP( 0 );

	m_pTypeInfo[MINE_AP_C1].Set(0, "Mine-AP-C1", 0, 0, 145,"µØÀ×AP-C1");
	m_pTypeInfo[MINE_AP_C1].SetMP( 0 );
	
	m_pTypeInfo[MINE_DIAMONDBACK].Set(0, "Mine-DiamondBack", 0, 0, 146,"µØÀ×DiamondBack");
	m_pTypeInfo[MINE_DIAMONDBACK].SetMP( 0 );

	m_pTypeInfo[MINE_SWIFT_EX].Set(0, "Mine-Swift-EX", 0, 0, 147,"µØÀ×Swift-EX");
	m_pTypeInfo[MINE_SWIFT_EX].SetMP( 0 );

	m_pTypeInfo[MINE_SIDEWINDER].Set(0, "Mine-SideWinder", 0, 0, 148,"µØÀ×SideWinder");
	m_pTypeInfo[MINE_SIDEWINDER].SetMP( 0 );

	m_pTypeInfo[MINE_COBRA].Set(0, "Mine-Cobra", 0, 0, 149,"µØÀ×Cobra");
	m_pTypeInfo[MINE_COBRA].SetMP( 0 );

	//----------------------------------------------------------------------
	// ´Á´ë ±â¼ú
	//----------------------------------------------------------------------
	m_pTypeInfo[MAGIC_EAT_CORPSE].Set(0, "Eat Corpse", 0, 0, 123,"ÍÌÊÉÊ¬Ìå");
	m_pTypeInfo[MAGIC_EAT_CORPSE].SetVampireSkill();
	m_pTypeInfo[MAGIC_EAT_CORPSE].SetMP( 0 );

	
	m_pTypeInfo[SKILL_BITE_OF_DEATH].Set(0, "Bite Of Death",0 , 0, 341,"ËÀÍöÖ®ÎÇ");
	m_pTypeInfo[SKILL_BITE_OF_DEATH].SetVampireSkill();
	m_pTypeInfo[SKILL_BITE_OF_DEATH].SetMP( 0 );

	//----------------------------------------------------------------------
	// º¯½Å ÇØÁ¦
	//----------------------------------------------------------------------
	// ´Á´ë º¯½Å ÇØÁ¦
	// ¹ÚÁã º¯½Å ÇØÁ¦
	m_pTypeInfo[MAGIC_UN_TRANSFORM].Set(0, "Untransform", 0, 0, 103,"»¹Ô­");
	m_pTypeInfo[MAGIC_UN_TRANSFORM].SetVampireSkill();
	m_pTypeInfo[MAGIC_UN_TRANSFORM].SetMP( 0 );
	
		// Invisible ÇØÁ¦
	m_pTypeInfo[MAGIC_UN_INVISIBILITY].Set(0, "Uninvisiblity", 0, 0, 104,"ÏÖÐÎ");
	m_pTypeInfo[MAGIC_UN_INVISIBILITY].SetVampireSkill();
	m_pTypeInfo[MAGIC_UN_INVISIBILITY].SetMP( 0 );

	//----------------------------------------------------------------------
	// °ü¿¡¼­ ³ª°¡±â [»õ±â¼ú3]
	//----------------------------------------------------------------------
	m_pTypeInfo[MAGIC_OPEN_CASKET].Set(0, "Open Casket", 0, 0, 103,"¿ª¹×");
	m_pTypeInfo[MAGIC_OPEN_CASKET].SetVampireSkill();
	m_pTypeInfo[MAGIC_OPEN_CASKET].SetMP( 0 );

	//----------------------------------------------------------------------
	// HolyLandBonus Skills
	//----------------------------------------------------------------------
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].Set(0, "Blood Bible Armega", 0, 0, 230,"ÑªÖ®Ê¥Êé »ùÂÞ");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE].Set(0, "Blood Bible MIhole", 0, 0, 231,"ÑªÖ®Ê¥Êé °¢ÒÁÄá");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_KIRO].Set(0, "Blood Bible Kiro", 0, 0, 232,"ÑªÖ®Ê¥Êé ¸ñÀ×¸ßÀ÷");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_KIRO].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_KIRO].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_KIRO].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_INI].Set(0, "Blood Bible Ini", 0, 0, 233,"Ê¥Êé ¿×ÇÐÀïÑÇ");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_INI].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_INI].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_INI].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI].Set(0, "Blood Bible Gregori", 0, 0, 234,"ÑªÖ®Ê¥Êé À×¼ª°ÂË¹");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA].Set(0, "Blood Bible Concilia", 0, 0, 235,"ÑªÖ®Ê¥Êé Ï£¶øÀï");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS].Set(0, "Blood Bible Legios", 0, 0, 236,"ÑªÖ®Ê¥Êé ¼Ó¸¥");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL].Set(0, "Blood Bible Hillel", 0, 0, 237,"ÑªÖ®Ê¥Êé ÄÚÂê");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_JAVE].Set(0, "Blood Bible Jave", 0, 0, 238,"ÑªÖ®Ê¥Êé °¢ÂÞÈø");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_JAVE].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_JAVE].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_JAVE].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_NEMA].Set(0, "Blood Bible Nema", 0, 0, 239,"ÑªÖ®Ê¥Êé ²ìË¹·¨");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_NEMA].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_NEMA].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_NEMA].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_AROSA].Set(0, "Blood Bible Arosa", 0, 0, 240,"ÑªÖ®Ê¥Êé °¢ÂÞÈø");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_AROSA].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_AROSA].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_AROSA].SetMP( 0 );

	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA].Set(0, "Blood Bible Chaspa", 0, 0, 241,"ÑªÖ®Ê¥Êé ²ìË¹ÅÁ");
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA].SetVampireSkill();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA].SetPassive();
	m_pTypeInfo[SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA].SetMP( 0 );

	m_pTypeInfo[SKILL_SWEEPER_BONUS_1].Set( 0, "M-03-01", 0, 0, 342, "Sweeper bonus 1");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_1].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_1].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_1].SetMP( 0 );

	m_pTypeInfo[SKILL_SWEEPER_BONUS_2].Set( 0, "M-03-02", 0, 0, 343, "Sweeper bonus 2");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_2].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_2].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_2].SetMP( 0 );

	m_pTypeInfo[SKILL_SWEEPER_BONUS_3].Set( 0, "M-03-03", 0, 0, 344, "Sweeper bonus 3");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_3].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_3].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_3].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_4].Set( 0, "M-03-04", 0, 0, 345, "Sweeper bonus 4");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_4].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_4].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_4].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_5].Set( 0, "M-03-05", 0, 0, 346, "Sweeper bonus 5");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_5].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_5].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_5].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_6].Set( 0, "M-03-06", 0, 0, 347, "Sweeper bonus 6");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_6].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_6].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_6].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_7].Set( 0, "M-03-07", 0, 0, 348, "Sweeper bonus 7");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_7].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_7].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_7].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_8].Set( 0, "M-03-08", 0, 0, 349, "Sweeper bonus 8");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_8].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_8].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_8].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_9].Set( 0, "M-03-09", 0, 0, 350, "Sweeper bonus 9");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_9].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_9].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_9].SetMP( 0 );

	m_pTypeInfo[SKILL_SWEEPER_BONUS_10].Set( 0, "M-03-10", 0, 0, 351, "Sweeper bonus 10");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_10].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_10].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_10].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_11].Set( 0, "M-03-11", 0, 0, 352, "Sweeper bonus 11");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_11].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_11].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_11].SetMP( 0 );
	
	m_pTypeInfo[SKILL_SWEEPER_BONUS_12].Set( 0, "M-03-12", 0, 0, 353, "Sweeper bonus 12");
	m_pTypeInfo[SKILL_SWEEPER_BONUS_12].SetVampireSkill();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_12].SetPassive();
	m_pTypeInfo[SKILL_SWEEPER_BONUS_12].SetMP( 0 );


	//------------------------------------------------------------------------------------------------
	// EffectStatus ¿ë Skill µé
	//------------------------------------------------------------------------------------------------
	m_pTypeInfo[SKILL_LOVE_CHAIN].Set(0, "Love Chain", 0, 0, 245,"°®Äã°®Äã");
	m_pTypeInfo[SKILL_TRANSLATION].Set(0, "Translation", 0, 0, 253,"ÓïÑÔ·­ÒëÆ÷");
	m_pTypeInfo[SKILL_MEGAPHONE].Set(0, "Mega Phone", 0, 0, 312,"À©Òô»ú");
	m_pTypeInfo[SKILL_MUTE].Set(0, "Mute", 0, 0, 320,"¿ÚÕÖ");
	m_pTypeInfo[FAMILY_BONUS].Set(0, "Family Bonus", 0, 0, 326,"¹«»á»ý·Ö");	
	

	m_pTypeInfo[SKILL_CLIENT_CAN_ENTER_GDR_LAIR].Set(0, "Can Enter GDR Lair", 0, 0,  383,"¿ÉÒÔ½øÈëGDR³²Ñ¨");
	m_pTypeInfo[SKILL_CLIENT_BLINDNESS].Set(0, "Blindness", 0, 0,  78,"ÖÂÃ¤");
	m_pTypeInfo[SKILL_PLEASURE_EXPLOSION].Set(0, "Pleasure Station", 0, 0,  384,"ÓäÔÃÒý±¬");

	m_pTypeInfo[SKILL_CLIENT_EXP_BONUS_EVENT].Set(0, "Exp Bonus", 0, 0,  483,"¾­ÑéÖµ½±Àø");

	//m_pTypeInfo[SKILL_CLIENT_DRAGON_EYES].Set(0, "Dragon Eyes", 0, 0,  384,"µå·¹°ïÀÇ ´«"); - ¾ÆÀÌÄÜÀÌ ¾øÀ¸¹Ç·Î ÀÏ´Ü ¹«½Ã..¤»
	
	
	//----------------------------------------------------------------------
	// Skill Step¿¡ µû¶ó¼­..
	//----------------------------------------------------------------------
	// m_listSkillStepÀ» ±¸¼ºÇØÁÖ¸é Ã³¸®ÇÒ¶§ ÆíÇÏ´Ù.
	//----------------------------------------------------------------------
#endif

	//----------------------------------------------------------------------
	// ¹Ù²î´Â Á¤º¸µé ÃÊ±âÈ­
	//----------------------------------------------------------------------
	Init();
}

MSkillInfoTable::~MSkillInfoTable()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
MSkillInfoTable::Init()
{
	for (int i=0; i<MIN_RESULT_ACTIONINFO; i++)
	{
		m_pTypeInfo[i].SetExpLevel( 0 );
		m_pTypeInfo[i].SetSkillExp( 0 );
		m_pTypeInfo[i].SetDelayTime( 0 );
		m_pTypeInfo[i].SetAvailableTime();
		m_pTypeInfo[i].SetEnable();
	}

	
	// delay ¼³Á¤
	m_pTypeInfo[SUMMON_HELICOPTER].SetDelayTime( 3000 );
//	m_pTypeInfo[MAGIC_BLOODY_TUNNEL].SetDelayTime( 3000 );
//	m_pTypeInfo[MAGIC_BLOODY_MARK].SetDelayTime( 3000 );
}
//----------------------------------------------------------------------
// Save From File  ServerSkillInfo
//----------------------------------------------------------------------
void			
MSkillInfoTable::SaveFromFileServerSkillInfo(std::ofstream& file)
{
	int num =MAGIC_BLOODY_TUNNEL_INTO-1;
	file.write((char*)&num, 4);
	for (int i=0;i<num;i++)
	{
		file.write((char*)&i, 4);
		m_pTypeInfo[i].SaveFromFileServerSkillInfo( file );
	}
}
//----------------------------------------------------------------------
// Load From File  ServerSkillInfo
//----------------------------------------------------------------------
void			
MSkillInfoTable::LoadFromFileServerSkillInfo(std::ifstream& file)
{
	int num, skillType;

	file.read((char*)&num, 4);

	for (int i=0; i<num; i++)
	{
		file.read((char*)&skillType, 4);
		if (skillType==219)
		{
			i=i;
		}

		// ÀûÀýÇÑ skillType¿¡ loadingÇÑ´Ù.
		m_pTypeInfo[skillType].LoadFromFileServerSkillInfo( file );
	}

	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].minRange = 8;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].maxRange = 8;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_SPLINTER].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_SPLINTER].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_ACER].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_ACER].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_BULLS].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_BULLS].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_STUN].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_STUN].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_CROSSBOW].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_CROSSBOW].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[BOMB_TWISTER].minRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®
	m_pTypeInfo[BOMB_TWISTER].maxRange = 6;		// ÃÖ¼Ò »çÁ¤°Å¸®

	m_pTypeInfo[MAGIC_RAPID_GLIDING].minRange = 2;
	m_pTypeInfo[MAGIC_RAPID_GLIDING].maxRange = 6;
	
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].minRange = 1;
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].maxRange = 3;
	// add by Coffee 2007-2-25  //ÑªÖ®ÀÓÓ¡
// 	SKILLDOMAIN_BLADE,
// 	SKILLDOMAIN_SWORD,
// 	SKILLDOMAIN_GUN,
// 	SKILLDOMAIN_HEAL,
// 	SKILLDOMAIN_ENCHANT,	
// 	SKILLDOMAIN_ETC,
// 	SKILLDOMAIN_VAMPIRE,	
// 	SKILLDOMAIN_OUSTERS,

	//modify by viva : Notice
#ifdef __NEW_SKILL__
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].Set(150, "Bloody Scarify", 0, 0, 505, "ÑªÖ®ÀÓÓ¡");
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].DomainType = SKILLDOMAIN_VAMPIRE;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].minDamage =60;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].maxDamage =200;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].minDelay =300;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].maxDelay =450;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].minDuration =700;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].maxDuration =700;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetMP(120);
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].minRange=6;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].maxRange=6;
	m_pTypeInfo[SKILL_BLLODY_SCARIFY].SetLearnLevel(150);

	m_pTypeInfo[SKILL_BLOOD_CURSE].Set(150, "Blood Curse", 0, 0, 506, "ÑªÖ®×çÖä");
	m_pTypeInfo[SKILL_BLOOD_CURSE].DomainType = SKILLDOMAIN_VAMPIRE;
	m_pTypeInfo[SKILL_BLOOD_CURSE].minDamage =60;
	m_pTypeInfo[SKILL_BLOOD_CURSE].maxDamage =220;
	m_pTypeInfo[SKILL_BLOOD_CURSE].minDelay =450;
	m_pTypeInfo[SKILL_BLOOD_CURSE].maxDelay =450;
	m_pTypeInfo[SKILL_BLOOD_CURSE].minDuration =200;
	m_pTypeInfo[SKILL_BLOOD_CURSE].maxDuration =200;
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetMP(60);
	m_pTypeInfo[SKILL_BLOOD_CURSE].minRange=6;
	m_pTypeInfo[SKILL_BLOOD_CURSE].maxRange=6;
	m_pTypeInfo[SKILL_BLOOD_CURSE].SetLearnLevel(150);

	m_pTypeInfo[SKILL_SHINE_SWORD].Set(150, "Shine Sword", 700, 270,  497,"ÉÁÒ«Ö®½£");
	m_pTypeInfo[SKILL_SHINE_SWORD].DomainType = SKILLDOMAIN_SWORD;
	m_pTypeInfo[SKILL_SHINE_SWORD].minDamage =60;
	m_pTypeInfo[SKILL_SHINE_SWORD].maxDamage =165;
	m_pTypeInfo[SKILL_SHINE_SWORD].minDelay =100;
	m_pTypeInfo[SKILL_SHINE_SWORD].maxDelay =450;
	m_pTypeInfo[SKILL_SHINE_SWORD].minDuration =200;
	m_pTypeInfo[SKILL_SHINE_SWORD].maxDuration =200;
	m_pTypeInfo[SKILL_SHINE_SWORD].SetMP(50);
	m_pTypeInfo[SKILL_SHINE_SWORD].minRange=7;
	m_pTypeInfo[SKILL_SHINE_SWORD].maxRange=7;
	m_pTypeInfo[SKILL_SHINE_SWORD].SetLearnLevel(150);


	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].Set(150, "Bomb Crash Walk", 700, 270,  499,"¾ÞÅÚºäÕ¨");
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].DomainType = SKILLDOMAIN_BLADE;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].minDamage =60;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].maxDamage =185;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].minDelay =300;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].maxDelay =300;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].minDuration =200;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].maxDuration =200;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].SetMP(50);
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].minRange=7;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].maxRange=7;
	m_pTypeInfo[SKILL_BOMB_CRASH_WALK].SetLearnLevel(150);

	m_pTypeInfo[SKILL_SATELLITE_BOMB].Set(150, "Satellite Bomb", 0, 0, 500,"ÎÀÐÇºä»÷");
	m_pTypeInfo[SKILL_SATELLITE_BOMB].DomainType = SKILLDOMAIN_GUN;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].minDamage =60;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].maxDamage =155;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].minDelay =100;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].maxDelay =450;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].minDuration =200;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].maxDuration =200;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].SetMP(40);
	m_pTypeInfo[SKILL_SATELLITE_BOMB].minRange=7;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].maxRange=7;
	m_pTypeInfo[SKILL_SATELLITE_BOMB].SetLearnLevel(150);

	m_pTypeInfo[SKILL_ILLUSION_INVERSION].Set(150, "Illusion Inversion", 700, 270,  501,"¿Ö²À»Ã¾õ");
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].DomainType = SKILLDOMAIN_HEAL;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].minDamage =60;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].maxDamage =220;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].minDelay =270;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].maxDelay =270;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].minDuration =200;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].maxDuration =200;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].SetMP(100);
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].minRange=6;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].maxRange=6;
	m_pTypeInfo[SKILL_ILLUSION_INVERSION].SetLearnLevel(150);


	m_pTypeInfo[SKILL_HEAVEN_GROUND].Set(150, "Heaven Ground", 700, 270,  502,"ÌìÉñ½µÁÙ");
	m_pTypeInfo[SKILL_HEAVEN_GROUND].DomainType = SKILLDOMAIN_ENCHANT;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].minDamage =60;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].maxDamage =220;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].minDelay =400;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].maxDelay =400;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].minDuration =200;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].maxDuration =200;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].SetMP(100);
	m_pTypeInfo[SKILL_HEAVEN_GROUND].minRange=6;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].maxRange=6;
	m_pTypeInfo[SKILL_HEAVEN_GROUND].SetLearnLevel(150);

	m_pTypeInfo[SKILL_DUMMY_DRAKE].Set(150, "Dummy Drake", 0, 0, 508,"µÂÀ×¿Ë¿þÀÜ"); 
	m_pTypeInfo[SKILL_DUMMY_DRAKE].DomainType = SKILLDOMAIN_OUSTERS;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].minDamage =60;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].maxDamage =220;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].minDelay =400;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].maxDelay =400;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].minDuration =200;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].maxDuration =200;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetMP(100);
	m_pTypeInfo[SKILL_DUMMY_DRAKE].minRange=5;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].maxRange=5;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SetLearnLevel(150);
	m_pTypeInfo[SKILL_DUMMY_DRAKE].Fire=25;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].ElementalDomain= SKILLINFO_NODE::ELEMENTAL_DOMAIN_FIRE;
	m_pTypeInfo[SKILL_DUMMY_DRAKE].SkillTypeList.push_back(349);

	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].Set(150, "Hydro Convergence", 0, 0, 509,"¸´ºÏË®ÁÆ");
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].DomainType = SKILLDOMAIN_OUSTERS;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].minDamage =60;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].maxDamage =150;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].minDelay =300;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].maxDelay =300;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].minDuration =300;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].maxDuration =300;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetMP(95);
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].minRange=5;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].maxRange=5;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SetLearnLevel(150);
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].Water=25;
	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].ElementalDomain= SKILLINFO_NODE::ELEMENTAL_DOMAIN_WATER;
 	m_pTypeInfo[SKILL_HYDRO_CONVERGENCE].SkillTypeList.push_back(351);

	m_pTypeInfo[SKILL_SUMMON_CLAY].Set(150, "Summon Clay", 0, 0, 510,"Õ³ÍÁÕÙ»½");
	m_pTypeInfo[SKILL_SUMMON_CLAY].DomainType = SKILLDOMAIN_OUSTERS;
	m_pTypeInfo[SKILL_SUMMON_CLAY].minDamage =60;
	m_pTypeInfo[SKILL_SUMMON_CLAY].maxDamage =150;
	m_pTypeInfo[SKILL_SUMMON_CLAY].minDelay =400;
	m_pTypeInfo[SKILL_SUMMON_CLAY].maxDelay =400;
	m_pTypeInfo[SKILL_SUMMON_CLAY].minDuration =200;
	m_pTypeInfo[SKILL_SUMMON_CLAY].maxDuration =200;
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetMP(100);
	m_pTypeInfo[SKILL_SUMMON_CLAY].minRange=5;
	m_pTypeInfo[SKILL_SUMMON_CLAY].maxRange=5;
	m_pTypeInfo[SKILL_SUMMON_CLAY].SetLearnLevel(150);
	m_pTypeInfo[SKILL_SUMMON_CLAY].Earth=25;
	m_pTypeInfo[SKILL_SUMMON_CLAY].ElementalDomain= SKILLINFO_NODE::ELEMENTAL_DOMAIN_EARTH;
 	m_pTypeInfo[SKILL_SUMMON_CLAY].SkillTypeList.push_back(352);

	m_pTypeInfo[SKILL_HETER_CHAKRAM].Set(150, "Heter Chakram", 0 ,0, 507,"ÏÄ²¼Àû»ùÒò");
	m_pTypeInfo[SKILL_HETER_CHAKRAM].DomainType = SKILLDOMAIN_OUSTERS;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].minDamage =60;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].maxDamage =150;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].minDelay =400;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].maxDelay =400;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].minDuration =200;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].maxDuration =200;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetMP(75);
	m_pTypeInfo[SKILL_HETER_CHAKRAM].minRange=4;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].maxRange=4;
	m_pTypeInfo[SKILL_HETER_CHAKRAM].SetLearnLevel(150);
	m_pTypeInfo[SKILL_HETER_CHAKRAM].ElementalDomain= SKILLINFO_NODE::ELEMENTAL_DOMAIN_NO_DOMAIN;
 	m_pTypeInfo[SKILL_HETER_CHAKRAM].SkillTypeList.push_back(304);

#endif
}