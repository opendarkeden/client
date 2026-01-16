//----------------------------------------------------------------------
// MActionInfoTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MActionInfoTable.h"
#include "EffectSpriteTypeDef.h"
#include "SkillDef.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MActionInfoTable*			g_pActionInfoTable = NULL;


//----------------------------------------------------------------------
//
//					ACTION_INFO_NODE class
//
//----------------------------------------------------------------------

//-------------------------------------------------------
// 애들 버전.. -_-;
//-------------------------------------------------------
void			
ACTION_INFO_NODE::SetChildMode()
{
	// 하...하드코딩-_-;
	if(EffectSpriteType >= EFFECTSPRITETYPE_BLOOD_DRAIN_1 &&
		EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
	{
		EffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_DRAIN_1 - EFFECTSPRITETYPE_BLOOD_DRAIN_1;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_BLOOD_GROUND_1_1 &&
		EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_GROUND_2_4)
	{
		EffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_GROUND_1_1 - EFFECTSPRITETYPE_BLOOD_GROUND_1_1;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_BLOOD_SLAYER_1 &&
		EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_SLAYER_3)
	{
		EffectSpriteType = EFFECTSPRITETYPE_GREEN_BLOOD_SLAYER_1+((EffectSpriteType-EFFECTSPRITETYPE_BLOOD_SLAYER_1)+2)%3;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_BLOOD_GUN_1_1 &&
		EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_HORIZONTAL_3)
	{
		EffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1 - EFFECTSPRITETYPE_BLOOD_GUN_1_1;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_BLOOD_VERTICAL_1 &&
		EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_VERTICAL_4)
	{
		EffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 - EFFECTSPRITETYPE_BLOOD_VERTICAL_1;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_TRANSFUSION_1 && EffectSpriteType <= EFFECTSPRITETYPE_TRANSFUSION_3)
	{
		EffectSpriteType += EFFECTSPRITETYPE_GREEN_TRANSFUSION_1 - EFFECTSPRITETYPE_TRANSFUSION_1;
	}
	else if(EffectSpriteType >= EFFECTSPRITETYPE_VIOLENT_PHANTOM_BLOOD_1 && EffectSpriteType <= EFFECTSPRITETYPE_VIOLENT_PHANTOM_BLOOD_4)
	{
		EffectSpriteType -= EFFECTSPRITETYPE_VIOLENT_PHANTOM_BLOOD_1 - EFFECTSPRITETYPE_VIOLENT_PHANTOM_BLOOD_TEEN_1;
	}

//	if (EffectSpriteType >=	EFFECTSPRITETYPE_BLOOD_GUN_1_1
//		&&	EffectSpriteType <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
//	{
//		EffectSpriteType = EFFECTSPRITETYPE_NULL;
//	}
}

//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
void			
ACTION_INFO_NODE::SaveToFile(std::ofstream& file)
{			
	file.write((const char*)&EffectGeneratorID, SIZE_EFFECTGENERATORID);			
	file.write((const char*)&EffectSpriteType, SIZE_EFFECTSPRITETYPE);
	file.write((const char*)&Step, 2);
	file.write((const char*)&Count, 2);
	file.write((const char*)&LinkCount, 2);
	file.write((const char*)&SoundID, SIZE_SOUNDID);
	//file.write((const char*)&Light, 1);
	file.write((const char*)&bDelayNode, 1);	
	file.write((const char*)&bResultTime, 1);	
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
ACTION_INFO_NODE::LoadFromFile(std::ifstream& file)
{			
	file.read((char*)&EffectGeneratorID, SIZE_EFFECTGENERATORID);			
	file.read((char*)&EffectSpriteType, SIZE_EFFECTSPRITETYPE);
	file.read((char*)&Step, 2);
	file.read((char*)&Count, 2);
	file.read((char*)&LinkCount, 2);
	file.read((char*)&SoundID, SIZE_SOUNDID);
	//file.read((char*)&Light, 1);
	file.read((char*)&bDelayNode, 1);
	file.read((char*)&bResultTime, 1);
}


//----------------------------------------------------------------------
//
//					ActionInfo class
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------
MActionInfo::MActionInfo()
{ 
	m_bAdvancementClassSkill = false;
	m_bNonAdvancementClassSkill = false;

	m_CastingActionInfo = ACTIONINFO_NULL;	// casting Action Info를 없앤다.
	m_bCastingAction = false;				// casting 동작은 아니라고 설정..

	m_fUserType = FLAG_ACTIONINFO_USER_ALL;
	m_fWeaponType = 0;//FLAG_ACTIONINFO_WEAPON_ALL;
	m_fCurrentWeapon = 0;	// 현재 들고 있는 무기의 적용을 받는가?
	m_MainNode = MAIN_NODE_NULL;
	m_bAttack = TRUE;			// 기본적으로 공격기술로 설정

	m_bUseRepeatFrame = false;

	for (int i=0; i<3; i++)
	{
		m_CastingStartFrame[i] = 0;	// casting effect의 시작 frame
		m_CastingFrames[i]		= 0;	// casting effect의 지속 frame
		
		m_StartFrame[i] = 0xFFFF;	// max값은 아니지만. 이 정도면 충분.. 음냐하.

		m_RepeatStartFrame[i] = m_RepeatEndFrame[i] = -1;
	}

	m_RepeatLimit = 0xFFFF;
	

	m_PacketType = ACTIONINFO_PACKET_NONE;

	m_ActionEffectSpriteTypeFemale = EFFECTSPRITETYPE_NULL;

	// 
	m_fOption = 0;

	m_EffectStatus = EFFECTSTATUS_NULL;

	// 결과 id
	m_ActionResultID	= ACTIONRESULTNODE_NULL;
	m_ActionResultValue	= 0;

	m_PlusActionInfo = 0;

	m_bCastingEffectToSelf = true;

	m_fSelectCreature = FLAG_ACTIONINFO_SELECT_ALL;
	m_bUseGrade = false;
	m_bUseActionStep = false;

	int i;
	for( i = 0 ; i< MAX_ACTION_STEP ;i ++ )
	{
		m_ActionStep[i] = 0;
	}
	m_bAttachSelf = false;
	m_Parent = ACTIONINFO_NULL;
	
	m_MasterySkillStep = 0;
	m_bIgnoreFailDelay = false;
}

//----------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------
MActionInfo::~MActionInfo()
{
}


//-------------------------------------------------------
// 애들 버전.. -_-;
//-------------------------------------------------------
void			
MActionInfo::SetChildMode()
{
	// 하...하드코딩-_-;

	// male
	if(m_ActionEffectSpriteType >= EFFECTSPRITETYPE_BLOOD_GUN_1_1 &&
		m_ActionEffectSpriteType <= EFFECTSPRITETYPE_BLOOD_HORIZONTAL_3)
	{
		m_ActionEffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1 - EFFECTSPRITETYPE_BLOOD_GUN_1_1;
	}
	else if(m_ActionEffectSpriteType >= EFFECTSPRITETYPE_BLOOD_VERTICAL_1 &&
		m_ActionEffectSpriteType <= EFFECTSPRITETYPE_BLOOD_VERTICAL_4)
	{
		m_ActionEffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 - EFFECTSPRITETYPE_BLOOD_VERTICAL_1;
	}
//	else if(m_ActionEffectSpriteType >= EFFECTSPRITETYPE_BLOOD_DRAIN_1 &&
//		m_ActionEffectSpriteType <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
//	{
//		m_ActionEffectSpriteType += EFFECTSPRITETYPE_GREEN_BLOOD_DRAIN_1 - EFFECTSPRITETYPE_BLOOD_DRAIN_1;
//	}
	
	// female
	if(m_ActionEffectSpriteTypeFemale >= EFFECTSPRITETYPE_BLOOD_GUN_1_1 &&
		m_ActionEffectSpriteTypeFemale <= EFFECTSPRITETYPE_BLOOD_HORIZONTAL_3)
	{
		m_ActionEffectSpriteTypeFemale += EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1 - EFFECTSPRITETYPE_BLOOD_GUN_1_1;
	}
	else if(m_ActionEffectSpriteTypeFemale >= EFFECTSPRITETYPE_BLOOD_VERTICAL_1 &&
		m_ActionEffectSpriteTypeFemale <= EFFECTSPRITETYPE_BLOOD_VERTICAL_4)
	{
		m_ActionEffectSpriteTypeFemale += EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 - EFFECTSPRITETYPE_BLOOD_VERTICAL_1;
	}
//	else if(m_ActionEffectSpriteTypeFemale >= EFFECTSPRITETYPE_BLOOD_DRAIN_1 &&
//		m_ActionEffectSpriteTypeFemale <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
//	{
//		m_ActionEffectSpriteTypeFemale += EFFECTSPRITETYPE_GREEN_BLOOD_DRAIN_1 - EFFECTSPRITETYPE_BLOOD_DRAIN_1;
//	}

//	if (m_ActionEffectSpriteType >=	EFFECTSPRITETYPE_BLOOD_GUN_1_1
//		&&	m_ActionEffectSpriteType <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
//	{
//		m_ActionEffectSpriteType = EFFECTSPRITETYPE_NULL;
//	}
//
//	if (m_ActionEffectSpriteTypeFemale >=	EFFECTSPRITETYPE_BLOOD_GUN_1_1
//		&&	m_ActionEffectSpriteTypeFemale <= EFFECTSPRITETYPE_BLOOD_DRAIN_3)
//	{
//		m_ActionEffectSpriteTypeFemale = EFFECTSPRITETYPE_NULL;
//	}

	for (int i=0; i<m_Size; i++)
	{
		m_pTypeInfo[i].SetChildMode();
	}	
}

//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
void			
MActionInfo::SaveToFile(std::ofstream& file)
{
	// 임시로 계산.. - -;
	// startframe이 늦은 만큼 delay도 줄어든다
	/*
	if (m_StartFrame==0xFFFF)
	{
		m_Delay = 0;
	}
	else 
	{
		int minusDelay = m_StartFrame * 1000 / 16;

		if (m_Delay < minusDelay)
		{
			m_Delay = 0;
		}
		else
		{
			m_Delay -= minusDelay;
		}
	}
	*/
	m_Name.SaveToFile( file );

	file.write((const char*)&m_Action, 1);
	file.write((const char*)&m_ActionEffectSpriteType, SIZE_EFFECTSPRITETYPE);
	file.write((const char*)&m_ActionEffectSpriteTypeFemale, SIZE_EFFECTSPRITETYPE);

	file.write((const char*)&m_bUseRepeatFrame, 1);

	for (int i=0; i<3; i++)
	{
		file.write((const char*)&m_StartFrame[i], 4);

		file.write((const char*)&m_CastingStartFrame[i], 4);
		file.write((const char*)&m_CastingFrames[i], 4);		

		file.write((const char*)&m_RepeatStartFrame[i], 4);	
		file.write((const char*)&m_RepeatEndFrame[i], 4);	

	}
	file.write((const char*)&m_RepeatLimit, 2);	

	file.write((const char*)&m_bCastingEffectToSelf, 1);
	file.write((const char*)&m_CastingActionInfo, 4);
	file.write((const char*)&m_bCastingAction, 1);

	file.write((const char*)&m_Range, 1);
	file.write((const char*)&m_fTarget, 1);	
	file.write((const char*)&m_fStart, 1);
	file.write((const char*)&m_fUserType, 1);
	file.write((const char*)&m_fWeaponType, 2);
	file.write((const char*)&m_fCurrentWeapon, 1);
	file.write((const char*)&m_fOption, 1);
	file.write((const char*)&m_PlusActionInfo, 4);

	BYTE pt = (BYTE)m_PacketType;
	file.write((const char*)&pt, 1);		

	file.write((const char*)&m_Delay, 2);
	file.write((const char*)&m_Value, 4);
	file.write((const char*)&m_SoundID, SIZE_SOUNDID);
	file.write((const char*)&m_MainNode, 4);
	
	// 결과 
	file.write((const char*)&m_ActionResultID, SIZE_ACTIONRESULTID);
	file.write((const char*)&m_ActionResultValue, 4);

	WORD es = m_EffectStatus;
	file.write((const char*)&es, 2);
	

	bool bAttack = (m_bAttack==TRUE);
	file.write((const char*)&bAttack, 1);
	file.write((const char*)&m_fSelectCreature, 1);	

	char flag = 0;	
	if( m_bUseGrade )
		flag |= 0x1;
	if( m_bUseActionStep )
		flag |= 0x2;
	if( m_bAttachSelf )
		flag |= 0x4;
	file.write((const char*)&flag, sizeof(char) );
	if( m_bUseActionStep )
	{
		int i;
		for( i = 0;i<MAX_ACTION_STEP; i++)
		{
			file.write((const char*)&m_ActionStep[i],sizeof( TYPE_ACTIONINFO ) );
		}
	}

	file.write((const char*)&m_Parent, sizeof(TYPE_ACTIONINFO ) );
	file.write((const char*)&m_MasterySkillStep, 1 );
	file.write((const char*)&m_bIgnoreFailDelay, 1 );

	flag = 0;
/*	
	if( m_bAdvancementClassSkill ) flag |= 0x1;
	if( m_bNonAdvancementClassSkill ) flag |= 0x2;
	file.write( (const char*)&flag, sizeof( char ) );
*/	
	// 각 단계에 대한 정보
	CTypeTable<ACTION_INFO_NODE>::SaveToFile(file);
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
MActionInfo::LoadFromFile(std::ifstream& file)
{
	m_Name.LoadFromFile( file );

	file.read((char*)&m_Action, 1);
	file.read((char*)&m_ActionEffectSpriteType, SIZE_EFFECTSPRITETYPE);
	file.read((char*)&m_ActionEffectSpriteTypeFemale, SIZE_EFFECTSPRITETYPE);

	file.read((char*)&m_bUseRepeatFrame, 1);

	for (int i=0; i<3; i++)
	{
		file.read((char*)&m_StartFrame[i], 4);

		file.read((char*)&m_CastingStartFrame[i], 4);	
		file.read((char*)&m_CastingFrames[i], 4);

		file.read((char*)&m_RepeatStartFrame[i], 4);	
		file.read((char*)&m_RepeatEndFrame[i], 4);	
	}
	file.read((char*)&m_RepeatLimit, 2);	

	file.read((char*)&m_bCastingEffectToSelf, 1);
	file.read((char*)&m_CastingActionInfo, 4);
	file.read((char*)&m_bCastingAction, 1);

	file.read((char*)&m_Range, 1);
	file.read((char*)&m_fTarget, 1);
	file.read((char*)&m_fStart, 1);
	file.read((char*)&m_fUserType, 1);
	file.read((char*)&m_fWeaponType, 2);
	file.read((char*)&m_fCurrentWeapon, 1);
	file.read((char*)&m_fOption, 1);
	file.read((char*)&m_PlusActionInfo, 4);

	BYTE pt;
	file.read((char*)&pt, 1);
	m_PacketType = (ACTIONINFO_PACKET)pt;


	file.read((char*)&m_Delay, 2);
	file.read((char*)&m_Value, 4);
	file.read((char*)&m_SoundID, SIZE_SOUNDID);
	file.read((char*)&m_MainNode, 4);
	
	// 결과 
	file.read((char*)&m_ActionResultID, SIZE_ACTIONRESULTID);
	file.read((char*)&m_ActionResultValue, 4);

	WORD es;
	file.read((char*)&es, 2);
	m_EffectStatus = (EFFECTSTATUS)es;
	bool bAttack;
	file.read((char*)&bAttack, 1);
	m_bAttack = bAttack;
	file.read((char*)&m_fSelectCreature, 1);

	char flag = 0;
	file.read((char*)&flag, sizeof(char) );

	m_bUseGrade = (flag & 0x1) != 0;
	m_bUseActionStep = (flag & 0x2) != 0;
	m_bAttachSelf = (flag & 0x4) != 0;		
	
	if( m_bUseActionStep )
	{
	int i;
		for( i = 0;i<MAX_ACTION_STEP; i++)
		{
			file.read((char*)&m_ActionStep[i],sizeof( TYPE_ACTIONINFO ) );
		}
	}
	file.read((char*)&m_Parent, sizeof(TYPE_ACTIONINFO ) );
	file.read((char*)&m_MasterySkillStep, 1 );
	file.read((char*)&m_bIgnoreFailDelay, 1 );
/*
	file.read((char*)&flag, sizeof( char ) );

	m_bAdvancementClassSkill = (flag & 0x1 ) != 0;
	m_bNonAdvancementClassSkill = (flag & 0x2) != 0;
*/
	// 각 단계에 대한 정보
	CTypeTable<ACTION_INFO_NODE>::LoadFromFile(file);
}

void		MActionInfo::SetActionStep(BYTE step,  TYPE_ACTIONINFO action)
{
	if(!IsUseActionStep() )
		return;

	if(step < 0 || step >= MAX_ACTION_STEP )
		return;

	m_ActionStep[step] = action;
}

TYPE_ACTIONINFO	MActionInfo::GetActionStep(BYTE step)
{
	if(!IsUseActionStep() || step < 0 || step >= MAX_ACTION_STEP )
		return ACTIONINFO_NULL;

	return m_ActionStep[step];
}

void		MActionInfo::SetParentActionInfo( TYPE_ACTIONINFO Parent )
{
	m_Parent = Parent;
}

TYPE_ACTIONINFO	MActionInfo::GetParentActionInfo()
{
	return m_Parent;
}

//----------------------------------------------------------------------
//
//							MActionInfoTable
//
//----------------------------------------------------------------------
MActionInfoTable::MActionInfoTable()
{
	m_nMinResultActionInfo = 0;
	m_nMaxResultActionInfo = 0;
}

MActionInfoTable::~MActionInfoTable()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// 피 없는 모드..
//----------------------------------------------------------------------
void			
MActionInfoTable::SetChildMode()
{
	for (int i=0; i<m_Size; i++)
	{
		m_pTypeInfo[i].SetChildMode();
	}

	m_pTypeInfo[RESULT_VAMPIRE_DIE].SetStartFrameAll( 8 );	
	m_pTypeInfo[RESULT_VAMPIRE_DIE].SetCastingStartFrameAll( 8 );
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void		
MActionInfoTable::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&m_nMinResultActionInfo, 4);
	file.write((const char*)&m_nMaxResultActionInfo, 4);

	CTypeTable<MActionInfo>::SaveToFile( file );
}
		
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void		
MActionInfoTable::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&m_nMinResultActionInfo, 4);
	file.read((char*)&m_nMaxResultActionInfo, 4);

	CTypeTable<MActionInfo>::LoadFromFile( file );
}
