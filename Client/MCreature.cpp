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
#include "MSkillManager.h"	// [»õ±â¼ú3]
#include "UIFunction.h"
#include "MEventManager.h"
#include "PacketFunction2.h"
#include "RankBonusTable.h"
#include "RankBonusDef.h"
#include "MTestDef.h"
#include "MTimeItemManager.h"
#ifdef PLATFORM_WINDOWS
#include "MinTr.h"
#endif
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
	bool g_bCheckError = false;	// Å×½ºÆ®¸¦ À§ÇØ¼­ ÀÓ½Ã·Î..
#endif

//#define	new			DEBUG_NEW
#define				MAX_DARKNESS_COUNT		13

#ifdef __METROTECH_TEST__
	extern bool g_bLight;
#endif

//----------------------------------------------------------------------
// Get ArmageddonSprite
//----------------------------------------------------------------------
// ÀÌ°Åµµ member functionÀ¸·Î ³Ö¾î¾ß µÇ´Âµ¥.. ÄÄÆÄÀÏ ½Ã°£¶§¹®¿¡ ÀÏ´Ü -_-;
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
// °Á ÇÔ¼ö·Î ¸¸µé¾î¾ß µÇ´Âµ¥.. Çì´õ¹Ù²Ù°í ÄÄÆÄÀÏÇÏ±â ½È¾î¼­ ÀÓ½Ã·Î - -;
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
// ÇÑ¹ø¿¡ ÀÌµ¿ÇÏ´Â pixel¿¡ ´ëÇÑ Á¤º¸ 
// ACTIONCOUNT_MOVE¸¸Å­ sXTableÀÇ ¹æÇâ¿¡ ´õÇØÁÖ¸é sXTable°ªÀÌ 0ÀÌ µÈ´Ù.
// 8¹æÇâ¿¡ ¹æÇâ¸¶´Ù Move¼ö¸¸Å­..
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
// Slayer action ¼Óµµ
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
// vampire action ¼Óµµ
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
// ousters action ¼Óµµ
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
// ÇÑ Tile ÀÌµ¿ÀÇ Pixel¼ö
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
// ¹æÇâ¿¡ µû¸¥ º¯È­°ª
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
// cX,cYtableÀ» ÃÊ±âÈ­ÇÑ´Ù.
// [maxFrameÁ¾·ù8][8¹æÇâ][maxFrame°³ ¸¸Å­ÀÇ frameµé]
//----------------------------------------------------------------------
void
MCreature::InitMoveTable()
{
	for (int maxFrame=0; maxFrame<MAX_FRAME_MOVE; maxFrame++)
	{
		int maxFrame_plus_1	= maxFrame + 1;

		for (int d=0; d<MAX_DIRECTION; d++)
		{			
			// maxFrame°³ ¸¸Å­ÀÇ frameµé
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
			
												// maxFrameÀÌ 8ÀÏ °æ¿ì..
			int totalX	= -m_sXTable[d];				// 48
			int totalY	= -m_sYTable[d];				// 24

			int		pX	= 0, pY = 0;			// ¹Ù·ÎÀüÀÇ x,y°ª
			float	fX	= 0, fY = 0;			// ÇöÀç±îÁö ´õÇÑ x,y°ª
			float	cX	= (float)totalX / maxFrame_plus_1;	// 6
			float	cY	= (float)totalY / maxFrame_plus_1;	// 3

			// °¢ FrameÀ» ÃÊ±âÈ­ ½ÃÅ²´Ù.
			// maxFrame°³¸¦ ´õÇÏ¸é totalX, totalY°¡ µÇ¾î¾ß ÇÑ´Ù.			
			for (int f=0; f<maxFrame; f++)
			{
				// ÇöÀç±îÁö ´õÇÑ x, y°ª
				fX += cX;
				fY += cY;

				// ÇöÀç °ª¿¡¼­ ÀÌÀü °ªÀ» »©¼­ Â÷ÀÌ¸¦ ±¸ÇÑ´Ù.
				// fX¸¦ ¹Ý¿Ã¸² ÇÏ´Â°Íµµ ±¦ÂúÁö ½Í´Ù.
				m_cXTable[maxFrame][d][f] = fX - pX;
				m_cYTable[maxFrame][d][f] = fY - pY;

				// ÇöÀçÀÇ °ªÀ» ±â¾ïÇØµÐ´Ù.
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
// cX,cYTableÀ» Á¦°ÅÇÑ´Ù.	
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

	// ¿òÁ÷ÀÌ´Â ¹æ¹ý°ú ±×¿¡ µû¸¥ ÀÌµ¿ ´ÜÀ§
	m_MoveDevice = MOVE_DEVICE_WALK;
	m_MoveAction = ACTION_MOVE;

	//m_ppCXTable = m_cXTableDeviceNull;
	//m_ppCYTable = m_cYTableDeviceNull;

	m_ColorBody1 = 0;
	m_ColorBody2 = 0;
	m_ChangeColorSet = ATTACHEFFECTCOLOR_NULL;	// ±âº»ÀûÀ¸·Î »ç¿ëÇÏÁö ¾Ê´Â´Ù.

	m_ID = OBJECTID_NULL;

	m_pZone = NULL;

	m_X = SECTORPOSITION_NULL;
	m_Y = SECTORPOSITION_NULL;
	
	m_sX = 0;	
	m_sY = 0;

	m_cX = 0;
	m_cY = 0;

	m_Action = ACTION_STAND;				// Action¿¡ ´ëÇÑ ID
	m_Direction = DIRECTION_LEFTDOWN;			// ¹Ù¶óºÁ¾ßÇÒ ¹æÇâ
	m_DirectionMove = m_Direction;
	m_DirectionMoved = m_Direction;
	m_CurrentDirection = DIRECTION_LEFTDOWN;	// ÇöÀç¹Ù¶óº¸´Â ¹æÇâ

	//  Çàµ¿	
	m_NextAction		= ACTION_STAND;


	m_ActionCount		= 0;				// Current Frame
	m_ActionCountMax	= 0;

	m_MoveCount			= 0;
	m_MoveCountMax		= 0;
	m_NextMoveCount		= 0;

	// ³ôÀÌ
	m_Z		= 0;
	
	//-------------------------------------------------------
	// ´ÙÀ½ ¿òÁ÷ÀÏ ¹æÇâ	
	//-------------------------------------------------------
	m_NextX			= SECTORPOSITION_NULL;
	m_NextY			= SECTORPOSITION_NULL;
	m_NextDirection	= DIRECTION_NULL;

	//-------------------------------------------------------
	// ¸¶Áö¸·À¸·Î °ËÁõµÈ SectorÀÇ ÁÂÇ¥
	//-------------------------------------------------------
	m_ServerX	= SECTORPOSITION_NULL;
	m_ServerY	= SECTORPOSITION_NULL;


	//-------------------------------------------------------
	// Chatting String
	//-------------------------------------------------------
	int i;
	m_ChatString = new char* [g_pClientConfig->MAX_CHATSTRING];
	for (i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
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
	// ±â¼ú Á¾·ù
	//-------------------------------------------------------
	m_nBasicActionInfo		= SKILL_ATTACK_MELEE;
	m_nSpecialActionInfo	= ACTIONINFO_NULL;
	m_nUsedActionInfo		= ACTIONINFO_NULL;

	//-------------------------------------------------------
	// Ä³¸¯ÅÍ¿¡ ºÙ¾î ÀÖ´Â EffectÁ¾·ù
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

//	m_MaxEffectLight = 0;	// ÃÖ°í ¹à±â

	//-------------------------------------------------------
	// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
	//-------------------------------------------------------
	SetTraceID( OBJECTID_NULL );
	m_TraceX				= SECTORPOSITION_NULL;
	m_TraceY				= SECTORPOSITION_NULL;
	m_TraceZ				= 0;

	// ºû³ª´Â Effectµé
	//m_nAlphaEffect = 0;

	// ÃÖ±Ù¿¡ Ã¤ÆÃ StringÀÌ Ãß°¡µÈ ½ÃÁ¡
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

	// -1ÀÌ¸é darkness¿¡ ÀÖ´Â°Ô ¾Æ´Ï´Ù.
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
	// Æê Á¦°Å
	//----------------------------------------
	if(GetPetID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetPetID());
	}

	//----------------------------------------
	// Á¤·É Á¦°Å
	//----------------------------------------
	if(GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}

	ClearAttachEffect();

	//-------------------------------------------------------
	// ÀÌ¸§ Á¦°Å
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
				// add by Coffee 2006.11.24  Ôö¼Ó¹í×åÑÇÂéÐÎÏó
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
	
	// ´ÙÀ½ µ¿ÀÛµµ ¾ø¾Ú
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


	
	// »ö±ò - ¸÷ÀÎ °æ¿ì¸¸.. ¼³Á¤ÇØÁØ´Ù.
	if(!IsCreatureTypeVampirePC(m_CreatureType))
	{
		m_ColorBody1		= (*g_pCreatureTable)[m_CreatureType].ColorSet;
		m_ColorBody2		= m_ColorBody1;
	}
	
	// ¾îµÓ°Ô Âï±â Á¦°Å
	m_bFade				= false;

	// ¿ø·¡ Å©¸®ÃÄ Å¸ÀÔÀÌ ¹Ù²ð¶§ ÀÌÆåÆ®¸¦ ³¯¸®µµ·Ï Çß´Ù.
	// ÀÌ´Â ½½·¹ÀÌ¾î<->¹ìÆÄÀÌ¾îÀÇ º¯½Å½Ã¸¸ ÇÏµµ·Ï ÇÏÀð
	// ºÙÀº effect Á¦°Å
//	ClearAttachEffect();

	//by csm
	spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0];
	//---------------------------------------------------------------------------
	// ±â¼ú ¼³Á¤
	//---------------------------------------------------------------------------
	// ±âº» °ø°Ý µ¿ÀÛÀÌ ¾ø´Â °æ¿ì..
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
	// ¿ÏÀüÈ÷ »ö±òÀÌ ¹Ù²î´Â ¸÷ÀÎ °æ¿ì
	//---------------------------------------------------------------------------
	int changeColor = (*g_pCreatureTable)[m_CreatureType].ChangeColorSet;
	if (changeColor < MAX_COLORSET)
	{
		m_ChangeColorSet = changeColor;
		m_AttachEffectColor = changeColor;
	}
	else
	{
		// ATTACHEFFECTCOLOR_NULLÀÌ¸é Á¤»óÀûÀÎ »ö±òÀÌ´Ù.

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
	// ±âº»(?)ÀûÀ¸·Î ¼ºº° ¼³Á¤..
	// ´Á´ë³ª ¹ÚÁãÀÎ °æ¿ì´Â creaturetypeÀ¸·Î ¼ºº°À» ¾Ë ¼ö°¡ ¾ø¾î¼­¸®..
	//---------------------------------------------------------------------------
	if( m_CreatureType != CREATURETYPE_WER_WOLF )
		m_bMale = (*g_pCreatureTable)[m_CreatureType].bMale;

	//---------------------------------------------------------------------------
	// ¸ö ÀÜ»ó
	//---------------------------------------------------------------------------
	m_ShadowCount = (*g_pCreatureTable)[m_CreatureType].ShadowCount;

	m_bInCasket = false;
	m_CasketCount = 0;

	//---------------------------------------------------------------------------
	// Effect Á¦°Å
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

	// 2004, 9, 13, sobeit add start - ÃÑ½½ 130 ½ºÅ³
	if (HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
		&& m_CreatureType != CREATURETYPE_INSTALL_TURRET)
	{
		RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
	}
	// 2004, 9, 13, sobeit add end - ÃÑ½½ 130 ½ºÅ³
	
	// Weapon Speed¸¦ ´Ù½Ã ¼³Á¤ÇØÁØ´Ù.
	int attackSpeed = m_Status[MODIFY_ATTACK_SPEED];

	// 0ÀÌ¸é °ªÀÌ ¼³Á¤ÀÌ ¾ÈµÆ´Ù°í °¡Á¤ÇÑ´Ù. Æ¯È÷, ¸÷ÀÎ °æ¿ìÀÌ´Ù.
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
	// Creature¿¡ ºÙ¾î ÀÖ´Â Effect¸¦ ¸Þ¸ð¸®¿¡¼­ Á¦°ÅÇÑ´Ù.
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
	// ¸ö¿¡ ºÙ´Â Effect
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
	// Ã¼Å© °ª Á¦°Å
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
			// ¸Þ¸ð¸® Á¦°Å
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list¿¡¼­ Á¦°Å
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
// Æ¯Á¤ÇÑ EffectSpriteTypeÀ» Á¦°ÅÇÑ´Ù.
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
		// [»õ±â¼ú7]
		DEBUG_ADD_FORMAT("NotEffectStatus(%d)", (int)status);
		return false;
	}

	m_bEffectStatus[status] = false;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;

	int pTypes = (*g_pEffectStatusTable)[status].EffectSpriteType;
	//------------------------------------------------------------
	// effectStatus¿¡ µû¶ó¼­.
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
		// ¸¶ºñ Ç®¸± ¶§
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
		// ¸¶ºñ Ç®¸± ¶§
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
		// EFFECTSTATUS_SUMMON_CASKET [»õ±â¼ú]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// ¹Ù·Î invisible·Î ¸¸µç´Ù.
			RemoveCasket();
		break;


		//------------------------------------------------------------
		// Invisible Á¦°Å
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
		case EFFECTSTATUS_SNIPPING_MODE :	
			// invisibleÀ» Á¦°ÅÇÑ´Ù.
			SetVisible();
		break;

		//------------------------------------------------------------
		// COMA Á¦°Å
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetAlive();
			if(IsVampire())
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_RESURRECT,										// »ç¿ë ±â¼ú ¹øÈ£
						
							m_X, m_Y, 0,
							(int)m_Direction,														// »ç¿ë ¹æÇâ
							
							m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
							m_X, m_Y, 0,
							
							2*16, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
		break;

		case EFFECTSTATUS_GHOST:
			if (!(*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// ¹ÚÁãÀÎ °æ¿ì
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
	//			EffectSprite·Î Ç¥ÇöÇÏ´Â °æ¿ì
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
			// Creature¿¡ ºÙ¾î ÀÖ´Â EffectÁß¿¡¼­
			// EffectSpriteTypeÀÌ typeÀÎ °ÍÀ» Á¦°ÅÇÑ´Ù.
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
					// ¸ö¿¡ ºÙÀº°Å
					//-------------------------------------------------------
					ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

					bool re = false;
					while (iEffect != m_listEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// °°Àº typeÀ» Ã£´Â´Ù.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() 
							&& (pEffect->GetEffectSpriteType() == type || type2 != EFFECTSPRITETYPE_NULL &&
							type2 == pEffect->GetEffectSpriteType()) )
						{
							// ¸Þ¸ð¸® Á¦°Å
							delete pEffect;

							ATTACHEFFECT_LIST::iterator dEffect = iEffect;
							iEffect--;

							// list¿¡¼­ Á¦°Å
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
					// ¹Ù´Ú¿¡ ºÙÀº °Å
					//-------------------------------------------------------
					iEffect = m_listGroundEffect.begin();

					while (iEffect != m_listGroundEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// °°Àº typeÀ» Ã£´Â´Ù.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type
							|| type2 != EFFECTSPRITETYPE_NULL && type2 == pEffect->GetEffectSpriteType() )
						{
							// ¸Þ¸ð¸® Á¦°Å
							delete pEffect;

							// list¿¡¼­ Á¦°Å
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
	//			EffectColor·Î Ç¥ÇöÇÏ´Â °æ¿ì
	//
	//------------------------------------------------------------
	else
	{
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		//-------------------------------------------------------
		// Creature¿¡ ºÙ¾î ÀÖ´Â EffectÁß¿¡¼­
		// EffectColor°¡ colorÀÎ °ÍÀ» Á¦°ÅÇÑ´Ù.
		//-------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		while (iEffect != m_listEffect.end())
		{
			MAttachEffect* pEffect = *iEffect;
			
			//-------------------------------------------------------
			// °°Àº typeÀ» Ã£´Â´Ù.
			//-------------------------------------------------------
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// ¸Þ¸ð¸® Á¦°Å
				delete pEffect;

				// list¿¡¼­ Á¦°Å
				m_listEffect.erase( iEffect );

				return true;
			}
			
			iEffect++;
		}		

		// GroundEffect´Â ¾ø´Ù°í º»´Ù.
	}
	
	return false;
}

//----------------------------------------------------------------------
// Add EffectStatus
//----------------------------------------------------------------------
// 
// Æ¯º°ÇÑ »óÅÂ¸¦ ³ªÅ¸³»´Â effect
//
// Creature¿¡ ´Þ¶óºÙ¾î¼­ Ç¥ÇöµÇ´Â Effect Ç¥Çö
//
// ÁßÃ¸À» Çã¿ëÇÏÁö ¾ÊÀ¸¹Ç·Î.. 
// ¾ÆÁ÷ ºÙ¾î¼­ Ç¥ÇöµÇÁö ¾ÊÀº Effect¸¸ »õ·Î Ãß°¡ÇÑ´Ù.
//
// ±×·¯³ª, ½Ã°£ È®ÀåÀ» ÇØ¾ßÇÏ´Â °æ¿ì°¡ »ý±ä´Ù.
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
	// ÀÌ¹Ì Á×Àº °æ¿ì
	//----------------------------------------------------------
	if (IsDead() && status != EFFECTSTATUS_COMA && 
		!(
		GetCreatureType() >= 371 && GetCreatureType() <= 376 || GetCreatureType() >= 560 && GetCreatureType() <= 563) && 
		GetCreatureType() != 482 && GetCreatureType() != 650 && !(GetCreatureType() >= 526 && GetCreatureType() <= 549) &&
		GetCreatureType() != 670 && GetCreatureType() != 672 && GetCreatureType() != 673
		)	// ¼º¹°ÀÎ °æ¿ì´Â ÀÌÆåÆ® ºÙÀÓ
	{
		DEBUG_ADD("Don't AddEffectStatus : Already Dead");
		
		return false;
	}

	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	int eCount = 0; // ÀÌÆåÆ®ÀÇ °³¼ö
	int orbit_type = 0;

	//------------------------------------------------------------
	// effectStatus¿¡ µû¶ó¼­.
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
			/********  Edit By sonic È¥³ýÄ§Áé¶þ×ªºó±äÇòÐ§¹û********/
			
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
			// ¹Ù·Î invisible·Î ¸¸µç´Ù.
			SetInvisibleSoon();
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [»õ±â¼ú]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// ¹Ù·Î invisible·Î ¸¸µç´Ù.
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
			// CauseCriticalWoundsÀÎ °æ¿ì ½½·¹ÀÌ¾î´Â AddEffect¸¦ ÇÏÁö ¾Ê´Â´Ù. ÀÌÆåÆ® ºÙÀÌÁö ¾Ê°í µ¥¹ÌÁö¸¸
			// 2005, 1, 18, sobeit modify start // g_pZone->GetID() == 4002 Ãß°¡ - Äù½ºÆ® cause_critical_wounds°É·ÁÀÖ´Â ½½·¹ÀÌ¾î npc
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
			// ´Ù ÀÌµ¿½ÃÅ²´Ù.
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
							SKILL_CLIENT_FIRE_ELEMENTAL_ATTACK,										// »ç¿ë ±â¼ú ¹øÈ£
						
							m_X, m_Y, 0,
							(int)m_Direction,														// »ç¿ë ¹æÇâ
							
							m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
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
							SKILL_CLIENT_WATER_ELEMENTAL_HEAL,										// »ç¿ë ±â¼ú ¹øÈ£
						
							m_X, m_Y, 0,
							(int)m_Direction,														// »ç¿ë ¹æÇâ
							
							m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
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
			delayFrame = 0xFFFF;		// ¹«ÇÑ..
		break;

		//------------------------------------------------------------
		// ¼º¹° °ü·Ã
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
			delayFrame = 0xFFFF;		// ¹«ÇÑ..
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

		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_ME :// 195 ³»°¡ °¡Áö°íÀÖ´ø ÇÇÀÇ¼º¼­°¡ ¿öÇÁµÉ¶§ ¸Ó¸®¿¡ Âï¾îÁÖ´Â-_- ÀÌÆåÆ®			
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_TILE :		// 196 ¶¥¿¡ ¶³¾îÁ®ÀÖ´ø ÇÇÀÇ¼º¼­°¡ ¿öÇÁµÉ¶§ ±× Å¸ÀÏ¿¡ Âï¾îÁÖ´Â ÀÌÆåÆ®
		case EFFECTSTATUS_SHRINE_GUARD_WARP :			// 187		¼öÈ£¼º´Ü¿¡¼­ ¿öÇÁµÉ¶§
		case EFFECTSTATUS_SHRINE_HOLY_WARP :			// 188		¼ºÁö¼º´Ü¿¡¼­ ¿öÇÁµÉ¶§
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

		// 2004, 6 ,12 sobeit add start - 130,150 ¸¶½ºÅÍ ÀÌÆåÆ®½Ã ±×Àü ¸¶½ºÅÍ ÀÌÆåÆ® Áö¿öÁØ´Ù.
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
		// 2004, 6 ,12 sobeit add end - 130,150 ¸¶½ºÅÍ ÀÌÆåÆ®½Ã ±×Àü ¸¶½ºÅÍ ÀÌÆåÆ® Áö¿öÁØ´Ù.
//		case EFFECTSTATUS_GDR_FLOATING:
//			//SetAction(ACTION_DRAINED);
//		//	m_MoveAction = ACTION_DRAINED;
//			if(GetCreatureType() == 717) // Áúµå·¹ ÀÏ¶§
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
		case EFFECTSTATUS_INSTALL_TURRET: // ÃÑ½½ 130 ÀÎ½ºÅç ÅÍ·¿
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
//				// ´Á´ë·Î º¯½ÅÇÏ´Â °á°ú
//				//--------------------------------------------------
//				MActionResult* pResult = new MActionResult;
//
//				pResult->Add( new MActionResultNodeChangeCreatureType( GetID(), CREATURETYPE_INSTALL_TURRET ) );

//					
//				ExecuteActionInfoFromMainNode(
//					RESULT_SKILL_INSTALL_TURRET,										// »ç¿ë ±â¼ú ¹øÈ£
//				
//					GetX(), GetY(), 0,
//					GetDirection(),														// »ç¿ë ¹æÇâ
//					
//					OBJECTID_NULL,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
//					GetX(), GetY(), 0, 
//					
//					0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
//					
//					NULL, //NULL,
//					
//					false);			// ±â¼ú Ã·ºÎÅÍ ½ÃÀÛÇÑ´Ù.
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
		// add by Coffee 2007-5-3 »ð·¨	
		case EFFECTSTATUS_DUMMY_DRAKE:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HYDRO_CONVERGENCE: //Ë®·¨
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_HYDRO_CONVERGENCE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HETER_CHAKRAM: //ÃôÕ½
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_SKILL_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
		case EFFECTSTATUS_BLOOD_CURSE: // ÎüÑª¹í¼¼ÄÜ
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
	//			EffectSprite·Î Ç¥ÇöÇÏ´Â °æ¿ì
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
		// ÀÌ¹Ì ÀÖ´Â °æ¿ì 
		//------------------------------------------------------------
		if (m_bAttachEffect[type])
		{		
			ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

			//------------------------------------------------------
			// ¸ö¿¡ ºÙ´Â°Å
			//------------------------------------------------------
			// °°Àº Effect¸¦ Ã£´Â´Ù. 
			while (iEffect != m_listEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// °°Àº typeÀ» Ã£´Â´Ù.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// Ãß°¡ÇÒ·Á´Â °ÍÀÌ ´õ ´Ê°Ô ³¡³¯ °æ¿ì¿¡¸¸ ½Ã°£À» È®ÀåÇÑ´Ù.
					if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
					{
						pEffect->SetCount( delayFrame );
					}

					return true;
				}

				iEffect++;
			}

			//------------------------------------------------------
			// ¹Ù´Ú¿¡ ºÙ´Â °Å
			//------------------------------------------------------
			iEffect = m_listGroundEffect.begin();

			// °°Àº Effect¸¦ Ã£´Â´Ù. 
			while (iEffect != m_listGroundEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// °°Àº typeÀ» Ã£´Â´Ù.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// Ãß°¡ÇÒ·Á´Â °ÍÀÌ ´õ ´Ê°Ô ³¡³¯ °æ¿ì¿¡¸¸ ½Ã°£À» È®ÀåÇÑ´Ù.
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
		// ¾ø´Â °æ¿ì Ãß°¡ÇÑ´Ù.
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
				
				// ºû³ª´Â EffectÀÇ °³¼ö¸¦ Áõ°¡?
				if (pEffect->GetBltType()==BLT_EFFECT)
				{
					// ÃÖÃÊ·Î Ãß°¡µÇ´Â ºû³ª´ÂEffectÀÌ¸é ½Ã¾ß¸¦ Ãß°¡½ÃÅ²´Ù.
					//if (m_nAlphaEffect==0)
					{
						pEffect->SetPosition( m_X, m_Y );
						//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
					}
					
					//m_nAlphaEffect ++;		
				}
				
				//------------------------------------------------------------
				// ¹Ù´Ú¿¡ ºÙ´Â °Å¶ó¸é..
				//------------------------------------------------------------
				if ((*g_pEffectStatusTable)[status].bAttachGround)
				{
					m_listGroundEffect.push_back( pEffect );
				}
				//------------------------------------------------------------
				// ¸ö¿¡ ºÙ´Â °Å..
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
			
			// ºû³ª´Â EffectÀÇ °³¼ö¸¦ Áõ°¡?
			if (pEffect->GetBltType()==BLT_EFFECT)
			{
				// ÃÖÃÊ·Î Ãß°¡µÇ´Â ºû³ª´ÂEffectÀÌ¸é ½Ã¾ß¸¦ Ãß°¡½ÃÅ²´Ù.
				//if (m_nAlphaEffect==0)
				{
					pEffect->SetPosition( m_X, m_Y );
					//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
				}
				
				//m_nAlphaEffect ++;		
			}
			
			//------------------------------------------------------------
			// ¹Ù´Ú¿¡ ºÙ´Â °Å¶ó¸é..
			//------------------------------------------------------------
			if ((*g_pEffectStatusTable)[status].bAttachGround)
			{
				m_listGroundEffect.push_back( pEffect );
			}
			//------------------------------------------------------------
			// ¸ö¿¡ ºÙ´Â °Å..
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
	//			EffectColor·Î Ç¥ÇöÇÏ´Â °æ¿ì
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
		// ÀÌ¹Ì ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.
		// »ö±ò·Î Ã¼Å©ÇØ¾ßµÇ¹Ç·Î.. list¸¦ ´Ù °Ë»öÇØ¾ß ÇÑ´Ù. T_T;
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("listEffect size = %d", m_listEffect.size());
		
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		//----------------------------------------------------------------
		// ¸ö¿¡ ºÙ´Â°Å
		//----------------------------------------------------------------
		// °°Àº Effect¸¦ Ã£´Â´Ù. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// °°Àº »ö±òÀ» Ã£´Â´Ù.
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// Ãß°¡ÇÒ·Á´Â °ÍÀÌ ´õ ´Ê°Ô ³¡³¯ °æ¿ì¿¡¸¸ ½Ã°£À» È®ÀåÇÑ´Ù.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				return true;
			}

			iEffect++;
		}

		//----------------------------------------------------------------
		// ¹Ù´Ú¿¡ ºÙ´Â°Å
		//----------------------------------------------------------------
		// ¾ø´Ù°í º»´Ù.

		DEBUG_ADD("[Create New AttachEffect]");
		
		//------------------------------------------------------------
		// ¾ø´Â °æ¿ì Ãß°¡ÇÑ´Ù.
		//------------------------------------------------------------
		MAttachEffect*	pEffect = new MAttachEffect(EFFECTSPRITETYPE_NULL, delayFrame);

		DEBUG_ADD("[Create New AttachEffect OK]");
		
		pEffect->SetAttachCreature( this );

		DEBUG_ADD("[Set AttachCreature]");
		
		// »ö±ò ¼³Á¤
		pEffect->SetEffectColor( colorSet );

		ADDON	part = (*g_pEffectStatusTable)[status].EffectColorPart;

		//------------------------------------------------------------
		// Æ¯Á¤ÇÑ ºÎÀ§¸¸ »ö±òÀÌ ¹Ù²î´Â °æ¿ìµµ ÀÖ´Ù.
		//------------------------------------------------------------
		//if (part<ADDON_MAX)
		//{
			pEffect->SetEffectColorPart( part );
		//}

		DEBUG_ADD("[if (pEffect->GetBltType()==BLT_EFFECT)]");
		
		// ºû³ª´Â EffectÀÇ °³¼ö¸¦ Áõ°¡?
		if (pEffect->GetBltType()==BLT_EFFECT)
		{
			// ÃÖÃÊ·Î Ãß°¡µÇ´Â ºû³ª´ÂEffectÀÌ¸é ½Ã¾ß¸¦ Ãß°¡½ÃÅ²´Ù.
			//if (m_nAlphaEffect==0)
			{
				pEffect->SetPosition( m_X, m_Y );
				//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
			}

			//m_nAlphaEffect ++;		
		}

		//------------------------------------------------------
		// »ö±ò º¯ÇÏ´Â°Å´Â ±×³É ¸ö¿¡ ºÙÀÌ´Â°Å¿¡ Ãß°¡ÇÑ´Ù.
		//------------------------------------------------------
		m_listEffect.push_back( pEffect );				
	}

	DEBUG_ADD("[AddEffectStatus - OK]");
	
	return true;
}

//----------------------------------------------------------------------
// Set Name - Ä³¸¯ÅÍ ÀÌ¸§ ¼³Á¤
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
	// ±âÁ¸ÀÇ MoveBuffer¸¦ ´Ù Áö¿ò
	//------------------------------------------------
	ReleaseMoveBuffer();

	SetServerPositionWithBlock(sX, sY);
}

//----------------------------------------------------------------------
// Is Slayer Character
//----------------------------------------------------------------------
// slayer ±×¸²À» »ç¿ëÇß³ª?
//----------------------------------------------------------------------
bool			
MCreature::IsSlayerCharacter() const
{
	return (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0] <= 1;
	//return m_CreatureType==CREATURETYPE_SLAYER_MALE
	//	||  m_CreatureType==CREATURETYPE_SLAYER_FEMALE;
}

//----------------------------------------------------------------------
// ¿òÁ÷ÀÌ´Â ¹æ¹ý°ú ±×¿¡ µû¸¥ ÀÌµ¿´ÜÀ§¸¦ °áÁ¤ÇÑ´Ù.
//----------------------------------------------------------------------
void		
MCreature::SetMoveDevice(MOVE_DEVICE md)
{	
	//--------------------------------------------------------
	// ¸ðµÎ Àû¿ë
	//--------------------------------------------------------
	AffectMoveBufferAll();

	//--------------------------------------------------
	// SlayerÀÎ °æ¿ì¸¸ º¯°æÀÌ °¡´ÉÇÏ´Ù.
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
	// ¾Æ´Ñ °æ¿ì..
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
// CreatureÀÇ À§Ä¡¸¦ SetÇÏ°í ¿òÁ÷ÀÌ´ø µ¿ÀÛÀ» ¸ØÃá´Ù.
//----------------------------------------------------------------------
void	
MCreature::SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)	
{ 
	//------------------------------------------------
	// ±âÁ¸ÀÇ MoveBuffer¸¦ ´Ù Áö¿ò
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
	// ¹ÚÁã³ª ´Á´ë¸é °ø°Ý¼Óµµ µû·Î ¾ø´ç..
	//-----------------------------------------------------------
	if (m_CreatureType==CREATURETYPE_BAT ||
		m_CreatureType==CREATURETYPE_WOLF ||
		m_CreatureType==CREATURETYPE_WER_WOLF)
	{
		m_WeaponSpeed = WEAPON_SPEED_NORMAL;
	}
	//-----------------------------------------------------------
	// °ø°Ý ¼Óµµ °è»ê..
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
// Creature¸¦ ¸ØÃß°Ô ÇÑ´Ù.
//----------------------------------------------------------------------
void
MCreature::SetStop()
{ 
	
	m_bFastMove = false;
	m_bKnockBack = 0;	

	//------------------------------------------------
	// ±âÁ¸ÀÇ MoveBuffer¸¦ ´Ù Áö¿ò
	//------------------------------------------------
	AffectMoveBufferAll();
	//ReleaseMoveBuffer();

	// 2001.11.8 - Á¤ÁöÇÒ¶§ ÁÂÇ¥ º¸Á¤ È®½ÇÈ÷..
	ActionMoveNextPosition();

	// Action ÁßÁö
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
	// 2004, 11, 3, sobeit modify start - m_ActionCount¸¦ ¹Ù²ï ¾×¼ÇÀÇ ¸Æ½º Ä«¿îÆ®·Î ¼¼ÆÃ ÇßÀ½ - ¾Æ¿ì½ºÅÍÁî °í½ºÆ® ¹ö±× ¶«½Ã..¤Ñ¤Ñ;
	m_ActionCount	= GetActionCountMax(); 
	//m_ActionCount	= (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	// 2004, 11, 3, sobeit modify end
	m_MoveCount		= m_MoveCountMax;

	// ´ÙÀ½ µ¿ÀÛµµ ¾ø¾Ú
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

	
	//m_nNextUsedActionInfo = ACTIONINFO_NULL;
}

//----------------------------------------------------------------------
// Get PixelX - CreatureÀÇ PixelXÁÂÇ¥
//----------------------------------------------------------------------
int			
MCreature::GetPixelX() const
{ 
	return MTopView::MapToPixelX(m_X) + m_sX;
}

//----------------------------------------------------------------------
// Get PixelY - CreatureÀÇ PixelYÁÂÇ¥
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
	// slayerÀÌ¸é ±â¼ú ·¹º§¿¡ µû¶ó¼­...

	// vampireÀÌ¸é ·¹º§¿¡ µû¶ó¼­?

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
// Zone¿¡¼­ (m_X, m_Y) Sector¿¡ ÀÖ´Â 
// this Creature¸¦ (x, y)·Î ¿Å°Ü¾ß ÇÑ´Ù.
//----------------------------------------------------------------------
bool
MCreature::MovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	if (m_pZone==NULL)
	{
		return false;
	}

	// Creature Type¿¡ µû¶ó¼­ ´Ù¸£´Ù.			
	switch (m_MoveType)
	{
		case CREATURE_UNDERGROUND : 
			if (m_pZone->MoveUndergroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect°¡ ÀÖÀ» °æ¿ì
				/*
				if (m_nAlphaEffect)
				{
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// ÀÌµ¿ÇßÀ¸¸é »õ·Î¿î ÁÂÇ¥ ¼³Á¤
				m_X	= x;
				m_Y	= y;		
				
				CheckInDarkness();
				
				return true;
			}
		break;

		case CREATURE_GROUND : 
			if (m_pZone->MoveGroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect°¡ ÀÖÀ» °æ¿ì
				/*
				if (m_nAlphaEffect)
				{
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// ÀÌµ¿ÇßÀ¸¸é »õ·Î¿î ÁÂÇ¥ ¼³Á¤
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		case CREATURE_FLYING : 
			if (m_pZone->MoveFlyingCreature(this, m_X, m_Y, x, y))
			{
				// Effect°¡ ÀÖÀ» °æ¿ì
				/*
				if (m_nAlphaEffect)
				{
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// ½Ã¾ß¸¦ ¹Ù²ãÁØ´Ù.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// ÀÌµ¿ÇßÀ¸¸é »õ·Î¿î ÁÂÇ¥ ¼³Á¤
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		// fake´Â ±×³É ¿òÁ÷ÀÎ´Ù.
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
// (x,y)¿¡¼­ ¹æÇâÀ¸·Î ÀÌµ¿ÇÑ À§Ä¡¸¦ ¾ò´Â´Ù.
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
// Creature¿¡ ´Þ¶óºÙ¾î¼­ Ç¥ÇöµÇ´Â Effect Ç¥Çö
//
// ÁßÃ¸À» Çã¿ëÇÏÁö ¾ÊÀ¸¹Ç·Î.. 
// ¾ÆÁ÷ ºÙ¾î¼­ Ç¥ÇöµÇÁö ¾ÊÀº Effect¸¸ »õ·Î Ãß°¡ÇÑ´Ù.
//
// ±×·¯³ª, ½Ã°£ È®ÀåÀ» ÇØ¾ßÇÏ´Â °æ¿ì°¡ »ý±ä´Ù.
//----------------------------------------------------------------------
MAttachEffect*		
MCreature::CreateAttachEffect(TYPE_EFFECTSPRITETYPE type, 
							  DWORD delayFrame, 
							  DWORD linkDelayFrame, 
							  BOOL bGroundEffect,
							  MEffect::EFFECT_TYPE eType)
{
	DEBUG_ADD_FORMAT("CreateAttachEffect. type=%d, delayf=%d", type, delayFrame);

	// Á×Àº °æ¿ì... delay¸¦ ¾ø¾Ø´Ù. 
	if (!m_bAlive)
	{
		// - -;;
		delayFrame = min( 10, delayFrame );
	}
	bool	IsMulti = false;
	int		orbitType = 0;
	// 2004, 08, 05 sobeit add start - ¼º¹®, ±æµå Å¸¿ö ÆÄÆíÆ¢±â
	if(GetCreatureType() >= 726 && GetCreatureType() <= 729)
	{	
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DUST_1 ||
		   type > EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED) // ¼º¹® °ü·Ã ÀÌÆåÆ®°¡ ¾Æ´Ï¸é ¹«Àú°Ç ÆÄÆíÆ¢±â..¤¾¤¾
		{
			int currentHP		= max(0,int(GetHP()));
			int frame = 4 -(currentHP*5/GetMAX_HP());
			
			if(m_NickNameType != frame) // º¯¼ö Ãß°¡ÇÏ±ä ±×·¸°í..¼º¹®Àº m_NickNameTypeÀ» ¾È¾²±â ¶§¹®¿¡ ¿ä±â¼­ TemporaryÇÏ°Ô ¾²ÀÚ-_-; 
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
			// ¼º¹® Å¸°Ý ÀÌÆåÆ® ÀÎµ¥.. ¾ø´Â°Ô ³ªÀ»µí..¤¾
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
	else if(GetCreatureType() == 734) // ±æµå Å¸¿ö
	{
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED ||
		   type > EFFECTSPRITETYPE_GUILD_TOWER_PROTECT) // ±æµå Å¸¿ö°ü·Ã ÀÌÆåÆ®°¡ ¾Æ´Ï¸é ¹«Àú°Ç ÆÄÆíÆ¢±â..¤¾¤¾
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
//	else if(GetCreatureType() == 723) // °¢¼º Áúµå·¹
//	{
//		if(type == EFFECTSPRITETYPE_GDR_DEAD)
//			delayFrame = 218;
//
//	}
	else
	{
//		if(GetCreatureType() == 734&&type == EFFECTSPRITETYPE_GUILD_TOWER_DESTROY) // ±æµå Å¸¿ö
//			delayFrame = 32;
	// 2004, 08, 05 sobeit add end - ¼º¹®, ±æµå Å¸¿ö ÆÄÆíÆ¢±â

		//------------------------------------------------
		// ÇÇÀÎ °æ¿ì´Â À§Ä¡¸¦ randomÇÏ°Ô
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_3_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_3_1)
		{
			// Å°¿¡ µû¶ó¼­.. 100³ÑÀ¸¸é... (°ñ·¹¸Ó)
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
		// ÇÇÀÎ °æ¿ì´Â À§Ä¡¸¦ randomÇÏ°Ô
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BULLET_OF_LIGHT_START_1)
		{
			// Å°¿¡ µû¶ó¼­.. 100³ÑÀ¸¸é... (°ñ·¹¸Ó)
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
		// Ä® ¸ÂÀº ÇÇ (¼¼·Î)
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
		// »ê¿¡ ¸ÂÀº smokeÀÎ °æ¿ì
		//------------------------------------------------
		else if (type==EFFECTSPRITETYPE_SMOKE)
		{
			type += rand()%5;
		}
		
		//------------------------------------------------
		// Typhoon back [»õ±â¼ú]
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
		// ¿©ÀÚ¿ë EffectSpriteTypeÀ» »ç¿ëÇÏ´Â °æ¿ì
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
	

		// °ø°Ý ¼Óµµ¿¡ µû¸¥ ÀÌÆåÆ®¸¦ ´Ù¸£°Ô ±¸º°ÇÑ´Ù.
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
	// ÀÌ¹Ì ÀÖ´Â °æ¿ì 
	//------------------------------
	if (m_bAttachEffect[type] && !IsMulti)
	{	
		//------------------------------------------------------
		// ¸ö¿¡ ºÙÀº°Å Ã¼Å©
		//------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		// °°Àº Effect¸¦ Ã£´Â´Ù. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// °°Àº typeÀ» Ã£´Â´Ù.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// Ãß°¡ÇÒ·Á´Â °ÍÀÌ ´õ ´Ê°Ô ³¡³¯ °æ¿ì¿¡¸¸ ½Ã°£À» È®ÀåÇÑ´Ù.
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
		// ¹Ù´Ú¿¡ ºÙÀº°Å Ã¼Å©
		//------------------------------------------------------
		iEffect = m_listGroundEffect.begin();

		// °°Àº Effect¸¦ Ã£´Â´Ù. 
		while (iEffect != m_listGroundEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// °°Àº typeÀ» Ã£´Â´Ù.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// Ãß°¡ÇÒ·Á´Â °ÍÀÌ ´õ ´Ê°Ô ³¡³¯ °æ¿ì¿¡¸¸ ½Ã°£À» È®ÀåÇÑ´Ù.
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
	// ¾ø´Â °æ¿ì Ãß°¡ÇÑ´Ù.
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

	// ºû³ª´Â EffectÀÇ °³¼ö¸¦ Áõ°¡?
	if (pEffect->GetBltType()==BLT_EFFECT)
	{
		// ÃÖÃÊ·Î Ãß°¡µÇ´Â ºû³ª´ÂEffectÀÌ¸é ½Ã¾ß¸¦ Ãß°¡½ÃÅ²´Ù.
		//if (m_nAlphaEffect==0)
		{
			pEffect->SetPosition( m_X, m_Y );
			//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
		}

		//m_nAlphaEffect ++;		
	}

	//------------------------------------------------------
	// ¹Ù´Ú¿¡ ºÙ´Â°Å
	//------------------------------------------------------
	if (bGroundEffect)
	{
		m_listGroundEffect.push_back( pEffect );
	}
	//------------------------------------------------------
	// ¸ö¿¡ ºÙ´Â °Å
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
// Creature¿¡ ºÙÀº ¸ðµç EffectÀÇ FrameÀ» ¹Ù²ãÁÖ°í..
// ³¡³ª´Â°Ô ÀÖÀ¸¸é list¿Í memory¿¡¼­ »èÁ¦ÇÑ´Ù.
//----------------------------------------------------------------------
void		
MCreature::UpdateAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	ATTACHEFFECT_LIST::iterator iEffectTemp;

	// ¹à±â´Â 0
	//m_MaxEffectLight = 0;

	//---------------------------------------------------------------------
	// Ä³¸¯ÅÍ »ö±ò ¹Ù²Ù´Â »ö..
	//---------------------------------------------------------------------
	// NULL°ªÀÌ´ç.. ¿ÜºÎ¿¡¼­ Ã¼Å©ÇØ¾ßÇÔ..
	m_AttachEffectColor = m_ChangeColorSet;	//ATTACHEFFECTCOLOR_NULL;
	int bShowColor =  (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
						|| g_CurrentFrame % g_pClientConfig->FRAME_DRAW_ORIGINAL_SPRITE);	// ¸î frame¸¶´Ù ÇÑ¹ø¾¿Àº ¿ø·¡ »ö±òÀ» º¸¿©ÁØ´Ù.
	int numColors = 0;
	
	//---------------------------------------------------------------------
	// ¸ðµç Effect¸¦ UpdateÇÑ´Ù.
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


		// ºü¸¥ ÀÌµ¿ÀÏ °æ¿ì ÀÌµ¿°ú Á¤ÁöÀÇ ÀÌÆåÆ®°¡ ´Ù¸£´Ù.... ÇÏµåÄÚµù..¤Ñ.¤Ì
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
			// Èí¿µÀÎ °æ¿ì¿¡´Â ¹æÇâÀ» ¹Ù²ÙÁö ¾Ê´Â´Ù.
			if( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_ABSORB_SOUL &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_FRONT &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_BACK )
				pEffect->SetDirection( m_CurrentDirection );

			// ÃÖ°í ¹à±â¸¦ °¡Áø EffectÀÇ ¹à±â¸¦ ÀúÀåÇÑ´Ù.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// À§Ä¡ ¼³Á¤
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// À§Ä¡°¡ ¹Ù²î¾ú°Å³ª
			// ºûÀÇ Å©±â(½Ã¾ß)°¡ ¹Ù²ï °æ¿ì
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// ½Ã¾ß ¹Ù²Ù±â
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
			// Ä³¸¯ÅÍ »ö±ò ¹Ù²Ù´Â EffectÀÌ¸é
			// ÇÏ³ª¸¦ ¼±ÅÃÇØ¾ß ÇÑ´Ù.
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
			// ÀÌ Effect°¡ ³¡³ª±â Àü¿¡ LinkCount¿¡ ÀÇÇØ¼­
			// ´ÙÀ½ ¿¬°áµÇ´Â Effect°¡ ÀÖÀ¸¸é »ý¼ºÇØ¾ß ÇÑ´Ù.
			//
			// ÇöÀçFrameÀÌ EndLinkFrameÀ» ³Ñ¾î°£ °æ¿ì
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext¿¡¼­ 
				// pEffectÀÇ EffectTargetÀ» NULL·Î ¸¸µé¾îÁÖ±â ¶§¹®¿¡
				// ¿©±â¼­ Áö¿ï ÇÊ¿ä ¾ø´Ù.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect´Â ¿©ÀüÈ÷ Á¸ÀçÇØ¾ß ÇÏ¹Ç·Î Áö¿ì¸é ¾ÈµÈ´Ù.
			}

			// Á¦´ë·Î µÈ °æ¿ì
			iEffect++;
		}
		//---------------------------------------------------------------------
		// ½Ã°£ÀÌ ´Ù µÅ¼­ ³¡³ª´Â °æ¿ì
		//---------------------------------------------------------------------
		else
		{
			
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flagÁ¦°Å
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flagÁ¦°Å
			}
		
			//---------------------------------------------------------------------
			//
			// ´ÙÀ½ ¿¬°áµÇ´Â Effect°¡ ÀÖÀ¸¸é »ý¼ºÇØ¾ß ÇÑ´Ù.
			//
			//---------------------------------------------------------------------
//			// 2004, 8, 16, sobeit add start test - µµ½½ 130 ½ºÅ³
//			if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_BURNING_SOL_CHARGING)
//			{
//				// 	¾ÆÁ÷ Â÷Â¡ ÁßÀÌ¸é ³¯¶ó°¡´Â°Ô ³ª¿À¸é ¾ÈµÈ´Ù..^^
//			}
//			// 2004, 8, 16, sobeit add end test - µµ½½ 130 ½ºÅ³
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// ºû³ª´Â Effect¸é ½Ã¾ß¸¦ »ç¶óÁö°Ô ÇØ¾ßÇÑ´Ù.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// ¸ðµç ºû³ª´ÂEffect°¡ »ç¶óÁ³À¸¸é ½Ã¾ß¸¦ Á¦°Å½ÃÅ²´Ù.
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
			
			// memory»èÁ¦, list»èÁ¦						
			delete pEffect;						// memoryÁ¦°Å
			
			// list¿¡¼­ »èÁ¦ÇÏ±â À§ÇØ¼­.. ÀÓ½Ã·Î ÀúÀå
			iEffectTemp = iEffect;

			iEffect++;
			m_listEffect.erase( iEffectTemp );	// list¿¡¼­ Á¦°Å
		}		
	}


	//---------------------------------------------------------------------
	// ¹Ù´Ú¿¡ ºÙ´Â effect
	//---------------------------------------------------------------------
	iEffect = m_listGroundEffect.begin();
	
	//---------------------------------------------------------------------
	// ¸ðµç Effect¸¦ UpdateÇÑ´Ù.
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
			// ÃÖ°í ¹à±â¸¦ °¡Áø EffectÀÇ ¹à±â¸¦ ÀúÀåÇÑ´Ù.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// À§Ä¡ ¼³Á¤
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// À§Ä¡°¡ ¹Ù²î¾ú°Å³ª
			// ºûÀÇ Å©±â(½Ã¾ß)°¡ ¹Ù²ï °æ¿ì
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// ½Ã¾ß ¹Ù²Ù±â
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
			// Ä³¸¯ÅÍ »ö±ò ¹Ù²Ù´Â EffectÀÌ¸é
			// ÇÏ³ª¸¦ ¼±ÅÃÇØ¾ß ÇÑ´Ù.
			//---------------------------------------------------------------------
			/*
			// ¹Ù´Ú¿¡ ±ò¸®´Â °Å´Â »ö±ò ¹Ù²Ù´Â°Ô ¾ø´Ù.
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
			// ÀÌ Effect°¡ ³¡³ª±â Àü¿¡ LinkCount¿¡ ÀÇÇØ¼­
			// ´ÙÀ½ ¿¬°áµÇ´Â Effect°¡ ÀÖÀ¸¸é »ý¼ºÇØ¾ß ÇÑ´Ù.
			//
			// ÇöÀçFrameÀÌ EndLinkFrameÀ» ³Ñ¾î°£ °æ¿ì
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext¿¡¼­ 
				// pEffectÀÇ EffectTargetÀ» NULL·Î ¸¸µé¾îÁÖ±â ¶§¹®¿¡
				// ¿©±â¼­ Áö¿ï ÇÊ¿ä ¾ø´Ù.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect´Â ¿©ÀüÈ÷ Á¸ÀçÇØ¾ß ÇÏ¹Ç·Î Áö¿ì¸é ¾ÈµÈ´Ù.
			}

			// Á¦´ë·Î µÈ °æ¿ì
			iEffect++;
		}
		//---------------------------------------------------------------------
		// ½Ã°£ÀÌ ´Ù µÅ¼­ ³¡³ª´Â °æ¿ì
		//---------------------------------------------------------------------
		else
		{
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flagÁ¦°Å
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flagÁ¦°Å
			}
		
			//---------------------------------------------------------------------
			//
			// ´ÙÀ½ ¿¬°áµÇ´Â Effect°¡ ÀÖÀ¸¸é »ý¼ºÇØ¾ß ÇÑ´Ù.
			//
			//---------------------------------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// ºû³ª´Â Effect¸é ½Ã¾ß¸¦ »ç¶óÁö°Ô ÇØ¾ßÇÑ´Ù.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// ¸ðµç ºû³ª´ÂEffect°¡ »ç¶óÁ³À¸¸é ½Ã¾ß¸¦ Á¦°Å½ÃÅ²´Ù.
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
			
			// memory»èÁ¦, list»èÁ¦						
			delete pEffect;						// memoryÁ¦°Å
			
			// list¿¡¼­ »èÁ¦ÇÏ±â À§ÇØ¼­.. ÀÓ½Ã·Î ÀúÀå
			iEffectTemp = iEffect;

			iEffect++;
			m_listGroundEffect.erase( iEffectTemp );	// list¿¡¼­ Á¦°Å
		}		
	}
}

//----------------------------------------------------------------------
// UnSet LightSight AttachEffect
//----------------------------------------------------------------------
// Creature¿¡ ºÙÀº ¸ðµç EffectÀÇ ½Ã¾ß¸¦ »èÁ¦ÇÑ´Ù.
//----------------------------------------------------------------------
/*
void		
MCreature::UnSetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// ¸ðµç Effect¸¦ UpdateÇÑ´Ù.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// ½Ã¾ß »èÁ¦
//		g_pZone->UnSetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// ´ÙÀ½ ²¨
		iEffect++;
	}
}
*/

//----------------------------------------------------------------------
// Set LightSight AttachEffect
//----------------------------------------------------------------------
// Creature¿¡ ºÙÀº ¸ðµç EffectÀÇ ½Ã¾ß¸¦ Ãß°¡ÇÑ´Ù.
//----------------------------------------------------------------------
/*
void		
MCreature::SetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// ¸ðµç Effect¸¦ UpdateÇÑ´Ù.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// ½Ã¾ß »èÁ¦
//		g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// ´ÙÀ½ ²¨
		iEffect++;
	}
}
*/


//----------------------------------------------------------------------
// Set Dead
//----------------------------------------------------------------------
// Á×´Â actionInfo¸¦ º¸¿©ÁÖ°í Á×´Â´Ù.
//----------------------------------------------------------------------
void
MCreature::SetDead()
{
	// »ì¾Æ ÀÖ´Â °æ¿ì¸¸ Á×ÀÎ´Ù. -_-;;
	if (m_bAlive)
	{	
//		if(HasEffectStatus(EFFECTSTATUS_HIDE_TO_ATTACKER))
//		{
//			if(GetCreatureType() == 738) // ÇÃ·¡Á® ½ºÅ×ÀÌ¼Ç
//				ExecuteActionInfoFromMainNode(SKILL_PLEASURE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);			
//			else if(GetCreatureType() == 739) // ·£µå ¸¶ÀÎ
//				ExecuteActionInfoFromMainNode(SKILL_LAND_MINE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 0, NULL, false);	
//			else if(GetCreatureType() == 740) // Å¬·¹ÀÌ ¸ð¾î
//				ExecuteActionInfoFromMainNode(SKILL_CLAYMORE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);	
//		}
		//if(HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) )
		if(GetCreatureType() == 723) // °¢¼º Áúµå·¹
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
		// server blockÁÂÇ¥ ¾ø¾ÖÁÖ±â
		//-------------------------------------------------------
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	
		// 2002.3.22 Ãß°¡
		SetGroundCreature();

		MovePosition( m_ServerX, m_ServerY );
		SetStop();

		// ´Ù¸¥ °á°ú¸¦ Àû¿ë½ÃÅ²´Ù.
		AffectUsedActionInfo(m_nUsedActionInfo);		
		m_nUsedActionInfo	= ACTIONINFO_NULL;
		m_nSpecialActionInfo = ACTIONINFO_NULL;

		// Á×À» ¶§ÀÇ ActionInfo	
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
			// Á×À» ¶§ÀÇ µ¿ÀÛ
			SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );

			// ³ªÇÑÅ× ÇÏ´Â µ¿ÀÛ
			SetTraceID( m_ID );
			m_TraceX				= m_X;
			m_TraceY				= m_Y;
			m_TraceZ				= 0;
		}

		//-------------------------------------------------------
		// º¸ÀÌ°Ô ÇÏ±â
		//-------------------------------------------------------
		SetVisibleSoon();

		// HP
		StopRecoveryHP();
		StopRecoveryMP();
		SetStatus( MODIFY_CURRENT_HP, 0 );

		m_Z = 0;


		// Á×¾ú´Ù.
		m_bAlive = false;	
		m_bInCasket = false;

		//-------------------------------------------------------
		// Effect Á¦°Å
		//-------------------------------------------------------
		ClearAttachEffect();

		//-------------------------------------------------------
		// Effect»óÅÂµé Á¦°Å
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

		// ´ÙÅ©´Ï½º ¹þ¾î³ª±â..
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;

	}	
}

//----------------------------------------------------------------------
// Set Corpse
//----------------------------------------------------------------------
// Á×¾î ÀÖ´Â »óÅÂ·Î ¸¸µç´Ù. --> ¹Ù·Î ½ÃÃ¼·Î..
//----------------------------------------------------------------------
void
MCreature::SetCorpse()
{	
	//-------------------------------------------------------
	// server blockÁÂÇ¥ ¾ø¾ÖÁÖ±â
	//-------------------------------------------------------
	m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);

	// Á¤Áö
	SetStop();

	// Á×´Â ¸ð½À
	ACTIONINFO deadAI = (enum ACTIONINFO)(*g_pCreatureTable)[m_CreatureType].DeadActionInfo;
	
	// Á×À» ¶§ÀÇ µ¿ÀÛ
	SetAction( GetActionInfoAction(deadAI) );

	// 2002.3.22 Ãß°¡
	SetGroundCreature();
	
	// µ¿ÀÛ ¸ðµÎ ¼öÇà..
	m_ActionCount	= GetActionCountMax(); 
	m_MoveCount		= m_MoveCountMax;
	
	// Á×¾ú´Ù.
	m_bAlive = false;

	// ´ÙÅ©´Ï½º ¹þ¾î³ª±â..
	m_DarknessCount = -1;
	m_DarknessCountInc = 0;
}

//----------------------------------------------------------------------
// Set Alive
//----------------------------------------------------------------------
void
MCreature::SetAlive()
{
	// »ì¾Æ³­´Ù. 
	m_bAlive = true;

	m_bHasHead = true;

	SetAction( ACTION_STAND );

	//-------------------------------------------------------
	// Effect»óÅÂµé Á¦°Å
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

	if ((*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// ¹ÚÁãÀÎ °æ¿ì¿¡´Â SetGroundCreatureÇß´ø°Å ¶§¸Å ´Ù½Ã m_MoveType¸¦ ¹Ù²ãÁØ´Ù
		m_MoveType = CREATURE_FLYING;

	if (m_MoveType == CREATURE_FLYING)
	{
		m_Z=(*g_pCreatureTable)[m_CreatureType].FlyingHeight;
	}
}

//----------------------------------------------------------------------
// Set NextAction
//----------------------------------------------------------------------
// ´ÙÀ½¿¡ ÇÒ Çàµ¿À» ¼³Á¤ÇÑ´Ù.
//----------------------------------------------------------------------
void			
MCreature::SetNextAction(BYTE action)
{
	m_NextAction = action;
}

//----------------------------------------------------------------------
// Set NextAction to Move
//----------------------------------------------------------------------
// ´ÙÀ½ action¿¡´Â moveÇÑ´Ù.
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
		
		// ºùºù µ¹¸°´Ù.
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
			// ºùºù µ¹¸°´Ù.
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

		if(m_CreatureType == 793) // ½½·¹ÀÌ¾î Á¦¹° -_-;
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
//			sprintf(TempBuffer,"ºí·¯µð Á¦´Ï½º ³¡ a:%d, n:%d, n:%d, z:%d,s:%d", m_nUsedActionInfo, m_NextAction, m_nNextUsedActionInfo,MAGIC_BLOODY_ZENITH,SKILL_SET_AFIRE);
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
// ÇÑ Action¸¶´Ù Frame¼ö°¡ ´Ù¸¦ °ÍÀÌ´Ù.. 
// ÀÌ°Å´Â file·Î »©¼­ Á¤º¸¸¦ µÎ´Â°Ô ³ªÀ» °ÍÀÎµ¥
// FRAME_PACK¿¡¼­ ÀÐ¾î¿Í¾ß ÇÑ´Ù.
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

	// ¹«Á¶°Ç ²¨ÁØ´Ù.
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
	// ÇöÀç ¸ð½ÀÀÇ action¿¡ ¸Â´Â °æ¿ì. 2001.10.5
	//-----------------------------------------------------------------
	if (action < GetActionMax())
	{
		
		//-----------------------------------------------------
		// ¿ÀÅä¹ÙÀÌ Å¸°í ÀÖÀ»¶§
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
			// °É¾î´Ù´Ò¶§
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
		// MoveÀÎ °æ¿ì
		//------------------------------------------------
		if (action==m_MoveAction)//ACTION_MOVE || action==ACTION_SLAYER_MOTOR_MOVE)
		{
			// ±×³É ¼­ÀÖ´Â °æ¿ì¸é... ³¡µ¿ÀÛÀ¸·Î ¸¸µç´Ù.
			// ActionÀÌ ³¡³µ´Ù°í Ç¥½ÃÇØÁÖ±â À§ÇØ¼­..
			if (m_Action==ACTION_STAND 
				|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
			{			
				m_ActionCount = 0;
				m_ActionCountMax = 0;			
			}		

			// ActionÀÌ ³¡³­ °æ¿ì¸¸ ActionÀ» Move·Î ÇÑ´Ù.
			// ¾Æ´Ñ °æ¿ì´Â.. ActionÀÌ ³¡³ª°í Move·Î ¹Ù²ï´Ù.
			if (m_ActionCount>=GetActionCountMax())
			{
				m_Action = action;
			}		

			// Á¤Áö
			m_sX = 0;
			m_sY = 0;

			// Server¿¡¼­ °ËÁõµÈ À§Ä¡·Î ÀÌµ¿½ÃÅ²´Ù.
			//MovePosition( m_ServerX, m_ServerY );
			
			// [ TEST CODE ]
			// ActionÀ» ¾ø¾Ø´Ù.
			// --> ¾È ¾ø¾Ö¸é.. ActionÀÌ ³¡³ª°í Move°¡ ¼öÇàµÈ´Ù. 
			// ±×·±µ¥, Move°¡ ¾ÈµÅ¼­ ÁÂÇ¥°¡ ºø³ª°¥ È®·üÀÌ ÀÖ´Ù. Èì!!
			//m_ActionCount = 0;
			//m_ActionCountMax = 0;

			m_RepeatCount = 0;

			// Move Ã¹ µ¿ÀÛ
			m_MoveCount = 0;
			m_NextMoveCount = 0;
			
			//m_MoveCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( action );
			m_MoveCountMax = GetCreatureActionCountMax( this, action );
				//g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
				//							[action][m_CurrentDirection].GetSize();//ACTIONCOUNT_MOVE;

		}
		//------------------------------------------------
		// ActionÀÎ °æ¿ì
		//------------------------------------------------
		else
		{
			//------------------------------------------------
			// extremeÀº Æ¯Á¤ µ¿ÀÛ¿¡¼­ Ç®¸°´Ù.
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

			if (// ¿ÀÅä¹ÙÀÌ Å¸°í ÀÖÀ»¶§´Â action ¾È º¸¿©ÁØ´Ù.
				m_MoveDevice==MOVE_DEVICE_RIDE && action!=ACTION_SLAYER_MOTOR_STAND
				// damagedÀÎ °æ¿ì..
				|| action==ACTION_DAMAGED 						
						&& (// °ø°Ý¹Þ´Â ÁßÀÌ ¾Æ´Ï°Å³ª Á¤Áö µ¿ÀÛÀÌ ¾Æ´Ï¸é °ø°Ý ¹Þ´Â ¸ð½À Ç¥Çö ¾È ÇÑ´Ù
							m_Action!=ACTION_DAMAGED && m_Action!=ACTION_STAND && m_Action!=ACTION_MOVE
						//&& (// ÈíÇ÷ ´çÇÏ´Â µ¿ÀÛ ÁßÀÌ°Å³ª 
						//	m_Action==ACTION_DRAINED
							// ¹ìÆÄÀÌ¾îÀÏ¶§, ÈíÇ÷ÇÏ´Â µ¿ÀÛ Áß¿¡´Â damaged¸¦ ¾È º¸¿©ÁØ´Ù.
						//	|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							)
				)
			{
				m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
				return;
			}

			
			// 2005, 1, 6, sobeit add start - motor ride move ÇÏ´øÁß stop °¡ ³¯¶ó¿À¸é ÇÑ¹øÀº ¹«½Ã..
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
				// µ¿ÀÛÀÌ ¾ø´Â °æ¿ì
				m_Action = (m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND;
				m_ActionCount = 0;		
				m_RepeatCount = 0;
				//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
				m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );
				
				// ¹Ø¿¡ ÇØ³ù±æ·¡ ÀÏ´Ü ÇØ³õ´Âµ¥.. ¹ºÁö ¸ô°Ú´Ù - -; 2002.1.10
				m_nSpecialActionInfo = ACTIONINFO_NULL;
			}
			else
			{
				m_Action = action;			

				//---------------------------------------------
				// actionÀ» ¹Ýº¹ÇÏ´Â °æ¿ì
				//---------------------------------------------
				// ¹Ýº¹ actionÀÇ ½ÃÀÛºÎÅÍ º¸¿©ÁØ´Ù.
				// ±×·¯³ª, ´ÙÀ½ ¹Ýº¹ packetÀÌ ¾ðÁ¦ ¿ÃÁö ¸ð¸£±â ¶§¹®¿¡..
				// ActionCounMax´Â ¹«Á¶°Ç ³¡~frameÀ¸·Î Á¤ÇÑ´Ù.
				//---------------------------------------------
//				BOOL bSlayer = IsSlayer();

				if (m_RepeatCount!=0	//m_Action == action			
					&& action!=ACTION_STAND
					&& (!IsSlayer() || IsSlayer() && action!=ACTION_SLAYER_MOTOR_STAND)
					//&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame())
					)
				{
					// count ¼³Á¤..
					
					m_ActionCount = GetActionInfoRepeatStartFrame( m_nSpecialActionInfo );
//					char Msg[128];
//					sprintf(Msg, "%d" , m_ActionCount);
//					g_pSystemMessage->Add(Msg);	
				}
				//---------------------------------------------
				// ÀÏ¹ÝÀûÀÎ °æ¿ì..
				//---------------------------------------------
				else
				{			
					// count ¼³Á¤..
					m_ActionCount = 0;		
					m_RepeatCount = 0;

					// 2004, 11, 12, sobeit modify start - block
					// ÀÌ°Å ¹¹Áö? ¿Ö ÇØ³ù´ÂÁö ¸ð¸£°Ú³×.. - -;; 2002.1.10
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

	// µ¿ÀÛÀ» ½ÃÀÛÇÒ¶§ ºÎ°¡ÀûÀ¸·Î ºÙ´Â Effect µé.
	// ¾îµò°¡·Î »©¾ß ÇÏ´Âµ¥.-_-
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

//	// 2004, 9, 8, sobeit add start - µµ½½ 130 ½ºÅ³ °ü·Ã
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
//	// 2004, 9, 8, sobeit add end - µµ½½ 130 ½ºÅ³ °ü·Ã
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

		// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµå ÄÚµù
		bool bTree = false;
		char *find = strchr(str+startIndex, '\n'); 
		if(find != NULL)
		{
			*find = '\0';
			endIndex = strlen(str+startIndex)+startIndex;
			bTree = true;
		}
		
		// lenÀÌ Â¥¸¦¸¸ÇÑ ±æÀÌµµ ¾ÈµÇ¸é...
		if (endIndex >= len)
		{
			endIndex = len;

			// ÀÏÁ¤ °³¼öÀÇ StringÀ» copyÇÑ´Ù.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH°³¸¦ Â¥¸¦ ¼ö ¾ø´Â°¡?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 ±îÁö strcpy
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

		// ´ÙÀ½ ÁÙ..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index¸¦ ¹Ù²Û´Ù.
		startIndex = endIndex;
	}
	
	// Ã¤ÆÃ StringÀÌ DelayµÉ ½Ã°£À» ÁöÁ¤ÇØÁØ´Ù.	
//	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;
//	m_NextChatFadeTime = g_CurrentTime + 0xFFFFFFFF;

	// Ã¤ÆÃ »ö±ò
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
// StringÀ» Ãß°¡ÇÑ´Ù. ³¡¿¡~..
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

	// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµåÄÚµù
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
	
	if(strstr(str, "¹èÃ¶¼ü´Ï´Ù") != NULL)
	{
		m_HeadSkin = 215;
	}
	else if(strstr(str, "ÇØ°ñ¹Ù°¡Áö") != NULL)
	{
		m_HeadSkin = 149;
	}
	else if(strstr(str, "´Á´ëÀÎ°£") != NULL)
	{
		m_HeadSkin = 397;
	}
	else if(strstr(str, "´ß´ë°¡¸®") != NULL)
	{
		m_HeadSkin = 221;
	}
	else if(strstr(str, "°¡¸é¶óÀÌ´õ") != NULL)
	{
		m_HeadSkin = 316;
	}


	// ÇÑ ¹®ÀåÀÇ StringÀ» ÀûÀýÇÑ ±æÀÌ·Î Àß¶óÁØ´Ù~~
	DEBUG_ADD("[SetChatString] before while");
	while (endIndex < len)
	{
		endIndex = startIndex + g_pClientConfig->MAX_CHATSTRING_LENGTH;

		// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµå ÄÚµù
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
		
		// lenÀÌ Â¥¸¦¸¸ÇÑ ±æÀÌµµ ¾ÈµÇ¸é...
		if (endIndex >= len)
		{
			endIndex = len;

			// ÀÏÁ¤ °³¼öÀÇ StringÀ» copyÇÑ´Ù.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH°³¸¦ Â¥¸¦ ¼ö ¾ø´Â°¡?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 ±îÁö strcpy
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

		// ´ÙÀ½ ÁÙ..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index¸¦ ¹Ù²Û´Ù.
		startIndex = endIndex;
	}
	DEBUG_ADD("[SetChatString] while ok");

	// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµåÄÚµù
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		DEBUG_ADD("[SetChatString] Tree code");

		int len = strlen(szTreeFrom) + 6;

		char szTemp[512];
		memset(szTemp, ' ', g_pClientConfig->MAX_CHATSTRING_LENGTH);

		strcpy(szTemp + max(g_pClientConfig->MAX_CHATSTRING_LENGTH - len, 0), "From. ");
		strcat(szTemp, szTreeFrom);

		strcpy(m_ChatString[m_ChatStringCurrent], szTemp);

		// ´ÙÀ½ ÁÙ..
		m_ChatStringCurrent++;

		DEBUG_ADD("[SetChatString] Tree code ok");
	}
	

	// Ã¤ÆÃ StringÀÌ DelayµÉ ½Ã°£À» ÁöÁ¤ÇØÁØ´Ù.	
	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;

	// Ã¤ÆÃ »ö±ò
	m_ChatColor = color;//RGB_WHITE;//0xFFFF;
	m_OriChatColor = m_ChatColor;
	
	// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµå ÄÚµù
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		m_NextChatFadeTime = 0;
	}
	else
	{
		m_OriChatColor |= 0xFF000000;
	}

	// ¸»ÇÑ ½Ã°£ ¼³Á¤
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
// ChatStringÀ» ¾îµÓ°Ô ÇÑ´Ù.
//----------------------------------------------------------------------
void		
MCreature::FadeChatString()
{
	/*
	m_ChatString[m_ChatStringCurrent][0] = NULL;	

	m_ChatStringCurrent++;
	if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

	// Ã¤ÆÃ StringÀÌ DelayµÉ ½Ã°£À» ÁöÁ¤ÇØÁØ´Ù.	
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

	BYTE color[3] ={ static_cast<BYTE>(m_OriChatColor & 0xFF), static_cast<BYTE>((m_OriChatColor & 0xFF00)>>8), static_cast<BYTE>((m_OriChatColor & 0xFF0000)>>16) };


	if (b>=g_pClientConfig->MIN_CHATSTRING_COLOR256)
	{
		// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµåÄÚµù
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
		// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµåÄÚµù
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

			// Ã¤ÆÃ »ö±ò
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
// 0ÀÌ °¡Àå ¿À·¡µÈ StringÀÌ°í g_pClientConfig->MAX_CHATSTRING-1ÀÌ °¡Àå ÃÖ±Ù¿¡ °ÍÀ¸·Î
// returnÇØ¾ß ÇÑ´Ù.
//----------------------------------------------------------------------
const char*	
MCreature::GetChatString(BYTE i)
{ 
	//                          i   = ½ÇÁ¦·Î returnµÇ¾î¾ß ÇÏ´Â °ª
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
// pItemÀ» this Creature°¡ »ç¿ëÇÒ ¼ö ÀÖ´ÂÁöÀÇ ¿©ºÎ¸¦ Ã¼Å©ÇØ¼­ ¼³Á¤ÇÑ´Ù.
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
			// ½½·¹ÀÌ¾îÀÎ °æ¿ì´Â STR, DEX, INTÃ¼Å©
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
			// Á¦ÇÑ ¼öÄ¡°¡ ¾ø°Å³ª 
			// Æ¯Á¤ ¼öÄ¡¸¦ ÇÏ³ª ¸¸Á·ÇÏ´Â °æ¿ì..
			//-----------------------------------------------------
			if (
				//-----------------------------------------------------
				// ¼ºº° °Ë»ç
				//-----------------------------------------------------
				(pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()
				)
				
				//-----------------------------------------------------
				// ÇÊ¿ä´É·ÂÄ¡°¡ ¾ø°Å³ª..
				//-----------------------------------------------------
				&& (
				
				//-----------------------------------------------------
				// ÀÖ´Â°æ¿ì.. ´Ù ¸¸Á·½ÃÄÑ¾ß ÇÑ´Ù.
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
			// Å¸Á¾Á· ¾ÆÀÌÅÛ »ç¿ë ºÒ°¡
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_VAMPIRE:
		if (pItem->IsVampireItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// ¹ìÆÄÀÌ¾îÀÎ °æ¿ì´Â level·Î Ã¼Å©ÇÑ´Ù.
			//-----------------------------------------------------
			if (
					//-----------------------------------------------------
					// ¼ºº° °Ë»ç
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
			// item Á¾·ù¿¡ µû¶ó¼­ skill Ã¼Å©ÇÑ´Ù.
			//-----------------------------------------------------
			// g_pSkillAvailableÀº playerÀÇ skillÁ¤º¸ÀÌ´Ù.
			// ¾îÂ÷ÇÇ ´Ù¸¥ creature°¡ CheckAffectStatus()¸¦ »ç¿ëÇÒ ÀÏÀº ¾øÁö¸¸.. --;
			// ÀÏ´ÜÀº item levelÃ¼Å©·Î¸¸ ³¡³»µµ·Ï ÇÑ´Ù...
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
			// Å¸Á¾Á· ¾ÆÀÌÅÛ »ç¿ë ºÒ°¡
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_OUSTERS:
		if (pItem->IsOustersItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// ½½·¹ÀÌ¾îÀÎ °æ¿ì´Â STR, DEX, INTÃ¼Å©
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
			// ¾Æ¿ì½ºÅÍÁîÀÎ °æ¿ìµµ-_- level·Î Ã¼Å©ÇÑ´Ù.
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
			// Å¸Á¾Á· ¾ÆÀÌÅÛ »ç¿ë ºÒ°¡
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;
	}
}

//----------------------------------------------------------------------
// Set Direction To Creature
//----------------------------------------------------------------------
// ´Ù¸¥ Creature¸¦ ÇâÇØ¼­ ¹Ù¶óº»´Ù.
//----------------------------------------------------------------------
void	
MCreature::SetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// °°Àº ÁÂÇ¥¶ó¸é.. ÇöÀç ¹æÇâÀ¸·Î µÐ´Ù.
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
// m_CurrentDirectionÀ» m_DirectionÀ¸·Î ÀÚ¿¬½º·´°Ô ¹Ù²Û´Ù.
// ±×·¯³ª, ÇöÀç ¿òÁ÷ÀÌ´Â ¹æÇâ(m_DirectionMoved)ÀÌ ¿ì¼±ÀÌ´Ù.
//----------------------------------------------------------------------
bool	
MCreature::ChangeNearDirection()
{
	int toDir;
	
	// actionÀ» º¸¿©ÁÖ´Â ÁßÀÌ¸é.. ¹æÇâ ¼³Á¤
	if (m_ActionCount<GetActionCountMax())
	{
		toDir = m_Direction;
	}
	// ¿òÁ÷ÀÌ´Â ÁßÀÌ¸é m_DirectionMoved
	// ´Ü, KnockBackÀÌ³ª FastMoveÀÎ °æ¿ì¿¡´Â... ¹æÇâ ¹«½Ã..
	else if (m_MoveCount>0 && m_MoveCount<m_MoveCountMax
		&& !m_bKnockBack
		&& !m_bFastMove)		
	{
		toDir = m_DirectionMoved;
	}
	// ¾Æ´Ï¸é.. m_Direction
	else
	{
		toDir = m_Direction;
	}

	// °°À¸¸é ¹Ù²Ü ÇÊ¿ä°¡ ¾ø´Ù.
	if (m_CurrentDirection==toDir)
	{
		return false;
	}

	// ´Ù¸£¸é.. 
	// ¿ÞÂÊÀ¸·Î µ¹°Å³ª ¿À¸¥ÂÊÀ¸·Î µ¹°Ô ÇØ¾ßÇÑ´Ù.

	// ¾ç¼ö·Î ¸¸µé±â À§ÇØ¼­.. ±Û°í.. ¼Óµµ¸¦ À§ÇØ¼­.. &¸¦ »ç¿ëÇß´Ù.
	int gap = (((m_CurrentDirection+MAX_DIRECTION) - toDir) & 0x07);//% MAX_DIRECTION;

	// 4, 7
	if (gap==4)
	{
		// ¿ÞÂÊÀÌ³ª ¿À¸¥ÂÊÀ¸·Î randomÇÏ°Ô µ·´Ù.
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

	// 0~7À» À¯ÁöÇÏµµ·Ï ÇÑ´Ù.
	m_CurrentDirection &= 0x07;

	return true;	
}

//----------------------------------------------------------------------
// Get Counter Direction
//----------------------------------------------------------------------
// ÁøÇà ¹æÇâÀÇ ¹Ý´ë ¹æÇâÀ» ±¸ÇÑ´Ù.
// À½ ÀÌ°Å staticÀ¸·Î ¹Ù²ÙµçÁö d¸¦ ¾ø¾ÖµçÁö ÇØ¾ß´Âµ¥... - -
//----------------------------------------------------------------------
int
MCreature::GetCounterDirection( int d )
{
	// ¹Ý´ë¹æÇâ
	d += 4;

	// 0~7±îÁö¸¸ µÇµµ·Ï ÇÑ´Ù.
	while (d < 8)
	{
		d -= 8;
	}

	return d;
}

//----------------------------------------------------------------------
// Get Direction To Creature
//----------------------------------------------------------------------
// ÇöÀç À§Ä¡(m_X,m_Y)¿¡¼­ (sX,sY)¸¦ ÇâÇØ¼­ ¹Ù¶óº¸´Â ¹æÇâÀ» °áÁ¤ÇÑ´Ù.
//----------------------------------------------------------------------
BYTE	
MCreature::GetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// °°Àº ÁÂÇ¥¶ó¸é.. ÇöÀç ¹æÇâÀ¸·Î µÐ´Ù.
	if (m_X==sX && m_Y==sY)
	{
		return m_CurrentDirection;
	}

	return MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// ÇöÀç CreatureÀÇ À§Ä¡¿¡¼­ direction¹æÇâÀ¸·Î ÀÌµ¿ÇÑ °æ¿ìÀÇ À§Ä¡
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(BYTE direction, POINT &next)
{
	// ÇöÀç À§Ä¡¿¡¼­ ´ÙÀ½ ¹æÇâ¿¡ ´ëÇØ¼­ ¸ñÇ¥ ¼³Á¤
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
// ÇöÀç CreatureÀÇ À§Ä¡¿¡¼­ m_CurrentDirection¹æÇâÀ¸·Î ÀÌµ¿ÇÑ °æ¿ìÀÇ À§Ä¡
//----------------------------------------------------------------------
/*
void
MCreature::GetNextPosition(POINT &next)
{
	// ÇöÀç À§Ä¡¿¡¼­ ´ÙÀ½ ¹æÇâ¿¡ ´ëÇØ¼­ ¸ñÇ¥ ¼³Á¤
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
// ÇöÀç CreatureÀÇ À§Ä¡¿¡¼­ m_CurrentDirection¹æÇâÀ¸·Î ÀÌµ¿ÇÑ °æ¿ìÀÇ À§Ä¡
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(TYPE_SECTORPOSITION &sX, TYPE_SECTORPOSITION &sY)
{
	// ÇöÀç À§Ä¡¿¡¼­ ´ÙÀ½ ¹æÇâ¿¡ ´ëÇØ¼­ ¸ñÇ¥ ¼³Á¤
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
// ÇöÀçÀÇ ¹æÇâ(m_CurrentDirection)À¸·Î ÇÑ FrameÀÌµ¿ÇÑ´Ù.
//
// ÇÑ Sector¸¦ ÀÌµ¿ÇÏ´Â°Ô ¾Æ´Ï°í Sector¿Í Sector»çÀÌ¸¦ ÀÌµ¿ÇÏ´Â Áß°£ÀÇ
// ÇÑ FrameÀ» ÀÌµ¿ÇÏ´Â °ÍÀÌ´Ù. 
//
// Sector --> Sector¿¡¼­ ¸î FrameÀ» ÀÌµ¿ÇÒ±î??°¡ ¹®Á¦ÀÎµ¥...
//    = cX,cY¿¡ ÇÑ¹ø¿¡ ÀÌµ¿ÇÏ´Â pixel´ÜÀ§¸¦ ´ëÀÔÇÏ°í
//    = sX,sY¿¡ ÇÑ¹ø¿¡ ÀÌµ¿ÇÏ´Â pixel´ÜÀ§¸¦ ÀÔ·ÂÇÏ¸é µÈ´Ù.
//
// Move()¿¡¼­´Â
//    cX,cY°¡ 0ÀÌ¸é ´Ù½Ã cX,cY, sX,sY °ªÀ» »ý¼ºÇÏ°í
//    0ÀÌ ¾Æ´Ï¸é sX
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

	// ¸ñÀû Sector¿¡ µµ´ÞÇÑ °æ¿ì	
	if (m_MoveCount==0)//m_sX==0 && m_sY==0)// && m_ActionCount==0)
	{
		// ÇöÀç ¹æÇâÀ¸·Î ÇÑ Sector¸¦ ÁøÇàÇÏ°í
		// cX,cY, sX,sY¸¦ ´Ù½Ã ÁöÁ¤ÇØÁà¾ß ÇÑ´Ù.

		//--------------------------------------------
		// ÀÌµ¿ °¡´ÉÇÑÁö check
		//--------------------------------------------
		TYPE_SECTORPOSITION	x, y;

		//--------------------------------------------
		// fast moveÀÎ °æ¿ì´Â nextX, nextY¿¡ ÁÂÇ¥°¡ ÀÖ´Ù.
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
			// ÇöÀç ¹æÇâÀ¸·Î ¿òÁ÷¿´À» ¶§ÀÇ ÁÂÇ¥¸¦ °áÁ¤ÇÑ´Ù.
			x = m_X; 
			y = m_Y;
			GetPositionToDirection( x, y, m_DirectionMove );
		}

		//--------------------------------------------
		// ¿òÁ÷ÀÏ ¼ö ÀÖÀ» °æ¿ì
		//--------------------------------------------
		//
		// ¿òÁ÷ÀÏ ¼ö ¾ø´õ¶óµµ ¿òÁ÷¿©¾ß ÇÑ´Ù.
		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ÀÌ±â ¶§¹®¿¡..
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
			// zoneÀÇ sectorÀÇ Á¤º¸¸¦ ¹Ù²ãÁØ´Ù.
			// m_X¿Í m_Yµµ ¼³Á¤ÇØÁØ´Ù.
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
				// 5 FrameÀÌ ÀÖ´Ù°í ÇÒ °æ¿ì
				//--------------------------------------------
				//
				// [1] UP,DOWN,LEFT,RIGHTÀÏ ¶§,
				//
				//     Xº¯È­ : 0 16 32 48 64   (+-16)
				//     Yº¯È­ : 0  8 16 24 32   (+-8)
				// 
				//
				// [2] ´ë°¢¼±(LEFTUP,LEFTDOWN,RIGHTUP,RIGHTDOWN)À¸·Î ¿òÁ÷ÀÏ¶§,
				//
				//     Xº¯È­ : 0  8 16 24 32   (+-8)
				//     Yº¯È­ : 0  4  8 12 16   (+-4)
				//
				//--------------------------------------------
				// sX,sY : ¿òÁ÷¿©¾ßÇÒ ÀüÃ¼ pixel(ÇÑ TILE)
				// cX,cY : ÀÌµ¿ÇÏ´Â ´ÜÀ§ pixel
				//--------------------------------------------

				//--------------------------------------------
				// Fast Move ÀÎ °æ¿ì
				//--------------------------------------------
				if (m_bFastMove)
				{
					// Á÷¼± °Å¸®·Î ¿òÁ÷ÀÏ¶§ÀÇ pixel°Å¸®
					// ex) ¿ÞÂÊ 2 --> 1 : ( 2 - 1 ) * TILE_X
					//     À§ÂÊ 2 --> 1 : ( 2 - 1 ) * TILE_Y
					m_sX = (ox - m_X) * TILE_X;
					m_sY = (oy - m_Y) * TILE_Y;
					//------------------------------------------------
					// ½ÇÁ¦ ÇÑ Å¸ÀÏ ÀÌµ¿ ¼ÓµµÀÇ ¹ÝÀÇ ¼Óµµ¿¡ ¸ñÀûÁö±îÁö ÀÌµ¿ÇÑ´Ù.
					//------------------------------------------------
					// ÇÑ Å¸ÀÏ ÀÌµ¿ÇÒ¶§ÀÇ Frame ¼ö
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
				// Á¤»ó ÀÌµ¿
				//--------------------------------------------
				else
				{				
					m_DirectionMoved = m_DirectionMove;
					m_sX = m_sXTable[m_DirectionMoved];
					m_sY = m_sYTable[m_DirectionMoved];
					
					//????????????????????????????????????????????????????????
					//???                                                  ???
					//???   ÇÑ FrameÀ» ÀÌµ¿...ÇØ¾ßÇÏ´Â°¡?? ¸»¾Æ¾ß ÇÏ´Â°¡   ???
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
				
				// ´ÙÀ½¿¡ ÀÌµ¿ÇÒ count¸¦ ÁöÁ¤ÇÑ´Ù.
				// frame¸¸ ¹Ù²î´Â °æ¿ì¿Í
				// frameÀÌ ¹Ù²î°í ÀÌµ¿ÇÏ´Â °æ¿ì°¡ ÀÖ´Ù.
				m_NextMoveCount = (*g_pCreatureTable)[m_CreatureType].MoveRatio;				

				//------------------------------------------------
				// Ä³¸¯ÅÍÀÇ MoveAction¿¡ ¸Â´Â Sound¸¦ Ãâ·ÂÇØÁØ´Ù.
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
			// ¸ø ¿òÁ÷ÀÎ °æ¿ì
			else
			{
				// ¸ØÃã
				SetAction( ACTION_STAND );	
			}			
		}
		//--------------------------------------------
		// ¿òÁ÷ÀÏ ¼ö ¾øÀ» °æ¿ì
		//--------------------------------------------
	}
	// ¾ÆÁ÷ ´ÙÀ½ Sector±îÁö ´ú µµ´ÞÇÑ °æ¿ì
	else
	{
		// ÇÑ FrameÀ» ÀÌµ¿ÇÑ´Ù.
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

		// ÀÌµ¿ÇÒ count°¡ µÆÀ»¶§¸¸ ÀÌµ¿ÇÑ´Ù.
		if (m_MoveCount>=m_NextMoveCount)
		{			
			//--------------------------------------------
			// Fast Move ÀÎ °æ¿ì
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

				// ´ÙÀ½¿¡ ÀÌµ¿ÇÒ count¸¦ ÁöÁ¤ÇÑ´Ù.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				//----------------------------------------------------------
				// fast move ÈÄ¿¡ ÇÑ¹ø ¶§¸®±â.. - -
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
						// »ó´ë°¡ ¸Â´Â ¸ð½À
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

						// °á°ú Ç¥Çö
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
			// ÀÏ¹ÝÀûÀÎ ÀÌµ¿ÀÇ °æ¿ì
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

				// ´ÙÀ½¿¡ ÀÌµ¿ÇÒ count¸¦ ÁöÁ¤ÇÑ´Ù.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				m_MoveCount++;
			}
		
		}
		else
		{
			//m_ActionCount++;
			m_MoveCount++;
		}	


		// frameÀ» ¹Ù²ãÁØ´Ù. ¸¶Áö¸· Frame±îÁö °¬À¸¸é 0¹øÂ° FrameÀ¸·Î ¹Ù²Û´Ù.
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
			
				// ¼³Á¤ÀÌ ¾È µÅ ÀÖÀ¸¸é.. action µ¿ÀÛ¿¡ ¸ÂÃá´Ù.
				if (castingFrames==0)
				{
					int actionInfoAction = GetActionInfoAction(nUsedActionInfo);
						
					//-------------------------------------------------
					// ¿ÀÅä¹ÙÀÌ¸¦ Å¸°í ÀÖ´Â °æ¿ì
					//-------------------------------------------------				
					if (m_MoveDevice==MOVE_DEVICE_RIDE)
					{
						//castingFrames = (*g_pCreatureTable)[m_CreatureType].GetActionCount( actionInfoAction );					
						m_ActionCountMax = GetCreatureActionCountMax( this, actionInfoAction );
					}					
					else
					{	
						//-------------------------------------------------
						// ÈíÇ÷ µ¿ÀÛÁß¿¡ °ø°Ý¹ÞÀº °æ¿ì
						//-------------------------------------------------
						if (actionInfoAction==ACTION_DAMAGED
							//&& (// ÈíÇ÷ ´çÇÏ´Â µ¿ÀÛ ÁßÀÌ°Å³ª 
								//m_Action==ACTION_DRAINED
								// ¹ìÆÄÀÌ¾îÀÏ¶§, ÈíÇ÷ÇÏ´Â µ¿ÀÛ Áß¿¡´Â damaged¸¦ ¾È º¸¿©ÁØ´Ù.
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

							// ÀÓ½Ã·Î..
							// ¿©±â¼­ À½¼ö°¡ ³ª¿À´Â °æ¿ì.. ¾îÄÉ ÇÏÁö.. - -;
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
				// castingÀÌ ´©±¸ÇÑÅ× ºÙ´Â°¡?
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
					// Creature¿¡°Ô ºÙÀÌ´Â Effect¸¦ »ý¼ºÇØ¼­ pointer¸¦ ³Ñ°Ü¹Þ´Â´Ù.
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

						// ºÙ¾î¾ß ÇÏ´Â Ä³¸¯ÅÍ
						pEffect->SetAttachCreatureID( targetCreature->GetID() );
						
						#ifdef OUTPUT_DEBUG
						
							if (m_MoveDevice==MOVE_DEVICE_RIDE)
							{
								
							}
							else
							{	
								//-------------------------------------------------
								// ÈíÇ÷ µ¿ÀÛÁß¿¡ °ø°Ý¹ÞÀº °æ¿ì
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
			// Casting ActionInfo Ç¥Çö
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
			// casting µ¿ÀÛ¿¡ ¸Â´Â sound
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
	// »ç¿ë ±â¼úÀÌ Á¤ÇØÁ® ÀÖ´Â °æ¿ì..
	//-------------------------------------------------------------
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND || IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND);
		int		actionCountMax_1 = GetActionCountMax()-1;
		BOOL	bStartAction	= (m_ActionCount==0);
		BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
		int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

		// Effect°¡ ½ÃÀÛµÇ´Â °æ¿ì´Â..
		// (1) StartFrameÀÎ °æ¿ì
		// (2) ¸¶Áö¸· ActionFrameÀÎ °æ¿ì
		BOOL bStartEffect = m_ActionCount==StartFrame || 
							StartFrame >= GetActionCountMax() && bEndAction;

		bStartAction |= (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
					&& m_RepeatCount!=0
					&& m_ActionCount == GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

			
		//BOOL	bCastingEffect	= GetActionInfoCastingStartFrame(m_nUsedActionInfo)==m_ActionCount;

		//-------------------------------------------------------------
		// ½ÃÀÛ µ¿ÀÛ
		//-------------------------------------------------------------
		if (bStartAction)
		{
			//------------------------------------------------
			// ±â¼úÀÇ µ¿ÀÛ¿¡ ¸Â´Â sound¸¦ Ãâ·ÂÇØÁØ´Ù.
			//------------------------------------------------
			//g_Sound.Play( g_SoundTable[(*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID()].pDSBuffer );
			//PlaySound( (*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID() ,
			//			false,
			//			m_X, m_Y);

			//------------------------------------------------
			// Ä³¸¯ÅÍÀÇ Action¿¡ ¸Â´Â Sound¸¦ Ãâ·ÂÇØÁØ´Ù.
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
		// Á¤Áö µ¿ÀÛÀÌ¸é..
		// ¹Ù·Î effect¸¦ º¸¿©ÁØ´Ù.
		//-------------------------------------------------------------
		if (bActionStand)
		{
			m_ActionCount = GetActionCountMax()-1;
		}

		//-------------------------------------------------------------
		// ±â¼úÀ» º¸¿©Áà¾ß ÇÏ´Â °æ¿ì
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
			// Action³¡
			//--------------------------------------------------------	
			//m_Action			= ACTION_STAND;
		}
	}

	m_ActionCount++;

	//--------------------------------------------------------	
	// ÀÓ½Ã·Î ÈíÇ÷µ¿ÀÛ ÁßÁöÇÏ´Â°Å..
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
// ±â¼ú »ç¿ëÇÑ°É Ç¥ÇöÇÑ´Ù.
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
		// Casting ActionInfoÀÎ°¡?
		//--------------------------------------------------------
		BOOL bCastingAction = (*g_pActionInfoTable)[nUsedActionInfo].IsCastingAction();
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);


		//--------------------------------------------------------
		// ¸ñÇ¥ À§Ä¡ PixelÁÂÇ¥
		//--------------------------------------------------------
		point = MTopView::MapToPixel(m_TraceX, m_TraceY);
		//point.x += m_sX;
		//point.y += m_sY;
		//point.x += TILE_X_HALF;

		//--------------------------------------------------------
		//
		//					Effect ¸ñÇ¥ ¼³Á¤
		//
		//--------------------------------------------------------
		MEffectTarget* pEffectTarget = NULL;

		//--------------------------------------------------------
		// casting actionÀÎ °æ¿ì
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

			// castingAction¿¡¼­´Â °á°úÃ³¸®°¡ ÇÊ¿ä¾ø´Ù.
		}
		//--------------------------------------------------------
		// casting actionÀÌ ¾Æ´Ñ.. ½ÇÁ¦ ±â¼úÀÎ °æ¿ì..
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
			// Áö¼Ó ½Ã°£ ¼³Á¤
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
			// °á°ú ¼³Á¤ : °á°ú Ã³¸®´Â EffectGenerator¿¡ ¸Ã±ä´Ù.
			pEffectTarget->SetResult( m_pActionResult );
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			
			// ¼³Á¤µÈ °á°ú¸¦ ¾ø°Ô ÇÑ´Ù.
			m_pActionResult = NULL;
		}

		//--------------------------------------------------------
		//
		//					½ÃÀÛ À§Ä¡¸¦ °áÁ¤ÇÑ´Ù.
		//
		//--------------------------------------------------------
		int x,y,z, direction;

		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		//--------------------------------------------------------
		// User À§Ä¡¿¡¼­ ½ÃÀÛÇÏ´Â °æ¿ì
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
		// Target À§Ä¡¿¡¼­ ½ÃÀÛÇÏ´Â °æ¿ì
		//--------------------------------------------------------
		else if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartTarget())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			x			= point.x;
			y			= point.y;
			z			= m_Z;//+60;			
		}

		//--------------------------------------------------------
		// °øÁß¿¡¼­ ½ÃÀÛÇÏ´Â °æ¿ì
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartSky())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			z	= m_Z + (*g_pActionInfoTable)[nUsedActionInfo].GetValue();

			direction	= DIRECTION_DOWN;
		}
		//--------------------------------------------------------
		// Áö»ó¿¡¼­ ½ÃÀÛÇÏ´Â °æ¿ì
		//--------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			direction	= m_Direction;
		}

		//--------------------------------------------------------
		//
		//                   Effect»ý¼º		
		//
		//--------------------------------------------------------
		
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		(*g_pEffectGeneratorTable).Generate(
				x,y,z,				// ½ÃÀÛ À§Ä¡
				direction, 		// ¹æÇâ
		//		1,					// power
				SpecialID ? bMonster ? 2 : 1 : 1,
				nUsedActionInfo,		//	ActionInfoTableÁ¾·ù,
				pEffectTarget,		// ¸ñÇ¥ Á¤º¸
				GetActionGrade()	
		);
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		ClearActionGrade();		

		DEBUG_ADD_FORMAT("[Creature::AffectUsedActionInfo] ai=%d, id=%d", nUsedActionInfo, m_ID);		
	}
	else
	{
		//------------------------------------------------------------
		// °á°ú¸¦ Ã³¸®ÇØ¾ßÇÏ´Â ½ÃÁ¡ÀÎ°¡? - ´ç¿¬ÇÏ´Ù°í º»´Ù *_*;
		//------------------------------------------------------------				
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		if (m_pActionResult != NULL)
		{					
			// (!) m_pActionResult°ªÀÌ Execute¿¡¼­ º¯ÇÒ ¼ö ÀÖ¾î¼­ 
			//		ÀúÀåÇß´Ù°¡ Áö¿öÁà¾ß ÇÑ´Ù.
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
	// Action³¡
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
// NextUsedActionInfo°¡ ÀÖ´Ù¸é
// UsedActionInfo --> NextUsedActionInfo¸¦ ¸ðµÎ Àû¿ëÇÑ´Ù.
//
// UsedActionInfo¸¸ ÀÖ´Â °æ¿ì´Â Ã³¸® ¾ÈÇÑ´Ù.
//----------------------------------------------------------------------
void
MCreature::AffectBufferedActionInfo()
{
	DEBUG_ADD_FORMAT("[AffectBufferedActionInfo] ai=%d, nai=%d, action=%d-%d/%d", m_nUsedActionInfo, m_nNextUsedActionInfo, (int)m_Action, (int)m_ActionCount, (int)GetActionCountMax());
	
	//--------------------------------------------------------
	// ´ÙÀ½¿¡ »ç¿ëÇÒ·Á´Â ±â¼úÀÌ ÀÖ´Ù¸é... ¹Ù·Î Àû¿ë ½ÃÅ²´Ù.
	//--------------------------------------------------------
	if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
	{	
		TYPE_ACTIONINFO nextActionInfo = m_nNextUsedActionInfo;
		DEBUG_ADD("[AffectBufferedActionInfo] Into Here");
		
		//--------------------------------------------------------
		// ÇöÀç »ç¿ëÇØ¾ßÇÒ ±â¼úÀÌ ÀÖ´Â °æ¿ì 
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			DEBUG_ADD("[AffectBufferedActionInfo] AttachCastingEffect" );
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
		
			// m_nUsedActionInfo Àû¿ë
			// --> ³»ºÎ¿¡¼­ m_nNextUsedActionInfo°¡ ¹Ù²ð ¼ö ÀÖ´Ù.

	// ÀÌ °æ¿ì´Â ¹«Á¶°Ç(!) CastingEffect¸¦ ºÙ¿©¾ß ÇÑ´Ù.
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

			// affectÇÏ°í ³ª¼­.. »ç¿ëÇØ¾ßÇÒ ±â¼úÀÌ »ý±ä´Ù¸é..
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
		// ´ÙÀ½¿¡ »ç¿ëÇØ¾ßÇÒ ±â¼ú Àû¿ë
		//--------------------------------------------------------
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		// --> ³»ºÎ¿¡¼­ m_nNextUsedActionInfo°¡ ¹Ù²ð ¼ö ÀÖ´Ù.
		DEBUG_ADD("~~~~~zzzz");
		AttachCastingEffect( nextActionInfo, TRUE );
		DEBUG_ADD("~~~~~zzzz2");
		AffectUsedActionInfo( nextActionInfo );
			
		// affectÇÏ°í ³ª¼­.. »ç¿ëÇØ¾ßÇÒ ±â¼úÀÌ »ý±ä´Ù¸é..
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
	// ÇöÀç »ç¿ëÇØ¾ßÇÒ ±â¼ú¸¸ ÀÖ´Â °æ¿ìÀÎ°¡?
	//--------------------------------------------------------
	else if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		DEBUG_ADD("[AFFE] USEDACTIONINFO != NULL ");
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
	
		// m_nUsedActionInfo Àû¿ë
		// --> ³»ºÎ¿¡¼­ m_nNextUsedActionInfo°¡ ¹Ù²ð ¼ö ÀÖ´Ù.

		// ÀÌ °æ¿ì´Â ¹«Á¶°Ç(!) CastingEffect¸¦ ºÙ¿©¾ß ÇÑ´Ù.
		DEBUG_ADD("[AFFE] CASTING EFF");
		AttachCastingEffect( m_nUsedActionInfo, TRUE );
		DEBUG_ADD("[AFFE] AFFECT USED");
		AffectUsedActionInfo( m_nUsedActionInfo );

		m_nUsedActionInfo	= ACTIONINFO_NULL;			

		// affectÇÏ°í ³ª¼­.. »ç¿ëÇØ¾ßÇÒ ±â¼úÀÌ »ý±ä´Ù¸é..
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{			
			DEBUG_ADD("[AFFE] ZZZZZZ ");
			AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
			AffectUsedActionInfo( m_nNextUsedActionInfo );
		}	

		//--------------------------------------------------------
		// ´ÙÀ½¿¡ »ç¿ëÇØ¾ßÇÒ ±â¼ú Àû¿ë
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
// amount¸¸Å­ times¹ø¾¿ delay¸¶´Ù È¸º¹ÇÑ´Ù.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryHP(int amount, int times, DWORD delay)
{ 
	if (times > 0)
	{
		// ¹Ù·Î ÇÑ¹ø È¸º¹

		if(! IsRecoveryHP() )					// ÇöÀç HP °¡ Â÷°í ÀÖÁö ¾ÊÀ¸¸é
		{
			SetStatus( MODIFY_CURRENT_HP, GetHP()+amount );
			m_RecoveryHPTimes = times - 1; 
		} else
			m_RecoveryHPTimes = times;

		m_RecoveryHPAmount = amount; 
		m_RecoveryHPDelayTime = delay;
		//m_RecoveryHPPart = MODIFY_CURRENT_HP;

		// ´ÙÀ½ È¸º¹ÇÒ ½Ã°£ ¼³Á¤
		m_RecoveryHPNextTime = g_CurrentTime + m_RecoveryHPDelayTime;
	}
}

//----------------------------------------------------------------------
// Set Recovery
//----------------------------------------------------------------------
// amount¸¸Å­ times¹ø¾¿ delay¸¶´Ù È¸º¹ÇÑ´Ù.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryMP(int amount, int times, DWORD delay)
{ 
	// 2004, 9, 3, sobeit add start - ÇÃ·¹Á® ½ºÅ×ÀÌ¼Ç¿¡ ¸ÂÀ¸¸é MP°¡ ¾ÈÂù´Ù...-_-
	if(HasEffectStatus(EFFECTSTATUS_PLEASURE_EXPLOSION))
		return;
	// 2004, 9, 3, sobeit add end
	if (times > 0)
	{
		// ¹Ù·Î ÇÑ¹ø È¸º¹
		if(! IsRecoveryMP() )
		{
			SetStatus( MODIFY_CURRENT_MP, GetMP()+amount );
			m_RecoveryMPTimes = times-1; 
		} else
			m_RecoveryMPTimes = times; 

		m_RecoveryMPAmount = amount; 		
		m_RecoveryMPDelayTime = delay;
		//m_RecoveryMPPart = MODIFY_CURRENT_MP;

		// ´ÙÀ½ È¸º¹ÇÒ ½Ã°£ ¼³Á¤
		m_RecoveryMPNextTime = g_CurrentTime + m_RecoveryMPDelayTime;
	}
}

//----------------------------------------------------------------------
// Check Drop Blood
//----------------------------------------------------------------------
// HP ³·À» ¶§.. ÇÇ Èê¸®±â
//----------------------------------------------------------------------
void				
MCreature::CheckDropBlood()
{
	if (!HasEffectStatus(EFFECTSTATUS_COMA))
	{
		//----------------------------------------------------------
		// ÇÇ Èê¸± ½Ã°£ÀÌ µÇ¾ú´ÂÁö(-_-;) Ã¼Å©..
		//----------------------------------------------------------
		if (g_pUserOption->BloodDrop 
			&& g_CurrentTime > m_NextBloodingTime)
		{
			int currentHP = m_Status[MODIFY_CURRENT_HP];
			int maxHP = m_Status[MODIFY_MAX_HP];

			int percentHP = ((maxHP==0)? 0 : currentHP*100 / maxHP);

			//----------------------------------------------------------
			// HP 30% ÀÌÇÏÀÏ¶§ ÇÇ Èê¸°´Ù.
			//----------------------------------------------------------
			if (percentHP <= g_pClientConfig->BLOOD_DROP_HP_PERCENT)
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_DROP_GROUND,										// »ç¿ë ±â¼ú ¹øÈ£
						
							m_X, m_Y, 0,
							(int)m_Direction,														// »ç¿ë ¹æÇâ
							
							m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
							m_X, m_Y, 0,
							
							80, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}

			//----------------------------------------------------------
			// ´ÙÀ½ ÇÇ Èê¸®±â °¡´ÉÇÑ ½Ã°£ ¼³Á¤. 
			// Á» ´Ê°Ô Ã¼Å©ÇØµµ °ü°è¾øÀ¸¹Ç·Î ÇÇ Èê¸®°Å³ª ¸»°Å³ª..½Ã°£ È®Àå
			//----------------------------------------------------------
			DWORD timeGap = 300 //+ 1000	//g_pClientConfig->BLOOD_DROP_GAP_TIME 
							+ (percentHP<<2) * rand()%20;//g_pClientConfig->BLOOD_DROP_RANDOM_TIME;

			//----------------------------------------------------------
			// ÈíÇ÷ ´çÇÑ »óÅÂÀÌ¸é ÇÇ¸¦ Á» ´ú Èê¸°´Ù.
			//----------------------------------------------------------
			if (HasEffectStatus(EFFECTSTATUS_BLOOD_DRAIN))
			{
				// 5¹è Á¤µµ ´ú Èê¸°´Ù.
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

	// [»õ±â¼ú3] -_-; virtual·Î ÇÏ´Â°Ô ³ªÀ»ÅÙµ¥.. 
	if (GetClassType()==CLASS_PLAYER)
	{		
		g_pSkillAvailable->SetAvailableSkills();
	}

	// [»õ±â¼ú7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
	
	CheckRegen();

	if(g_pPlayer == this)	// PlayerÀÎ °æ¿ì °ü¼ÒÈ¯ Áß¿¡´Â ¿Ê ¸ø°¥¾Æ ÀÔ´Â´Ù.
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

	// [»õ±â¼ú7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = false;
	
	CheckRegen();

	if(g_pPlayer == this)	// PlayerÀÎ °æ¿ì °ü¼ÒÈ¯ Áß¿¡´Â ¿Ê ¸ø°¥¾Æ ÀÔ´Â°É Ç¬´Ù.
	{
		UI_UnlockGear();;
	}
}

//----------------------------------------------------------------------
// Add Casket
//----------------------------------------------------------------------
// °üÀÌ ¼­¼­È÷ »ý±ä´Ù.
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
//	// [»õ±â¼ú3] -_-; virtual·Î ÇÏ´Â°Ô ³ªÀ»ÅÙµ¥.. 
//	if (GetClassType()==CLASS_PLAYER)
//	{		
//		g_pSkillAvailable->SetAvailableSkills();
//	}
//
//	// [»õ±â¼ú7]
//	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
//	
//	CheckRegen();
}

//----------------------------------------------------------------------
// Remove Casket
//----------------------------------------------------------------------
// °üÀÌ ¼­¼­È÷ »ç¶óÁø´Ù.
//----------------------------------------------------------------------
void				
MCreature::RemoveCasket()
{
	RemoveCasketSoon();
	m_bInCasket = true;		// 64µÇ¸é false·Î ¹Ù²ã¾ß ÇÑ´Ù.
	m_CasketCount = 0;
	m_CasketCountInc = 2;

	//SetVisible();

//	// [»õ±â¼ú7]
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

			// [»õ±â¼ú3] - -;;
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
// ÀÏ´ÜÀº.. HP / MP º¯È­
//----------------------------------------------------------------------
void
MCreature::UpdateStatus()
{
	//--------------------------------------------------------
	// coma »óÅÂ¿¡¼­´Â HP¸¦ ¿Ã¸®Áö ¾Ê´Â´Ù.
	//--------------------------------------------------------
	if (!m_bAlive 
		|| HasEffectStatus(EFFECTSTATUS_COMA))
	{
		return;
	}

	bool bChangedHP = false;

	//--------------------------------------------------------
	// HP È¸º¹
	//--------------------------------------------------------
	if (m_RecoveryHPTimes > 0 
		&& g_CurrentTime >= m_RecoveryHPNextTime)
	{		
		m_RecoveryHPTimes--;

		// È¸º¹
		SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RecoveryHPAmount );

		bChangedHP = true;
		
		// ´ÙÀ½ È¸º¹ÇÒ ½Ã°£ ¼³Á¤
		m_RecoveryHPNextTime += m_RecoveryHPDelayTime;		
	}

	//--------------------------------------------------------
	// MP È¸º¹
	//--------------------------------------------------------
	if (m_RecoveryMPTimes > 0 
		&& g_CurrentTime >= m_RecoveryMPNextTime)
	{		
		m_RecoveryMPTimes--;

		// È¸º¹
		SetStatus( MODIFY_CURRENT_MP, GetStatus(MODIFY_CURRENT_MP)+m_RecoveryMPAmount );

		// ´ÙÀ½ È¸º¹ÇÒ ½Ã°£ ¼³Á¤
		m_RecoveryMPNextTime += m_RecoveryMPDelayTime;		
	}

	//--------------------------------------------------------
	// ¹ìÆÄÀÌ¾îÀÎ °æ¿ì ÀÚµ¿ È¸º¹
	//--------------------------------------------------------
	if (m_RegenAmount > 0)
	{
		//--------------------------------------------------------
		// ±âº»ÀûÀÎ È¸º¹
		//--------------------------------------------------------
		if (g_CurrentTime >= m_RegenNextTime)
		{		
			// [»õ±â¼ú4] mephisto °É¸®¸é HP regen ¾ÈµÈ´Ù.

			//if ( !m_bEffectStatus[EFFECTSTATUS_MEPHISTO] || !m_bEffectStatus[EFFECTSTATUS_MEPHISTO]&&IsInCasket() )
			{
				// °ü¼Ó¿¡ ÀÖÀ» °æ¿ì Àºµ¥¹ÌÁöºÎÅÍ È¸º¹ÇÑ´Ù.
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
			// ºÎ°¡ÀûÀÎ È¸º¹ - -;
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
	// ÆÄÆ¼ÀÎ °æ¿ì´Â HP ´Ù½Ã ¼³Á¤ÇØÁØ´Ù.
	//--------------------------------------------------------
	if (bChangedHP)
	{	
		if (IsPlayerParty())
		{
			PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
				
			// ÁÂÇ¥¸¦ ¼öÁ¤ÇØÁØ´Ù.
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
// ´ÙÀ½ µ¿ÀÛÀ» ÃëÇÑ´Ù.
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
	// Ã¤ÆÃ String ¾îµÓ°Ô ÇÒ ½Ã°£
	//--------------------------------------------------------
	if (m_NextChatFadeTime < g_CurrentTime)
	{
		FadeChatString();		
	}
	
	//--------------------------------------------------------
	// Status »óÅÂ º¯È­	- HP, MP º¯È­
	//--------------------------------------------------------
	UpdateStatus();
	
//	// 2004, 08, 05 sobeit add start
//	if( GetCreatureType() == 726 ||  GetCreatureType() == 727 || GetCreatureType() == 728 || GetCreatureType() == 729) // ¼º¹®
//		return; // ¹ØÀ¸·Î ³»·Á°¡ ºÁ¾ß º°·Î ÇÒ°Ô ¾ø´Ù.
//	// 2004, 08, 05 sobeit add end

	//--------------------------------------------------------
	// Æ¯¼öÇÑ È¿°úµé
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
	// ¹æÇâ ÀüÈ¯À» ÀÚ¿¬½º·´°Ô
	//--------------------------------------------------------
	if (!ChangeNearDirection())
	{
			//--------------------------------------------------------
			// ¹æÇâÀ» ¹Ù²Ü ÇÊ¿ä°¡ ¾ø´ø °æ¿ì¿¡..
			// ½É½ÉÇÒ¶§¸¶´Ù ÇÑ¹ø¾¿ ¹æÇâ ¹Ù²ãÁÖ±â.. - -;
			//--------------------------------------------------------			
			if (// Player°¡ ¾Æ´Ï°í
//				m_CreatureType >= 4
				!IsCreatureTypeVampirePC(m_CreatureType)
				&& m_CreatureType != CREATURETYPE_SLAYER_OPERATOR
				&& m_CreatureType != CREATURETYPE_VAMPIRE_OPERATOR
				&& m_CreatureType != CREATURETYPE_OUSTERS_OPERATOR
				&& m_CreatureType != CREATURETYPE_SLAYER_MALE
				&& m_CreatureType != CREATURETYPE_SLAYER_FEMALE
				&& m_CreatureType != CREATURETYPE_OUSTERS
				// °øÁß¹ÙÅä¸®°¡ ¾Æ´Ï°í
				&& m_CreatureType != 431
				// NPC°¡ ¾Æ´Ï°í... (¹æÇâº°·Î ±×¸²ÀÌ ´Ù ¾ø¾î¼­¸® -_-;)
				&& !IsNPC()
				// »ì¾Æ ÀÖ°í..
				&& m_bAlive
				// Á¤Áö»óÅÂÀÌ°í
				&& m_Action==ACTION_STAND
				// ¿òÁ÷ÀÏ°÷ÀÌ ¾ø°í
				&& m_listMoveBuffer.size()==0
				// Á¤Áöµ¿ÀÛÀÇ ³¡¿¡..
				&& m_ActionCount>=GetActionCountMax()-1
				// randomÇÏ°Ô.. - -;
				&& (rand() % 5)==0
				&& !HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
				)
			{
				m_Direction = ((int)m_Direction + 8 + rand()%5-2) % 8;
			}
	}
	//m_CurrentDirection = m_Direction;

	//--------------------------------------------------------
	// ¹«½¼ effect°¡ °É·ÁÀÖ´Ù¸é 2¹è ´À¸®°Ô ¿òÁ÷ÀÎ´Ù.
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
	// KnockBack Ã³¸® 2001.10.9
	//--------------------------------------------------------
	if (m_bKnockBack > 0)
	{
		m_sX += m_cX;
		m_sY += m_cY;

		// knockBack ´Ùx µÆÀ¸¸é Á¤Áöµ¿ÀÛ.
		if (--m_bKnockBack==0)
		{
			m_sX = 0;
			m_sY = 0;

			SetAction( ACTION_STAND );

			return;
		}			
	}
	//--------------------------------------------------------
	// FastMoveÁß¿¡´Â actionÀ» ÃëÇÏÁö ¾Ê´Â´Ù. 
	//--------------------------------------------------------
	else if (!m_bFastMove)
	{
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{	
			int bOldNextAction = m_NextAction;

			// affectUsedActionInfo¸¦ ½ÇÇàÇÏ°í ³ª¸é
			// NextActionInfo°¡ ¹Ù²ð ¼ö°¡ ÀÖ´Ù.			
			TYPE_ACTIONINFO nextUsedActionInfo = m_nNextUsedActionInfo;
			TYPE_ACTIONINFO nextNextUsedActionInfo = ACTIONINFO_NULL;

			//--------------------------------------------------------
			// ÀÌÀü¿¡ »ç¿ë ÁßÀÎ ±â¼úÀÌ ÀÖ´Ù¸é... ¹Ù·Î Àû¿ë ½ÃÅ²´Ù.
			//--------------------------------------------------------
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{	
				m_nNextUsedActionInfo = ACTIONINFO_NULL;

				// castingEffect°¡ ¾ÆÁ÷ Ãâ·Â ¾ÈµÆÀ¸¸é Ãâ·Â½ÃÅ²´Ù.
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

			// 2001.9.30ÀÏ Ãß°¡
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				int actionInfoAction = GetActionInfoAction(m_nUsedActionInfo);

				// ¸Â´Â µ¿ÀÛÀÌ¸é.. ÇÇ Æ¢±è..
				// 2002.3.19 ÁÖ¼® Á¦°Å.. ¾Æ¹« µ¿ÀÛÀÌ³ª ÀÏ´Ü ÀÌÆåÆ® º¸¿©ÁÖÀÚ
				//if (actionInfoAction==ACTION_DAMAGED)
				{
					if //(m_Action!=ACTION_STAND 
						//&& (!IsSlayer() || IsSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
						//&& 
						(GetActionInfoCastingStartFrame(m_nUsedActionInfo) <= m_ActionCount)
					{
						AttachCastingEffect( m_nUsedActionInfo, TRUE );

						AffectUsedActionInfo( m_nUsedActionInfo );
				
						// ¼Ò¸® Ãâ·Â
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

				// 2001.05.21 Ãß°¡
				// ±â¼ú µ¿ÀÛ¿¡¼­ ACTION_STAND´Â º¸¿©ÁÖÁö ¾Ê´Â´Ù.
				if (actionInfoAction!=ACTION_STAND)
				{
					SetAction( actionInfoAction );
				}
			}				

			//--------------------------------------------------------
			// ÀÌµ¿ÇÒ·Á´Ù°¡ ¸Â°Å³ªÇØ¼­ ´Ù¸¥ actionÀ» ½ÇÇàÇÏ°Ô µÇ´Â °æ¿ì
			// ÀÌµ¿ÀÌ ¾ÈµÇ´Â °æ¿ì°¡ ÀÖ¾ú´Ù.
			// ´ëÃ¥ --> NextActionÀ» ±â¾ïÇß´Ù°¡ ÀÌµ¿ÀÌ¸é.. ÀÌµ¿½ÃÅ°±â
			// 2001.9.24
			//--------------------------------------------------------
			if (bOldNextAction==m_MoveAction)
			{
				SetNextAction( m_MoveAction );
			}	 			
		}
		//--------------------------------------------------------
		// ±â¾ïµÈ ´ÙÀ½ Çàµ¿... 
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
	// ¸¶ºñ µÆÀ» ¶§..
	//--------------------------------------------------------
	if(HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS) && !IsSlayer())
	{
		//--------------------------------------------------------
		// ±â¼úÀ» »ç¿ëÇÏ·Á´Â °æ¿ì
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
		// ±â¼úÀ» »ç¿ëÇÏ·Á´Â °æ¿ì
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}

		if (m_ActionCount>=GetActionCountMax())
		{			
			//--------------------------------------------------------
			// ¸ñÇ¥ Å¸ÀÏ¿¡ µµÂøÇÑ »óÅÂ¿¡¼­..
			// °è¼Ó ¼­ ÀÖ°Å³ª.. ´Ù °ÉÀº °æ¿ì´Â.. 
			// Á¦ÀÚ¸®¿¡¼­ Èçµé°Å¸®´Â ¸ð½ÀÀ» Ç¥ÇöÇØÁØ´Ù.
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
	// ÀÏ¹ÝÀûÀÎ °æ¿ì
	//--------------------------------------------------------
	else
	{
		//--------------------------------------------------------
		// ÇÇ Èê¸®´Â°Å Ã¼Å©
		//--------------------------------------------------------
		CheckDropBlood();

		//--------------------------------------------------------
		// Action Ã³¸®
		//--------------------------------------------------------
		if (m_ActionCount < GetActionCountMax())
		{		
			//--------------------------------------------------------
			// ±â¼úÀ» »ç¿ëÇÏ·Á´Â °æ¿ì
			//--------------------------------------------------------
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				ActionEffect();
			}
			//--------------------------------------------------------
			// Çàµ¿..
			//--------------------------------------------------------
			else
			{
				// ÀûÀýÇÑ Action ¼öÇà
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
						// °È´Â µ¿ÀÛÀº µû·Î Ã³¸®ÇÑ´Ù.
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
		// »ì¾Æ ÀÖ´Â °æ¿ì¸¸
		//--------------------------------------------------------
		if (m_bAlive)
		{
			//--------------------------------------------------------
			// ActionÀÌ ³¡³­ °æ¿ì...
			//--------------------------------------------------------
			if (m_ActionCount>=GetActionCountMax())
			{			
				//--------------------------------------------------------
				// ¸ñÇ¥ Å¸ÀÏ¿¡ µµÂøÇÑ »óÅÂ¿¡¼­..
				// °è¼Ó ¼­ ÀÖ°Å³ª.. ´Ù °ÉÀº °æ¿ì´Â.. 
				// Á¦ÀÚ¸®¿¡¼­ Èçµé°Å¸®´Â ¸ð½ÀÀ» Ç¥ÇöÇØÁØ´Ù.
				//--------------------------------------------------------
				if (m_MoveCount>=m_MoveCountMax)
				{		
					//#ifdef CONNECT_SERVER
					//--------------------------------------------------------
					// ´ÙÀ½ ÀÌµ¿ÀÌ ¾ø´Ù¸é.. Á¤Áö½ÃÅ²´Ù.
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
				// °È´Ù°¡ ´Ù¸¥ actionÀ» º¸¿©Áá°Å³ª °è¼Ó °È´ø ÁßÀÌ´Ù.
				// ´Ù½Ã °È´Â´Ù.
				//--------------------------------------------------------
				else 
				{			
					m_Action = m_MoveAction;				
					
					ActionMove();

					if (m_MoveCount>=m_MoveCountMax)
					{
						// ´ÙÀ½ ÀÌµ¿ÇÒ °÷ÀÌ ÀÖÀ¸¸é..
						if (m_NextX != SECTORPOSITION_NULL
							&& m_NextDirection != DIRECTION_NULL)
						{		
							MoveNextPosition();
						}
						else
						{
							//#ifdef CONNECT_SERVER

							//--------------------------------------------------------
							// ´ÙÀ½ ÀÌµ¿
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
			// ActionÀ» º¸¿©ÁÖ´Â ÁßÀÌ¶óµµ.. 
			// ÀÌµ¿ÇÒ ²¨¸®?°¡ ÀÖÀ¸¸é ÀÌµ¿µµ ½ÃÄÑÁØ´Ù. 2001.10.10
			//--------------------------------------------------------
			else
			{		
				/*
				// 2001.11.8 - ÁÖ¼®Ã³¸®.. º¸±â ¾È ÁÁ´Ù³ª.. - -;;
				if (// KnockBackÀÌ³ª FastMove°¡ ¾Æ´Ñ °æ¿ì
					!m_bFastMove && m_bKnockBack==0	&& m_MoveCount>0)
				{
					if (m_MoveCount<m_MoveCountMax)
					{
						ActionMove();
					}
					else if (m_MoveCount>=m_MoveCountMax)
					{
						// ´ÙÀ½ ÀÌµ¿ÇÒ °÷ÀÌ ÀÖÀ¸¸é..
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
// this Creature´Â Move¸¦ ¹Þ¾ÒÀ¸¹Ç·Î
//
// //0. Player¿ÍÀÇ ÁÂÇ¥ Ãæµ¹À» °í·ÁÇØ¼­ ¿òÁ÷¿©ÁØ´Ù.
//
// (x,y)¿¡¼­ d¹æÇâÀ¸·Î ÇÑ Ä­ ¿òÁ÷ÀÎ´Ù.
//----------------------------------------------------------------------
void		
MCreature::PacketMove(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE direction)
{
//	// 2004, 9, 16, sobeit add start - ÅÍ·¿ ¹æÇâ ¹Ù²Ù±â, »¬±î..¤Ñ¤Ñ;
//	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
//	{
//		SetCurrentDirection(direction);
//		return;
//	}
//	// 2004, 9, 16, sobeit add end
	//---------------------------------------------------
	// »õ·Î¿î ÀÌµ¿ Á¤º¸¸¦ buffer¿¡ Ãß°¡½ÃÅ²´Ù.
	//---------------------------------------------------
	MoveNode* pNode = new MoveNode;
	pNode->x = x;
	pNode->y = y;
	pNode->direction = direction;

	if (IsPlayerParty())
	{
		PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );		
			
		// ÁÂÇ¥¸¦ ¼öÁ¤ÇØÁØ´Ù.
		if (pInfo!=NULL)
		{
			pInfo->zoneID = (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);
			pInfo->zoneX = x;
			pInfo->zoneY = y;
		}
	}

	//---------------------------------------------------
	// ServerÁÂÇ¥´Â ¹Ù·Î Àû¿ë½ÃÅ²´Ù.
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

	// serverÁÂÇ¥ ±â¾ï
	// 2001.8.6
	// 2002.3.22 ´Ù½Ã »ì·È´Ù.
	//m_ServerX	= next.x;
	//m_ServerY	= next.y;
	
	SetServerPositionWithBlock( next.x, next.y );
	//*/

	m_listMoveBuffer.push_back( pNode );

	//-----------------------------------------------------------
	// Á¤ÁöµÈ »óÅÂÀÌ¸é ¹Ù·Î Àû¿ëÇÑ´Ù.
	//-----------------------------------------------------------
	if (m_Action==ACTION_STAND 
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		AffectMoveBuffer();
	}
	else
	{
		//---------------------------------------------------
		// ÇÑ°è¸¦ ³Ñ´Â MoveBuffer´Â ¸ðµÎ Àû¿ë½ÃÄÑ¹ö¸°´Ù.
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
// ±âÁ¸ÀÇ MoveBuffer¸¦ ´Ù Áö¿ò
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
// ¸ðµç MoveBuffer¸¦ Àû¿ë½ÃÅ²´Ù.
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
// BufferingµÈ ´ÙÀ½ÀÇ ÀÌµ¿ Á¤º¸¸¦ ÇÏ³ª ¼³Á¤ÇÑ´Ù.
//----------------------------------------------------------------------
bool	
MCreature::AffectMoveBuffer()
{
	//-------------------------------------------
	// ÀÌµ¿ÇÒ Á¤º¸°¡ ¾ø´Â °æ¿ì
	//-------------------------------------------
	if (m_listMoveBuffer.size()==0)
	{
		return false;
	}

	//-------------------------------------------
	// Á© ¾Õ¿¡²¬ ÀÐ¾î¿Â´Ù.
	//-------------------------------------------
	MoveNode* pNode = m_listMoveBuffer.front();
	m_listMoveBuffer.pop_front();

	//-------------------------------------------
	// °ª ¼³Á¤
	//-------------------------------------------
	// ¿ø·¡´Â PacketMove()¿¡ ÀÖ´ø°Çµ¥
	// move buffering Ãß°¡ÇÏ¸é¼­ cut & paste - -;;
	// Àß µ¹¾Æ°¥·Á³ª..
	//-------------------------------------------
	int x = pNode->x;
	int y = pNode->y;
	int direction = pNode->direction;

	delete pNode;

	// Á×Àº °æ¿ì
	if (!m_bAlive)
	{
		return false;
	}


	DEBUG_ADD_FORMAT("AffectMoveBuffer : [ID=%d] From(%d,%d) Direction(%d)", m_ID, x,y, direction);

	// ÀÌÀü ÁÂÇ¥¸¦ ±â¾ïÇØµÐ´Ù.		
	int oldX = m_X;
	int oldY = m_Y;

	// ÀÓ½Ã·Î.. »õ ÁÂÇ¥¸¦ ¼³Á¤
	m_X = x;
	m_Y = y;	// m_X=x; m_Y=y;  and.. Etc...
		
	// Server¿¡¼­ ³¯¾Æ¿Â ¹æÇâÀ¸·Î ¿òÁ÷ÀÎ ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	POINT next;
	GetNextPosition( direction, next );

	// ¿µ¿ª ³Ñ¾î°¡´ÂÁö Ã¼Å© . 2001.10.7
	if (next.x < 0 
		|| next.y < 0 
		|| next.x >= g_pZone->GetWidth() 
		|| next.y >= g_pZone->GetHeight()) 
	{
		m_X = oldX;
		m_Y = oldY;

		return false;
	}

	// serverÁÂÇ¥ ±â¾ï
	// 2001.8.6
	// ÁÖ¼® 2002.3.22... PacketMove·Î ¿Å°å´Ù.
	//SetServerPositionWithBlock( next.x, next.y );	

	// ¿ø·¡ ÁÂÇ¥·Î µÇµ¹¸°´Ù.
	m_X = oldX;
	m_Y = oldY;

	//-----------------------------------------------------
	// °¥ ¼ö ¾ø´Â °÷ÀÎ °æ¿ì...
	//-----------------------------------------------------
	// Server¿¡¼­ Àß¸øµÈ Á¤º¸°¡ ¿Ô´Ù.. - -;;
	// timing¹®Á¦¶ó°í ÇÒ±î?
	#ifdef OUTPUT_DEBUG
		//if (!m_pZone->CanMove(m_MoveType, next.x, next.y))
		{
		//	DEBUG_ADD_FORMAT("[ Collide ] Creature[ID=%d] Can't Move to (%d,%d) - But, Go!", m_ID, next.x, next.y);				
			
			// ±×·¡µµ ¿òÁ÷¿©º¸ÀÚ~
			//return;
		}
	#endif

	//--------------------------------------------------
	// Server¿¡¼­ °ËÁõµÈ ÁÂÇ¥¸¦ ¼³Á¤ÇÑ´Ù.
	//--------------------------------------------------
	//m_ServerX = x;//next.x;
	//m_ServerY = y;//next.y;

	// ¾ÆÁ÷ ÀÌÀü Á¤º¸°¡ updateµÇÁö ¾Ê¾ÒÀ¸¸é...
	if (m_NextX != SECTORPOSITION_NULL)
		// && m_NextY != SECTORPOSITION_NULL
	{
		// message
		DEBUG_ADD_FORMAT("[ SlowUpdate ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", 
							m_ID, 
							m_X, m_Y, 
							m_NextX, m_NextY, 
							g_pPlayer->GetX(), g_pPlayer->GetY());			
		
		// ÀÌÀü Á¤º¸¸¦ ¹Ù·Î update½ÃÅ²´Ù.
		MovePosition(m_NextX, m_NextY);
		m_Direction = m_NextDirection;
		m_DirectionMove = m_NextDirection;

		// (m_X, m_Y)¿¡¼­ m_CurrentDirectionÀ¸·Î ÀÌµ¿½ÃÅ²´Ù.
		SetNextAction( m_MoveAction );

		m_NextX = x;
		m_NextY = y;
		m_NextDirection = direction;	

		return true;
	}

	//--------------------------------------------------
	// Player°¡ ÀÌ¹Ì °¥ ÀÚ¸®¿¡ ÀÖ´Ù¸é....
	//--------------------------------------------------
	// ¸ø °£´Ù~~
	/*
	if (g_pPlayer->GetX()==next.x && g_pPlayer->GetY()==next.y)
	{
		// ¹æÇâ¸¸ ¼³Á¤..
		//pCreature->SetDirection( pGCMove->getDir() );	
		
		// ÀÌÀü¿¡ ÀÖ´ø ÁÂÇ¥¿¡¼­ ¿òÁ÷ÀÌ·Á°í ÇÑ °æ¿ì..
		if (oldX==x && oldY==y)
		{
			// ±×³É ¼­ ÀÖÀ¸¸é µÈ´Ù.
		}
		else
		{
			// ÀÌÀü¿¡ ÀÖ´ø ÁÂÇ¥(oldX, oldY)¿¡¼­ »õÁÂÇ¥(x,y)·Î ÀÌµ¿ÇÑ´Ù.
			m_X = oldX;
			m_Y = oldY;

			MovePosition( x, y );			

			// »õ ÁÂÇ¥¿¡¼­ ¹æÇâ(direction)À¸·Î ¿òÁ÷¿©¾ß ÇÏÁö¸¸
			// ±×³É »õ ÁÂÇ¥¿¡ ¼­ ÀÖ¾î¾ß ÇÑ´Ù.
		}

		// message		
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif
	}
	*/
	//--------------------------------------------------
	// ¸ñÀûÁö¿¡ ÀÌ¹Ì °¡ ÀÖ´Â °æ¿ì´Â ¿òÁ÷ÀÏ ÇÊ¿ä¾ø´Ù.
	//--------------------------------------------------		
	//else 
	if (m_X==next.x && m_Y==next.y)
	{	
		// message
		DEBUG_ADD_FORMAT("[ Stand ] [ID=%d] : Already Reached (%d, %d) ,  Player=(%d, %d)", m_ID, oldX, oldY, g_pPlayer->GetX(), g_pPlayer->GetY());						
	}
	//--------------------------------------------------
	// ¸ñÀûÁö¿¡ °¡ ÀÖÁö ¾ÊÀº °æ¿ì --> ¿òÁ÷ÀÎ´Ù.
	//--------------------------------------------------
	else
	{		
		//--------------------------------------------------
		// ÀÌÀü¿¡ ÀÖ´ø À§Ä¡¿¡¼­ ¿òÁ÷ÀÌ´Â °ÍÀÌ¸é.. 
		//--------------------------------------------------
		if (m_X==x && m_Y==y)
		{
			// ¿òÁ÷ÀÌ´Â µ¿ÀÛÀÌ ³¡³­ »óÅÂÀÌ¸é..
			if (m_MoveCount>=m_MoveCountMax)
			{
				DEBUG_ADD_FORMAT("[ MoveSetting ] [ID=%d] From(%d, %d) to Direction(%d) ,  Player=(%d, %d)", m_ID, x,y, direction, g_pPlayer->GetX(), g_pPlayer->GetY());						
				
				m_Direction = direction;
				m_DirectionMove = direction;
				SetNextAction( m_MoveAction );
				//m_bNextAction = true;
			}
			// ¾ÆÁ÷ ¿òÁ÷ÀÌ°í ÀÖ´Â ÁßÀÌ¸é..
			else
			{
				DEBUG_ADD_FORMAT("[ MoveBuffering ][ID=%d] Current(%d, %d), Next(%d, %d) to Direction(%d)", m_ID, m_X, m_Y, x,y, direction);						
				
				m_NextX = x;
				m_NextY = y;
				m_NextDirection = direction;
			}
		}
		//--------------------------------------------------
		// jumpÇÏ°Ô µÇ´Â °æ¿ì¿¡...
		//--------------------------------------------------
		// ÀÌÀü¿¡ ÀÖ´ø CreatureÀÇ À§Ä¡¸¦ Á¦°ÅÇÑ´Ù.
		else
		{
			// ÀÌÀü¿¡ ÀÖ´ø ÁÂÇ¥(m_X, m_Y)¿¡¼­ »õÁÂÇ¥(x,y)·Î ÀÌµ¿ÇÑ´Ù.			
			MovePosition( x, y );			
			
			// »õ ÁÂÇ¥¿¡¼­ ¹æÇâ(direction)À¸·Î ¿òÁ÷¿©¾ß ÇÑ´Ù.
			m_Direction = direction;
			m_DirectionMove = direction;
			m_NextDirection = direction;	// 2001.11.6
			SetNextAction( m_MoveAction );

			// message
			DEBUG_ADD_FORMAT("[ Jump ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_ID, m_X, m_Y, next.x, next.y, g_pPlayer->GetX(), g_pPlayer->GetY());
		}			
		
		// ¹æÇâÀ» Á¤ÇÏ°í ¿òÁ÷ÀÌµµ·Ï ÇÑ´Ù.
		//m_CurrentDirection = direction;
		//SetNextAction( m_MoveAction );
	}	

	return true;
}

//----------------------------------------------------------------------
// Get ActionInfo's Action
//----------------------------------------------------------------------
// nActionInfo¿¡ ÀûÀýÇÑ ACTIONÀ» Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
	// ¹«±â ¼Óµµ¿¡ µû¶ó¼­ actionÀÌ ´Þ¶óÁø´Ù.
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
		// Lightning Hand´Â ¹«Á¶°Ç ACTION_SLAYER_SWORD_2ÀÌ´Ù. - -;d
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
	if (m_CreatureType==717 || m_CreatureType==721 || m_CreatureType==723) // Áúµå·¹, Áúµå·¹ ºÐ½Å, °¢¼º Áúµå·¹
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
// nActionInfo¿¡ ÀûÀýÇÑ CastingStartFrameÀ» Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ RepeatStartFrameÀ» Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ RepeatEndFrameÀ» Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ CastingFramesÀ» Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ Delay¸¦ Ã£´Â´Ù.
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
	// ±âº» actionÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ ACTIONÀ» Ã£´Â´Ù.
//----------------------------------------------------------------------
TYPE_SOUNDID
MCreature::GetCastingSoundID(TYPE_ACTIONINFO nActionInfo)
{
	//-------------------------------------------------------------
	// ±âº» ¹«±âÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
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
// nActionInfo¿¡ ÀûÀýÇÑ ActionEffectSpriteTypeÀ» Ã£´Â´Ù.
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
	// ±âº» ¹«±âÀÇ Àû¿ëÀ» ¹Þ´Â°¡?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponActionEffectSpriteType())
	{
		useActionInfo = GetBasicActionInfo(); //m_nBasicActionInfo;
	}
	else
	{
		useActionInfo = nActionInfo;
	}

	// plus¸¸Å­ ´õÇÑ´Ù.
	// ÀÓ½Ã ¶«»§~~ ±×·¯³ª.. ¾ÕÀ¸·Îµµ °íÄ¡Áö ¾Ê°ÚÁö - -;
	useActionInfo += (*g_pActionInfoTable)[nActionInfo].GetAffectCurrentWeaponActionInfoPlus();


	TYPE_EFFECTSPRITETYPE re_est = EFFECTSPRITETYPE_NULL;

	//------------------------------------------------------------
	// ³²ÀÚÀÎ °æ¿ì
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
	// ¿©ÀÚ¿ë Effect check
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
	
	// ¾ø´Â °æ¿ì´Â ³²ÀÚ²¨ »ç¿ë..
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
// this Creature´Â Damage¸¦ ¹Þ¾ÒÀ¸¹Ç·Î
//
// 0. ¸ðµç µ¿ÀÛÀ» ÁßÁöÇÏ°í..
// 1. Action --> Damage¹Þ´Â µ¿ÀÛ
// 2. Damage Ç¥½Ã
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
	//--------------------------------------------------
	/*
	// ¿©±â¼­ returnÇØ¹ö¸®¸é Á×À»¶§ ¸Â´Â ÀÌÆåÆ®°¡ ¾È º¸ÀÎ´Ù.
	if (!m_bAlive)
	{
		// Á×¾úÀ»¶§´Â Á×´Â effect¸»°í´Â º¸¿©ÁÖ¸é ¾ÈµÈ´Ù.
		if (nResultActionInfo!=(*g_pCreatureTable)[m_CreatureType].DeadActionInfo)
			return;
	}
	*/

	//----------------------------------------------------------------------
	// °á°ú actionÀÌ ¾Æ´Ñ °æ¿ì..  - ÀÇ¹Ì°¡ ÀÖ³ª?? Èì..
	//----------------------------------------------------------------------
	if (nResultActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo()
		// fast move, knockbackÁß¿¡´Â actionÀ» º¸¿©ÁÖÁö ¾Ê´Â´Ù... // 2001.10.9
		|| m_bFastMove || m_bKnockBack || nResultActionInfo == ACTIONINFO_NULL)
	{
		return;
	}


	//----------------------------------------------------------------------
	// ¹æ¾î¸· È¿°ú.. - -;
	//----------------------------------------------------------------------
	if (GetActionInfoAction( nResultActionInfo )==ACTION_DAMAGED)
	{
		// Á×¾úÀ»¶§ ¸Â´Â µ¿ÀÛ ¾È º¸¿©ÁÖ±â..
		// Å¸ÀÏ ±â¼ú¶§¹®¿¡ ¸Â±âµµ ÇÏ´õ¶ó´Â.. - -;
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


	// ÀÌµ¿À» ÁßÁöÇÑ´Ù.
	////m_sX = 0;	
	////m_sY = 0;	
	//SetStop();

	//m_ActionCount = 0; 
	//------------------------------------------------------
	// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
	//------------------------------------------------------
	AffectBufferedActionInfo();
	
	ActionMoveNextPosition();

	// nResultActionInfo¿¡ ÇØ´çÇÏ´Â ActionInfo¸¦ Ã£¾Æ¾ß ÇÑ´Ù.
	// ¿ø·¡ActionInfo + MIN_RESULT_ACTIONINFO¸¦ ÇÏ¸é µÈ´Ù.
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

	// ÀÚ½Å¿¡°Ô »ç¿ë?..
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
// this Creature´Â Attack µ¿ÀÛÀ» ÃëÇÑ´Ù.
//
// 0. ¸ðµç µ¿ÀÛÀ» ÁßÁöÇÏ°í..
// 1. Action Attack
//----------------------------------------------------------------------
void		
MCreature::PacketAttackNormal(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE direction)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
	//--------------------------------------------------
	if (!m_bAlive)
		return;

	// ÀÌµ¿À» ÁßÁöÇÑ´Ù.
	m_sX = 0;	
	m_sY = 0;
	
	// server¿¡¼­ ³¯¾Æ¿Â~ Á¤º¸ ¼³Á¤..
	//--------------------------------------------------
	// Player°¡ ÀÌ¹Ì ±× ÀÚ¸®¿¡ ÀÖ´Ù¸é....
	//--------------------------------------------------
	// ¸ø °£´Ù~~
	/*
	if (g_pPlayer->GetX()==sX && g_pPlayer->GetY()==sY)
	{
		// message
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move To Attack] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif

		// ÇöÀç Player°¡ ÀÖ´Â À§Ä¡·Î ¹Ù¶óº»´Ù.
		SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );
	}	
	else
	*/
	{
		//--------------------------------------------------
		// ´Ù¸¥ Creature°¡ ÀÌ¹Ì ÀÖ´Ù¸é?
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

		// ÇöÀç ÁÂÇ¥¿Í °°Àº °æ¿ì.. ¿òÁ÷ÀÏ ÇÊ¿ä°¡ ¾ø´Ù.
		if (m_X==sX && m_Y==sY)
		{
		}
		// ÁÂÇ¥°¡ ´Ù¸£¸é.. ÀÏ´Ü ¿òÁ÷¿©ÁÖ°í AttackÇØ¾ß ÇÑ´Ù.
		else
		{
			// ÀÌµ¿
			MovePosition(sX, sY);
			
			// message
			DEBUG_ADD_FORMAT("[ Move To Attack ] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_X, m_Y, sX, sY, g_pPlayer->GetX(), g_pPlayer->GetY());								
		}	

		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..		
		m_Direction = direction;
	}		

	m_ActionCount = 0; 

	// °ø°ÝÇÏ´Â motion
	m_nNextUsedActionInfo	= SKILL_ATTACK_MELEE;
	SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );	
}

//----------------------------------------------------------------------
// Packet SpecialAction To Sector
//----------------------------------------------------------------------
// this Creature´Â nActionInfo¿¡ µû¸¥ µ¿ÀÛÀ» ÃëÇÑ´Ù.
//
// 0. ¸ðµç µ¿ÀÛÀ» ÁßÁöÇÏ°í..
// 1. SpecialAction
//----------------------------------------------------------------------
void		

MCreature::PacketSpecialActionToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, MActionResult* pActionResult)
{
	DEBUG_ADD_FORMAT("PacketSpecialActionToSector ai : %d", nActionInfo );
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
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
	// Sector¿¡ »ç¿ëÇÏ´Â ±â¼úÀÌ ¸Â´Â °æ¿ì
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
		// ³È³È.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();
	//	DEBUG_ADD("3");

		ActionMoveNextPosition();
	//	DEBUG_ADD("2");

		// ÀÌµ¿À» ÁßÁöÇÑ´Ù.
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
		// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
		//------------------------------------------------------
		AffectBufferedActionInfo();
	//	DEBUG_ADD("1");

		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= sX;
		m_TraceY		= sY;
		m_TraceZ		= 0;	// z°ªÀº.. ÀÇ¹Ì°¡ ÀÖÀ»±î..
		// 2004, 11, 4, sobeit modify start - ±×·¹ÀÌÆ® ·¯ÇÇ¾ð ¶«¿¡...
		if(GetCreatureType() == 764 || GetCreatureType() == 765)
		{
			SetTraceID( GetID() );
			if(nActionInfo == SKILL_CHAIN_THROWING_AXE)
				SetShadowCount(3);
		}
		else
			SetTraceID( OBJECTID_NULL );
		// 2004, 11, 4, sobeit modify end

		
		// ¸ñÇ¥¸¦ ÇâÇØ¼­ ¹Ù¶óº»´Ù.
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
		// ÀÌ¹Ì Ç¥ÇöÇØ¾ßÇÒ °á°ú°¡ ÀÖ´Â °æ¿ì..
		//---------------------------------------------------------------
	//	DEBUG_ADD("7");
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// ÀÌ¹Ì ÀÖ´ø °á°ú¸¦ Ã³¸®ÇØÁØ´Ù.	
			// [ TEST CODE ]
			// °á°ú Ã³¸®..
			//
			// (!) m_pActionResult°ªÀÌ Execute¿¡¼­ º¯ÇÒ ¼ö ÀÖ¾î¼­ 
			//		ÀúÀåÇß´Ù°¡ Áö¿öÁà¾ß ÇÑ´Ù.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;			
		}

	//	DEBUG_ADD("6");
		// °á°ú¸¦ ¼³Á¤
		m_pActionResult = pActionResult;

	//	DEBUG_ADD("5");
	}
	//------------------------------------------------------
	// ¾Æ´Ñ °æ¿ì
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
// this Creature´Â nActionInfo¿¡ µû¸¥ µ¿ÀÛÀ» ÃëÇÑ´Ù.
//
// 0. ¸ðµç µ¿ÀÛÀ» ÁßÁöÇÏ°í..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToOther(TYPE_ACTIONINFO nActionInfo, TYPE_OBJECTID id, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
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
	// Å¸ÀÎ¿¡°Ô »ç¿ëÇÏ´Â ±â¼úÀÌ ¸Â´Â °æ¿ì
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		// ´ë»óÀÌ µÇ´Â creature¸¦ ¾ò´Â´Ù.
		MCreature* pCreature = g_pZone->GetCreature( id );
		
		if (pCreature==NULL)
		{
			// ±×·± creature°¡ ¾øÀ» °æ¿ì
			DEBUG_ADD_FORMAT("[Skill : %s] There's no such creature(%d)", (*g_pActionInfoTable)[nActionInfo].GetName(), id);

			if (pActionResult!=NULL)
			{	
				delete pActionResult;
			}
			
			return;
		}

		//---------------------------------------------------------------
		// ³È³È.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// ÀÌµ¿À» ÁßÁöÇÑ´Ù.
		//m_sX = 0;	
		//m_sY = 0;

		//------------------------------------------------------
		// ÀÌÀü µ¿ÀÛÀÌ¶û °°Àº °æ¿ì
		// 2001.7.23 Ãß°¡
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
		// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= pCreature->GetX();
		m_TraceY		= pCreature->GetY();
		m_TraceZ		= pCreature->GetZ();
		SetTraceID ( id );

		// ¸ñÇ¥¸¦ ÇâÇØ¼­ ¹Ù¶óº»´Ù.		
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
		// ÀÌ¹Ì Ç¥ÇöÇØ¾ßÇÒ °á°ú°¡ ÀÖ´Â °æ¿ì..
		//---------------------------------------------------------------		
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// ÀÌ¹Ì ÀÖ´ø °á°ú¸¦ Ã³¸®ÇØÁØ´Ù.	
			// [ TEST CODE ]
			// °á°ú Ã³¸®..			
			// (!) m_pActionResult°ªÀÌ Execute¿¡¼­ º¯ÇÒ ¼ö ÀÖ¾î¼­ 
			//		ÀúÀåÇß´Ù°¡ Áö¿öÁà¾ß ÇÑ´Ù.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}

		// °á°ú¸¦ ¼³Á¤
		m_pActionResult = pActionResult;

	}
	//------------------------------------------------------
	// ¾Æ´Ñ °æ¿ì
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
// castingµ¿ÀÛ¸¸ º¸¿©ÁÖ°Ô µÈ´Ù.
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToInventoryItem(TYPE_ACTIONINFO nActionInfo)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// Å¸ÀÎ¿¡°Ô »ç¿ëÇÏ´Â ±â¼úÀÌ ¸Â´Â °æ¿ì
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetItem())
	{		
		//------------------------------------------------------
		// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..	
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
		// ÀÌ¹Ì Ç¥ÇöÇØ¾ßÇÒ °á°ú°¡ ÀÖ´Â °æ¿ì..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// ÀÌ¹Ì ÀÖ´ø °á°ú¸¦ Ã³¸®ÇØÁØ´Ù.	
			// [ TEST CODE ]
			// °á°ú Ã³¸®..			
			// (!) m_pActionResult°ªÀÌ Execute¿¡¼­ º¯ÇÒ ¼ö ÀÖ¾î¼­ 
			//		ÀúÀåÇß´Ù°¡ Áö¿öÁà¾ß ÇÑ´Ù.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}
	}
	//------------------------------------------------------
	// ¾Æ´Ñ °æ¿ì
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
// ´ë»óÀÌ Zone¿¡ ¾ø´Â °æ¿ì...
//
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToNobody(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// Å¸ÀÎ¿¡°Ô »ç¿ëÇÏ´Â ±â¼úÀÌ ¸Â´Â °æ¿ì
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		//---------------------------------------------------------------
		// ³È³È.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		//------------------------------------------------------
		// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= x;
		m_TraceY		= y;
		m_TraceZ		= 0;
		SetTraceID( OBJECTID_NULL );

		// ¸ñÇ¥¸¦ ÇâÇØ¼­ ¹Ù¶óº»´Ù.		
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
	// ¾Æ´Ñ °æ¿ì
	//------------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Other", (*g_pActionInfoTable)[nActionInfo].GetName());	
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Self
//----------------------------------------------------------------------
// this Creature´Â nActionInfo¿¡ µû¸¥ µ¿ÀÛÀ» ÃëÇÑ´Ù.
//
// 0. ¸ðµç µ¿ÀÛÀ» ÁßÁöÇÏ°í..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToSelf(TYPE_ACTIONINFO nActionInfo, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// Á×¾úÀ¸¸é return
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
	// ÀÚ½ÅÇÑÅ× »ç¿ëÇÏ´Â ±â¼úÀÌ ¸Â´Â °æ¿ì
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetSelf())
	{
		//---------------------------------------------------------------
		// ³È³È.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// ÀÌµ¿À» ÁßÁöÇÑ´Ù.
	//	m_sX = 0;	
	//	m_sY = 0;
		//------------------------------------------------------
		// ÀÌ¹Ì NextUsedACtionInfo°¡ ÀÖ´Â °æ¿ì Ã¼Å©
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server¿¡¼­ ³¯¾Æ¿Â Á¤º¸ ¼³Á¤..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= m_X;
		m_TraceY		= m_Y;
		m_TraceZ		= m_Z;	// z°ªÀº.. ÀÇ¹Ì°¡ ÀÖÀ»±î..
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
		// ÀÌ¹Ì Ç¥ÇöÇØ¾ßÇÒ °á°ú°¡ ÀÖ´Â °æ¿ì..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// ÀÌ¹Ì ÀÖ´ø °á°ú¸¦ Ã³¸®ÇØÁØ´Ù.	
			// [ TEST CODE ]
			// °á°ú Ã³¸®..			
			// (!) m_pActionResult°ªÀÌ Execute¿¡¼­ º¯ÇÒ ¼ö ÀÖ¾î¼­ 
			//		ÀúÀåÇß´Ù°¡ Áö¿öÁà¾ß ÇÑ´Ù.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}

		// °á°ú¸¦ ¼³Á¤
		m_pActionResult = pActionResult;
	}
	//------------------------------------------------------
	// ¾Æ´Ñ °æ¿ì
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
		// Guild ID ¹Ù²ñ
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
		// ¼ºÇâ ¹Ù²ñ
		//--------------------------------------------------
		case MODIFY_ALIGNMENT :			// ¼ºÇâ
		{
			//--------------------------------------------------
			// -10000 ~ 10000À» 0~4·Î ¹Ù²Û´Ù.
			//--------------------------------------------------
			value = ConvertAlignment( value );
			m_Status[n] = value;	
		}
		break;

		//--------------------------------------------------
		// HP ¹Ù²î´Â °æ¿ì
		//--------------------------------------------------
		case MODIFY_CURRENT_HP :
			{
				const int localValue = min(GetMAX_HP()-GetSilverDamage(), value);
				AddHPModify( localValue - oldValue );

				// max¸¦ ¾È ³Ñ°Ô
				m_Status[n] = localValue;	

				// ÆÄÆ¼ÀÎ °æ¿ì´Â HP ´Ù½Ã ¼³Á¤ÇØÁØ´Ù.
				if (IsPlayerParty())
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
						
					// ÁÂÇ¥¸¦ ¼öÁ¤ÇØÁØ´Ù.
					if (pInfo!=NULL)
					{
						pInfo->HP = localValue;
					}
				}
			}
		break;

		//--------------------------------------------------
		// MaxHP ¹Ù²î´Â °æ¿ì
		//--------------------------------------------------
		case MODIFY_MAX_HP :
			// ÆÄÆ¼ÀÎ °æ¿ì´Â HP ´Ù½Ã ¼³Á¤ÇØÁØ´Ù.
			if (IsPlayerParty())
			{
				PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
					
				// ÁÂÇ¥¸¦ ¼öÁ¤ÇØÁØ´Ù.
				if (pInfo!=NULL)
				{
					pInfo->MaxHP = value;
				}
			}
		break;

//		//--------------------------------------------------
//		// Armageddon HP ¹Ù²î´Â °æ¿ì
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
//				// ´Ù½Ã Ãß°¡				
//				if (value!=0)
//				{
//					AddEffectStatus(EFFECTSTATUS_ARMAGEDDON, 0xFFFF);
//				}
//
//				// ºÎ¼­Áö´Â ºÎºÐ Ç¥½Ã
//				int skillID = ARMAGEDDON_CRASH_1 + (changeValue-1);
//				
//				//--------------------------------------------------
//				// ºÎ¼­Áö´Â Effect
//				//--------------------------------------------------		
//				ExecuteActionInfoFromMainNode(
//					skillID,
//				
//					m_X, m_Y, 0,
//					m_Direction,
//					
//					m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
//					m_X, m_Y, 0, 
//					
//					0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
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
// slayer·Î º¯½ÅÇÏ´Âµ¥..
// ÀÌ¹Ì slayerÀÌ¸é.. return false
//----------------------------------------------------------------------
bool	
MCreature::ChangeToSlayer()
{
	//-----------------------------------------------------
	// ÀÌ¹Ì slayerÀÌ¸é..
	//-----------------------------------------------------
	if (IsSlayer())
	{
		return false;
	}

	ClearAttachEffect();

	//-----------------------------------------------------
	// Á¤º¸ ¼³Á¤
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// Á¤Áö ½ÃÅ²´Ù.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// ³², ¿© ?
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
	// ¹øÂ½
	//-----------------------------------------------------
	//g_pTopView->SetFadeStart(1, 31, 6,  5,5,31);

	return true;
}

//----------------------------------------------------------------------
// Change To Vampire
//----------------------------------------------------------------------
// vampire·Î º¯½ÅÇÏ´Âµ¥..
// ÀÌ¹Ì vampireÀÌ¸é.. return false
//----------------------------------------------------------------------
bool
MCreature::ChangeToVampire()
{
	//-----------------------------------------------------
	// ÀÌ¹Ì vampireÀÌ¸é..
	//-----------------------------------------------------
	if (IsVampire())
	{
		return false;
	}

	ClearAttachEffect();

	DEBUG_ADD("MCreature::ChangeToVampire - RemoveEffectStatus OK");
	
	//-----------------------------------------------------
	// Á¤º¸ ¼³Á¤
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// Á¤Áö ½ÃÅ²´Ù.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// ³², ¿© ?
	//-----------------------------------------------------
	if (IsMale())
	{
		// º¯½Å ¼Ò¸®
		PlaySound(SOUND_VAMPIRE_CHANGE_MALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_MALE1 );
	}
	else
	{
		// º¯½Å ¼Ò¸®
		PlaySound(SOUND_VAMPIRE_CHANGE_FEMALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_FEMALE1 );
	}

	//-----------------------------------------------------
	// º¹Àå Á¦°Å
	//-----------------------------------------------------
	// ¾ÈÇØµµ µÈ´Ù.
	//for (int i=0; i<ADDON_MAX; i++)
	//{
	//	ClearAddonInfo( i );
	//}

	//-----------------------------------------------------
	// ¹øÂ½
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
		// Á¤Áö
		//------------------------------------------------
		SetStop();

		m_nNextUsedActionInfo = ACTIONINFO_NULL;
		m_bNextAction = false;
		
		//------------------------------------------------
		// fast move ¼³Á¤
		//------------------------------------------------	
		SetAction( m_MoveAction );

		//MovePosition( x, y );

		m_NextX = x;
		m_NextY = y;
		SetServerPositionWithBlock( x, y );

		// SetActionÇÏ°í ³ª¼­ ¼³Á¤ÇØÁà¾ß ÇÑ´Ù.
		m_bFastMove = true;

		return true;
	}

	// ¾îÂ¶µç ¼­¹öÁÂÇ¥´Â °ËÁõµÆ´Ù°í º»´Ù.
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
		// ¶ß¾Æ ÀÌ°Å bool¾Æ´Ñµ­.. m_b·Î ½ÃÀÛÇÏ³× - -; ÄÄÆÄÀÏ ´Ù½Ã ÇÏ±â ±ÍÂ÷³ª¼­  --;

		if( Action == 0 )
		{
			//int knockBackCount = (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_DAMAGED );
			int knockBackCount = GetCreatureActionCountMax( this, ACTION_DAMAGED );
			
			if (knockBackCount<=0)
			{
				// ¸Â±â µ¿ÀÛÀÌ ¾ø´ç - -;
				return false;
			}
			
			//------------------------------------------------
			// Á¤Áö
			//------------------------------------------------
			SetStop();
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
			m_bNextAction = false;

			//------------------------------------------------
			// KnockBack µ¿ÀÛÀ¸·Î damaged¸¦ ¼³Á¤
			//------------------------------------------------	
			SetAction( ACTION_DAMAGED );
			
			//------------------------------------------------
			// Ä³¸¯ÅÍÀÇ Action¿¡ ¸Â´Â Sound¸¦ Ãâ·ÂÇØÁØ´Ù.
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
		// sx, sy·Î ÁÂÇ¥ º¸Á¤
		//
		// ÇöÀçÀ§Ä¡(m_X, m_Y)¿¡¼­ KnockBackÀ§Ä¡(x, y)·Î ÀÌµ¿.
		// 
		//------------------------------------------------	
		int oldX = m_X;
		int oldY = m_Y;
		int newX = x;
		int newY = y;
		POINT	oldPoint = g_pTopView->MapToPixel(oldX, oldY);
		POINT	newPoint = g_pTopView->MapToPixel(newX, newY);
		
		// ÀÌµ¿ÇØ¾ßÇÒ pixel¼ö °è»ê
		int gapX = newPoint.x - oldPoint.x; //+ m_sX);
		int gapY = newPoint.y - oldPoint.y; //+ m_sY);

		m_sX = -gapX;
		m_sY = -gapY;

		// ÇÑ¹ø¿¡ ÀÌµ¿ÇØ¾ßÇÒ pixel¼ö --> (m_cX, m_cY)¿¡ ³Ö¾îµÐ´Ù.
		m_cX = gapX / m_bKnockBack;
		m_cY = gapY / m_bKnockBack;

		
		// ÁÂÇ¥°¡ ´Þ¶óÁ³À» °æ¿ì¸¸..
		if (newX!=oldX || newY!=oldY)
		{
			//------------------------------------------------	
			// KnockBackµÇ´Â ¹æÇâ ¼³Á¤
			// »õÁÂÇ¥¿¡¼­ ¿¹ÀüÁÂÇ¥¸¦ ¹Ù¶óº¸´Â ¹æÇâ
			//------------------------------------------------	
			m_Direction = MTopView::GetDirectionToPosition(newX, newY, oldX, oldY);
		
			//------------------------------------------------	
			// ÁÂÇ¥´Â ¹Ù·Î ÀÌµ¿.
			//------------------------------------------------			
			MovePosition( x, y );
		}

		SetServerPositionWithBlock( x, y );

		if( Action == 0 )
		{
			//------------------------------------------------			
			// ¸ö¿¡ ºÙ´Â ÇÇ
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
			// ¹Ù´Ú¿¡ Èê¸®´Â ÇÇ 3¹æ¿ï(-_-;)
			//------------------------------------------------			
			if (g_pUserOption->BloodDrop)
			{
				int num = rand()%3 + 3;
				for (int i=0; i<num; i++)
				{
					ExecuteActionInfoFromMainNode(
						BLOOD_CRITICAL_HIT,										// »ç¿ë ±â¼ú ¹øÈ£
						
						oldX, oldY, 0,
						(int)m_Direction,														// »ç¿ë ¹æÇâ
						
						m_ID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
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

	// ¾îÂ¶µç ¼­¹öÁÂÇ¥´Â °ËÁõµÆ´Ù°í º»´Ù.
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

	// 2001.8.10¿¡ Ãß°¡
	// ÄÀ..±Ùµ¥ ÀÌ ÇÔ¼ö ¾²ÀÌÁöµµ ¾Ê´Â±º.. - -;;
	m_MoveCount = m_MoveCountMax;
}

//----------------------------------------------------------------------
// Find Enemy
//----------------------------------------------------------------------
int
MCreature::FindEnemy()
{
	// Áö±ÝÀº MPlayer¿¡¼­¸¸ »ç¿ëÇÑ´Ù.
	// ¹Ì·¡¸¦ À§ÇØ¼­...
	return OBJECTID_NULL;
}

//----------------------------------------------------------------------
// Set ShadowCount
//----------------------------------------------------------------------
void				
MCreature::SetShadowCount(int n)
{ 
	// 5°³±îÁö.. - -;
	m_ShadowCount = min(5, n); 
}

//----------------------------------------------------------------------
// Stop Blood Drain
//----------------------------------------------------------------------
// m_bStopBloodDrainÀÌ¸é ÀÌ°É È£ÃâÇØ¾ß ÇÑ´Ù.
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
// m_bStopAbsorbSoulÀÌ¸é ÀÌ°É È£ÃâÇØ¾ß ÇÑ´Ù.
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
					
					// flagÁ¦°Å
					if (bUseEffectSprite)
					{
						m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flagÁ¦°Å
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
// ´Ü¼øÈ÷ count¸¸ ¹Ù²ãÁØ´Ù.
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

			// darkness¿¡ µé¾î°¡´Â °æ¿ì
			if (m_DarknessCount < 0)
			{
				m_DarknessCount = 0;
				m_DarknessCountInc = 0;
			}
			// darkness¿¡¼­ ºüÁ®³ª¿À´Â °æ¿ì
			else if (m_DarknessCount >= MAX_DARKNESS_COUNT)
			{
				m_DarknessCount = -1;
				m_DarknessCountInc = 0;
			}
		}
	}
	else
	{
		// Slayer°¡ ¾Æ´Ñ °æ¿ì´Â Darkness¶û °ü°è¾ø´Ù.
		// È®ÀÎ¿ë
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;
	}
}

//----------------------------------------------------------------------
// Check In Darkness
//----------------------------------------------------------------------
// darkness°¡ ³ªÅ¸³ª°í/»ç¶óÁö´Â ¼ø°£
//            player°¡ ¿òÁ÷ÀÌ´Â ¼ø°£À» Ã¼Å©ÇÏ¸é µÈ´Ù.
//
// player°¡ ¾Æ´Ï¹Ç·Î  ½Ã¾ß °ü°è¾ø´ç.
//----------------------------------------------------------------------
void
MCreature::CheckInDarkness()
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Darkness¿¡ µé¾î¿Ô´ÂÁö Ã¼Å©
			//-------------------------------------------------------
			// ¼­¹ö ÁÂÇ¥º¸´Ù´Â ÇöÀç ÁÂÇ¥°¡ º¸±â¿¡ ÁÁ´Ù.. ¤»¤»..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && (sector.HasDarkness()))
				{
					// darkness¿¡ ÀÖ´Â °æ¿ì
					//PlaceInDarkness();
					m_DarknessCount = MAX_DARKNESS_COUNT;
					m_DarknessCountInc = 0;
				}
				else
				{		
					// darkness´Â »ç¶óÁ³´Ù.
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
			// Darkness¿¡ µé¾î¿Ô´ÂÁö Ã¼Å©
			//-------------------------------------------------------
			// ¼­¹ö ÁÂÇ¥º¸´Ù´Â ÇöÀç ÁÂÇ¥°¡ º¸±â¿¡ ÁÁ´Ù.. ¤»¤»..
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
			// Fury Of Gnome¿¡ µé¾î¿Ô´ÂÁö Ã¼Å©
			//-------------------------------------------------------
			// ¼­¹ö ÁÂÇ¥º¸´Ù´Â ÇöÀç ÁÂÇ¥°¡ º¸±â¿¡ ÁÁ´Ù.. ¤»¤»..
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
// player´Â darkness¿¡ ÀÖ´Ù.
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
		// darkness¿¡ ÀÖÁö ¾Ê´ø °æ¿ì
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
// player´Â darkness¿¡ ÀÖÁö ¾Ê´Ù.
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
		// darkness¿¡ ÀÖ´ø °æ¿ì
		if (m_DarknessCount >= 0 && m_DarknessCount < MAX_DARKNESS_COUNT)
		{
			m_DarknessCountInc = 1;
		}	
	}
}

//----------------------------------------------------------------------
// Show In Darkness
//----------------------------------------------------------------------
// darkness¿¡ ÀÖÁö ¾Ê¾Æ¼­ Àß º¸ÀÌ°Å³ª
// darkness¿¡ ÀÖ´õ¶óµµ °Å¸®¿¡ ÀÇÇØ¼­ º¸ÀÌ´Â°¡?
//----------------------------------------------------------------------
BOOL	
MCreature::ShowInDarkness(int sX, int sY) const
{

//	DEBUG_ADD_FORMAT("[ShowInDarkness] g_bLight %d", g_bLight);
	// ÀÌ¹Ì darkness¿¡ ÀÖ´Â °æ¿ì
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
		// darknessCount¾È¿¡´Â º¼ ¼ö ÀÖ´Ù.
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
// halluÀÎ °æ¿ì... ¾î¶² ¸÷À¸·Î º¸¿©Áö°Ô µÈ´Ù.
//----------------------------------------------------------------------
void				
MCreature::SetHalluCreature(TYPE_CREATURETYPE type)
{
	// NPC´Â Hallu°É·Áµµ Á¦´ë·Î º¸ÀÎ´Ù.
	if (IsNPC())
	{
		m_bHallu = false;
		return;
	}

	m_HalluCreatureType = type;
	
	m_HalluCreatureFrameID	= (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[type].SpriteTypes[0]].FrameID;
	
	// »ö±ò - ¸÷ÀÎ °æ¿ì¸¸.. ¼³Á¤ÇØÁØ´Ù.
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
	// ÀûÀýÇÑ actionÀ» ¼±ÅÃÇÑ´Ù.
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
	// µ¿ÀÛÀÇ frame ¼ö¸¦ °áÁ¤ÇÑ´Ù.
	//------------------------------------------------------
	int countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );

	if (countMax==0)
	{
		// µ¿ÀÛÀÌ ¾ø´Ù¸é..
		if (m_HalluAction!=ACTION_ATTACK)
		{
			m_HalluAction = ACTION_ATTACK;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );
		}

		// µ¿ÀÛÀÌ ¾ø´Ù¸é Á¤Áö..
		if (countMax==0)
		{
			m_HalluAction = ACTION_STAND;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );						
		}
	}
	
	if (countMax==0)	// ºñ±³ ºñ±³.. - -;
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
//		strstr(GetName(), "¿î¿µÀÚ") != NULL)
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
// CheckRegen - Regen¼³Á¤
//----------------------------------------------------------------------
void
MCreature::CheckRegen()
{
	//-------------------------------------------------------
	// ¹ìÆÄÀÌ¾î°¡ ¾Æ´Ï¸é RegenÇÏÁö ¾Ê´Â´Ù.
	//-------------------------------------------------------
	if (!IsVampire())
	{
		SetRegen( 0, 0 );
		return;
	}

	//-------------------------------------------------------
	// Burrow»óÅÂ
	//-------------------------------------------------------
	if (IsUndergroundCreature())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_BURROW, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Casket »óÅÂ
	//-------------------------------------------------------
	if (IsInCasket())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_CASKET, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Creature Type¿¡ µû¶ó¼­..
	//-------------------------------------------------------
	switch (m_CreatureType)
	{
		//---------------------------------------------------------------
		// ¹ìÆÄÀÌ¾î, ´Á´ë
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
			SetRegen( g_pClientConfig->REGEN_AMOUNT_VAMPIRE, 1000 );	// 2¾¿ 1ÃÊ¸¶´Ù			
		break;

		//---------------------------------------------------------------
		// µý °æ¿ì..
		//---------------------------------------------------------------
		default :
			SetRegen( 0, 0 );	// regen ¾ÈÇÑ´Ù.		
	}
}

//----------------------------------------------------------------------
// ³ª¸¦ ÈíÇ÷ÇÏ°í ÀÖ´ø CreatureÀÇ ÈíÇ÷µ¿ÀÛÀ» ¸ØÃá´Ù
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
// ³ª¸¦ Èí¿µÇÏ°í ÀÖ´ø CreatureÀÇ Èí¿µµ¿ÀÛÀ» ¸ØÃá´Ù
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
			// ¸Þ¸ð¸® Á¦°Å
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list¿¡¼­ Á¦°Å
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
			// ¸Þ¸ð¸® Á¦°Å
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list¿¡¼­ Á¦°Å
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

	// ½ÂÁ÷ Ä³¸¯ÅÍ ÀÏ¶§
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
				if(IsDead()) return 0; //Á×Àº³ÑÇÑÅÙ ÀÌÆåÆ® ¹«½Ã
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
					// ½ÂÁ÷ Ä³¸¯ÅÍ´Â Market Master Effect Àû¿ë ÇÏÁö ¸»¶ó°í ÇØ¼­ ÁÖ¼® Ã³¸® ÇÔ..
					// ³ªÁß¿¡ ¶Ç ¸» ¹Ù²Ù¸é ÁÖ¼® Ç®¾îÁÖ¸é µÊ.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit By Sonic 2006.10.28 È¥³ý¶þ×ªºó½ÇÉ«ÉíÉÏµÄ¹âÎÞÐ§¹û

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
					// ½ÂÁ÷ Ä³¸¯ÅÍ´Â Market Master Effect Àû¿ë ÇÏÁö ¸»¶ó°í ÇØ¼­ ÁÖ¼® Ã³¸® ÇÔ..
					// ³ªÁß¿¡ ¶Ç ¸» ¹Ù²Ù¸é ÁÖ¼® Ç®¾îÁÖ¸é µÊ.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_ADVANCE_FEAR + MarketType - 1;
//					else
						// edit by sonic 2006.10.28   È¥³ý¶þ×ªºó¿´²»µ½ÉíÉÏ»ðÑÕÉ«Ð§¹û
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
					// ½ÂÁ÷ Ä³¸¯ÅÍ´Â Market Master Effect Àû¿ë ÇÏÁö ¸»¶ó°í ÇØ¼­ ÁÖ¼® Ã³¸® ÇÔ..
					// ³ªÁß¿¡ ¶Ç ¸» ¹Ù²Ù¸é ÁÖ¼® Ç®¾îÁÖ¸é µÊ.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit by sonic 2006.10.28  ÐÞÕý¶þ×ªºó½ÇÉ«»ðÑÕÉ«ÎÊÌâ
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
//			// ¸Þ¸ð¸® Á¦°Å
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list¿¡¼­ Á¦°Å
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
//			// ¸Þ¸ð¸® Á¦°Å
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list¿¡¼­ Á¦°Å
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