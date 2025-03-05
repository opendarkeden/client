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

#ifndef __NEW_SKILL__
	#define __NEW_SKILL__
	//#undef  __NEW_SKILL__
#endif


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
	SKILL_STEP_NULL,			// °ü°è¾ø´Â °Íµé

	// Slayer
	SKILL_STEP_APPRENTICE,
	SKILL_STEP_ADEPT,
	SKILL_STEP_EXPERT,
	SKILL_STEP_MASTER, 
	SKILL_STEP_GRAND_MASTER,
	SKILL_STEP_SLAYER_BLADE_ADVANCEMENT,
	SKILL_STEP_SLAYER_SWORD_ADVANCEMENT,
	SKILL_STEP_SLAYER_GUN_BOMB_ADVANCEMENT,
	SKILL_STEP_SLAYER_HEAL_ADVANCEMENT,
	SKILL_STEP_SLAYER_ENCHANT_ADVANCEMENT,

	// Vampire
	SKILL_STEP_VAMPIRE_POISON,
	SKILL_STEP_VAMPIRE_ACID,
	SKILL_STEP_VAMPIRE_CURSE,
	SKILL_STEP_VAMPIRE_SUMMON,
	SKILL_STEP_VAMPIRE_BLOOD,
	SKILL_STEP_VAMPIRE_INNATE,
	SKILL_STEP_VAMPIRE_ADVANCEMENT, 
	
	// Ousters
	SKILL_STEP_OUSTERS_COMBAT,
	SKILL_STEP_OUSTERS_ELEMENTAL,
	SKILL_STEP_OUSTERS_ETC,
	SKILL_STEP_OUSTERS_COMMON_ADVANCEMENT,
	SKILL_STEP_OUSTERS_COMBAT_ADVANCEMENT,
	SKILL_STEP_OUSTERS_EARTH_ADVANCEMENT,
	SKILL_STEP_OUSTERS_FIRE_ADVANCEMENT,
	SKILL_STEP_OUSTERS_WATER_ADVANCEMENT,

	SKILL_STEP_ETC,

};

//----------------------------------------------------------------------
// SkillID == ActionInfo Á¤ÀÇ
//----------------------------------------------------------------------
enum ACTIONINFO
{
		// °Ë°è¿­,
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

	// µµ °è¿­,
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

	// °Ç, ¶óÀÌÇÃ °è¿­
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
    
	// ¼ºÁ÷¸¶¹ý,
	// ÀÎÃ¾ °è¿­
	MAGIC_CREATE_HOLY_WATER, 			 		// 39
	MAGIC_LIGHT,				      			// 40
	MAGIC_DETECT_HIDDEN,	   	 				// 41
	MAGIC_AURA_BALL,	     		 		 	// 55
	MAGIC_BLESS,			       		 		// 45
	MAGIC_CONTINUAL_LIGHT,	 			 		// 46
	MAGIC_FLARE,								// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
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

	// Èú °è¿­
	MAGIC_CURE_LIGHT_WOUNDS,					// 52
	MAGIC_CURE_POISON,							// 54
	MAGIC_PROTECTION_FROM_POISON,				// 53
	MAGIC_CAUSE_LIGHT_WOUNDS,					// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	MAGIC_CURE_SERIOUS_WOUNDS,					// 56
	MAGIC_REMOVE_CURSE,							// 89
	MAGIC_PROTECTION_FROM_CURSE,				// 59
	MAGIC_CAUSE_SERIOUS_WOUNDS,					// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	MAGIC_CURE_CRITICAL_WOUNDS,					// 
	MAGIC_PROTECTION_FROM_ACID,					//
	MAGIC_SACRIFICE,							//
	MAGIC_CAUSE_CRITICAL_WOUNDS,				// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	MAGIC_CURE_ALL,								// 62
	MAGIC_REGENERATION,							// 61
	MAGIC_MASS_CURE,							//
	MAGIC_MASS_HEAL,							//

	// ¹ìÇÁ±â¼ú,
	SKILL_BLOOD_DRAIN,							// 65

	// ¹ìÇÁ¸¶¹ý,
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

	// ½á¸ó
	MAGIC_SUMMON_WOLF,							// 87
	MAGIC_SUMMON_CASKET,						// 88
	MAGIC_RAISING_DEAD,							// 88
	MAGIC_SUMMON_SERVANT,						//

	// ETC
	MAGIC_UN_BURROW,							// 89
	MAGIC_UN_TRANSFORM,							// 89
	MAGIC_UN_INVISIBILITY,
	MAGIC_THROW_HOLY_WATER,						// 91

	// ´Á´ë ±â¼ú
	MAGIC_EAT_CORPSE,
	MAGIC_HOWL,

	// NPC ±â¼ú
	MAGIC_RESTORE,								// 90

	MAGIC_BLOODY_MARK,
	MAGIC_BLOODY_TUNNEL,

	MAGIC_SEDUCTION,
	SKILL_WIND_DIVIDER,
	SKILL_EARTHQUAKE,

	MAGIC_RESURRECT,
	MAGIC_PRAYER,
	MAGIC_MEDITATION,

	// 2001.12.4 Ãß°¡
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
	SKILL_CRITICAL_GROUND,		// criticalÀ» »ç¿ëÇÏ´Â °æ¿ì	
	
	SKILL_VIGOR_DROP,
	
	// [»õ±â¼ú]
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

 // Áúµå·¹ ½ºÅ³
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
    SKILL_CHOPPING_FIREWOOD,                    // 337 ÀåÀÛÆÐ±â	

	SKILL_CHAIN_THROWING_AXE,                   // 338
    SKILL_MULTI_THROWING_AXE,                   // 339
    SKILL_PLAYING_WITH_FIRE,                    // 340 ºÒÀå³­

	SKILL_INFINITY_THUNDERBOLT,                 // 341
    SKILL_SPIT_STREAM,                          // 342
    SKILL_PLASMA_ROCKET_LAUNCHER,               // 343
    SKILL_INTIMATE_GRAIL,                       // 344
    SKILL_BOMBING_STAR,                         // 345
    SKILL_SET_AFIRE,                            // 346
    SKILL_NOOSE_OF_WRAITH,                      // 347

    SKILL_SHARP_HAIL,                           // 348
    SKILL_SUMMON_MIGA,                          // 349  // ¾Æ¿ì½ºÅÍÁî°¡ ¾²´Â ½ºÅ³
    SKILL_SUMMON_MIGA_ATTACK,                   // 350  // ¼ÒÈ¯µÈ³ðÀÌ ¾²´Â ½ºÅ³
    SKILL_ICE_HORIZON,                          // 351
    SKILL_FURY_OF_GNOME,                        // 352
	
	SKILL_CANNONADE,                          // 353  // Æ÷°Ý
    SKILL_SELF_DESTRUCTION,                   // 354  // ÀÚÆø°ø°Ý

	SKILL_AR_ATTACK,                            // 355  // ¸ó½ºÅÍ¿ë
    SKILL_SMG_ATTACK,                           // 356  // ¸ó½ºÅÍ¿ë
    SKILL_GRENADE_ATTACK,                       // 357  // ¸ó½ºÅÍ¿ë

	SKILL_DRAGON_TORNADO,                       // 358
    SKILL_BIKE_CRASH,                           // 359
    SKILL_HARPOON_BOMB,                         // 360
    SKILL_PASSING_HEAL,                         // 361
    SKILL_ROTTEN_APPLE,                         // 362
    SKILL_WILD_WOLF,                            // 363
    SKILL_ABERRATION,                           // 364
	SKILL_HALO,                                 // 365
    SKILL_DESTINIES,                            // 366
    SKILL_FIERCE_FLAME,                         // 367
    SKILL_SHADOW_OF_STORM,                      // 368
	
	SKILL_HEAL_PASS,                            // 369 // ¿Å°Ü°¡´Â Èú

	SKILL_TRASLA_ATTACK,                      // 370
    SKILL_PUSCA_ATTACK,                       // 371
	SKILL_NOD_COPILA_ATTACK,                  // 372

    SKILL_NOD_COPILA_ATTACK_2,                    // 373
#ifdef __NEW_SKILL__

	// 2007-2-15 Coffee add start
	//SkillID: 409 EN: Bloody Scarify  
	SKILL_BLLODY_SCARIFY,		//374  ÑªÖ®ÀÓÓ¡  (ÎüÑª¹íÐÂ¼¼ÄÜ )
	SKILL_BLOOD_CURSE,		    //375  ÑªÖ®Ô{Öä  
	//////////////////////////////////////////////////////////////////////////
	//ÈËÀà¼¼ÄÜ
	SKILL_SHINE_SWORD,			//376  ÉÁÒ«Ö®½£
	SKILL_BOMB_CRASH_WALK,		//377  ¾ÞÅÚºäÕ¨
	SKILL_SATELLITE_BOMB,		//378  ÎÀÐÇºä»÷
	SKILL_ILLUSION_INVERSION,	//379  ¿Ö²À»Ã¾õ
	SKILL_HEAVEN_GROUND,		//380  ÌìÉñ½µÁÙ
		// ¿Ö²À»Ã¾õ·ÖÉíµ¥¸öÏÔÊ¾Ð§¹û
	TEMP_SKILL_ILLUSION_INVERSION,//381
	//////////////////////////////////////////////////////////////////////////
	// Ä§ÁéÐÂ¼¼ÄÜ
	// 2007-2-15 Coffee add end
	SKILL_DUMMY_DRAKE,       // 382 µÂÀ×¿Ë¿þÀÜ(»ð)
	SKILL_HYDRO_CONVERGENCE, // 383 ¸´ºÏË®ÁÆ(Ë®)
	SKILL_SUMMON_CLAY,		 // 384 Õ³ÍÁÕÙ»½(ÍÁ)
	SKILL_HETER_CHAKRAM,			 // 385 ÏÄ²¼Àû»ùÒò(Õ½Ê¿)

#endif
	//add by viva 
	//new Slayer
	SKILL_SLAYER_SWORD_SKY_FIRE,	//ÌìÀ×»ð»¨
	SKILL_SLAYER_BLADE_CUT_STORM,	//¾âÈÐ·ç±©
	SKILL_SLAYER_GUN_BOMB_XRL_MISSILE,	//XRL Õ¨ÁÑµ¼µ¯
	SKILL_SLAYER_HEAL_SACRED_STAMP,	//ÆðÔ´Ê¥Ó¡
	SKILL_SLAYER_ENCHANT_BRAMBLE_HALO,	//¾£¼¬¹â»·
	//New Vampire
	SKILL_VAMPIRE_INNATE_DEADLY_CLAW,	//ÖÁÉËÁÑ×¦
	SKILL_VAMPIRE_POISON_VOODOO_RING,	//Î×¶¾Ö®»·
	//New Ousters
	SKILL_OUSTERS_COMBAT_PENETRATE_WHEEL,	//¹á´©Ö®ÂÖ
	SKILL_OUSTERS_FIRE_FIRE_METEOR,	//·Ù»ðÁ÷ÐÇ
	SKILL_OUSTERS_EARTH_BIG_ROCKFALL,	//¾ÞÊ¯ÔÉÂä
	SKILL_OUSTERS_WATER_RAPID_FREEZE,	//¼«¶³±ù»·
	///////////////////
	SKILL_UNKOWN1,
	SKILL_UNKOWN2,
	SKILL_UNKOWN3,
	SKILL_UNKOWN4,
	SKILL_UNKOWN5,
	SKILL_UNKOWN6,
	SKILL_UNKOWN7,
	SKILL_UNKOWN8,
	SKILL_UNKOWN9,
	SKILL_UNKOWN10,
	SKILL_UNKOWN11,
	SKILL_UNKOWN12,
	MAGIC_VAMPIRE_TRANSFORM_TO_D_BAT, //by viva : 2th new
	MAGIC_VAMPIRE_TRANSFORM_TO_D_ANGEL, //by viva : 2th new
//end


	//------------------------------------------------------------
	// ¿©±âºÎÅÍ´Â client Àü¿ë SKILL ID
	//------------------------------------------------------------
	// ÀÓ½Ã·Î Ãß°¡	
	
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
	SKILL_ATTACK_GUN_SR,			// ÃÑ1
	SKILL_ATTACK_GUN_SG,			// ÃÑ2
	SKILL_ATTACK_GUN_AR,			// ÃÑ3
	SKILL_ATTACK_GUN_SMG,			// ÃÑ4

	// sword
	SKILL_ATTACK_SWORD,		// Ä®1
	SKILL_ATTACK_BLADE,		// Ä®2	

	// ÃÑ¿¡ µû¸¥ holy shooting
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
	// ½ºÅ³ ´Ü°è¿¡ µû¸¥ ½ºÅ³
	//------------------------------------------------------------------------------
	STEP_SKILL_FLOURISH_2,							// 2´Ü°è FLOURISH
	STEP_SKILL_FLOURISH_3,							// 3´Ü°è FLOURISH
	STEP_SKILL_SHARP_ROUND_2,						// 2´Ü°è Sharp Round
	STEP_SKILL_SHARP_ROUND_3,						// 3´Ü°è Sharp Round
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
	MAGIC_ENCHANT_OPTION_NULL,				// ¿É¼ÇÀÌ ¾ø¾îÁø´Ù
	MAGIC_ENCHANT_OPTION_PLUS,				// ¿É¼ÇÀÌ ÇÏ³ª ¿Ã¶ó°£´Ù
	MAGIC_ENCHANT_REMOVE_ITEM,				// ¾ÆÀÌÅÛÀÌ ¾ø¾îÁø´Ù
	MAGIC_UNKOWN1,			//add by viva
	
	// HolyLandBonus Skills 2003.2.10
	SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA,				// ±â¼ú »ç¿ë½Ã MP(HP)¼Ò¸ð·® 50% °¨¼Ò  
	SKILL_HOLYLAND_BLOOD_BIBLE_MIHOLE,				// Å¸Á¾Á·ÀÇ ¾ð¾î Åë¿ª  
	SKILL_HOLYLAND_BLOOD_BIBLE_KIRO,				// INT+5, DEX +5  
	SKILL_HOLYLAND_BLOOD_BIBLE_INI,					// ¹°¸®°ø°Ý µ¥¹ÌÁö +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_GREGORI,				// ¹°¾à°ª 50% ÇÒÀÎ  
	SKILL_HOLYLAND_BLOOD_BIBLE_CONCILIA,			// HP +50  
	SKILL_HOLYLAND_BLOOD_BIBLE_LEGIOS,				// ¸¶¹ý°ø°Ýµ¥¹ÌÁö +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL,				// Çà¿î +10  
	SKILL_HOLYLAND_BLOOD_BIBLE_JAVE,				// °×ºí°¡°Ý 50% ÇÒÀÎ  
	SKILL_HOLYLAND_BLOOD_BIBLE_NEMA,				// ½Ã¾ß +5, ¸ðµç ´É·ÂÄ¡ +3  
	SKILL_HOLYLAND_BLOOD_BIBLE_AROSA,				// ¸ðµç ÀúÇ×·Â +9  
	SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA,				// STR +5, INT +5 

	SKILL_LOVE_CHAIN,								// ·¯ºê Ã¼ÀÎ ½ºÅ³	
	
	MAGIC_TRANS_ITEM_OK,	
	SKILL_TRANSLATION,
	
	SKILL_SWEEPER_BONUS_1,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_2,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_3,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_4,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_5,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_6,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_7,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_8,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_9,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_10,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_11,							// ½ºÀ§ÆÛ º¸³Ê½º
	SKILL_SWEEPER_BONUS_12,							// ½ºÀ§ÆÛ º¸³Ê½º
	
	// 2004, 6, 15 sobeit add start 
	SKILL_CLIENT_CAN_ENTER_GDR_LAIR, //- Áúµå·¹ µé¾î°¥¼ö ÀÖÀ½
	SKILL_CLIENT_BLINDNESS,			// ´ÙÅ©´Ï½º¿Í °°Àº È¿°ú
	// 2004, 6, 15 sobeit add end - 
	SKILL_CLIENT_DRAGON_EYES,	// µå·¹°ï ¾ÆÀÌÁî

	SKILL_CLIENT_EXP_BONUS_EVENT, // °æÇèÄ¡ º¸³Ê½º

	SKILL_ClIENT_UNKOWN1,	//add by viva
	//-----------------------------------------------------------------
	// RESULT_ACTIONINFO
	//-----------------------------------------------------------------		
	// ±âº» ±â¼ú
	RESULT_SKILL_ATTACK_MELEE,						// 0
	RESULT_SKILL_ATTACK_ARMS,							// 1
	RESULT_SKILL_SELF,									// 2
	RESULT_SKILL_TILE,									// 3
	RESULT_SKILL_OBJECT,	    						// 4

	// °Ë°è¿­,	
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

	// µµ °è¿­,
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

	// °Ç, ¶óÀÌÇÃ °è¿­
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
    
	// ¼ºÁ÷¸¶¹ý,
	// ÀÎÃ¾ °è¿­
	RESULT_MAGIC_CREATE_HOLY_WATER, 			 		// 39
	RESULT_MAGIC_LIGHT,				      			// 40
	RESULT_MAGIC_DETECT_HIDDEN,	   	 				// 41
	RESULT_MAGIC_AURA_BALL,	     		 		 	// 55
	RESULT_MAGIC_BLESS,			       		 		// 45
	RESULT_MAGIC_CONTINUAL_LIGHT,	 			 		// 46
	RESULT_MAGIC_FLARE,								// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
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

	// Èú °è¿­
	RESULT_MAGIC_CURE_LIGHT_WOUNDS,					// 52
	RESULT_MAGIC_CURE_POISON,							// 54
	RESULT_MAGIC_PROTECTION_FROM_POISON,				// 53
	RESULT_MAGIC_CAUSE_LIGHT_WOUNDS,					// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	RESULT_MAGIC_CURE_SERIOUS_WOUNDS,					// 56
	RESULT_MAGIC_REMOVE_CURSE,							// 89
	RESULT_MAGIC_PROTECTION_FROM_CURSE,				// 59
	RESULT_MAGIC_CAUSE_SERIOUS_WOUNDS,					// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	RESULT_MAGIC_CURE_CRITICAL_WOUNDS,					// 
	RESULT_MAGIC_PROTECTION_FROM_ACID,					//
	RESULT_MAGIC_SACRIFICE,							//
	RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,				// ¾ó¸¶ÀÏ±î ´Ù½Ã Á¤¸® ÇØ¾ßÁö
	RESULT_MAGIC_CURE_ALL,								// 62
	RESULT_MAGIC_REGENERATION,							// 61
	RESULT_MAGIC_MASS_CURE,							//
	RESULT_MAGIC_MASS_HEAL,							//

	// ¹ìÇÁ±â¼ú,
	RESULT_SKILL_BLOOD_DRAIN,							// 65

	// ¹ìÇÁ¸¶¹ý,
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

	// ½á¸ó
	RESULT_MAGIC_SUMMON_WOLF,							// 87
	RESULT_MAGIC_SUMMON_CASKET,						// 88
	RESULT_MAGIC_RAISING_DEAD,							// 88
	RESULT_MAGIC_SUMMON_SERVANT,						//

	// ETC
	RESULT_MAGIC_UN_BURROW,							// 89
	RESULT_MAGIC_UN_TRANSFORM,							// 89
	RESULT_MAGIC_UN_INVISIBILITY,
	RESULT_MAGIC_THROW_HOLY_WATER,						// 91

	// ´Á´ë ±â¼ú
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

	// 2001.12.4 Ãß°¡
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

	// Ãß°¡ 2002.3.23
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
	RESULT_SKILL_CRITICAL_GROUND,		// criticalÀ» »ç¿ëÇÏ´Â °æ¿ì	

	RESULT_SKILL_VIGOR_DROP,

	// [»õ±â¼ú]
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
	//RESULT_SKILL_INSTALL_TRAP,//RESULT_SKILL_HOLY_ARMOR,//by viva //ousters							// 272
	RESULT_SKILL_HOLY_ARMOR,
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
	// Áúµå·¹ ½ºÅ³
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
    RESULT_SKILL_CHOPPING_FIREWOOD,                    // 337 ÀåÀÛÆÐ±â	

	RESULT_SKILL_CHAIN_THROWING_AXE,                   // 338
    RESULT_SKILL_MULTI_THROWING_AXE,                   // 339
    RESULT_SKILL_PLAYING_WITH_FIRE,                    // 340 ºÒÀå³­

	RESULT_SKILL_INFINITY_THUNDERBOLT,                 // 341
    RESULT_SKILL_SPIT_STREAM,                          // 342
    RESULT_SKILL_PLASMA_ROCKET_LAUNCHER,               // 343
    RESULT_SKILL_INTIMATE_GRAIL,                       // 344
    RESULT_SKILL_BOMBING_STAR,                         // 345
    RESULT_SKILL_SET_AFIRE,                            // 346
    RESULT_SKILL_NOOSE_OF_WRAITH,                      // 347

	RESULT_SKILL_SHARP_HAIL,                           // 348
    RESULT_SKILL_SUMMON_MIGA,                          // 349  // ¾Æ¿ì½ºÅÍÁî°¡ ¾²´Â ½ºÅ³
    RESULT_SKILL_SUMMON_MIGA_ATTACK,                   // 350  // ¼ÒÈ¯µÈ³ðÀÌ ¾²´Â ½ºÅ³
    RESULT_SKILL_ICE_HORIZON,                          // 351
    RESULT_SKILL_FURY_OF_GNOME,                        // 352
	
	RESULT_SKILL_CANNONADE,								// 353  // Æ÷°Ý
    RESULT_SKILL_SELF_DESTRUCTION,						// 354  // ÀÚÆø°ø°Ý

	RESULT_SKILL_AR_ATTACK,                            // 355  // ¸ó½ºÅÍ¿ë
    RESULT_SKILL_SMG_ATTACK,                           // 356  // ¸ó½ºÅÍ¿ë
    RESULT_SKILL_GRENADE_ATTACK, //by viva ousters(problem)(RESULT_SKILL_GUN_SHOT_GUIDANCE)                     // 357  // ¸ó½ºÅÍ¿ë

	RESULT_SKILL_DRAGON_TORNADO,                       // 358
    RESULT_SKILL_BIKE_CRASH,                           // 359
    RESULT_SKILL_HARPOON_BOMB,                         // 360
    RESULT_SKILL_PASSING_HEAL,                         // 361
    RESULT_SKILL_ROTTEN_APPLE,                         // 362
    RESULT_SKILL_WILD_WOLF,                            // 363
    RESULT_SKILL_ABERRATION,                           // 364
	RESULT_SKILL_HALO,                                 // 365
    RESULT_SKILL_DESTINIES,                            // 366
    RESULT_SKILL_FIERCE_FLAME,                         // 367
    RESULT_SKILL_SHADOW_OF_STORM,                      // 368

	RESULT_SKILL_HEAL_PASS,                            // 369 // ¿Å°Ü°¡´Â Èú

	RESULT_SKILL_TRASLA_ATTACK,                      // 370
    RESULT_SKILL_PUSCA_ATTACK,                       // 371
    RESULT_SKILL_NOD_COPILA_ATTACK,                  // 372
    RESULT_SKILL_NOD_COPILA_ATTACK_2,                    // 373
	// 2007-2-15  add by coffee start
#ifdef __NEW_SKILL__
	RESULT_SKILL_BLLODY_SCARIFY,					// 374
	RESULT_SKILL_BLOOD_CURSE,						// 375
	RESULT_SKILL_SHINE_SWORD,						// 376  ÉÁÒ«Ö®½£
	RESULT_SKILL_BOMB_CRASH_WALK,					// 377  ¾ÞÅÚºäÕ¨
	RESULT_SKILL_SATELLITE_BOMB,					// 378  ÎÀÐÇºä»÷
	RESULT_SKILL_ILLUSION_INVERSION,				// 379  ¿Ö²À»Ã¾õ
	RESULT_SKILL_HEAVEN_GROUND,						// 380  ÌìÉñ½µÁÙ
	// ¿Ö²À»Ã¾õ·ÖÉíµ¥¸öÏÔÊ¾Ð§¹û
	RESULT_TEMP_SKILL_ILLUSION_INVERSION,			// 
	// Ä§Áé¼¼ÄÜ
	RESULT_SKILL_DUMMY_DRAKE,        // 382 µÂÀ×¿Ë¿þÀÜ(»ð)
	RESULT_SKILL_HYDRO_CONVERGENCE,  // 383 ¸´ºÏË®ÁÆ(Ë®)
	RESULT_SKILL_SUMMON_CLAY,		 // 384 Õ³ÍÁÕÙ»½(ÍÁ)
	RESULT_SKILL_HETER_CHAKRAM,			 // 385 ÏÄ²¼Àû»ùÒò(Õ½Ê¿)
#endif
	// 2007-2-15 add by coffee end

	//add by viva
	RESULT_SKILL_SLAYER_SWORD_SKY_FIRE,
	RESULT_SKILL_SLAYER_BLADE_CUT_STORM,
	RESULT_SKILL_SLAYER_GUN_BOMB_XRL_MISSILE,
	RESULT_SKILL_SLAYER_HEAL_SACRED_STAMP,
	RESULT_SKILL_SLAYER_ENCHANT_BRAMBLE_HALO,
	RESULT_SKILL_VAMPIRE_INNATE_DEADLY_CLAW,
	RESULT_SKILL_VAMPIRE_POISON_VOODOO_RING,
	RESULT_SKILL_OUSTERS_COMBAT_PENETRATE_WHEEL,
	RESULT_SKILL_UNKOWN,
	RESULT_SKILL_OUSTERS_EARTH_BIG_ROCKFALL,
	RESULT_SKILL_OUSTERS_WATER_RAPID_FREEZE,
	RESULT_SKILL_UNKOWN1,
	RESULT_SKILL_UNKOWN2,
	RESULT_SKILL_UNKOWN3,
	RESULT_SKILL_UNKOWN4,
	RESULT_SKILL_UNKOWN5,
	RESULT_SKILL_UNKOWN6,
	RESULT_SKILL_UNKOWN7,
	RESULT_SKILL_UNKOWN8,
	RESULT_SKILL_UNKOWN9,
	RESULT_SKILL_UNKOWN10,
	RESULT_SKILL_UNKOWN11,
	RESULT_SKILL_UNKOWN12,
	RESULT_MAGIC_VAMPIRE_TRANSFORM_TO_D_BAT,	//by viva : 2th new
	RESULT_MAGIC_VAMPIRE_TRANSFORM_TO_D_ANGEL,	//by viva : 2th new
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	
	// ÀÓ½Ã·Î Ãß°¡
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
	RESULT_SKILL_ATTACK_GUN_SR,			// ÃÑ1
	RESULT_SKILL_ATTACK_GUN_SG,			// ÃÑ2
	RESULT_SKILL_ATTACK_GUN_AR,			// ÃÑ3
	RESULT_SKILL_ATTACK_GUN_SMG,		// ÃÑ4
	
	// Sword
	RESULT_SKILL_ATTACK_SWORD,			// Ä®
	RESULT_SKILL_ATTACK_BLADE,			// µµ

	// ÃÑ¿¡ µû¸¥ holy shooting
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
	// ½ºÅ³ ´Ü°è
	//-----------------------------------------------------------------------------
	RESULT_STEP_SKILL_FLOURISH_2,							// 2´Ü°è FLOURISH
	RESULT_STEP_SKILL_FLOURISH_3,							// 3´Ü°è FLOURISH
	RESULT_STEP_SKILL_SHARP_ROUND_2,						// 2´Ü°è Sharp Round
	RESULT_STEP_SKILL_SHARP_ROUND_3,						// 3´Ü°è Sharp Round
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

	// client Àü¿ë
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

	// ¹æ¾î¸· È¿°ú¸¦ ³»±â À§ÇØ¼­..
	REFLECT_AURA_PRISM,				//	
	REFLECT_AURA_SHIELD,				//	
	PROTECTION_FROM_ACID,
	REFLECT_REFLECTION,				//	SKILL_REFLECTION¿¡ ÀÇÇÑ°Í

	// ¹Ù´Ú¿¡ Èê¸®´Â ÇÇ
	BLOOD_DROP_GROUND,
	BLOOD_CRITICAL_HIT,	
	BLOOD_RESURRECT,
	
	// ¾Æ¸¶°Ôµ· 2002.3.4
	ARMAGEDDON_CRASH_1,			// Ã¹¹øÂ° ºÎ¼­Áö´Â°Å
	ARMAGEDDON_CRASH_2,			// µÎ¹øÂ°
	ARMAGEDDON_CRASH_3,			// ¼¼¹øÂ°
	ARMAGEDDON_CRASH_4,			// ¸¶Áö¸· ³²±â
	
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

	ARMEGA_TILE,					// ¾Æ¸£¸Þ°¡
	MIHOLE_TILE,					// ¹ÌÈ£·¹
	KIRO_TILE,						// Å°·Î
	INI_TILE,						// ¾ÆÀÌ´Ï
	GREGORI_TILE,					// ±×¸®°í¸® ÀÌÆåÆ®(Å¸ÀÏ¿¡ ºÙ´Â°Í)
	CONCILIA_TILE,					// ÄÜÄ¥¸®¾Æ
	LEGIOS_TILE,					// ·¹Áö¿À½º
	HILLEL_TILE,					// Èú¸±
	JAVE_TILE,						// Àðºê
	NEMA_TILE,						// ³×¸¶
	AROSA_TILE,						// ¾Æ·Î»ç
	CHASPA_TILE,					// Â÷½ºÆÄ

	HOLY_LAND_WARP_VAMPIRE,
	HOLY_LAND_WARP_SLAYER,

	WARP_BLOOD_BIBLE_TILE,

	AMBER_OF_GUARD_TILE,
	AMBER_OF_AVENGER_TILE,
	AMBER_OF_IMMORTAL_TILE,
	AMBER_OF_CURSE_TILE,

	SKILL_DENIAL_MAGIC_DAMAGED,
	SKILL_DRAGON_FIRE_CRACKER,

	SKILL_FIRE_CRACKER_4,			// Ãß°¡
	
	SKILL_FIRE_CRACKER_VOLLEY_1,		// 3¿¬¹ß ÆøÁ×-_-
	SKILL_FIRE_CRACKER_VOLLEY_2,		// 3¿¬¹ß ÆøÁ×-_-
	SKILL_FIRE_CRACKER_VOLLEY_3,		// 3¿¬¹ß ÆøÁ×-_-
	SKILL_FIRE_CRACKER_VOLLEY_4,		// 3¿¬¹ß ÆøÁ×-_-
	
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_1,	// 3¿¬¹ß ±¤¿ª ÆøÁ×
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_2,	// 3¿¬¹ß ±¤¿ª ÆøÁ×
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_3,	// 3¿¬¹ß ±¤¿ª ÆøÁ×
	SKILL_FIRE_CRACKER_WIDE_VOLLEY_4,	// 3¿¬¹ß ±¤¿ª ÆøÁ×

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
	
	REGEN_TOWER_TRING_POSITION,			// ¸®Á¨Á¸ Å¸¿ö ÀïÅ» Å¸ÀÏ
	REGEN_TOWER_THROW_LIGHT,			// ¸®Á¨Á¸ ºû ºñÄ¡´Â°Å 
	REGEN_TOWER_TRING_POSITION_LIGHT,	// ¸®Á¨Á¸ Å¸¿ö ÀïÅ» Å¸ÀÏ¿¡ ¿Ã¶úÀ»¶§
	REGEN_TOWER_FLASH_BEAD,				// ¸®Á¨Á¸ Å¸¿ö ±¸½½ ¹ÝÂ¦°Å¸®´Â°Å.

	SKILL_CLIENT_TELEPORT,
	SKILL_CLIENT_TELEPORT2,
	SKILL_CLIENT_DUCKING_WALLOP,
	SKILL_CLIENT_CHARGING_ATTACK,

	SKILL_CLIENT_SUMMON_GROUND_ELEMENTAL,
	SKILL_CLIENT_FIRE_ELEMENTAL_ATTACK,
	//SKILL_CLIENT_WATER_ELEMENTAL_ATTACK,//SKILL_CLIENT_WATER_ELEMENTAL_HEAL,//by viva ousters
	SKILL_CLIENT_WATER_ELEMENTAL_HEAL,
	
	SKILL_CLIENT_TANK_SMOKE,
	SKILL_CLIENT_TANK_ATTACK_3,
	SKILL_CLIENT_TANK_ATTACK_4,
	SKILL_CLIENT_TANK_ATTACK_5,
	SKILL_CLIENT_TANK_ATTACKED,


	// 2004,3,6 sobeit add start - Æê ½ºÅ³
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
	// 2004,3,6 sobeit add end - Æê ½ºÅ³

	// 2004,3,22 sobeit add start - °ËÀº ±â¿î
	MAP_BLACK_LARGE_SMOKE,
	MAP_BLACK_HALL,
	MAP_BLACK_SMALL_SMOKE_1,
	MAP_BLACK_SMALL_SMOKE_2,
	MAP_BLACK_SMALL_SMOKE_3,

	MAP_ELECTRIC_POST,
	MAP_GHOST_1,
	MAP_GHOST_2,
	// 2004,3,22 sobeit add end - °ËÀº ±â¿î
	
	// 2004, 3, 30 sobeit add start - ¿ï¹ö¸° ³ª¿Ã¶§ ÀÌÆåÆ®
	RESULT_SUMMON_ULBERINE,
	// 2004, 3, 30 sobeit add end - ¿ï¹ö¸° ³ª¿Ã¶§ ÀÌÆåÆ®

	// 2004, 4,2  sobeit add start - Çà¿îÀÇ ¿À·Î¶ó(³×ÀÙ Å¬·Î¹Ù)
	RESULT_LUCKY_AURA,
	RESULT_MISFORTUNE_AURA,
	// 2004, 4,2 sobeit add end - Çà¿îÀÇ ¿À·Î¶ó(³×ÀÙ Å¬·Î¹Ù)

	// 2004, 5, 6, sobeit add start
	AMBER_OF_LIFE_TILE,
	AMBER_OF_LIGHT_TILE,
	// 2004, 5, 6, sobeit add end

	// 2004, 5, 18 sobeit add start
	SKILL_CLIENT_TRAP_ICICLE_DROP,					// ¶³¾îÁö´Â ¾óÀ½ Æ®·¦
	SKILL_CLIENT_TRAP_ICICLE_DROP_SIGN,

	SKILL_CLIENT_TRAP_ICICLE_AUGER,
	SKILL_CLIENT_TRAP_ICICLE_AUGER_SIGN,

	SKILL_CLIENT_TRAP_ICE_STICK_SW,		// ÁÂÇÏ
	SKILL_CLIENT_TRAP_ICE_STICK_SW_SIGN,// ÁÂÇÏ
	SKILL_CLIENT_TRAP_ICE_STICK_SE,		// ¿ìÇÏ
	SKILL_CLIENT_TRAP_ICE_STICK_SE_SIGN,// ¿ìÇÏ
	SKILL_CLIENT_TRAP_ICE_STICK_NE,		// ¿ì»ó
	SKILL_CLIENT_TRAP_ICE_STICK_NE_SIGN,// ¿ì»ó
	SKILL_CLIENT_TRAP_ICE_STICK_NW,		// ÁÂ»ó
	SKILL_CLIENT_TRAP_ICE_STICK_NW_SIGN,// ÁÂ»ó

	SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER,
	SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER_SIGN,
	// 2004, 5, 18 sobeit add start
	

	// 2004, 6, 8 sobeit add start - Áúµå·¹ ÁÖº¯ ÀÌÆåÆ®
//	SKILL_CLIENT_SUMMON_GDR, 
//    SKILL_CLIENT_GDR_SATELLITE,
//    SKILL_CLIENT_GDR_FLOATING, 
//
//	SKILL_CLIENT_SLAYER_MASTER_130,
//	SKILL_CLIENT_SLAYER_MASTER_150,
//	SKILL_CLIENT_VAMPIRE_MASTER_130,
//	SKILL_CLIENT_VAMPIRE_MASTER_150, // 150 ·¹º§ ¸¶½ºÅÍ ÀÌÆåÆ®
//	SKILL_CLIENT_OUSTERS_MASTER_130,
//	SKILL_CLIENT_OUSTERS_MASTER_150,
	// 2004, 6, 8 sobeit add end

	// 2004, 6, 15 sobeit add start - gdr skill - glacier
	SKILL_CLIENT_GDR_GLACIER_SLAYER,
	SKILL_CLIENT_GDR_GLACIER_VAMPIRE,
	SKILL_CLIENT_GDR_GLACIER_OUSTERS,
	// 2004, 6, 15 sobeit add end - gdr skill - glacier

	// 2004, 6, 26 sobeit add start
	MAP_GDR_LAIR_POTAL,	// Áúµå·¹ Æ÷Å» ÀÌÆåÆ®
	// 2004, 6, 26 sobeit add end

	// 2004, 8, 5 sobeit add start
	SKILL_CLIENT_CASTLE_GATE_DEAD, // ¼º¹® ºÎ¼­Áú¶§(Á×À»¶§) Å« ¸ÕÁöÀÌÆåÆ®
	SKILL_CLIENT_CASTLE_GATE_DAMAGED,// ¼º¹® Å¸°Ý ¹ÞÀ» ¶§
	SKILL_CLIENT_GUILD_TOWER_DEAD, // ±æµåÅ¸¿ö ºÎ¼­Áú¶§(Á×À»¶§) Å« ¸ÕÁöÀÌÆåÆ®
	SKILL_CLIENT_GUILD_TOWER_PROTECT, // ±æµåÅ¸¿ö º¸È£ ÀÌÆåÆ®
	// 2004, 8, 5 sobeit add end

	// 2004, 8, 31, sobeit add start
	SKILL_CLIENT_LAND_MINE_LIGHT,		// ·£µå¸¶ÀÎ¿¡ °è¼Ó ºÙ¾î ÀÖ´Â ÀÌÆåÆ®
	SKILL_CLIENT_PLEASUER_STATION_LIGHT,// ÇÁ·¹Á® ½ºÅ×ÀÌ¼Ç¿¡ °è¼Ó ºÙ¾î ÀÖ´Â ÀÌÆåÆ®
	// 2004, 8, 31, sobeit add end

	// 2004, 9, 2, sobeit add start
	SKILL_CLIENT_TURRET_LASER,					// ÅÍ·¿ ·¹ÀÌÀú 1Å¸ÀÏ
	SKILL_CLIENT_TURRET_LASER_ATTACK,           // ÅÍ·¿ ·¹ÀÌÀú °ø°Ý 1Å¸ÀÏ (°ø°ÝÃø¿¡µµ º¸ÀÓ) 
	// 2004, 9, 2, sobeit add end

	// 2004, 9, 6, sobeit add start - Áúµå·¹ Á×±â ÀÌÆåÆ® , ¸®ÇªÅä ÆåÅÒ Å¸°Ý
	SKILL_CLIENT_GDR_DEAD,
	SKILL_CLIENT_HIT_REPUTO_FACTUM,
	// 2004, 9, 6, sobeit add end - Áúµå·¹ Á×±â ÀÌÆåÆ®

	SKILL_CLIENT_SWEEP_VICE,// ÀÎÃ¦ ½ºÀ¬ ¹ÙÀÌ½º 1´Ü°è
	SKILL_CLIENT_SWORD_OF_THOR, // ¼Òµå ¿Àºê Åä¸£ Áö¼Ó µÇ´Â°Å

	SKILL_CLIENT_BURNING_SOL_1, // µµ½½ ¹ö´×¼Ö 1´Ü°è
	SKILL_CLIENT_BURNING_SOL_2, // µµ½½ ¹ö´×¼Ö 2´Ü°è
	SKILL_CLIENT_BURNING_SOL_3, // µµ½½ ¹ö´×¼Ö 3´Ü°è
	SKILL_CLIENT_BURNING_SOL_4, // µµ½½ ¹ö´×¼Ö 4´Ü°è
	RESULT_CLIENT_BURNING_SOL_1, // µµ½½ ¹ö´×¼Ö 1´Ü°è
	RESULT_CLIENT_BURNING_SOL_2, // µµ½½ ¹ö´×¼Ö 2´Ü°è
	RESULT_CLIENT_BURNING_SOL_3, // µµ½½ ¹ö´×¼Ö 3´Ü°è
	RESULT_CLIENT_BURNING_SOL_4, // µµ½½ ¹ö´×¼Ö 4´Ü°è
	
	// 2004, 10, 21, sobeit add start - ±×·¹ÀÌÆ® ·¯ÇÇ¾ð °ü·Ã
	SKILL_CLIENT_GREAT_RUFFIAN_ALIVE, // 2Â÷ »ý¼ºµÉ¶§ ÀÌÆåÆ®
	SKILL_CLIENT_GREAT_RUFFIAN_BODY, // ¸ö¿¡ ºÙ´Â ÀÌÆåÆ®
	SKILL_CLIENT_GREAT_RUFFIAN_DIE, // Á×À»¶§ ³ª¿À´Â ÀÌÆåÆ®
	// 2004, 10, 21, sobeit add end

	// 2004, 11, 11, sobeit add start - ÃÑ 140 ÅÍÁö´Â°Å
	SKILL_CLIENT_PLASMA_ROCKET_LAUNCHER_BLOW,
	// 2004, 11, 11, sobeit add end - 

	// 2004, 11, 18, sobeit add start
	SKILL_CLIENT_FURY_OF_GNOME,
	// 2004, 11, 18, sobeit add end

	// 2004, 11, 22, sobeit add start
	SKILL_CLIENT_BLOOD_BIBLE_IN_GEAR_WINDOW,
	// 2004, 11, 22, sobeit add end
	
	// 2004, 12, 30, sobeit add start
	SKILL_CLIENT_HALO_ATTACK,
	SKILL_CLIENT_HALO_STOP,
	SKILL_CLIENT_FIERCE_FLAME,
	SKILL_CLIENT_HARPOON_BOMB,
	SKILL_CLIENT_BIKE_CRASH,
	//RESULT_SKILL_INTIMATE_GRAIL,//SKILL_CLIENT_ROTTEN_APPLE,//by viva ousters
	SKILL_CLIENT_ROTTEN_APPLE,
	// 2004, 12, 30, sobeit add end

	SKILL_CLIENT_CANDLE, // ÃÐºÒ ÀÌÆåÆ®
	
	SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_START,
	SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_END,

	SKILL_CLIENT_QUEST_MAGIC_ZONE,
	SKILL_CLIENT_QUEST_MAGIC_ZONE_POTAL,

	// 2004, 10, 15, sobeit add start - Å×½ºÆ®¿ë
	TEMP_SKILL_INFINITY_THUNDERBLOT, // °Ë½½ 140
	TEMP_SKILL_SPIT_STREAM, // µµ½½ 140
	TEMP_SKILL_PLASMA_ROCKET_LAUNCHER, //ÃÑ 140
	TEMP_SKILL_INTIMATE_GRAIL, // ÀÎÃ¾ 140
	TEMP_SKILL_BOMBING_STAR, // Èú 140,
	TEMP_SKILL_SET_AFIRE, // ÀüÅõ¹ì 140
	TEMP_SKILL_NOOSE_OF_WRAITH, // ¸¶¹ý¹ì 140

	RESULT_TEMP_SKILL_INFINITY_THUNDERBLOT, // °Ë½½ 140
	RESULT_TEMP_SKILL_SPIT_STREAM, // µµ½½ 140
	RESULT_TEMP_SKILL_PLASMA_ROCKET_LAUNCHER, //ÃÑ 140
	RESULT_TEMP_SKILL_INTIMATE_GRAIL, // ÀÎÃ¾ 140
	RESULT_TEMP_SKILL_BOMBING_STAR, // Èú 140,
	RESULT_TEMP_SKILL_SET_AFIRE, // ÀüÅõ¹ì 140
	RESULT_TEMP_SKILL_NOOSE_OF_WRAITH, // ¸¶¹ý¹ì 140
	// 2004, 10, 15, sobeit add end - Å×½ºÆ®¿ë
// add by Coffee 2007-3-17
#ifdef __NEW_SKILL__

    RESULT_SKILL_SATELLITE_BOMB_AIM,
    RESULT_SKILL_SATELLITE_BOMB_POINT,
	SKILL_CLIENT_DUMMY_DRAKE,
	SKILL_CLIENT_HYDRO_CONVERGENCE,
	//SKILL_HETER_CHAKRAM,//SKILL_CLIENT_SKILL_DUMMY_DRAKE,//by viva ousters
	SKILL_CLIENT_SKILL_DUMMY_DRAKE,
	SKILL_CLIENT_BLOOD_CURSE, //ÎüÑª¹íÐÂ¼¼ÄÜ(ÑªÖ®×çÖä)
#endif
// end
	SKILL_CLIENT_XRL_ROCKET_LAUNCHER_BLOW,	//add by viva
	// max
	MAX_ACTIONINFO
};


// MAX_ACTIONINFO
#define		MIN_RESULT_ACTIONINFO		RESULT_SKILL_ATTACK_MELEE
#define		MAX_RESULT_ACTIONINFO		RESULT_SKILL_ATTACK_BLADE 
#define		MIN_RESULT_STEP_ACTIONINFO	STEP_SKILL_FLOURISH_2

#endif