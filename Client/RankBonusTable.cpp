//---------------------------------------------------------------------------------
// RankBonusTable.cpp
//---------------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MString.h"
#include "RankBonusTable.h"
#include "RankBonusDef.h"

//---------------------------------------------------------------------------------
// Global
//---------------------------------------------------------------------------------
RankBonusTable* g_pRankBonusTable = NULL;

//---------------------------------------------------------------------------------
//
//					StatusInfo
//
//---------------------------------------------------------------------------------
RankBonusInfo::RankBonusInfo()
{
	m_type = 0;
	m_level = 0;
	m_race = RACE_SLAYER;	// 0 : Slayer  1 : Vampire
	m_skillIconID = 0;
	m_status = STATUS_NULL;
	m_point = 0;
}

void				
RankBonusInfo::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&m_type, 2);
	m_Name.LoadFromFile(file);
	file.read((char*)&m_level, 1);
	file.read((char*)&m_race, 1);
	file.read((char*)&m_point, 4);
	file.read((char*)&m_skillIconID, 2);
}

#ifndef __GAME_CLIENT__
void		
RankBonusInfo::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&m_type, 2);
	m_Name.SaveToFile(file);
	file.write((const char*)&m_level, 1);
	file.write((const char*)&m_race, 1);
	file.write((const char*)&m_point, 4);
	file.write((const char*)&m_skillIconID, 2);
}


RankBonusTable::RankBonusTable()
{
	Init(RANK_BONUS_MAX);

/////////////////////////////////////////////
// Slayer Rank Bonus Skill
/////////////////////////////////////////////

	// 0 Deadly Spear Level : 5 race : Slayer
	Get(RANK_BONUS_DEADLY_SPEAR).SetSkillIconID(177);

	// 1 Behemoth Armor Level : 5 race : Slayer
	Get(RANK_BONUS_BEHEMOTH_ARMOR).SetSkillIconID(178);

	// 2 Dragon Eye Level : 5 race : Slayer
	Get(RANK_BONUS_DARGON_EYE).SetSkillIconID(179);

	// 3 Immortal Heart Level : 5 race : Slayer
	Get(RANK_BONUS_IMMORTAL_HEART).SetSkillIconID(180);

	// 4 Reliance Brain Level : 5 race : Slayer
	Get(RANK_BONUS_RELIANCE_BRAIN).SetSkillIconID(181);

	// 5 Slaying Knife Level : 10 race : Slayer
	Get(RANK_BONUS_SLAYING_KNIFE).SetSkillIconID(182);

	// 6 Hawk Wing Level : 10 race : Slayer
	Get(RANK_BONUS_HAWK_WING).SetSkillIconID(183);

	// 7 Holy Smashing Level : 10 race : Slayer
	Get(RANK_BONUS_HOLY_SMASHING).SetSkillIconID(184);

	// 8 Soul Smashing Level : 10 race : Slayer
	Get(RANK_BONUS_SOUL_SMASHING).SetSkillIconID(185);

	// 9 Sapphire Bless Level : 15 race : Slayer
	Get(RANK_BONUS_SAPPHIRE_BLESS).SetSkillIconID(186);

	// 10 Ruby Bless Level : 15 race : Slayer
	Get(RANK_BONUS_RUBY_BLESS).SetSkillIconID(187);

	// 11 Diamond Bless Level : 15 race : Slayer
	Get(RANK_BONUS_DIAMOND_BLESS).SetSkillIconID(188);

	// 12 Emerald Bless Level : 15 race : Slayer
	Get(RANK_BONUS_EMERALD_BLESS).SetSkillIconID(189);

	// 13 Magic Brain Level : 20 race : Slayer
	Get(RANK_BONUS_MAGIC_BRAIN).SetSkillIconID(190);

	// 14 Wight Hand Level : 20 race : Slayer
	Get(RANK_BONUS_WIGHT_HAND).SetSkillIconID(191);

	// 15 Seiren Hand Level : 20 race : Slayer
	Get(RANK_BONUS_SEIREN_HAND).SetSkillIconID(192);

	// 16 Fortune Hand Level : 20 race : Slayer
	Get(RANK_BONUS_FORTUNE_HAND).SetSkillIconID(193);


/////////////////////////////////////////////
// Vampire Rank Bonus Skill
/////////////////////////////////////////////

	// 17 Immortal Blood Level : 5 race : Vampire
	Get(RANK_BONUS_IMMORTAL_BLOOD).SetSkillIconID(194);

	// 18 Behemoth Skin Level : 5 race : Vampire
	Get(RANK_BONUS_BEHEMOTH_SKIN).SetSkillIconID(195);

	// 19 Safe Robe Level : 5 race : Vampire
	Get(RANK_BONUS_SAFE_ROBE).SetSkillIconID(196);

	// 20 Crow Wing Level : 10 race : Vampire
	Get(RANK_BONUS_CROW_WING).SetSkillIconID(197);

	// 21 Wisdom of Blood Level : 10 race : Vampire
	Get(RANK_BONUS_WISDOM_OF_BLOOD).SetSkillIconID(198);

	// 22 Tiger Nail Level : 10 race : Vampire
	Get(RANK_BONUS_TIGER_NAIL).SetSkillIconID(199);

	// 23 Uranus Bless Level : 10 race : Vampire
	Get(RANK_BONUS_URANUS_BLESS).SetSkillIconID(200);

	// 24 Disruption Storm Level : 15 race : Vampire
	Get(RANK_BONUS_DISRUPTION_STORM).SetSkillIconID(201);

	// 25 Wide Storm Level : 15 race : Vampire
	Get(RANK_BONUS_WIDE_STORM).SetSkillIconID(202);

	// 26 Knowledge of Poison Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_POISON).SetSkillIconID(203);

	// 27 Knowledge of Acid Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_ACID).SetSkillIconID(204);

	// 28 Knowledge of Curse Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_CURSE).SetSkillIconID(205);

	// 29 Knowledge of Blood Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_BLOOD).SetSkillIconID(206);

	// 30 Knowledge of Innate Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_INNATE).SetSkillIconID(207);

	// 31 Knowledge of Summon Level : 15 race : Vampire
	Get(RANK_BONUS_KNOWLEDGE_OF_SUMMON).SetSkillIconID(208);

	// 32 Wisdom of Swamp Level : 20 race : Vampire
	Get(RANK_BONUS_WISDOM_OF_SWAMP).SetSkillIconID(209);

	// 33 Wisdom of Silence Level : 20 race : Vampire
	Get(RANK_BONUS_WISDOM_OF_SILENCE).SetSkillIconID(210);

	// 34 Wisdom of Darkness Level : 20 race : Vampire
	Get(RANK_BONUS_WISDOM_OF_DARKNESS).SetSkillIconID(211);

	// 35 Wide Darkness Level : 20 race : Vampire
	Get(RANK_BONUS_WIDE_DARKNESS).SetSkillIconID(212);
	
	Get(RANK_BONUS_WOOD_SKIN).SetSkillIconID( 290 );
	Get(RANK_BONUS_WIND_SENSE).SetSkillIconID( 291 );
	Get(RANK_BONUS_HOMING_EYE).SetSkillIconID( 292 );
	Get(RANK_BONUS_LIFE_ENERGY).SetSkillIconID( 293 );
	Get(RANK_BONUS_SOUL_ENERGY).SetSkillIconID( 294 );
	Get(RANK_BONUS_STONE_MAUL).SetSkillIconID( 295 );
	Get(RANK_BONUS_SWIFT_ARM).SetSkillIconID( 296 );
	Get(RANK_BONUS_FIRE_ENDOW).SetSkillIconID( 297 );
	Get(RANK_BONUS_WATER_ENDOW).SetSkillIconID( 298 );
	Get(RANK_BONUS_EARTH_ENDOW).SetSkillIconID( 299);
	Get(RANK_BONUS_ANTI_ACID_SKIN).SetSkillIconID( 300);
	Get(RANK_BONUS_ANTI_BLOODY_SKIN).SetSkillIconID( 301 );
	Get(RANK_BONUS_ANTI_CURSE_SKIN).SetSkillIconID( 302 );
	Get(RANK_BONUS_ANTI_POISON_SKIN).SetSkillIconID( 303 );
	Get(RANK_BONUS_ANTI_SILVER_DAMAGE_SKIN).SetSkillIconID( 304 );
	Get(RANK_BONUS_BLESS_OF_NATURE).SetSkillIconID( 305 );
	Get(RANK_BONUS_LIFE_ABSORB).SetSkillIconID( 306 );
	Get(RANK_BONUS_SOUL_ABSORB).SetSkillIconID( 307 );
	Get(RANK_BONUS_MYSTIC_RULE).SetSkillIconID( 308 );

	int TempSkillIcon = 394;
	Get(RANK_BONUS_PERCEPTION).SetSkillIconID( TempSkillIcon++ );
	Get(RANK_BONUS_STONE_OF_SAGE).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_FOOT_OF_RANGER).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_WARRIORS_FIST).SetSkillIconID( TempSkillIcon++ );

    Get(RANK_BONUS_ACID_INQUIRY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_BLOODY_INQUIRY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_CURSE_INQUIRY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_POISON_INQUIRY).SetSkillIconID( TempSkillIcon++ );
	Get(RANK_BONUS_INQUIRY_MASTERY).SetSkillIconID( TempSkillIcon++ );

    Get(RANK_BONUS_POWER_OF_SPIRIT).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_WIND_OF_SPIRIT).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_PIXIES_EYES).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_GROUND_OF_SPIRIT).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_FIRE_OF_SPIRIT).SetSkillIconID( TempSkillIcon++ );

    Get(RANK_BONUS_EVOLUTION_IMMORTAL_HEART).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_BEHEMOTH_ARMOR_2).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_DRAGON_EYE_2).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_EVOLUTION_RELIANCE_BRAIN).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_HEAT_CONTROL).SetSkillIconID( TempSkillIcon++ );

	Get(RANK_BONUS_ACID_MASTERY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_BLOODY_MASTERY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_CURSE_MASTERY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_POISON_MASTERY).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_SKILL_MASTERY).SetSkillIconID( TempSkillIcon++ );

    Get(RANK_BONUS_SALAMANDERS_KNOWLEDGE).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_UNDINES_KNOWLEDGE).SetSkillIconID( TempSkillIcon++ );
    Get(RANK_BONUS_GNOMES_KNOWLEDGE).SetSkillIconID( TempSkillIcon++ );


}
#endif