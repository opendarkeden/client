//----------------------------------------------------------------------
// MActionInfo.h
//----------------------------------------------------------------------
//
//  = 연결되는 Effect들로 하나의 현상(필살기..)을 표현하기 위해 필요한 정보
//
//  = ActionInfoNode
//    - EffectGeneratorTableID
//    - BltType
//    - FrameID
//    - 지속시간
//
//  = ActionInfoNode의 array
//
//----------------------------------------------------------------------
/*

  [ 나(Player)의 경우 ]

	Action이 실행되는 시점에서 
		Server로 사용 기술 Packet을 보내고
		g_ActionInfoTable을 참조해서 기술 사용 때의 효과를 표현해준다.

	기술 사용 도중에 Server에서 결과Packet이 도착하면,
		(Player의 진행중인 기술ID가 같은 경우)
		그 결과들을 ActionResult로 만들어서 
		Player에 있는 EffectTarget의 ActionResult*에 설정한다.


	아닌 경우, 
		그 결과들을 ActionResult로 만들어서 바로 결과를 적용시킨다.
		EffectTarget을 바로 생성하고 EffectGenerator를 통해서..
		Effect생성..... 등등..


  [ 다른 사람의 경우 ]

	시작Action과 결과Action과 결과들...이 하나의 Packet으로 넘어온다.
	결과Action들을 ActionResult로 만들어서 
	시작Action을 실행할때, EffectTarget에 ActionResult*에 설정한다.


  (*) EffectGenerator에서 ActionResult가 


  [ 참고 ]

  - 어떤 기술을 세부적인 단계(ACTION_INFO_NODE)로 나눠보면..
	다음이 대표적인 경우이다.

  ==> 캐스팅동작+캐스팅Effect --> 기술시작Effect --> 기술진행Effect --> 기술끝Effect
     
  ==> [변형]  캐스팅동작 --> 기술진행Effect --> 기술진행Effect --> .... 등도 물론 있다.

  - 여기서 캐스팅동작+캐스팅Effect는 MActionInfo로,
    그 뒤의 동작단계들은 하나의 ACTION_INFO_NODE들로 나타낼 수 있다.

  - casting Effect는 AttachEffect로.. 캐릭터에 붙어서 표현되는 것이다.
  - Casting ActionInfo는 더 복잡한 Casting을 표현할 수 있다.

  - 하나의 기술(MActionInfo)은 그 기술 고유의 정보와
    ACTION_INFO_NODE의 array로서 표현이 된다.

  - 한 기술에 대한 ACTION_INFO_NODE 중에서는 '기본 동작' 혹은 '진행 동작' 등의 
    의미로 불릴만한 동작이 있을 수 있다.
	(순간적으로 발동되는 기술이라면 없을 수도 있다.)
	이런 단계를 MainNode라 부른다.
	
  - MainNode는 기술의 본격적인(-_-;) 시작을 의미한다.
    예를 들면, 'Light기술'같은 것이 레벨에 따라서
	사용 시간이 달라질 수 있다.
	이 때, 사용 시간을 표현하는 단계를 MainNode로 설정할 수 있다.
	기술이 바로 적용되어야하는 경우 MainNode부터 기술이 시작하게 된다.

  - node에서 bDelayNode는
    delay를 적용받는 node를 의미한다.
	MainNode와 비슷하다고 할 수 있으나
	실제적인 delay를 적용받는 node이므로 의미가 좀 다르다.
	MainNode는 하나밖에 없지만, bDelayNode는 여러개가 될 수 있다.
	

  -	이미 MainNode가 진행중인 상태를 바로 표현해줘야 하는 경우도 있다.
	(다른 화면에 있다가 시야에 보이는 경우 Server에서 정보를 보내주겠지)
	이런 경우.. 캐스팅동작이나 기술시작Effect를 보여주지 않고 바로 
	MainNode를 표현해줘야 한다.

  - ResultTime은 결과를 처리해주는 시점이다.
    여러가지 ACTION_INFO_NODE중에서 Server에서 받은 결과(!)를 
	적용시켜주는 시점..
	설정이 안된 경우.. 끝~~에서 처리해주면 되겠지..

  - StartWithCasting?
    기술의 시작은 
	(1)캐릭터가 Casting동작을 시작하면서
	(2)캐릭터의 Casting동작이 끝나면서(StartAfterCasting)
	...의 두 가지 경우가 있다.

  - Casting?
     EffectSpriteTypeID로.. AttachEffect 하나만 표현되는 경우가 대부분이다.
	 하지만, CastingActionInfo로.. 특정한 ActionInfo로 표현해야 되는 경우가 있다.
	 bCastingActionInfo가 설정되어 있다면 ResultActionInfo는 없다고 가정한다. 

*/
//----------------------------------------------------------------------


#ifndef	__MACTIONINFOTABLE_H__
#define	__MACTIONINFOTABLE_H__

#include <Windows.h>
#include "MTypeDef.h"
#include "DrawTypeDef.h"
#include "MActionResultDef.h"
#include "MEffectStatusDef.h"
#include "CTypeTable.h"
//#include "SkillDef.h"
#include "MString.h"
class ofstream;
class ifstream;

//----------------------------------------------------------------------
// 기술의 속성에 대한 flag
//----------------------------------------------------------------------
#define	FLAG_ACTIONINFO_TARGET_NONE			0		// 사용할 수 없음
#define	FLAG_ACTIONINFO_TARGET_SELF			0x01	// 본인에게 사용
#define FLAG_ACTIONINFO_TARGET_OTHER		0x02	// 타인에게 사용
#define	FLAG_ACTIONINFO_TARGET_ZONE			0x04	// Zone에 사용
#define	FLAG_ACTIONINFO_TARGET_ITEM			0x08	// Item에 사용

//----------------------------------------------------------------------
// packet 종류
//----------------------------------------------------------------------
enum ACTIONINFO_PACKET {
	ACTIONINFO_PACKET_NONE,
	ACTIONINFO_PACKET_SELF,			// 본인에게 사용
	ACTIONINFO_PACKET_OTHER,		// 타인에게 사용
	ACTIONINFO_PACKET_ZONE,			// Zone에 사용
	ACTIONINFO_PACKET_ITEM,			// Item에 사용
	ACTIONINFO_PACKET_BLOOD_DRAIN,	// 흡협인 경우	
	ACTIONINFO_PACKET_THROW_BOMB,	// 폭탄 던지는 경우
	ACTIONINFO_PACKET_UNTRANSFORM,	// 변신 풀기
	ACTIONINFO_PACKET_VISIBLE,		// invisible 풀기
	ACTIONINFO_PACKET_ABSORB_SOUL,	// 흡영인 경우	
};

//----------------------------------------------------------------------
// 기술의 시작 위치에 대한 flag
//----------------------------------------------------------------------
#define	FLAG_ACTIONINFO_START_USER				0x01	// 사용자 위치에서 시작
#define	FLAG_ACTIONINFO_START_TARGET			0x02	// 목표 위치에서 시작
#define	FLAG_ACTIONINFO_START_SKY				0x04	// 공중에서 시작

//----------------------------------------------------------------------
// 기술의 사용 대상
//----------------------------------------------------------------------
#define	FLAG_ACTIONINFO_USER_ALL				0xFF	// 모든 사용자
#define	FLAG_ACTIONINFO_USER_NONCREATURE		0x01	// creature가 아닌 것이 사용
#define	FLAG_ACTIONINFO_USER_SLAYER				0x02	// slayer전용 기술
#define	FLAG_ACTIONINFO_USER_VAMPIRE			0x04	// vampire전용 기술
#define	FLAG_ACTIONINFO_USER_NPC				0x08	// npc전용 기술(-_-;)
#define	FLAG_ACTIONINFO_USER_MONSTER			0x10	// slayer전용 기술
#define	FLAG_ACTIONINFO_USER_OUSTERS			0x20	// ousters전용 기술

//----------------------------------------------------------------------
// 기술 사용 가능 무기
//----------------------------------------------------------------------
// 총을 설정해두면.. 총알이 있어야 나간다.
#define FLAG_ACTIONINFO_WEAPON_ALL				0x007F	// 모든 무기
#define FLAG_ACTIONINFO_WEAPON_HAND				0x0001	// 맨손
#define FLAG_ACTIONINFO_WEAPON_SWORD			0x0002	// 칼
#define FLAG_ACTIONINFO_WEAPON_BLADE			0x0004	// 검
#define FLAG_ACTIONINFO_WEAPON_GUN_ALL			0x0078	// TR
#define FLAG_ACTIONINFO_WEAPON_GUN_SR			0x0008	// TR
#define FLAG_ACTIONINFO_WEAPON_GUN_SG			0x0010	// SG
#define FLAG_ACTIONINFO_WEAPON_GUN_AR			0x0020	// AR
#define FLAG_ACTIONINFO_WEAPON_GUN_SMG			0x0040	// SMG
#define FLAG_ACTIONINFO_WEAPON_SHIELD			0x0080	// 방패
#define FLAG_ACTIONINFO_WEAPON_HOLY_WATER		0x0100	// holy water사용하는 기술
#define FLAG_ACTIONINFO_WEAPON_BOMB				0x0200	// 폭탄 사용하는 기술
#define FLAG_ACTIONINFO_WEAPON_CHAKRAM			0x0400	// 챠크람 사용하는 기술

//----------------------------------------------------------------------
// 현재 들고 있는 무기의 적용을 받는다.
//----------------------------------------------------------------------
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_ACTION					0x01	// basic actionInfo의 action적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_RANGE					0x02	// basic actionInfo의 range적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_SOUND					0x04	// basic actionInfo의 sound적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_STARTFRAME		0x08	// basic actionInfo의 GetCastingStartFrame()적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_FRAMES			0x10	// basic actionInfo의 GetCastingFrames()적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_DELAY					0x20	// basic actionInfo의 GetDelay()적용
#define	FLAG_ACTIONINFO_CURRENT_WEAPON_ACTIONEFFECTSPRITETYPE	0x40	// (엽기적으로 길군 - -;)

//----------------------------------------------------------------------
// option flag
//----------------------------------------------------------------------
#define	FLAG_ACTIONINFO_OPTION_RANGE_TO_DIRECTION	0x01	// range값을 direction으로 사용한다.
#define FLAG_ACTIONINFO_OPTION_USE_WITH_BLESS		0x02	// bless걸린 상태에서 사용할 수 있다.

//----------------------------------------------------------------------
// Select Creature
//----------------------------------------------------------------------
#define FLAG_ACTIONINFO_SELECT_ALL					0xFF
#define	FLAG_ACTIONINFO_SELECT_ENEMY				0x01
#define	FLAG_ACTIONINFO_SELECT_FRIEND				0x02
#define	FLAG_ACTIONINFO_SELECT_PARTY				0x04
#define	FLAG_ACTIONINFO_SELECT_GUILD				0x08
#define	FLAG_ACTIONINFO_SELECT_FOLLOWER				0x10

//----------------------------------------------------------------------
// main node NULL
//----------------------------------------------------------------------
#define	MAIN_NODE_NULL						0xFFFF

#define MAX_ACTION_STEP							5

//----------------------------------------------------------------------
// ACTION_INFO_NODE class
//----------------------------------------------------------------------
class ACTION_INFO_NODE {
	public :
		ACTION_INFO_NODE()
		{
			SoundID		= SOUNDID_NULL;
			//Light		= 1;
			LinkCount	= 0xFFFF;

			bDelayNode	= false;
			bResultTime = false;	
			bUseCoord = false;
		}
		
		~ACTION_INFO_NODE() {}
		

	public :
		TYPE_EFFECTGENERATORID		EffectGeneratorID;	// EffectGenerator ID		
		TYPE_EFFECTSPRITETYPE		EffectSpriteType;	// EffectSpriteType
		WORD						Step;				// 속도
		WORD						Count;				// 이번 node의 지속 시간
		WORD						LinkCount;			// 이번 node에 머무르는 시간 == 다음 node로 넘어갈때까지의 시간
		TYPE_SOUNDID				SoundID;			// 기술의 각 단계의 Sound ID
		//BYTE						Light;				// 빛의 밝기
		bool						bDelayNode;			// delay가 적용되는 node인가?
		bool						bResultTime;		// 결과를 보여주는 시점이다.
		bool						bUseCoord;			// Step 을 좌표로 사용한다.
		
	public :
		void			SetUseCoord()				{ bUseCoord = true; }		
		void			SetDelayNode()				{ bDelayNode = true; }
		void			SetResultTime()				{ bResultTime = true; }
		void			SetCoord(BYTE x, BYTE y)	{ if( bUseCoord ) Step = (x<<8)|y; }
		BYTE			GetX()						{ if( bUseCoord ) return Step>>8; return 0; }
		BYTE			GetY()						{ if( bUseCoord ) return Step&0xff; return 0; }

		void			operator = (const ACTION_INFO_NODE& node)
		{
			EffectGeneratorID	= node.EffectGeneratorID;
			EffectSpriteType	= node.EffectSpriteType;
			Step				= node.Step;
			Count				= node.Count;
			LinkCount			= node.LinkCount;
			SoundID				= node.SoundID;
			//Light				= node.Light;
			bResultTime			= node.bResultTime;
			bUseCoord			= node.bUseCoord;
		}

		//-------------------------------------------------------
		// 애들 버전.. -_-;
		//-------------------------------------------------------
		void			SetChildMode();

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(class ofstream& file);
		void			LoadFromFile(class ifstream& file);
};


//----------------------------------------------------------------------
// 하나의 기술에 대한 정보
// ACTION_INFO_NODE의 array 
//----------------------------------------------------------------------
class MActionInfo : public CTypeTable<ACTION_INFO_NODE> {
	public :
		MActionInfo();	
		~MActionInfo();

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void	Set(const char* name, BYTE action, TYPE_EFFECTSPRITETYPE ActionEffectSpriteType, BYTE range, BYTE fTarget, BYTE fStart=FLAG_ACTIONINFO_START_USER, WORD delay=0, int value=0)
		{
			m_Name						= name;
			m_Action					= action;
			m_ActionEffectSpriteType	= ActionEffectSpriteType;
			m_Range						= range;
			m_fTarget					= fTarget;
			m_fStart					= fStart;
			m_Delay						= delay;
			m_Value						= value;			
		}

		//------------------------------------------------------
		// female
		//------------------------------------------------------
		TYPE_EFFECTSPRITETYPE	GetActionEffectSpriteTypeFemale() const	{ return m_ActionEffectSpriteTypeFemale; }
		void		SetActionEffectSpriteTypeFemale(TYPE_EFFECTSPRITETYPE et)
		{
			m_ActionEffectSpriteTypeFemale = et; 
		}

		//------------------------------------------------------
		// Casting 관련
		//------------------------------------------------------
		bool		IsCastingEffectToSelf() const			{ return m_bCastingEffectToSelf; }
		void		SetCastingEffectToSelf()				{ m_bCastingEffectToSelf = true; }
		void		SetCastingEffectToOther()				{ m_bCastingEffectToSelf = false; }

		// speed는 slow:normal:fast = 0:1:2 이다. 귀차나서.. ㅋㅋ
		void		SetCastingStartFrameAll(int f)			{ m_CastingStartFrame[0] = m_CastingStartFrame[1] = m_CastingStartFrame[2] = f; }
		void		SetCastingFramesAll(int f)				{ m_CastingFrames[0] = m_CastingFrames[1] = m_CastingFrames[2] = f; }

		void		SetCastingStartFrame(int speed, int f)			{ m_CastingStartFrame[speed] = f; }
		void		SetCastingFrames(int speed, int f)				{ m_CastingFrames[speed] = f; }

		int			GetCastingStartFrame(int speed) const	{ return m_CastingStartFrame[speed]; }
		int			GetCastingFrames(int speed) const		{ return m_CastingFrames[speed]; }
		
		void		SetStartWithCasting()	{ m_StartFrame[0] = m_StartFrame[1] = m_StartFrame[2] = 0; }
		void		SetStartAfterCasting()	{ m_StartFrame[0] = m_StartFrame[1] = m_StartFrame[2] = 0xFFFF; }

		// 사용할려는 CastingActionInfo
		void		SetCastingActionInfo(TYPE_ACTIONINFO ai)	{ m_CastingActionInfo=ai; }		
		TYPE_ACTIONINFO	GetCastingActionInfo() const		{ return m_CastingActionInfo; }		

		// 이 기술은 CastingAction인가?
		bool		IsCastingAction() const					{ return m_bCastingAction; }		
		void		SetCastingAction()						{ m_bCastingAction=true; }
		void		UnSetCastingAction()					{ m_bCastingAction=false; }	

		
		

		//------------------------------------------------------
		// Sound ID
		//------------------------------------------------------
		void	SetSoundID(TYPE_SOUNDID id)			{ m_SoundID = id; }

		//------------------------------------------------------
		// Main Node
		//------------------------------------------------------
		void	SetMainNode(int n)					{ m_MainNode=n; }

		//------------------------------------------------------
		// attack - 공격 기술인가?
		//------------------------------------------------------
		void		SetAttack()				{ m_bAttack = TRUE; }
		void		UnSetAttack()			{ m_bAttack = FALSE; }
		BOOL		IsAttack() const		{ return m_bAttack; }

		//------------------------------------------------------
		// get
		//------------------------------------------------------
		const char*	GetName() const			{ return m_Name; }
		BYTE		GetAction() const		{ return m_Action; }
		BYTE		GetRange() const		{ return m_Range; }
		TYPE_EFFECTSPRITETYPE	GetActionEffectSpriteType() const { return m_ActionEffectSpriteType; }
		WORD		GetDelay() const		{ return m_Delay; }
		int			GetValue() const		{ return m_Value; }
		TYPE_SOUNDID	GetSoundID() const		{ return m_SoundID; }
		BOOL		HasMainNode() const		{ return m_MainNode!=MAIN_NODE_NULL; }
		int			GetMainNode() const		{ return m_MainNode; }
		
		//------------------------------------------------------
		// 목표의 속성
		//------------------------------------------------------
		BYTE		GetTarget() const		{ return m_fTarget; }
		bool		IsTargetNone() const	{ return m_fTarget==0; }
		BYTE		IsTargetSelf() const	{ return m_fTarget & FLAG_ACTIONINFO_TARGET_SELF; }
		BYTE		IsTargetOther() const	{ return m_fTarget & FLAG_ACTIONINFO_TARGET_OTHER; }
		BYTE		IsTargetZone() const	{ return m_fTarget & FLAG_ACTIONINFO_TARGET_ZONE; }
		BYTE		IsTargetItem() const	{ return m_fTarget & FLAG_ACTIONINFO_TARGET_ITEM; }

		//------------------------------------------------------
		// 보내는 packet 종류
		//------------------------------------------------------
		void		SetPacketType(ACTIONINFO_PACKET ap)		{ m_PacketType = ap; }
		ACTIONINFO_PACKET	GetPacketType() const			{ return m_PacketType; }
		bool		IsPacketTypeNone() const	{ return m_PacketType == ACTIONINFO_PACKET_NONE; }
		BYTE		IsPacketTypeSelf() const	{ return m_PacketType == ACTIONINFO_PACKET_SELF; }
		BYTE		IsPacketTypeOther() const	{ return m_PacketType == ACTIONINFO_PACKET_OTHER; }
		BYTE		IsPacketTypeZone() const	{ return m_PacketType == ACTIONINFO_PACKET_ZONE; }
		BYTE		IsPacketTypeItem() const	{ return m_PacketType == ACTIONINFO_PACKET_ITEM; }
		BYTE		IsPacketTypeThrowBomb() const	{ return m_PacketType == ACTIONINFO_PACKET_THROW_BOMB; }
		BYTE		IsPacketTypeUntransform() const	{ return m_PacketType == ACTIONINFO_PACKET_UNTRANSFORM; }
		BYTE		IsPacketTypeVisible() const		{ return m_PacketType == ACTIONINFO_PACKET_VISIBLE; }


		//------------------------------------------------------
		// 시작 위치
		//------------------------------------------------------
		BYTE		GetStart() const		{ return m_fStart; }
		bool		IsStartNone() const		{ return m_fStart==0; }
		BYTE		IsStartUser() const		{ return m_fStart & FLAG_ACTIONINFO_START_USER; }
		BYTE		IsStartTarget() const	{ return m_fStart & FLAG_ACTIONINFO_START_TARGET; }
		BYTE		IsStartSky() const		{ return m_fStart & FLAG_ACTIONINFO_START_SKY; }
		
		void		SetStartFrameAll(int f)	{ m_StartFrame[0] = m_StartFrame[1] = m_StartFrame[2] = f; }
		void		SetStartFrame(int speed, int f)	{ m_StartFrame[speed] = f; }
		int			GetStartFrame(int speed) const	{ return m_StartFrame[speed]; }

		//------------------------------------------------------
		// 기술의 사용 대상
		//------------------------------------------------------
		void	SetUser(BYTE fUser)				{ m_fUserType=fUser; }
		BYTE	GetUser() const					{ return m_fUserType; }
		void	AddUserNonCreature() 			{ m_fUserType|=FLAG_ACTIONINFO_USER_NONCREATURE; }
		void	AddUserSlayer() 				{ m_fUserType|=FLAG_ACTIONINFO_USER_SLAYER; }
		void	AddUserVampire() 				{ m_fUserType|=FLAG_ACTIONINFO_USER_VAMPIRE; }
		void	AddUserOusters() 				{ m_fUserType|=FLAG_ACTIONINFO_USER_OUSTERS; }
		void	AddUserNPC() 					{ m_fUserType|=FLAG_ACTIONINFO_USER_NPC; }
		void	AddUserMonster()				{ m_fUserType|=FLAG_ACTIONINFO_USER_MONSTER; }
		BYTE	IsUserNonCreature() const		{ return m_fUserType & FLAG_ACTIONINFO_USER_NONCREATURE; }
		BYTE	IsUserSlayer() const			{ return m_fUserType & FLAG_ACTIONINFO_USER_SLAYER; }
		BYTE	IsUserVampire() const			{ return m_fUserType & FLAG_ACTIONINFO_USER_VAMPIRE; }
		BYTE	IsUserOusters() const			{ return m_fUserType & FLAG_ACTIONINFO_USER_OUSTERS; }
		BYTE	IsUserNPC() const				{ return m_fUserType & FLAG_ACTIONINFO_USER_NPC; }
		BYTE	IsUserMonster() const			{ return m_fUserType & FLAG_ACTIONINFO_USER_MONSTER; }

		//------------------------------------------------------
		// 기술 사용 가능 무기
		//------------------------------------------------------
		WORD	IsWeaponTypeAll() const			{ return (m_fWeaponType & FLAG_ACTIONINFO_WEAPON_ALL)==FLAG_ACTIONINFO_WEAPON_ALL; }
		WORD	IsWeaponTypeGunAll() const		{ return (m_fWeaponType & FLAG_ACTIONINFO_WEAPON_GUN_ALL)==FLAG_ACTIONINFO_WEAPON_GUN_ALL; }
		WORD	IsWeaponTypeGunAny() const		{ return m_fWeaponType & FLAG_ACTIONINFO_WEAPON_GUN_ALL; }
		WORD	GetWeaponType() const			{ return m_fWeaponType; }
		void	SetWeaponType(WORD flag)		{ m_fWeaponType = flag; }
		void	AddWeaponType(WORD flag)		{ m_fWeaponType |= flag; }		

		//-------------------------------------------------------
		// 현재 들고 있는 무기
		//-------------------------------------------------------
		BYTE	IsAffectCurrentWeaponAction() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_ACTION; }
		BYTE	IsAffectCurrentWeaponRange() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_RANGE; }
		BYTE	IsAffectCurrentWeaponSound() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_SOUND; }
		BYTE	IsAffectCurrentWeaponCastingStartFrame() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_STARTFRAME; }
		BYTE	IsAffectCurrentWeaponCastingFrames() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_FRAMES; }
		BYTE	IsAffectCurrentWeaponDelay() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_DELAY; }
		BYTE	IsAffectCurrentWeaponActionEffectSpriteType() const	{ return m_fCurrentWeapon & FLAG_ACTIONINFO_CURRENT_WEAPON_ACTIONEFFECTSPRITETYPE; }
		int		GetAffectCurrentWeaponActionInfoPlus() const	{ return m_PlusActionInfo; }
		void	SetAffectCurrentWeaponAction()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_ACTION; }
		void	SetAffectCurrentWeaponRange()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_RANGE; }
		void	SetAffectCurrentWeaponSound()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_SOUND; }
		void	SetAffectCurrentWeaponCastingStartFrame()	{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_STARTFRAME; }
		void	SetAffectCurrentWeaponCastingFrames()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_CASTING_FRAMES; }
		void	SetAffectCurrentWeaponDelay()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_DELAY; }
		void	SetAffectCurrentWeaponActionEffectSpriteType()		{ m_fCurrentWeapon |= FLAG_ACTIONINFO_CURRENT_WEAPON_ACTIONEFFECTSPRITETYPE; }
		void	SetAffectCurrentWeaponActionInfoPlus(int pai)	{ m_PlusActionInfo = pai; }
				
		//-------------------------------------------------------
		// option flag
		//-------------------------------------------------------
		BYTE	HasOption() const					{ return m_fOption; }
		BYTE	IsOptionRangeToDirection() const	{ return m_fOption & FLAG_ACTIONINFO_OPTION_RANGE_TO_DIRECTION; }
		BYTE	IsOptionUseWithBless() const		{ return m_fOption & FLAG_ACTIONINFO_OPTION_USE_WITH_BLESS; }
		void	SetOptionRangeToDirection()			{ m_fOption |= FLAG_ACTIONINFO_OPTION_RANGE_TO_DIRECTION; }
		void	SetOptionUseWithBless()				{ m_fOption |= FLAG_ACTIONINFO_OPTION_USE_WITH_BLESS; }
		

		//-------------------------------------------------------
		// 기술의 결과 종류
		//-------------------------------------------------------
		// ActionResultID에 따라서 반드시 value를 설정해야 하는 경우가 있다.
		//
		//				id						value
		//
		// ACTIONRESULTNODE_ACTIONINFO :	실행할려는 Skill ID
		//-------------------------------------------------------
		void	SetActionResult(TYPE_ACTIONRESULTID id, int value=0)	{ m_ActionResultID = id; m_ActionResultValue = value; }		
		TYPE_ACTIONRESULTID		GetActionResultID() const				{ return m_ActionResultID; }
		int		GetActionResultValue() const							{ return m_ActionResultValue; }

		//-------------------------------------------------------
		// EffectStatus
		//-------------------------------------------------------
		void			SetEffectStatus(EFFECTSTATUS effectStatus)		{ m_EffectStatus = effectStatus; }
		EFFECTSTATUS	GetEffectStatus() const							{ return m_EffectStatus; }

		//-------------------------------------------------------
		// 반복 action
		//-------------------------------------------------------
		int				IsUseRepeatFrame() const							{ return m_bUseRepeatFrame; }
		void			UseRepeatFrame()									{ m_bUseRepeatFrame = true; }
		void			SetRepeatFrame(int speed, int start, int frames)			{ m_RepeatStartFrame[speed] = start; m_RepeatEndFrame[speed] = start+frames-1; }
		int				GetRepeatStartFrame(int speed) const						{ return m_RepeatStartFrame[speed]; }
		int				GetRepeatEndFrame(int speed) const						{ return m_RepeatEndFrame[speed]; }

		void			SetRepeatLimit(WORD limit)							{ m_RepeatLimit = limit; }
		WORD			GetRepeatLimit() const								{ return m_RepeatLimit; }
		
		//-------------------------------------------------------
		// 피 없는 모드..
		//-------------------------------------------------------
		void			SetChildMode();
		
		//-------------------------------------------------------
		// SelectCreature- 기술의 선택 대상
		//-------------------------------------------------------
		void		SetSelectCreatureAll()				{ m_fSelectCreature = FLAG_ACTIONINFO_SELECT_ALL; }
		void		SetSelectCreature(BYTE fSelect)		{ m_fSelectCreature = fSelect; }
		BYTE		GetActionTarget() const				{ return m_fSelectCreature; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);

		//-------------------------------------------------------
		// Grade 
		//-------------------------------------------------------
		void		SetUseActionGrade() {	m_bUseGrade = true; }
		void		UnsetUseActionGrade() { m_bUseGrade = false; }
		bool		IsUseActionGrade() { return m_bUseGrade; }

		//-------------------------------------------------------
		// ActionStep
		//-------------------------------------------------------
		void		SetUseActionStep()	{ m_bUseActionStep = true; }
		void		SetActionStep(BYTE step,  TYPE_ACTIONINFO action);
		bool		IsUseActionStep() { return m_bUseActionStep; }
		TYPE_ACTIONINFO	GetActionStep(BYTE step);

		void		SetParentActionInfo( TYPE_ACTIONINFO Parent );
		TYPE_ACTIONINFO	GetParentActionInfo();

		//-------------------------------------------------------
		// ForceSelectEffectTarget
		//-------------------------------------------------------
		void		SetAttachSelf()	{ m_bAttachSelf = true; }
		bool		IsAttachSelf() { return m_bAttachSelf; }

		//-------------------------------------------------------
		// MasterySkill
		//-------------------------------------------------------
		void		SetMasterySkillStep(BYTE step = 3)	{ m_MasterySkillStep = step; }
		bool		IsMasterySkillStep() { return m_MasterySkillStep!=0; }
		BYTE		GetMasterySkillStep() { return m_MasterySkillStep; }

		bool		IsIgnoreSkillFailDelay() { return m_bIgnoreFailDelay;}
		void		SetSkillFailDelay(bool bflag = true) {	m_bIgnoreFailDelay = bflag;}
	protected :
		MString						m_Name;						// 기술 이름		
		BYTE						m_Action;					// 보여지는 동작

		bool						m_bUseActionStep;
		TYPE_ACTIONINFO				m_ActionStep[MAX_ACTION_STEP];			// 액션 스텝
		TYPE_ACTIONINFO				m_Parent;				

		// casting 정보
		TYPE_ACTIONINFO				m_CastingActionInfo;		// 사용할려는 CastingActionInfo의 ID
		bool						m_bCastingAction;			// 이거는 casting action인가?
		TYPE_EFFECTSPRITETYPE		m_ActionEffectSpriteType;	// Casting EffectSpriteType		
		TYPE_EFFECTSPRITETYPE		m_ActionEffectSpriteTypeFemale;	// female용 effect - -;
		int							m_CastingStartFrame[3];		// Casting 시작 frame
		int							m_CastingFrames[3];			// casting 지속 frame
		int							m_PlusActionInfo;			// 임시로.. 그러나 영원히.. -_-;
		bool						m_bCastingEffectToSelf;		// 자기한테 casting effect 붙인다.(default) 2001.9.4추가
		
		//WORD						m_CastingTime;				// Casting Sprite 표현 시간
		BYTE						m_Range;					// 사용 가능 거리		
		BYTE						m_fTarget;					// 목표에 대한 속성
		BYTE						m_fStart;					// 시작 위치에 대한 속성
		BYTE						m_fUserType;				// 시작 위치에 대한 속성
		WORD						m_fWeaponType;				// 기술을 사용하는데 필요한 무기
		BYTE						m_fCurrentWeapon;			// 현재 들고 있는 무기의 적용을 받는다.

		ACTIONINFO_PACKET			m_PacketType;				// 보내는 packet종류

		WORD						m_Delay;					// 기술 사용 delay시간	
		int							m_Value;					// 값
		TYPE_SOUNDID				m_SoundID;					// 기술 사용 시점의 Sound ID
		int							m_MainNode;					// 기본이 되는 순간, 반복되는 순간..등

		BOOL						m_bAttack;					// 공격하는 기술(상대에게 피해를 입히는)인가?

		
		int							m_StartFrame[3];				// Effect가 시작하는 Frame		
		
		TYPE_ACTIONRESULTID			m_ActionResultID;			// 기술의 결과 종류
		int							m_ActionResultValue;		// 결과에 관련된 값

		BYTE						m_fOption;

		EFFECTSTATUS				m_EffectStatus;			// 결과 EffectStatus

		// 반복 action할때 (시작~끝) frame
		bool						m_bUseRepeatFrame;
		int							m_RepeatStartFrame[3];
		int							m_RepeatEndFrame[3];
		WORD						m_RepeatLimit;			// 반복회수 제한

		// 기술의 선택 대상
		BYTE						m_fSelectCreature;
		bool						m_bUseGrade;		
		bool						m_bAttachSelf;
		
		BYTE						m_MasterySkillStep;
		bool						m_bIgnoreFailDelay; //  스킬 실패해도 딜레이는 남는다.
		
		// 전직 관련 
		bool						m_bAdvancementClassSkill;		// 전직했을때만 사용가능한 스킬인가?
		bool						m_bNonAdvancementClassSkill;	// 전직 하지 않았을 때만 사용가능한 스킬인가?		
};

//----------------------------------------------------------------------
// ACTION_INFO의 array
//----------------------------------------------------------------------
class MActionInfoTable : public CTypeTable<MActionInfo> {
	public :
		MActionInfoTable();
		~MActionInfoTable();

		//---------------------------------------------
		// Set/Get Min Result ActionInfo
		//---------------------------------------------
		void	SetMinResultActionInfo(DWORD nActionInfo)	{ m_nMinResultActionInfo = nActionInfo; }
		DWORD	GetMinResultActionInfo() const				{ return m_nMinResultActionInfo; }

		//---------------------------------------------
		// Set/Get Max Result ActionInfo (포함되는 값이다)
		//---------------------------------------------
		void	SetMaxResultActionInfo(DWORD nActionInfo)	{ m_nMaxResultActionInfo = nActionInfo; }
		DWORD	GetMaxResultActionInfo() const				{ return m_nMaxResultActionInfo; }

		void	SetMinStepActionInfo(DWORD nActionInfo) {m_nMinStepActionInfo = nActionInfo; }
		DWORD	GetMinStepActionInfo()	const				{ return m_nMinResultActionInfo; }

		bool	IsResultActionInfo(TYPE_ACTIONINFO type);
		bool	IsActionInfo(TYPE_ACTIONINFO type);

		TYPE_ACTIONINFO	GetResultActionInfo(TYPE_ACTIONINFO action);

		//-------------------------------------------------------
		// 피 없는 모드..
		//-------------------------------------------------------
		void			SetChildMode();

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);

	protected :
		// ActionInfo / ResultActionInfo / ClientOnlyActionInfo의 경계값
		DWORD			m_nMinResultActionInfo;
		DWORD			m_nMaxResultActionInfo;		// 포함값
		DWORD			m_nMinStepActionInfo;
};


extern	MActionInfoTable*			g_pActionInfoTable;


#endif
