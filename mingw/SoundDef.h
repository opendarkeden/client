//----------------------------------------------------------------------
// SoundDef.h
//----------------------------------------------------------------------

#ifndef	__SOUNDDEF_H__
#define	__SOUNDDEF_H__


//-------------------------------------------------------
// 각 상황에 맞는 Sound ID정의
//-------------------------------------------------------
enum SOUND_ID 
{
	//-------------------------------------------------------
	// slayer 행동	
	//-------------------------------------------------------
	SOUND_SLAYER_ATTACK_BARE,
	SOUND_SLAYER_ATTACK_KICK,
	SOUND_SLAYER_ATTACK_MELEE,
	SOUND_SLAYER_ATTACK_AR,
	SOUND_SLAYER_ATTACK_SMG,
	SOUND_SLAYER_ATTACK_BOMB,
	SOUND_SLAYER_ATTACK_BOTTLE,
	SOUND_SLAYER_DIE_MALE,
	SOUND_SLAYER_DIE_FEMALE,
	SOUND_SLAYER_DAMAGE_MALE,
	SOUND_SLAYER_DAMAGE_FEMALE,
	SOUND_SLAYER_DRAINED_MALE,
	SOUND_SLAYER_DRAINED_FEMALE,
	SOUND_SLAYER_SWORD_MALE,
	SOUND_SLAYER_SWORD_FEMALE,
	SOUND_SLAYER_MAGIC_MALE,
	SOUND_SLAYER_MAGIC_FEMALE,

	//-------------------------------------------------------
	// Item
	//-------------------------------------------------------
	SOUND_ITEM_MOVE_CLOTH,
	SOUND_ITEM_MOVE_SWORD,
	SOUND_ITEM_MOVE_GUN,
	SOUND_ITEM_MOVE_SHIELD,
	SOUND_ITEM_MOVE_BRACELET,
	SOUND_ITEM_MOVE_SHOES,
	SOUND_ITEM_MOVE_GLOVE,
	SOUND_ITEM_MOVE_HELM,
	SOUND_ITEM_MOVE_BELT,
	SOUND_ITEM_MOVE_MAGAZINE,
	SOUND_ITEM_MOVE_SCROLL,
	SOUND_ITEM_MOVE_KEY,
	SOUND_ITEM_MOVE_BOMB,
	SOUND_ITEM_MOVE_BOTTLE,
	SOUND_ITEM_MOVE_C4,
	SOUND_ITEM_MOVE_SKULL,
	SOUND_ITEM_MOVE_MONEY,
	SOUND_ITEM_MOVE_POTION,
	SOUND_ITEM_USE_MPOTION,
	SOUND_ITEM_USE_HPOTION,
	SOUND_ITEM_USE_SCROLL,
	SOUND_ITEM_USE_C4,
	SOUND_ITEM_USE_MAGAZINE,
	SOUND_ITEM_NO_MAGAZINE,
	SOUND_ITEM_FLYING_BOMB,
	SOUND_ITEM_FLYING_BOTTLE,
	SOUND_DAMAGE_BOMB,	
	SOUND_DAMAGE_BOTTLE,
	SOUND_DAMAGE_GUN,
	SOUND_WORLD_BIKE_GO,
	SOUND_WORLD_BIKE_RUN,
	SOUND_WORLD_BIKE_STOP,
	SOUND_WORLD_BIKE_DAMAGE,
	SOUND_WORLD_BIKE_EXPLODE,

	//-------------------------------------------------------
	// Interface
	//-------------------------------------------------------
	SOUND_PDS_MENU,
	SOUND_PCS_1,
	SOUND_PCS_2,
	SOUND_PCS_3,
	SOUND_PCS_4,
	SOUND_PCS_5,
	SOUND_PCS_6,
	SOUND_PCS_7,
	SOUND_PCS_8,
	SOUND_PCS_9,
	SOUND_PCS_0,
	SOUND_PCS_STAR,
	SOUND_PCS_SHARP,
	SOUND_PDS_ETC,
	SOUND_SLAYER_BELT,
	SOUND_SLAYER_SKILL,
	SOUND_SLAYER_BUTTON,
	SOUND_SLAYER_MENU,
	SOUND_VAMPIRE_ICON,
	SOUND_VAMPIRE_BUTTON,


	//-------------------------------------------------------
	// PC Vampire행동
	//-------------------------------------------------------
	SOUND_VAMPIRE_CHANGE_MALE,
	SOUND_VAMPIRE_CHANGE_FEMALE,	
	SOUND_VAMPIRE_ATTACK,
	SOUND_VAMPIRE_DAMAGE_MALE,
	SOUND_VAMPIRE_DAMAGE_FEMALE,
	SOUND_VAMPIRE_DIE_MALE,
	SOUND_VAMPIRE_DIE_FEMALE,
	SOUND_VAMPIRE_DRAINED_MALE,
	SOUND_VAMPIRE_DRAINED_FEMALE,
	SOUND_VAMPIRE_DRAIN_MALE,
	SOUND_VAMPIRE_DRAIN_FEMALE,
	SOUND_VAMPIRE_MAGIC,
	
	//-------------------------------------------------------
	// 데드 바디
	//-------------------------------------------------------
	SOUND_DB_ATTACK,
	SOUND_DB_DAMAGE,
	SOUND_DB_DIE,
	SOUND_DB_DRAINED,
	SOUND_DB_DRAIN,

	//-------------------------------------------------------
	// 터닝 데드
	//-------------------------------------------------------
	SOUND_TD_ATTACK,
	SOUND_TD_DAMAGE,
	SOUND_TD_DIE,
	SOUND_TD_DRAINED,
	SOUND_TD_DRAIN,

	//-------------------------------------------------------
	// 터닝 소울
	//-------------------------------------------------------
	SOUND_TS_ATTACK,
	SOUND_TS_DAMAGE,
	SOUND_TS_DIE,
	SOUND_TS_DRAINED,
	SOUND_TS_DRAIN,

	//-------------------------------------------------------
	// 키드
	//-------------------------------------------------------
	SOUND_K_ATTACK,
	SOUND_K_DAMAGE,
	SOUND_K_DIE,
	SOUND_K_DRAINED,
	SOUND_K_DRAIN,

	//-------------------------------------------------------
	// 솔져 
	//-------------------------------------------------------
	SOUND_S_ATTACK,
	SOUND_S_DAMAGE,
	SOUND_S_DIE,
	SOUND_S_DRAINED,
	SOUND_S_DRAIN,

	//-------------------------------------------------------
	// 캡틴
	//-------------------------------------------------------
	SOUND_C_ATTACK,
	SOUND_C_DAMAGE,
	SOUND_C_DIE,
	SOUND_C_DRAINED,
	SOUND_C_DRAIN,

	//-------------------------------------------------------
	// 섀도우 윙
	//-------------------------------------------------------
	SOUND_SW_ATTACK,
	SOUND_SW_DAMAGE,
	SOUND_SW_DIE,
	SOUND_SW_DRAINED,
	SOUND_SW_DRAIN,
	SOUND_SW_MAGIC,

	//-------------------------------------------------------
	// 더티 스트라이더
	//-------------------------------------------------------
	SOUND_DS_ATTACK,
	SOUND_DS_DAMAGE,
	SOUND_DS_DIE,
	SOUND_DS_DRAINED,
	SOUND_DS_DRAIN,
	SOUND_DS_MAGIC,

	//-------------------------------------------------------
	// 블러드 워록
	//-------------------------------------------------------
	SOUND_BW_ATTACK,
	SOUND_BW_DAMAGE,
	SOUND_BW_DIE,
	SOUND_BW_DRAINED,
	SOUND_BW_DRAIN,
	SOUND_BW_MAGIC,

	//-------------------------------------------------------
	// 골레머
	//-------------------------------------------------------
	SOUND_GO_MOVE,
	SOUND_GO_ATTACK,
	SOUND_GO_DAMAGE,
	SOUND_GO_DIE,
	SOUND_GO_DRAINED,
	SOUND_GO_DRAIN,

	//-------------------------------------------------------
	// 레드 아이
	//-------------------------------------------------------
	SOUND_RE_ATTACK,
	SOUND_RE_DAMAGE,
	SOUND_RE_DIE,
	SOUND_RE_DRAINED,
	SOUND_RE_DRAIN,
	SOUND_RE_MAGIC,

	//-------------------------------------------------------
	// 알칸
	//-------------------------------------------------------
	SOUND_AL_ATTACK,
	SOUND_AL_DAMAGE,
	SOUND_AL_DIE,
	SOUND_AL_DRAINED,
	SOUND_AL_DRAIN,
	SOUND_AL_MAGIC,

	//-------------------------------------------------------
	// 카오스 가디언
	//-------------------------------------------------------
	SOUND_CG_ATTACK,
	SOUND_CG_DAMAGE,
	SOUND_CG_DIE,
	SOUND_CG_DRAINED,
	SOUND_CG_DRAIN,	

	//-------------------------------------------------------
	// 환경
	//-------------------------------------------------------
	SOUND_WORLD_DOG_BARK,
	SOUND_WORLD_DOG_GRRR,
	SOUND_WORLD_DOG_KICKED,
	SOUND_WORLD_WOLF_CRY,
	SOUND_WORLD_CROW,
	SOUND_WORLD_BIRD,
	SOUND_WORLD_WEATHER_RAIN_1,
	SOUND_WORLD_WEATHER_RAIN_2,
	SOUND_WORLD_WEATHER_THUNDER_1,
	SOUND_WORLD_WEATHER_THUNDER_2,
	SOUND_WORLD_WEATHER_WIND,
	SOUND_WORLD_DOOR_WOOD,
	SOUND_WORLD_DOOR_IRON,
	SOUND_WORLD_WATER_DROP,

	//-------------------------------------------------------
	// Slayer 검 기술
	//-------------------------------------------------------
	SOUND_SLAYER_SWORD_DI,
	SOUND_SLAYER_SWORD_TS,
	SOUND_SLAYER_SWORD_SS,
	SOUND_SLAYER_SWORD_RS,
	SOUND_SLAYER_SWORD_THS,

	//-------------------------------------------------------
	// Slayer 도 기술
	//-------------------------------------------------------
	SOUND_SLAYER_BLADE_SB,
	SOUND_SLAYER_BLADE_TB,
	SOUND_SLAYER_BLADE_SS,
	SOUND_SLAYER_BLADE_TRB,
	SOUND_SLAYER_BLADE_WS,

	//-------------------------------------------------------
	// Slayer Enchant 기술
	//-------------------------------------------------------
	SOUND_SLAYER_ENCHANT_B1,	// bless시작
	SOUND_SLAYER_ENCHANT_B2,	// bless바닥에서
	SOUND_SLAYER_ENCHANT_CHW1,	// create holy water시작
	SOUND_SLAYER_ENCHANT_CHW2,	// 완성
	SOUND_SLAYER_ENCHANT_CL,	// continual light
	SOUND_SLAYER_ENCHANT_DH,	// detect hidden
	SOUND_SLAYER_ENCHANT_L,		// light
	SOUND_SLAYER_ENCHANT_S1,	// striking 시작
	SOUND_SLAYER_ENCHANT_S2,	// striking 끝?

	//-------------------------------------------------------
	// Slayer Healing 기술
	//-------------------------------------------------------
	SOUND_SLAYER_HEALING_CCW,	// cure critical wounds
	SOUND_SLAYER_HEALING_CLW,	// cure light wounds
	SOUND_SLAYER_HEALING_CP,	// cure poison
	SOUND_SLAYER_HEALING_CPA,	// cure paralysis
	SOUND_SLAYER_HEALING_PFC,	// protection from curse
	SOUND_SLAYER_HEALING_PFP,	// protection from poison
	SOUND_SLAYER_HEALING_PFPA,	// protection from paralysis
	SOUND_SLAYER_HEALING_R1,	// restore 시작
	SOUND_SLAYER_HEALING_R2,	// restore 끝	
	SOUND_SLAYER_HEALING_RC,	// remove curse

	//-------------------------------------------------------
	// Vampire 기술
	//-------------------------------------------------------
	SOUND_VAMPIRE_ACID_ABOLT1,		// acid bolt날아가기
	SOUND_VAMPIRE_ACID_ABOLT2,		// acid bolt터질때
	SOUND_VAMPIRE_MISC_BD,			// blood drain
	SOUND_VAMPIRE_MISC_H,			// hide
	SOUND_VAMPIRE_POISON_GP,		// green poison
	SOUND_VAMPIRE_POISON_PW,		// purple wall
	SOUND_VAMPIRE_POISON_YP,		// yellow poison
	SOUND_VAMPIRE_ACID_AT,			// acid touch
	SOUND_VAMPIRE_MISC_D,			// darkness
	SOUND_VAMPIRE_POISON_PH,		// poisonous hand

	//-------------------------------------------------------
	// 호블
	//-------------------------------------------------------
	SOUND_HB_ATTACK,
	SOUND_HB_DAMAGE,
	SOUND_HB_DIE,
	SOUND_HB_DRAINED,
	SOUND_HB_DRAIN,
	SOUND_HB_MAGIC,


	// 추가 2001.7.17

	//-------------------------------------------------------
	// 박쥐
	//-------------------------------------------------------
	SOUND_BAT_DAMAGE,
	SOUND_BAT_DIE,
	SOUND_BAT_WING,

	//-------------------------------------------------------
	// ITEM
	//-------------------------------------------------------
	SOUND_ITEM_MOVE_HEAD,
	SOUND_ITEM_MOVE_NECKLACE,
	SOUND_ITEM_MOVE_RING,

	//-------------------------------------------------------
	// slayer
	//-------------------------------------------------------
	SOUND_SLAYER_ATTACK_SG,
	SOUND_SLAYER_ATTACK_SILENCER,
	SOUND_SLAYER_ATTACK_SR,
	SOUND_SLAYER_CHARGE,

	SOUND_SLAYER_ENCHANT_AURABALL1,
	SOUND_SLAYER_ENCHANT_AURABALL2,
	SOUND_SLAYER_ENCHANT_AURARING1,
	SOUND_SLAYER_ENCHANT_AURARING2,
	SOUND_SLAYER_ENCHANT_AURASHIELD1,
	SOUND_SLAYER_ENCHANT_AURASHIELD2,
	SOUND_SLAYER_ENCHANT_BLESSGROUND,

	SOUND_SLAYER_HEALING_CSW,
	SOUND_SLAYER_HEALING_REMOVECURSE,

	//-------------------------------------------------------
	// Vampire
	//-------------------------------------------------------
	SOUND_VAMPIRE_ACIDBALL1,
	SOUND_VAMPIRE_ACIDBALL2,
	SOUND_VAMPIRE_ACIDBURN,
	SOUND_VAMPIRE_DOOM,
	SOUND_VAMPIRE_GREENSTALKER1,
	SOUND_VAMPIRE_GREENSTALKER2,

	SOUND_VAMPIRE_PARALYSIS,
	SOUND_VAMPIRE_POISON_DB,
	SOUND_VAMPIRE_TRANSFORM,
	
	//-------------------------------------------------------
	// 늑대
	//-------------------------------------------------------
	SOUND_WOLF_ATTACK,
	SOUND_WOLF_DAMAGE,
	SOUND_WOLF_DIE,
	SOUND_WOLF_DRAINED,
	SOUND_WOLF_EAT,
	SOUND_WOLF_GROWL,
	SOUND_WOLF_HOWL,

	//-------------------------------------------------------
	// World
	//-------------------------------------------------------
	SOUND_WORLD_COCK,
	SOUND_WORLD_CRICKET,
	SOUND_WORLD_FROG,
	SOUND_WORLD_GRASS,
	SOUND_WORLD_OWL,
	SOUND_WORLD_PROPELLER,
	SOUND_WORLD_RATS,
	SOUND_WORLD_RIVER,
	SOUND_WORLD_SEWER,
	SOUND_WORLD_TREE,
	SOUND_WORLD_WIND,	
	
	//-------------------------------------------------------
	// 으흠?
	//-------------------------------------------------------
	SOUND_BLOOD_SWORD,

	//-------------------------------------------------------
	// 에스트로이더
	//-------------------------------------------------------
	SOUND_ES_ATTACK,
	SOUND_ES_DAMAGE,
	SOUND_ES_DIE,
	SOUND_ES_DRAIN,
	SOUND_ES_DRAINED,

	//-------------------------------------------------------
	// 모데라스
	//-------------------------------------------------------
	SOUND_MD_ATTACK,
	SOUND_MD_DAMAGE,
	SOUND_MD_DIE,
	SOUND_MD_DRAIN,
	SOUND_MD_DRAINED,

	//-------------------------------------------------------
	// 뮤턴트
	//-------------------------------------------------------
	SOUND_MT_ATTACK,
	SOUND_MT_DAMAGE,
	SOUND_MT_DIE,
	SOUND_MT_DRAIN,
	SOUND_MT_DRAINED,

	//-------------------------------------------------------
	// 위도우즈
	//-------------------------------------------------------
	SOUND_WD_ATTACK,
	SOUND_WD_DAMAGE,
	SOUND_WD_DIE,
	SOUND_WD_DRAIN,
	SOUND_WD_DRAINED,

	//-------------------------------------------------------
	// 이벤트용
	//-------------------------------------------------------
	SOUND_EVENT_FANFARE,


	// 2001. 9. 26 추가

	//-------------------------------------------------------
	// 빅팽
	//-------------------------------------------------------
	SOUND_BF_ATTACK,
	SOUND_BF_DAMAGE,
	SOUND_BF_DIE,
	SOUND_BF_DRAIN,
	SOUND_BF_DRAINED,

	//-------------------------------------------------------
	// 반델라이저
	//-------------------------------------------------------
	SOUND_VD_ATTACK,
	SOUND_VD_DAMAGE,
	SOUND_VD_DIE,
	SOUND_VD_DRAIN,
	SOUND_VD_DRAINED,

	//-------------------------------------------------------
	// 슬레이어 기술
	//-------------------------------------------------------
	SOUND_SLAYER_BLADE_TS,						// tornado sever
	SOUND_SLAYER_ENCHANT_DETECTINVISIBILITY,
	SOUND_SLAYER_ENCHANT_IDENTIFY,
	SOUND_SLAYER_ENCHANT_PFA,						// protection from acid
	SOUND_SLAYER_HEAL_SACRIFICE,
	SOUND_SLAYER_SWORD_LH,						// lightning hands
	SOUND_SLAYER_SWORD_SC,						// snake combo
	SOUND_SLAYER_SWORD_SW,						// sword wave

	//-------------------------------------------------------
	// 뱀파이어 기술
	//-------------------------------------------------------
	SOUND_VAMPIRE_ACIDSWAMP,		


	// 2001.11.1 추가

	//-------------------------------------------------------
	// 카오스 나이트
	//-------------------------------------------------------
	SOUND_CK_ATTACK,
	SOUND_CK_DAMAGE,
	SOUND_CK_DIE,
	SOUND_CK_DRAIN,
	SOUND_CK_DRAINED,
	SOUND_CK_MAGIC,

	//-------------------------------------------------------
	// 크림슨 슬로터
	//-------------------------------------------------------
	SOUND_CS_ATTACK,
	SOUND_CS_DAMAGE,
	SOUND_CS_DIE,
	SOUND_CS_DRAIN,
	SOUND_CS_DRAINED,

	//-------------------------------------------------------
	// 다크스크리머
	//-------------------------------------------------------
	SOUND_DSC_ATTACK,
	SOUND_DSC_DAMAGE,
	SOUND_DSC_DIE,
	SOUND_DSC_DRAIN,
	SOUND_DSC_DRAINED,
	SOUND_DSC_MAGIC,

	//-------------------------------------------------------
	// 아이언티쓰
	//-------------------------------------------------------
	SOUND_IT_ATTACK,
	SOUND_IT_DAMAGE,
	SOUND_IT_DIE,
	SOUND_IT_DRAIN,
	SOUND_IT_DRAINED,

	//-------------------------------------------------------
	// item
	//-------------------------------------------------------
	SOUND_ITEM_MOVE_JEWEL,

	//-------------------------------------------------------
	// world
	//-------------------------------------------------------
	SOUND_WORLD_MAP,
	SOUND_WORLD_WALKIETALKIE,

	//-------------------------------------------------------
	// 2001.12.17 추가
	//-------------------------------------------------------
	//-------------------------------------------------------
	// 폭탄 소리
	//-------------------------------------------------------
	SOUND_ITEM_BOOM1,
	SOUND_ITEM_BOOM2,
	SOUND_ITEM_BOOM3,
	SOUND_ITEM_BOOM4,

	//-------------------------------------------------------
	// 슬레이어 기술
	//-------------------------------------------------------
	SOUND_SLAYER_BLADE_BERSERKER,
	SOUND_SLAYER_BLADE_EARTHQUAKE,
	SOUND_SLAYER_ENCHANT_ENCHANT,
	SOUND_SLAYER_ENCHANT_VISIBLEA,
	SOUND_SLAYER_ENCHANT_VISIBLEB,
	SOUND_SLAYER_HEAL_ACTIVATION,
	SOUND_SLAYER_HEAL_CUREALL,
	SOUND_SLAYER_SWORD_EXPANSION,
	SOUND_SLAYER_SWORD_THUNDERBOLTA,
	SOUND_SLAYER_SWORD_THUNDERBOLTB,
	SOUND_SLAYER_SWORD_WINDDIVIDER,

	//-------------------------------------------------------
	// 뱀파이어 기술
	//-------------------------------------------------------
	SOUND_VAMPIRE_BLOODYBALL,
	SOUND_VAMPIRE_BLOODYKNIFE,
	SOUND_VAMPIRE_BLOODYWALL,
	SOUND_VAMPIRE_DEATH,
	SOUND_VAMPIRE_HALLUCINATION,
	SOUND_VAMPIRE_SEDUCTION,
	SOUND_VAMPIRE_SUMMONCASKET,

	//-------------------------------------------------------
	// 크리스마스 이벤트 사운드
	//-------------------------------------------------------
	SOUND_XMAS_GIFTBOX,
	SOUND_XMAS_STAR,

	//-------------------------------------------------------
	// 신종 몬스터 3종 볼바메두사, 딤가고일, 애쉬발록 - 2003.4.23
	//-------------------------------------------------------
	SOUND_VOL_MAGIC,
	SOUND_VOL_DIE,
	SOUND_VOL_DAMAGE,
	SOUND_VOL_ATTACK,

	SOUND_ASH_DIE,
	SOUND_ASH_DAMAGE,
	SOUND_ASH_ATTACK,
	
	SOUND_DIM_DIE,
	SOUND_DIM_DAMAGE,
	SOUND_DIM_ATTACK,
	SOUND_DIM_MAGIC,

	//------------------------------------------------------------
	// 2003.5.16
	//------------------------------------------------------------
	SOUND_DRAGON_FIRE_CRACKER,
	SOUND_AMPUTATE,
	SOUND_CONCEALMENT,
	SOUND_DENIAL_MAGIC_DAMAGED,
	SOUND_DENIAL_MAGIC_START,
	SOUND_REQUITAL,

	//------------------------------------------------------------
	// 아우스터즈 기본 
	//------------------------------------------------------------
	SOUND_OUSTERS_ARMSBAND,
	SOUND_OUSTERS_BOOTS,
	SOUND_OUSTERS_CHAKRAM,
	SOUND_OUSTERS_CIRCLET,
	SOUND_OUSTERS_COAT,
	SOUND_OUSTERS_ELEMENTAL_STONE,
	SOUND_OUSTERS_INTERFACE,
	SOUND_OUSTERS_PENDANT,
	SOUND_OUSTERS_RING,
	SOUND_OUSTERS_SKILL_INFO,
	SOUND_OUSTERS_WRISTLET,

	//------------------------------------------------------------
	// 아우스터즈 기본동작
	//------------------------------------------------------------
	SOUND_OUSTERS_ATTACK_1, // 일반 MELEE 공격
	SOUND_OUSTERS_ATTACK_2, // 차크람 던지기
	SOUND_OUSTERS_MAGIC_1,  // 
	SOUND_OUSTERS_MAGIC_2,
	SOUND_OUSTERS_DAMAGED,
	SOUND_OUSTERS_FAST_MOVE_1,
	SOUND_OUSTERS_FAST_MOVE_2,
	SOUND_OUSTERS_DIE,
	SOUND_OUSTERS_ABSORB_SOUL,
	SOUND_OUSTERS_DRAIN,

	//------------------------------------------------------------
	// 아우스터즈 스킬
	//------------------------------------------------------------
	SOUND_OUSTERS_PROMINENCE,
	SOUND_OUSTERS_BACK_STAB,
	SOUND_OUSTERS_BLAZE_BOLT,
	SOUND_OUSTERS_BLUNTING,
	SOUND_OUSTERS_CROSS_GUARD,
	SOUND_OUSTERS_EVADE,
	SOUND_OUSTERS_FLOURISH,
	SOUND_OUSTERS_GAMMA_CHOP,
	SOUND_OUSTERS_LIBERTY,
	SOUND_OUSTERS_SHARP_ROUND,
	
	SOUND_OUSTERS_BLOCK_HEAD,
	SOUND_OUSTERS_GNOME_WHISPER,
	SOUND_OUSTERS_EARTH_TEETH,
	SOUND_OUSTERS_SAND_OF_SOUL_STONE,
	SOUND_OUSTERS_ICE_OF_SOUL_STONE,
	SOUND_OUSTERS_FIRE_OF_SOUL_STONE,
	SOUND_OUSTERS_REFUSAL_ETHER,
	SOUND_OUSTERS_TENDRIL,
	SOUND_OUSTERS_HANDS_OF_FIRE,
	SOUND_OUSTERS_ICE_FIELD,
	SOUND_OUSTERS_RING_OF_FLARE,
	SOUND_OUSTERS_PROMINENCE_2,
	SOUND_OUSTERS_HANDS_OF_NIZIE,
	SOUND_OUSTERS_STONE_AUGER,
	SOUND_OUSTERS_NYMPH_RECOVERY,
	SOUND_OUSTERS_WATER_BARRIER,
	
	SOUND_OUSTERS_PUPA,
	SOUND_OUSTERS_MEI,
	
	//------------------------------------------------------------
	// 110레벨 스킬
	//------------------------------------------------------------
	SOUND_SLAYER_BLITZ_SLIDING,
	SOUND_SLAYER_BLAZE_WALK,
	SOUND_SLAYER_JABBING_VEIN,
	SOUND_SLAYER_GREAT_HEAL,
	SOUND_SLAYER_DIVINE_GUIDANCE,
	SOUND_VAMPIRE_BLOODY_ZENITH,

	// 리젠타워 관련
	SOUND_REGEN_TOWER_REPEAT,
	SOUND_REGEN_TOWER_START,
	SOUND_REGEN_TOWER_FLASH,
	SOUND_REGEN_TOWER_OWNER_CHANGED,
	SOUND_SWEEPER_START,
	SOUND_SWEEPER_REPEAT,
	SOUND_SWEEPER_END,

	// 아우스킬
	SOUND_DESTRUCTION_SPEAR,
	SOUND_DUCKING_WALLOP,
	SOUND_FATAL_SNICK,
	SOUND_FROZEN_ARMOR,
	SOUND_GROUND_BLESS,
	SOUND_HELLFIRE,
	SOUND_ICE_LANCE,
	SOUND_MAGNUM_SPEAR_1,
	SOUND_MAGNUM_SPEAR_2,
	SOUND_MAGNUM_SPEAR_3,
	SOUND_REACTIVE_ARMOR,
	SOUND_SAND_CROSS,
	SOUND_SHIFT_BREAK,
	SOUND_SOUL_REBIRTH,
	SOUND_TELEPORT,

	//2004,04,27 sobeit 2nd pet sound add start
	SOUND_WORLD_CENTAURO_ATTACK,
	SOUND_WORLD_CENTAURO_MOVE,
	SOUND_WORLD_CENTAURO_PICKUP,
	SOUND_WORLD_STIRGE_ATTACK,
	SOUND_WORLD_STIRGE_MOVE,
	SOUND_WORLD_PIXIE_ABSORB,
	SOUND_WORLD_PIXIE_MOVE,
	//2004,04,27 sobeit 2nd pet sound add end

	// 2004, 7, 7, sobeit add start - 질드레 레어 사운드 관련
	SOUND_BLIND_SUMMON,		// 질드레 얼음 석상 소환 시
	SOUND_BLIND_DESTORY,	// 질드레 얼음 석상 파괴 시
	SOUND_PIPE_ROUTING,		// 맹인의 피리 루팅 시
	SOUND_PIPE_DROP,		// 맹인의 피리 드롭 시
	SOUND_BLINDNESS,		// 블라인드니스 스킬 걸릴 시
	SOUND_TRAP_A,			// TRAP_ICICLE_AUGER 시전 시
	SOUND_TRAP_B,			// TRAP_ICICLE_LARGE_AUGER 시전 시
	SOUND_DROP_A,			// ICICLE_DROP 시전 시
	SOUND_SIDE_SUMMON,		// ICE_STICK 시전 시
	SOUND_SIDE_DESTORY,		// ICE_STICK 종료 시
	SOUND_GLACIER_START,	// 글래셔 시전 시 - 질드레 스킬
	SOUND_GLACIER_END,		// 글래셔 종료 시 - 질드레 스킬
	SOUND_ICEAUGER,			// 아이스 오거 시전 시 - 질드레 스킬
	SOUND_ICEHAIL,			// 아이스 헤일 시전 시 - 질드레 스킬
	SOUND_ICEWAVE,			// 아이스 웨이브 시전 시 - 질드레 스킬
	SOUND_GDR_GHOST1,		// 질드레 레어 유령
	SOUND_GDR_GHOST2,		// 질드레 레어 유령
	SOUND_GDR_GHOST3,		// 질드레 레어 유령
	SOUND_GDR_GHOST4,		// 질드레 레어 유령
	// 2004, 7, 7, sobeit add end- 질드레 레어 사운드 관련
	
	// 2004, 10, 6, sobeit add start - 130 스킬 사운드
	SOUND_SLAYER_TURRET_FIRE,
	SOUND_SLAYER_TURRET_FIRE_START,
	SOUND_SLAYER_TURRET_FIRE_END,
	SOUND_SLAYER_SWORDOFTHOR,
	SOUND_SLAYER_WITHSUNTIDE,
	SOUND_SLAYER_SWEEPVICE_HIT,
	SOUND_SLAYER_SWEEPVICE_RESIZE,
	SOUND_VAMPIRE_SUMMONGOREGRAND,
	// 2004, 10, 6, sobeit add end - 130 스킬 사운드
	
	// 2004, 11, 8, sobeit add start - 그레이트 러피언 도끼
	SOUND_GREAT_RUFFIAN_AXE,
	// 2004, 11, 8, sobeit add end

	// 2004, 12, 14, sobeit add start - 140 스킬 사운드
	SOUND_SLAYER_PLASMA_ROCKET_LAUNCHER,
	SOUND_SLAYER_BOMBING_STAR,
	SOUND_SLAYER_INTIMATE_GRAIL,
	SOUND_OUSTERS_FURY_OF_GNOME,
	SOUND_OUSTERS_SUMMON_MIGA,
	// 2004, 12, 14, sobeit add end - 140 스킬 사운드
	
	// 2005, 1, 11, sobeit add start - 새로운 몬스터들
	//---------------------------------------------
	// 플리거 - 자폭병
	//---------------------------------------------
	SOUND_FLIEGER_BOMB,
	//---------------------------------------------
	// 기프레이터 - 박격포
	//---------------------------------------------
	SOUND_GEFREITER_ATTACK,
	SOUND_GEFREITER_DAMAGE,
	SOUND_GEFREITER_DEAD,
	SOUND_GEFREITER_DRAINED,
	//---------------------------------------------
	// 하우트만 - 장교
	//---------------------------------------------
	SOUND_HAUPTMANN_DAMAGE,
	SOUND_HAUPTMANN_DEAD,
	SOUND_HAUPTMANN_DRAINED,
	SOUND_HAUPTMANN_MAGIC,
	//---------------------------------------------
	// 룽가테스타 - 신체변이
	//---------------------------------------------
	SOUND_LUNGA_TESTA_ATTACK,
	SOUND_LUNGA_TESTA_DAMAGE,
	SOUND_LUNGA_TESTA_DEAD,
	SOUND_LUNGA_TESTA_DRAINED,
	SOUND_LUNGA_TESTA_VAMPIRE_DRAIN,
	//---------------------------------------------
	// 오베르슈츠 - SR
	//---------------------------------------------
	SOUND_OBERSCHUTZE_SR_ATTACK,
	SOUND_OBERSCHUTZE_SR_DAMAGE,
	SOUND_OBERSCHUTZE_SR_DEAD,
	SOUND_OBERSCHUTZE_SR_DRAINED,
	SOUND_OBERSCHUTZE_SR_MELEE,
	//---------------------------------------------
	// 로이카다브루 - 철퇴인간
	//---------------------------------------------
	SOUND_ROI_CADAVRU_ATTACK,
	SOUND_ROI_CADAVRU_DAMAGE,
	SOUND_ROI_CADAVRU_DEAD,
	SOUND_ROI_CADAVRU_DRAINED,
	SOUND_ROI_CADAVRU_VAMPIRE_DRAIN,
	//---------------------------------------------
	// 시아메스 - 쌍둥이
	//---------------------------------------------
	SOUND_SIAMESE_ATTACK,
	SOUND_SIAMESE_DAMAGE,
	SOUND_SIAMESE_DEAD,
	SOUND_SIAMESE_DRAINED,
	SOUND_SIAMESE_VAMPIRE_DRAIN,
	//---------------------------------------------
	// 스터르만 - SMG
	//---------------------------------------------
	SOUND_STURMMANN_SMG_ATTACK,
	SOUND_STURMMANN_SMG_DAMAGE,
	SOUND_STURMMANN_SMG_DEAD,
	SOUND_STURMMANN_SMG_DRAINED,
	//---------------------------------------------
	// 터그레그 - 골룸
	//---------------------------------------------
	SOUND_TUGLEG_ATTACK,
	SOUND_TUGLEG_DAMAGE,
	SOUND_TUGLEG_DEAD,
	SOUND_TUGLEG_DRAINED,
	SOUND_TUGLEG_MOVE,
	SOUND_TUGLEG_VAMPIRE_DRAIN,
	// 2005, 1, 11, sobeit add end - 새로운 몬스터들
	MAX_SOUND
};

#endif