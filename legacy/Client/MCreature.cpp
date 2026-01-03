//----------------------------------------------------------------------
// MCreature.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MZone.h"
#include "MObject.h"
#include "MCreature.h"
#include "ClientDef.h"
#include "MTopView.h"
#include "MActionInfoTable.h"
#include "MEffectGeneratorTable.h"
#include "MCreatureTable.h"
#include "MSoundTable.h"
#include "MEffectStatusTable.h"
#include "FL2.h"
#include "ClientConfig.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "SkillDef.h"
#include "MLevelNameTable.h"
#include "EffectSpriteTypeDef.h"
#include "SoundDef.h"
#include "MParty.h"
#include "PacketFunction.h"
#include "MonsterNameTable.h"
#include "MEffectSpriteTypeTable.h"
#include "MAttachOrbitEffect.h"
#include "UserInformation.h"
//#include "RequestClientPlayerManager.h"
//#include "packet\Rpackets\CRRequest.h"
#include "VS_UI_base.h"
#include "MSkillManager.h"	// [货扁贱3]
#include "UIFunction.h"
#include "MEventManager.h"
#include "PacketFunction2.h"
#include "RankBonusTable.h"
#include "RankBonusDef.h"
#include "MTestDef.h"
#include "MTimeItemManager.h"
#include "MinTr.h"
#include "MFakeCreature.h"
#include "SystemAvailabilities.h"
#include "MGameStringTable.h"
#include "vs_ui.h"
#include "Client.h"

#define ATTACHEFFECTCOLOR_NULL	0xFFFF

extern DWORD	g_CurrentTime;
extern DWORD	g_CurrentFrame;

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;
extern WORD					g_ZoneCreatureColorSet;

#ifdef OUTPUT_DEBUG	
	bool g_bCheckError = false;	// 抛胶飘甫 困秦辑 烙矫肺..
#endif

//#define	new			DEBUG_NEW
#define				MAX_DARKNESS_COUNT		13

#ifdef __METROTECH_TEST__
	extern bool g_bLight;
#endif

//----------------------------------------------------------------------
// Get ArmageddonSprite
//----------------------------------------------------------------------
// 捞芭档 member function栏肺 持绢具 登绰单.. 哪颇老 矫埃锭巩俊 老窜 -_-;
//----------------------------------------------------------------------
//TYPE_EFFECTSPRITETYPE
//GetArmageddonSprite(MCreature* pCreature)
//{
//	DWORD aHP = pCreature->GetArmageddon();
//
//	if (aHP > 300)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_0_2;
//	}
//	else if (aHP > 200)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_1_2;
//	}
//	else if (aHP > 100)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_2_2;
//	}
//	
//	return EFFECTSPRITETYPE_ARMAGEDDON_BACK_3_2;	
//}
//
//int
//IsChangedArmageddonSprite(DWORD previousHP, DWORD currentHP)
//{
//	if (previousHP > 300)
//	{
//		if (currentHP <= 300)
//		{
//			return 1;
//		}
//
//		return 0;
//	}
//	else if (previousHP > 200)
//	{
//		if (currentHP <= 200)
//		{
//			return 2;
//		}
//
//		return 0;
//	}
//	else if (previousHP > 100)
//	{
//		if (currentHP <= 100)
//		{
//			return 3;
//		}
//
//		return 0;
//	}
//	
//	if (currentHP == 0)
//	{
//		return 4;
//	}
//	
//	return 0;
//}

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------
// 傲 窃荐肺 父甸绢具 登绰单.. 庆歹官操绊 哪颇老窍扁 饺绢辑 烙矫肺 - -;
//DEBUG_ADD_FORMAT("[ Apply Buffering Move ] [ID=%d] Current(%d, %d) Dir(%d) --> Next(%d, %d) Dir(%d)",	\
//															m_ID,												\
//															m_X, m_Y, m_Direction,								\
//															m_NextX,m_NextY, m_NextDirection);					\
		
#define MoveNextPosition()					\
											\
		MovePosition( m_NextX, m_NextY );	\
		int dir = m_Direction;				\
											\
		SetAction( m_MoveAction );			\
		m_Direction = m_NextDirection;		\
		m_DirectionMove = m_NextDirection;	\
		ActionMove();						\
		m_Direction = dir;					\
											\
		m_NextX = SECTORPOSITION_NULL;		\
		m_NextY = SECTORPOSITION_NULL;		\
		m_NextDirection = DIRECTION_NULL;

#define	ActionMoveNextPosition()					\
		if (!m_bFastMove)							\
		{											\
			if (m_NextX != SECTORPOSITION_NULL		\
				&& m_NextDirection != SECTORPOSITION_NULL)	\
			{										\
				MoveNextPosition();					\
			}										\
			else if (m_NextAction==m_MoveAction)	\
			{										\
				SetAction( m_NextAction );			\
				ActionMove();						\
			}										\
		}

#define SetServerPositionWithBlock(sX, sY)							\
		{															\
			CLASS_TYPE classType = GetClassType();					\
																	\
			if (classType!=CLASS_PLAYER && classType!=CLASS_FAKE	\
				&& (m_ServerX!=sX || m_ServerY!=sY))				\
			{														\
				m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);	\
																	\
				m_ServerX = sX;										\
				m_ServerY = sY;										\
																	\
				if (m_bAlive)										\
				{													\
					m_pZone->SetServerBlock(m_MoveType, sX, sY);	\
				}													\
			}														\
		}

//----------------------------------------------------------------------
// 茄锅俊 捞悼窍绰 pixel俊 措茄 沥焊 
// ACTIONCOUNT_MOVE父怒 sXTable狼 规氢俊 歹秦林搁 sXTable蔼捞 0捞 等促.
// 8规氢俊 规氢付促 Move荐父怒..
//----------------------------------------------------------------------
/*
int MCreature::m_cXTableFrame8[8][8] = 
{
	{ -6, -6, -6, -6, -6, -6, -6, -6 },		// LEFT
	{ -6, -6, -6, -6, -6, -6, -6, -6 },		// LEFTDOWN
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// DOWN
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHTDOWN
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHT
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHTUP
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// UP
	{ -6, -6, -6, -6, -6, -6, -6, -6 }		// LEFTUP
};

int MCreature::m_cXTableFrame8[8][8] = 
{
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// LEFT
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// LEFTDOWN
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// DOWN
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// RIGHTDOWN
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// RIGHT
	{ -3, -3, -3, -3, -3, -3, -3, -3 },		// RIGHTUP
	{ -3, -3, -3, -3, -3, -3, -3, -3 },		// UP
	{ -3, -3, -3, -3, -3, -3, -3, -3 }		// LEFTUP
};

int MCreature::m_cXTableFrame4[8][4] = 
{
	{ -12, -12, -12, -12 },		// LEFT
	{ -12, -12, -12, -12 },		// LEFTDOWN
	{   0,   0,   0,   0 },		// DOWN
	{  12,  12,  12,  12 },		// RIGHTDOWN
	{  12,  12,  12,  12 },		// RIGHT
	{  12,  12,  12,  12 },		// RIGHTUP
	{   0,   0,   0,   0 },		// UP
	{ -12, -12, -12, -12 }		// LEFTUP
};

int MCreature::m_cXTableFrame4[8][4] = 
{
	{  0,  0,  0,  0 },		// LEFT
	{  6,  6,  6,  6 },		// LEFTDOWN
	{  6,  6,  6,  6 },		// DOWN
	{  6,  6,  6,  6 },		// RIGHTDOWN
	{  0,  0,  0,  0 },		// RIGHT
	{ -6, -6, -6, -6 },		// RIGHTUP
	{ -6, -6, -6, -6 },		// UP
	{ -6, -6, -6, -6 }		// LEFTUP
};
*/

//------------------------------------------------------------------
// Slayer action 加档
//------------------------------------------------------------------
int MCreature::s_SlayerActionSpeed[ACTION_MAX_SLAYER][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },

	{ ACTION_SLAYER_GUN_SR_SLOW, ACTION_SLAYER_GUN_SR, ACTION_SLAYER_GUN_SR_FAST },
	{ ACTION_SLAYER_GUN_AR_SLOW, ACTION_SLAYER_GUN_AR, ACTION_SLAYER_GUN_AR_FAST },
	{ ACTION_SLAYER_SWORD_SLOW, ACTION_SLAYER_SWORD, ACTION_SLAYER_SWORD_FAST },
	{ ACTION_SLAYER_BLADE_SLOW, ACTION_SLAYER_BLADE, ACTION_SLAYER_BLADE_FAST },
	{ ACTION_SLAYER_THROW_WEAPON, ACTION_SLAYER_THROW_WEAPON, ACTION_SLAYER_THROW_WEAPON },
	{ ACTION_SLAYER_MOTOR_MOVE, ACTION_SLAYER_MOTOR_MOVE, ACTION_SLAYER_MOTOR_MOVE },
	{ ACTION_SLAYER_GUN_SG_SLOW, ACTION_SLAYER_GUN_SG, ACTION_SLAYER_GUN_SG_FAST },
	{ ACTION_SLAYER_GUN_SMG_SLOW, ACTION_SLAYER_GUN_SMG, ACTION_SLAYER_GUN_SMG_FAST },
	{ ACTION_SLAYER_SWORD_2_SLOW, ACTION_SLAYER_SWORD_2, ACTION_SLAYER_SWORD_2_FAST },
	{ ACTION_SLAYER_BLADE_2_SLOW, ACTION_SLAYER_BLADE_2, ACTION_SLAYER_BLADE_2_FAST },
	{ ACTION_SLAYER_THROW_POTION, ACTION_SLAYER_THROW_POTION, ACTION_SLAYER_THROW_POTION },
	{ ACTION_SLAYER_MOTOR_STAND, ACTION_SLAYER_MOTOR_STAND, ACTION_SLAYER_MOTOR_STAND },
};

//------------------------------------------------------------------
// vampire action 加档
//------------------------------------------------------------------
int MCreature::s_VampireActionSpeed[ACTION_MAX_SLAYER][3] = //ACTION_MAX_VAMPIRE][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_VAMPIRE_ATTACK_SLOW, ACTION_ATTACK, ACTION_VAMPIRE_ATTACK_FAST },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },

	{ ACTION_VAMPIRE_DRAIN, ACTION_VAMPIRE_DRAIN, ACTION_VAMPIRE_DRAIN },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },	
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK }
};

//------------------------------------------------------------------
// ousters action 加档
//------------------------------------------------------------------
int MCreature::s_OustersActionSpeed[ACTION_MAX_SLAYER][3] = //ACTION_MAX_OUSTERS][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_OUSTERS_ATTACK_SLOW, ACTION_ATTACK, ACTION_OUSTERS_ATTACK_FAST },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },
	
	{ ACTION_OUSTERS_STAND, ACTION_OUSTERS_STAND, ACTION_OUSTERS_STAND },
	{ ACTION_OUSTERS_MOVE, ACTION_OUSTERS_MOVE, ACTION_OUSTERS_MOVE },	
	{ ACTION_OUSTERS_CHAKRAM_SLOW, ACTION_OUSTERS_CHAKRAM, ACTION_OUSTERS_CHAKRAM_FAST },
	{ ACTION_OUSTERS_MAGIC_ATTACK, ACTION_OUSTERS_MAGIC_ATTACK, ACTION_OUSTERS_MAGIC_ATTACK },
	{ ACTION_OUSTERS_DRAIN, ACTION_OUSTERS_DRAIN, ACTION_OUSTERS_DRAIN },
	{ ACTION_OUSTERS_FAST_MOVE_STAND, ACTION_OUSTERS_FAST_MOVE_STAND, ACTION_OUSTERS_FAST_MOVE_STAND },
	{ ACTION_OUSTERS_FAST_MOVE_MOVE, ACTION_OUSTERS_FAST_MOVE_MOVE, ACTION_OUSTERS_FAST_MOVE_MOVE },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK }
};

//----------------------------------------------------------------------
// 茄 Tile 捞悼狼 Pixel荐
//----------------------------------------------------------------------
int MCreature::m_sXTable[MAX_DIRECTION] = 
{
	TILE_X, TILE_X, 0, -TILE_X, -TILE_X, -TILE_X, 0, TILE_X
};

int MCreature::m_sYTable[MAX_DIRECTION] = 
{
	0, -TILE_Y, -TILE_Y, -TILE_Y, 0, TILE_Y, TILE_Y, TILE_Y
};

int *MCreature::m_cXTable[MAX_FRAME_MOVE][MAX_DIRECTION] = { NULL, };
int *MCreature::m_cYTable[MAX_FRAME_MOVE][MAX_DIRECTION] = { NULL, };

//----------------------------------------------------------------------
// 规氢俊 蝶弗 函拳蔼
//----------------------------------------------------------------------
POINT g_DirectionValue[MAX_DIRECTION] =
{
	{ -1, 0 },
	{ -1, 1 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 },
	{ 1, -1 },
	{ 0, -1 },
	{ -1, -1 }
};

bool			IsCreatureMove( MCreature *pCreature )
{
	if( pCreature == NULL )
		return false;

	BYTE Action = pCreature->GetAction();
	if( Action == ACTION_MOVE )
		return true;

	if( pCreature->IsSlayer() )
	{		
		if( Action == ACTION_SLAYER_MOTOR_MOVE )
			return true;
	} else
	if( pCreature->IsVampire() )
	{				
	} else
	if( pCreature->IsOusters() )
	{
		if( Action == ACTION_OUSTERS_MOVE || Action == ACTION_OUSTERS_FAST_MOVE_MOVE )
			return true;
	}

	return false;
}

int ConvAdvancementSlayerActionFromSlayerAction( int CurAction, MCreatureWear* pCreatureWear );
int ConvAdvancementOustersActionFromOusterAction( int CurAction, bool bChakram );
int GetAdvancementVampireActionFromVampireAction( int CurAction, const MCreature* pCreature );

BYTE GetCreatureActionCountMax( const MCreature* pCreature, int action )
{
	bool bBat = pCreature->GetCreatureType() == CREATURETYPE_BAT;
	bool bWolf = pCreature->GetCreatureType() == CREATURETYPE_WOLF;
	bool bWerWolf = pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF;
	// 2005, 1, 7, sobeit add start
	bool bInstallTurret = pCreature->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
	// 2005, 1, 7, sobeit add end
	if( !pCreature->IsAdvancementClass() || g_pTopView == NULL || bBat || bWolf || bWerWolf || bInstallTurret)
	{
		return (*g_pCreatureTable)[pCreature->GetCreatureType()].GetActionCount( action );
	}

	BYTE* refActionMaxTable = NULL;
	switch( pCreature->GetRace() )
	{
	case RACE_SLAYER :
		action = ConvAdvancementSlayerActionFromSlayerAction( action, (MCreatureWear*)pCreature );
		refActionMaxTable = g_AdvanceSlayerActionMaxCount;
		break;
		
	case RACE_VAMPIRE :
		action = GetAdvancementVampireActionFromVampireAction( action, pCreature );
		refActionMaxTable = g_AdvanceVampireActionMaxCount;
		break;
		
	case RACE_OUSTERS :
		{
			MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;
			const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
			bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
			
			action = ConvAdvancementOustersActionFromOusterAction( action, bChakram );
			refActionMaxTable = g_AdvanceOustersActionMaxCount;
		}
		break;
	}
	
	action -= ADVANCEMENT_ACTION_START;

	if( refActionMaxTable != NULL && action >= 0 )
		return refActionMaxTable[ action ];
	
	return 0;
}

bool			IsCreatureActionAttack( MCreature *pCreature )
{
	if( pCreature == NULL )
		return false;

	BYTE Action = pCreature->GetAction();
	
	if( Action == ACTION_ATTACK || Action == ACTION_MAGIC )
		return true;

	if( pCreature->IsSlayer() )
	{
		if ( Action == ACTION_SLAYER_GUN_SR || Action == ACTION_SLAYER_GUN_AR ||
			Action == ACTION_SLAYER_SWORD || Action == ACTION_SLAYER_BLADE ||
			Action == ACTION_SLAYER_THROW_WEAPON || Action == ACTION_SLAYER_GUN_SG ||
			Action == ACTION_SLAYER_GUN_SMG || Action == ACTION_SLAYER_SWORD_2 ||
			Action == ACTION_SLAYER_BLADE_2	|| Action == ACTION_SLAYER_THROW_POTION	||
			(Action >= ACTION_SLAYER_GUN_SR_SLOW && Action <= ACTION_SLAYER_BLADE_2_FAST )
			)
			return true;
	} 
	else if( pCreature->IsOusters() )
	{
		if ( Action == ACTION_OUSTERS_CHAKRAM || Action == ACTION_OUSTERS_MAGIC_ATTACK ||
			Action == ACTION_OUSTERS_ATTACK_SLOW || Action == ACTION_OUSTERS_ATTACK_FAST ||
			Action == ACTION_OUSTERS_CHAKRAM_SLOW || Action == ACTION_OUSTERS_CHAKRAM_FAST )
			return true;
	}
	else if (pCreature->IsVampire() )
	{		
		if( Action == ACTION_VAMPIRE_ATTACK_FAST || Action == ACTION_VAMPIRE_ATTACK_SLOW)
			return true;
	}

	return false;
}

bool			HasEffectStatusSummonSylph( MCreature* pCreature )
{
	if( 
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_GREEN ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_ORANGE ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_RED ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_YELLOW ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_WHITE ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BROWN ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_LILAC ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BLACK ) 
		)
		return true;

	return false;
}

void			RemoveEffectStatusSummonSylph( MCreature* pCreature )
{
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_GREEN );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_ORANGE );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_RED );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_YELLOW );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_WHITE );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BROWN );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_LILAC );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BLACK );
}

//----------------------------------------------------------------------
// Init MoveTable
//----------------------------------------------------------------------
// cX,cYtable阑 檬扁拳茄促.
// [maxFrame辆幅8][8规氢][maxFrame俺 父怒狼 frame甸]
//----------------------------------------------------------------------
void
MCreature::InitMoveTable()
{
	for (int maxFrame=0; maxFrame<MAX_FRAME_MOVE; maxFrame++)
	{
		int maxFrame_plus_1	= maxFrame + 1;

		for (int d=0; d<MAX_DIRECTION; d++)
		{			
			// maxFrame俺 父怒狼 frame甸
			if (m_cXTable[maxFrame][d]!=NULL)
			{
				delete [] m_cXTable[maxFrame][d];			
			}
			m_cXTable[maxFrame][d] = new int [maxFrame_plus_1];

			if (m_cYTable[maxFrame][d]!=NULL)
			{
				delete [] m_cYTable[maxFrame][d];				
			}
			m_cYTable[maxFrame][d] = new int [maxFrame_plus_1];
			
												// maxFrame捞 8老 版快..
			int totalX	= -m_sXTable[d];				// 48
			int totalY	= -m_sYTable[d];				// 24

			int		pX	= 0, pY = 0;			// 官肺傈狼 x,y蔼
			float	fX	= 0, fY = 0;			// 泅犁鳖瘤 歹茄 x,y蔼
			float	cX	= (float)totalX / maxFrame_plus_1;	// 6
			float	cY	= (float)totalY / maxFrame_plus_1;	// 3

			// 阿 Frame阑 檬扁拳 矫挪促.
			// maxFrame俺甫 歹窍搁 totalX, totalY啊 登绢具 茄促.			
			for (int f=0; f<maxFrame; f++)
			{
				// 泅犁鳖瘤 歹茄 x, y蔼
				fX += cX;
				fY += cY;

				// 泅犁 蔼俊辑 捞傈 蔼阑 哗辑 瞒捞甫 备茄促.
				// fX甫 馆棵覆 窍绰巴档 宝满瘤 酵促.
				m_cXTable[maxFrame][d][f] = fX - pX;
				m_cYTable[maxFrame][d][f] = fY - pY;

				// 泅犁狼 蔼阑 扁撅秦敌促.
				pX = fX;
				pY = fY;
			}

			m_cXTable[maxFrame][d][maxFrame] = totalX - pX;
			m_cYTable[maxFrame][d][maxFrame] = totalY - pY;
		}		
	}	
}	

//----------------------------------------------------------------------
// Release MoveTable
//----------------------------------------------------------------------
// cX,cYTable阑 力芭茄促.	
//----------------------------------------------------------------------
void
MCreature::ReleaseMoveTable()
{
	for (int maxFrame=0; maxFrame<MAX_FRAME_MOVE; maxFrame++)
	{
		for (int d=0; d<MAX_DIRECTION; d++)
		{
			if (m_cXTable[maxFrame][d]!=NULL)
			{
				delete [] m_cXTable[maxFrame][d];
				m_cXTable[maxFrame][d] = NULL;
			}

			if (m_cYTable[maxFrame][d]!=NULL)
			{
				delete [] m_cYTable[maxFrame][d];
				m_cYTable[maxFrame][d] = NULL;
			}
		}
	}
}

		
//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MCreature::MCreature()
{
	m_PersonalShopOpenTime = 0;
	m_HeadSkin = 0;
	m_DelayLastAction = 0;
	m_ObjectType	= TYPE_CREATURE;

	m_CreatureType	= CREATURETYPE_NULL;

	m_CreatureFrameIDs.Init(0);
	m_bExistBody	= FALSE;

	m_MoveType = CREATURE_GROUND;

	// 框流捞绰 规过苞 弊俊 蝶弗 捞悼 窜困
	m_MoveDevice = MOVE_DEVICE_WALK;
	m_MoveAction = ACTION_MOVE;

	//m_ppCXTable = m_cXTableDeviceNull;
	//m_ppCYTable = m_cYTableDeviceNull;

	m_ColorBody1 = 0;
	m_ColorBody2 = 0;
	m_ChangeColorSet = ATTACHEFFECTCOLOR_NULL;	// 扁夯利栏肺 荤侩窍瘤 臼绰促.

	m_ID = OBJECTID_NULL;

	m_pZone = NULL;

	m_X = SECTORPOSITION_NULL;
	m_Y = SECTORPOSITION_NULL;
	
	m_sX = 0;	
	m_sY = 0;

	m_cX = 0;
	m_cY = 0;

	m_Action = ACTION_STAND;				// Action俊 措茄 ID
	m_Direction = DIRECTION_LEFTDOWN;			// 官扼毫具且 规氢
	m_DirectionMove = m_Direction;
	m_DirectionMoved = m_Direction;
	m_CurrentDirection = DIRECTION_LEFTDOWN;	// 泅犁官扼焊绰 规氢

	//  青悼	
	m_NextAction		= ACTION_STAND;


	m_ActionCount		= 0;				// Current Frame
	m_ActionCountMax	= 0;

	m_MoveCount			= 0;
	m_MoveCountMax		= 0;
	m_NextMoveCount		= 0;

	// 臭捞
	m_Z		= 0;
	
	//-------------------------------------------------------
	// 促澜 框流老 规氢	
	//-------------------------------------------------------
	m_NextX			= SECTORPOSITION_NULL;
	m_NextY			= SECTORPOSITION_NULL;
	m_NextDirection	= DIRECTION_NULL;

	//-------------------------------------------------------
	// 付瘤阜栏肺 八刘等 Sector狼 谅钎
	//-------------------------------------------------------
	m_ServerX	= SECTORPOSITION_NULL;
	m_ServerY	= SECTORPOSITION_NULL;


	//-------------------------------------------------------
	// Chatting String
	//-------------------------------------------------------
	m_ChatString = new char* [g_pClientConfig->MAX_CHATSTRING];
	for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i] = new char [g_pClientConfig->MAX_CHATSTRINGLENGTH_PLUS1];
	}
	
	m_ChatStringCurrent = 0;
	for (i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i][0] = NULL;
	}
	m_ChatColor = RGB_WHITE;//0xFFFF;
	m_OriChatColor = RGB_WHITE;

	m_ChatTime = 0;

	//-------------------------------------------------------
	// 扁贱 辆幅
	//-------------------------------------------------------
	m_nBasicActionInfo		= SKILL_ATTACK_MELEE;
	m_nSpecialActionInfo	= ACTIONINFO_NULL;
	m_nUsedActionInfo		= ACTIONINFO_NULL;

	//-------------------------------------------------------
	// 某腐磐俊 嘿绢 乐绰 Effect辆幅
	//-------------------------------------------------------
	m_bAttachEffect = new bool [(*g_pEffectSpriteTypeTable).GetSize()];
	for (i=0; i<(*g_pEffectSpriteTypeTable).GetSize(); i++)
	{
		m_bAttachEffect[i] = false;
	}

	m_bEffectStatus = new bool [(*g_pEffectStatusTable).GetSize()];
	for (i=0; i<(*g_pEffectStatusTable).GetSize(); i++)
	{
		m_bEffectStatus[i] = false;
	}

	m_AttachEffectColor = ATTACHEFFECTCOLOR_NULL;

//	m_MaxEffectLight = 0;	// 弥绊 灌扁

	//-------------------------------------------------------
	// 格钎俊 措茄 沥焊
	//-------------------------------------------------------
	SetTraceID( OBJECTID_NULL );
	m_TraceX				= SECTORPOSITION_NULL;
	m_TraceY				= SECTORPOSITION_NULL;
	m_TraceZ				= 0;

	// 蝴唱绰 Effect甸
	//m_nAlphaEffect = 0;

	// 弥辟俊 盲泼 String捞 眠啊等 矫痢
	m_NextChatFadeTime = g_CurrentTime;

	m_pActionResult = NULL;

	m_pName = NULL;

	m_bAlive = true;

	m_bNextAction = false;
		
	m_nNextUsedActionInfo = ACTIONINFO_NULL;

	m_ItemCount = 0;

	m_EffectDelayFrame = 0;

	m_InvisibleCount = 0;
	m_InvisibleStep = 0;

	//-------------------------------------------------------
	// HP
	//-------------------------------------------------------
	MStatus::ClearStatus();

	SetStatus( MODIFY_CURRENT_HP, 100 );
	SetStatus( MODIFY_MAX_HP, 100 );
	SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );

	m_LevelName = 0;

	m_GuildNumber = -1;
	m_nUnionGuildID = 0;

	m_bFastMove = false;
	m_bKnockBack = 0;

	m_RepeatCount = 0;


	m_WeaponSpeed = WEAPON_SPEED_NORMAL;

	
	m_bHasHead = true;

	m_RecoveryHPAmount = 0; 
	m_RecoveryHPTimes = 0;
	m_RecoveryHPNextTime = 0;
	m_RecoveryHPDelayTime = 0;	
	m_RecoveryMPAmount = 0; 
	m_RecoveryMPTimes = 0;
	m_RecoveryMPNextTime = 0;
	m_RecoveryMPDelayTime = 0;	
	//m_RecoveryPart = MODIFY_CURRENT_HP;

	m_NextBloodingTime  = 0;

	m_DelayActionInfo = ACTIONINFO_NULL;

	m_bMale = true;

	m_ShadowCount = 0;

	m_bInCasket = false;
	m_CasketCount = 0;
	m_CasketType = 0;

	m_bCutHeight = false;
	m_CutHeightCount = 0;
	m_CutHeightCountInc = 0;

	m_bStopBloodDrain = 0;
	m_bStopAbsorbSoul = 0;

	// -1捞搁 darkness俊 乐绰霸 酒聪促.
	m_DarknessCount = -1;
	m_DarknessCountInc = 0;

	m_bPlayerParty = false;

	m_bHallu = false;
//	m_HalluName = 0;
	m_HalluName = rand()%g_pMonsterNameTable->GetLastNameSize();

	m_RegenDelayTime = 0;
	m_RegenNextTime = 0;
	m_RegenAmount = 0;
	m_RegenBonusDelayTime = 0;
	m_RegenBonusNextTime = 0;
	m_RegenBonusAmount = 0;

	m_Competence = 3;

	m_bTurning = false;
	m_TurningCount = 0;

	m_bCauseCriticalWounds = false;
	m_CauseCriticalWoundsCount = 0;

	m_bBloodyZenith = false;
	m_BloodyZenithCount = 0;

	m_bGunShotGuidance = false;
	m_GunShotGuidanceCount = 0;
	m_fGhost = 0;
	m_GhostCount = 0;

	m_bFakeDie = false;
	m_FakeDieCount = 0;
	m_GradeActionInfo = 0;
	m_TraceTimer = 0;
	m_BatColorSet = 0xFFFF;

	m_PetID = OBJECTID_NULL;
	m_ElementalID = OBJECTID_NULL;

	m_bSiegeWar = 0;

	m_bBurningSolCount = 0;
	m_bBurningSol = false;
	m_current_sprite_index = 0;
	m_Input_Chat_Flag = false;
	m_IsSkipMotorStand = false;

	m_Persnal_flag = false;

	m_MasterEffectType = 0;
}

MCreature::~MCreature()
{
	//----------------------------------------
	// 脐 力芭
	//----------------------------------------
	if(GetPetID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetPetID());
	}

	//----------------------------------------
	// 沥飞 力芭
	//----------------------------------------
	if(GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}

	ClearAttachEffect();

	//-------------------------------------------------------
	// 捞抚 力芭
	//-------------------------------------------------------
	if (m_pName!=NULL)
	{
		delete [] m_pName;
		m_pName = NULL;
	}

	//-------------------------------------------------------
	// Chatting String
	//-------------------------------------------------------
	if (m_ChatString != NULL)
	{
		for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
		{
			delete [] m_ChatString[i];
		}

		delete [] m_ChatString;		
		m_ChatString = NULL;
	}

	//-------------------------------------------------------
	// effectspritetype
	//-------------------------------------------------------
	if (m_bAttachEffect != NULL)
	{
		delete [] m_bAttachEffect;
		m_bAttachEffect = NULL;
	}

	if (m_bEffectStatus != NULL)
	{
		delete [] m_bEffectStatus;
		m_bEffectStatus = NULL;
	}

	if (m_pActionResult!=NULL)
	{
		delete m_pActionResult;
		m_pActionResult = NULL;
	}
	
	ReleaseMoveBuffer();
}

bool IsCreatureTypeVampirePC(int type)
{
	return type == CREATURETYPE_VAMPIRE_MALE1 ||
		type == CREATURETYPE_VAMPIRE_MALE2 ||
		type == CREATURETYPE_VAMPIRE_MALE3 ||
		type == CREATURETYPE_VAMPIRE_FEMALE1 ||
		type == CREATURETYPE_VAMPIRE_FEMALE2 ||
		type == CREATURETYPE_VAMPIRE_FEMALE3 ||
		type == CREATURETYPE_WER_WOLF	||	
				// add by Coffee 2006.11.24  增加鬼族亚麻形象
		type == CREATURETYPE_VAMPIRE_FEMALE4 ||
		type == CREATURETYPE_VAMPIRE_MALE4	||
		//add by viva
		type == CREATURETYPE_VAMPIRE_MALE5	||
		type == CREATURETYPE_VAMPIRE_FEMALE5	||
		type == CREATURETYPE_VAMPIRE_MALE6	||
		type == CREATURETYPE_VAMPIRE_FEMALE6;

}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Creature Type
//----------------------------------------------------------------------
void	
MCreature::SetCreatureType(TYPE_CREATURETYPE type)
{ 
	if (type < (*g_pCreatureTable).GetSize())
	{

	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] CreatureType is Wrong. type=%d", (int)type);
		
		type = 0;
	}
//	type = 812;//add by viva
	m_CreatureType		= type;   

	m_ActionCount		= 0;
	m_ActionCountMax	= 0; 
	m_MoveCount			= 0;
	m_MoveCountMax		= 0; 
	m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
	m_sX=0; 
	m_sY=0;
	
	// 促澜 悼累档 绝沮
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

//	int spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0];
//
//	m_CreatureFrameID	= (*g_pCreatureSpriteTable)[spriteType].FrameID;
//	m_bExistBody		= m_CreatureFrameID!=FRAMEID_NULL;

		//by csm
	int spriteType;
	m_CreatureFrameIDs.Init((*g_pCreatureTable)[m_CreatureType].SpriteTypes.GetSize());
	for(int framecnt = 0;  framecnt < (*g_pCreatureTable)[m_CreatureType].SpriteTypes.GetSize(); framecnt++ )
	{
		spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[framecnt];
		m_CreatureFrameIDs[framecnt]	= (*g_pCreatureSpriteTable)[spriteType].FrameID;
	}
	
	
	m_bExistBody		= m_CreatureFrameIDs[0]!=FRAMEID_NULL;


	
	// 祸彬 - 各牢 版快父.. 汲沥秦霖促.
	if(!IsCreatureTypeVampirePC(m_CreatureType))
	{
		m_ColorBody1		= (*g_pCreatureTable)[m_CreatureType].ColorSet;
		m_ColorBody2		= m_ColorBody1;
	}
	
	// 绢涤霸 嘛扁 力芭
	m_bFade				= false;

	// 盔贰 农府媚 鸥涝捞 官拆锭 捞棋飘甫 朝府档废 沁促.
	// 捞绰 浇饭捞绢<->轨颇捞绢狼 函脚矫父 窍档废 窍鲤
	// 嘿篮 effect 力芭
//	ClearAttachEffect();

	//by csm
	spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0];
	//---------------------------------------------------------------------------
	// 扁贱 汲沥
	//---------------------------------------------------------------------------
	// 扁夯 傍拜 悼累捞 绝绰 版快..
	if (!(*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite()
		//&& (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_ATTACK )==0)
		&& GetCreatureActionCountMax( this, ACTION_ATTACK )==0)
	{
		m_nBasicActionInfo = ACTIONINFO_NULL;
	}
	else
	{
		m_nBasicActionInfo = SKILL_ATTACK_MELEE;
	}

//	m_nSpecialActionInfo	= ACTIONINFO_NULL;
	m_nUsedActionInfo		= ACTIONINFO_NULL;

	//m_WeaponSpeed			= WEAPON_SPEED_NORMAL;

	//---------------------------------------------------------------------------
	// 肯傈洒 祸彬捞 官差绰 各牢 版快
	//---------------------------------------------------------------------------
	int changeColor = (*g_pCreatureTable)[m_CreatureType].ChangeColorSet;
	if (changeColor < MAX_COLORSET)
	{
		m_ChangeColorSet = changeColor;
		m_AttachEffectColor = changeColor;
	}
	else
	{
		// ATTACHEFFECTCOLOR_NULL捞搁 沥惑利牢 祸彬捞促.

		if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
		{
			m_ChangeColorSet = g_ZoneCreatureColorSet;//ATTACHEFFECTCOLOR_NULL;
			m_AttachEffectColor = g_ZoneCreatureColorSet;//ATTACHEFFECTCOLOR_NULL;
		}
		else
		{
			m_ChangeColorSet = ATTACHEFFECTCOLOR_NULL;
			m_AttachEffectColor = ATTACHEFFECTCOLOR_NULL;
		}
	}

	//---------------------------------------------------------------------------
	// 扁夯(?)利栏肺 己喊 汲沥..
	// 戳措唱 冠零牢 版快绰 creaturetype栏肺 己喊阑 舅 荐啊 绝绢辑府..
	//---------------------------------------------------------------------------
	if( m_CreatureType != CREATURETYPE_WER_WOLF )
		m_bMale = (*g_pCreatureTable)[m_CreatureType].bMale;

	//---------------------------------------------------------------------------
	// 个 儡惑
	//---------------------------------------------------------------------------
	m_ShadowCount = (*g_pCreatureTable)[m_CreatureType].ShadowCount;

	m_bInCasket = false;
	m_CasketCount = 0;

	//---------------------------------------------------------------------------
	// Effect 力芭
	//---------------------------------------------------------------------------
	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_BAT)
		&& m_CreatureType!=CREATURETYPE_BAT)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_BAT);
	}

	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WOLF)
		&& m_CreatureType!=CREATURETYPE_WOLF)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WOLF);
	}

	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WERWOLF)
		&& m_CreatureType != CREATURETYPE_WER_WOLF)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WERWOLF);
	}

	// 2004, 9, 13, sobeit add start - 醚浇 130 胶懦
	if (HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
		&& m_CreatureType != CREATURETYPE_INSTALL_TURRET)
	{
		RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
	}
	// 2004, 9, 13, sobeit add end - 醚浇 130 胶懦
	
	// Weapon Speed甫 促矫 汲沥秦霖促.
	int attackSpeed = m_Status[MODIFY_ATTACK_SPEED];

	// 0捞搁 蔼捞 汲沥捞 救灯促绊 啊沥茄促. 漂洒, 各牢 版快捞促.
	if (attackSpeed!=0)	
	{
		SetWeaponSpeed( attackSpeed );
	}

	CheckRegen();
}

//----------------------------------------------------------------------
// Set BodyColor1
//----------------------------------------------------------------------
void		
MCreature::SetBodyColor1(WORD set)		
{ 
	m_ColorBody1 = set; 
}

//----------------------------------------------------------------------
// Set BodyColor2
//----------------------------------------------------------------------
void		
MCreature::SetBodyColor2(WORD set)		
{ 
	m_ColorBody2 = set; 

	if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR
		|| m_CreatureType==CREATURETYPE_SLAYER_OPERATOR
		|| m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR)
	{
		m_ColorBody1 = set;
	}
}

//----------------------------------------------------------------------
// Clear AttachEffect
//----------------------------------------------------------------------
void
MCreature::ClearAttachEffect()
{
	//-------------------------------------------------------
	// Creature俊 嘿绢 乐绰 Effect甫 皋葛府俊辑 力芭茄促.
	//-------------------------------------------------------
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	while (iEffect != m_listEffect.end())
	{
		MEffect* pEffect = *iEffect;
		delete pEffect;
		
		iEffect++;
	}

	m_listEffect.clear();

	//-------------------------------------------------------
	// 个俊 嘿绰 Effect
	//-------------------------------------------------------
	iEffect = m_listGroundEffect.begin();

	while (iEffect != m_listGroundEffect.end())
	{
		MEffect* pEffect = *iEffect;
		delete pEffect;
		
		iEffect++;
	}

	m_listGroundEffect.clear();

	//-------------------------------------------------------
	// 眉农 蔼 力芭
	//-------------------------------------------------------
	if (g_pEffectSpriteTypeTable!=NULL)
	{
		for (int i=0; i<(*g_pEffectSpriteTypeTable).GetSize(); i++)
		{
			m_bAttachEffect[i] = false;
		}
	}

	if (g_pEffectStatusTable!=NULL)
	{
		for (int i=0; i<(*g_pEffectStatusTable).GetSize(); i++)
		{
			m_bEffectStatus[i] = false;
		}	
	}

	UnSetHalluCreature();
}
void
MCreature::RemoveGlacierEffect()
{
	SetStop();
	StopCauseCriticalWounds();
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	bool re = false;
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_END == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_END == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_END == Type 

			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 皋葛府 力芭
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list俊辑 力芭
			m_listEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	m_bEffectStatus[EFFECTSTATUS_GLACIER]  = false;
}

void
MCreature::Remove_Curse_Paralsis_Effect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	EFFECTSPRITETYPE_PURPLE_WALL_2 == Type )
		{
			pEffect->SetCount( 0, 0 );
		}
		iEffect++;
	}
}

//----------------------------------------------------------------------
// Remove EffectStatus
//----------------------------------------------------------------------
// 漂沥茄 EffectSpriteType阑 力芭茄促.
//----------------------------------------------------------------------
bool
MCreature::RemoveEffectStatus(EFFECTSTATUS status)
{
	DEBUG_ADD_FORMAT("[RemoveEffectStatus] id=%d, status=%d", m_ID, (int)status);
	
	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	if (!m_bEffectStatus[status])
	{
		// [货扁贱7]
		DEBUG_ADD_FORMAT("NotEffectStatus(%d)", (int)status);
		return false;
	}

	m_bEffectStatus[status] = false;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;

	int pTypes = (*g_pEffectStatusTable)[status].EffectSpriteType;
	//------------------------------------------------------------
	// effectStatus俊 蝶扼辑.
	//------------------------------------------------------------
	switch (status)
	{
		case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
		case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
		case EFFECTSTATUS_SUMMON_SYLPH_RED :
		case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
		case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
		case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
		case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
		case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
		case EFFECTSTATUS_SUMMON_SYLPH:
			SetMoveDevice(MOVE_DEVICE_WALK);
			if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
			{
				pTypes = EFFECTSPRITETYPE_OUSTERS_FASTMOVE_LOOP;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_END,
					GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, 8, NULL, false);
			}
		break;
		case EFFECTSTATUS_BIKE_CRASH:
			SetMoveDevice(MOVE_DEVICE_WALK);
		break;

		//------------------------------------------------------------
		// 付厚 钱副 锭
		//------------------------------------------------------------
		case EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS :
		case EFFECTSTATUS_EXPLOSION_WATER :
			if (m_bAlive)
			{
				SetStop();
				StopCauseCriticalWounds();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[CCW] Stop");
	#endif
			}			
		break;

		case EFFECTSTATUS_CURSE_PARALYSIS :
			if (m_bAlive)
			{
				SetStop();			
				Remove_Curse_Paralsis_Effect();
			}			
		break;

		//------------------------------------------------------------
		// 付厚 钱副 锭
		//------------------------------------------------------------
		// add by Coffee 2007-3-21
		case EFFECTSTATUS_SATELLITE_BOMB_AIM :
		// end	
		case EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM :
			if (m_bAlive)
			{
				SetStop();
				StopGunShotGuidance();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[GSG] Stop");
	#endif
			}			
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [货扁贱]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// 官肺 invisible肺 父电促.
			RemoveCasket();
		break;


		//------------------------------------------------------------
		// Invisible 力芭
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
		case EFFECTSTATUS_SNIPPING_MODE :	
			// invisible阑 力芭茄促.
			SetVisible();
		break;

		//------------------------------------------------------------
		// COMA 力芭
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetAlive();
			if(IsVampire())
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_RESURRECT,										// 荤侩 扁贱 锅龋
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 荤侩 规氢
							
							m_ID,												// 格钎俊 措茄 沥焊
							m_X, m_Y, 0,
							
							2*16, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
		break;

		case EFFECTSTATUS_GHOST:
			if (!(*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 冠零牢 版快
				SetGroundCreature();
			break;
		case EFFECTSTATUS_WILL_OF_LIFE :
			if( IsVampire() )
				CheckRegen();
			break;

		case EFFECTSTATUS_GLACIER :
			RemoveGlacierEffect();
			break;
		case EFFECTSTATUS_SIEGE_DEFENDERL:
		case EFFECTSTATUS_SIEGE_REINFORCE:
		case EFFECTSTATUS_SIEGE_ATTACKER_1:
		case EFFECTSTATUS_SIEGE_ATTACKER_2:
		case EFFECTSTATUS_SIEGE_ATTACKER_3:
		case EFFECTSTATUS_SIEGE_ATTACKER_4:
		case EFFECTSTATUS_SIEGE_ATTACKER_5:
			SetSiegeWar(0);
			break;
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_1:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_2:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_3:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_4:
//			StopBurningSol();
//			break;
		case EFFECTSTATUS_INSTALL_TURRET:
			ExecuteActionInfoFromMainNode(RESULT_SKILL_INSTALL_TURRET,GetX(), GetY(), 0,0,GetID()	,	
				GetX(), GetY(), 0, 0, NULL, false);
			SetAction( ACTION_STAND );
			SetInstallTurretCount(0);
			SetInstallTurretDirect(2);
			SetDirection( 2 );
			SetCurrentDirection( 2 );
			break;

		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				pTypes = GetMasterEffectType(status);
			}
			break;
	}


	//------------------------------------------------------------
	//
	//			EffectSprite肺 钎泅窍绰 版快
	//
	//------------------------------------------------------------
	if (bUseEffectSprite)
	{
		int numTypes = 1;
//		int* pTypes = NULL;
		
//		if (status==EFFECTSTATUS_SPIRIT_GUARD_1 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_2 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_3 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_4)
//		{
//			numTypes = EFFECTSTATUS_SPIRIT_GUARD_4 - EFFECTSTATUS_SPIRIT_GUARD_1 +1;
//
//			pTypes = new int[numTypes];
//
//			for(int i = 0; i < numTypes; i++)
//				pTypes[i] = EFFECTSPRITETYPE_SPIRIT_GUARD;
//		}
//		else
//		if (status==EFFECTSTATUS_ARMAGEDDON)
//		{			
//			numTypes = 4;
//			pTypes = new int[numTypes];
//
//			pTypes[0] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_0_2;
//			pTypes[1] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_1_2;
//			pTypes[2] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_2_2;
//			pTypes[3] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_3_2;
//		}
//		else
		{
			numTypes = 1;
//			pTypes = new int[numTypes];

			//pTypes[0] = (*g_pEffectStatusTable)[status].EffectSpriteType;
			
		}		

		//for (int i=0; i<numTypes; i++)
		{
		//	TYPE_EFFECTSPRITETYPE type = pTypes[i];
			TYPE_EFFECTSPRITETYPE type = pTypes;
			TYPE_EFFECTSPRITETYPE type2 = EFFECTSPRITETYPE_NULL;

			if(type == EFFECTSPRITETYPE_FAST_MOVE_STOP)
				type2 = EFFECTSPRITETYPE_FAST_MOVE_FLY;
			
			if( type >= EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_STOP &&
				type <= EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP &&
				((type - EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_STOP)&0x1) == 0
				)
				type2 = type - 1;

			//-------------------------------------------------------
			// Creature俊 嘿绢 乐绰 Effect吝俊辑
			// EffectSpriteType捞 type牢 巴阑 力芭茄促.
			//-------------------------------------------------------
			if (type < g_pEffectSpriteTypeTable->GetSize())
			{
				if (IsFemale()
					&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
				{
					type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;
				}
				

				if (m_bAttachEffect[type])
				{
					//-------------------------------------------------------
					// 个俊 嘿篮芭
					//-------------------------------------------------------
					ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

					bool re = false;
					while (iEffect != m_listEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// 鞍篮 type阑 茫绰促.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() 
							&& (pEffect->GetEffectSpriteType() == type || type2 != EFFECTSPRITETYPE_NULL &&
							type2 == pEffect->GetEffectSpriteType()) )
						{
							// 皋葛府 力芭
							delete pEffect;

							ATTACHEFFECT_LIST::iterator dEffect = iEffect;
							iEffect--;

							// list俊辑 力芭
							m_listEffect.erase( dEffect );

							m_bAttachEffect[type] = false;
					
							re = true;
						}
						
						iEffect++;

					}

					if(re)
					{
					//	delete [] pTypes;
						return true;
					}

					//-------------------------------------------------------
					// 官蹿俊 嘿篮 芭
					//-------------------------------------------------------
					iEffect = m_listGroundEffect.begin();

					while (iEffect != m_listGroundEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// 鞍篮 type阑 茫绰促.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type
							|| type2 != EFFECTSPRITETYPE_NULL && type2 == pEffect->GetEffectSpriteType() )
						{
							// 皋葛府 力芭
							delete pEffect;

							// list俊辑 力芭
							m_listGroundEffect.erase( iEffect );

							m_bAttachEffect[type] = false;
					
						//	delete [] pTypes;
							return true;
						}
						
						iEffect++;
					}
				}
			}
		}

	//	delete [] pTypes;
	}
	//------------------------------------------------------------
	//
	//			EffectColor肺 钎泅窍绰 版快
	//
	//------------------------------------------------------------
	else
	{
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		//-------------------------------------------------------
		// Creature俊 嘿绢 乐绰 Effect吝俊辑
		// EffectColor啊 color牢 巴阑 力芭茄促.
		//-------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		while (iEffect != m_listEffect.end())
		{
			MAttachEffect* pEffect = *iEffect;
			
			//-------------------------------------------------------
			// 鞍篮 type阑 茫绰促.
			//-------------------------------------------------------
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// 皋葛府 力芭
				delete pEffect;

				// list俊辑 力芭
				m_listEffect.erase( iEffect );

				return true;
			}
			
			iEffect++;
		}		

		// GroundEffect绰 绝促绊 夯促.
	}
	
	return false;
}

//----------------------------------------------------------------------
// Add EffectStatus
//----------------------------------------------------------------------
// 
// 漂喊茄 惑怕甫 唱鸥郴绰 effect
//
// Creature俊 崔扼嘿绢辑 钎泅登绰 Effect 钎泅
//
// 吝酶阑 倾侩窍瘤 臼栏骨肺.. 
// 酒流 嘿绢辑 钎泅登瘤 臼篮 Effect父 货肺 眠啊茄促.
//
// 弊矾唱, 矫埃 犬厘阑 秦具窍绰 版快啊 积变促.
//----------------------------------------------------------------------
bool
MCreature::AddEffectStatus(enum EFFECTSTATUS status, DWORD delayFrame)
{
	DEBUG_ADD_FORMAT("[AddEffectStatus] id=%d, status=%d, delay=%d", m_ID, (int)status, delayFrame);
//	if( status == EFFECTSTATUS_VIEW_HP )
//	{
//		int a = 0;
//	}
	
	//----------------------------------------------------------
	// 捞固 磷篮 版快
	//----------------------------------------------------------
	if (IsDead() && status != EFFECTSTATUS_COMA && 
		!(
		GetCreatureType() >= 371 && GetCreatureType() <= 376 || GetCreatureType() >= 560 && GetCreatureType() <= 563) && 
		GetCreatureType() != 482 && GetCreatureType() != 650 && !(GetCreatureType() >= 526 && GetCreatureType() <= 549) &&
		GetCreatureType() != 670 && GetCreatureType() != 672 && GetCreatureType() != 673
		)	// 己拱牢 版快绰 捞棋飘 嘿烙
	{
		DEBUG_ADD("Don't AddEffectStatus : Already Dead");
		
		return false;
	}

	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	int eCount = 0; // 捞棋飘狼 俺荐
	int orbit_type = 0;

	//------------------------------------------------------------
	// effectStatus俊 蝶扼辑.
	//------------------------------------------------------------
	MEffect::EFFECT_TYPE eType = MEffect::EFFECT_ATTACH;
	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
	switch (status)
	{
	case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
	case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
	case EFFECTSTATUS_SUMMON_SYLPH_RED :
	case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
	case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
	case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
	case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
	case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
	case EFFECTSTATUS_SUMMON_SYLPH:
		{
			SetMoveDevice(MOVE_DEVICE_SUMMON_SYLPH);
			/********  Edit By sonic 去除魔灵二转后变球效果********/
			
			if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
			{
				type = EFFECTSPRITETYPE_OUSTERS_FASTMOVE_LOOP;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_START,GetX(), GetY(), 0,GetDirection(),	GetID(),	
				GetX(), GetY(), 0, 8, NULL, false);
			}
			/********End *******************************************/
		}
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_INVISIBILITY
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
			// 官肺 invisible肺 父电促.
			SetInvisibleSoon();
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [货扁贱]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// 官肺 invisible肺 父电促.
			AddCasketSoon(0);
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM
		//------------------------------------------------------------

		// add by Coffee 2007-3-21
		case EFFECTSTATUS_SATELLITE_BOMB_AIM :
		// end		
		case EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM :
#ifdef OUTPUT_DEBUG					
				DEBUG_ADD_FORMAT("[GSG] Set");
#endif
//				AffectMoveBufferAll();
//				ActionMoveNextPosition();
			if(!IsVampire())
			{
				SetAction((*g_pActionInfoTable)[m_nBasicActionInfo].GetAction());
				m_ActionCount = 0;
				SetGunShotGuidance(delayFrame);
			}
			else
			{
				return false;
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_CURSE_PARALYSIS
		//------------------------------------------------------------
		case EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS :
#ifdef OUTPUT_DEBUG					
				DEBUG_ADD_FORMAT("[CCW] Set");
#endif
//				AffectMoveBufferAll();
//				ActionMoveNextPosition();
			// CauseCriticalWounds牢 版快 浇饭捞绢绰 AddEffect甫 窍瘤 臼绰促. 捞棋飘 嘿捞瘤 臼绊 单固瘤父
			// 2005, 1, 18, sobeit modify start // g_pZone->GetID() == 4002 眠啊 - 涅胶飘 cause_critical_wounds吧妨乐绰 浇饭捞绢 npc
			if(!IsSlayer() || GetCreatureType() == 793) 
			// 2005, 1, 18, sobeit modify end
			{
				SetAction(ACTION_DRAINED);
				m_ActionCount = 0;
				SetCauseCriticalWounds(delayFrame);
			}
			else
			{
				SetAction(ACTION_DAMAGED);
				return false;
			}
			break;

		case EFFECTSTATUS_EXPLOSION_WATER:
			SetAction(ACTION_DRAINED);
			m_ActionCount = 0;
			SetCauseCriticalWounds(delayFrame);
			break;
			
		case EFFECTSTATUS_CURSE_PARALYSIS :
			// 促 捞悼矫挪促.
			AffectMoveBufferAll();
			
			ActionMoveNextPosition();
		break;

		case EFFECTSTATUS_FIRE_ELEMENTAL:
			if(GetClassType() == CLASS_FAKE)
			{
				delayFrame = 0xFFFF;
				eType = MEffect::EFFECT_ATTACH_ORBIT;
				eCount = 1;
				orbit_type = 2;
			}
			else
			{
				AddOustersElemental( this, 699, status );
				delayFrame = 0;
			}
			break;

		case EFFECTSTATUS_FIRE_ELEMENTAL_ATTACK:
			if(GetClassType() == CLASS_FAKE)
			{
//				ClearAttachEffect();
				delayFrame = 0;
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_FIRE_ELEMENTAL_ATTACK,										// 荤侩 扁贱 锅龋
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 荤侩 规氢
							
							m_ID,												// 格钎俊 措茄 沥焊
							m_X, m_Y, 0,
							
							0xFFFF, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
			else
			{
				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetElementalID());
				if(pFakeCreature != NULL)
				{
					pFakeCreature->AddEffectStatus(status, 0xFFFF);
				}
				return false;
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL:
			if(GetClassType() == CLASS_FAKE)
			{
				eType = MEffect::EFFECT_ATTACH_ORBIT;
				eCount = 1;
				orbit_type = 2;
				delayFrame = 0xFFFF;
			}
			else
			{
				AddOustersElemental( this, 699, status );
				delayFrame = 0;
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL_HEAL:
			if(GetClassType() == CLASS_FAKE)
			{
//				ClearAttachEffect();
				delayFrame = 0;
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_WATER_ELEMENTAL_HEAL,										// 荤侩 扁贱 锅龋
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 荤侩 规氢
							
							m_ID,												// 格钎俊 措茄 沥焊
							m_X, m_Y, 0,
							
							0xFFFF, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
			else
			{
				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetElementalID());
				if(pFakeCreature != NULL)
				{
					pFakeCreature->AddEffectStatus(status, 0xFFFF);
				}
				return false;
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_COMA
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetDead();
		break;

		case EFFECTSTATUS_FIRE_ELEMENTAL_DAMAGED:
			{
				TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
				if(type < g_pEffectSpriteTypeTable->GetSize())
				{
					TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;
					BYTE			maxFrame;
					
					// BLT_NORMAL		
					BLT_TYPE bltType = (*g_pEffectSpriteTypeTable)[type].BltType;
					maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

					delayFrame = maxFrame;
				}
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL_HEALED:
			{
				TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
				if(type < g_pEffectSpriteTypeTable->GetSize())
				{
					TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;
					BYTE			maxFrame;
					
					// BLT_NORMAL		
					BLT_TYPE bltType = (*g_pEffectSpriteTypeTable)[type].BltType;
					maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

					delayFrame = maxFrame;
				}
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_ARMAGEDDON
		//------------------------------------------------------------
		case EFFECTSTATUS_ARMAGEDDON :
		case EFFECTSTATUS_GROUND_ELEMENTAL_CENTER:
			delayFrame = 0xFFFF;		// 公茄..
		break;

		//------------------------------------------------------------
		// 己拱 包访
		//------------------------------------------------------------
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI :         // 151			
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA :         // 152				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS :         // 153				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE :         // 154				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA :         // 155				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA :         // 156				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_INI :         // 157				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE :         // 158				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA :         // 159				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA :         // 160				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO :         // 161				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL :         // 162						
		case EFFECTSTATUS_SHRINE_GUARD_GREGORI :				// 163				
		case EFFECTSTATUS_SHRINE_GUARD_NEMA :				// 164				
		case EFFECTSTATUS_SHRINE_GUARD_LEGIOS :				// 165				
		case EFFECTSTATUS_SHRINE_GUARD_MIHOLE :				// 166				
		case EFFECTSTATUS_SHRINE_GUARD_AROSA :				// 167				
		case EFFECTSTATUS_SHRINE_GUARD_ARMEGA :				// 168				
		case EFFECTSTATUS_SHRINE_GUARD_INI :				// 169				
		case EFFECTSTATUS_SHRINE_GUARD_JAVE :				// 170				
		case EFFECTSTATUS_SHRINE_GUARD_CHASPA :				// 171				
		case EFFECTSTATUS_SHRINE_GUARD_CONCILIA :				// 172				
		case EFFECTSTATUS_SHRINE_GUARD_KIRO :				// 173				
		case EFFECTSTATUS_SHRINE_GUARD_HILLEL :				// 174						
		case EFFECTSTATUS_SHRINE_HOLY_GREGORI :				// 175				
		case EFFECTSTATUS_SHRINE_HOLY_NEMA :				// 176				
		case EFFECTSTATUS_SHRINE_HOLY_LEGIOS :				// 177				
		case EFFECTSTATUS_SHRINE_HOLY_MIHOLE :				// 178				
		case EFFECTSTATUS_SHRINE_HOLY_AROSA :				// 179				
		case EFFECTSTATUS_SHRINE_HOLY_ARMEGA :				// 180				
		case EFFECTSTATUS_SHRINE_HOLY_INI :				// 181				
		case EFFECTSTATUS_SHRINE_HOLY_JAVE :				// 182				
		case EFFECTSTATUS_SHRINE_HOLY_CHASPA :				// 183				
		case EFFECTSTATUS_SHRINE_HOLY_CONCILIA :			// 184				
		case EFFECTSTATUS_SHRINE_HOLY_KIRO :			// 185				
		case EFFECTSTATUS_SHRINE_HOLY_HILLEL :			// 186				
		case EFFECTSTATUS_SHRINE_SHIELD:
		case EFFECTSTATUS_SLAYER_RELIC:
//		case EFFECTSTATUS_DROP_BLOOD_BIBLE:			
		case EFFECTSTATUS_VAMPIRE_RELIC:
			delayFrame = 0xFFFF;		// 公茄..
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_CURE_CRITICAL_WOUNDS
		//------------------------------------------------------------
		case EFFECTSTATUS_CURE_CRITICAL_WOUNDS :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 1;
			orbit_type = 0;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_1 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 1;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_2 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 2;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_3 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 3;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_4 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 4;
			orbit_type = 1;
			break;

		case EFFECTSTATUS_HALLUCINATION:
			{
				if(this == g_pPlayer)
				{
					MEvent event;
					event.eventID = EVENTID_HALLUCINATION;
					event.eventType = EVENTTYPE_EFFECT;
					event.eventFlag = EVENTFLAG_FADE_SCREEN;
					event.eventDelay = delayFrame/g_pClientConfig->FPS*1000;
					event.parameter1 = EFFECTSTATUS_HALLUCINATION;
					event.parameter2 = (30 << 16) | (0 << 8) | 30;	// alpha depth, r, g, b
					g_pEventManager->AddEvent(event);
				}
			}
			break;

		case EFFECTSTATUS_GHOST:
			SetFlyingCreature();
			break;

		case EFFECTSTATUS_LEVELUP_SLAYER:
			delayFrame = 30;
			PlaySound(SOUND_SLAYER_ENCHANT_PFA);
			break;

		case EFFECTSTATUS_DISTANCE_BLITZ:
			delayFrame = 10;
			break;

		case EFFECTSTATUS_LEVELUP_VAMPIRE:
			delayFrame = 24;
			PlaySound(SOUND_SLAYER_ENCHANT_PFA);
			break;

		case EFFECTSTATUS_WATER_SHIELD:
			delayFrame = 12;
			break;

		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_ME :// 195 郴啊 啊瘤绊乐带 乔狼己辑啊 况橇瞪锭 赣府俊 嘛绢林绰-_- 捞棋飘			
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_TILE :		// 196 顶俊 冻绢廉乐带 乔狼己辑啊 况橇瞪锭 弊 鸥老俊 嘛绢林绰 捞棋飘
		case EFFECTSTATUS_SHRINE_GUARD_WARP :			// 187		荐龋己窜俊辑 况橇瞪锭
		case EFFECTSTATUS_SHRINE_HOLY_WARP :			// 188		己瘤己窜俊辑 况橇瞪锭
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_SLAYER:
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_VAMPIRE:
			delayFrame = 14;				
			break;
		case EFFECTSTATUS_WILL_OF_LIFE :
			delayFrame = 84;			
			break;
		case EFFECTSTATUS_ILLUSION_OF_AVENGE :
			delayFrame = 32;
			break;
		case EFFECTSTATUS_DENIAL_MAGIC_DAMAGED :
			delayFrame = 14;
			break;
		case EFFECTSTATUS_BACK_STAB :
		case EFFECTSTATUS_BACK_STAB_2 :
			delayFrame = 12;
			break;
		case EFFECTSTATUS_FIRE_OF_SOUL_STONE :
			delayFrame = 16;
			break;
		case EFFECTSTATUS_SAND_OF_SOUL_STONE :
			delayFrame = 10;
			break;
		case EFFECTSTATUS_HAS_FLAG :
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_FLAG_INSERT :
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_STONE_SKIN_DAMAGED :
			delayFrame = 8;
			break;
		case EFFECTSTATUS_HAS_SWEEPER :                       // 292
		case EFFECTSTATUS_HAS_SWEEPER_2 :                     // 293
		case EFFECTSTATUS_HAS_SWEEPER_3 :                     // 294
		case EFFECTSTATUS_HAS_SWEEPER_4 :                     // 295
		case EFFECTSTATUS_HAS_SWEEPER_5 :                     // 296
		case EFFECTSTATUS_HAS_SWEEPER_6 :                     // 297
		case EFFECTSTATUS_HAS_SWEEPER_7 :                     // 298
		case EFFECTSTATUS_HAS_SWEEPER_8 :                     // 299
		case EFFECTSTATUS_HAS_SWEEPER_9 :                     // 300
		case EFFECTSTATUS_HAS_SWEEPER_10 :                    // 301
		case EFFECTSTATUS_HAS_SWEEPER_11 :                    // 302
		case EFFECTSTATUS_HAS_SWEEPER_12 :                    // 303
		case EFFECTSTATUS_KEEP_SWEEPER :                      // 304
		case EFFECTSTATUS_KEEP_SWEEPER_2 :                    // 305
		case EFFECTSTATUS_KEEP_SWEEPER_3 :                    // 306
		case EFFECTSTATUS_KEEP_SWEEPER_4 :                    // 307
		case EFFECTSTATUS_KEEP_SWEEPER_5 :                    // 308
		case EFFECTSTATUS_KEEP_SWEEPER_6 :                    // 309
		case EFFECTSTATUS_KEEP_SWEEPER_7 :                    // 310
		case EFFECTSTATUS_KEEP_SWEEPER_8 :                    // 311
		case EFFECTSTATUS_KEEP_SWEEPER_9 :                    // 312
		case EFFECTSTATUS_KEEP_SWEEPER_10 :                   // 313
		case EFFECTSTATUS_KEEP_SWEEPER_11 :                   // 314
		case EFFECTSTATUS_KEEP_SWEEPER_12 :                   // 315			
		case EFFECTSTATUS_SLAYER_REGEN_ZONE :                 // 316
		case EFFECTSTATUS_VAMPIRE_REGEN_ZONE :                // 317
		case EFFECTSTATUS_OUSTERS_REGEN_ZONE :                // 318
		case EFFECTSTATUS_DEFAULT_REGEN_ZONE :                // 319
		case EFFECTSTATUS_SLAYER_TRYING_1 :                   // 320
		case EFFECTSTATUS_SLAYER_TRYING_2 :                   // 321
		case EFFECTSTATUS_SLAYER_TRYING_3 :                   // 322
		case EFFECTSTATUS_VAMPIRE_TRYING_1 :                  // 323
		case EFFECTSTATUS_VAMPIRE_TRYING_2 :                  // 324
		case EFFECTSTATUS_VAMPIRE_TRYING_3 :                  // 325
		case EFFECTSTATUS_OUSTERS_TRYING_1 :                  // 326
		case EFFECTSTATUS_OUSTERS_TRYING_2 :                  // 327
		case EFFECTSTATUS_OUSTERS_TRYING_3 :                  // 328
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_CAPTURE_REGEN_ZONE :                // 331
			delayFrame = 36;
			break;

		// 2004, 6 ,12 sobeit add start - 130,150 付胶磐 捞棋飘矫 弊傈 付胶磐 捞棋飘 瘤况霖促.
		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				delayFrame = 0xFFFF;
				type = GetMasterEffectType(status);
				if(0 == type || IsDead()) 
					return false;
			}
			break;
		// 2004, 6 ,12 sobeit add end - 130,150 付胶磐 捞棋飘矫 弊傈 付胶磐 捞棋飘 瘤况霖促.
//		case EFFECTSTATUS_GDR_FLOATING:
//			//SetAction(ACTION_DRAINED);
//		//	m_MoveAction = ACTION_DRAINED;
//			if(GetCreatureType() == 717) // 龙靛饭 老锭
//			{ 
//				
//			}
//			break;
		case EFFECTSTATUS_BLINDNESS:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_BLINDNESS,GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
		case EFFECTSTATUS_GLACIER:
			if(IsAlive() /*&& !HasEffectStatus(EFFECTSTATUS_GLACIER)*/)
			{
				
				SetAction(ACTION_DRAINED);
				m_ActionCount = 0;
				SetCauseCriticalWounds(delayFrame);
			//	if(!HasEffectStatus(EFFECTSTATUS_GLACIER))
				{
					if(IsSlayer())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_SLAYER,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
					else if(IsVampire())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_VAMPIRE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
					else if(IsOusters())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_OUSTERS,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
				}
			}
		//	m_bEffectStatus[status] = true;
		//	return true;
			break;
		case EFFECTSTATUS_SUMMON_GDR:
			delayFrame = 34;
			break;
//		case EFFECTSTATUS_ICICLE_DROP:
//			delayFrame = 16;
//			break;
//		case EFFECTSTATUS_ICICLE_AUGER:
//			delayFrame = 14;
//			break;
//		case EFFECTSTATUS_ICICLE_LARGE_AUGER:
//			delayFrame = 12;
//			break;
		
		case EFFECTSTATUS_GOLD_MEDAL:
			delayFrame = 21;
			break;
		case EFFECTSTATUS_SIEGE_DEFENDERL:
		case EFFECTSTATUS_SIEGE_REINFORCE:
		case EFFECTSTATUS_SIEGE_ATTACKER_1:
		case EFFECTSTATUS_SIEGE_ATTACKER_2:
		case EFFECTSTATUS_SIEGE_ATTACKER_3:
		case EFFECTSTATUS_SIEGE_ATTACKER_4:
		case EFFECTSTATUS_SIEGE_ATTACKER_5:
			SetSiegeWar(status - EFFECTSTATUS_SIEGE_DEFENDERL + 1);
			break;
		case EFFECTSTATUS_REPUTO_FACTUM:
			delayFrame = 0;
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_HIT_REPUTO_FACTUM, GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, 8, NULL, false);
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_1:
			SetAction(ACTION_SLAYER_BLADE);
			SetBurningSol(0);
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_2:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_3:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_4:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			break;
		case EFFECTSTATUS_INSTALL_TURRET: // 醚浇 130 牢胶喷 磐房
			{
				SetAction( ACTION_STAND );
				SetInstallTurretCount(0);
				SetInstallTurretDirect(2);
//				SetCreatureType( CREATURETYPE_INSTALL_TURRET );
//				SetGroundCreature();
//				
				SetDirection( 2 );
				SetCurrentDirection( 2 );
//				//--------------------------------------------------
//				// 戳措肺 函脚窍绰 搬苞
//				//--------------------------------------------------
//				MActionResult* pResult = new MActionResult;
//
//				pResult->Add( new MActionResultNodeChangeCreatureType( GetID(), CREATURETYPE_INSTALL_TURRET ) );

//					
//				ExecuteActionInfoFromMainNode(
//					RESULT_SKILL_INSTALL_TURRET,										// 荤侩 扁贱 锅龋
//				
//					GetX(), GetY(), 0,
//					GetDirection(),														// 荤侩 规氢
//					
//					OBJECTID_NULL,												// 格钎俊 措茄 沥焊
//					GetX(), GetY(), 0, 
//					
//					0,													// 扁贱狼 (巢篮) 瘤加 矫埃		
//					
//					NULL, //NULL,
//					
//					false);			// 扁贱 梅何磐 矫累茄促.
			}
			break;
			// 2004, 10, 25, sobeit add start
		case EFFECTSTATUS_DRAGON_EYES:
			delayFrame = 0xffff;
			break;
			// 2004, 10, 25, sobeit add end

		case EFFECTSTATUS_SET_AFIRE_ATTACK:
			delayFrame = 20;
			break;

		case EFFECTSTATUS_ICE_HORIZON:
				delayFrame = 22;
			break;
			
		case EFFECTSTATUS_FIERCE_FLAME:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_FIERCE_FLAME,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		// add by Coffee 2007-5-3 火法	
		case EFFECTSTATUS_DUMMY_DRAKE:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HYDRO_CONVERGENCE: //水法
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_HYDRO_CONVERGENCE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HETER_CHAKRAM: //敏战
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_SKILL_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
		case EFFECTSTATUS_BLOOD_CURSE: // 吸血鬼技能
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_BLOOD_CURSE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		// end 	
		case EFFECTSTATUS_PASSING_HEAL:
				delayFrame = 20;
			break;
		case EFFECTSTATUS_DONATION_200501:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_CANDLE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
			
	}

	m_bEffectStatus[status] = true;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;	

	//------------------------------------------------------------
	//
	//			EffectSprite肺 钎泅窍绰 版快
	//
	//------------------------------------------------------------		
	if (bUseEffectSprite)
	{
		if( (status == EFFECTSTATUS_GRAND_MASTER_OUSTERS ||
			status == EFFECTSTATUS_GRAND_MASTER_SLAYER ||
			status == EFFECTSTATUS_GRAND_MASTER_VAMPIRE ) &&
			!g_pSystemAvailableManager->IsAvailableGrandMasterEffectSystem() )
			return false;

		DEBUG_ADD("[Use EffectSprite]");
		
		//TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;

		if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
		{
			DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : %d", (int)type);
			
			return false;
		}

		if (IsFemale()
			&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
		{
			type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;
		}

//		//------------------------------------------------------------
//		// EFFECTSTATUS_ARMAGEDDON
//		//------------------------------------------------------------
//		if (status==EFFECTSTATUS_ARMAGEDDON)
//		{
//			type = GetArmageddonSprite( this );			
//		}

		TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;

		if (frameID==FRAMEID_NULL)
		{
			DEBUG_ADD("[Error]FrameID of EffectSpriteType is NULL");
			
			return false;
		}

		//------------------------------------------------------------
		// 捞固 乐绰 版快 
		//------------------------------------------------------------
		if (m_bAttachEffect[type])
		{		
			ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

			//------------------------------------------------------
			// 个俊 嘿绰芭
			//------------------------------------------------------
			// 鞍篮 Effect甫 茫绰促. 
			while (iEffect != m_listEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// 鞍篮 type阑 茫绰促.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// 眠啊且妨绰 巴捞 歹 词霸 场朝 版快俊父 矫埃阑 犬厘茄促.
					if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
					{
						pEffect->SetCount( delayFrame );
					}

					return true;
				}

				iEffect++;
			}

			//------------------------------------------------------
			// 官蹿俊 嘿绰 芭
			//------------------------------------------------------
			iEffect = m_listGroundEffect.begin();

			// 鞍篮 Effect甫 茫绰促. 
			while (iEffect != m_listGroundEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// 鞍篮 type阑 茫绰促.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// 眠啊且妨绰 巴捞 歹 词霸 场朝 版快俊父 矫埃阑 犬厘茄促.
					if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
					{
						pEffect->SetCount( delayFrame );
					}

					return true;
				}

				iEffect++;
			}

			return false;
		}

		//------------------------------------------------------------
		// 绝绰 版快 眠啊茄促.
		//------------------------------------------------------------
		MAttachEffect*	pEffect = NULL;
		
		if (eType==MEffect::EFFECT_ATTACH_ORBIT)
		{
			for(int i = 0; i < eCount; i++)
			{
				int effectPosition = 0;
				if( eCount > 1 )
				{
					effectPosition = (MAX_EFFECT_ORBIT_STEP/(eCount*2))*i;
				}
				else
				{
					effectPosition = -1;
				}
				pEffect = new MAttachOrbitEffect(type, delayFrame, orbit_type, effectPosition);
				
				pEffect->SetAttachCreature( this );
				
				// 蝴唱绰 Effect狼 俺荐甫 刘啊?
				if (pEffect->GetBltType()==BLT_EFFECT)
				{
					// 弥檬肺 眠啊登绰 蝴唱绰Effect捞搁 矫具甫 眠啊矫挪促.
					//if (m_nAlphaEffect==0)
					{
						pEffect->SetPosition( m_X, m_Y );
						//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
					}
					
					//m_nAlphaEffect ++;		
				}
				
				//------------------------------------------------------------
				// 官蹿俊 嘿绰 芭扼搁..
				//------------------------------------------------------------
				if ((*g_pEffectStatusTable)[status].bAttachGround)
				{
					m_listGroundEffect.push_back( pEffect );
				}
				//------------------------------------------------------------
				// 个俊 嘿绰 芭..
				//------------------------------------------------------------
				else
				{
					m_listEffect.push_back( pEffect );
				}
				
				m_bAttachEffect[type] = true;
				
			}
		}
		else
		{
			pEffect = new MAttachEffect(type, delayFrame);
			
			pEffect->SetAttachCreature( this );
			
			// 蝴唱绰 Effect狼 俺荐甫 刘啊?
			if (pEffect->GetBltType()==BLT_EFFECT)
			{
				// 弥檬肺 眠啊登绰 蝴唱绰Effect捞搁 矫具甫 眠啊矫挪促.
				//if (m_nAlphaEffect==0)
				{
					pEffect->SetPosition( m_X, m_Y );
					//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
				}
				
				//m_nAlphaEffect ++;		
			}
			
			//------------------------------------------------------------
			// 官蹿俊 嘿绰 芭扼搁..
			//------------------------------------------------------------
			if ((*g_pEffectStatusTable)[status].bAttachGround)
			{
				m_listGroundEffect.push_back( pEffect );
			}
			//------------------------------------------------------------
			// 个俊 嘿绰 芭..
			//------------------------------------------------------------
			else
			{
				m_listEffect.push_back( pEffect );
			}
			
			m_bAttachEffect[type] = true;
			
		}
	

		// Set LightSight
		/*
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "NewEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
			g_pDebugMessage->Next();
		#endif
		*/
	}
	//------------------------------------------------------------
	//
	//			EffectColor肺 钎泅窍绰 版快
	//
	//------------------------------------------------------------
//	else
	if((*g_pEffectStatusTable)[status].EffectColor != 0xFFFF)
	{
		DEBUG_ADD("[Use EffectColor]");
		
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		if (colorSet>=MAX_COLORSET)
		{
			DEBUG_ADD_FORMAT("[Error]EffectColor of EffectStatus exceed Max : ", (int)colorSet);
			
			return false;
		}

		//------------------------------------------------------------
		// 捞固 乐绰瘤 眉农茄促.
		// 祸彬肺 眉农秦具登骨肺.. list甫 促 八祸秦具 茄促. T_T;
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("listEffect size = %d", m_listEffect.size());
		
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		//----------------------------------------------------------------
		// 个俊 嘿绰芭
		//----------------------------------------------------------------
		// 鞍篮 Effect甫 茫绰促. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 鞍篮 祸彬阑 茫绰促.
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// 眠啊且妨绰 巴捞 歹 词霸 场朝 版快俊父 矫埃阑 犬厘茄促.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				return true;
			}

			iEffect++;
		}

		//----------------------------------------------------------------
		// 官蹿俊 嘿绰芭
		//----------------------------------------------------------------
		// 绝促绊 夯促.

		DEBUG_ADD("[Create New AttachEffect]");
		
		//------------------------------------------------------------
		// 绝绰 版快 眠啊茄促.
		//------------------------------------------------------------
		MAttachEffect*	pEffect = new MAttachEffect(EFFECTSPRITETYPE_NULL, delayFrame);

		DEBUG_ADD("[Create New AttachEffect OK]");
		
		pEffect->SetAttachCreature( this );

		DEBUG_ADD("[Set AttachCreature]");
		
		// 祸彬 汲沥
		pEffect->SetEffectColor( colorSet );

		ADDON	part = (*g_pEffectStatusTable)[status].EffectColorPart;

		//------------------------------------------------------------
		// 漂沥茄 何困父 祸彬捞 官差绰 版快档 乐促.
		//------------------------------------------------------------
		//if (part<ADDON_MAX)
		//{
			pEffect->SetEffectColorPart( part );
		//}

		DEBUG_ADD("[if (pEffect->GetBltType()==BLT_EFFECT)]");
		
		// 蝴唱绰 Effect狼 俺荐甫 刘啊?
		if (pEffect->GetBltType()==BLT_EFFECT)
		{
			// 弥檬肺 眠啊登绰 蝴唱绰Effect捞搁 矫具甫 眠啊矫挪促.
			//if (m_nAlphaEffect==0)
			{
				pEffect->SetPosition( m_X, m_Y );
				//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
			}

			//m_nAlphaEffect ++;		
		}

		//------------------------------------------------------
		// 祸彬 函窍绰芭绰 弊成 个俊 嘿捞绰芭俊 眠啊茄促.
		//------------------------------------------------------
		m_listEffect.push_back( pEffect );				
	}

	DEBUG_ADD("[AddEffectStatus - OK]");
	
	return true;
}

//----------------------------------------------------------------------
// Set Name - 某腐磐 捞抚 汲沥
//----------------------------------------------------------------------
void	
MCreature::SetName(const char* pName)
{
	if (m_pName!=NULL)
	{
		delete [] m_pName;
	}

	m_pName = new char [strlen(pName) + 1];

	strcpy(m_pName, pName);
}

//----------------------------------------------------------------------
// Is Stop
//----------------------------------------------------------------------
BOOL	
MCreature::IsStop()
{ 
	return m_MoveCount>=m_MoveCountMax 
			&& m_ActionCount>=GetActionCountMax() 
			|| m_Action==ACTION_STAND && m_MoveAction==ACTION_MOVE
			|| m_Action==ACTION_SLAYER_MOTOR_STAND && m_MoveAction==ACTION_SLAYER_MOTOR_MOVE
			|| m_Action==ACTION_OUSTERS_FAST_MOVE_STAND && m_MoveAction==ACTION_OUSTERS_FAST_MOVE_MOVE;
}

//----------------------------------------------------------------------
// Set Server Position
//----------------------------------------------------------------------
void			
MCreature::SetServerPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	
{ 
	//------------------------------------------------
	// 扁粮狼 MoveBuffer甫 促 瘤框
	//------------------------------------------------
	ReleaseMoveBuffer();

	SetServerPositionWithBlock(sX, sY);
}

//----------------------------------------------------------------------
// Is Slayer Character
//----------------------------------------------------------------------
// slayer 弊覆阑 荤侩沁唱?
//----------------------------------------------------------------------
bool			
MCreature::IsSlayerCharacter() const
{
	return (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0] <= 1;
	//return m_CreatureType==CREATURETYPE_SLAYER_MALE
	//	||  m_CreatureType==CREATURETYPE_SLAYER_FEMALE;
}

//----------------------------------------------------------------------
// 框流捞绰 规过苞 弊俊 蝶弗 捞悼窜困甫 搬沥茄促.
//----------------------------------------------------------------------
void		
MCreature::SetMoveDevice(MOVE_DEVICE md)
{	
	//--------------------------------------------------------
	// 葛滴 利侩
	//--------------------------------------------------------
	AffectMoveBufferAll();

	//--------------------------------------------------
	// Slayer牢 版快父 函版捞 啊瓷窍促.
	//--------------------------------------------------
	if (IsSlayer())
	{
		m_MoveDevice = md; 

//		SetAction(ACTION_STAND);
		switch (md)
		{
			case MOVE_DEVICE_WALK :
			{
				m_MoveAction = ACTION_MOVE;

				if (m_Action==ACTION_SLAYER_MOTOR_STAND)
				{
					SetAction( ACTION_STAND );
				}				
			}
			break;

			case MOVE_DEVICE_RIDE :
			{
				m_MoveAction = ACTION_SLAYER_MOTOR_MOVE;

				if (m_Action==ACTION_STAND)
				{
					SetAction( ACTION_SLAYER_MOTOR_STAND );
				}
			}
			break;
		}	
	}
	//--------------------------------------------------
	// 酒囱 版快..
	//--------------------------------------------------
	else if( IsOusters() )
	{
		m_MoveDevice = md; 
		if(m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH)
		{	
			m_MoveAction = ACTION_OUSTERS_FAST_MOVE_MOVE;
		}
		else
		{
			m_MoveAction = ACTION_MOVE;
		}
		
		SetAction(ACTION_STAND);
	} else
	{
		m_MoveDevice = MOVE_DEVICE_WALK;
		m_MoveAction = ACTION_MOVE;
	}
}

//----------------------------------------------------------------------
// Creature狼 困摹甫 Set窍绊 框流捞带 悼累阑 肛冕促.
//----------------------------------------------------------------------
void	
MCreature::SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)	
{ 
	//------------------------------------------------
	// 扁粮狼 MoveBuffer甫 促 瘤框
	//------------------------------------------------
	ReleaseMoveBuffer();

	if (m_X!=SECTORPOSITION_NULL
		&& m_Y!=SECTORPOSITION_NULL)
	{
		MovePosition( x, y );
	}
	else
	{
		m_X	= x; 
		m_Y	= y; 	
	}

	m_sX = 0;
	m_sY = 0;
	m_ActionCount		= 0;
	m_ActionCountMax	= 0;
	m_MoveCount			= 0;
	m_MoveCountMax		= 0;

	CheckInDarkness();	
}

//----------------------------------------------------------------------
// Set Weapon Speed
//----------------------------------------------------------------------
void				
MCreature::SetWeaponSpeed(int speed)
{
	//-----------------------------------------------------------
	// 冠零唱 戳措搁 傍拜加档 蝶肺 绝寸..
	//-----------------------------------------------------------
	if (m_CreatureType==CREATURETYPE_BAT ||
		m_CreatureType==CREATURETYPE_WOLF ||
		m_CreatureType==CREATURETYPE_WER_WOLF)
	{
		m_WeaponSpeed = WEAPON_SPEED_NORMAL;
	}
	//-----------------------------------------------------------
	// 傍拜 加档 拌魂..
	//-----------------------------------------------------------
	else
	{
		if (speed <= 33)
		{
			m_WeaponSpeed = WEAPON_SPEED_SLOW;
		}
		else if (speed <= 66)
		{
			m_WeaponSpeed = WEAPON_SPEED_NORMAL;
		}
		else
		{
			m_WeaponSpeed = WEAPON_SPEED_FAST;
		}
	}
	if(GetClassType() == CLASS_PLAYER)
	{
		UI_SetWeaponSpeed(speed, m_WeaponSpeed);
	}
}

//----------------------------------------------------------------------
// Creature甫 肛眠霸 茄促.
//----------------------------------------------------------------------
void
MCreature::SetStop()
{ 
	
	m_bFastMove = false;
	m_bKnockBack = 0;	

	//------------------------------------------------
	// 扁粮狼 MoveBuffer甫 促 瘤框
	//------------------------------------------------
	AffectMoveBufferAll();
	//ReleaseMoveBuffer();

	// 2001.11.8 - 沥瘤且锭 谅钎 焊沥 犬角洒..
	ActionMoveNextPosition();

	// Action 吝瘤
	m_sX=0; 
	m_sY=0;

	int action = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_MOVE : ACTION_STAND;
	if( IsOusters() && HasEffectStatusSummonSylph( this ))
	{
		if (action==ACTION_STAND)
		{
			action = ACTION_OUSTERS_FAST_MOVE_STAND;
		} 
		else if( action == ACTION_MOVE )
		{
			action = ACTION_OUSTERS_FAST_MOVE_MOVE;
		}
	}
	m_Action		= action;
	// 2004, 11, 3, sobeit modify start - m_ActionCount甫 官诧 咀记狼 钙胶 墨款飘肺 技泼 沁澜 - 酒快胶磐令 绊胶飘 滚弊 东矫..ぱぱ;
	m_ActionCount	= GetActionCountMax(); 
	//m_ActionCount	= (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	// 2004, 11, 3, sobeit modify end
	m_MoveCount		= m_MoveCountMax;

	// 促澜 悼累档 绝沮
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

	
	//m_nNextUsedActionInfo = ACTIONINFO_NULL;
}

//----------------------------------------------------------------------
// Get PixelX - Creature狼 PixelX谅钎
//----------------------------------------------------------------------
int			
MCreature::GetPixelX() const
{ 
	return MTopView::MapToPixelX(m_X) + m_sX;
}

//----------------------------------------------------------------------
// Get PixelY - Creature狼 PixelY谅钎
//----------------------------------------------------------------------
int			
MCreature::GetPixelY() const			
{ 
	return MTopView::MapToPixelY(m_Y) + m_sY;// + ((m_MoveType == CREATURE_FLYING)<<2);
}

//----------------------------------------------------------------------
// Set Level Name
//----------------------------------------------------------------------
void				
MCreature::SetLevelName(int ln)
{
	// slayer捞搁 扁贱 饭骇俊 蝶扼辑...

	// vampire捞搁 饭骇俊 蝶扼辑?

	// 
	m_LevelName = rand() % g_pLevelNameTable->GetSize();
}

//----------------------------------------------------------------------
// Get Level Name
//----------------------------------------------------------------------
const char*			
MCreature::GetLevelName() const
{
	return (*g_pLevelNameTable)[m_LevelName].GetString();
}

//----------------------------------------------------------------------
// Move Position
//----------------------------------------------------------------------
// Zone俊辑 (m_X, m_Y) Sector俊 乐绰 
// this Creature甫 (x, y)肺 颗败具 茄促.
//----------------------------------------------------------------------
bool
MCreature::MovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	if (m_pZone==NULL)
	{
		return false;
	}

	// Creature Type俊 蝶扼辑 促福促.			
	switch (m_MoveType)
	{
		case CREATURE_UNDERGROUND : 
			if (m_pZone->MoveUndergroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect啊 乐阑 版快
				/*
				if (m_nAlphaEffect)
				{
					// 矫具甫 官层霖促.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 矫具甫 官层霖促.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 捞悼沁栏搁 货肺款 谅钎 汲沥
				m_X	= x;
				m_Y	= y;		
				
				CheckInDarkness();
				
				return true;
			}
		break;

		case CREATURE_GROUND : 
			if (m_pZone->MoveGroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect啊 乐阑 版快
				/*
				if (m_nAlphaEffect)
				{
					// 矫具甫 官层霖促.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 矫具甫 官层霖促.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 捞悼沁栏搁 货肺款 谅钎 汲沥
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		case CREATURE_FLYING : 
			if (m_pZone->MoveFlyingCreature(this, m_X, m_Y, x, y))
			{
				// Effect啊 乐阑 版快
				/*
				if (m_nAlphaEffect)
				{
					// 矫具甫 官层霖促.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 矫具甫 官层霖促.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 捞悼沁栏搁 货肺款 谅钎 汲沥
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		// fake绰 弊成 框流牢促.
		case CREATURE_FAKE_NO_BLOCK :
		case CREATURE_FAKE_UNDERGROUND :
		case CREATURE_FAKE_GROUND :
		case CREATURE_FAKE_FLYING :
			m_X = x;
			m_Y = y;
		return true;
		break;
	}

	return false;
}

//----------------------------------------------------------------------
// (x,y)俊辑 规氢栏肺 捞悼茄 困摹甫 掘绰促.
//----------------------------------------------------------------------
void
MCreature::GetPositionToDirection(TYPE_SECTORPOSITION &x, TYPE_SECTORPOSITION &y, BYTE direction)
{
	switch (direction)
	{
		case DIRECTION_LEFTDOWN		: x--;	y++;	break;
		case DIRECTION_RIGHTUP		: x++;	y--;	break;
		case DIRECTION_LEFTUP		: x--;	y--;	break;
		case DIRECTION_RIGHTDOWN	: x++;	y++;	break;
		case DIRECTION_LEFT			: x--;			break;
		case DIRECTION_DOWN			:		y++;	break;
		case DIRECTION_UP			:		y--;	break;
		case DIRECTION_RIGHT		: x++;			break;
	}
}

//----------------------------------------------------------------------
// Add Effect
//----------------------------------------------------------------------
// Creature俊 崔扼嘿绢辑 钎泅登绰 Effect 钎泅
//
// 吝酶阑 倾侩窍瘤 臼栏骨肺.. 
// 酒流 嘿绢辑 钎泅登瘤 臼篮 Effect父 货肺 眠啊茄促.
//
// 弊矾唱, 矫埃 犬厘阑 秦具窍绰 版快啊 积变促.
//----------------------------------------------------------------------
MAttachEffect*		
MCreature::CreateAttachEffect(TYPE_EFFECTSPRITETYPE type, 
							  DWORD delayFrame, 
							  DWORD linkDelayFrame, 
							  BOOL bGroundEffect,
							  MEffect::EFFECT_TYPE eType)
{
	DEBUG_ADD_FORMAT("CreateAttachEffect. type=%d, delayf=%d", type, delayFrame);

	// 磷篮 版快... delay甫 绝矩促. 
	if (!m_bAlive)
	{
		// - -;;
		delayFrame = min( 10, delayFrame );
	}
	bool	IsMulti = false;
	int		orbitType = 0;
	// 2004, 08, 05 sobeit add start - 己巩, 辨靛 鸥况 颇祈脾扁
	if(GetCreatureType() >= 726 && GetCreatureType() <= 729)
	{	
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DUST_1 ||
		   type > EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED) // 己巩 包访 捞棋飘啊 酒聪搁 公历扒 颇祈脾扁..ぞぞ
		{
			int currentHP		= max(0,int(GetHP()));
			int frame = 4 -(currentHP*5/GetMAX_HP());
			
			if(m_NickNameType != frame) // 函荐 眠啊窍变 弊犯绊..己巩篮 m_NickNameType阑 救静扁 锭巩俊 夸扁辑 Temporary窍霸 静磊-_-; 
			{
				m_NickNameType = frame;
				if(frame<1) frame = 1;
				type = EFFECTSPRITETYPE_CASTLE_GATE_DUST_1 + rand()%(5-frame);
				delayFrame = 32;
			}
			else
			{
				if(frame<1) frame = 1;
				type = EFFECTSPRITETYPE_CASTLE_GATE_PIECE_1 + rand()%(5-frame);
				delayFrame = 14;
			}
			// 己巩 鸥拜 捞棋飘 牢单.. 绝绰霸 唱阑淀..ぞ
//			ExecuteActionInfoFromMainNode(SKILL_CLIENT_CASTLE_GATE_DAMAGED,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//				GetX(), GetY(), 0, 8, NULL, false);
		}
		else
		{
			if(type == EFFECTSPRITETYPE_CASTLE_GATE_LARGE_DUST)
				delayFrame = 44;
			else if(type == EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED)
				delayFrame = 8;
			else 
				delayFrame = 32;
		}
	}
	else if(GetCreatureType() == 734) // 辨靛 鸥况
	{
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED ||
		   type > EFFECTSPRITETYPE_GUILD_TOWER_PROTECT) // 辨靛 鸥况包访 捞棋飘啊 酒聪搁 公历扒 颇祈脾扁..ぞぞ
		{
			type = EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED;
			delayFrame = 8;
//			ExecuteActionInfoFromMainNode(SKILL_CLIENT_GUILD_TOWER_PROTECT,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//				GetX(), GetY(), 0, 32, NULL, false);
		}
		else
		{
			delayFrame = 32;
		}
	}
//	else if(GetCreatureType() == 723) // 阿己 龙靛饭
//	{
//		if(type == EFFECTSPRITETYPE_GDR_DEAD)
//			delayFrame = 218;
//
//	}
	else
	{
//		if(GetCreatureType() == 734&&type == EFFECTSPRITETYPE_GUILD_TOWER_DESTROY) // 辨靛 鸥况
//			delayFrame = 32;
	// 2004, 08, 05 sobeit add end - 己巩, 辨靛 鸥况 颇祈脾扁

		//------------------------------------------------
		// 乔牢 版快绰 困摹甫 random窍霸
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_3_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_3_1)
		{
			// 虐俊 蝶扼辑.. 100逞栏搁... (榜饭赣)
			if ((*g_pCreatureTable)[m_CreatureType].Height > 100)
			{
				// type + 0~6
				type += rand()%7;
			}
			else
			{
				// type + 1~6
				type += rand()%6 + 1;
			}		
		}

		//------------------------------------------------
		// 乔牢 版快绰 困摹甫 random窍霸
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BULLET_OF_LIGHT_START_1)
		{
			// 虐俊 蝶扼辑.. 100逞栏搁... (榜饭赣)
			if ((*g_pCreatureTable)[m_CreatureType].Height > 100)
			{
				// type + 0~6
				type += (rand()%7)*2;
			}
			else
			{
				// type + 1~6
				type += (rand()%6)*2;
			}		
		}
		//------------------------------------------------
		// 漠 嘎篮 乔 (技肺)
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BLOOD_VERTICAL_1 
			|| type==EFFECTSPRITETYPE_BLOOD_HORIZONTAL_1)
		{
			if (rand()%2)
			{
				type = EFFECTSPRITETYPE_BLOOD_VERTICAL_1 + rand()%4;
			}
			else
			{
				type = EFFECTSPRITETYPE_BLOOD_HORIZONTAL_1 + rand()%3;
			}		
		}
		if (type==EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_HORIZONTAL_1)
		{
			if (rand()%2)
			{
				type = EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 + rand()%4;
			}
			else
			{
				type = EFFECTSPRITETYPE_GREEN_BLOOD_HORIZONTAL_1 + rand()%3;
			}		
		}
		//------------------------------------------------
		// 魂俊 嘎篮 smoke牢 版快
		//------------------------------------------------
		else if (type==EFFECTSPRITETYPE_SMOKE)
		{
			type += rand()%5;
		}
		
		//------------------------------------------------
		// Typhoon back [货扁贱]
		//------------------------------------------------
		else if (type==EFFECTSPRITETYPE_TYPHOON_BACK)
		{	
			SetTurning( 16 );
		}


		if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
		{
			DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : ", (int)type);
			
			return NULL;
		}

		//------------------------------------------------
		// 咯磊侩 EffectSpriteType阑 荤侩窍绰 版快
		//------------------------------------------------
		if (IsFemale()
			&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
		{
			type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;

			if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
			{
				DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : ", (int)type);
				
				return NULL;
			}
		}
	

		// 傍拜 加档俊 蝶弗 捞棋飘甫 促福霸 备喊茄促.
		switch( type )
		{
		case EFFECTSPRITETYPE_FLOURISH_NORMAL :
		case EFFECTSPRITETYPE_FLOURISH_NORMAL_2 :
		case EFFECTSPRITETYPE_FLOURISH_NORMAL_3 :
		case EFFECTSPRITETYPE_NEW_BLOODY_NAIL_FEMALE_NORMAL :
		case EFFECTSPRITETYPE_NEW_ACID_TOUCH_FEMALE_NORMAL :	
		case EFFECTSPRITETYPE_NEW_BLOODY_NAIL_MALE_NORMAL :
		case EFFECTSPRITETYPE_NEW_ACID_TOUCH_MALE_NORMAL :	
		case EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL_ATTACK_NORMAL :
		case EFFECTSPRITETYPE_REDIANCE_FEMALE_NORMAL_ATTACK_NORMAL :
		case EFFECTSPRITETYPE_TALON_OF_CROW_MALE_NORMAL :
		case EFFECTSPRITETYPE_TALON_OF_CROW_FEMALE_NORMAL :
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 15;
				++type;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 13;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 11;
				--type;			
				break;
			}
			break;
		case EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL :
		case EFFECTSPRITETYPE_REDIANCE_FEMALE_NORMAL :
			
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_21:
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_23:
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_25:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				++type;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				--type;			
				break;
			}
	//		if(type == EFFECTSPRITETYPE_THROW_CHACRAM_23
	//			|| type == EFFECTSPRITETYPE_THROW_CHACRAM_21
	//			|| type == EFFECTSPRITETYPE_THROW_CHACRAM_25)
	//		{
	//			SetGhost(2, delayFrame-2);
	//			IsMulti = true;
	//		}
			break;

		case EFFECTSPRITETYPE_THROW_CHACRAM_21:
		case EFFECTSPRITETYPE_THROW_CHACRAM_23:
		case EFFECTSPRITETYPE_THROW_CHACRAM_25:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_25;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_23;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_21;			
				break;
			}
			//SetGhost(2, delayFrame);
			break;

		case EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY:
		case EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY:
		case EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY;
				break;
			}
			SetGhost(2, delayFrame);
			break;

		
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_12:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_3:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_4:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_5:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_6:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_21:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_23:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_25:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY:
			{
				delayFrame = 25;
				SetGhost(2, delayFrame-2);
				IsMulti = true;
			}
			break;

		case EFFECTSPRITETYPE_PIERCING_FRONT :
		case EFFECTSPRITETYPE_PIERCING_BACK :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_1 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_2 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_3 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_4 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_1 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_2 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_3 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_4 :

		case EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW:
		case EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW_SHADOW:
			IsMulti = true;
			break;

		case EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL:
		case EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL_ATTACK:
		case EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL:
		case EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL_HEAL:
			orbitType = 2;
			break;
		
		}

		if( type >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && type <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW )
			delayFrame = 0xFFFF;
	}	
	TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;

	if (frameID==FRAMEID_NULL)
	{
		DEBUG_ADD("[Error]FrameID of EffectSpriteType is NULL");
		
		return NULL;
	}


	//------------------------------
	// 捞固 乐绰 版快 
	//------------------------------
	if (m_bAttachEffect[type] && !IsMulti)
	{	
		//------------------------------------------------------
		// 个俊 嘿篮芭 眉农
		//------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		// 鞍篮 Effect甫 茫绰促. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 鞍篮 type阑 茫绰促.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// 眠啊且妨绰 巴捞 歹 词霸 场朝 版快俊父 矫埃阑 犬厘茄促.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				DEBUG_ADD("find same AttachEffect");
				return pEffect;
			}

			iEffect++;
		}

		//------------------------------------------------------
		// 官蹿俊 嘿篮芭 眉农
		//------------------------------------------------------
		iEffect = m_listGroundEffect.begin();

		// 鞍篮 Effect甫 茫绰促. 
		while (iEffect != m_listGroundEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 鞍篮 type阑 茫绰促.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// 眠啊且妨绰 巴捞 歹 词霸 场朝 版快俊父 矫埃阑 犬厘茄促.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				DEBUG_ADD("find same GroundAttachEffect");
				return pEffect;
			}

			iEffect++;
		}

		return NULL;
	}

	//------------------------------
	// 绝绰 版快 眠啊茄促.
	//------------------------------
	MAttachEffect*	pEffect = NULL;
	
	if (eType==MEffect::EFFECT_ATTACH_ORBIT)
	{
		pEffect = new MAttachOrbitEffect(type, delayFrame, orbitType, 0, linkDelayFrame);
	}
	else	// EFFECT_ATTACH
	{
		pEffect = new MAttachEffect(type, delayFrame, linkDelayFrame);
	}

	pEffect->SetAttachCreatureID( m_ID );

	// 蝴唱绰 Effect狼 俺荐甫 刘啊?
	if (pEffect->GetBltType()==BLT_EFFECT)
	{
		// 弥檬肺 眠啊登绰 蝴唱绰Effect捞搁 矫具甫 眠啊矫挪促.
		//if (m_nAlphaEffect==0)
		{
			pEffect->SetPosition( m_X, m_Y );
			//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
		}

		//m_nAlphaEffect ++;		
	}

	//------------------------------------------------------
	// 官蹿俊 嘿绰芭
	//------------------------------------------------------
	if (bGroundEffect)
	{
		m_listGroundEffect.push_back( pEffect );
	}
	//------------------------------------------------------
	// 个俊 嘿绰 芭
	//------------------------------------------------------
	else
	{
		m_listEffect.push_back( pEffect );
	}

	m_bAttachEffect[type] = true;

	// Set LightSight
	DEBUG_ADD_FORMAT("NewEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());			
	
	return pEffect;
}

//----------------------------------------------------------------------
// Update Effect
//----------------------------------------------------------------------
// Creature俊 嘿篮 葛电 Effect狼 Frame阑 官层林绊..
// 场唱绰霸 乐栏搁 list客 memory俊辑 昏力茄促.
//----------------------------------------------------------------------
void		
MCreature::UpdateAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	ATTACHEFFECT_LIST::iterator iEffectTemp;

	// 灌扁绰 0
	//m_MaxEffectLight = 0;

	//---------------------------------------------------------------------
	// 某腐磐 祸彬 官操绰 祸..
	//---------------------------------------------------------------------
	// NULL蔼捞寸.. 寇何俊辑 眉农秦具窃..
	m_AttachEffectColor = m_ChangeColorSet;	//ATTACHEFFECTCOLOR_NULL;
	int bShowColor =  (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
						|| g_CurrentFrame % g_pClientConfig->FRAME_DRAW_ORIGINAL_SPRITE);	// 割 frame付促 茄锅究篮 盔贰 祸彬阑 焊咯霖促.
	int numColors = 0;
	
	//---------------------------------------------------------------------
	// 葛电 Effect甫 Update茄促.
	//---------------------------------------------------------------------
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		
		bool		bErase = false;

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif


		// 狐弗 捞悼老 版快 捞悼苞 沥瘤狼 捞棋飘啊 促福促.... 窍靛内爹..ぱ.ぬ
		if( pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_FLY ||
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_STOP ||
			(
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP
			)
			)
		{		
			BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].BltType;
			TYPE_EFFECTSPRITETYPE		EffectSpriteType = pEffect->GetEffectSpriteType();
			bool			bNewSummonSylphEffect = false;
			bool			bNewSummonSylphEffectStop = false;
			
			if( EffectSpriteType >= EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
				EffectSpriteType <= EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP )
			{
				bNewSummonSylphEffect = true;

				if( (EffectSpriteType - EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY) & 0x1 )
					bNewSummonSylphEffectStop = true;
			}
			
			if( bNewSummonSylphEffect && !bNewSummonSylphEffectStop )
				bltType = (*g_pEffectSpriteTypeTable)[EffectSpriteType + 1].BltType;

			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_STAND && (
				EffectSpriteType != EFFECTSPRITETYPE_FAST_MOVE_STOP ||
				( bNewSummonSylphEffect && !bNewSummonSylphEffectStop )
				)
				)
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].FrameID;
				
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType + 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			
			
				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_STOP );
				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType + 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
			else
			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_MOVE && 
				( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_FAST_MOVE_FLY ||
				( bNewSummonSylphEffect && bNewSummonSylphEffectStop )
			  )
			  )
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_FLY].FrameID;
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType - 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			

				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_FLY );

				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType - 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
		}
		else if( pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW )
		{
			if(!(GetAction() == ACTION_SLAYER_SWORD_2 || GetAction() == ACTION_SLAYER_SWORD_2_SLOW || GetAction() == ACTION_SLAYER_SWORD_2_FAST ) 
				&&	m_RepeatCount <= 0 )
			{
				pEffect->SetCount(0);
			}
		}
		else if (pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_START &&
			pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_END &&
			!HasEffectStatus( EFFECTSTATUS_TRYING )
			)
		{
			bErase = true;
		}
		

		if (bErase == false && pEffect->Update())
		{
			// 软康牢 版快俊绰 规氢阑 官操瘤 臼绰促.
			if( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_ABSORB_SOUL &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_FRONT &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_BACK )
				pEffect->SetDirection( m_CurrentDirection );

			// 弥绊 灌扁甫 啊柳 Effect狼 灌扁甫 历厘茄促.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 困摹 汲沥
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 困摹啊 官差菌芭唱
			// 蝴狼 农扁(矫具)啊 官诧 版快
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 矫具 官操扁
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
			}

			//---------------------------------------------------------------------
			// 某腐磐 祸彬 官操绰 Effect捞搁
			// 窍唱甫 急琶秦具 茄促.
			//---------------------------------------------------------------------
			if (bShowColor && pEffect->IsEffectColor())
			{
				numColors++;
				//if (m_AttachEffectColor == ATTACHEFFECTCOLOR_NULL || rand()%numColors==0)
				if (m_AttachEffectColor == m_ChangeColorSet || rand()%numColors==0)
				{
					m_AttachEffectColor = pEffect->GetEffectColor();
				}
			}

			//-----------------------------------------------
			//
			// 捞 Effect啊 场唱扁 傈俊 LinkCount俊 狼秦辑
			// 促澜 楷搬登绰 Effect啊 乐栏搁 积己秦具 茄促.
			//
			// 泅犁Frame捞 EndLinkFrame阑 逞绢埃 版快
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext俊辑 
				// pEffect狼 EffectTarget阑 NULL肺 父甸绢林扁 锭巩俊
				// 咯扁辑 瘤匡 鞘夸 绝促.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect绰 咯傈洒 粮犁秦具 窍骨肺 瘤快搁 救等促.
			}

			// 力措肺 等 版快
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 矫埃捞 促 蹬辑 场唱绰 版快
		//---------------------------------------------------------------------
		else
		{
			
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag力芭
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag力芭
			}
		
			//---------------------------------------------------------------------
			//
			// 促澜 楷搬登绰 Effect啊 乐栏搁 积己秦具 茄促.
			//
			//---------------------------------------------------------------------
//			// 2004, 8, 16, sobeit add start test - 档浇 130 胶懦
//			if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_BURNING_SOL_CHARGING)
//			{
//				// 	酒流 瞒隆 吝捞搁 朝扼啊绰霸 唱坷搁 救等促..^^
//			}
//			// 2004, 8, 16, sobeit add end test - 档浇 130 胶懦
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// 蝴唱绰 Effect搁 矫具甫 荤扼瘤霸 秦具茄促.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 葛电 蝴唱绰Effect啊 荤扼脸栏搁 矫具甫 力芭矫挪促.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			
			// memory昏力, list昏力						
			delete pEffect;						// memory力芭
			
			// list俊辑 昏力窍扁 困秦辑.. 烙矫肺 历厘
			iEffectTemp = iEffect;

			iEffect++;
			m_listEffect.erase( iEffectTemp );	// list俊辑 力芭
		}		
	}


	//---------------------------------------------------------------------
	// 官蹿俊 嘿绰 effect
	//---------------------------------------------------------------------
	iEffect = m_listGroundEffect.begin();
	
	//---------------------------------------------------------------------
	// 葛电 Effect甫 Update茄促.
	//---------------------------------------------------------------------
	while (iEffect != m_listGroundEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif

		if (pEffect->Update())
		{
			// 弥绊 灌扁甫 啊柳 Effect狼 灌扁甫 历厘茄促.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 困摹 汲沥
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 困摹啊 官差菌芭唱
			// 蝴狼 农扁(矫具)啊 官诧 版快
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 矫具 官操扁
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
			}

			//---------------------------------------------------------------------
			// 某腐磐 祸彬 官操绰 Effect捞搁
			// 窍唱甫 急琶秦具 茄促.
			//---------------------------------------------------------------------
			/*
			// 官蹿俊 彬府绰 芭绰 祸彬 官操绰霸 绝促.
			if (bShowColor && pEffect->IsEffectColor())
			{
				numColors++;
				if (m_AttachEffectColor == ATTACHEFFECTCOLOR_NULL || rand()%numColors==0)
				{
					m_AttachEffectColor = pEffect->GetEffectColor();
				}
			}
			*/
			//-----------------------------------------------
			//
			// 捞 Effect啊 场唱扁 傈俊 LinkCount俊 狼秦辑
			// 促澜 楷搬登绰 Effect啊 乐栏搁 积己秦具 茄促.
			//
			// 泅犁Frame捞 EndLinkFrame阑 逞绢埃 版快
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext俊辑 
				// pEffect狼 EffectTarget阑 NULL肺 父甸绢林扁 锭巩俊
				// 咯扁辑 瘤匡 鞘夸 绝促.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect绰 咯傈洒 粮犁秦具 窍骨肺 瘤快搁 救等促.
			}

			// 力措肺 等 版快
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 矫埃捞 促 蹬辑 场唱绰 版快
		//---------------------------------------------------------------------
		else
		{
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag力芭
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag力芭
			}
		
			//---------------------------------------------------------------------
			//
			// 促澜 楷搬登绰 Effect啊 乐栏搁 积己秦具 茄促.
			//
			//---------------------------------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// 蝴唱绰 Effect搁 矫具甫 荤扼瘤霸 秦具茄促.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 葛电 蝴唱绰Effect啊 荤扼脸栏搁 矫具甫 力芭矫挪促.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			
			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			
			// memory昏力, list昏力						
			delete pEffect;						// memory力芭
			
			// list俊辑 昏力窍扁 困秦辑.. 烙矫肺 历厘
			iEffectTemp = iEffect;

			iEffect++;
			m_listGroundEffect.erase( iEffectTemp );	// list俊辑 力芭
		}		
	}
}

//----------------------------------------------------------------------
// UnSet LightSight AttachEffect
//----------------------------------------------------------------------
// Creature俊 嘿篮 葛电 Effect狼 矫具甫 昏力茄促.
//----------------------------------------------------------------------
/*
void		
MCreature::UnSetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// 葛电 Effect甫 Update茄促.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// 矫具 昏力
//		g_pZone->UnSetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// 促澜 波
		iEffect++;
	}
}
*/

//----------------------------------------------------------------------
// Set LightSight AttachEffect
//----------------------------------------------------------------------
// Creature俊 嘿篮 葛电 Effect狼 矫具甫 眠啊茄促.
//----------------------------------------------------------------------
/*
void		
MCreature::SetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// 葛电 Effect甫 Update茄促.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// 矫具 昏力
//		g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// 促澜 波
		iEffect++;
	}
}
*/


//----------------------------------------------------------------------
// Set Dead
//----------------------------------------------------------------------
// 磷绰 actionInfo甫 焊咯林绊 磷绰促.
//----------------------------------------------------------------------
void
MCreature::SetDead()
{
	// 混酒 乐绰 版快父 磷牢促. -_-;;
	if (m_bAlive)
	{	
//		if(HasEffectStatus(EFFECTSTATUS_HIDE_TO_ATTACKER))
//		{
//			if(GetCreatureType() == 738) // 敲贰廉 胶抛捞记
//				ExecuteActionInfoFromMainNode(SKILL_PLEASURE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);			
//			else if(GetCreatureType() == 739) // 罚靛 付牢
//				ExecuteActionInfoFromMainNode(SKILL_LAND_MINE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 0, NULL, false);	
//			else if(GetCreatureType() == 740) // 努饭捞 葛绢
//				ExecuteActionInfoFromMainNode(SKILL_CLAYMORE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);	
//		}
		//if(HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) )
		if(GetCreatureType() == 723) // 阿己 龙靛饭
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_DEAD, GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, 20, NULL, false);			

		if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
		{
			RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
			SetAction( ACTION_STAND );
		}

		if( HasEffectStatusSummonSylph( this ) )
		{
			RemoveEffectStatusSummonSylph( this );
			SetAction( ACTION_STAND );
		}
		if(HasEffectStatus(EFFECTSTATUS_GLACIER))
		{ 
			RemoveEffectStatus( EFFECTSTATUS_GLACIER );
			SetAction(ACTION_STAND);
			ClearAttachEffect();
		}
		StopDrain();		
		StopAbsorb();

		//-------------------------------------------------------`
		// server block谅钎 绝局林扁
		//-------------------------------------------------------
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	
		// 2002.3.22 眠啊
		SetGroundCreature();

		MovePosition( m_ServerX, m_ServerY );
		SetStop();

		// 促弗 搬苞甫 利侩矫挪促.
		AffectUsedActionInfo(m_nUsedActionInfo);		
		m_nUsedActionInfo	= ACTIONINFO_NULL;
		m_nSpecialActionInfo = ACTIONINFO_NULL;

		// 磷阑 锭狼 ActionInfo	
		m_nNextUsedActionInfo	= (*g_pCreatureTable)[m_CreatureType].DeadActionInfo;	

		MActionResultNode* pActionResultNode = CreateActionResultNode(this, m_nNextUsedActionInfo);
		if(pActionResultNode != NULL)
		{
			if(m_pActionResult != NULL)
			{
				delete m_pActionResult;
				m_pActionResult = NULL;
			}
			
			m_pActionResult = new MActionResult;
			
			m_pActionResult->Add(pActionResultNode);
		}

			
		
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{		
			// 磷阑 锭狼 悼累
			SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );

			// 唱茄抛 窍绰 悼累
			SetTraceID( m_ID );
			m_TraceX				= m_X;
			m_TraceY				= m_Y;
			m_TraceZ				= 0;
		}

		//-------------------------------------------------------
		// 焊捞霸 窍扁
		//-------------------------------------------------------
		SetVisibleSoon();

		// HP
		StopRecoveryHP();
		StopRecoveryMP();
		SetStatus( MODIFY_CURRENT_HP, 0 );

		m_Z = 0;


		// 磷菌促.
		m_bAlive = false;	
		m_bInCasket = false;

		//-------------------------------------------------------
		// Effect 力芭
		//-------------------------------------------------------
		ClearAttachEffect();

		//-------------------------------------------------------
		// Effect惑怕甸 力芭
		//-------------------------------------------------------
		if (g_pEffectStatusTable!=NULL)
		{
			for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
			{
				if (e!=EFFECTSTATUS_BLOOD_DRAIN)
				{
					RemoveEffectStatus( (EFFECTSTATUS)e );
				}
			}
		}

		// 促农聪胶 哈绢唱扁..
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;

	}	
}

//----------------------------------------------------------------------
// Set Corpse
//----------------------------------------------------------------------
// 磷绢 乐绰 惑怕肺 父电促. --> 官肺 矫眉肺..
//----------------------------------------------------------------------
void
MCreature::SetCorpse()
{	
	//-------------------------------------------------------
	// server block谅钎 绝局林扁
	//-------------------------------------------------------
	m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);

	// 沥瘤
	SetStop();

	// 磷绰 葛嚼
	ACTIONINFO deadAI = (enum ACTIONINFO)(*g_pCreatureTable)[m_CreatureType].DeadActionInfo;
	
	// 磷阑 锭狼 悼累
	SetAction( GetActionInfoAction(deadAI) );

	// 2002.3.22 眠啊
	SetGroundCreature();
	
	// 悼累 葛滴 荐青..
	m_ActionCount	= GetActionCountMax(); 
	m_MoveCount		= m_MoveCountMax;
	
	// 磷菌促.
	m_bAlive = false;

	// 促农聪胶 哈绢唱扁..
	m_DarknessCount = -1;
	m_DarknessCountInc = 0;
}

//----------------------------------------------------------------------
// Set Alive
//----------------------------------------------------------------------
void
MCreature::SetAlive()
{
	// 混酒抄促. 
	m_bAlive = true;

	m_bHasHead = true;

	SetAction( ACTION_STAND );

	//-------------------------------------------------------
	// Effect惑怕甸 力芭
	//-------------------------------------------------------
	/*
	if (g_pEffectStatusTable!=NULL)
	{
		for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
		{
			RemoveEffectStatus( (EFFECTSTATUS)e );
		}
	}
	*/

	if ((*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 冠零牢 版快俊绰 SetGroundCreature沁带芭 锭概 促矫 m_MoveType甫 官层霖促
		m_MoveType = CREATURE_FLYING;

	if (m_MoveType == CREATURE_FLYING)
	{
		m_Z=(*g_pCreatureTable)[m_CreatureType].FlyingHeight;
	}
}

//----------------------------------------------------------------------
// Set NextAction
//----------------------------------------------------------------------
// 促澜俊 且 青悼阑 汲沥茄促.
//----------------------------------------------------------------------
void			
MCreature::SetNextAction(BYTE action)
{
	m_NextAction = action;
}

//----------------------------------------------------------------------
// Set NextAction to Move
//----------------------------------------------------------------------
// 促澜 action俊绰 move茄促.
//----------------------------------------------------------------------
void	
MCreature::SetNextActionToMove()
{
	m_NextAction = m_MoveAction;
}

//----------------------------------------------------------------------
// Set CutHeight From
//----------------------------------------------------------------------
void				
MCreature::SetCutHeightFrom(int startCount, int lastCount, int inc)
{
	m_bCutHeight = true;
	m_CutHeightCountLast = lastCount;
	m_CutHeightCount = startCount;
	m_CutHeightCountInc = inc;
}

//----------------------------------------------------------------------
// Update CutHeight
//----------------------------------------------------------------------
void				
MCreature::UpdateCutHeight()
{
	if (m_bCutHeight)
	{
		m_CutHeightCount += m_CutHeightCountInc;

		if (m_CutHeightCountInc < 0 && m_CutHeightCount <= m_CutHeightCountLast)
		{
			m_bCutHeight = false;
			m_CutHeightCount = 0;
			m_CutHeightCountInc = 0;
		}

		else if (m_CutHeightCountInc > 0 && m_CutHeightCount >= m_CutHeightCountLast)
		{
			m_bCutHeight = false;
			m_CutHeightCount = m_CutHeightCountLast;
			m_CutHeightCountInc = 0;
		}	
		
		// 葫葫 倒赴促.
		m_Direction = ((m_Direction+1) & 0x07);		

		if( GetCreatureType() == 702 || GetCreatureType() == 703 || GetCreatureType() == 704 )
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)this;
			pFakeCreature->SyncTurretDirection();
		}
	}
}

//----------------------------------------------------------------------
// Update Turning
//----------------------------------------------------------------------
void				
MCreature::UpdateTurning()
{
	if (m_bTurning)
	{
		if (--m_TurningCount <= 0)
		{
			m_bTurning = false;
		}
		else
		{
			// 葫葫 倒赴促.
			m_Direction = ((m_Direction+1) & 0x07);
		}
	}
}

//----------------------------------------------------------------------
// Update CauseCriticalWounds
//----------------------------------------------------------------------
void				
MCreature::UpdateCauseCriticalWounds()
{
	if(m_bCauseCriticalWounds)
	{
		if(IsDead())
		{
			StopCauseCriticalWounds();
			SetStop();
			return;
		}
		if(--m_CauseCriticalWoundsCount < 0)
		{
			StopCauseCriticalWounds();
			SetStop();
		}
		if(m_CauseCriticalWoundsCount < 10 && m_CauseCriticalWoundsCount >= 0)
			m_ActionCount = m_CauseCriticalWoundsCount;
		else if(m_ActionCount < 10)
			m_ActionCount++;

		if(m_CreatureType == 793) // 浇饭捞绢 力拱 -_-;
			m_ActionCount = 20;

	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateCCW] bCCW : %d CCWCount : %d Action : %d ActionCount : %d", m_bCauseCriticalWounds, m_CauseCriticalWoundsCount, m_Action, m_ActionCount);
	#endif
	}
}

//----------------------------------------------------------------------
// Update Ghost
//----------------------------------------------------------------------
void				
MCreature::UpdateGhost()
{
//	DEBUG_ADD_FORMAT("[UpdateGhost] %s, %d", m_fGhost?"true":"false", m_GhostCount);
	if(m_fGhost)
	{
		if(--m_GhostCount < 0)
		{
			StopGhost();
			//SetStop();
		}
	}
}

bool MCreature::IsBloodyZenith()
{
//	bool bActionAttack = 
//	{
//		m_Action == ACTION_ATTACK ||
//		m_Action == ACTION_VAMPIRE_ATTACK_FAST ||
//		m_Action == ACTION_VAMPIRE_ATTACK_SLOW
//	};

	return m_bBloodyZenith;//  && bActionAttack; 
}

int MCreature::GetBloodyZenithCount()
{
	return m_BloodyZenithCount;
}

//----------------------------------------------------------------------
// Update BloodyZenith
//----------------------------------------------------------------------
void				
MCreature::UpdateBloodyZenith()
{
	if(IsBloodyZenith() && !IsFastMove())
	{
		if(--m_BloodyZenithCount < 0)
		{
			StopBloodyZenith();
//			char TempBuffer[256];
//			sprintf(TempBuffer,"喉矾叼 力聪胶 场 a:%d, n:%d, n:%d, z:%d,s:%d", m_nUsedActionInfo, m_NextAction, m_nNextUsedActionInfo,MAGIC_BLOODY_ZENITH,SKILL_SET_AFIRE);
//			
//			g_pSystemMessage->Add(TempBuffer);
//			SetStop();
		}
	}
}

//----------------------------------------------------------------------
// Update GunShotGuidance
//----------------------------------------------------------------------
void				
MCreature::UpdateGunShotGuidance()
{
	if(m_bGunShotGuidance)
	{
		if(IsDead())
		{
			StopGunShotGuidance();
			SetStop();
			return;
		}
		if(--m_GunShotGuidanceCount < 0)
		{
			StopGunShotGuidance();
			SetStop();
		}
		if(m_GunShotGuidanceCount < 8 && m_GunShotGuidanceCount >= 0)
			m_ActionCount = m_GunShotGuidanceCount;
		else if(m_ActionCount < 8)
			m_ActionCount++;
		
	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateCCW] bCCW : %d CCWCount : %d Action : %d ActionCount : %d", m_bGunShotGuidance, m_GunShotGuidanceCount, m_Action, m_ActionCount);
	#endif
	}
}

//----------------------------------------------------------------------
// Set FakeDie
//----------------------------------------------------------------------
void				
MCreature::SetFakeDie()
{
	m_bFakeDie = true;
	m_FakeDieCount = 0;
}

//----------------------------------------------------------------------
// Update CauseCriticalWounds
//----------------------------------------------------------------------
void				
MCreature::UpdateFakeDie()
{
	if(m_bFakeDie)
	{
		++m_FakeDieCount;
//		if( < 0)
//		{
//			StopFakeDie();
//		}
	}
}

//----------------------------------------------------------------------
// Set Invisible
//----------------------------------------------------------------------
void				
MCreature::SetInvisible()
{
	m_InvisibleCount = 0;
	m_InvisibleStep = 1;
}

//----------------------------------------------------------------------
// Set Invisible
//----------------------------------------------------------------------
void				
MCreature::SetInvisibleSoon()
{
	m_InvisibleCount = 64;
	m_InvisibleStep = 0;
}

//----------------------------------------------------------------------
// Set Visible
//----------------------------------------------------------------------
void				
MCreature::SetVisible()
{
	m_InvisibleCount = 64;
	m_InvisibleStep = -1;
}

//----------------------------------------------------------------------
// Set Visible
//----------------------------------------------------------------------
void				
MCreature::SetVisibleSoon()
{
	m_InvisibleCount = 0;
	m_InvisibleStep = 0;
}

//----------------------------------------------------------------------
// Set SameBody
//----------------------------------------------------------------------
void		
MCreature::SetSameBody(const MCreature* pCreature)
{
	SetCreatureType( pCreature->GetCreatureType() );

	if (GetClassType()!=CLASS_FAKE)
	{
		m_MoveType = pCreature->m_MoveType;
	}

//	if(pCreature->IsAdvancementClass())
//		SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, pCreature->GetAdvancementClassLevel() );
	m_Z	= pCreature->m_Z;

	m_ColorBody1 = pCreature->m_ColorBody1;
	m_ColorBody2 = pCreature->m_ColorBody2;
}

//----------------------------------------------------------------------
// Set Action
//----------------------------------------------------------------------
// 茄 Action付促 Frame荐啊 促甫 巴捞促.. 
// 捞芭绰 file肺 哗辑 沥焊甫 滴绰霸 唱阑 巴牢单
// FRAME_PACK俊辑 佬绢客具 茄促.
//----------------------------------------------------------------------
void
MCreature::SetAction(BYTE action)
{ 	
//	if ( m_nUsedActionInfo == MAGIC_BLOODY_ZENITH && m_Action == ACTION_ATTACK &&
//		m_ActionCount >= m_ActionCountMax && GetActionDelay() > 0)
//	{
//		SetActionDelay( GetActionDelay() - 1 );
//		m_ActionCount = m_ActionCountMax - 2;
//		return;
//	}

	// 公炼扒 波霖促.
	m_bFastMove = false;

	if (!m_bAlive
		&& action!=ACTION_DIE)
	{		
		return;
	}
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET) && action != ACTION_STAND && action != ACTION_ATTACK)
		return;

	
	if(GetCreatureType() == 782 && action == ACTION_ATTACK)
		action = ACTION_VAMPIRE_DRAIN;
//	if(action>ACTION_OUSTERS_MAGIC_ATTACK)
//		int i = 0;
	//-----------------------------------------------------------------
	// 泅犁 葛嚼狼 action俊 嘎绰 版快. 2001.10.5
	//-----------------------------------------------------------------
	if (action < GetActionMax())
	{
		
		//-----------------------------------------------------
		// 坷配官捞 鸥绊 乐阑锭
		//-----------------------------------------------------
		if (m_MoveDevice==MOVE_DEVICE_RIDE)
		{
			if (IsSlayer())
			{
				if (action==ACTION_STAND)
				{
					action = ACTION_SLAYER_MOTOR_STAND;
				}
				else if (action==ACTION_MOVE)
				{
					action = ACTION_SLAYER_MOTOR_MOVE;
				}
			}
		} else
			//-----------------------------------------------------
			// 吧绢促匆锭
			//-----------------------------------------------------
		{
			if (IsSlayer())
			{
				if (action==ACTION_SLAYER_MOTOR_STAND)
				{
					action = ACTION_STAND;
				}
				else if (action==ACTION_SLAYER_MOTOR_MOVE)
				{
					action = ACTION_MOVE;
				}
			}
		}
		if( IsOusters() && (HasEffectStatusSummonSylph( this ) || m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH))
		{
			if (action==ACTION_STAND)
			{
				action = ACTION_OUSTERS_FAST_MOVE_STAND;
			} else
			if( action == ACTION_MOVE )
			{
				action = ACTION_OUSTERS_FAST_MOVE_MOVE;
			}
		}
				
		m_bNextAction = false;
			
		//------------------------------------------------
		// Move牢 版快
		//------------------------------------------------
		if (action==m_MoveAction)//ACTION_MOVE || action==ACTION_SLAYER_MOTOR_MOVE)
		{
			// 弊成 辑乐绰 版快搁... 场悼累栏肺 父电促.
			// Action捞 场车促绊 钎矫秦林扁 困秦辑..
			if (m_Action==ACTION_STAND 
				|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
			{			
				m_ActionCount = 0;
				m_ActionCountMax = 0;			
			}		

			// Action捞 场抄 版快父 Action阑 Move肺 茄促.
			// 酒囱 版快绰.. Action捞 场唱绊 Move肺 官诧促.
			if (m_ActionCount>=GetActionCountMax())
			{
				m_Action = action;
			}		

			// 沥瘤
			m_sX = 0;
			m_sY = 0;

			// Server俊辑 八刘等 困摹肺 捞悼矫挪促.
			//MovePosition( m_ServerX, m_ServerY );
			
			// [ TEST CODE ]
			// Action阑 绝矩促.
			// --> 救 绝局搁.. Action捞 场唱绊 Move啊 荐青等促. 
			// 弊繁单, Move啊 救蹬辑 谅钎啊 壶唱哎 犬伏捞 乐促. 褥!!
			//m_ActionCount = 0;
			//m_ActionCountMax = 0;

			m_RepeatCount = 0;

			// Move 霉 悼累
			m_MoveCount = 0;
			m_NextMoveCount = 0;
			
			//m_MoveCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( action );
			m_MoveCountMax = GetCreatureActionCountMax( this, action );
				//g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
				//							[action][m_CurrentDirection].GetSize();//ACTIONCOUNT_MOVE;

		}
		//------------------------------------------------
		// Action牢 版快
		//------------------------------------------------
		else
		{
			//------------------------------------------------
			// extreme篮 漂沥 悼累俊辑 钱赴促.
			//------------------------------------------------
//			if (HasEffectStatus(EFFECTSTATUS_EXTREME))
//			{
//				if (action==ACTION_MAGIC
//					|| action==ACTION_DIE
//					|| action==ACTION_VAMPIRE_DRAIN
//					|| action==ACTION_DRAINED)
//				{
//					RemoveEffectStatus(EFFECTSTATUS_EXTREME);
//				}
//			}

			if (// 坷配官捞 鸥绊 乐阑锭绰 action 救 焊咯霖促.
				m_MoveDevice==MOVE_DEVICE_RIDE && action!=ACTION_SLAYER_MOTOR_STAND
				// damaged牢 版快..
				|| action==ACTION_DAMAGED 						
						&& (// 傍拜罐绰 吝捞 酒聪芭唱 沥瘤 悼累捞 酒聪搁 傍拜 罐绰 葛嚼 钎泅 救 茄促
							m_Action!=ACTION_DAMAGED && m_Action!=ACTION_STAND && m_Action!=ACTION_MOVE
						//&& (// 软趋 寸窍绰 悼累 吝捞芭唱 
						//	m_Action==ACTION_DRAINED
							// 轨颇捞绢老锭, 软趋窍绰 悼累 吝俊绰 damaged甫 救 焊咯霖促.
						//	|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							)
				)
			{
				m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
				return;
			}

			
			// 2005, 1, 6, sobeit add start - motor ride move 窍带吝 stop 啊 朝扼坷搁 茄锅篮 公矫..
			if (IsAdvancementClass() && m_MoveDevice==MOVE_DEVICE_RIDE && action == ACTION_SLAYER_MOTOR_STAND&& m_Action==ACTION_SLAYER_MOTOR_MOVE)
			{
				if(!m_IsSkipMotorStand)
				{
					m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
					m_IsSkipMotorStand = true;
					return;
				}
				else
					m_IsSkipMotorStand = false;
			}
			// 2005, 1, 6, sobeit add end
			m_ActionCountMax = GetCreatureActionCountMax( this, action );//(*g_pCreatureTable)[m_CreatureType].GetActionCount( action );

			if (m_ActionCountMax==0)
			{
				// 悼累捞 绝绰 版快
				m_Action = (m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND;
				m_ActionCount = 0;		
				m_RepeatCount = 0;
				//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
				m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );
				
				// 关俊 秦躇辨贰 老窜 秦初绰单.. 购瘤 隔摆促 - -; 2002.1.10
				m_nSpecialActionInfo = ACTIONINFO_NULL;
			}
			else
			{
				m_Action = action;			

				//---------------------------------------------
				// action阑 馆汗窍绰 版快
				//---------------------------------------------
				// 馆汗 action狼 矫累何磐 焊咯霖促.
				// 弊矾唱, 促澜 馆汗 packet捞 攫力 棵瘤 葛福扁 锭巩俊..
				// ActionCounMax绰 公炼扒 场~frame栏肺 沥茄促.
				//---------------------------------------------
//				BOOL bSlayer = IsSlayer();

				if (m_RepeatCount!=0	//m_Action == action			
					&& action!=ACTION_STAND
					&& (!IsSlayer() || IsSlayer() && action!=ACTION_SLAYER_MOTOR_STAND)
					//&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame())
					)
				{
					// count 汲沥..
					
					m_ActionCount = GetActionInfoRepeatStartFrame( m_nSpecialActionInfo );
//					char Msg[128];
//					sprintf(Msg, "%d" , m_ActionCount);
//					g_pSystemMessage->Add(Msg);	
				}
				//---------------------------------------------
				// 老馆利牢 版快..
				//---------------------------------------------
				else
				{			
					// count 汲沥..
					m_ActionCount = 0;		
					m_RepeatCount = 0;

					// 2004, 11, 12, sobeit modify start - block
					// 捞芭 构瘤? 恐 秦躇绰瘤 葛福摆匙.. - -;; 2002.1.10
				//	m_nSpecialActionInfo = ACTIONINFO_NULL;	
					// 2004, 11, 12, sobeit modify end - block
				}				
			}
		}
	}
	// 2004, 11, 12, sobeit add start - block
	if(m_Action == ACTION_STAND)	
		m_nSpecialActionInfo = ACTIONINFO_NULL;	
	// 2004, 11, 12, sobeit add end - block

	// 悼累阑 矫累且锭 何啊利栏肺 嘿绰 Effect 甸.
	// 绢凋啊肺 哗具 窍绰单.-_-
	if( HasEffectStatus( EFFECTSTATUS_REDIANCE ) )
	{
		if( 
			( m_Action == ACTION_SLAYER_SWORD_2 || m_Action == ACTION_SLAYER_SWORD_2_SLOW || m_Action == ACTION_SLAYER_SWORD_2_FAST ) 
			)
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		} 
		else if(
			( m_Action == ACTION_SLAYER_SWORD || m_Action == ACTION_SLAYER_SWORD_FAST || m_Action == ACTION_SLAYER_SWORD_SLOW ) )
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL_ATTACK_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		}				
	}

//	// 2004, 9, 8, sobeit add start - 档浇 130 胶懦 包访
//	else if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4)
//			)
//	{
//		if(m_Action == ACTION_SLAYER_BLADE)
//		{
//			if(m_ActionCount>4)
//				m_ActionCount = 5;
//		}
//		else
//			m_Action = ACTION_SLAYER_BLADE;
//	
//	}
//	// 2004, 9, 8, sobeit add end - 档浇 130 胶懦 包访
	m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
}

//----------------------------------------------------------------------
// Clear Chat String
//----------------------------------------------------------------------
void
MCreature::ClearChatString()
{
	// Chatting String
	m_ChatStringCurrent = 0;
	for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i][0] = NULL;
	}
	m_HeadSkin = 0;

//	if( CurPernalShop() == 1 && m_NextChatFadeTime != 0 )
	if( CurPernalShop() == 1 && GetInputChatting() == true)
	{
		SetInputChatting(false);
		
		if( m_PersnalMessage.empty() )
			SetPersnalString( (*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString() );
		else
			SetPersnalString( (char*)m_PersnalMessage.c_str(), g_pUserOption->ChattingColor );
	}
}

void		
MCreature::SetPersnalString(char *str, COLORREF color)
{
	if (str==NULL)
	{
		DEBUG_ADD("[SetPersnalString] str == NULL");
		return;
	}

	DEBUG_ADD_FORMAT("[SetPersnalString] %s", str);

//if(CurPernalShop() == 0 )
	ClearChatString();

	int startIndex = 0,
		endIndex = 0;

	m_NextChatFadeTime = 0;
	SetInputChatting(false);

	DEBUG_ADD("[SetPersnalString] before while");
	int len = strlen(str);
	while (endIndex < len)
	{
		endIndex = startIndex + g_pClientConfig->MAX_CHATSTRING_LENGTH;

		// 农府胶付胶 飘府侩 窍靛 内爹
		bool bTree = false;
		char *find = strchr(str+startIndex, '\n'); 
		if(find != NULL)
		{
			*find = '\0';
			endIndex = strlen(str+startIndex)+startIndex;
			bTree = true;
		}
		
		// len捞 楼甫父茄 辨捞档 救登搁...
		if (endIndex >= len)
		{
			endIndex = len;

			// 老沥 俺荐狼 String阑 copy茄促.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH俺甫 楼甫 荐 绝绰啊?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 鳖瘤 strcpy
			char	*pSource=str+startIndex, 
					*pDest	=m_ChatString[m_ChatStringCurrent];
			for (int i=startIndex; i<endIndex; i++)
			{
				*pDest++ = *pSource++;
			}
			*pDest = NULL;

			if(bTree)
			{
				endIndex++;
			}
		}

		// 促澜 临..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index甫 官槽促.
		startIndex = endIndex;
	}
	
	// 盲泼 String捞 Delay瞪 矫埃阑 瘤沥秦霖促.	
//	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;
//	m_NextChatFadeTime = g_CurrentTime + 0xFFFFFFFF;

	// 盲泼 祸彬
//	m_ChatColor = RGB(50, 50, 200);//RGB_WHITE;//0xFFFF;
//	m_OriChatColor = m_ChatColor;
	
	m_ChatColor = color;
	m_OriChatColor |= 0xFF000000;
//BYTE b = (m_OriChatColor & 0xFF000000) >> 24;
	m_ChatTime = g_CurrentFrame;

//	if(CurPernalShop() == 0 )
//		ClearChatString();

	DEBUG_ADD("[SetPersnalString] Excute ok");
	for(int i = 0; i < m_ChatStringCurrent; i++)
	{
		DEBUG_ADD_FORMAT("%s", m_ChatString[i]);
	}
	DEBUG_ADD("[SetPersnalString] Excute ok 2");
}
//----------------------------------------------------------------------
// Set ChatString
//----------------------------------------------------------------------
// String阑 眠啊茄促. 场俊~..
//----------------------------------------------------------------------
void		
MCreature::SetChatString(char *str, COLORREF color)
{
	if (str==NULL)
	{
		DEBUG_ADD("[SetChatString] str == NULL");
		return;
	}

//	if(CurPernalShop() == 1)
//		return;

	DEBUG_ADD_FORMAT("[SetChatString] %s", str);

	if(g_pUserOption->ChatWhite)
		color = RGB_WHITE;

	SetInputChatting(false);
	ClearChatString();
	SetInputChatting(true);



	int startIndex = 0,
		endIndex = 0;

//	size_t cur = -1;
//	std::string temp;
//	temp = str;;
//	
//	while((cur= temp.find("\n")) != -1)
//	{
//		temp.replace(temp.begin() + cur , temp.begin() + cur+2, "\0");
//	}
//
//	str = (char*)temp.c_str();

	// 农府胶付胶 飘府侩 窍靛内爹
	char *szTreeFrom = NULL;
	if( GetCreatureType() == 482 || GetCreatureType() == 650 )
	{
		if(GetCreatureType() == 482)
		{
			DEBUG_ADD("[SetChatString] Tree!!!");
			
			char *find = strchr(str, '%');
			if(find == NULL)
			{
				DEBUG_ADD("[SetChatString] find == NULL");
				
				return;
			}
			
			*find = '\0';
			
			strcpy(m_ChatString[m_ChatStringCurrent], "Dear. ");
			strcat(m_ChatString[m_ChatStringCurrent], str);					
			
			m_ChatStringCurrent++;
			
			str = find+1;
		}
		char *find2 = strchr(str, '%');
		if(find2 == NULL)
		{
			DEBUG_ADD("[SetChatString] find2 == NULL");

			return;
		}

		*find2 = '\0';

		szTreeFrom = find2+1;
	}    
	

	int len = strlen(str);
	
	if(strstr(str, "硅枚键聪促") != NULL)
	{
		m_HeadSkin = 215;
	}
	else if(strstr(str, "秦榜官啊瘤") != NULL)
	{
		m_HeadSkin = 149;
	}
	else if(strstr(str, "戳措牢埃") != NULL)
	{
		m_HeadSkin = 397;
	}
	else if(strstr(str, "催措啊府") != NULL)
	{
		m_HeadSkin = 221;
	}
	else if(strstr(str, "啊搁扼捞歹") != NULL)
	{
		m_HeadSkin = 316;
	}


	// 茄 巩厘狼 String阑 利例茄 辨捞肺 肋扼霖促~~
	DEBUG_ADD("[SetChatString] before while");
	while (endIndex < len)
	{
		endIndex = startIndex + g_pClientConfig->MAX_CHATSTRING_LENGTH;

		// 农府胶付胶 飘府侩 窍靛 内爹
		bool bTree = false;
		if(GetCreatureType() == 482 || GetCreatureType() == 650 )
		{
			char *find = strchr(str+startIndex, '\n'); 
			if(find != NULL)
			{
				*find = '\0';
				endIndex = strlen(str+startIndex)+startIndex;
				bTree = true;
			}
		}
		
		// len捞 楼甫父茄 辨捞档 救登搁...
		if (endIndex >= len)
		{
			endIndex = len;

			// 老沥 俺荐狼 String阑 copy茄促.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH俺甫 楼甫 荐 绝绰啊?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 鳖瘤 strcpy
			char	*pSource=str+startIndex, 
					*pDest	=m_ChatString[m_ChatStringCurrent];
			for (int i=startIndex; i<endIndex; i++)
			{
				*pDest++ = *pSource++;
			}
			*pDest = NULL;

			if(bTree)
			{
				endIndex++;
			}
		}

		// 促澜 临..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index甫 官槽促.
		startIndex = endIndex;
	}
	DEBUG_ADD("[SetChatString] while ok");

	// 农府胶付胶 飘府侩 窍靛内爹
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		DEBUG_ADD("[SetChatString] Tree code");

		int len = strlen(szTreeFrom) + 6;

		char szTemp[512];
		memset(szTemp, ' ', g_pClientConfig->MAX_CHATSTRING_LENGTH);

		strcpy(szTemp + max(g_pClientConfig->MAX_CHATSTRING_LENGTH - len, 0), "From. ");
		strcat(szTemp, szTreeFrom);

		strcpy(m_ChatString[m_ChatStringCurrent], szTemp);

		// 促澜 临..
		m_ChatStringCurrent++;

		DEBUG_ADD("[SetChatString] Tree code ok");
	}
	

	// 盲泼 String捞 Delay瞪 矫埃阑 瘤沥秦霖促.	
	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;

	// 盲泼 祸彬
	m_ChatColor = color;//RGB_WHITE;//0xFFFF;
	m_OriChatColor = m_ChatColor;
	
	// 农府胶付胶 飘府侩 窍靛 内爹
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		m_NextChatFadeTime = 0;
	}
	else
	{
		m_OriChatColor |= 0xFF000000;
	}

	// 富茄 矫埃 汲沥
	m_ChatTime = g_CurrentFrame;
	

	DEBUG_ADD("[SetChatString] Excute ok");
	for(int i = 0; i < m_ChatStringCurrent; i++)
	{
		DEBUG_ADD_FORMAT("%s", m_ChatString[i]);
	}
	DEBUG_ADD("[SetChatString] Excute ok 2");
}

//----------------------------------------------------------------------
// Fade ChatString
//----------------------------------------------------------------------
// ChatString阑 绢涤霸 茄促.
//----------------------------------------------------------------------
void		
MCreature::FadeChatString()
{
	/*
	m_ChatString[m_ChatStringCurrent][0] = NULL;	

	m_ChatStringCurrent++;
	if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

	// 盲泼 String捞 Delay瞪 矫埃阑 瘤沥秦霖促.	
	m_NextChatFadeTime = g_CurrentTime + DELAY_FADECHATSTRING;
	*/

//	MCreature* pCreature = g_pZone->GetCreature(GetID());
//	if(CurPernalShop() == 1 && m_NextChatFadeTime+3000 >= g_CurrentTime)
//	{
//		if(pCreature->GetPersnalShopMessage().length()>0)
//			pCreature->SetPersnalString((char*)g_pPlayer->GetPersnalShopMessage().c_str(),g_pUserOption->ChattingColor);
//		else
//			pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);
//	}
//	else if(CurPernalShop() == 1)
//	{
//		return;
//	}

//	if (m_NextChatFadeTime >= g_CurrentTime || ( CurPernalShop() == 1 && m_NextChatFadeTime == 0 ) )
	if (m_NextChatFadeTime >= g_CurrentTime || ( CurPernalShop() == 1 && GetInputChatting() == false ) )
		return;

	BYTE b = (m_OriChatColor & 0xFF000000) >> 24;

	BYTE color[3] ={ m_OriChatColor & 0xFF, (m_OriChatColor & 0xFF00)>>8, (m_OriChatColor & 0xFF0000)>>16};


	if (b>=g_pClientConfig->MIN_CHATSTRING_COLOR256)
	{
		// 农府胶付胶 飘府侩 窍靛内爹
		if(GetCreatureType() == 482 || GetCreatureType() == 650)
		{
			b -= g_pClientConfig->VALUE_CHATSTRING_FADE*4;
		}
		else
		{
			b -= g_pClientConfig->VALUE_CHATSTRING_FADE;
			if(b > 0)
			{
				color[0] = color[0]*b/0xFF;
				color[1] = color[1]*b/0xFF;
				color[2] = color[2]*b/0xFF;
			}

		}
	}
	else
	{
		// 农府胶付胶 飘府侩 窍靛内爹
		if(GetCreatureType() == 482 || GetCreatureType() == 650)
		{
			const int treeColorMax = 18;
			static COLORREF treeColor[treeColorMax] = 
			{
				RGB(255, 150, 150),
				RGB(150, 255, 150),
				RGB(150, 150, 255),
				RGB(255, 255, 150),
				RGB(150, 255, 255),
				RGB(255, 150, 255),

				RGB(255, 200, 150),
				RGB(200, 255, 150),
				RGB(200, 150, 255),
				RGB(255, 255, 200),
				RGB(200, 255, 255),
				RGB(255, 200, 255),

				RGB(255, 150, 200),
				RGB(150, 255, 200),
				RGB(150, 200, 255),

				RGB(255, 200, 200),
				RGB(200, 255, 200),
				RGB(200, 200, 255),
			};

			// 盲泼 祸彬
			COLORREF color = 0;
			
			do
			{
				color = treeColor[rand()%treeColorMax];
			}while(color == (m_OriChatColor&0x00FFFFFF));
			
			m_ChatColor = color;//RGB_WHITE;//0xFFFF;
			m_OriChatColor = m_ChatColor;
			m_OriChatColor |= 0xFF000000;
			if(GetCreatureType() == 650 )
				m_NextChatFadeTime = g_CurrentTime + 0xffffff;
			
			return;
		}
		else
		{
			ClearChatString();
		}
	}

	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_FADE;
	
	m_ChatColor = RGB(color[0], color[1], color[2]);
	m_OriChatColor = (m_OriChatColor&0xFFFFFF) | (b<<24);
}


//----------------------------------------------------------------------
// Get ChatString
//----------------------------------------------------------------------
// 0 ~ g_pClientConfig->MAX_CHATSTRING-1
// 0捞 啊厘 坷贰等 String捞绊 g_pClientConfig->MAX_CHATSTRING-1捞 啊厘 弥辟俊 巴栏肺
// return秦具 茄促.
//----------------------------------------------------------------------
const char*	
MCreature::GetChatString(BYTE i)
{ 
	//                          i   = 角力肺 return登绢具 窍绰 蔼
	//m_ChatStringCurrent - (3-[0]) = m_ChatStringCurrent;
	//m_ChatStringCurrent - (3-[1]) = m_ChatStringCurrent - 2;
	//m_ChatStringCurrent - (3-[2]) = m_ChatStringCurrent - 1;

	BYTE gap = g_pClientConfig->MAX_CHATSTRING - i;

	if (gap > m_ChatStringCurrent)
	{
		return m_ChatString[m_ChatStringCurrent + g_pClientConfig->MAX_CHATSTRING - gap];
	}
	
	return m_ChatString[m_ChatStringCurrent - gap];	
}

//----------------------------------------------------------------------
// Check AffectStatus
//----------------------------------------------------------------------
// pItem阑 this Creature啊 荤侩且 荐 乐绰瘤狼 咯何甫 眉农秦辑 汲沥茄促.
//----------------------------------------------------------------------
void	
MCreature::CheckAffectStatus(MItem* pItem)
{
	if(pItem->GetItemClass() == ITEM_CLASS_PET_ITEM)
	{
		int timeGap = (timeGetTime() - ((MPetItem *)pItem)->GetUpdateTime())/1000/60;
		
		if(pItem->GetCurrentDurability() <= timeGap )
		{
			pItem->UnSetAffectStatus();
			return;
		}
	}

	//-----------------------------------------------------
	// Slayer
	//-----------------------------------------------------
	switch(GetRace())
	{
	case RACE_SLAYER:
		if (pItem->IsSlayerItem())
		{
			//-----------------------------------------------------
			// 浇饭捞绢牢 版快绰 STR, DEX, INT眉农
			//-----------------------------------------------------
			int reqSTR = pItem->GetRequireSTR();
			int reqDEX = pItem->GetRequireDEX();
			int reqINT = pItem->GetRequireINT();
			int reqSUM = pItem->GetRequireSUM();
			
			int str = GetSTR();
			int dex = GetDEX();
			int inte = GetINT();
			int sum = str+dex+inte;
			
			//-----------------------------------------------------
			// 力茄 荐摹啊 绝芭唱 
			// 漂沥 荐摹甫 窍唱 父练窍绰 版快..
			//-----------------------------------------------------
			if (
				//-----------------------------------------------------
				// 己喊 八荤
				//-----------------------------------------------------
				(pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()
				)
				
				//-----------------------------------------------------
				// 鞘夸瓷仿摹啊 绝芭唱..
				//-----------------------------------------------------
				&& (
				
				//-----------------------------------------------------
				// 乐绰版快.. 促 父练矫难具 茄促.
				//-----------------------------------------------------
				(reqSTR==0 || reqSTR!=0 && str >= reqSTR)
				&&	(reqDEX==0 || reqDEX!=0 && dex >= reqDEX)
				&&	(reqINT==0 || reqINT!=0 && inte >= reqINT)
				&&	(reqSUM==0 || reqSUM!=0 && sum >= reqSUM)
				)					
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			
			if((pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()) &&
				pItem->IsQuestItem() )
			{
				pItem->SetAffectStatus();
			}
		}
		else
		{
			//-----------------------------------------------------
			// 鸥辆练 酒捞袍 荤侩 阂啊
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_VAMPIRE:
		if (pItem->IsVampireItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// 轨颇捞绢牢 版快绰 level肺 眉农茄促.
			//-----------------------------------------------------
			if (
					//-----------------------------------------------------
					// 己喊 八荤
					//-----------------------------------------------------
					(pItem->IsGenderForAll() 
						|| pItem->IsGenderForMale() && IsMale()
						|| pItem->IsGenderForFemale() && IsFemale()
					)
					&&
					(reqLevel==0 || reqLevel!=0 && GetLEVEL() >= reqLevel)
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			if((pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()) &&
				pItem->IsQuestItem()  )
			{
				pItem->SetAffectStatus();
			}
			

			//-----------------------------------------------------
			// item 辆幅俊 蝶扼辑 skill 眉农茄促.
			//-----------------------------------------------------
			// g_pSkillAvailable篮 player狼 skill沥焊捞促.
			// 绢瞒乔 促弗 creature啊 CheckAffectStatus()甫 荤侩且 老篮 绝瘤父.. --;
			// 老窜篮 item level眉农肺父 场郴档废 茄促...
			/*
			switch (pItem->GetItemClass())
			{
				//-----------------------------------------------------
				// Portal Item
				//-----------------------------------------------------
				case ITEM_CLASS_VAMPIRE_PORTAL_ITEM :
					if (((MVampirePortalItem*)pItem)->IsMarked())
					{
						if (g_pSkillAvailable->find( MAGIC_BLOODY_TUNNEL ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
					else
					{
						if (g_pSkillAvailable->find( MAGIC_BLOODY_MARK ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
				break;

				//-----------------------------------------------------
				// Vampire ETC
				//-----------------------------------------------------
				case ITEM_CLASS_VAMPIRE_ETC :
					if (pItem->GetItemType()==0)
					{
						if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_WOLF ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					} 
					else if (pItem->GetItemType()==1)
					{
						if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_BAT ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
				break;
			}
			*/
		}
		else
		{
			//-----------------------------------------------------
			// 鸥辆练 酒捞袍 荤侩 阂啊
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_OUSTERS:
		if (pItem->IsOustersItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// 浇饭捞绢牢 版快绰 STR, DEX, INT眉农
			//-----------------------------------------------------
			int reqSTR = pItem->GetRequireSTR();
			int reqDEX = pItem->GetRequireDEX();
			int reqINT = pItem->GetRequireINT();
			int reqSUM = pItem->GetRequireSUM();
			
			int str = GetSTR();
			int dex = GetDEX();
			int inte = GetINT();
			int sum = str+dex+inte;

			//-----------------------------------------------------
			// 酒快胶磐令牢 版快档-_- level肺 眉农茄促.
			//-----------------------------------------------------
			if (
				(reqLevel==0 || reqLevel!=0 && GetLEVEL() >= reqLevel)
				&&	(reqSTR==0 || reqSTR!=0 && str >= reqSTR)
				&&	(reqDEX==0 || reqDEX!=0 && dex >= reqDEX)
				&&	(reqINT==0 || reqINT!=0 && inte >= reqINT)
				&&	(reqSUM==0 || reqSUM!=0 && sum >= reqSUM)
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			if(	pItem->IsQuestItem() )
			{
				pItem->SetAffectStatus();
			}

		}
		else
		{
			//-----------------------------------------------------
			// 鸥辆练 酒捞袍 荤侩 阂啊
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;
	}
}

//----------------------------------------------------------------------
// Set Direction To Creature
//----------------------------------------------------------------------
// 促弗 Creature甫 氢秦辑 官扼夯促.
//----------------------------------------------------------------------
void	
MCreature::SetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// 鞍篮 谅钎扼搁.. 泅犁 规氢栏肺 敌促.
	if (m_X==sX && m_Y==sY)
	{
		m_Direction = m_CurrentDirection;
	}
	else
	{
		m_Direction = MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
	}
}

//----------------------------------------------------------------------
// Change Near Direction
//----------------------------------------------------------------------
// m_CurrentDirection阑 m_Direction栏肺 磊楷胶反霸 官槽促.
// 弊矾唱, 泅犁 框流捞绰 规氢(m_DirectionMoved)捞 快急捞促.
//----------------------------------------------------------------------
bool	
MCreature::ChangeNearDirection()
{
	int toDir;
	
	// action阑 焊咯林绰 吝捞搁.. 规氢 汲沥
	if (m_ActionCount<GetActionCountMax())
	{
		toDir = m_Direction;
	}
	// 框流捞绰 吝捞搁 m_DirectionMoved
	// 窜, KnockBack捞唱 FastMove牢 版快俊绰... 规氢 公矫..
	else if (m_MoveCount>0 && m_MoveCount<m_MoveCountMax
		&& !m_bKnockBack
		&& !m_bFastMove)		
	{
		toDir = m_DirectionMoved;
	}
	// 酒聪搁.. m_Direction
	else
	{
		toDir = m_Direction;
	}

	// 鞍栏搁 官曹 鞘夸啊 绝促.
	if (m_CurrentDirection==toDir)
	{
		return false;
	}

	// 促福搁.. 
	// 哭率栏肺 倒芭唱 坷弗率栏肺 倒霸 秦具茄促.

	// 剧荐肺 父甸扁 困秦辑.. 臂绊.. 加档甫 困秦辑.. &甫 荤侩沁促.
	int gap = (((m_CurrentDirection+MAX_DIRECTION) - toDir) & 0x07);//% MAX_DIRECTION;

	// 4, 7
	if (gap==4)
	{
		// 哭率捞唱 坷弗率栏肺 random窍霸 捣促.
		if (rand() & 0x01)
		{
			m_CurrentDirection ++;			
		}
		else
		{
			m_CurrentDirection += 7;			
		}
	}
	// 
	else if (gap<4)
	{
		m_CurrentDirection += 7;		
	}
	else
	{
		m_CurrentDirection ++;		
	}

	// 0~7阑 蜡瘤窍档废 茄促.
	m_CurrentDirection &= 0x07;

	return true;	
}

//----------------------------------------------------------------------
// Get Counter Direction
//----------------------------------------------------------------------
// 柳青 规氢狼 馆措 规氢阑 备茄促.
// 澜 捞芭 static栏肺 官操电瘤 d甫 绝局电瘤 秦具绰单... - -
//----------------------------------------------------------------------
int
MCreature::GetCounterDirection( int d )
{
	// 馆措规氢
	d += 4;

	// 0~7鳖瘤父 登档废 茄促.
	while (d < 8)
	{
		d -= 8;
	}

	return d;
}

//----------------------------------------------------------------------
// Get Direction To Creature
//----------------------------------------------------------------------
// 泅犁 困摹(m_X,m_Y)俊辑 (sX,sY)甫 氢秦辑 官扼焊绰 规氢阑 搬沥茄促.
//----------------------------------------------------------------------
BYTE	
MCreature::GetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// 鞍篮 谅钎扼搁.. 泅犁 规氢栏肺 敌促.
	if (m_X==sX && m_Y==sY)
	{
		return m_CurrentDirection;
	}

	return MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 泅犁 Creature狼 困摹俊辑 direction规氢栏肺 捞悼茄 版快狼 困摹
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(BYTE direction, POINT &next)
{
	// 泅犁 困摹俊辑 促澜 规氢俊 措秦辑 格钎 汲沥
	next.x = m_X;
	next.y = m_Y;
	
	switch (direction)
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;			break;
		case DIRECTION_DOWN			:		next.y++;	break;
		case DIRECTION_UP			:		next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;			break;
	}
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 泅犁 Creature狼 困摹俊辑 m_CurrentDirection规氢栏肺 捞悼茄 版快狼 困摹
//----------------------------------------------------------------------
/*
void
MCreature::GetNextPosition(POINT &next)
{
	// 泅犁 困摹俊辑 促澜 规氢俊 措秦辑 格钎 汲沥
	next.x = m_X;
	next.y = m_Y;
	
	switch (m_CurrentDirection)
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;			break;
		case DIRECTION_DOWN			:		next.y++;	break;
		case DIRECTION_UP			:		next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;			break;
	}
}
*/

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 泅犁 Creature狼 困摹俊辑 m_CurrentDirection规氢栏肺 捞悼茄 版快狼 困摹
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(TYPE_SECTORPOSITION &sX, TYPE_SECTORPOSITION &sY)
{
	// 泅犁 困摹俊辑 促澜 规氢俊 措秦辑 格钎 汲沥
	sX = m_X;
	sY = m_Y;
	
	switch (m_Direction)
	{
		case DIRECTION_LEFTDOWN		: sX--;	sY++;	break;
		case DIRECTION_RIGHTUP		: sX++;	sY--;	break;
		case DIRECTION_LEFTUP		: sX--;	sY--;	break;
		case DIRECTION_RIGHTDOWN	: sX++;	sY++;	break;
		case DIRECTION_LEFT			: sX--;			break;
		case DIRECTION_DOWN			:		sY++;	break;
		case DIRECTION_UP			:		sY--;	break;
		case DIRECTION_RIGHT		: sX++;			break;
	}
}

//----------------------------------------------------------------------
// Move
//----------------------------------------------------------------------
//
// 泅犁狼 规氢(m_CurrentDirection)栏肺 茄 Frame捞悼茄促.
//
// 茄 Sector甫 捞悼窍绰霸 酒聪绊 Sector客 Sector荤捞甫 捞悼窍绰 吝埃狼
// 茄 Frame阑 捞悼窍绰 巴捞促. 
//
// Sector --> Sector俊辑 割 Frame阑 捞悼且鳖??啊 巩力牢单...
//    = cX,cY俊 茄锅俊 捞悼窍绰 pixel窜困甫 措涝窍绊
//    = sX,sY俊 茄锅俊 捞悼窍绰 pixel窜困甫 涝仿窍搁 等促.
//
// Move()俊辑绰
//    cX,cY啊 0捞搁 促矫 cX,cY, sX,sY 蔼阑 积己窍绊
//    0捞 酒聪搁 sX
//----------------------------------------------------------------------
void	
MCreature::ActionMove()
{
	if( GetCreatureType() == 672 )
		return;

	if( HasEffectStatus( EFFECTSTATUS_TRYING ) )
	{
		MZone::EFFECT_MAP::const_iterator itr = g_pZone->GetGroundEffects();
		bool bFind = false;

		for(int i = 0; i < g_pZone->GetGroundEffectCount(); itr++,i++ )
		{
			const MEffect* pEffect = itr->second;

			if( g_pPlayer->GetX() == pEffect->GetX() &&
				g_pPlayer->GetY() == pEffect->GetY() &&
				pEffect->GetFrameID() == EFFECTSPRITETYPE_REGEN_TOWER_GROUND )
			{
				bFind = true;
				break;
			}
		}
		if( bFind == false )
			RemoveEffectStatus( EFFECTSTATUS_TRYING );
	}

	// 格利 Sector俊 档崔茄 版快	
	if (m_MoveCount==0)//m_sX==0 && m_sY==0)// && m_ActionCount==0)
	{
		// 泅犁 规氢栏肺 茄 Sector甫 柳青窍绊
		// cX,cY, sX,sY甫 促矫 瘤沥秦拎具 茄促.

		//--------------------------------------------
		// 捞悼 啊瓷茄瘤 check
		//--------------------------------------------
		TYPE_SECTORPOSITION	x, y;

		//--------------------------------------------
		// fast move牢 版快绰 nextX, nextY俊 谅钎啊 乐促.
		//--------------------------------------------
		if (m_bFastMove)
		{
			x = m_NextX;
			y = m_NextY;

			m_NextX = SECTORPOSITION_NULL;					

			//m_CurrentDirection = 
			m_DirectionMove = m_Direction = GetDirectionToPosition( x, y );
		}
		else
		{
			// 泅犁 规氢栏肺 框流看阑 锭狼 谅钎甫 搬沥茄促.
			x = m_X; 
			y = m_Y;
			GetPositionToDirection( x, y, m_DirectionMove );
		}

		//--------------------------------------------
		// 框流老 荐 乐阑 版快
		//--------------------------------------------
		//
		// 框流老 荐 绝歹扼档 框流咯具 茄促.
		// Server俊辑 朝酒柯 沥焊捞扁 锭巩俊..
		// 
		//--------------------------------------------
		// [ TEST CODE ]
		/*
		#ifdef	OUTPUT_DEBUG			
			if (!m_pZone->CanMove(m_MoveType, x,y))
			{
				DEBUG_ADD_FORMAT("[Can't Move] But Go! [ID=%d] (%d,%d) --> (%d,%d)",
										m_ID,
										m_X, m_Y,
										x, y);
				
			}
		#endif
		*/

		//#ifdef	OUTPUT_DEBUG
			TYPE_SECTORPOSITION ox = m_X;
			TYPE_SECTORPOSITION oy = m_Y;
		//#endif
		
		{
			// zone狼 sector狼 沥焊甫 官层霖促.
			// m_X客 m_Y档 汲沥秦霖促.
			if (MovePosition( x, y ))
			{
				#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("[Move] [ID=%d] (%d,%d) --> (%d,%d)",
									m_ID,
									ox, oy,
									m_X, m_Y);					

					if (m_DirectionMove >= 8)
					{
						DEBUG_ADD_FORMAT("[Error] Direction exceed MAX : dir=%d", (int)m_DirectionMove);
						return;
					}
				#endif
				//--------------------------------------------
				// 5 Frame捞 乐促绊 且 版快
				//--------------------------------------------
				//
				// [1] UP,DOWN,LEFT,RIGHT老 锭,
				//
				//     X函拳 : 0 16 32 48 64   (+-16)
				//     Y函拳 : 0  8 16 24 32   (+-8)
				// 
				//
				// [2] 措阿急(LEFTUP,LEFTDOWN,RIGHTUP,RIGHTDOWN)栏肺 框流老锭,
				//
				//     X函拳 : 0  8 16 24 32   (+-8)
				//     Y函拳 : 0  4  8 12 16   (+-4)
				//
				//--------------------------------------------
				// sX,sY : 框流咯具且 傈眉 pixel(茄 TILE)
				// cX,cY : 捞悼窍绰 窜困 pixel
				//--------------------------------------------

				//--------------------------------------------
				// Fast Move 牢 版快
				//--------------------------------------------
				if (m_bFastMove)
				{
					// 流急 芭府肺 框流老锭狼 pixel芭府
					// ex) 哭率 2 --> 1 : ( 2 - 1 ) * TILE_X
					//     困率 2 --> 1 : ( 2 - 1 ) * TILE_Y
					m_sX = (ox - m_X) * TILE_X;
					m_sY = (oy - m_Y) * TILE_Y;
					//------------------------------------------------
					// 角力 茄 鸥老 捞悼 加档狼 馆狼 加档俊 格利瘤鳖瘤 捞悼茄促.
					//------------------------------------------------
					// 茄 鸥老 捞悼且锭狼 Frame 荐
					int moveTimes_div_2 = (*g_pCreatureTable)[m_CreatureType].MoveTimes >> 1;

					// 2005, 1, 5, sobeit add start
					if(HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
					{
//						moveTimes_div_2 = moveTimes_div_2/2;
						m_MoveCountMax = m_MoveCountMax*2;
					}
					// 2005, 1, 5, sobeit add end

					m_cX = -m_sX / moveTimes_div_2;
					m_cY = -m_sY / moveTimes_div_2;

					m_MoveCount += 2;

					if (m_MoveCount > m_MoveCountMax)
					{
						m_MoveCount = m_MoveCountMax;
					}
				}
				//--------------------------------------------	
				// 沥惑 捞悼
				//--------------------------------------------
				else
				{				
					m_DirectionMoved = m_DirectionMove;
					m_sX = m_sXTable[m_DirectionMoved];
					m_sY = m_sYTable[m_DirectionMoved];
					
					//????????????????????????????????????????????????????????
					//???                                                  ???
					//???   茄 Frame阑 捞悼...秦具窍绰啊?? 富酒具 窍绰啊   ???
					//???                                                  ???
					//????????????????????????????????????????????????????????
					int moveTimes_1;

					#ifdef OUTPUT_DEBUG
						if (m_CreatureType >= g_pCreatureTable->GetSize())
						{
							DEBUG_ADD_FORMAT("[Error] CreatureType is exceed MAX. id=%d, type=%d", m_ID, m_CreatureType);

							SetStop();

							return;
						}
					#endif
				
					if (m_MoveDevice==MOVE_DEVICE_WALK)
					{
						moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
					}
					else if (m_MoveDevice==MOVE_DEVICE_RIDE)
					{
						moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
					}
					else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( this ))
					{
						moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
					}
					else
					{
						DEBUG_ADD_FORMAT("[Error] What is MoveType of this Creature??? id=%d", m_ID);
						
						SetStop();

						return;
					}

					m_MoveTableCount = 0;
					m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
					m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];

					//m_ActionCount++;
					m_MoveCount++;
				}
				
				m_sX += m_cX;
				m_sY += m_cY;	
				
				// 促澜俊 捞悼且 count甫 瘤沥茄促.
				// frame父 官差绰 版快客
				// frame捞 官差绊 捞悼窍绰 版快啊 乐促.
				m_NextMoveCount = (*g_pCreatureTable)[m_CreatureType].MoveRatio;				

				//------------------------------------------------
				// 某腐磐狼 MoveAction俊 嘎绰 Sound甫 免仿秦霖促.
				//------------------------------------------------
				TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( m_MoveAction );

				if (soundID!=SOUNDID_NULL  )
				{
					PlaySound( soundID,
								false,
								m_X,
								m_Y);
				}

				DEBUG_ADD("[Move] OK");				
			}
			// 给 框流牢 版快
			else
			{
				// 肛勉
				SetAction( ACTION_STAND );	
			}			
		}
		//--------------------------------------------
		// 框流老 荐 绝阑 版快
		//--------------------------------------------
	}
	// 酒流 促澜 Sector鳖瘤 代 档崔茄 版快
	else
	{
		// 茄 Frame阑 捞悼茄促.
		/*
		switch (m_MoveDevice)
		{
			case MOVE_DEVICE_NULL :			
				m_sX += m_cXTableDeviceNull[m_CurrentDirection][m_ActionCount];
				m_sY += m_cYTableDeviceNull[m_CurrentDirection][m_ActionCount];
			break;

			case MOVE_DEVICE_MOTOR1 :						
				m_sX += m_cXTableDeviceMotor1[m_CurrentDirection][m_ActionCount];
				m_sY += m_cYTableDeviceMotor1[m_CurrentDirection][m_ActionCount];
			break;
		}
		*/
		//m_sX += m_cX;
		//m_sY += m_cY;

		// 捞悼且 count啊 灯阑锭父 捞悼茄促.
		if (m_MoveCount>=m_NextMoveCount)
		{			
			//--------------------------------------------
			// Fast Move 牢 版快
			//--------------------------------------------
			if (m_bFastMove)
			{
				m_sX += m_cX;
				m_sY += m_cY;

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}

				// 促澜俊 捞悼且 count甫 瘤沥茄促.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				//----------------------------------------------------------
				// fast move 饶俊 茄锅 锭府扁.. - -
				// Flash Sliding
				//----------------------------------------------------------
				if (m_MoveCount+1 >= m_MoveCountMax
					&& m_nNextUsedActionInfo!=m_nBasicActionInfo)
				{
					MCreature* pCreature = g_pZone->GetCreature( m_TraceID );

					if (pCreature!=NULL 
						&& m_nSpecialActionInfo != MAGIC_RAPID_GLIDING 
						&& m_nSpecialActionInfo != SKILL_TELEPORT 
						&& m_nSpecialActionInfo != SKILL_CHARGING_ATTACK 
						&& !HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
					{
						//---------------------------------------------------
						// 惑措啊 嘎绰 葛嚼
						//---------------------------------------------------
						MActionResult* pResult = new MActionResult;
						
						int ActionInfo = GetBasicActionInfo();//m_nBasicActionInfo;

						if(m_nSpecialActionInfo == SKILL_BLITZ_SLIDING || m_nSpecialActionInfo == SKILL_BLAZE_WALK )
						{
							ActionInfo = SKILL_BLITZ_SLIDING_ATTACK;

							if( m_nSpecialActionInfo == SKILL_BLAZE_WALK )
								ActionInfo = SKILL_BLAZE_WALK_ATTACK;
							
							SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );
						}
						else if(m_nSpecialActionInfo == SKILL_SET_AFIRE)
						{
							ActionInfo = RESULT_SKILL_SET_AFIRE;
						}
//						if(m_nSpecialActionInfo == MAGIC_BLOODY_ZENITH)
//						{
//							SetBloodyZenith(16);
//						}						

						// 搬苞 钎泅
						SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );
						pResult->Add( new MActionResultNodeActionInfo( 
														ActionInfo,
														m_ID,
														pCreature->GetID(), 
														pCreature->GetX(),
														pCreature->GetY(),
														0 ) 
										);	
					
						PacketSpecialActionToOther( ActionInfo, 
													m_TraceID, 
													pResult );						
					}
				}
			}
			//--------------------------------------------
			// 老馆利牢 捞悼狼 版快
			//--------------------------------------------
			else
			{
				int moveTimes_1;
				
				if (m_MoveDevice==MOVE_DEVICE_WALK)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_RIDE)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( this ))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				} 
				else
				{
					DEBUG_ADD_FORMAT("[Error] MoveTimes Error. MCreature.cpp Lines:%d",__LINE__);
					return;
				}

				m_MoveTableCount++;
				if(m_MoveTableCount > moveTimes_1)
					m_MoveTableCount = moveTimes_1;
				
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];

				m_sX += m_cX;
				m_sY += m_cY;

				// 促澜俊 捞悼且 count甫 瘤沥茄促.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				m_MoveCount++;
			}
		
		}
		else
		{
			//m_ActionCount++;
			m_MoveCount++;
		}	


		// frame阑 官层霖促. 付瘤阜 Frame鳖瘤 艾栏搁 0锅掳 Frame栏肺 官槽促.
		//if (++m_ActionCount == (*m_pFrames)[m_Action][m_CurrentDirection].GetCount())
		//	m_ActionCount = 0;
	}
}

//----------------------------------------------------------------------
// Attach CastingEffect
//----------------------------------------------------------------------
void	
MCreature::AttachCastingEffect(TYPE_ACTIONINFO nUsedActionInfo, BOOL bForceAttach)
{
	DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect(%d, %d)", nUsedActionInfo, bForceAttach);	
	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect(%d, %d)", nUsedActionInfo, bForceAttach);
		}
	#endif

	if (nUsedActionInfo!=ACTIONINFO_NULL)
	{
		int		castingStartFrame = GetActionInfoCastingStartFrame(nUsedActionInfo);
		BOOL	bCastingEffect	= bForceAttach || (castingStartFrame==m_ActionCount);

		//-------------------------------------------------------------
		// Casting Effect
		//-------------------------------------------------------------
		if (bCastingEffect)
		{
			if (GetActionInfoActionEffectSpriteType(nUsedActionInfo) != EFFECTSPRITETYPE_NULL)
			{
				int castingFrames = GetActionInfoCastingFrames(nUsedActionInfo);
			
				// 汲沥捞 救 蹬 乐栏搁.. action 悼累俊 嘎冕促.
				if (castingFrames==0)
				{
					int actionInfoAction = GetActionInfoAction(nUsedActionInfo);
						
					//-------------------------------------------------
					// 坷配官捞甫 鸥绊 乐绰 版快
					//-------------------------------------------------				
					if (m_MoveDevice==MOVE_DEVICE_RIDE)
					{
						//castingFrames = (*g_pCreatureTable)[m_CreatureType].GetActionCount( actionInfoAction );					
						m_ActionCountMax = GetCreatureActionCountMax( this, actionInfoAction );
					}					
					else
					{	
						//-------------------------------------------------
						// 软趋 悼累吝俊 傍拜罐篮 版快
						//-------------------------------------------------
						if (actionInfoAction==ACTION_DAMAGED
							//&& (// 软趋 寸窍绰 悼累 吝捞芭唱 
								//m_Action==ACTION_DRAINED
								// 轨颇捞绢老锭, 软趋窍绰 悼累 吝俊绰 damaged甫 救 焊咯霖促.
								//|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							//	)
							)
						{
							//castingFrames = (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_DAMAGED );
							castingFrames = GetCreatureActionCountMax( this, ACTION_DAMAGED );
						}
						else
						{
							castingFrames = (int)GetActionCountMax() - (int)castingStartFrame;

							// 烙矫肺..
							// 咯扁辑 澜荐啊 唱坷绰 版快.. 绢纳 窍瘤.. - -;
							if (castingFrames < 0)
							{
								castingFrames = 16;
							}
						}
					}					
				}

				TYPE_EFFECTSPRITETYPE effectSpriteType = GetActionInfoActionEffectSpriteType(nUsedActionInfo);				

				MCreature* targetCreature = NULL;

				//-------------------------------------------------
				// casting捞 穿备茄抛 嘿绰啊?
				//-------------------------------------------------
				if(nUsedActionInfo != ACTIONINFO_NULL)
				{
					if ((*g_pActionInfoTable)[nUsedActionInfo].IsCastingEffectToSelf())
					{
						targetCreature = this;
					}
					else
					{
						targetCreature = g_pZone->GetCreature( m_TraceID );					
					}
				}


				if (targetCreature!=NULL)
				{
					// [ TEST CODE ]
					// Creature俊霸 嘿捞绰 Effect甫 积己秦辑 pointer甫 逞败罐绰促.
					MAttachEffect* pEffect = targetCreature->CreateAttachEffect( 
																effectSpriteType, 
																//(*g_pActionInfoTable)[nUsedActionInfo].GetDelay() 
																castingFrames,
																castingFrames
															);
		
					if (pEffect != NULL)
					{
						pEffect->SetDirection( targetCreature->GetDirection() );

						pEffect->SetLink( nUsedActionInfo, NULL );

						// 嘿绢具 窍绰 某腐磐
						pEffect->SetAttachCreatureID( targetCreature->GetID() );
						
						#ifdef OUTPUT_DEBUG
						
							if (m_MoveDevice==MOVE_DEVICE_RIDE)
							{
								
							}
							else
							{	
								//-------------------------------------------------
								// 软趋 悼累吝俊 傍拜罐篮 版快
								//-------------------------------------------------
								if (GetActionInfoAction(nUsedActionInfo)==ACTION_DAMAGED)
								{
									DEBUG_ADD_FORMAT("[ATTACH] damaged. id=%d, ai=%d, est=%d, frames=%d", m_ID, nUsedActionInfo, (int)effectSpriteType, (int)castingFrames);
									DEBUG_ADD_FORMAT("Action[%d]=%d/%d, Move=%d/%d, Frame=%d", m_Action, m_ActionCount, GetActionCountMax(), m_MoveCount, m_MoveCountMax, GetFrame());

									g_bCheckError = true;
								}
							}
						#endif
					}
				}
			}

			//-------------------------------------------------------------
			// Casting ActionInfo 钎泅
			//-------------------------------------------------------------
			TYPE_ACTIONINFO castingActionInfo = (*g_pActionInfoTable)[nUsedActionInfo].GetCastingActionInfo();
			if (castingActionInfo!=ACTIONINFO_NULL)
			{
				#ifdef OUTPUT_DEBUG
					
					DEBUG_ADD_FORMAT("AffectUsedActionInfo(%d)", (int)castingActionInfo);
					
					AffectUsedActionInfo(castingActionInfo);					

					DEBUG_ADD("castingActionInfo OK");

				#else

					AffectUsedActionInfo(castingActionInfo);

				#endif
			}

			//-------------------------------------------------------------
			// casting 悼累俊 嘎绰 sound
			//-------------------------------------------------------------
			PlaySound( GetCastingSoundID(nUsedActionInfo) ,
						false,
						m_X, m_Y);
		}
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect OK~~");
		}
	#endif
}

//----------------------------------------------------------------------
// Set Underground Creature
//----------------------------------------------------------------------
void	
MCreature::SetUndergroundCreature()		
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_UNDERGROUND; 
	m_Z = 0;

	CheckRegen();
}

//----------------------------------------------------------------------
// Set Ground Creature
//----------------------------------------------------------------------
void	
MCreature::SetGroundCreature()				
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_GROUND; 
	m_Z=0; 

	CheckRegen();
}

//----------------------------------------------------------------------
// Set Flying Creature
//----------------------------------------------------------------------
void	
MCreature::SetFlyingCreature()				
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_FLYING; 
	m_Z = (*g_pCreatureTable)[m_CreatureType].FlyingHeight; 
	
	CheckRegen();
}

//----------------------------------------------------------------------
// Action Effect
//----------------------------------------------------------------------
void	
MCreature::ActionEffect()
{	
	//-------------------------------------------------------------
	// 荤侩 扁贱捞 沥秦廉 乐绰 版快..
	//-------------------------------------------------------------
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND || IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND);
		int		actionCountMax_1 = GetActionCountMax()-1;
		BOOL	bStartAction	= (m_ActionCount==0);
		BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
		int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

		// Effect啊 矫累登绰 版快绰..
		// (1) StartFrame牢 版快
		// (2) 付瘤阜 ActionFrame牢 版快
		BOOL bStartEffect = m_ActionCount==StartFrame || 
							StartFrame >= GetActionCountMax() && bEndAction;

		bStartAction |= (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
					&& m_RepeatCount!=0
					&& m_ActionCount == GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

			
		//BOOL	bCastingEffect	= GetActionInfoCastingStartFrame(m_nUsedActionInfo)==m_ActionCount;

		//-------------------------------------------------------------
		// 矫累 悼累
		//-------------------------------------------------------------
		if (bStartAction)
		{
			//------------------------------------------------
			// 扁贱狼 悼累俊 嘎绰 sound甫 免仿秦霖促.
			//------------------------------------------------
			//g_Sound.Play( g_SoundTable[(*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID()].pDSBuffer );
			//PlaySound( (*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID() ,
			//			false,
			//			m_X, m_Y);

			//------------------------------------------------
			// 某腐磐狼 Action俊 嘎绰 Sound甫 免仿秦霖促.
			//------------------------------------------------
			PlaySound( 
				(*g_pCreatureTable)[m_CreatureType].GetActionSound( 
					GetActionInfoAction(m_nUsedActionInfo)
				),
				false,
				m_X, m_Y
			);
		}

		//-------------------------------------------------------------
		// Casting Effect
		//-------------------------------------------------------------
		AttachCastingEffect( m_nUsedActionInfo );	

		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] ActionEffect");
			}
		#endif


		//-------------------------------------------------------------
		// 沥瘤 悼累捞搁..
		// 官肺 effect甫 焊咯霖促.
		//-------------------------------------------------------------
		if (bActionStand)
		{
			m_ActionCount = GetActionCountMax()-1;
		}

		//-------------------------------------------------------------
		// 扁贱阑 焊咯拎具 窍绰 版快
		//-------------------------------------------------------------
		if (bStartEffect)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}
	}
	else
	{
		//if (m_ActionCount==m_ActionCountMax-1)		
		{
			//--------------------------------------------------------
			// Action场
			//--------------------------------------------------------	
			//m_Action			= ACTION_STAND;
		}
	}

	m_ActionCount++;

	//--------------------------------------------------------	
	// 烙矫肺 软趋悼累 吝瘤窍绰芭..
	//--------------------------------------------------------	
	if (m_bStopBloodDrain)
	{
		StopBloodDrain();
	}
	if (m_bStopAbsorbSoul)
	{
		StopAbsorbSoul();
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD("[CheckError] ActionEffect OK");
		}
	#endif
}

//----------------------------------------------------------------------
// Affect Used ActionInfo
//----------------------------------------------------------------------
// 扁贱 荤侩茄吧 钎泅茄促.
//----------------------------------------------------------------------
void
MCreature::AffectUsedActionInfo(TYPE_ACTIONINFO nUsedActionInfo)
{
	bool	bMonster = !IsPlayerOnly();
	bool	SpecialID = false;
	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo(%d)", nUsedActionInfo);
		}
	#endif

	if (nUsedActionInfo==ACTIONINFO_NULL)
	{
		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] AffectUsedActionInfo NULL -> return");
			}
		#endif

		return;
	}

	DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,nUsedActionInfo);

	if ((*g_pActionInfoTable)[nUsedActionInfo].GetSize()!=0)
	{
		POINT point;	

		//--------------------------------------------------------
		// Casting ActionInfo牢啊?
		//--------------------------------------------------------
		BOOL bCastingAction = (*g_pActionInfoTable)[nUsedActionInfo].IsCastingAction();
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);


		//--------------------------------------------------------
		// 格钎 困摹 Pixel谅钎
		//--------------------------------------------------------
		point = MTopView::MapToPixel(m_TraceX, m_TraceY);
		//point.x += m_sX;
		//point.y += m_sY;
		//point.x += TILE_X_HALF;

		//--------------------------------------------------------
		//
		//					Effect 格钎 汲沥
		//
		//--------------------------------------------------------
		MEffectTarget* pEffectTarget = NULL;

		//--------------------------------------------------------
		// casting action牢 版快
		//--------------------------------------------------------
		if (bCastingAction)
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			pEffectTarget->NewEffectID();		
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			if( nUsedActionInfo == CASTING_FIRE || nUsedActionInfo == CASTING_WATER ||
				nUsedActionInfo == CASTING_GROUND || nUsedActionInfo == CASTING_FIRE2 ||
				nUsedActionInfo == CASTING_GROUND2 || nUsedActionInfo == CASTING_WATER2 )
			{
				pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );
			} else
			{
				
				pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );				
			}
			pEffectTarget->SetDelayFrame( m_EffectDelayFrame );

			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			// castingAction俊辑绰 搬苞贸府啊 鞘夸绝促.
		}
		//--------------------------------------------------------
		// casting action捞 酒囱.. 角力 扁贱牢 版快..
		//--------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,(*g_pActionInfoTable)[nUsedActionInfo].GetSize());

			pEffectTarget->NewEffectID();
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,m_nUsedActionInfo);

			//pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
			
//			if(m_nUsedActionInfo == MAGIC_ACID_TOUCH || m_nUsedActionInfo == MAGIC_BLOODY_NAIL 
//				|| m_nUsedActionInfo == SKILL_FLOURISH || m_nUsedActionInfo == SKILL_ABSORB_SOUL)
			if(m_nUsedActionInfo != ACTIONINFO_NULL && (*g_pActionInfoTable)[m_nUsedActionInfo].IsAttachSelf() )
			{
				SpecialID = true;
//				if(!bMonster)
				{
					DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
					pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );					
				}
//				else
//					pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );

				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			}
			else
			{
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
				pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
			}
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			//--------------------------------------------------------
			// 瘤加 矫埃 汲沥
			//--------------------------------------------------------

			if (m_DelayActionInfo==nUsedActionInfo)
			{
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
				pEffectTarget->SetDelayFrame( m_EffectDelayFrame );
				m_DelayActionInfo	= ACTIONINFO_NULL;
				m_EffectDelayFrame	= 0;
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			}
			
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			// 搬苞 汲沥 : 搬苞 贸府绰 EffectGenerator俊 该变促.
			pEffectTarget->SetResult( m_pActionResult );
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			
			// 汲沥等 搬苞甫 绝霸 茄促.
			m_pActionResult = NULL;
		}

		//--------------------------------------------------------
		//
		//					矫累 困摹甫 搬沥茄促.
		//
		//--------------------------------------------------------
		int x,y,z, direction;

		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		//--------------------------------------------------------
		// User 困摹俊辑 矫累窍绰 版快
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartUser())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			point = MTopView::MapToPixel(m_X, m_Y);
			point.x += m_sX;
			point.y += m_sY;
			//point.x += TILE_X_HALF;

			x			= point.x;
			y			= point.y;
			z			= m_Z;//+60;			
		}
		//--------------------------------------------------------
		// Target 困摹俊辑 矫累窍绰 版快
		//--------------------------------------------------------
		else if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartTarget())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			x			= point.x;
			y			= point.y;
			z			= m_Z;//+60;			
		}

		//--------------------------------------------------------
		// 傍吝俊辑 矫累窍绰 版快
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartSky())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			z	= m_Z + (*g_pActionInfoTable)[nUsedActionInfo].GetValue();

			direction	= DIRECTION_DOWN;
		}
		//--------------------------------------------------------
		// 瘤惑俊辑 矫累窍绰 版快
		//--------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			direction	= m_Direction;
		}

		//--------------------------------------------------------
		//
		//                   Effect积己		
		//
		//--------------------------------------------------------
		
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		(*g_pEffectGeneratorTable).Generate(
				x,y,z,				// 矫累 困摹
				direction, 		// 规氢
		//		1,					// power
				SpecialID ? bMonster ? 2 : 1 : 1,
				nUsedActionInfo,		//	ActionInfoTable辆幅,
				pEffectTarget,		// 格钎 沥焊
				GetActionGrade()	
		);
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		ClearActionGrade();		

		DEBUG_ADD_FORMAT("[Creature::AffectUsedActionInfo] ai=%d, id=%d", nUsedActionInfo, m_ID);		
	}
	else
	{
		//------------------------------------------------------------
		// 搬苞甫 贸府秦具窍绰 矫痢牢啊? - 寸楷窍促绊 夯促 *_*;
		//------------------------------------------------------------				
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		if (m_pActionResult != NULL)
		{					
			// (!) m_pActionResult蔼捞 Execute俊辑 函且 荐 乐绢辑 
			//		历厘沁促啊 瘤况拎具 茄促.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			pOldResult->Execute();
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			delete pOldResult;	
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		}				
	}

	//--------------------------------------------------------
	// Action场
	//--------------------------------------------------------	
	//m_Action			= ACTION_STAND;	


	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo OKOK ~~");
		}
	#endif
}

//----------------------------------------------------------------------
// Affect Buffered ActionInfo
//----------------------------------------------------------------------
// NextUsedActionInfo啊 乐促搁
// UsedActionInfo --> NextUsedActionInfo甫 葛滴 利侩茄促.
//
// UsedActionInfo父 乐绰 版快绰 贸府 救茄促.
//----------------------------------------------------------------------
void
MCreature::AffectBufferedActionInfo()
{
	DEBUG_ADD_FORMAT("[AffectBufferedActionInfo] ai=%d, nai=%d, action=%d-%d/%d", m_nUsedActionInfo, m_nNextUsedActionInfo, (int)m_Action, (int)m_ActionCount, (int)GetActionCountMax());
	
	//--------------------------------------------------------
	// 促澜俊 荤侩且妨绰 扁贱捞 乐促搁... 官肺 利侩 矫挪促.
	//--------------------------------------------------------
	if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
	{	
		TYPE_ACTIONINFO nextActionInfo = m_nNextUsedActionInfo;
		DEBUG_ADD("[AffectBufferedActionInfo] Into Here");
		
		//--------------------------------------------------------
		// 泅犁 荤侩秦具且 扁贱捞 乐绰 版快 
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			DEBUG_ADD("[AffectBufferedActionInfo] AttachCastingEffect" );
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
		
			// m_nUsedActionInfo 利侩
			// --> 郴何俊辑 m_nNextUsedActionInfo啊 官拆 荐 乐促.

	// 捞 版快绰 公炼扒(!) CastingEffect甫 嘿咯具 茄促.
			AttachCastingEffect( m_nUsedActionInfo, TRUE );

			#ifdef OUTPUT_DEBUG					
				if (g_bCheckError)
				{
					DEBUG_ADD("[CheckError] AffectBufferedActionInfo");
				}
			#endif

			AffectUsedActionInfo( m_nUsedActionInfo );

			#ifdef OUTPUT_DEBUG					
				if (g_bCheckError)
				{
					DEBUG_ADD("[CheckError] AffectUsedActionInfo OK");
				}
			#endif

			m_nUsedActionInfo	= ACTIONINFO_NULL;			

			// affect窍绊 唱辑.. 荤侩秦具且 扁贱捞 积变促搁..
			if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
			{	
				#ifdef OUTPUT_DEBUG					
					if (g_bCheckError)
					{
						DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect( %d )", m_nNextUsedActionInfo);

						AttachCastingEffect( m_nNextUsedActionInfo, TRUE );

						DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo( %d )", m_nNextUsedActionInfo);

						AffectUsedActionInfo( m_nNextUsedActionInfo );

						DEBUG_ADD_FORMAT("[CheckError] m_nNextUsedActionInfo!=ACTIONINFO_NULL OK");
					}
					else
					{
						AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
						AffectUsedActionInfo( m_nNextUsedActionInfo );
					}
				#else
					AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
					AffectUsedActionInfo( m_nNextUsedActionInfo );
				#endif
			}
		}
		
		//--------------------------------------------------------
		// 促澜俊 荤侩秦具且 扁贱 利侩
		//--------------------------------------------------------
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		// --> 郴何俊辑 m_nNextUsedActionInfo啊 官拆 荐 乐促.
		DEBUG_ADD("~~~~~zzzz");
		AttachCastingEffect( nextActionInfo, TRUE );
		DEBUG_ADD("~~~~~zzzz2");
		AffectUsedActionInfo( nextActionInfo );
			
		// affect窍绊 唱辑.. 荤侩秦具且 扁贱捞 积变促搁..
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{	
			DEBUG_ADD("~~~~~zzzz3");
			AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
			AffectUsedActionInfo( m_nNextUsedActionInfo );
		}

		m_nUsedActionInfo = ACTIONINFO_NULL;
		
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] AffectBufferedActionInfo OKOK -_-;");
			}
		#endif
	}
	//--------------------------------------------------------
	// 泅犁 荤侩秦具且 扁贱父 乐绰 版快牢啊?
	//--------------------------------------------------------
	else if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		DEBUG_ADD("[AFFE] USEDACTIONINFO != NULL ");
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
	
		// m_nUsedActionInfo 利侩
		// --> 郴何俊辑 m_nNextUsedActionInfo啊 官拆 荐 乐促.

		// 捞 版快绰 公炼扒(!) CastingEffect甫 嘿咯具 茄促.
		DEBUG_ADD("[AFFE] CASTING EFF");
		AttachCastingEffect( m_nUsedActionInfo, TRUE );
		DEBUG_ADD("[AFFE] AFFECT USED");
		AffectUsedActionInfo( m_nUsedActionInfo );

		m_nUsedActionInfo	= ACTIONINFO_NULL;			

		// affect窍绊 唱辑.. 荤侩秦具且 扁贱捞 积变促搁..
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{			
			DEBUG_ADD("[AFFE] ZZZZZZ ");
			AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
			AffectUsedActionInfo( m_nNextUsedActionInfo );
		}	

		//--------------------------------------------------------
		// 促澜俊 荤侩秦具且 扁贱 利侩
		//--------------------------------------------------------
		m_nUsedActionInfo = ACTIONINFO_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
	}
}

//----------------------------------------------------------------------
// Is Recovery HP
//----------------------------------------------------------------------
inline BOOL				
MCreature::IsRecoveryHP()	
{ 
	return m_RecoveryHPTimes > 0;
	//return m_RecoveryPart==MODIFY_CURRENT_HP; 
}

//----------------------------------------------------------------------
// Is Recovery MP
//----------------------------------------------------------------------
inline BOOL				
MCreature::IsRecoveryMP()	
{ 
	return m_RecoveryMPTimes > 0;
	//return m_RecoveryPart==MODIFY_CURRENT_MP;
}		

//----------------------------------------------------------------------
// Set Recovery
//----------------------------------------------------------------------
// amount父怒 times锅究 delay付促 雀汗茄促.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryHP(int amount, int times, DWORD delay)
{ 
	if (times > 0)
	{
		// 官肺 茄锅 雀汗

		if(! IsRecoveryHP() )					// 泅犁 HP 啊 瞒绊 乐瘤 臼栏搁
		{
			SetStatus( MODIFY_CURRENT_HP, GetHP()+amount );
			m_RecoveryHPTimes = times - 1; 
		} else
			m_RecoveryHPTimes = times;

		m_RecoveryHPAmount = amount; 
		m_RecoveryHPDelayTime = delay;
		//m_RecoveryHPPart = MODIFY_CURRENT_HP;

		// 促澜 雀汗且 矫埃 汲沥
		m_RecoveryHPNextTime = g_CurrentTime + m_RecoveryHPDelayTime;
	}
}

//----------------------------------------------------------------------
// Set Recovery
//----------------------------------------------------------------------
// amount父怒 times锅究 delay付促 雀汗茄促.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryMP(int amount, int times, DWORD delay)
{ 
	// 2004, 9, 3, sobeit add start - 敲饭廉 胶抛捞记俊 嘎栏搁 MP啊 救蛮促...-_-
	if(HasEffectStatus(EFFECTSTATUS_PLEASURE_EXPLOSION))
		return;
	// 2004, 9, 3, sobeit add end
	if (times > 0)
	{
		// 官肺 茄锅 雀汗
		if(! IsRecoveryMP() )
		{
			SetStatus( MODIFY_CURRENT_MP, GetMP()+amount );
			m_RecoveryMPTimes = times-1; 
		} else
			m_RecoveryMPTimes = times; 

		m_RecoveryMPAmount = amount; 		
		m_RecoveryMPDelayTime = delay;
		//m_RecoveryMPPart = MODIFY_CURRENT_MP;

		// 促澜 雀汗且 矫埃 汲沥
		m_RecoveryMPNextTime = g_CurrentTime + m_RecoveryMPDelayTime;
	}
}

//----------------------------------------------------------------------
// Check Drop Blood
//----------------------------------------------------------------------
// HP 撤阑 锭.. 乔 汝府扁
//----------------------------------------------------------------------
void				
MCreature::CheckDropBlood()
{
	if (!HasEffectStatus(EFFECTSTATUS_COMA))
	{
		//----------------------------------------------------------
		// 乔 汝副 矫埃捞 登菌绰瘤(-_-;) 眉农..
		//----------------------------------------------------------
		if (g_pUserOption->BloodDrop 
			&& g_CurrentTime > m_NextBloodingTime)
		{
			int currentHP = m_Status[MODIFY_CURRENT_HP];
			int maxHP = m_Status[MODIFY_MAX_HP];

			int percentHP = ((maxHP==0)? 0 : currentHP*100 / maxHP);

			//----------------------------------------------------------
			// HP 30% 捞窍老锭 乔 汝赴促.
			//----------------------------------------------------------
			if (percentHP <= g_pClientConfig->BLOOD_DROP_HP_PERCENT)
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_DROP_GROUND,										// 荤侩 扁贱 锅龋
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 荤侩 规氢
							
							m_ID,												// 格钎俊 措茄 沥焊
							m_X, m_Y, 0,
							
							80, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}

			//----------------------------------------------------------
			// 促澜 乔 汝府扁 啊瓷茄 矫埃 汲沥. 
			// 粱 词霸 眉农秦档 包拌绝栏骨肺 乔 汝府芭唱 富芭唱..矫埃 犬厘
			//----------------------------------------------------------
			DWORD timeGap = 300 //+ 1000	//g_pClientConfig->BLOOD_DROP_GAP_TIME 
							+ (percentHP<<2) * rand()%20;//g_pClientConfig->BLOOD_DROP_RANDOM_TIME;

			//----------------------------------------------------------
			// 软趋 寸茄 惑怕捞搁 乔甫 粱 代 汝赴促.
			//----------------------------------------------------------
			if (HasEffectStatus(EFFECTSTATUS_BLOOD_DRAIN))
			{
				// 5硅 沥档 代 汝赴促.
				m_NextBloodingTime = g_CurrentTime + timeGap*5;
			}
			else
			{			
				m_NextBloodingTime = g_CurrentTime + timeGap;
			}
		}
	}
}

//----------------------------------------------------------------------
// Add Casket Soon
//----------------------------------------------------------------------
void				
MCreature::AddCasketSoon(int type)
{
	m_bInCasket = true;
	m_CasketCount = 0;
	m_CasketCountInc = 0;
	m_CasketType = type;

	//SetInvisibleSoon();

	// [货扁贱3] -_-; virtual肺 窍绰霸 唱阑刨单.. 
	if (GetClassType()==CLASS_PLAYER)
	{		
		g_pSkillAvailable->SetAvailableSkills();
	}

	// [货扁贱7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
	
	CheckRegen();

	if(g_pPlayer == this)	// Player牢 版快 包家券 吝俊绰 渴 给哎酒 涝绰促.
	{
		UI_LockGear();
	}
}

//----------------------------------------------------------------------
// Remove Casket Soon
//----------------------------------------------------------------------
void				
MCreature::RemoveCasketSoon()
{
	m_bInCasket = false;
	m_CasketCount = 64;
	m_CasketCountInc = 0;

	//SetVisibleSoon();

	// [货扁贱7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = false;
	
	CheckRegen();

	if(g_pPlayer == this)	// Player牢 版快 包家券 吝俊绰 渴 给哎酒 涝绰吧 乾促.
	{
		UI_UnlockGear();;
	}
}

//----------------------------------------------------------------------
// Add Casket
//----------------------------------------------------------------------
// 包捞 辑辑洒 积变促.
//----------------------------------------------------------------------
void				
MCreature::AddCasket(int type)
{
	AddCasketSoon(type);
//	m_bInCasket = true;
	m_CasketCount = 64;
	m_CasketCountInc = -2;
//	m_CasketType = type;
//
//	//SetInvisible();
//
//	// [货扁贱3] -_-; virtual肺 窍绰霸 唱阑刨单.. 
//	if (GetClassType()==CLASS_PLAYER)
//	{		
//		g_pSkillAvailable->SetAvailableSkills();
//	}
//
//	// [货扁贱7]
//	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
//	
//	CheckRegen();
}

//----------------------------------------------------------------------
// Remove Casket
//----------------------------------------------------------------------
// 包捞 辑辑洒 荤扼柳促.
//----------------------------------------------------------------------
void				
MCreature::RemoveCasket()
{
	RemoveCasketSoon();
	m_bInCasket = true;		// 64登搁 false肺 官层具 茄促.
	m_CasketCount = 0;
	m_CasketCountInc = 2;

	//SetVisible();

//	// [货扁贱7]
//	m_bEffectStatus[EFFECTSTATUS_CASKET] = false;
//
//	CheckRegen();
}

//----------------------------------------------------------------------
// Update Casket
//----------------------------------------------------------------------
void				
MCreature::UpdateCasket()
{
	if (m_bInCasket)
	{		
		int incValue = m_CasketCountInc;

		if (m_CasketCount < 48)
		{
			if (m_CasketCount < 32)
			{
				incValue *= 3;
			}
			else
			{
				incValue *= 2;
			}
		}
		m_CasketCount += incValue;
	
		if (m_CasketCount<0)
		{
			m_CasketCount = 0;
			m_CasketCountInc = 0;	
			
			CheckRegen();
		}
		else if (m_CasketCount>64)
		{
			m_bInCasket = false;
			m_CasketCount = 64;
			m_CasketCountInc = 0;

			// [货扁贱3] - -;;
			if (GetClassType()==CLASS_PLAYER)
			{		
				g_pSkillAvailable->SetAvailableSkills();
			}

			CheckRegen();
		}		
	}
}

//----------------------------------------------------------------------
// Update Status
//----------------------------------------------------------------------
// 老窜篮.. HP / MP 函拳
//----------------------------------------------------------------------
void
MCreature::UpdateStatus()
{
	//--------------------------------------------------------
	// coma 惑怕俊辑绰 HP甫 棵府瘤 臼绰促.
	//--------------------------------------------------------
	if (!m_bAlive 
		|| HasEffectStatus(EFFECTSTATUS_COMA))
	{
		return;
	}

	bool bChangedHP = false;

	//--------------------------------------------------------
	// HP 雀汗
	//--------------------------------------------------------
	if (m_RecoveryHPTimes > 0 
		&& g_CurrentTime >= m_RecoveryHPNextTime)
	{		
		m_RecoveryHPTimes--;

		// 雀汗
		SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RecoveryHPAmount );

		bChangedHP = true;
		
		// 促澜 雀汗且 矫埃 汲沥
		m_RecoveryHPNextTime += m_RecoveryHPDelayTime;		
	}

	//--------------------------------------------------------
	// MP 雀汗
	//--------------------------------------------------------
	if (m_RecoveryMPTimes > 0 
		&& g_CurrentTime >= m_RecoveryMPNextTime)
	{		
		m_RecoveryMPTimes--;

		// 雀汗
		SetStatus( MODIFY_CURRENT_MP, GetStatus(MODIFY_CURRENT_MP)+m_RecoveryMPAmount );

		// 促澜 雀汗且 矫埃 汲沥
		m_RecoveryMPNextTime += m_RecoveryMPDelayTime;		
	}

	//--------------------------------------------------------
	// 轨颇捞绢牢 版快 磊悼 雀汗
	//--------------------------------------------------------
	if (m_RegenAmount > 0)
	{
		//--------------------------------------------------------
		// 扁夯利牢 雀汗
		//--------------------------------------------------------
		if (g_CurrentTime >= m_RegenNextTime)
		{		
			// [货扁贱4] mephisto 吧府搁 HP regen 救等促.

			//if ( !m_bEffectStatus[EFFECTSTATUS_MEPHISTO] || !m_bEffectStatus[EFFECTSTATUS_MEPHISTO]&&IsInCasket() )
			{
				// 包加俊 乐阑 版快 篮单固瘤何磐 雀汗茄促.
				if( IsInCasket() )
				{
					DWORD SilverDamage = GetStatus( MODIFY_SILVER_DAMAGE );
					DWORD Value = m_RegenAmount;
					
					if( SilverDamage > 0 )
					{
						if( SilverDamage < Value )
							Value = SilverDamage;
						
						SetStatus( MODIFY_SILVER_DAMAGE, SilverDamage - Value );
					} else
					{
						SetStatus( MODIFY_CURRENT_HP, GetStatus( MODIFY_CURRENT_HP ) + Value );
					}
				}
				else if( !m_bEffectStatus[EFFECTSTATUS_MEPHISTO] )
					SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RegenAmount );

				bChangedHP = true;
			}

			m_RegenNextTime += m_RegenDelayTime;		
			

			//--------------------------------------------------------
			// 何啊利牢 雀汗 - -;
			//--------------------------------------------------------
			if (g_CurrentTime >= m_RegenBonusNextTime)
			{		
				if( GetStatus( MODIFY_SILVER_DAMAGE ) > 0 && IsInCasket() )
					SetStatus( MODIFY_SILVER_DAMAGE, max( 0, GetStatus( MODIFY_SILVER_DAMAGE ) - m_RegenBonusAmount ) );
				else
					SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RegenBonusAmount );

				//bChangedHP = true;

				m_RegenBonusNextTime += m_RegenBonusDelayTime;		
			}
		}		
	}

	//--------------------------------------------------------
	// 颇萍牢 版快绰 HP 促矫 汲沥秦霖促.
	//--------------------------------------------------------
	if (bChangedHP)
	{	
		if (IsPlayerParty())
		{
			PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
				
			// 谅钎甫 荐沥秦霖促.
			if (pInfo!=NULL)
			{
				pInfo->HP = m_Status[MODIFY_CURRENT_HP];
			}
		}
	}

}

//----------------------------------------------------------------------
// Update Invisible
//----------------------------------------------------------------------
void
MCreature::UpdateInvisible()
{
	if (m_InvisibleStep)
	{
		m_InvisibleCount += m_InvisibleStep;

		if (m_InvisibleCount>=64 || m_InvisibleCount<=0)
		{
			m_InvisibleStep = 0;
		}
	}
}

//----------------------------------------------------------------------
// 促澜 悼累阑 秒茄促.
//----------------------------------------------------------------------
void
MCreature::Action()
{	
	#ifdef OUTPUT_DEBUG						
		g_bCheckError = false;


		//if (rand()%8==5)
		{
		//	PlaySound( rand()%g_pSoundTable->GetSize() );
		}
	#endif

	//--------------------------------------------------------
	// 盲泼 String 绢涤霸 且 矫埃
	//--------------------------------------------------------
	if (m_NextChatFadeTime < g_CurrentTime)
	{
		FadeChatString();		
	}
	
	//--------------------------------------------------------
	// Status 惑怕 函拳	- HP, MP 函拳
	//--------------------------------------------------------
	UpdateStatus();
	
//	// 2004, 08, 05 sobeit add start
//	if( GetCreatureType() == 726 ||  GetCreatureType() == 727 || GetCreatureType() == 728 || GetCreatureType() == 729) // 己巩
//		return; // 关栏肺 郴妨啊 毫具 喊肺 且霸 绝促.
//	// 2004, 08, 05 sobeit add end

	//--------------------------------------------------------
	// 漂荐茄 瓤苞甸
	//--------------------------------------------------------
//	if(UpDateInstallTurret())
//		return;
	UpDateInstallTurret();

	UpdateInvisible();
	UpdateCasket();
	UpdateCutHeight();
	UpdateTurning();
	UpdateCauseCriticalWounds();
	UpdateGhost();
//	UpdateBloodyZenith();
	UpdateGunShotGuidance();
	UpdateFakeDie();
	UpdateBurningSol();

	UpdateBikeCrash();
	if( !HasEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL ) && !HasEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL ) && GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}
	
	if( GetCreatureType() == 672 )
	{
		if( HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_2 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_3 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_4 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_5 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_6 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_7 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_8 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_9 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_10 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_11 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_12 )
			)
		{
			if( GetAction() != ACTION_MOVE )
			{
				m_Action = ACTION_MOVE;				
			}
			if(GetActionCountMax() != 0)
			{
				m_ActionCount = g_CurrentFrame % GetActionCountMax();
			}
		}
		else
		{
			m_Action = ACTION_STAND;
			m_ActionCount = 0;
		}
		
		return;
	} 
	else if( GetCreatureType() == 675 )
	{
		m_Action = ACTION_MAGIC;
		m_ActionCount = 0;		
	}
	//UpdateDarkness();
	
	//--------------------------------------------------------
	// 规氢 傈券阑 磊楷胶反霸
	//--------------------------------------------------------
	if (!ChangeNearDirection())
	{
			//--------------------------------------------------------
			// 规氢阑 官曹 鞘夸啊 绝带 版快俊..
			// 缴缴且锭付促 茄锅究 规氢 官层林扁.. - -;
			//--------------------------------------------------------			
			if (// Player啊 酒聪绊
//				m_CreatureType >= 4
				!IsCreatureTypeVampirePC(m_CreatureType)
				&& m_CreatureType != CREATURETYPE_SLAYER_OPERATOR
				&& m_CreatureType != CREATURETYPE_VAMPIRE_OPERATOR
				&& m_CreatureType != CREATURETYPE_OUSTERS_OPERATOR
				&& m_CreatureType != CREATURETYPE_SLAYER_MALE
				&& m_CreatureType != CREATURETYPE_SLAYER_FEMALE
				&& m_CreatureType != CREATURETYPE_OUSTERS
				// 傍吝官配府啊 酒聪绊
				&& m_CreatureType != 431
				// NPC啊 酒聪绊... (规氢喊肺 弊覆捞 促 绝绢辑府 -_-;)
				&& !IsNPC()
				// 混酒 乐绊..
				&& m_bAlive
				// 沥瘤惑怕捞绊
				&& m_Action==ACTION_STAND
				// 框流老镑捞 绝绊
				&& m_listMoveBuffer.size()==0
				// 沥瘤悼累狼 场俊..
				&& m_ActionCount>=GetActionCountMax()-1
				// random窍霸.. - -;
				&& (rand() % 5)==0
				&& !HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
				)
			{
				m_Direction = ((int)m_Direction + 8 + rand()%5-2) % 8;
			}
	}
	//m_CurrentDirection = m_Direction;

	//--------------------------------------------------------
	// 公郊 effect啊 吧妨乐促搁 2硅 蠢府霸 框流牢促.
	//--------------------------------------------------------
	if (HasEffectStatus( EFFECTSTATUS_HAS_SLAYER_RELIC ) || HasEffectStatus( EFFECTSTATUS_HAS_VAMPIRE_RELIC ) || 
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI )|| HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA )  || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_INI )    || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO )   || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL ) ||
		HasEffectStatus( EFFECTSTATUS_AMBER_OF_GUARD ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_AVENGER ) ||
		HasEffectStatus( EFFECTSTATUS_AMBER_OF_IMMORTAL ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_CURSE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_6 ) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_ICE_FIELD_TO_CREATURE ) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_FROZEN_ARMOR_TO_ENEMY ) ||
		IsCreatureActionAttack( this ) && (	HasEffectStatus( EFFECTSTATUS_ICE_OF_SOUL_STONE ) || (IsInFuryOfGnome() && !IsOusters()/*(IsSlayer() || IsVampire())*/)	) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_JABBING_VEIN ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_2 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_3 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_4 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_6 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_7 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_8 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_9 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_10 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_11 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_12 )
		)
	{
		if (g_CurrentFrame & 0x01)
		{
			return;
		}
	}

//	BOOL bSlayer = IsSlayer();

	//--------------------------------------------------------
	// KnockBack 贸府 2001.10.9
	//--------------------------------------------------------
	if (m_bKnockBack > 0)
	{
		m_sX += m_cX;
		m_sY += m_cY;

		// knockBack 促x 灯栏搁 沥瘤悼累.
		if (--m_bKnockBack==0)
		{
			m_sX = 0;
			m_sY = 0;

			SetAction( ACTION_STAND );

			return;
		}			
	}
	//--------------------------------------------------------
	// FastMove吝俊绰 action阑 秒窍瘤 臼绰促. 
	//--------------------------------------------------------
	else if (!m_bFastMove)
	{
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{	
			int bOldNextAction = m_NextAction;

			// affectUsedActionInfo甫 角青窍绊 唱搁
			// NextActionInfo啊 官拆 荐啊 乐促.			
			TYPE_ACTIONINFO nextUsedActionInfo = m_nNextUsedActionInfo;
			TYPE_ACTIONINFO nextNextUsedActionInfo = ACTIONINFO_NULL;

			//--------------------------------------------------------
			// 捞傈俊 荤侩 吝牢 扁贱捞 乐促搁... 官肺 利侩 矫挪促.
			//--------------------------------------------------------
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{	
				m_nNextUsedActionInfo = ACTIONINFO_NULL;

				// castingEffect啊 酒流 免仿 救灯栏搁 免仿矫挪促.
				if (GetActionInfoCastingStartFrame(m_nUsedActionInfo) >= m_ActionCount)
				{
					AttachCastingEffect( m_nUsedActionInfo, TRUE );
				}

				AffectUsedActionInfo( m_nUsedActionInfo );
				m_nUsedActionInfo	= ACTIONINFO_NULL;

				nextNextUsedActionInfo = m_nNextUsedActionInfo;
			}

			m_nUsedActionInfo = nextUsedActionInfo;
			m_nNextUsedActionInfo = nextNextUsedActionInfo;

			// 2001.9.30老 眠啊
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				int actionInfoAction = GetActionInfoAction(m_nUsedActionInfo);

				// 嘎绰 悼累捞搁.. 乔 脾辫..
				// 2002.3.19 林籍 力芭.. 酒公 悼累捞唱 老窜 捞棋飘 焊咯林磊
				//if (actionInfoAction==ACTION_DAMAGED)
				{
					if //(m_Action!=ACTION_STAND 
						//&& (!IsSlayer() || IsSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
						//&& 
						(GetActionInfoCastingStartFrame(m_nUsedActionInfo) <= m_ActionCount)
					{
						AttachCastingEffect( m_nUsedActionInfo, TRUE );

						AffectUsedActionInfo( m_nUsedActionInfo );
				
						// 家府 免仿
						PlaySound( 
							(*g_pCreatureTable)[m_CreatureType].GetActionSound( 
								GetActionInfoAction(m_nUsedActionInfo)
							),
							false,
							m_X, m_Y
						);

						m_nUsedActionInfo = ACTIONINFO_NULL;					
					}
				}

				// 2001.05.21 眠啊
				// 扁贱 悼累俊辑 ACTION_STAND绰 焊咯林瘤 臼绰促.
				if (actionInfoAction!=ACTION_STAND)
				{
					SetAction( actionInfoAction );
				}
			}				

			//--------------------------------------------------------
			// 捞悼且妨促啊 嘎芭唱秦辑 促弗 action阑 角青窍霸 登绰 版快
			// 捞悼捞 救登绰 版快啊 乐菌促.
			// 措氓 --> NextAction阑 扁撅沁促啊 捞悼捞搁.. 捞悼矫虐扁
			// 2001.9.24
			//--------------------------------------------------------
			if (bOldNextAction==m_MoveAction)
			{
				SetNextAction( m_MoveAction );
			}	 			
		}
		//--------------------------------------------------------
		// 扁撅等 促澜 青悼... 
		//--------------------------------------------------------
		else if (m_bNextAction 
				|| m_NextAction!=ACTION_STAND 
				&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
				&& (!IsSlayer() || IsSlayer() && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
		{
			SetAction( m_NextAction );
		}	 
	}

	//--------------------------------------------------------
	// 付厚 灯阑 锭..
	//--------------------------------------------------------
	if(HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS) && !IsSlayer())
	{
		//--------------------------------------------------------
		// 扁贱阑 荤侩窍妨绰 版快
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}
	}
	else
	if (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
		|| HasEffectStatus(EFFECTSTATUS_EXPLOSION_WATER)
		|| HasEffectStatus(EFFECTSTATUS_GLACIER)
		)
	{			
		//--------------------------------------------------------
		// 扁贱阑 荤侩窍妨绰 版快
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}

		if (m_ActionCount>=GetActionCountMax())
		{			
			//--------------------------------------------------------
			// 格钎 鸥老俊 档馒茄 惑怕俊辑..
			// 拌加 辑 乐芭唱.. 促 吧篮 版快绰.. 
			// 力磊府俊辑 如甸芭府绰 葛嚼阑 钎泅秦霖促.
			//--------------------------------------------------------
			if (m_MoveCount>=m_MoveCountMax)
			{		
				BOOL bSlayer = IsSlayer();

				if (m_bNextAction 
					|| m_NextAction!=ACTION_STAND 
					&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
					&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
				{
					m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
					if(IsOusters() && HasEffectStatusSummonSylph( this ))
						m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;

					SetAction( m_NextAction );
				}	 
				else
				{
					SetAction( ACTION_STAND );
				}
			}
		}
	}
	//--------------------------------------------------------
	// 老馆利牢 版快
	//--------------------------------------------------------
	else
	{
		//--------------------------------------------------------
		// 乔 汝府绰芭 眉农
		//--------------------------------------------------------
		CheckDropBlood();

		//--------------------------------------------------------
		// Action 贸府
		//--------------------------------------------------------
		if (m_ActionCount < GetActionCountMax())
		{		
			//--------------------------------------------------------
			// 扁贱阑 荤侩窍妨绰 版快
			//--------------------------------------------------------
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				ActionEffect();
			}
			//--------------------------------------------------------
			// 青悼..
			//--------------------------------------------------------
			else
			{
				// 利例茄 Action 荐青
				//switch (m_Action)
				if (m_Action==ACTION_STAND 
					|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
				{
					m_ActionCount++;
				}

				/*
				else if (m_Action==ACTION_MOVE
					|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_MOVE)
				{
						// 叭绰 悼累篮 蝶肺 贸府茄促.
						//ActionMove();
				}
				*/
				
				//	default :
				else 
				{
					ActionEffect();
				}
			}
		}
		
		//--------------------------------------------------------
		// 混酒 乐绰 版快父
		//--------------------------------------------------------
		if (m_bAlive)
		{
			//--------------------------------------------------------
			// Action捞 场抄 版快...
			//--------------------------------------------------------
			if (m_ActionCount>=GetActionCountMax())
			{			
				//--------------------------------------------------------
				// 格钎 鸥老俊 档馒茄 惑怕俊辑..
				// 拌加 辑 乐芭唱.. 促 吧篮 版快绰.. 
				// 力磊府俊辑 如甸芭府绰 葛嚼阑 钎泅秦霖促.
				//--------------------------------------------------------
				if (m_MoveCount>=m_MoveCountMax)
				{		
					//#ifdef CONNECT_SERVER
					//--------------------------------------------------------
					// 促澜 捞悼捞 绝促搁.. 沥瘤矫挪促.
					//--------------------------------------------------------
					AffectMoveBuffer();

					BOOL bSlayer = IsSlayer();

					if (m_bNextAction 
						|| m_NextAction!=ACTION_STAND 
						&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
						&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
					{
						m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
						if(IsOusters() && HasEffectStatusSummonSylph( this ))
							m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;
						SetAction( m_NextAction );
					}	 
					else
					{
						SetAction( ACTION_STAND );				
					}
					//#else
					//	SetAction( m_MoveAction );				
					//#endif
				}
				//--------------------------------------------------------
				// 叭促啊 促弗 action阑 焊咯玲芭唱 拌加 叭带 吝捞促.
				// 促矫 叭绰促.
				//--------------------------------------------------------
				else 
				{			
					m_Action = m_MoveAction;				
					
					ActionMove();

					if (m_MoveCount>=m_MoveCountMax)
					{
						// 促澜 捞悼且 镑捞 乐栏搁..
						if (m_NextX != SECTORPOSITION_NULL
							&& m_NextDirection != DIRECTION_NULL)
						{		
							MoveNextPosition();
						}
						else
						{
							//#ifdef CONNECT_SERVER

							//--------------------------------------------------------
							// 促澜 捞悼
							//--------------------------------------------------------
							AffectMoveBuffer();

							BOOL bSlayer = IsSlayer();

							if (m_bNextAction 
								|| m_NextAction!=ACTION_STAND 
								&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND )
								&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
							{
								m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
								if(IsOusters() && HasEffectStatusSummonSylph( this ))
									m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;
								SetAction( m_NextAction );
							}	 
							else
							{
								SetAction( ACTION_STAND );				
							}
							//#else
							//	SetAction( m_MoveAction );				
							//#endif
						}
					}
				}
			}
			//--------------------------------------------------------
			// Action阑 焊咯林绰 吝捞扼档.. 
			// 捞悼且 波府?啊 乐栏搁 捞悼档 矫难霖促. 2001.10.10
			//--------------------------------------------------------
			else
			{		
				/*
				// 2001.11.8 - 林籍贸府.. 焊扁 救 亮促唱.. - -;;
				if (// KnockBack捞唱 FastMove啊 酒囱 版快
					!m_bFastMove && m_bKnockBack==0	&& m_MoveCount>0)
				{
					if (m_MoveCount<m_MoveCountMax)
					{
						ActionMove();
					}
					else if (m_MoveCount>=m_MoveCountMax)
					{
						// 促澜 捞悼且 镑捞 乐栏搁..
						if (m_NextX != SECTORPOSITION_NULL)
						{		
							MoveNextPosition();
						}
					}
				}
				*/
			}
		}
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] Action OK");
		}
	#endif		
}

//----------------------------------------------------------------------
// Packet Move (x, y, d)
//----------------------------------------------------------------------
// this Creature绰 Move甫 罐疽栏骨肺
//
// //0. Player客狼 谅钎 面倒阑 绊妨秦辑 框流咯霖促.
//
// (x,y)俊辑 d规氢栏肺 茄 沫 框流牢促.
//----------------------------------------------------------------------
void		
MCreature::PacketMove(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE direction)
{
//	// 2004, 9, 16, sobeit add start - 磐房 规氢 官操扁, 滑鳖..ぱぱ;
//	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
//	{
//		SetCurrentDirection(direction);
//		return;
//	}
//	// 2004, 9, 16, sobeit add end
	//---------------------------------------------------
	// 货肺款 捞悼 沥焊甫 buffer俊 眠啊矫挪促.
	//---------------------------------------------------
	MoveNode* pNode = new MoveNode;
	pNode->x = x;
	pNode->y = y;
	pNode->direction = direction;

	if (IsPlayerParty())
	{
		PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );		
			
		// 谅钎甫 荐沥秦霖促.
		if (pInfo!=NULL)
		{
			pInfo->zoneID = (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);
			pInfo->zoneX = x;
			pInfo->zoneY = y;
		}
	}

	//---------------------------------------------------
	// Server谅钎绰 官肺 利侩矫挪促.
	//---------------------------------------------------
	///*
	int oldX = m_X;
	int oldY = m_Y;

	m_X = x;
	m_Y = y;

	POINT next;
	GetNextPosition( direction, next );

	m_X = oldX;
	m_Y = oldY;

	// server谅钎 扁撅
	// 2001.8.6
	// 2002.3.22 促矫 混啡促.
	//m_ServerX	= next.x;
	//m_ServerY	= next.y;
	
	SetServerPositionWithBlock( next.x, next.y );
	//*/

	m_listMoveBuffer.push_back( pNode );

	//-----------------------------------------------------------
	// 沥瘤等 惑怕捞搁 官肺 利侩茄促.
	//-----------------------------------------------------------
	if (m_Action==ACTION_STAND 
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		AffectMoveBuffer();
	}
	else
	{
		//---------------------------------------------------
		// 茄拌甫 逞绰 MoveBuffer绰 葛滴 利侩矫难滚赴促.
		//---------------------------------------------------
		while (m_listMoveBuffer.size() > g_pClientConfig->MAX_CREATURE_MOVE_BUFFER)
		{
			AffectMoveBuffer();
		}		
	}
}

//----------------------------------------------------------------------
// Release MoveBuffer
//----------------------------------------------------------------------
// 扁粮狼 MoveBuffer甫 促 瘤框
//----------------------------------------------------------------------
void
MCreature::ReleaseMoveBuffer()
{
	MOVENODE_LIST::iterator iNode = m_listMoveBuffer.begin();

	while (iNode != m_listMoveBuffer.end())
	{
		MoveNode* pNode = *iNode;

		delete pNode;

		iNode++;
	}

	m_listMoveBuffer.clear();
}

//----------------------------------------------------------------------
// Affect MoveBuffer All
//----------------------------------------------------------------------
// 葛电 MoveBuffer甫 利侩矫挪促.
//----------------------------------------------------------------------
void
MCreature::AffectMoveBufferAll()
{
	while (m_listMoveBuffer.size() != 0)
	{
		AffectMoveBuffer();
	}
}

//----------------------------------------------------------------------
// Affect Move Buffer
//----------------------------------------------------------------------
// Buffering等 促澜狼 捞悼 沥焊甫 窍唱 汲沥茄促.
//----------------------------------------------------------------------
bool	
MCreature::AffectMoveBuffer()
{
	//-------------------------------------------
	// 捞悼且 沥焊啊 绝绰 版快
	//-------------------------------------------
	if (m_listMoveBuffer.size()==0)
	{
		return false;
	}

	//-------------------------------------------
	// 俩 菊俊铂 佬绢柯促.
	//-------------------------------------------
	MoveNode* pNode = m_listMoveBuffer.front();
	m_listMoveBuffer.pop_front();

	//-------------------------------------------
	// 蔼 汲沥
	//-------------------------------------------
	// 盔贰绰 PacketMove()俊 乐带扒单
	// move buffering 眠啊窍搁辑 cut & paste - -;;
	// 肋 倒酒哎妨唱..
	//-------------------------------------------
	int x = pNode->x;
	int y = pNode->y;
	int direction = pNode->direction;

	delete pNode;

	// 磷篮 版快
	if (!m_bAlive)
	{
		return false;
	}


	DEBUG_ADD_FORMAT("AffectMoveBuffer : [ID=%d] From(%d,%d) Direction(%d)", m_ID, x,y, direction);

	// 捞傈 谅钎甫 扁撅秦敌促.		
	int oldX = m_X;
	int oldY = m_Y;

	// 烙矫肺.. 货 谅钎甫 汲沥
	m_X = x;
	m_Y = y;	// m_X=x; m_Y=y;  and.. Etc...
		
	// Server俊辑 朝酒柯 规氢栏肺 框流牢 谅钎甫 备茄促.
	POINT next;
	GetNextPosition( direction, next );

	// 康开 逞绢啊绰瘤 眉农 . 2001.10.7
	if (next.x < 0 
		|| next.y < 0 
		|| next.x >= g_pZone->GetWidth() 
		|| next.y >= g_pZone->GetHeight()) 
	{
		m_X = oldX;
		m_Y = oldY;

		return false;
	}

	// server谅钎 扁撅
	// 2001.8.6
	// 林籍 2002.3.22... PacketMove肺 颗板促.
	//SetServerPositionWithBlock( next.x, next.y );	

	// 盔贰 谅钎肺 登倒赴促.
	m_X = oldX;
	m_Y = oldY;

	//-----------------------------------------------------
	// 哎 荐 绝绰 镑牢 版快...
	//-----------------------------------------------------
	// Server俊辑 肋给等 沥焊啊 吭促.. - -;;
	// timing巩力扼绊 且鳖?
	#ifdef OUTPUT_DEBUG
		//if (!m_pZone->CanMove(m_MoveType, next.x, next.y))
		{
		//	DEBUG_ADD_FORMAT("[ Collide ] Creature[ID=%d] Can't Move to (%d,%d) - But, Go!", m_ID, next.x, next.y);				
			
			// 弊贰档 框流咯焊磊~
			//return;
		}
	#endif

	//--------------------------------------------------
	// Server俊辑 八刘等 谅钎甫 汲沥茄促.
	//--------------------------------------------------
	//m_ServerX = x;//next.x;
	//m_ServerY = y;//next.y;

	// 酒流 捞傈 沥焊啊 update登瘤 臼疽栏搁...
	if (m_NextX != SECTORPOSITION_NULL)
		// && m_NextY != SECTORPOSITION_NULL
	{
		// message
		DEBUG_ADD_FORMAT("[ SlowUpdate ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", 
							m_ID, 
							m_X, m_Y, 
							m_NextX, m_NextY, 
							g_pPlayer->GetX(), g_pPlayer->GetY());			
		
		// 捞傈 沥焊甫 官肺 update矫挪促.
		MovePosition(m_NextX, m_NextY);
		m_Direction = m_NextDirection;
		m_DirectionMove = m_NextDirection;

		// (m_X, m_Y)俊辑 m_CurrentDirection栏肺 捞悼矫挪促.
		SetNextAction( m_MoveAction );

		m_NextX = x;
		m_NextY = y;
		m_NextDirection = direction;	

		return true;
	}

	//--------------------------------------------------
	// Player啊 捞固 哎 磊府俊 乐促搁....
	//--------------------------------------------------
	// 给 埃促~~
	/*
	if (g_pPlayer->GetX()==next.x && g_pPlayer->GetY()==next.y)
	{
		// 规氢父 汲沥..
		//pCreature->SetDirection( pGCMove->getDir() );	
		
		// 捞傈俊 乐带 谅钎俊辑 框流捞妨绊 茄 版快..
		if (oldX==x && oldY==y)
		{
			// 弊成 辑 乐栏搁 等促.
		}
		else
		{
			// 捞傈俊 乐带 谅钎(oldX, oldY)俊辑 货谅钎(x,y)肺 捞悼茄促.
			m_X = oldX;
			m_Y = oldY;

			MovePosition( x, y );			

			// 货 谅钎俊辑 规氢(direction)栏肺 框流咯具 窍瘤父
			// 弊成 货 谅钎俊 辑 乐绢具 茄促.
		}

		// message		
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif
	}
	*/
	//--------------------------------------------------
	// 格利瘤俊 捞固 啊 乐绰 版快绰 框流老 鞘夸绝促.
	//--------------------------------------------------		
	//else 
	if (m_X==next.x && m_Y==next.y)
	{	
		// message
		DEBUG_ADD_FORMAT("[ Stand ] [ID=%d] : Already Reached (%d, %d) ,  Player=(%d, %d)", m_ID, oldX, oldY, g_pPlayer->GetX(), g_pPlayer->GetY());						
	}
	//--------------------------------------------------
	// 格利瘤俊 啊 乐瘤 臼篮 版快 --> 框流牢促.
	//--------------------------------------------------
	else
	{		
		//--------------------------------------------------
		// 捞傈俊 乐带 困摹俊辑 框流捞绰 巴捞搁.. 
		//--------------------------------------------------
		if (m_X==x && m_Y==y)
		{
			// 框流捞绰 悼累捞 场抄 惑怕捞搁..
			if (m_MoveCount>=m_MoveCountMax)
			{
				DEBUG_ADD_FORMAT("[ MoveSetting ] [ID=%d] From(%d, %d) to Direction(%d) ,  Player=(%d, %d)", m_ID, x,y, direction, g_pPlayer->GetX(), g_pPlayer->GetY());						
				
				m_Direction = direction;
				m_DirectionMove = direction;
				SetNextAction( m_MoveAction );
				//m_bNextAction = true;
			}
			// 酒流 框流捞绊 乐绰 吝捞搁..
			else
			{
				DEBUG_ADD_FORMAT("[ MoveBuffering ][ID=%d] Current(%d, %d), Next(%d, %d) to Direction(%d)", m_ID, m_X, m_Y, x,y, direction);						
				
				m_NextX = x;
				m_NextY = y;
				m_NextDirection = direction;
			}
		}
		//--------------------------------------------------
		// jump窍霸 登绰 版快俊...
		//--------------------------------------------------
		// 捞傈俊 乐带 Creature狼 困摹甫 力芭茄促.
		else
		{
			// 捞傈俊 乐带 谅钎(m_X, m_Y)俊辑 货谅钎(x,y)肺 捞悼茄促.			
			MovePosition( x, y );			
			
			// 货 谅钎俊辑 规氢(direction)栏肺 框流咯具 茄促.
			m_Direction = direction;
			m_DirectionMove = direction;
			m_NextDirection = direction;	// 2001.11.6
			SetNextAction( m_MoveAction );

			// message
			DEBUG_ADD_FORMAT("[ Jump ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_ID, m_X, m_Y, next.x, next.y, g_pPlayer->GetX(), g_pPlayer->GetY());
		}			
		
		// 规氢阑 沥窍绊 框流捞档废 茄促.
		//m_CurrentDirection = direction;
		//SetNextAction( m_MoveAction );
	}	

	return true;
}

//----------------------------------------------------------------------
// Get ActionInfo's Action
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 ACTION阑 茫绰促.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoAction(TYPE_ACTIONINFO nActionInfo, bool IsSelfAction)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	int action;
	
	
	// 2004, 11, 23, sobeit add start - set afire
//	if(nActionInfo == SKILL_SET_AFIRE && !IsFastMove())
//		return ACTION_ATTACK;
	// 2004, 11, 23, sobeit add end
	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponAction())
	{
		action = (*g_pActionInfoTable)[m_nBasicActionInfo].GetAction();
	}
	else
	{
		action = (*g_pActionInfoTable)[nActionInfo].GetAction();	
	}

	if(action == ACTION_MAGIC && IsAdvancementClass())
	{
		if(!(*g_pActionInfoTable)[nActionInfo].IsTargetSelf() && !IsSelfAction)
		{
			if(IsSlayer())
				return ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK;
			else if(IsOusters())
				return ACTION_ADVANCEMENT_OUSTERS_MAGIC_ATTACK;
			else 
				return ACTION_ADVANCEMENT_MAGIC_ATTACK;

		}
	}
	//-------------------------------------------------------------
	// 公扁 加档俊 蝶扼辑 action捞 崔扼柳促.
	//-------------------------------------------------------------
	if (IsSlayer())
	{
		if (m_CreatureType==CREATURETYPE_SLAYER_OPERATOR)
		{
			if (action>ACTION_DAMAGED
				|| action==ACTION_ATTACK)
			{
				return ACTION_ATTACK;
			}
		}
		
		//-----------------------------------------------------
		// Lightning Hand绰 公炼扒 ACTION_SLAYER_SWORD_2捞促. - -;d
		//-----------------------------------------------------
		if (nActionInfo==SKILL_LIGHTNING_HAND || nActionInfo == SKILL_LARSLASH )
		{
			return ACTION_SLAYER_SWORD_2;
		} 
		else
		if (nActionInfo == SKILL_BLITZ_SLIDING_ATTACK)
		{
			return ACTION_SLAYER_SWORD_2_FAST;
		} else
		if ( nActionInfo==SKILL_BLAZE_WALK_ATTACK )
		{
			return ACTION_SLAYER_BLADE_2_FAST;
		}
		else 
		if( nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER )
		{
			return s_SlayerActionSpeed[action][1];
		}
		return s_SlayerActionSpeed[action][m_WeaponSpeed];
	} 
	else if( IsOusters() )
	{
		if(nActionInfo == SKILL_DISTANCE_BLITZ)
			return action;

		// test code
//		if(nActionInfo == SKILL_FATAL_SNICK || nActionInfo == SKILL_FATAL_SNICK_MASTERY)
//			action = ACTION_OUSTERS_CHAKRAM;
//		
		if(m_CreatureType==CREATURETYPE_OUSTERS)
			return s_OustersActionSpeed[action][m_WeaponSpeed];		
	}

	if (m_CreatureType==CREATURETYPE_WOLF)		
	{
		if (action==ACTION_ATTACK)
		{
			return ACTION_ATTACK;
		}
	}

	if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR || m_CreatureType==CREATURETYPE_SLAYER_OPERATOR || m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR)
	{
		if (action>ACTION_DAMAGED
			|| action==ACTION_ATTACK)
		{
			return ACTION_ATTACK;
		}		
	}
	// 2004, 7, 27 sobeit add start
	if (m_CreatureType==717 || m_CreatureType==721 || m_CreatureType==723) // 龙靛饭, 龙靛饭 盒脚, 阿己 龙靛饭
	{
		if(action <ACTION_MAX_OUSTERS)
			return action;
	}
	// 2004, 7, 27 sobeit add end
	if(m_CreatureType == 755 || m_CreatureType == 756 || m_CreatureType == 757 || m_CreatureType == 758 ||
		m_CreatureType == 759 || m_CreatureType == 760 || m_CreatureType == 761 || m_CreatureType == 762 || m_CreatureType == 763)
	{
		if(action==ACTION_ATTACK)
		{
			if (rand()%2)
				action = ACTION_MAGIC;
			else
				action = ACTION_ATTACK;
		}
	}
	

//	if (IsCreatureTypeVampirePC(m_CreatureType))
//		return s_VampireActionSpeed[action][m_WeaponSpeed];	


	return s_VampireActionSpeed[action][m_WeaponSpeed];	
//	return action;
}

//----------------------------------------------------------------------
// Get ActionInfo's CastingStartFrame
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 CastingStartFrame阑 茫绰促.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoCastingStartFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 7;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 4;
//			}
//		}
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetCastingStartFrame( m_WeaponSpeed );
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetCastingStartFrame( m_WeaponSpeed );	
}

//----------------------------------------------------------------------
// Get ActionInfo's RepeatStartFrame
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 RepeatStartFrame阑 茫绰促.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoRepeatStartFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return GetActionCountMax();
	}

	int repeat;

	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 7;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 4;
//			}
//		}
		if (m_nBasicActionInfo < g_pActionInfoTable->GetSize())
		{
			repeat = (*g_pActionInfoTable)[m_nBasicActionInfo].GetRepeatStartFrame( m_WeaponSpeed );
		}
		else
		{
			repeat = GetActionCountMax();
		}
	}
	else
	{
		repeat = (*g_pActionInfoTable)[nActionInfo].GetRepeatStartFrame( m_WeaponSpeed );
	}

	
	if (repeat==-1)
	{
		return 0;
	}

	return repeat;
}
		
//----------------------------------------------------------------------
// Get ActionInfo's RepeatEndFrame
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 RepeatEndFrame阑 茫绰促.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoRepeatEndFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return GetActionCountMax();
	}

	int repeat;
	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 17;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 14;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 14;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 14;
//			}
//		}
		if (m_nBasicActionInfo < g_pActionInfoTable->GetSize())
		{
			repeat = (*g_pActionInfoTable)[m_nBasicActionInfo].GetRepeatEndFrame( m_WeaponSpeed );		
		}
		else
		{
			repeat = GetActionCountMax();
		}
	}
	else
	{
		repeat = (*g_pActionInfoTable)[nActionInfo].GetRepeatEndFrame( m_WeaponSpeed );
	}

	if (repeat==-1)
	{
		return GetCreatureActionCountMax( this, m_Action );//(*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	}

	return repeat;
}

//----------------------------------------------------------------------
// Get ActionInfo's CastingFrames
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 CastingFrames阑 茫绰促.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoCastingFrames(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingFrames())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetCastingFrames( m_WeaponSpeed );
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetCastingFrames( m_WeaponSpeed );	
}

//----------------------------------------------------------------------
// Get ActionInfo's Delay
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 Delay甫 茫绰促.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoDelay(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}


//	if(nActionInfo == SKILL_FATAL_SNICK || nActionInfo == SKILL_FATAL_SNICK_MASTERY)
//	{
//		switch( GetWeaponSpeed() )
//		{
//		case WEAPON_SPEED_SLOW :
//			return 25 * 1000 / 16;
//		case WEAPON_SPEED_NORMAL :
//			return 23 * 1000 / 16;
//		case WEAPON_SPEED_FAST :
//			return 21 * 1000 / 16;
//		}
//	} 
	//-------------------------------------------------------------
	// 扁夯 action狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponDelay())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetDelay();
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetDelay();	
}

//----------------------------------------------------------------------
// Get Casting SoundID
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 ACTION阑 茫绰促.
//----------------------------------------------------------------------
TYPE_SOUNDID
MCreature::GetCastingSoundID(TYPE_ACTIONINFO nActionInfo)
{
	//-------------------------------------------------------------
	// 扁夯 公扁狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponSound())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetSoundID();
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetSoundID();	
}

//----------------------------------------------------------------------
// Get ActionInfo ActionEffectSpriteType
//----------------------------------------------------------------------
// nActionInfo俊 利例茄 ActionEffectSpriteType阑 茫绰促.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoActionEffectSpriteType(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	int useActionInfo;


	//-------------------------------------------------------------
	// 扁夯 公扁狼 利侩阑 罐绰啊?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponActionEffectSpriteType())
	{
		useActionInfo = GetBasicActionInfo(); //m_nBasicActionInfo;
	}
	else
	{
		useActionInfo = nActionInfo;
	}

	// plus父怒 歹茄促.
	// 烙矫 东户~~ 弊矾唱.. 菊栏肺档 绊摹瘤 臼摆瘤 - -;
	useActionInfo += (*g_pActionInfoTable)[nActionInfo].GetAffectCurrentWeaponActionInfoPlus();


	TYPE_EFFECTSPRITETYPE re_est = EFFECTSPRITETYPE_NULL;

	//------------------------------------------------------------
	// 巢磊牢 版快
	//------------------------------------------------------------
	if (IsMale())
	{
		re_est = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteType();
		if(nActionInfo == SKILL_BULLET_OF_LIGHT)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SMG_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SR_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SG_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_MALE;
				break;
			}
		} 
		else
		if(nActionInfo == SKILL_JABBING_VEIN)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SMG_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SR_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SG_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_MALE;
				break;
			}
		}
		return re_est;
	}
	
	//------------------------------------------------------------
	// 咯磊侩 Effect check
	//------------------------------------------------------------	
	TYPE_EFFECTSPRITETYPE effectSpriteTypeFemale = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteTypeFemale();

	if (effectSpriteTypeFemale!=EFFECTSPRITETYPE_NULL)
	{
		re_est = effectSpriteTypeFemale;
		if(nActionInfo == SKILL_BULLET_OF_LIGHT)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SMG_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SR_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SG_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_FEMALE;
				break;
			}
		} 
		else
		if(nActionInfo == SKILL_JABBING_VEIN)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SMG_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SR_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SG_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_FEMALE;
				break;
			}
		}
		return re_est;
	}
	
	// 绝绰 版快绰 巢磊波 荤侩..
	re_est = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteType();
	if(nActionInfo == SKILL_BULLET_OF_LIGHT)
	{
		switch(re_est)
		{
		case EFFECTSPRITETYPE_GUN_AR_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SMG_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SR_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SG_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_MALE;
			break;
		}
	} 
	else if(nActionInfo == SKILL_JABBING_VEIN)
	{
		switch(re_est)
		{
		case EFFECTSPRITETYPE_GUN_AR_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SMG_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SR_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SG_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_MALE;
			break;
		}
	}
	return re_est;	
}


//----------------------------------------------------------------------
// Packet SpecialAction Result
//----------------------------------------------------------------------
// this Creature绰 Damage甫 罐疽栏骨肺
//
// 0. 葛电 悼累阑 吝瘤窍绊..
// 1. Action --> Damage罐绰 悼累
// 2. Damage 钎矫
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	/*
	// 咯扁辑 return秦滚府搁 磷阑锭 嘎绰 捞棋飘啊 救 焊牢促.
	if (!m_bAlive)
	{
		// 磷菌阑锭绰 磷绰 effect富绊绰 焊咯林搁 救等促.
		if (nResultActionInfo!=(*g_pCreatureTable)[m_CreatureType].DeadActionInfo)
			return;
	}
	*/

	//----------------------------------------------------------------------
	// 搬苞 action捞 酒囱 版快..  - 狼固啊 乐唱?? 褥..
	//----------------------------------------------------------------------
	if (nResultActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo()
		// fast move, knockback吝俊绰 action阑 焊咯林瘤 臼绰促... // 2001.10.9
		|| m_bFastMove || m_bKnockBack || nResultActionInfo == ACTIONINFO_NULL)
	{
		return;
	}


	//----------------------------------------------------------------------
	// 规绢阜 瓤苞.. - -;
	//----------------------------------------------------------------------
	if (GetActionInfoAction( nResultActionInfo )==ACTION_DAMAGED)
	{
		// 磷菌阑锭 嘎绰 悼累 救 焊咯林扁..
		// 鸥老 扁贱锭巩俊 嘎扁档 窍歹扼绰.. - -;
		// 2002.3.9
		if (!m_bAlive)
		{
			return;
		}

		//if (HasEffectStatus(EFFECTSTATUS_AURA_PRISM))
		{
		//	nResultActionInfo = REFLECT_AURA_PRISM;
		}
		//else 
			if (HasEffectStatus(EFFECTSTATUS_AURA_SHIELD))
		{
			nResultActionInfo = REFLECT_AURA_PRISM;//REFLECT_AURA_SHIELD;
		}

//		#ifndef CONNECT_SERVER
//			if (HasEffectStatus(EFFECTSTATUS_ARMAGEDDON))
//			{				
//				int hp = GetArmageddonHP()-30;
//				hp = max(0, hp);
//
//				SetStatus( MODIFY_ARMAGEDDON_HP, hp );
//			}
//		#endif
	}


	// 捞悼阑 吝瘤茄促.
	////m_sX = 0;	
	////m_sY = 0;	
	//SetStop();

	//m_ActionCount = 0; 
	//------------------------------------------------------
	// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
	//------------------------------------------------------
	AffectBufferedActionInfo();
	
	ActionMoveNextPosition();

	// nResultActionInfo俊 秦寸窍绰 ActionInfo甫 茫酒具 茄促.
	// 盔贰ActionInfo + MIN_RESULT_ACTIONINFO甫 窍搁 等促.
	m_nNextUsedActionInfo	= nResultActionInfo; // +(*g_pActionInfoTable).GetMinResultActionInfo()

	if( temp != 0)
		SetActionGrade( temp );

	if (m_bAlive)
	{
		BYTE nextAction = GetActionInfoAction(m_nNextUsedActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}
	}

	// 磊脚俊霸 荤侩?..
	SetTraceID( id );
	m_TraceX	= sX;
	m_TraceY	= sY;

	// Test Code
	/*
	if (rand()%2)
		SetChatString( "Shit!" );
	else
		SetChatString( "Oops!" );
	*/
}


//----------------------------------------------------------------------
// Packet AttackNormal (x,y, d)
//----------------------------------------------------------------------
// this Creature绰 Attack 悼累阑 秒茄促.
//
// 0. 葛电 悼累阑 吝瘤窍绊..
// 1. Action Attack
//----------------------------------------------------------------------
void		
MCreature::PacketAttackNormal(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE direction)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive)
		return;

	// 捞悼阑 吝瘤茄促.
	m_sX = 0;	
	m_sY = 0;
	
	// server俊辑 朝酒柯~ 沥焊 汲沥..
	//--------------------------------------------------
	// Player啊 捞固 弊 磊府俊 乐促搁....
	//--------------------------------------------------
	// 给 埃促~~
	/*
	if (g_pPlayer->GetX()==sX && g_pPlayer->GetY()==sY)
	{
		// message
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move To Attack] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif

		// 泅犁 Player啊 乐绰 困摹肺 官扼夯促.
		SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );
	}	
	else
	*/
	{
		//--------------------------------------------------
		// 促弗 Creature啊 捞固 乐促搁?
		//--------------------------------------------------
		/*
		bool bIsExistCreature;
		switch (m_MoveType)
		{
			case CREATURE_UNDERGROUND : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistUndergroundCreature();
			break;

			case CREATURE_GROUND : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistGroundCreature();
			break;

			case CREATURE_FLYING : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistFlyingCreature();
			break;
		}
		*/	

		// 泅犁 谅钎客 鞍篮 版快.. 框流老 鞘夸啊 绝促.
		if (m_X==sX && m_Y==sY)
		{
		}
		// 谅钎啊 促福搁.. 老窜 框流咯林绊 Attack秦具 茄促.
		else
		{
			// 捞悼
			MovePosition(sX, sY);
			
			// message
			DEBUG_ADD_FORMAT("[ Move To Attack ] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_X, m_Y, sX, sY, g_pPlayer->GetX(), g_pPlayer->GetY());								
		}	

		// Server俊辑 朝酒柯 沥焊 汲沥..		
		m_Direction = direction;
	}		

	m_ActionCount = 0; 

	// 傍拜窍绰 motion
	m_nNextUsedActionInfo	= SKILL_ATTACK_MELEE;
	SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );	
}

//----------------------------------------------------------------------
// Packet SpecialAction To Sector
//----------------------------------------------------------------------
// this Creature绰 nActionInfo俊 蝶弗 悼累阑 秒茄促.
//
// 0. 葛电 悼累阑 吝瘤窍绊..
// 1. SpecialAction
//----------------------------------------------------------------------
void		

MCreature::PacketSpecialActionToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, MActionResult* pActionResult)
{
	DEBUG_ADD_FORMAT("PacketSpecialActionToSector ai : %d", nActionInfo );
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive || (
		nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() &&
		!(nActionInfo>=STEP_SKILL_FLOURISH_2 && nActionInfo <MAGIC_ENCHANT_OPTION_NULL )
		)
		)
	{
		//DEBUG_ADD("delete?");
		if (pActionResult!=NULL)
		{	
			DEBUG_ADD("delete");
			delete pActionResult;
		}
//		DEBUG_ADD("return");
		return;
	}

	//------------------------------------------------------
	// Sector俊 荤侩窍绰 扁贱捞 嘎绰 版快
	//------------------------------------------------------
	//DEBUG_ADD("fuck");
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetZone() ||
		nActionInfo == SKILL_TRANSFUSION || nActionInfo == SKILL_PIERCING || nActionInfo == SKILL_VIOLENT_PHANTOM 
		|| nActionInfo == SKILL_ICE_LANCE_MASTERY || nActionInfo == SKILL_ICE_LANCE
		|| nActionInfo == SKILL_DESTRUCTION_SPEAR_MASTERY || nActionInfo == SKILL_DESTRUCTION_SPEAR
		|| nActionInfo == SKILL_SPIT_STREAM || nActionInfo == SKILL_ROTTEN_APPLE || nActionInfo == SKILL_VAMPIRE_INNATE_DEADLY_CLAW
		)
	{
	//	DEBUG_ADD("hmhm");
		//---------------------------------------------------------------
		// 橙橙.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();
	//	DEBUG_ADD("3");

		ActionMoveNextPosition();
	//	DEBUG_ADD("2");

		// 捞悼阑 吝瘤茄促.
	//	m_sX = 0;	
	//	m_sY = 0;
		if (nActionInfo==m_nSpecialActionInfo && m_nSpecialActionInfo!=ACTIONINFO_NULL)
		{
			m_RepeatCount ++;
		}
		else
		{
			m_RepeatCount = 0;
			m_ActionCount = 0;
		}

		m_nSpecialActionInfo = nActionInfo;
		//------------------------------------------------------
		// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
		//------------------------------------------------------
		AffectBufferedActionInfo();
	//	DEBUG_ADD("1");

		// Server俊辑 朝酒柯 沥焊 汲沥..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= sX;
		m_TraceY		= sY;
		m_TraceZ		= 0;	// z蔼篮.. 狼固啊 乐阑鳖..
		// 2004, 11, 4, sobeit modify start - 弊饭捞飘 矾乔攫 东俊...
		if(GetCreatureType() == 764 || GetCreatureType() == 765)
		{
			SetTraceID( GetID() );
			if(nActionInfo == SKILL_CHAIN_THROWING_AXE)
				SetShadowCount(3);
		}
		else
			SetTraceID( OBJECTID_NULL );
		// 2004, 11, 4, sobeit modify end

		
		// 格钎甫 氢秦辑 官扼夯促.
		//SetDirectionToPosition(sX, sY);
		
	//	m_ActionCount = 0;

		// [ TEST CODE ]	
	//	DEBUG_ADD("0");
		BYTE nextAction = GetActionInfoAction(nActionInfo);

	//	DEBUG_ADD("9");
		SetNextAction( nextAction );
	//	DEBUG_ADD("8");

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 捞固 钎泅秦具且 搬苞啊 乐绰 版快..
		//---------------------------------------------------------------
	//	DEBUG_ADD("7");
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 捞固 乐带 搬苞甫 贸府秦霖促.	
			// [ TEST CODE ]
			// 搬苞 贸府..
			//
			// (!) m_pActionResult蔼捞 Execute俊辑 函且 荐 乐绢辑 
			//		历厘沁促啊 瘤况拎具 茄促.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;			
		}

	//	DEBUG_ADD("6");
		// 搬苞甫 汲沥
		m_pActionResult = pActionResult;

	//	DEBUG_ADD("5");
	}
	//------------------------------------------------------
	// 酒囱 版快
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Sector", (*g_pActionInfoTable)[nActionInfo].GetName());					
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Other
//----------------------------------------------------------------------
// this Creature绰 nActionInfo俊 蝶弗 悼累阑 秒茄促.
//
// 0. 葛电 悼累阑 吝瘤窍绊..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToOther(TYPE_ACTIONINFO nActionInfo, TYPE_OBJECTID id, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive || 
		nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() &&
		!( nActionInfo >= STEP_SKILL_FLOURISH_2 && nActionInfo < MAGIC_ENCHANT_OPTION_NULL )
		)
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		return;
	}
	//------------------------------------------------------
	// 鸥牢俊霸 荤侩窍绰 扁贱捞 嘎绰 版快
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		// 措惑捞 登绰 creature甫 掘绰促.
		MCreature* pCreature = g_pZone->GetCreature( id );
		
		if (pCreature==NULL)
		{
			// 弊繁 creature啊 绝阑 版快
			DEBUG_ADD_FORMAT("[Skill : %s] There's no such creature(%d)", (*g_pActionInfoTable)[nActionInfo].GetName(), id);

			if (pActionResult!=NULL)
			{	
				delete pActionResult;
			}
			
			return;
		}

		//---------------------------------------------------------------
		// 橙橙.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// 捞悼阑 吝瘤茄促.
		//m_sX = 0;	
		//m_sY = 0;

		//------------------------------------------------------
		// 捞傈 悼累捞尔 鞍篮 版快
		// 2001.7.23 眠啊
		//------------------------------------------------------
		if (nActionInfo==m_nSpecialActionInfo && m_nSpecialActionInfo!=ACTIONINFO_NULL)
		{
			m_RepeatCount ++;
		}
		else
		{
			m_RepeatCount = 0;
			m_ActionCount = 0;
		}

		m_nSpecialActionInfo = nActionInfo;

		//------------------------------------------------------
		// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server俊辑 朝酒柯 沥焊 汲沥..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= pCreature->GetX();
		m_TraceY		= pCreature->GetY();
		m_TraceZ		= pCreature->GetZ();
		SetTraceID ( id );

		// 格钎甫 氢秦辑 官扼夯促.		
		//SetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
		
		//m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 捞固 钎泅秦具且 搬苞啊 乐绰 版快..
		//---------------------------------------------------------------		
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 捞固 乐带 搬苞甫 贸府秦霖促.	
			// [ TEST CODE ]
			// 搬苞 贸府..			
			// (!) m_pActionResult蔼捞 Execute俊辑 函且 荐 乐绢辑 
			//		历厘沁促啊 瘤况拎具 茄促.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}

		// 搬苞甫 汲沥
		m_pActionResult = pActionResult;

	}
	//------------------------------------------------------
	// 酒囱 版快
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}

		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Other", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To InventoryItem
//----------------------------------------------------------------------
// casting悼累父 焊咯林霸 等促.
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToInventoryItem(TYPE_ACTIONINFO nActionInfo)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// 鸥牢俊霸 荤侩窍绰 扁贱捞 嘎绰 版快
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetItem())
	{		
		//------------------------------------------------------
		// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server俊辑 朝酒柯 沥焊 汲沥..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= m_X;
		m_TraceY		= m_Y;
		m_TraceZ		= m_Z;
		SetTraceID( m_ID );

		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 捞固 钎泅秦具且 搬苞啊 乐绰 版快..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 捞固 乐带 搬苞甫 贸府秦霖促.	
			// [ TEST CODE ]
			// 搬苞 贸府..			
			// (!) m_pActionResult蔼捞 Execute俊辑 函且 荐 乐绢辑 
			//		历厘沁促啊 瘤况拎具 茄促.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}
	}
	//------------------------------------------------------
	// 酒囱 版快
	//------------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Item", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Nobody
//----------------------------------------------------------------------
//
// 措惑捞 Zone俊 绝绰 版快...
//
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToNobody(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// 鸥牢俊霸 荤侩窍绰 扁贱捞 嘎绰 版快
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		//---------------------------------------------------------------
		// 橙橙.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		//------------------------------------------------------
		// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server俊辑 朝酒柯 沥焊 汲沥..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= x;
		m_TraceY		= y;
		m_TraceZ		= 0;
		SetTraceID( OBJECTID_NULL );

		// 格钎甫 氢秦辑 官扼夯促.		
		//SetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
		
		//m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}
	}
	//------------------------------------------------------
	// 酒囱 版快
	//------------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Other", (*g_pActionInfoTable)[nActionInfo].GetName());	
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Self
//----------------------------------------------------------------------
// this Creature绰 nActionInfo俊 蝶弗 悼累阑 秒茄促.
//
// 0. 葛电 悼累阑 吝瘤窍绊..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToSelf(TYPE_ACTIONINFO nActionInfo, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// 磷菌栏搁 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() || nActionInfo == ACTIONINFO_NULL)
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		return;
	}

	//------------------------------------------------------
	// 磊脚茄抛 荤侩窍绰 扁贱捞 嘎绰 版快
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetSelf())
	{
		//---------------------------------------------------------------
		// 橙橙.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// 捞悼阑 吝瘤茄促.
	//	m_sX = 0;	
	//	m_sY = 0;
		//------------------------------------------------------
		// 捞固 NextUsedACtionInfo啊 乐绰 版快 眉农
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server俊辑 朝酒柯 沥焊 汲沥..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= m_X;
		m_TraceY		= m_Y;
		m_TraceZ		= m_Z;	// z蔼篮.. 狼固啊 乐阑鳖..
		SetTraceID( m_ID ) ;
		
	//	m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo, true);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 捞固 钎泅秦具且 搬苞啊 乐绰 版快..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 捞固 乐带 搬苞甫 贸府秦霖促.	
			// [ TEST CODE ]
			// 搬苞 贸府..			
			// (!) m_pActionResult蔼捞 Execute俊辑 函且 荐 乐绢辑 
			//		历厘沁促啊 瘤况拎具 茄促.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}

		// 搬苞甫 汲沥
		m_pActionResult = pActionResult;
	}
	//------------------------------------------------------
	// 酒囱 版快
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Self", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Set Status
//----------------------------------------------------------------------
void	
MCreature::SetStatus(DWORD n, DWORD value)
{
	if(n >= MAX_MODIFY)
		return;

	DWORD oldValue = m_Status[n];

	m_Status[n] = value; 

	switch (n)
	{
		//--------------------------------------------------
		// Guild ID 官柴
		//--------------------------------------------------
		case MODIFY_GUILDID :
		{
			m_Status[n] = value;
			SetGuildNumber( value );
		}
		break;
		case MODIFY_UNIONID :
		{
			SetUnionGuildID( value );
		}
		break;

		// 2004, 10, 19, sobeit add start

		// 2004, 10, 19, sobeit add end
		//--------------------------------------------------
		// 己氢 官柴
		//--------------------------------------------------
		case MODIFY_ALIGNMENT :			// 己氢
		{
			//--------------------------------------------------
			// -10000 ~ 10000阑 0~4肺 官槽促.
			//--------------------------------------------------
			value = ConvertAlignment( value );
			m_Status[n] = value;	
		}
		break;

		//--------------------------------------------------
		// HP 官差绰 版快
		//--------------------------------------------------
		case MODIFY_CURRENT_HP :
			{
				const int localValue = min(GetMAX_HP()-GetSilverDamage(), value);
				AddHPModify( localValue - oldValue );

				// max甫 救 逞霸
				m_Status[n] = localValue;	

				// 颇萍牢 版快绰 HP 促矫 汲沥秦霖促.
				if (IsPlayerParty())
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
						
					// 谅钎甫 荐沥秦霖促.
					if (pInfo!=NULL)
					{
						pInfo->HP = localValue;
					}
				}
			}
		break;

		//--------------------------------------------------
		// MaxHP 官差绰 版快
		//--------------------------------------------------
		case MODIFY_MAX_HP :
			// 颇萍牢 版快绰 HP 促矫 汲沥秦霖促.
			if (IsPlayerParty())
			{
				PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
					
				// 谅钎甫 荐沥秦霖促.
				if (pInfo!=NULL)
				{
					pInfo->MaxHP = value;
				}
			}
		break;

//		//--------------------------------------------------
//		// Armageddon HP 官差绰 版快
//		//--------------------------------------------------
//		case MODIFY_ARMAGEDDON_HP :
//		{
//			int changeValue = IsChangedArmageddonSprite(oldValue, value);
//
//			if (HasEffectStatus(EFFECTSTATUS_ARMAGEDDON)
//				&& changeValue)
//			{
//				RemoveEffectStatus(EFFECTSTATUS_ARMAGEDDON);
//
//				// 促矫 眠啊				
//				if (value!=0)
//				{
//					AddEffectStatus(EFFECTSTATUS_ARMAGEDDON, 0xFFFF);
//				}
//
//				// 何辑瘤绰 何盒 钎矫
//				int skillID = ARMAGEDDON_CRASH_1 + (changeValue-1);
//				
//				//--------------------------------------------------
//				// 何辑瘤绰 Effect
//				//--------------------------------------------------		
//				ExecuteActionInfoFromMainNode(
//					skillID,
//				
//					m_X, m_Y, 0,
//					m_Direction,
//					
//					m_ID,												// 格钎俊 措茄 沥焊
//					m_X, m_Y, 0, 
//					
//					0,													// 扁贱狼 (巢篮) 瘤加 矫埃		
//					
//					NULL,
//					
//					true);
//			}
//		}
//		break;
	}
}

//----------------------------------------------------------------------
// Change To Slayer
//----------------------------------------------------------------------
// slayer肺 函脚窍绰单..
// 捞固 slayer捞搁.. return false
//----------------------------------------------------------------------
bool	
MCreature::ChangeToSlayer()
{
	//-----------------------------------------------------
	// 捞固 slayer捞搁..
	//-----------------------------------------------------
	if (IsSlayer())
	{
		return false;
	}

	ClearAttachEffect();

	//-----------------------------------------------------
	// 沥焊 汲沥
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// 沥瘤 矫挪促.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// 巢, 咯 ?
	//-----------------------------------------------------
	if (IsMale())
	{
		SetCreatureType( CREATURETYPE_SLAYER_MALE );
	}
	else
	{
		SetCreatureType( CREATURETYPE_SLAYER_FEMALE );
	}

	//-----------------------------------------------------
	// 锅陆
	//-----------------------------------------------------
	//g_pTopView->SetFadeStart(1, 31, 6,  5,5,31);

	return true;
}

//----------------------------------------------------------------------
// Change To Vampire
//----------------------------------------------------------------------
// vampire肺 函脚窍绰单..
// 捞固 vampire捞搁.. return false
//----------------------------------------------------------------------
bool
MCreature::ChangeToVampire()
{
	//-----------------------------------------------------
	// 捞固 vampire捞搁..
	//-----------------------------------------------------
	if (IsVampire())
	{
		return false;
	}

	ClearAttachEffect();

	DEBUG_ADD("MCreature::ChangeToVampire - RemoveEffectStatus OK");
	
	//-----------------------------------------------------
	// 沥焊 汲沥
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// 沥瘤 矫挪促.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// 巢, 咯 ?
	//-----------------------------------------------------
	if (IsMale())
	{
		// 函脚 家府
		PlaySound(SOUND_VAMPIRE_CHANGE_MALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_MALE1 );
	}
	else
	{
		// 函脚 家府
		PlaySound(SOUND_VAMPIRE_CHANGE_FEMALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_FEMALE1 );
	}

	//-----------------------------------------------------
	// 汗厘 力芭
	//-----------------------------------------------------
	// 救秦档 等促.
	//for (int i=0; i<ADDON_MAX; i++)
	//{
	//	ClearAddonInfo( i );
	//}

	//-----------------------------------------------------
	// 锅陆
	//-----------------------------------------------------
	//g_pTopView->SetFadeStart(1, 31, 6,  31,0,0);

	DEBUG_ADD("MCreature::ChangeToVampire OK");
	
	return true;
}

//----------------------------------------------------------------------
// Fast Move Position
//----------------------------------------------------------------------
// (m_X, m_Y) --> (x, y)
//----------------------------------------------------------------------
bool		
MCreature::FastMovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y,bool server)
{
	//if (m_X!=x || m_Y!=y)
	{
		//------------------------------------------------
		// 沥瘤
		//------------------------------------------------
		SetStop();

		m_nNextUsedActionInfo = ACTIONINFO_NULL;
		m_bNextAction = false;
		
		//------------------------------------------------
		// fast move 汲沥
		//------------------------------------------------	
		SetAction( m_MoveAction );

		//MovePosition( x, y );

		m_NextX = x;
		m_NextY = y;
		SetServerPositionWithBlock( x, y );

		// SetAction窍绊 唱辑 汲沥秦拎具 茄促.
		m_bFastMove = true;

		return true;
	}

	// 绢露电 辑滚谅钎绰 八刘灯促绊 夯促.
	//m_ServerX = x;
	//m_ServerY = y;

	return false;
}

//----------------------------------------------------------------------
// KnockBackPosition
//----------------------------------------------------------------------
bool
MCreature::KnockBackPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE Action)
{
	//if (m_X!=x || m_Y!=y)
	{
		// 哆酒 捞芭 bool酒囱淡.. m_b肺 矫累窍匙 - -; 哪颇老 促矫 窍扁 蓖瞒唱辑  --;

		if( Action == 0 )
		{
			//int knockBackCount = (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_DAMAGED );
			int knockBackCount = GetCreatureActionCountMax( this, ACTION_DAMAGED );
			
			if (knockBackCount<=0)
			{
				// 嘎扁 悼累捞 绝寸 - -;
				return false;
			}
			
			//------------------------------------------------
			// 沥瘤
			//------------------------------------------------
			SetStop();
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
			m_bNextAction = false;

			//------------------------------------------------
			// KnockBack 悼累栏肺 damaged甫 汲沥
			//------------------------------------------------	
			SetAction( ACTION_DAMAGED );
			
			//------------------------------------------------
			// 某腐磐狼 Action俊 嘎绰 Sound甫 免仿秦霖促.
			//------------------------------------------------
			TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( ACTION_DAMAGED );
			
			if (soundID!=SOUNDID_NULL)
			{
				PlaySound( soundID,
					false,
					m_X,
					m_Y);
			}
			m_bKnockBack = knockBackCount;
		}
		else
		{
			m_bKnockBack = 3;
			AddEffectStatus( EFFECTSTATUS_FADE_OUT, 8 );
		}
		
		
		//------------------------------------------------	
		// sx, sy肺 谅钎 焊沥
		//
		// 泅犁困摹(m_X, m_Y)俊辑 KnockBack困摹(x, y)肺 捞悼.
		// 
		//------------------------------------------------	
		int oldX = m_X;
		int oldY = m_Y;
		int newX = x;
		int newY = y;
		POINT	oldPoint = g_pTopView->MapToPixel(oldX, oldY);
		POINT	newPoint = g_pTopView->MapToPixel(newX, newY);
		
		// 捞悼秦具且 pixel荐 拌魂
		int gapX = newPoint.x - oldPoint.x; //+ m_sX);
		int gapY = newPoint.y - oldPoint.y; //+ m_sY);

		m_sX = -gapX;
		m_sY = -gapY;

		// 茄锅俊 捞悼秦具且 pixel荐 --> (m_cX, m_cY)俊 持绢敌促.
		m_cX = gapX / m_bKnockBack;
		m_cY = gapY / m_bKnockBack;

		
		// 谅钎啊 崔扼脸阑 版快父..
		if (newX!=oldX || newY!=oldY)
		{
			//------------------------------------------------	
			// KnockBack登绰 规氢 汲沥
			// 货谅钎俊辑 抗傈谅钎甫 官扼焊绰 规氢
			//------------------------------------------------	
			m_Direction = MTopView::GetDirectionToPosition(newX, newY, oldX, oldY);
		
			//------------------------------------------------	
			// 谅钎绰 官肺 捞悼.
			//------------------------------------------------			
			MovePosition( x, y );
		}

		SetServerPositionWithBlock( x, y );

		if( Action == 0 )
		{
			//------------------------------------------------			
			// 个俊 嘿绰 乔
			//------------------------------------------------			
			MAttachEffect* pEffect = NULL;
			if(g_pUserInformation->GoreLevel == false)
			{
				pEffect = CreateAttachEffect( EFFECTSPRITETYPE_GREEN_BLOOD, 6, 6);		
			}
			else
			{
				pEffect = CreateAttachEffect( EFFECTSPRITETYPE_BLOOD, 6, 6);		
			}
			
			if (pEffect != NULL)
			{
				pEffect->SetDirection( m_Direction );
				pEffect->SetAttachCreatureID( m_ID );
			}						
			
			//------------------------------------------------			
			// 官蹿俊 汝府绰 乔 3规匡(-_-;)
			//------------------------------------------------			
			if (g_pUserOption->BloodDrop)
			{
				int num = rand()%3 + 3;
				for (int i=0; i<num; i++)
				{
					ExecuteActionInfoFromMainNode(
						BLOOD_CRITICAL_HIT,										// 荤侩 扁贱 锅龋
						
						oldX, oldY, 0,
						(int)m_Direction,														// 荤侩 规氢
						
						m_ID,												// 格钎俊 措茄 沥焊
						oldX, oldY, 0,
						
						50 + rand()%30, //5*16, 
						
						NULL,
						
						false);	
				}
			}
		}
		SetServerPosition( x,y);

		return true;
	}

	// 绢露电 辑滚谅钎绰 八刘灯促绊 夯促.
	//m_ServerX = x;
	//m_ServerY = y;

	return false;
}

//----------------------------------------------------------------------
// Stop Fast Move
//----------------------------------------------------------------------
void				
MCreature::StopFastMove()
{
	m_sX = 0;
	m_sY = 0;
	m_bFastMove = false;

	// 2001.8.10俊 眠啊
	// 睦..辟单 捞 窃荐 静捞瘤档 臼绰焙.. - -;;
	m_MoveCount = m_MoveCountMax;
}

//----------------------------------------------------------------------
// Find Enemy
//----------------------------------------------------------------------
int
MCreature::FindEnemy()
{
	// 瘤陛篮 MPlayer俊辑父 荤侩茄促.
	// 固贰甫 困秦辑...
	return OBJECTID_NULL;
}

//----------------------------------------------------------------------
// Set ShadowCount
//----------------------------------------------------------------------
void				
MCreature::SetShadowCount(int n)
{ 
	// 5俺鳖瘤.. - -;
	m_ShadowCount = min(5, n); 
}

//----------------------------------------------------------------------
// Stop Blood Drain
//----------------------------------------------------------------------
// m_bStopBloodDrain捞搁 捞吧 龋免秦具 茄促.
//----------------------------------------------------------------------
void	
MCreature::StopBloodDrain()
{
	if (m_Action==ACTION_VAMPIRE_DRAIN
		&& IsVampire()
		&& GetActionCountMax()>60)
	{
		if( m_ActionCount>9 && m_ActionCount<70 && GetCreatureType() == CREATURETYPE_WER_WOLF )
		{
			m_ActionCount = 70;
			UnSetStopBloodDrain();
		} else
		if (m_ActionCount>9 && m_ActionCount<60)
		{
			m_ActionCount = 60;

			UnSetStopBloodDrain();
		}	
	}
	else
	{
		//UnSetStopBloodDrain();
	}
}

//----------------------------------------------------------------------
// Stop Absorb Soul
//----------------------------------------------------------------------
// m_bStopAbsorbSoul捞搁 捞吧 龋免秦具 茄促.
//----------------------------------------------------------------------
void	
MCreature::StopAbsorbSoul()
{
	if (m_Action==ACTION_OUSTERS_DRAIN
		&& IsOusters()
		&& GetActionCountMax()>30)
	{
		if (m_ActionCount>9 && m_ActionCount<30)
		{
			m_ActionCount = 30;

			ATTACHEFFECT_LIST::iterator itr = m_listEffect.begin();
			ATTACHEFFECT_LIST::iterator endItr = m_listEffect.end();

			while(itr != endItr)
			{
				MAttachEffect*	pEffect = *itr;
				
				if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_ABSORB_SOUL)
				{
					bool bUseEffectSprite = pEffect->IsEffectSprite();
					
					// flag力芭
					if (bUseEffectSprite)
					{
						m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag力芭
					}

					delete pEffect;
					m_listEffect.erase(itr);
					break;
				}
				itr++;
			}
			
			UnSetStopAbsorbSoul();
		}	
	}
	else
	{
		//UnSetStopBloodDrain();
	}
}

//----------------------------------------------------------------------
// Update Darkness
//----------------------------------------------------------------------
// 窜鉴洒 count父 官层霖促.
//----------------------------------------------------------------------
void	
MCreature::UpdateDarkness()
{
	if (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		if (m_DarknessCountInc==0)
		{
			if (m_ActionCount>=GetActionCountMax()-1)
			{
				CheckInDarkness();
			}
		}
		else
		{
			m_DarknessCount += m_DarknessCountInc;

			// darkness俊 甸绢啊绰 版快
			if (m_DarknessCount < 0)
			{
				m_DarknessCount = 0;
				m_DarknessCountInc = 0;
			}
			// darkness俊辑 狐廉唱坷绰 版快
			else if (m_DarknessCount >= MAX_DARKNESS_COUNT)
			{
				m_DarknessCount = -1;
				m_DarknessCountInc = 0;
			}
		}
	}
	else
	{
		// Slayer啊 酒囱 版快绰 Darkness尔 包拌绝促.
		// 犬牢侩
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;
	}
}

//----------------------------------------------------------------------
// Check In Darkness
//----------------------------------------------------------------------
// darkness啊 唱鸥唱绊/荤扼瘤绰 鉴埃
//            player啊 框流捞绰 鉴埃阑 眉农窍搁 等促.
//
// player啊 酒聪骨肺  矫具 包拌绝寸.
//----------------------------------------------------------------------
void
MCreature::CheckInDarkness()
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Darkness俊 甸绢吭绰瘤 眉农
			//-------------------------------------------------------
			// 辑滚 谅钎焊促绰 泅犁 谅钎啊 焊扁俊 亮促.. せせ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && (sector.HasDarkness()))
				{
					// darkness俊 乐绰 版快
					//PlaceInDarkness();
					m_DarknessCount = MAX_DARKNESS_COUNT;
					m_DarknessCountInc = 0;
				}
				else
				{		
					// darkness绰 荤扼脸促.
					//PlaceNotInDarkness();
					m_DarknessCount = -1;
					m_DarknessCountInc = 0;
				}
			}
		}
	}
}

BOOL
MCreature::IsInGroundElemental() const
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Darkness俊 甸绢吭绰瘤 眉农
			//-------------------------------------------------------
			// 辑滚 谅钎焊促绰 泅犁 谅钎啊 焊扁俊 亮促.. せせ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && sector.HasGroundElemental())
				{
					return true;
				}
			}
		}
	}
	return false;
}

BOOL
MCreature::IsInFuryOfGnome() const
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Fury Of Gnome俊 甸绢吭绰瘤 眉农
			//-------------------------------------------------------
			// 辑滚 谅钎焊促绰 泅犁 谅钎啊 焊扁俊 亮促.. せせ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && sector.HasFuryOfGnome())
				{
					return true;
				}
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
// Place In Darkness
//----------------------------------------------------------------------
// player绰 darkness俊 乐促.
//----------------------------------------------------------------------
void
MCreature::PlaceInDarkness(bool IsBlindness)
{
	if (
		  (
				(IsBlindness || (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001))
				
				&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
			)
		  
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		// darkness俊 乐瘤 臼带 版快
		if (m_DarknessCount < 0)
		{
			m_DarknessCount = MAX_DARKNESS_COUNT;		
			m_DarknessCountInc = -1;
		}	
		else if (m_DarknessCount > 0)
		{
			m_DarknessCountInc = -1;
		}
		else if (m_DarknessCount==0)
		{
			m_DarknessCountInc = 0;
		}
	}
}

//----------------------------------------------------------------------
// Move In Darkness
//----------------------------------------------------------------------
// player绰 darkness俊 乐瘤 臼促.
//----------------------------------------------------------------------
void
MCreature::PlaceNotInDarkness()
{
	if (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		// darkness俊 乐带 版快
		if (m_DarknessCount >= 0 && m_DarknessCount < MAX_DARKNESS_COUNT)
		{
			m_DarknessCountInc = 1;
		}	
	}
}

//----------------------------------------------------------------------
// Show In Darkness
//----------------------------------------------------------------------
// darkness俊 乐瘤 臼酒辑 肋 焊捞芭唱
// darkness俊 乐歹扼档 芭府俊 狼秦辑 焊捞绰啊?
//----------------------------------------------------------------------
BOOL	
MCreature::ShowInDarkness(int sX, int sY) const
{

//	DEBUG_ADD_FORMAT("[ShowInDarkness] g_bLight %d", g_bLight);
	// 捞固 darkness俊 乐绰 版快
	if (//!(m_CreatureType >= 526 && m_CreatureType <= 549 || m_CreatureType >= 371 && m_CreatureType <= 376 || m_CreatureType >= 560 && m_CreatureType <= 563) &&
//		!IsNPC() &&
		m_DarknessCount >= 0 &&
		(!HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) || g_pZone->GetID() == 3001 )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
	)
	{
		// darknessCount救俊绰 杭 荐 乐促.
		if ((max(abs(m_X-sX), abs(m_Y-sY))) <= m_DarknessCount)
		{
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------
// Set HalluCreature
//----------------------------------------------------------------------
// hallu牢 版快... 绢恫 各栏肺 焊咯瘤霸 等促.
//----------------------------------------------------------------------
void				
MCreature::SetHalluCreature(TYPE_CREATURETYPE type)
{
	// NPC绰 Hallu吧妨档 力措肺 焊牢促.
	if (IsNPC())
	{
		m_bHallu = false;
		return;
	}

	m_HalluCreatureType = type;
	
	m_HalluCreatureFrameID	= (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[type].SpriteTypes[0]].FrameID;
	
	// 祸彬 - 各牢 版快父.. 汲沥秦霖促.
	m_HalluColorBody	= (*g_pCreatureTable)[m_CreatureType].ColorSet;

//	m_HalluName = rand()%g_pMonsterNameTable->GetLastNameSize();

	m_bHallu = true;
}

//----------------------------------------------------------------------
// Determine HalluAction
//----------------------------------------------------------------------
void			
MCreature::DetermineHalluActionFrame()
{
	//------------------------------------------------------
	// 利例茄 action阑 急琶茄促.
	//------------------------------------------------------
	if (m_Action <= ACTION_DIE)
	{
		m_HalluAction = m_Action;
	}
	else if (m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		m_HalluAction = ACTION_STAND;
	}
	else if (m_Action==ACTION_SLAYER_MOTOR_MOVE)
	{
		m_HalluAction = ACTION_MOVE;
	}
	else
	{
		m_HalluAction = ACTION_ATTACK;
	}

	BYTE frame = GetFrame();

	//------------------------------------------------------
	// 悼累狼 frame 荐甫 搬沥茄促.
	//------------------------------------------------------
	int countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );

	if (countMax==0)
	{
		// 悼累捞 绝促搁..
		if (m_HalluAction!=ACTION_ATTACK)
		{
			m_HalluAction = ACTION_ATTACK;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );
		}

		// 悼累捞 绝促搁 沥瘤..
		if (countMax==0)
		{
			m_HalluAction = ACTION_STAND;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );						
		}
	}
	
	if (countMax==0)	// 厚背 厚背.. - -;
	{
		m_HalluFrame = 0;
	}
	else
	{
		m_HalluFrame = frame % countMax;
	}
}

//----------------------------------------------------------------------
// Get HalluName
//----------------------------------------------------------------------
const char*			
MCreature::GetHalluName() const
{ 
	if( strncmp( GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0 )
//		strstr(GetName(), "款康磊") != NULL)
		return GetName();
	return g_pMonsterNameTable->GetLastName(m_HalluName);
}

//----------------------------------------------------------------------
// Set Regen
//----------------------------------------------------------------------
void				
MCreature::SetRegen(int amount, DWORD delay)
{ 
	if(this == g_pPlayer && IsVampire() && g_pPlayer->GetCreatureType() != CREATURETYPE_BAT)
	{
		const int dex = g_pPlayer->GetBASIC_DEX();
		if(dex < 51)
			;
		else if(dex > 50 && dex < 121)
			amount += 1;
		else if(dex > 120 && dex < 191)
			amount += 2;
		else if(dex > 190 && dex < 261)
			amount += 3;
		else if(dex > 260 && dex < 331)
			amount += 4;
		else if(dex > 330 && dex < 391)
			amount +=5;
		else if(dex > 390 && dex < 451)
			amount +=6;
		else if(dex > 450)
			amount +=7;

		if((*g_pRankBonusTable)[RANK_BONUS_URANUS_BLESS].GetStatus() == RankBonusInfo::STATUS_LEARNED)
			amount += (*g_pRankBonusTable)[RANK_BONUS_URANUS_BLESS].GetPoint();

		if( HasEffectStatus( EFFECTSTATUS_WILL_OF_LIFE ) )
			amount += (5+ ( GetLEVEL() / 7 ) );		
	}
	m_RegenAmount = amount; 
	
	m_RegenDelayTime = delay; 
	m_RegenNextTime = g_CurrentTime; 
}

//----------------------------------------------------------------------
// Set Regen Bonus
//----------------------------------------------------------------------
void				
MCreature::SetRegenBonus(int amount, DWORD delay)
{ 
	m_RegenBonusAmount = amount; 
	m_RegenBonusDelayTime = delay; 
	m_RegenBonusNextTime = g_CurrentTime; 
}

//----------------------------------------------------------------------
// CheckRegen - Regen汲沥
//----------------------------------------------------------------------
void
MCreature::CheckRegen()
{
	//-------------------------------------------------------
	// 轨颇捞绢啊 酒聪搁 Regen窍瘤 臼绰促.
	//-------------------------------------------------------
	if (!IsVampire())
	{
		SetRegen( 0, 0 );
		return;
	}

	//-------------------------------------------------------
	// Burrow惑怕
	//-------------------------------------------------------
	if (IsUndergroundCreature())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_BURROW, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Casket 惑怕
	//-------------------------------------------------------
	if (IsInCasket())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_CASKET, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Creature Type俊 蝶扼辑..
	//-------------------------------------------------------
	switch (m_CreatureType)
	{
		//---------------------------------------------------------------
		// 轨颇捞绢, 戳措
		//---------------------------------------------------------------
		case CREATURETYPE_VAMPIRE_MALE1 :
		case CREATURETYPE_VAMPIRE_FEMALE1 :
		case CREATURETYPE_VAMPIRE_MALE2 :
		case CREATURETYPE_VAMPIRE_FEMALE2 :
		case CREATURETYPE_VAMPIRE_MALE3 :
		case CREATURETYPE_VAMPIRE_FEMALE3 :
		case CREATURETYPE_VAMPIRE_OPERATOR :
		case CREATURETYPE_WOLF :
		case CREATURETYPE_WER_WOLF :
		// add by coffee 2006.11.24  
		case CREATURETYPE_VAMPIRE_FEMALE4:
		case CREATURETYPE_VAMPIRE_MALE4:
		//add by viva
		case CREATURETYPE_VAMPIRE_MALE5:
		case CREATURETYPE_VAMPIRE_FEMALE5:
		case CREATURETYPE_VAMPIRE_MALE6:
		case CREATURETYPE_VAMPIRE_FEMALE6:
		// add end
			SetRegen( g_pClientConfig->REGEN_AMOUNT_VAMPIRE, 1000 );	// 2究 1檬付促			
		break;

		//---------------------------------------------------------------
		// 谍 版快..
		//---------------------------------------------------------------
		default :
			SetRegen( 0, 0 );	// regen 救茄促.		
	}
}

//----------------------------------------------------------------------
// 唱甫 软趋窍绊 乐带 Creature狼 软趋悼累阑 肛冕促
//----------------------------------------------------------------------
void 
MCreature::StopDrain()
{
	if(m_Action == ACTION_DRAINED)
	{
		MCreature *pCreature = g_pZone->GetCreature(GetDrainCreatureID());
		if(pCreature != NULL)
		{
			pCreature->StopBloodDrain();
		}
	}
}

//----------------------------------------------------------------------
// 唱甫 软康窍绊 乐带 Creature狼 软康悼累阑 肛冕促
//----------------------------------------------------------------------
void 
MCreature::StopAbsorb()
{
	if(m_Action == ACTION_DRAINED)
	{
		MCreature *pCreature = g_pZone->GetCreature(GetAbsorbCreatureID());
		if(pCreature != NULL)
		{
			pCreature->StopAbsorb();
		}
	}
}

void
MCreature::AddHPModify(const int modify)
{
	m_HPModifyList.push_back(HPModify(modify, GetTickCount()));
	if(m_HPModifyList.size() > g_pClientConfig->HPModifyListMax)m_HPModifyList.pop_front(); 
}

void 
MCreature::SetTraceID(TYPE_OBJECTID id)
{
	m_TraceID = id;	
}

TYPE_ACTIONINFO		MCreature::GetBasicActionInfo() const
{ 	
	return m_nBasicActionInfo; 
}

int MCreature::GetHeadSkin() const

{
	return m_HeadSkin;
}


bool MCreature::UpdateBurningSol()
{
	if(m_bBurningSol)
	{
		if(IsDead())
		{
			StopBurningSol();
			SetStop();
			return false ;
		}
		else if(HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS))
		{
			StopBurningSol();
			SetStop();
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4);
			return false;
		}
		int bCheckFrame = 0;
		if(!IsAdvancementClass())
			bCheckFrame = IsMale()?7:11;
		else
			bCheckFrame = 7;

		m_ActionCount = m_bBurningSolCount;

		m_bBurningSolCount++;
		if(	HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4))
		{
			if(m_bBurningSolCount>=bCheckFrame)
				m_bBurningSolCount = bCheckFrame;
		}
		if(m_ActionCount>=GetActionCountMax())
			StopBurningSol();
		return true;
	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateBurningSol] ActionCount : %d", m_bBurningSolCount);
	#endif
	}
	return false;
}
void MCreature::InstallTurretStopAttack()
{
	m_nUsedActionInfo = ACTIONINFO_NULL;
	if(m_Action == ACTION_ATTACK)
	{
		if(m_ActionCount<14)
			m_ActionCount = 54 - m_ActionCount;
		else if(m_ActionCount<20)
			m_ActionCount = 20;
		m_ActionCountMax = 54;
		PlaySound( SOUND_SLAYER_TURRET_FIRE_END );
		
//		if(m_ActionCount<28)
//			m_ActionCount = 75 - m_ActionCount;
//		else if(m_ActionCount<42)
//			m_ActionCount = 42;
//		m_ActionCountMax = 75;
	}
}
bool MCreature::UpDateInstallTurret()
{
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
	{
		if(IsAlive() && (m_Action == ACTION_STAND))
		{
			if (m_bInstallTurretDirect != m_Direction)
			{
				if( ((m_bInstallTurretDirect - m_Direction + 8)%8) < 4 )
				{
					m_bInstallTurretCount = ((m_bInstallTurretCount-1)+16)%16;
				}
				else
				{
					m_bInstallTurretCount = (m_bInstallTurretCount+1)%16;
				}

				if(!(m_bInstallTurretCount&0x01))
					m_bInstallTurretDirect = ((m_bInstallTurretCount /2) +2 )%8;

			}
			else
			{
				if(!(g_CurrentFrame%100))
					SetDirection((m_bInstallTurretDirect + 2+ rand()%5)%8);
			}
			return true;
		}
		else if( m_Action == ACTION_ATTACK)
		{
			if(m_ActionCount<20)
				m_ActionCount = 20;
	
			m_ActionCountMax = 54;
		}

//		
		SetInstallTurretDirect(m_CurrentDirection);
		SetInstallTurretCount(((m_CurrentDirection+6)%8)*2);

	}
	return false;

}

BYTE		MCreature::GetActionCountMax() const
{ 
	//if( IsAdvancementClass() )
	//	return GetCreatureActionCountMax(( this, GetAction() );
	return m_ActionCountMax; 
}
// 2005, 1, 5, sobeit add start
void		MCreature::UpdateBikeCrash()
{
	if(HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
	{
		int direction = (GetDirection() + 1)%8;
		int TempDir = GetInstallTurretDirect();
		
		if(GetAction() == ACTION_SLAYER_MOTOR_STAND)
		{
			if(direction == ((TempDir+3)%8))
			{
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_BIKE_CRASH, GetX(), GetY(), 0, GetDirection(),	GetID(),	
					GetX(), GetY(), 0, 5000, NULL, false);	
			}
			SetDirection(direction);
			SetCurrentDirection(direction);
			if(direction == TempDir)
			{
				m_bInstallTurretCount ++;
				if(m_bInstallTurretCount == 3)
				{
					RemoveEffectStatus(EFFECTSTATUS_BIKE_CRASH);
					if(g_pPlayer->GetID() == GetID())
						g_pPlayer->SetDelay(0);
				}
			}
		}
//		else
//		{
//			if(direction < ((TempDir+3)%8))
//			{
//				SetDirection(direction);
//				SetCurrentDirection(direction);
//			}
//		}
	}
	
}
// 2005, 1, 5, sobeit add end	

void 
MCreature::RemoveCauseCriticalWoundsEffect()
{
	SetStop();
	StopCauseCriticalWounds();
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	bool re = false;
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	
			EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 皋葛府 力芭
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list俊辑 力芭
			m_listEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	iEffect = m_listGroundEffect.begin();

	while (iEffect != m_listGroundEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	
			EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 皋葛府 力芭
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list俊辑 力芭
			m_listGroundEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	m_bEffectStatus[EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS]  = false;

}

// 2005, 1, 27, sobeit add start- max action count;
int
MCreature::GetActionMax() const
{
	if(!IsAdvancementClass())
		 return (*g_pCreatureTable)[m_CreatureType].GetActionMax(); 

	// 铰流 某腐磐 老锭
	if(IsSlayer())
		return ACTION_ADVANCEMENT_SLAYER_MAX/* - ACTION_ADVANCEMENT_SLAYER_STOP_SWORD*/;
	else if(IsVampire())
		return ACTION_ADVANCEMENT_MAX/* - ACTION_ADVANCEMENT_STOP*/;
	else if(IsOusters())
		return ACTION_ADVANCEMENT_OUSTERS_MAX/* - ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP*/;

	return 0;
}

// 2005, 1, 27, sobeit add end

// 2005, 2, 22, sobeit add start
int 
MCreature::GetMasterEffectType(DWORD Status)
{
	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[Status].EffectSpriteType;
	int MarketType = GetMasterEffectType();
	
	//if(MarketType>3) return 0;
	
	switch(Status)
	{
		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_100_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_100_FEAR + MarketType - 1;
			}
			break;
			
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER);
				if(IsDead()) return 0; //磷篮逞茄刨 捞棋飘 公矫
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_130);
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_ADVANCE_NEW_4;
						break;
					}
					// 铰流 某腐磐绰 Market Master Effect 利侩 窍瘤 富扼绊 秦辑 林籍 贸府 窃..
					// 唱吝俊 肚 富 官操搁 林籍 钱绢林搁 凳.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit By Sonic 2006.10.28 去除二转后角色身上的光无效果

					//	type = EFFECTSPRITETYPE_ADVANCEMENT_MASTER_SLAYER;
// 					if(MarketType)
// 						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
					// end  By Sonic 2006.10.28
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
				}
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE);
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130);
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_ADVANCE_NEW_4;
						break;	
					}
					// 铰流 某腐磐绰 Market Master Effect 利侩 窍瘤 富扼绊 秦辑 林籍 贸府 窃..
					// 唱吝俊 肚 富 官操搁 林籍 钱绢林搁 凳.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_ADVANCE_FEAR + MarketType - 1;
//					else
						// edit by sonic 2006.10.28   去除二转后看不到身上火颜色效果
							//type =  EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK;
// 							if(MarketType)
// 								type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
						// end by sonic 2006.10.28
						
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
				}
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS);
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_130);
	
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_NEW_4;
						break;	
					}	
					// 铰流 某腐磐绰 Market Master Effect 利侩 窍瘤 富扼绊 秦辑 林籍 贸府 窃..
					// 唱吝俊 肚 富 官操搁 林籍 钱绢林搁 凳.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit by sonic 2006.10.28  修正二转后角色火颜色问题
						//type =  EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER;
// 					if(MarketType)
// 					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;

					// end by sonic 2006.10.28
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;
				}

			}
			break;
	}
	
	return type;
}

void 
MCreature::ChangeMasterEffectType(int MarketEffect)
{
	int status = 0;

	if(IsSlayer())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER))		status = EFFECTSTATUS_GRAND_MASTER_SLAYER;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_130))	status = EFFECTSTATUS_GRAND_MASTER_SLAYER_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_150))	status = EFFECTSTATUS_GRAND_MASTER_SLAYER_150;
	}
	else if(IsVampire())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE))		status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130))	status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150))	status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150;
	}
	else if(IsOusters())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS))		status = EFFECTSTATUS_GRAND_MASTER_OUSTERS;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_130))	status = EFFECTSTATUS_GRAND_MASTER_OUSTERS_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_150))	status = EFFECTSTATUS_GRAND_MASTER_OUSTERS_150;
	}

	if( 0 == status )
		return;
		
	RemoveEffectStatus((EFFECTSTATUS)status);

	SetMasterEffectType(MarketEffect);

	AddEffectStatus((EFFECTSTATUS)status, 0xffff);
	
		
//	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
//
//	bool re = false;
//	while (iEffect != m_listEffect.end())
//	{
//		MAttachEffect* pEffect = *iEffect;
//		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
//		if(	
//			Type >= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_VAMPIRE_100_NEW && Type <= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_OUSTERS_150_NEW
//			|| Type >= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK && Type <= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER
//			|| Type >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR && Type <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_HOPE
//			)
//		{
//			// 皋葛府 力芭
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list俊辑 力芭
//			m_listEffect.erase( dEffect );
//
//			m_bAttachEffect[Type] = false;
//	
//			re = true;
//		}
//		
//		iEffect++;
//
//	}
//	iEffect = m_listGroundEffect.begin();
//
//	while (iEffect != m_listGroundEffect.end())
//	{
//		MAttachEffect* pEffect = *iEffect;
//		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
//		if(	
//			Type >= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_VAMPIRE_100_NEW && Type <= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_OUSTERS_150_NEW
//			|| Type >= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK && Type <= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER
//			|| Type >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR && Type <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_HOPE
//			)
//		{
//			// 皋葛府 力芭
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list俊辑 力芭
//			m_listGroundEffect.erase( dEffect );
//
//			m_bAttachEffect[Type] = false;
//	
//			re = true;
//		}
//		
//		iEffect++;
//
//	}
//	m_bEffectStatus[Status]  = false;

}
// 2005, 2, 22, sobeit add end