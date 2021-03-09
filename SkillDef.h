//----------------------------------------------------------------------
// SkillDef.h
//----------------------------------------------------------------------

#ifndef	__SKILLDEF_H__
#define	__SKILLDEF_H__

// defines
#define SELF_SKILL_MP_POINT			5
#define TILE_SKILL_MP_POINT			5
#define OBJECT_SKILL_MP_POINT		5

#define MAX_VAMPIRE_SKILL_SLOT		20	// Test
#define MAX_SLAYER_SKILL_SLOT		20


//----------------------------------------------------------------------
// Skill Domain
//----------------------------------------------------------------------
enum SKILLDOMAIN {
	SKILLDOMAIN_BLADE,
	SKILLDOMAIN_SWORD,
	SKILLDOMAIN_GUN,
	SKILLDOMAIN_HEAL,
	SKILLDOMAIN_ENCHANT,	
	SKILLDOMAIN_ETC,
	SKILLDOMAIN_VAMPIRE,	
	SKILLDOMAIN_OUSTERS,	

	MAX_SKILLDOMAIN
};

// Name String
extern int SKILLDOMAIN_NAME[MAX_SKILLDOMAIN];

//----------------------------------------------------------------------
// Skill Step
//----------------------------------------------------------------------
enum SKILL_STEP {
	SKILL_STEP_NULL,			// 관계없는 것들

	// Slayer
	SKILL_STEP_APPRENTICE,
	SKILL_STEP_ADEPT,
	SKILL_STEP_EXPERT,
	SKILL_STEP_MASTER, 
	SKILL_STEP_GRAND_MASTER,

	// Vampire
	SKILL_STEP_VAMPIRE_POISON,
	SKILL_STEP_VAMPIRE_ACID,
	SKILL_STEP_VAMPIRE_CURSE,
	SKILL_STEP_VAMPIRE_SUMMON,
	SKILL_STEP_VAMPIRE_BLOOD,
	SKILL_STEP_VAMPIRE_INNATE,
	
	// Ousters
	SKILL_STEP_OUSTERS_COMBAT,
	SKILL_STEP_OUSTERS_ELEMENTAL,
	SKILL_STEP_OUSTERS_ETC,

	SKILL_STEP_ETC,

};

//----------------------------------------------------------------------
// SkillID == ActionInfo 정의
//----------------------------------------------------------------------
enum ACTIONINFO
{
		// 검계열,
	SKILL_ATTACK_MELEE = 0,						// 0
	SKILL_ATTACK_ARMS,							// 1
	SKILL_SELF,									// 2
	SKILL_TILE,									// 3
	SKILL_OBJECT,	    						// 4

	SKILL_DOUBLE_IMPACT,  					  	// 5
	SKILL_TRIPLE_SLASHER,				  	  	// 6
	SKILL_RAINBOW_SLASHER,					  	// 7
	SKILL_THUNDER_SPARK,	   				 	// 8
	SKILL_DANCING_SWORD,						// 92
	SKILL_CROSS_COUNTER,		   			 	// 10
	SKILL_FLASH_SLIDING,		  			  	// 11
	SKILL_LIGHTNING_HAND,	  					// 12
	SKILL_SNAKE_COMBO,	  	 				 	// 9
	SKILL_SWORD_WAVE,		  	 				// 13
	SKILL_DRAGON_RISING,		 			    // 14
	SKILL_FIVE_STORM_CRASH,	 				 	// 15
	SKILL_HEAVENS_SWORD,		  			  	// 16

	// 도 계열,
	SKILL_SINGLE_BLOW,						    // 17
	SKILL_SPIRAL_SLAY,						    // 18
	SKILL_TRIPLE_BREAK,						    // 19
	SKILL_WILD_SMASH,						    // 20
	SKILL_GHOST_BLADE,							// 93
	SKILL_POTENTIAL_EXPLOSION,			  		// 22
	SKILL_SHADOW_WALK,			  			  	// 23
	SKILL_CHARGING_POWER,		  			  	// 24
	SKILL_HURRICANE_COMBO,	 				 	// 21
	SKILL_TORNADO_SEVER,		  			  	// 25
	SKILL_ARMAGEDDON_SLASHER,	  				// 26
	SKILL_SOUL_SHOCK,			   				// 27
	SKILL_SAINT_BLADE,			   			  	// 28

	// 건, 라이플 계열
	SKILL_FAST_RELOAD,						    // 30
	SKILL_QUICK_FIRE,							//
	SKILL_SMG_MASTERY,							// 
	SKILL_MULTI_SHOT,						    // 94
	SKILL_HEAD_SHOT,						    // 94
	SKILL_AR_MASTERY,							// 
	SKILL_PIERCING,						    	// 97
	SKILL_SNIPPING,						    	// 95
	SKILL_SG_MASTERY,							//
	SKILL_DETECT_MINE,						    // 35
	SKILL_MAKE_BOMB,		 				    // 36
	SKILL_SR_MASTERY,
	SKILL_DISARM_MINE,						    // 37
	SKILL_INSTALL_MINE,						    // 37
	SKILL_MAKE_MINE,			  			    // 38
    
	// 성직마법,
	// 인첸 계열
	MAGIC_CREATE_HOLY_WATER, 			 		// 39
	MAGIC_LIGHT,				      			// 40
	MAGIC_DETECT_HIDDEN,	   	 				// 41
	MAGIC_AURA_BALL,	     		 		 	// 55
	MAGIC_BLESS,			       		 		// 45
	MAGIC_CONTINUAL_LIGHT,	 			 		// 46
	MAGIC_FLARE,								// 얼마일까 다시 정리 해야지
	MAGIC_PURIFY,    						// 43
	MAGIC_AURA_RING,		  	  		  		// 42
	MAGIC_STRIKING,		      					// 44
	MAGIC_DETECT_INVISIBILITY,					// 48
	MAGIC_IDENTIFY,								//
	MAGIC_AURA_SHIELD,	    					// 60
	MAGIC_ENCHANT,	    					// 60
	MAGIC_VISIBLE,			    			  	// 49
	MAGIC_CHAIN_AURA,							// 50
	MAGIC_SAINT_AURA,							// 51

	// 힐 계열
	MAGIC_CURE_LIGHT_WOUNDS,					// 52
	MAGIC_CURE_POISON,							// 54
	MAGIC_PROTECTION_FROM_POISON,				// 53
	MAGIC_CAUSE_LIGHT_WOUNDS,					// 얼마일까 다시 정리 해야지
	MAGIC_CURE_SERIOUS_WOUNDS,					// 56
	MAGIC_REMOVE_CURSE,							// 89
	MAGIC_PROTECTION_FROM_CURSE,				// 59
	MAGIC_CAUSE_SERIOUS_WOUNDS,					// 얼마일까 다시 정리 해야지
	MAGIC_CURE_CRITICAL_WOUNDS,					// 
	MAGIC_PROTECTION_FROM_ACID,					//
	MAGIC_SACRIFICE,							//
	MAGIC_CAUSE_CRITICAL_WOUNDS,				// 얼마일까 다시 정리 해야지
	MAGIC_CURE_ALL,								// 62
	MAGIC_REGENERATION,							// 61
	MAGIC_MASS_CURE,							//
	MAGIC_MASS_HEAL,							//

	// 뱀프기술,
	SKILL_BLOOD_DRAIN,							// 65

	// 뱀프마법,
	// Poison
	MAGIC_POISONOUS_HANDS,						// 67
	MAGIC_GREEN_POISON,							// 69
	MAGIC_YELLOW_POISON,						// 70
	MAGIC_DARKBLUE_POISON,						// 79
	MAGIC_GREEN_STALKER,						// 75

	// Acid
	MAGIC_ACID_TOUCH,							// 68
	MAGIC_ACID_BOLT,							// 73
	MAGIC_ACID_BALL,							// 78
	MAGIC_ACID_SWAMP,							// 81

	// Curse
	MAGIC_PARALYZE,								// 74
	MAGIC_DOOM,									// 74
	MAGIC_HALLUCINATION,							//
	MAGIC_DEATH,								//

	// Bloody
	MAGIC_BLOODY_NAIL,							// 82
	MAGIC_BLOODY_KNIFE,							// 83
	MAGIC_BLOODY_BALL,							// 84
	MAGIC_BLOODY_WALL,							// 85
	MAGIC_BLOODY_SPEAR,							// 86

	// Innate
	MAGIC_HIDE,									// 66
	MAGIC_DARKNESS,								// 71
//	MAGIC_PURPLE_WALL,							// 72
	MAGIC_INVISIBILITY,							// 76
	MAGIC_TRANSFORM_TO_WOLF,					// 77
	MAGIC_TRANSFORM_TO_BAT,						// 80

	// 써몬
	MAGIC_SUMMON_WOLF,							// 87
	MAGIC_SUMMON_CASKET,						// 88
	MAGIC_RAISING_DEAD,							// 88
	MAGIC_SUMMON_SERVANT,						//

	// ETC
	MAGIC_UN_BURROW,							// 89
	MAGIC_UN_TRANSFORM,							// 89
	MAGIC_UN_INVISIBILITY,
	MAGIC_THROW_HOLY_WATER,						// 91

	// 늑대 기술
	MAGIC_EAT_CORPSE,
	MAGIC_HOWL,

	// NPC 기술
	MAGIC_RESTORE,								// 90

	MAGIC_BLOODY_MARK,
	MAGIC_BLOODY_TUNNEL,

	MAGIC_SEDUCTION,
	SKILL_WIND_DIVIDER,
	SKILL_EARTHQUAKE,

	MAGIC_RESURRECT,
	MAGIC_PRAYER,
	MAGIC_MEDITATION,

	// 2001.12.4 추가
	SKILL_THUNDER_BOLT,							// 122
	SKILL_EXPANSION, 							// 123
	SKILL_MIRACLE_SHIELD, 						// 124
	SKILL_THUNDER_FLASH,						// 125
	SKILL_MASSACRE,								// 126
	SKILL_INVINCIBLE,							// 127

	SKILL_BERSERKER,							// 128
	SKILL_MOONLIGHT_SEVER,						// 129
	SKILL_SHADOW_DANCING,						// 130
	SKILL_TYPHOON,								// 131
	SKILL_PSYCHOKINESIS,						// 132
	SKILL_EXTERMINATION,						// 133

	SKILL_MIND_CONTROL,							// 134
	SKILL_REVOLVING,							// 135
	SKILL_FATALITY,								// 136
	SKILL_BLITZ,								// 137

	SKILL_ACTIVATION,							// 138
	SKILL_PEACE,								// 139
	SKILL_ENERGY_DROP,							// 140
	SKILL_EXORCISM,								// 141

	SKILL_SANCTUARY,							// 142
	SKILL_REFLECTION,							// 143
	SKILL_ARMAGEDDON,							// 144

	
	SKILL_POISON_STRIKE,						// 145
	SKILL_POISON_STORM,							// 146
	SKILL_ACID_STRIKE,							// 147
	SKILL_ACID_STORM,							// 148
	SKILL_BLOODY_STRIKE,						// 149
	SKILL_BLOODY_STORM,							// 150
	
	// 2002.3.23
	SKILL_SUMMON_BAT,
	SKILL_CHARM,
	SKILL_POLYMORPH,
	SKILL_MEPHISTO,
	SKILL_HYPNOSIS,
	SKILL_TRANSFUSION,
	SKILL_EXTREME,
	SKILL_BLOODY_WAVE,	

	SKILL_THROW_BOMB,

	SKILL_DOUBLE_SHOT,
	SKILL_TRIPLE_SHOT,
	
	SKILL_CURE_EFFECT,			// cure
	SKILL_CRITICAL_EFFECT,
	SKILL_CRITICAL_GROUND,		// critical을 사용하는 경우	
	
	SKILL_VIGOR_DROP,
	
	// [새기술]
	SKILL_SWORD_MASTERY,                        // 166
    SKILL_SHIELD_MASTERY,                       // 167
    SKILL_THUNDER_STORM,                        // 168
    SKILL_CONCENTRATION,                        // 169
    SKILL_EVASION,                              // 170
    SKILL_HOLY_BLAST,                           // 171
    SKILL_HYMN,                                 // 172
	SKILL_MENTAL_SWORD,							// 173
	SKILL_OBSERVING_EYE,						// 174
	SKILL_REFLECTION_EFFECT,					// 175

	// 2002.6.21
	SKILL_TEMP,									// 176
	MAGIC_OPEN_CASKET,							// 177

	// 2002.9.2
	MAGIC_SUMMON_MONSTERS,						// 178
	MAGIC_GROUND_ATTACK,						// 179
	MAGIC_METEOR,								// 180
	MAGIC_DUPLICATE_SELF,						// 181
	MAGIC_BLOODY_MASTER_WAVE,							// 182

	// 2002.10.1
    MAGIC_BLOODY_WARP,                          // 183
    MAGIC_BLOODY_SNAKE,                         // 184

	// 2002.10.23
	SKILL_SOUL_CHAIN,							// 185

	// 2002.11.18
	SKILL_LIVENESS,								// 186

	// 2002.11.20
	MAGIC_DARKNESS_WIDE,							// 187
	SKILL_POISON_STORM_WIDE,						// 188
	SKILL_ACID_STORM_WIDE,							// 189

    // 2002.12.26
    SKILL_SHARP_SHIELD,                         // 190
    SKILL_WIDE_LIGHTNING,                       // 191
    SKILL_AIR_SHIELD,                           // 192
    SKILL_POWER_OF_LAND,                        // 193
    SKILL_BULLET_OF_LIGHT,                      // 194
    SKILL_GUN_SHOT_GUIDANCE,                    // 195
    SKILL_REBUKE,                               // 196
    SKILL_SPIRIT_GUARD,                         // 197
    SKILL_TURN_UNDEAD,                          // 198

	SKILL_HANDS_OF_WISDOM,						// 199
	SKILL_LIGHT_BALL,							// 200
	SKILL_HOLY_ARROW,							// 201

	// 2003.2.26 

	MAGIC_BLOODY_BREAKER,						// 202
	MAGIC_RAPID_GLIDING,						// 203

    SKILL_MAGIC_ELUSION,                        // 204
    SKILL_POISON_MESH,                          // 205
    SKILL_ILLUSION_OF_AVENGE,                   // 206
    SKILL_WILL_OF_LIFE,                         // 207
	
	SKILL_DENIAL_MAGIC,							// 208
	SKILL_REQUITAL,								// 209
	SKILL_CONCEALMENT,							// 210
	SKILL_SWORD_RAY,							// 211
	SKILL_MULTI_AMPUTATE,						// 212
	SKILL_NAIL_MASTERY,							// 213	
	
	SKILL_HIT_CONVERT,							// 214
	SKILL_WILD_TYPHOON,							// 215
	SKILL_ULTIMATE_BLOW,						// 216
	SKILL_ILLENDUE,								// 217
	SKILL_LIGHTNESS,							// 218

	SKILL_FLOURISH,                             // 219
    SKILL_EVADE,                                // 220
    SKILL_SHARP_ROUND,                          // 221
    SKILL_HIDE_SIGHT,                           // 222
    SKILL_BACK_STAB,                            // 223
    SKILL_BLUNTING,                             // 224
    SKILL_GAMMA_CHOP,                           // 225

    SKILL_CROSS_GUARD,                          // 226
    SKILL_FIRE_OF_SOUL_STONE,                   // 227
    SKILL_ICE_OF_SOUL_STONE,                    // 228
    SKILL_SAND_OF_SOUL_STONE,                   // 229
    SKILL_BLOCK_HEAD,                           // 230
    SKILL_KASAS_ARROW,                          // 231
    SKILL_HANDS_OF_FIRE,                        // 232

    SKILL_PROMINENCE,                           // 233
    SKILL_RING_OF_FLARE,                        // 234
    SKILL_BLAZE_BOLT,                           // 235
    SKILL_ICE_FIELD,                            // 236
    SKILL_WATER_BARRIER,                        // 237
    SKILL_HANDS_OF_NIZIE,                       // 238
    SKILL_NYMPH_RECOVERY,                       // 239

    SKILL_LIBERTY,                              // 240
    SKILL_TENDRIL,                              // 241
    SKILL_GNOMES_WHISPER,                       // 242
    SKILL_STONE_AUGER,                          // 243
    SKILL_REFUSAL_ETHER,                        // 244
    SKILL_EARTHS_TEETH,                         // 245
    SKILL_ABSORB_SOUL,                          // 246

    SKILL_SUMMON_SYLPH,                         // 247
    SKILL_DRIFTING_SOUL,                        // 248
    SKILL_CRITICAL_MAGIC,                       // 249
	
	SKILL_EMISSION_WATER,						// 250
	SKILL_BEAT_HEAD,							// 251
	SKILL_DIVINE_SPIRITS,						// 252
	
	SKILL_BLITZ_SLIDING,                        // 253
    SKILL_BLAZE_WALK,                           // 254
    SKILL_JABBING_VEIN,                         // 255
    SKILL_GREAT_HEAL,                           // 256
    SKILL_DIVINE_GUIDANCE,                      // 257

	MAGIC_BLOODY_ZENITH,						// 258	
	
	SKILL_REDIANCE,								// 259
	SKILL_LARSLASH,								// 260
	SKILL_HEART_CATALYST,						// 261
	SKILL_ARMS_MASTERY1,						// 262
	SKILL_VIVID_MAGAZINE,						// 263
	SKILL_TRIDENT,								// 264
	SKILL_ARMS_MASTERY2,						// 265
	SKILL_MOLE_SHOT,							// 266

	SKILL_ETERNITY,								// 267
	SKILL_PROTECTION_FROM_BLOOD,				// 268

	SKILL_INSTALL_TRAP,                         // 269
	SKILL_CREATE_HOLY_POTION,                   // 270
	SKILL_MERCY_GROUND,                         // 271
	SKILL_HOLY_ARMOR,                           // 272
	
	SKILL_TRANSFORM_TO_WERWOLF,					// 273
	SKILL_STONE_SKIN,							// 274

	SKILL_ACID_ERUPTION,						// 275
	SKILL_TALON_OF_CROW,						// 276
	SKILL_GRAY_DARKNESS,						// 277	

    SKILL_BITE_OF_DEATH,                        // 278

	SKILL_WIDE_GRAY_DARKNESS,					// 279
    SKILL_TELEPORT,                             // 280

    SKILL_FIRE_PIERCING,                        // 281
    SKILL_SUMMON_OF_FIRE_ELEMENTAL,             // 282
    SKILL_MAGNUM_SPEAR,                         // 283
    SKILL_HELLFIRE,                             // 284

    SKILL_ICE_LANCE,                            // 285
    SKILL_FROZEN_ARMOR,                         // 286
    SKILL_SUMMON_OF_WATER_ELEMENTAL,            // 287
    SKILL_EXPLOSION_WATER,                      // 288
    SKILL_SOUL_REBIRTH,                         // 289
    SKILL_SOUL_REBIRTH_MASTERY,                 // 290

    SKILL_REACTIVE_ARMOR,                       // 291
    SKILL_GROUND_BLESS,                         // 292
    SKILL_SUMMON_OF_GROUND_ELEMENTAL,           // 293
    SKILL_METEOR_STORM,                         // 294

    SKILL_SHARP_CHAKRAM,                        // 295
    SKILL_SHIFT_BREAK,                          // 296
    SKILL_WATER_SHIELD,                         // 297
    SKILL_DESTRUCTION_SPEAR,                    // 298
    SKILL_BLESS_FIRE,                           // 299
    SKILL_FATAL_SNICK,                          // 300
    SKILL_SAND_CROSS,                           // 301
    SKILL_DUCKING_WALLOP,                       // 302
    SKILL_CHARGING_ATTACK,                      // 303
    SKILL_DISTANCE_BLITZ,                       // 304

	SKILL_FABULOUS_SOUL,						//305
	SKILL_WILL_OF_IRON,							//306

 // 질드레 스킬
    SKILL_WIDE_ICE_FIELD,                       // 307
    SKILL_GLACIER_1,                            // 308
    SKILL_GLACIER_2,                            // 309
    SKILL_ICE_AUGER,                            // 310
    SKILL_ICE_HAIL,                             // 311
    SKILL_WIDE_ICE_HAIL,                        // 312
    SKILL_ICE_WAVE,                             // 313

	SKILL_LAND_MINE_EXPLOSION,                  // 314
    SKILL_CLAYMORE_EXPLOSION,                   // 315
    SKILL_PLEASURE_EXPLOSION,                   // 316

	SKILL_DELEO_EFFICIO,                        // 317  // DELETE EFFECT
    SKILL_REPUTO_FACTUM,                        // 318

	SKILL_SWORD_OF_THOR,                        // 319
    SKILL_BURNING_SOUL_CHARGING,                // 320
    SKILL_BURNING_SOUL_LAUNCH,                  // 321
    SKILL_INSTALL_TURRET,                       // 322
    SKILL_TURRET_FIRE,                          // 323
    SKILL_SWEEP_VICE_1,                         // 324
    SKILL_SWEEP_VICE_3,                         // 325
	SKILL_SWEEP_VICE_5,                         // 326
    SKILL_WHITSUNTIDE,                          // 327
    SKILL_VIOLENT_PHANTOM,                      // 328
    SKILL_SUMMON_GORE_GLAND,                    // 329
    SKILL_GORE_GLAND_FIRE,                      // 330
    SKILL_DESTRUCTION_SPEAR_MASTERY,            // 331
    SKILL_FATAL_SNICK_MASTERY,                  // 332
	
	SKILL_MAGNUM_SPEAR_MASTERY,                // 333
    SKILL_ICE_LANCE_MASTERY,                  // 334
    SKILL_REACTIVE_ARMOR_MASTERY,             // 335

	SKILL_THROWING_AXE,                         // 336
    SKILL_CHOPPING_FIREWOOD,                    // 337 장작패기	

	SKILL_CHAIN_THROWING_AXE,                   // 338
    SKILL_MULTI_THROWING_AXE,                   // 339
    SKILL_PLAYING_WITH_FIRE,                    // 340 불장난

	SKILL_INFINITY_THUNDERBOLT,                 // 341
    SKILL_SPIT_STREAM,                          // 342
    SKILL_PLASMA_ROCKET_LAUNCHER,               // 343
    SKILL_INTIMATE_GRAIL,                       // 344
    SKILL_BOMBING_STAR,                         // 345
    SKILL_SET_AFIRE,                            // 346
    SKILL_NOOSE_OF_WRAITH,                      // 347

	//------------------------------------------------------------
	// 여기부터는 client 전용 SKILL ID
	//------------------------------------------------------------
	// 임시로 추가	
	
	MAGIC_BLOODY_TUNNEL_INTO,
//	MAGIC_OPEN_CASKET,
	SUMMON_HELICOPTER,	

	
	// Bomb
	BOMB_SPLINTER,
	BOMB_ACER,
	BOMB_BULLS,	
	BOMB_STUN,
	BOMB_CROSSBOW,
	BOMB_TWISTER,

	// Mine
	MINE_ANKLE_KILLER,
	MINE_POMZ,
	MINE_AP_C1,
	MINE_DIAMONDBACK,
	MINE_SWIFT_EX,
	MINE_SIDEWINDER,
	MINE_COBRA,

	// gun
	SKILL_ATTACK_GUN_SR,			// 총1
	SKILL_ATTACK_GUN_SG,			// 총2
	SKILL_ATTACK_GUN_AR,			// 총3
	SKILL_ATTACK_GUN_SMG,			// 총4

	// sword
	SKILL_ATTACK_SWORD,		// 칼1
	SKILL_ATTACK_BLADE,		// 칼2	

	// 총에 따른 holy shooting
	SKILL_HOLY_SHOOTING_TR,					//	Holy shooting
	SKILL_HOLY_SHOOTING_SG,					//	Holy shooting
	SKILL_HOLY_SHOOTING_AR,					//	Holy shooting
	SKILL_HOLY_SHOOTING_SMG,				//	Holy shooting

	// etc
	GUIDANCE_FIRE,
	FALLING_LIGHTNING,
	ATTACH_FIRE,
	ATTACH_AURA,  

	SKILL_MEGAPHONE,
	SKILL_BLITZ_SLIDING_ATTACK,
	SKILL_BLAZE_WALK_ATTACK,
	SKILL_MUTE,	
	SKILL_ATTACK_MELEE_REDIANCE,
	FAMILY_BONUS,	
	//------------------------------------------------------------------------------
	// 스킬 단계에 따른 스킬
	//------------------------------------------------------------------------------
	STEP_SKILL_FLOURISH_2,							// 2단계 FLOURISH
	STEP_SKILL_FLOURISH_3,							// 3단계 FLOURISH
	STEP_SKILL_SHARP_ROUND_2,						// 2단계 Sharp Round
	STEP_SKILL_SHARP_ROUND_3,						// 3단계 Sharp Round
	STEP_SKILL_GAMMA_CHOP_2,
	STEP_SKILL_GAMMA_CHOP_3,
	STEP_SKILL_CROSS_GUARD_2,
	STEP_SKILL_CROSS_GUARD_3,
	STEP_SKILL_PROMINENCE_2,
	STEP_SKILL_PROMINENCE_3,
	STEP_SKILL_HANDS_OF_NIZE_2,
	STEP_SKILL_HANDS_OF_NIZE_3,
	STEP_SKILL_NYMPH_RECOVERY_2,
	STEP_SKILL_NYMPH_RECOVERY_3,
	STEP_SKILL_EARTHS_TEETH_2,
	STEP_SKILL_EARTHS_TEETH_3,
	STEP_SKILL_REFUSAL_ETHER_2,
	STEP_SKILL_REFUSAL_ETHER_3,
	STEP_SKILL_KASAS_ARROW_2,
	STEP_SKILL_KASAS_ARROW_3,
	STEP_SKILL_BLAZE_BOLT_2,
	STEP_SKILL_BLAZE_BOLT_3,
	STEP_SKILL_STONE_AUGER_2,
	STEP_SKILL_STONE_AUGER_3,

	STEP_SKILL_EMISSION_WATER_2,
	STEP_SKILL_EMISSION_WATER_3,
	STEP_SKILL_BEAT_HEAD_2,
	STEP_SKILL_BEAT_HEAD_3,
	STEP_SKILL_MAGNUM_SPEAR_2,
	STEP_SKILL_MAGNUM_SPEAR_3,
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

	// enchant
	MAGIC_ENCHANT_OPTION_NULL,				// 옵션이 없어진다
	MAGIC_ENCHANT_OPTION_PLUS,				// 옵션이 하나 올라간다
	MAGIC_ENCHANT_REMOVE_ITEM,				// 아이템이 없어진다

	// HolyLandBonus Skills 2003.2.10
	SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA,				// 기술 사용시 MP(HP)소모량 50% 감소  
	SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE,				// 타종족의 언어 통역  
	SKILL_HOLYLAND_BLOOD_BIBLE_KIRO,				// INT+5, DEX +5  
	SKILL_HOLYLAND_BLOOD_BIBLE_INI,					// 물리공격 데미지 +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI,				// 물약값 50% 할인  
	SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA,			// HP +50  
	SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS,				// 마법공격데미지 +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL,				// 행운 +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_JAVE,				// 겜블가격 50% 할인  
	SKILL_HOLYLAND_BLOOD_BIBLE_NEMA,				// 시야 +5, 모든 능력치 +3  
	SKILL_HOLYLAND_BLOOD_BIBLE_AROSA,				// 모든 저항력 +9  
	SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA,				// STR +5, INT +5 

	SKILL_LOVE_CHAIN,								// 러브 체인 스킬	
	
	MAGIC_TRANS_ITEM_OK,	
	SKILL_TRANSLATION,
	
	SKILL_SWEEPER_BONUS_1,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_2,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_3,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_4,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_5,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_6,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_7,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_8,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_9,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_10,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_11,							// 스위퍼 보너스
	SKILL_SWEEPER_BONUS_12,							// 스위퍼 보너스
	
	// 2004, 6, 15 sobeit add start 
	SKILL_CLIENT_CAN_ENTER_GDR_LAIR, //- 질드레 들어갈수 있음
	SKILL_CLIENT_BLINDNESS,			// 다크니스와 같은 효과
	// 2004, 6, 15 sobeit add end - 
	SKILL_CLIENT_DRAGON_EYES,	// 드레곤 아이즈
	
	//-----------------------------------------------------------------
	// RESULT_ACTIONINFO
	//-----------------------------------------------------------------		
	// 기본 기술
	RESULT_SKILL_ATTACK_MELEE,						// 0
	RESULT_SKILL_ATTACK_ARMS,							// 1
	RESULT_SKILL_SELF,									// 2
	RESULT_SKILL_TILE,									// 3
	RESULT_SKILL_OBJECT,	    						// 4

	// 검계열,	
	RESULT_SKILL_DOUBLE_IMPACT,  					  	// 5
	RESULT_SKILL_TRIPLE_SLASHER,				  	  	// 6
	RESULT_SKILL_RAINBOW_SLASHER,					  	// 7
	RESULT_SKILL_THUNDER_SPARK,	   				 	// 8
	RESULT_SKILL_DANCING_SWORD,						// 92
	RESULT_SKILL_CROSS_COUNTER,		   			 	// 10
	RESULT_SKILL_FLASH_SLIDING,		  			  	// 11
	RESULT_SKILL_LIGHTNING_HAND,	  					// 12
	RESULT_SKILL_SNAKE_COMBO,	  	 				 	// 9
	RESULT_SKILL_SWORD_WAVE,		  	 				// 13
	RESULT_SKILL_DRAGON_RISING,		 			    // 14
	RESULT_SKILL_FIVE_STORM_CRASH,	 				 	// 15
	RESULT_SKILL_HEAVENS_SWORD,		  			  	// 16

	// 도 계열,
	RESULT_SKILL_SINGLE_BLOW,						    // 17
	RESULT_SKILL_SPIRAL_SLAY,						    // 18
	RESULT_SKILL_TRIPLE_BREAK,						    // 19
	RESULT_SKILL_WILD_SMASH,						    // 20
	RESULT_SKILL_GHOST_BLADE,							// 93
	RESULT_SKILL_POTENTIAL_EXPLOSION,			  		// 22
	RESULT_SKILL_SHADOW_WALK,			  			  	// 23
	RESULT_SKILL_CHARGING_POWER,		  			  	// 24
	RESULT_SKILL_HURRICANE_COMBO,	 				 	// 21
	RESULT_SKILL_TORNADO_SEVER,		  			  	// 25
	RESULT_SKILL_ARMAGEDDON_SLASHER,	  				// 26
	RESULT_SKILL_SOUL_SHOCK,			   				// 27
	RESULT_SKILL_SAINT_BLADE,			   			  	// 28

	// 건, 라이플 계열
	RESULT_SKILL_FAST_RELOAD,						    // 30
	RESULT_SKILL_QUICK_FIRE,							//
	RESULT_SKILL_SMG_MASTERY,							// 
	RESULT_SKILL_MULTI_SHOT,						    // 94
	RESULT_SKILL_HEAD_SHOT,						    // 94
	RESULT_SKILL_AR_MASTERY,							// 
	RESULT_SKILL_PIERCING,						    	// 97
	RESULT_SKILL_SNIPPING,						    	// 95
	RESULT_SKILL_SG_MASTERY,							//
	RESULT_SKILL_DETECT_MINE,						    // 35
	RESULT_SKILL_MAKE_BOMB,		 				    // 36
	RESULT_SKILL_SR_MASTERY,
	RESULT_SKILL_DISARM_MINE,						    // 37
	RESULT_SKILL_INSTALL_MINE,						    // 37
	RESULT_SKILL_MAKE_MINE,			  			    // 38
    
	// 성직마법,
	// 인첸 계열
	RESULT_MAGIC_CREATE_HOLY_WATER, 			 		// 39
	RESULT_MAGIC_LIGHT,				      			// 40
	RESULT_MAGIC_DETECT_HIDDEN,	   	 				// 41
	RESULT_MAGIC_AURA_BALL,	     		 		 	// 55
	RESULT_MAGIC_BLESS,			       		 		// 45
	RESULT_MAGIC_CONTINUAL_LIGHT,	 			 		// 46
	RESULT_MAGIC_FLARE,								// 얼마일까 다시 정리 해야지
	RESULT_MAGIC_PURIFY,    						// 43
	RESULT_MAGIC_AURA_RING,		  	  		  		// 42
	RESULT_MAGIC_STRIKING,		      					// 44
	RESULT_MAGIC_DETECT_INVISIBILITY,					// 48
	RESULT_MAGIC_IDENTIFY,								//
	RESULT_MAGIC_AURA_SHIELD,	    					// 60
	RESULT_MAGIC_ENCHANT,	    					// 60
	RESULT_MAGIC_VISIBLE,			    			  	// 49
	RESULT_MAGIC_CHAIN_AURA,							// 50
	RESULT_MAGIC_SAINT_AURA,							// 51

	// 힐 계열
	RESULT_MAGIC_CURE_LIGHT_WOUNDS,					// 52
	RESULT_MAGIC_CURE_POISON,							// 54
	RESULT_MAGIC_PROTECTION_FROM_POISON,				// 53
	RESULT_MAGIC_CAUSE_LIGHT_WOUNDS,					// 얼마일까 다시 정리 해야지
	RESULT_MAGIC_CURE_SERIOUS_WOUNDS,					// 56
	RESULT_MAGIC_REMOVE_CURSE,							// 89
	RESULT_MAGIC_PROTECTION_FROM_CURSE,				// 59
	RESULT_MAGIC_CAUSE_SERIOUS_WOUNDS,					// 얼마일까 다시 정리 해야지
	RESULT_MAGIC_CURE_CRITICAL_WOUNDS,					// 
	RESULT_MAGIC_PROTECTION_FROM_ACID,					//
	RESULT_MAGIC_SACRIFICE,							//
	RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,				// 얼마일까 다시 정리 해야지
	RESULT_MAGIC_CURE_ALL,								// 62
	RESULT_MAGIC_REGENERATION,							// 61
	RESULT_MAGIC_MASS_CURE,							//
	RESULT_MAGIC_MASS_HEAL,							//

	// 뱀프기술,
	RESULT_SKILL_BLOOD_DRAIN,							// 65

	// 뱀프마법,
	// Poison
	RESULT_MAGIC_POISONOUS_HANDS,						// 67
	RESULT_MAGIC_GREEN_POISON,							// 69
	RESULT_MAGIC_YELLOW_POISON,						// 70
	RESULT_MAGIC_DARKBLUE_POISON,						// 79
	RESULT_MAGIC_GREEN_STALKER,						// 75

	// Acid
	RESULT_MAGIC_ACID_TOUCH,							// 68
	RESULT_MAGIC_ACID_BOLT,							// 73
	RESULT_MAGIC_ACID_BALL,							// 78
	RESULT_MAGIC_ACID_SWAMP,							// 81

	// Curse
	RESULT_MAGIC_PARALYZE,						// 74
	RESULT_MAGIC_DOOM,							// 74
	RESULT_MAGIC_HALLUCINATION,							//
	RESULT_MAGIC_DEATH,							//

	// Bloody
	RESULT_MAGIC_BLOODY_NAIL,							// 82
	RESULT_MAGIC_BLOODY_KNIFE,							// 83
	RESULT_MAGIC_BLOODY_BALL,							// 84
	RESULT_MAGIC_BLOODY_WALL,							// 85
	RESULT_MAGIC_BLOODY_SPEAR,							// 86

	// Innate
	RESULT_MAGIC_HIDE,									// 66
	RESULT_MAGIC_DARKNESS,								// 71
//	RESULT_MAGIC_PURPLE_WALL,							// 72
	RESULT_MAGIC_INVISIBILITY,							// 76
	RESULT_MAGIC_TRANSFORM_TO_WOLF,					// 77
	RESULT_MAGIC_TRANSFORM_TO_BAT,						// 80

	// 써몬
	RESULT_MAGIC_SUMMON_WOLF,							// 87
	RESULT_MAGIC_SUMMON_CASKET,						// 88
	RESULT_MAGIC_RAISING_DEAD,							// 88
	RESULT_MAGIC_SUMMON_SERVANT,						//

	// ETC
	RESULT_MAGIC_UN_BURROW,							// 89
	RESULT_MAGIC_UN_TRANSFORM,							// 89
	RESULT_MAGIC_UN_INVISIBILITY,
	RESULT_MAGIC_THROW_HOLY_WATER,						// 91

	// 늑대 기술
	RESULT_MAGIC_EAT_CORPSE,
	RESULT_MAGIC_HOWL,

	RESULT_MAGIC_RESTORE,								// 90

	RESULT_MAGIC_BLOODY_MARK,
	RESULT_MAGIC_BLOODY_TUNNEL,
	
	RESULT_MAGIC_SEDUCTION,
	RESULT_SKILL_WIND_DIVIDER,
	RESULT_SKILL_EARTHQUAKE,

	RESULT_MAGIC_RESURRECT,
	RESULT_MAGIC_PRAYER,
	RESULT_MAGIC_MEDITATION,

	// 2001.12.4 추가
	RESULT_SKILL_THUNDER_BOLT,							// 122
	RESULT_SKILL_EXPANSION, 							// 123
	RESULT_SKILL_MIRACLE_SHIELD, 						// 124
	RESULT_SKILL_THUNDER_FLASH,						// 125
	RESULT_SKILL_MASSACRE,								// 126
	RESULT_SKILL_INVINCIBLE,							// 127

	RESULT_SKILL_BERSERKER,							// 128
	RESULT_SKILL_MOONLIGHT_SEVER,						// 129
	RESULT_SKILL_SHADOW_DANCING,						// 130
	RESULT_SKILL_TYPHOON,								// 131
	RESULT_SKILL_PSYCHOKINESIS,						// 132
	RESULT_SKILL_EXTERMINATION,						// 133

	RESULT_SKILL_MIND_CONTROL,							// 134
	RESULT_SKILL_REVOLVING,							// 135
	RESULT_SKILL_FATALITY,								// 136
	RESULT_SKILL_BLITZ,								// 137

	RESULT_SKILL_ACTIVATION,							// 138
	RESULT_SKILL_PEACE,								// 139
	RESULT_SKILL_ENERGY_DROP,							// 140
	RESULT_SKILL_EXORCISM,								// 141

	RESULT_SKILL_SANCTUARY,							// 142
	RESULT_SKILL_REFLECTION,							// 143
	RESULT_SKILL_ARMAGEDDON,							// 144

	RESULT_SKILL_POISON_STRIKE,						// 145
	RESULT_SKILL_POISON_STORM,							// 146
	RESULT_SKILL_ACID_STRIKE,							// 147
	RESULT_SKILL_ACID_STORM,							// 148
	RESULT_SKILL_BLOODY_STRIKE,						// 149
	RESULT_SKILL_BLOODY_STORM,							// 150

	// 추가 2002.3.23
	RESULT_SKILL_SUMMON_BAT,
	RESULT_SKILL_CHARM,
	RESULT_SKILL_POLYMORPH,
	RESULT_SKILL_MEPHISTO,
	RESULT_SKILL_HYPNOSIS,
	RESULT_SKILL_TRANSFUSION,
	RESULT_SKILL_EXTREME,
	RESULT_SKILL_BLOODY_WAVE,	
	RESULT_SKILL_THROW_BOMB,

	RESULT_SKILL_DOUBLE_SHOT,
	RESULT_SKILL_TRIPLE_SHOT,

	RESULT_SKILL_CURE_EFFECT,			// cure
	RESULT_SKILL_CRITICAL_EFFECT,
	RESULT_SKILL_CRITICAL_GROUND,		// critical을 사용하는 경우	

	RESULT_SKILL_VIGOR_DROP,

	// [새기술]
	RESULT_SKILL_SWORD_MASTERY,                        // 166
    RESULT_SKILL_SHIELD_MASTERY,                       // 167
    RESULT_SKILL_THUNDER_STORM,                        // 168
    RESULT_SKILL_CONCENTRATION,                        // 169
    RESULT_SKILL_EVASION,                              // 170
    RESULT_SKILL_HOLY_BLAST,                           // 171
    RESULT_SKILL_HYMN,                                 // 172
	RESULT_SKILL_MENTAL_SWORD,							// 173
	RESULT_SKILL_OBSERVING_EYE,						// 174
	RESULT_SKILL_REFLECTION_EFFECT,					// 175

	// 2002.6.21
	RESULT_SKILL_TEMP,									// 176
	RESULT_MAGIC_OPEN_CASKET,							// 177

	// 2002.9.2
	RESULT_MAGIC_SUMMON_MONSTERS,						// 178

	RESULT_MAGIC_GROUND_ATTACK,						// 179
	RESULT_MAGIC_METEOR,								// 180

	RESULT_MAGIC_DUPLICATE_SELF,						// 181

	RESULT_MAGIC_BLOODY_MASTER_WAVE,						// 182

	// 2002.10.1
    RESULT_MAGIC_BLOODY_WARP,                          // 183
    RESULT_MAGIC_BLOODY_SNAKE,                         // 184

	RESULT_SKILL_SOUL_CHAIN,							//185

	// 2002.11.18
	RESULT_SKILL_LIVENESS,								// 186

	// 2002.11.20
	RESULT_MAGIC_DARKNESS_WIDE,								// 187
	RESULT_SKILL_POISON_STORM_WIDE,							// 188
	RESULT_SKILL_ACID_STORM_WIDE,							// 189

    // 2002.12.26
    RESULT_SKILL_SHARP_SHIELD,                         // 190
    RESULT_SKILL_WIDE_LIGHTNING,                       // 191
    RESULT_SKILL_AIR_SHIELD,                           // 192
    RESULT_SKILL_POWER_OF_LAND,                        // 193
    RESULT_SKILL_BULLET_OF_LIGHT,                      // 194
    RESULT_SKILL_GUN_SHOT_GUIDANCE,                    // 195
    RESULT_SKILL_REBUKE,                               // 196
    RESULT_SKILL_SPIRIT_GUARD,                         // 197
    RESULT_SKILL_TURN_UNDEAD,                          // 198

	RESULT_SKILL_HANDS_OF_WISDOM,						// 199
	RESULT_SKILL_LIGHT_BALL,							// 200
	RESULT_SKILL_HOLY_ARROW,							// 201

	RESULT_MAGIC_BLOODY_BREAKER,						// 202
	RESULT_MAGIC_RAPID_GLIDING,							// 203

	RESULT_SKILL_MAGIC_ELUSION,							// 204
	RESULT_SKILL_POISON_MESH,							// 205
	RESULT_SKILL_ILLUSION_OF_AVENGE,					// 206
	RESULT_SKILL_WILL_OF_LIFE,							// 207

	RESULT_SKILL_DENIAL_MAGIC,							// 208
	RESULT_SKILL_REQUITAL,								// 209
	RESULT_SKILL_CONCEALMENT,							// 210
	RESULT_SKILL_SWORD_RAY,								// 211
	RESULT_SKILL_MULTI_AMPUTATE,						// 212
	RESULT_SKILL_NAIL_MASTERY,							// 213	
	
	RESULT_SKILL_HIT_CONVERT,							// 214
	RESULT_SKILL_WILD_TYPHOON,							// 215
	RESULT_SKILL_ULTIMATE_BLOW,							// 216
	RESULT_SKILL_ILLENDUE,								// 217
	RESULT_SKILL_LIGHTNESS,								// 218

	RESULT_SKILL_FLOURISH,                             // 219
    RESULT_SKILL_EVADE,                                // 220
    RESULT_SKILL_SHARP_ROUND,                          // 221
    RESULT_SKILL_HIDE_SIGHT,                           // 222
    RESULT_SKILL_BACK_STAB,                            // 223
    RESULT_SKILL_BLUNTING,                             // 224
    RESULT_SKILL_GAMMA_CHOP,                           // 225

    RESULT_SKILL_CROSS_GUARD,                          // 226
    RESULT_SKILL_FIRE_OF_SOUL_STONE,                   // 227
    RESULT_SKILL_ICE_OF_SOUL_STONE,                    // 228
    RESULT_SKILL_SAND_OF_SOUL_STONE,                   // 229
    RESULT_SKILL_BLOCK_HEAD,                           // 230
    RESULT_SKILL_KASAS_ARROW,                          // 231
    RESULT_SKILL_HANDS_OF_FIRE,                        // 232

    RESULT_SKILL_PROMINENCE,                           // 233
    RESULT_SKILL_RING_OF_FLARE,                        // 234
    RESULT_SKILL_BLAZE_BOLT,                           // 235
    RESULT_SKILL_ICE_FIELD,                            // 236
    RESULT_SKILL_WATER_BARRIER,                        // 237
    RESULT_SKILL_HANDS_OF_NIZIE,                       // 238
    RESULT_SKILL_NYMPH_RECOVERY,                       // 239

    RESULT_SKILL_LIBERTY,                              // 240
    RESULT_SKILL_TENDRIL,                              // 241
    RESULT_SKILL_GNOMES_WHISPER,                       // 242
    RESULT_SKILL_STONE_AUGER,                          // 243
    RESULT_SKILL_REFUSAL_ETHER,                        // 244
    RESULT_SKILL_EARTHS_TEETH,                         // 245
    RESULT_SKILL_ABSORB_SOUL,                          // 246

    RESULT_SKILL_SUMMON_SYLPH,                         // 247
    RESULT_SKILL_DRIFTING_SOUL,                        // 248
    RESULT_SKILL_CRITICAL_MAGIC,                       // 249

	RESULT_SKILL_EMISSION_WATER,						// 250
	RESULT_SKILL_BEAT_HEAD,								// 251
	RESULT_SKILL_DIVINE_SPIRITS,						// 252

	RESULT_SKILL_BLITZ_SLIDING,							// 253
    RESULT_SKILL_BLAZE_WALK,							// 254
    RESULT_SKILL_JABBING_VEIN,							// 255
    RESULT_SKILL_GREAT_HEAL,							// 256
    RESULT_SKILL_DIVINE_GUIDANCE,						// 257

	RESULT_MAGIC_BLOODY_ZENITH,							// 258
	
	RESULT_SKILL_REDIANCE,								// 259
	RESULT_SKILL_LARSLASH,								// 260
	RESULT_SKILL_HEART_CATALYST,						// 261
	RESULT_SKILL_ARMS_MASTERY1,							// 262
	RESULT_SKILL_VIVID_MAGAZINE,						// 263
	RESULT_SKILL_TRIDENT,								// 264
	RESULT_SKILL_ARMS_MASTERY2,							// 265
	RESULT_SKILL_MOLE_SHOT,								// 266

	RESULT_SKILL_ETERNITY,								// 267
	RESULT_SKILL_PROTECTION_FROM_BLOOD,					// 268

	RESULT_SKILL_INSTALL_TRAP,							// 269
	RESULT_SKILL_CREATE_HOLY_POTION,					// 270
	RESULT_SKILL_MERCY_GROUND,							// 271
	RESULT_SKILL_HOLY_ARMOR,							// 272
	RESULT_SKILL_TRANSFORM_TO_WERWOLF,					// 273
	RESULT_SKILL_STONE_SKIN,							// 274

	RESULT_SKILL_ACID_ERUPTION,							// 275
	RESULT_SKILL_TALON_OF_CROW,							// 276
	RESULT_SKILL_GRAY_DARKNESS,							// 277	

	RESULT_SKILL_BITE_OF_DEATH,							// 278
	RESULT_SKILL_WIDE_GRAY_DARKNESS,					// 279
    RESULT_SKILL_TELEPORT,                             // 280

    RESULT_SKILL_FIRE_PIERCING,                        // 281
    RESULT_SKILL_SUMMON_OF_FIRE_ELEMENTAL,             // 282
    RESULT_SKILL_MAGNUM_SPEAR,                         // 283
    RESULT_SKILL_HELLFIRE,                             // 284

    RESULT_SKILL_ICE_LANCE,                            // 285
    RESULT_SKILL_FROZEN_ARMOR,                         // 286
    RESULT_SKILL_SUMMON_OF_WATER_ELEMENTAL,            // 287
    RESULT_SKILL_EXPLOSION_WATER,                      // 288
    RESULT_SKILL_SOUL_REBIRTH,                         // 289
    RESULT_SKILL_SOUL_REBIRTH_MASTERY,                 // 290

    RESULT_SKILL_REACTIVE_ARMOR,                       // 291
    RESULT_SKILL_GROUND_BLESS,                         // 292
    RESULT_SKILL_SUMMON_OF_GROUND_ELEMENTAL,           // 293
    RESULT_SKILL_METEOR_STORM,                         // 294

    RESULT_SKILL_SHARP_CHAKRAM,                        // 295
    RESULT_SKILL_SHIFT_BREAK,                          // 296
    RESULT_SKILL_WATER_SHIELD,                         // 297
    RESULT_SKILL_DESTRUCTION_SPEAR,                    // 298
    RESULT_SKILL_BLESS_FIRE,                           // 299
    RESULT_SKILL_FATAL_SNICK,                          // 300
    RESULT_SKILL_SAND_CROSS,                           // 301
    RESULT_SKILL_DUCKING_WALLOP,                       // 302
    RESULT_SKILL_CHARGING_ATTACK,                      // 303
    RESULT_SKILL_DISTANCE_BLITZ,                       // 304
	
	RESULT_SKILL_FABULOUS_SOUL,						//305
	RESULT_SKILL_WILL_OF_IRON,							//306
	// 질드레 스킬
    RESULT_SKILL_WIDE_ICE_FIELD,                       // 307
    RESULT_SKILL_GLACIER_1,                            // 308
    RESULT_SKILL_GLACIER_2,                            // 309
    RESULT_SKILL_ICE_AUGER,                            // 310
    RESULT_SKILL_ICE_HAIL,                             // 311
    RESULT_SKILL_WIDE_ICE_HAIL,                        // 312
    RESULT_SKILL_ICE_WAVE,                             // 313

	RESULT_SKILL_LAND_MINE_EXPLOSION,                  // 314
    RESULT_SKILL_CLAYMORE_EXPLOSION,                   // 315
    RESULT_SKILL_PLEASURE_EXPLOSION,                   // 316

	RESULT_SKILL_DELEO_EFFICIO,                        // 317  // DELETE EFFECT
    RESULT_SKILL_REPUTO_FACTUM,                        // 318

	RESULT_SKILL_SWORD_OF_THOR,                        // 319
    RESULT_SKILL_BURNING_SOUL_CHARGING,                // 320
    RESULT_SKILL_BURNING_SOUL_LAUNCH,                  // 321
    RESULT_SKILL_INSTALL_TURRET,                       // 322
    RESULT_SKILL_TURRET_FIRE,                          // 323
    RESULT_SKILL_SWEEP_VICE_1,                         // 324
    RESULT_SKILL_SWEEP_VICE_3,                         // 325
	RESULT_SKILL_SWEEP_VICE_5,                         // 326
    RESULT_SKILL_WHITSUNTIDE,                          // 327
    RESULT_SKILL_VIOLENT_PHANTOM,                      // 328
    RESULT_SKILL_SUMMON_GORE_GLAND,                    // 329
    RESULT_SKILL_GORE_GLAND_FIRE,                      // 330
    RESULT_SKILL_DESTRUCTION_SPEAR_MASTERY,            // 331
    RESULT_SKILL_FATAL_SNICK_MASTERY,                  // 332
	
	RESULT_SKILL_MAGNUM_SPEAR_MASTERY,                // 333
    RESULT_SKILL_ICE_LANCE_MASTERY,                  // 334
    RESULT_SKILL_REACTIVE_ARMOR_MASTERY,             // 335

	RESULT_SKILL_THROWING_AXE,                         // 336
    RESULT_SKILL_CHOPPING_FIREWOOD,                    // 337 장작패기	

	RESULT_SKILL_CHAIN_THROWING_AXE,                   // 338
    RESULT_SKILL_MULTI_THROWING_AXE,                   // 339
    RESULT_SKILL_PLAYING_WITH_FIRE,                    // 340 불장난

	RESULT_SKILL_INFINITY_THUNDERBOLT,                 // 341
    RESULT_SKILL_SPIT_STREAM,                          // 342
    RESULT_SKILL_PLASMA_ROCKET_LAUNCHER,               // 343
    RESULT_SKILL_INTIMATE_GRAIL,                       // 344
    RESULT_SKILL_BOMBING_STAR,                         // 345
    RESULT_SKILL_SET_AFIRE,                            // 346
    RESULT_SKILL_NOOSE_OF_WRAITH,                      // 347

	
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	
	// 임시로 추가
	RESULT_MAGIC_BLOODY_TUNNEL_INTO,
//	RESULT_MAGIC_OPEN_CASKET,
	RESULT_SUMMON_HELICOPTER,		
	
	// Bomb
	RESULT_BOMB_SPLINTER,
	RESULT_BOMB_ACER,
	RESULT_BOMB_BULLS,	
	RESULT_BOMB_STUN,
	RESULT_BOMB_CROSSBOW,
	RESULT_BOMB_TWISTER,

	// Mine
	RESULT_MINE_ANKLE_KILLER,
	RESULT_MINE_POMZ,
	RESULT_MINE_AP_C1,
	RESULT_MINE_DIAMONDBACK,
	RESULT_MINE_SWIFT_EX,
	RESULT_MINE_SIDEWINDER,
	RESULT_MINE_COBRA,

	// Gun
	RESULT_SKILL_ATTACK_GUN_SR,			// 총1
	RESULT_SKILL_ATTACK_GUN_SG,			// 총2
	RESULT_SKILL_ATTACK_GUN_AR,			// 총3
	RESULT_SKILL_ATTACK_GUN_SMG,		// 총4
	
	// Sword
	RESULT_SKILL_ATTACK_SWORD,			// 칼
	RESULT_SKILL_ATTACK_BLADE,			// 도

	// 총에 따른 holy shooting
	RESULT_SKILL_HOLY_SHOOTING_TR,					//	Holy shooting
	RESULT_SKILL_HOLY_SHOOTING_SG,					//	Holy shooting
	RESULT_SKILL_HOLY_SHOOTING_AR,					//	Holy shooting
	RESULT_SKILL_HOLY_SHOOTING_SMG,				//	Holy shooting

	// etc
	RESULT_GUIDANCE_FIRE,
	RESULT_FALLING_LIGHTNING,
	RESULT_ATTACH_FIRE,
	RESULT_ATTACH_AURA,	
	RESULT_SKILL_MEGAPHONE,
	RESULT_SKILL_BLITZ_SLIDING_ATTACK,
	RESULT_SKILL_BLAZE_WALK_ATTACK,
	RESULT_SKILL_MUTE,
	RESULT_SKILL_ATTACK_MELEE_REDIANCE,
	RESULT_FAMILY_BONUS,
	
	//-----------------------------------------------------------------------------
	// 스킬 단계
	//-----------------------------------------------------------------------------
	RESULT_STEP_SKILL_FLOURISH_2,							// 2단계 FLOURISH
	RESULT_STEP_SKILL_FLOURISH_3,							// 3단계 FLOURISH
	RESULT_STEP_SKILL_SHARP_ROUND_2,						// 2단계 Sharp Round
	RESULT_STEP_SKILL_SHARP_ROUND_3,						// 3단계 Sharp Round
	RESULT_STEP_SKILL_GAMMA_CHOP_2,
	RESULT_STEP_SKILL_GAMMA_CHOP_3,
	RESULT_STEP_SKILL_CROSS_GUARD_2,
	RESULT_STEP_SKILL_CROSS_GUARD_3,
	RESULT_STEP_SKILL_PROMINENCE_2,
	RESULT_STEP_SKILL_PROMINENCE_3,
	RESULT_STEP_SKILL_HANDS_OF_NIZE_2,
	RESULT_STEP_SKILL_HANDS_OF_NIZE_3,
	RESULT_STEP_SKILL_NYMPH_RECOVERY_2,
	RESULT_STEP_SKILL_NYMPH_RECOVERY_3,
	RESULT_STEP_SKILL_EARTHS_TEETH_2,
	RESULT_STEP_SKILL_EARTHS_TEETH_3,
	RESULT_STEP_SKILL_REFUSAL_ETHER_2,
	RESULT_STEP_SKILL_REFUSAL_ETHER_3,
	RESULT_STEP_SKILL_KASAS_ARROW_2,
	RESULT_STEP_SKILL_KASAS_ARROW_3,
	RESULT_STEP_SKILL_BLAZE_BOLT_2,
	RESULT_STEP_SKILL_BLAZE_BOLT_3,
	RESULT_STEP_SKILL_STONE_AUGER_2,
	RESULT_STEP_SKILL_STONE_AUGER_3,
	RESULT_STEP_SKILL_EMISSION_WATER_2,
	RESULT_STEP_SKILL_EMISSION_WATER_3,
	RESULT_STEP_SKILL_BEAT_HEAD_2,
	RESULT_STEP_SKILL_BEAT_HEAD_3,
	RESULT_STEP_SKILL_MAGNUM_SPEAR_2,
	RESULT_STEP_SKILL_MAGNUM_SPEAR_3,
	//-----------------------------------------------------------------------------

	// client 전용
	RESULT_SLAYER_DIE,
	RESULT_VAMPIRE_DIE,
	RESULT_OUSTERS_DIE,
	RESULT_MASTER_DIE,
	RESULT_VAMPIRE_DIE_NO_EFFECT,
	RESULT_FAKE_DIE,
	RESULT_GROUND_ELEMENTAL_DIE,

	// casting ActionInfo
	CASTING_ENCHANT,					// enchant domain casting
	CASTING_HEALING,					// healing domain casting
	CASTING_FIRE,
	CASTING_GROUND,
	CASTING_WATER,
	CASTING_FIRE2,
	CASTING_GROUND2,
	CASTING_WATER2,

	// 방어막 효과를 내기 위해서..
	REFLECT_AURA_PRISM,				//	
	REFLECT_AURA_SHIELD,				//	
	PROTECTION_FROM_ACID,
	REFLECT_REFLECTION,				//	SKILL_REFLECTION에 의한것

	// 바닥에 흘리는 피
	BLOOD_DROP_GROUND,
	BLOOD_CRITICAL_HIT,	
	BLOOD_RESURRECT,
	
	// 아마게돈 2002.3.4
	ARMAGEDDON_CRASH_1,			// 첫번째 부서지는거
	ARMAGEDDON_CRASH_2,			// 두번째
	ARMAGEDDON_CRASH_3,			// 세번째
	ARMAGEDDON_CRASH_4,			// 마지막 남기
	
	// bloody_wall_single
	RESULT_MAGIC_BLOODY_WALL_SINGLE,

	// 2002.12.09
    SKILL_FIRE_CRACKER_1,
    SKILL_FIRE_CRACKER_2,
    SKILL_FIRE_CRACKER_3,

    RESULT_SKILL_SHARP_SHIELD_1,
    RESULT_SKILL_SHARP_SHIELD_2,
    RESULT_SKILL_SHARP_SHIELD_3,
    RESULT_SKILL_SHARP_SHIELD_4,

	RESULT_SKILL_AIR_SHIELD_1,
	RESULT_SKILL_AIR_SHIELD_2,
	RESULT_SKILL_AIR_SHIELD_3,
	RESULT_SKILL_AIR_SHIELD_4,

	RESULT_SKILL_SPIRIT_GUARD_1,
	RESULT_SKILL_SPIRIT_GUARD_2,
	RESULT_SKILL_SPIRIT_GUARD_3,
	RESULT_SKILL_SPIRIT_GUARD_4,
	
    RESULT_SKILL_GUN_SHOT_GUIDANCE_BOMB,
    RESULT_SKILL_GUN_SHOT_GUIDANCE_POINT,

	SHRINE_SHIELD,
	DROP_BLOOD_BIBLE,

	ARMEGA_TILE,					// 아르메가
	MIHOLE_TILE,					// 미호레
	KIRO_TILE,						// 키로
	INI_TILE,						// 아이니
	GREGORI_TILE,					// 그리고리 이펙트(타일에 붙는것)
	CONCILIA_TILE,					// 콘칠리아
	LEGIOS_TILE,					// 레지오스
	HILLEL_TILE,					// 힐릴
	JAVE_TILE,						// 쟈브
	NEMA_TILE,						// 네마
	AROSA_TILE,						// 아로사
	CHASPA_TILE,					// 차스파

	HOLY_LAND_WARP_VAMPIRE,
	HOLY_LAND_WARP_SLAYER,

	WARP_BLOOD_BIBLE_TILE,

	AMBER_OF_GUARD_TILE,
	AMBER_OF_AVENGER_TILE,
	AMBER_OF_IMMORTAL_TILE,
	AMBER_OF_CURSE_TILE,

	SKILL_DENIAL_MAGIC_DAMAGED,
	SKILL_DRAGON_FIRE_CRACKER,

	SKILL_FIRE_CRACKER_4,			// 추가
	
	SKILL_FIRE_CRACKER_VOLLEY_1,		// 3연발 폭죽-_-
	SKILL_FIRE_CRACKER_VOLLEY_2,		// 3연발 폭죽-_-
	SKILL_FIRE_CRACKER_VOLLEY_3,		// 3연발 폭죽-_-
	SKILL_FIRE_CRACKER_VOLLEY_4,		// 3연발 폭죽-_-
	
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_1,	// 3연발 광역 폭죽
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_2,	// 3연발 광역 폭죽
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_3,	// 3연발 광역 폭죽
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_4,	// 3연발 광역 폭죽

	SKILL_FIRE_CRACKER_STORM,
	SAND_OF_SOUL_STONE,
	ICE_OF_SOUL_STONE,
	BACK_STAB_GRADE_3,

	BLESS_FIRE,
	WATER_SHIELD,
	SAND_CROSS,
	
	HOLY_LAND_WARP_OUSTERS,

	ETERNITY,
	TRAP_INSTALLED,
	TRAP_TRIGGERED,
	TRAPPED,
	
	REGEN_TOWER_TRING_POSITION,			// 리젠존 타워 쟁탈 타일
	REGEN_TOWER_THROW_LIGHT,			// 리젠존 빛 비치는거 
	REGEN_TOWER_TRING_POSITION_LIGHT,	// 리젠존 타워 쟁탈 타일에 올랐을때
	REGEN_TOWER_FLASH_BEAD,				// 리젠존 타워 구슬 반짝거리는거.

	SKILL_CLIENT_TELEPORT,
	SKILL_CLIENT_TELEPORT2,
	SKILL_CLIENT_DUCKING_WALLOP,
	SKILL_CLIENT_CHARGING_ATTACK,

	SKILL_CLIENT_SUMMON_GROUND_ELEMENTAL,
	SKILL_CLIENT_FIRE_ELEMENTAL_ATTACK,
	SKILL_CLIENT_WATER_ELEMENTAL_HEAL,
	
	SKILL_CLIENT_TANK_SMOKE,
	SKILL_CLIENT_TANK_ATTACK_3,
	SKILL_CLIENT_TANK_ATTACK_4,
	SKILL_CLIENT_TANK_ATTACK_5,
	SKILL_CLIENT_TANK_ATTACKED,


	// 2004,3,6 sobeit add start - 펫 스킬
	SKILL_CLIENT_STIRGE_BLOOD_DRAIN_1,
	SKILL_CLIENT_STIRGE_BLOOD_DRAIN_2,
	SKILL_CLIENT_STIRGE_BLOOD_DRAIN_3,
	SKILL_CLIENT_STIRGE_BLOOD_DRAIN_4,
	SKILL_CLIENT_STIRGE_BLOOD_DRAIN_5,

	SKILL_CLIENT_PIXIE_ABSORB_SOUL_1,
	SKILL_CLIENT_PIXIE_ABSORB_SOUL_2,
	SKILL_CLIENT_PIXIE_ABSORB_SOUL_3,
	SKILL_CLIENT_PIXIE_ABSORB_SOUL_4,
	SKILL_CLIENT_PIXIE_ABSORB_SOUL_5,
	// 2004,3,6 sobeit add end - 펫 스킬

	// 2004,3,22 sobeit add start - 검은 기운
	MAP_BLACK_LARGE_SMOKE,
	MAP_BLACK_HALL,
	MAP_BLACK_SMALL_SMOKE_1,
	MAP_BLACK_SMALL_SMOKE_2,
	MAP_BLACK_SMALL_SMOKE_3,

	MAP_ELECTRIC_POST,
	MAP_GHOST_1,
	MAP_GHOST_2,
	// 2004,3,22 sobeit add end - 검은 기운
	
	// 2004, 3, 30 sobeit add start - 울버린 나올때 이펙트
	RESULT_SUMMON_ULBERINE,
	// 2004, 3, 30 sobeit add end - 울버린 나올때 이펙트

	// 2004, 4,2  sobeit add start - 행운의 오로라(네잎 클로바)
	RESULT_LUCKY_AURA,
	RESULT_MISFORTUNE_AURA,
	// 2004, 4,2 sobeit add end - 행운의 오로라(네잎 클로바)

	// 2004, 5, 6, sobeit add start
	AMBER_OF_LIFE_TILE,
	AMBER_OF_LIGHT_TILE,
	// 2004, 5, 6, sobeit add end

	// 2004, 5, 18 sobeit add start
	SKILL_CLIENT_TRAP_ICICLE_DROP,					// 떨어지는 얼음 트랩
	SKILL_CLIENT_TRAP_ICICLE_DROP_SIGN,

	SKILL_CLIENT_TRAP_ICICLE_AUGER,
	SKILL_CLIENT_TRAP_ICICLE_AUGER_SIGN,

	SKILL_CLIENT_TRAP_ICE_STICK_SW,		// 좌하
	SKILL_CLIENT_TRAP_ICE_STICK_SW_SIGN,// 좌하
	SKILL_CLIENT_TRAP_ICE_STICK_SE,		// 우하
	SKILL_CLIENT_TRAP_ICE_STICK_SE_SIGN,// 우하
	SKILL_CLIENT_TRAP_ICE_STICK_NE,		// 우상
	SKILL_CLIENT_TRAP_ICE_STICK_NE_SIGN,// 우상
	SKILL_CLIENT_TRAP_ICE_STICK_NW,		// 좌상
	SKILL_CLIENT_TRAP_ICE_STICK_NW_SIGN,// 좌상

	SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER,
	SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER_SIGN,
	// 2004, 5, 18 sobeit add start
	

	// 2004, 6, 8 sobeit add start - 질드레 주변 이펙트
//	SKILL_CLIENT_SUMMON_GDR, 
//    SKILL_CLIENT_GDR_SATELLITE,
//    SKILL_CLIENT_GDR_FLOATING, 
//
//	SKILL_CLIENT_SLAYER_MASTER_130,
//	SKILL_CLIENT_SLAYER_MASTER_150,
//	SKILL_CLIENT_VAMPIRE_MASTER_130,
//	SKILL_CLIENT_VAMPIRE_MASTER_150, // 150 레벨 마스터 이펙트
//	SKILL_CLIENT_OUSTERS_MASTER_130,
//	SKILL_CLIENT_OUSTERS_MASTER_150,
	// 2004, 6, 8 sobeit add end

	// 2004, 6, 15 sobeit add start - gdr skill - glacier
	SKILL_CLIENT_GDR_GLACIER_SLAYER,
	SKILL_CLIENT_GDR_GLACIER_VAMPIRE,
	SKILL_CLIENT_GDR_GLACIER_OUSTERS,
	// 2004, 6, 15 sobeit add end - gdr skill - glacier

	// 2004, 6, 26 sobeit add start
	MAP_GDR_LAIR_POTAL,	// 질드레 포탈 이펙트
	// 2004, 6, 26 sobeit add end

	// 2004, 8, 5 sobeit add start
	SKILL_CLIENT_CASTLE_GATE_DEAD, // 성문 부서질때(죽을때) 큰 먼지이펙트
	SKILL_CLIENT_CASTLE_GATE_DAMAGED,// 성문 타격 받을 때
	SKILL_CLIENT_GUILD_TOWER_DEAD, // 길드타워 부서질때(죽을때) 큰 먼지이펙트
	SKILL_CLIENT_GUILD_TOWER_PROTECT, // 길드타워 보호 이펙트
	// 2004, 8, 5 sobeit add end

	// 2004, 8, 31, sobeit add start
	SKILL_CLIENT_LAND_MINE_LIGHT,		// 랜드마인에 계속 붙어 있는 이펙트
	SKILL_CLIENT_PLEASUER_STATION_LIGHT,// 프레져 스테이션에 계속 붙어 있는 이펙트
	// 2004, 8, 31, sobeit add end

	// 2004, 9, 2, sobeit add start
	SKILL_CLIENT_TURRET_LASER,					// 터렛 레이저 1타일
	SKILL_CLIENT_TURRET_LASER_ATTACK,           // 터렛 레이저 공격 1타일 (공격측에도 보임) 
	// 2004, 9, 2, sobeit add end

	// 2004, 9, 6, sobeit add start - 질드레 죽기 이펙트 , 리푸토 펙텀 타격
	SKILL_CLIENT_GDR_DEAD,
	SKILL_CLIENT_HIT_REPUTO_FACTUM,
	// 2004, 9, 6, sobeit add end - 질드레 죽기 이펙트

	SKILL_CLIENT_SWEEP_VICE,// 인챈 스윕 바이스 1단계
	SKILL_CLIENT_SWORD_OF_THOR, // 소드 오브 토르 지속 되는거

	SKILL_CLIENT_BURNING_SOL_1, // 도슬 버닝솔 1단계
	SKILL_CLIENT_BURNING_SOL_2, // 도슬 버닝솔 2단계
	SKILL_CLIENT_BURNING_SOL_3, // 도슬 버닝솔 3단계
	SKILL_CLIENT_BURNING_SOL_4, // 도슬 버닝솔 4단계
	RESULT_CLIENT_BURNING_SOL_1, // 도슬 버닝솔 1단계
	RESULT_CLIENT_BURNING_SOL_2, // 도슬 버닝솔 2단계
	RESULT_CLIENT_BURNING_SOL_3, // 도슬 버닝솔 3단계
	RESULT_CLIENT_BURNING_SOL_4, // 도슬 버닝솔 4단계
	
	// 2004, 10, 21, sobeit add start - 그레이트 러피언 관련
	SKILL_CLIENT_GREAT_RUFFIAN_ALIVE, // 2차 생성될때 이펙트
	SKILL_CLIENT_GREAT_RUFFIAN_BODY, // 몸에 붙는 이펙트
	SKILL_CLIENT_GREAT_RUFFIAN_DIE, // 죽을때 나오는 이펙트
	// 2004, 10, 21, sobeit add end

	// 2004, 11, 11, sobeit add start - 총 140 터지는거
	SKILL_CLIENT_PLASMA_ROCKET_LAUNCHER_BLOW,
	// 2004, 11, 11, sobeit add end - 


	// 2004, 10, 15, sobeit add start - 테스트용
	TEMP_SKILL_INFINITY_THUNDERBLOT, // 검슬 140
	TEMP_SKILL_SPIT_STREAM, // 도슬 140
	TEMP_SKILL_PLASMA_ROCKET_LAUNCHER, //총 140
	TEMP_SKILL_INTIMATE_GRAIL, // 인첸 140
	TEMP_SKILL_BOMBING_STAR, // 힐 140,
	TEMP_SKILL_SET_AFIRE, // 전투뱀 140
	TEMP_SKILL_NOOSE_OF_WRAITH, // 마법뱀 140

	RESULT_TEMP_SKILL_INFINITY_THUNDERBLOT, // 검슬 140
	RESULT_TEMP_SKILL_SPIT_STREAM, // 도슬 140
	RESULT_TEMP_SKILL_PLASMA_ROCKET_LAUNCHER, //총 140
	RESULT_TEMP_SKILL_INTIMATE_GRAIL, // 인첸 140
	RESULT_TEMP_SKILL_BOMBING_STAR, // 힐 140,
	RESULT_TEMP_SKILL_SET_AFIRE, // 전투뱀 140
	RESULT_TEMP_SKILL_NOOSE_OF_WRAITH, // 마법뱀 140
	// 2004, 10, 15, sobeit add end - 테스트용

	// max
	MAX_ACTIONINFO
};


// MAX_ACTIONINFO
#define		MIN_RESULT_ACTIONINFO		RESULT_SKILL_ATTACK_MELEE
#define		MAX_RESULT_ACTIONINFO		RESULT_SKILL_ATTACK_BLADE
#define		MIN_RESULT_STEP_ACTIONINFO	STEP_SKILL_FLOURISH_2

#endif