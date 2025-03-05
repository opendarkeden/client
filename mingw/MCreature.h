//----------------------------------------------------------------------
// MCreature.h
//----------------------------------------------------------------------
// 움직이는 생물에 대한 class
//----------------------------------------------------------------------
// UndergroundCreature
// GroundCreature
// FlyingCreature 로 구분된다.
//
// 모두 Z좌표를 갖고 있지만, 
// FlyingCreature만이 일정한 높이에 존재하게 된다.
//----------------------------------------------------------------------

#ifndef	__MCREATURE_H__
#define	__MCREATURE_H__

#include "MTypeDef.h"
#include "MObject.h"
#include "MAttachEffect.h"
#include "MActionInfoTable.h"
#include "MCreatureTable.h"
#include "MCreatureSpriteTable.h"
#include "MEffectStatusTable.h"
#include "MStatus.h"
#include "RaceType.h"
#include "MemoryPool.h"
#include "NicknameInfo.h"
class MItem;

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------
#define	MAX_FRAME_MOVE	16

// 공중 캐릭터의 높이(일정)
//#define	FLYINGCREATURE_HEIGHT	0	//64

class MZone;
class MActionFrameArray;
class MActionResult;

class MCreature : public MObject, public MStatus {
	//--------------------------------------------------------------
	// enum
	//--------------------------------------------------------------
	public :
		enum CLASS_TYPE
		{
			CLASS_CREATURE,
			CLASS_CREATUREWEAR,
			CLASS_NPC,
			CLASS_FAKE,
			CLASS_PLAYER
		};

		// Creature의 종류 : 지하,지상,공중
		enum MOVE_TYPE
		{
			CREATURE_UNDERGROUND = 0,
			CREATURE_GROUND,
			CREATURE_FLYING,
			CREATURE_FAKE_NO_BLOCK,		// sector에 존재하지 않는 애덜..
			CREATURE_FAKE_UNDERGROUND,
			CREATURE_FAKE_GROUND,
			CREATURE_FAKE_FLYING,
		};

		// 이동하는 방법
		enum MOVE_DEVICE
		{
			MOVE_DEVICE_WALK,		// 걸어다닌다.
			MOVE_DEVICE_RIDE,		// 모터싸이클			
			MOVE_DEVICE_SUMMON_SYLPH,		// 정령타고 슉~ 
			MOVE_DEVICE_MAX
		};

		// 이동하는 방법
		enum WEAPON_SPEED
		{
			WEAPON_SPEED_SLOW,			// 느린 무기
			WEAPON_SPEED_NORMAL,			// 보통 무기
			WEAPON_SPEED_FAST				// 빠른 무기
		};

	public :
		//----------------------------------------------------------------------
		// MoveNode
		//----------------------------------------------------------------------
		// (x,y)에서 direction방향으로 움직인다는 의미
		class MoveNode {
			public :
				int		x;
				int		y;
				int		direction;
		};

		typedef	std::list<MoveNode*>	MOVENODE_LIST;

		//----------------------------------------------------------------------
		// HPModifyList
		//----------------------------------------------------------------------
		class HPModify {
			public :
				HPModify(int value_modify, DWORD value_TickCount) { modify = value_modify; TickCount = value_TickCount; }
				int		modify;
				DWORD	TickCount;
		};

		typedef	std::list<HPModify>	HPMODIFYLIST;

	public :
		MCreature();//MActionFrameArray* pFrames);
		virtual ~MCreature();
		
		void* operator new( size_t size )
		{
			return g_CreatureMemoryPool.Alloc();
		}

		void  operator delete( void *pmem )
		{
			g_CreatureMemoryPool.Free( pmem );
		}

		virtual CLASS_TYPE	GetClassType() const	{ return CLASS_CREATURE; }

		//------------------------------------------------------
		// Init
		//------------------------------------------------------
		static void	InitMoveTable();
		static void	ReleaseMoveTable();

		//------------------------------------------------------
		// (x,y)에서 direction으로 움직인 좌표를 넘겨준다.
		//------------------------------------------------------
		static void	GetPositionToDirection(TYPE_SECTORPOSITION &x, TYPE_SECTORPOSITION &y, BYTE direction);

		int				GetHeadSkin() const;
		//-------------------------------------------------------
		// Creature Type
		//-------------------------------------------------------
		virtual void	SetCreatureType(TYPE_CREATURETYPE type);		
		TYPE_CREATURETYPE	GetCreatureType() const				{ return m_CreatureType; }

		CREATURETRIBE	GetCreatureTribe() const				{ return (*g_pCreatureTable)[m_CreatureType].GetCreatureTribe(); }
		bool			IsPlayer() const		{ return (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[m_CreatureType].SpriteTypes[m_current_sprite_index]].IsPlayerSprite() != false; }
		bool			IsPlayerOnly() const	{ return (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[m_CreatureType].SpriteTypes[m_current_sprite_index]].IsPlayerOnlySprite() != false; }
		bool			IsSlayer() const		{ return (*g_pCreatureTable)[m_CreatureType].IsSlayer(); }
		bool			IsVampire() const		{ return (*g_pCreatureTable)[m_CreatureType].IsVampire(); }
		bool			IsOusters() const		{ return (*g_pCreatureTable)[m_CreatureType].IsOusters(); }
		bool			IsNPC() const			{ return (*g_pCreatureTable)[m_CreatureType].IsNPC(); }
		int				GetActionMax() const;	
		bool			IsSlayerCharacter() const;
		Race			GetRace() const			{ if(IsSlayer())return RACE_SLAYER; else if(IsVampire())return RACE_VAMPIRE; return RACE_OUSTERS; }

		// 2004, 12, 7, sobeit add start - 운영자 인지 아닌지
		bool			IsSlayerOperator() const { return m_CreatureType == CREATURETYPE_SLAYER_OPERATOR ;}
		bool			IsVampireOperator() const { return m_CreatureType == CREATURETYPE_VAMPIRE_OPERATOR ;}
		bool			IsOustersOperator() const { return m_CreatureType == CREATURETYPE_OUSTERS_OPERATOR ;}
		bool			IsOperator() const { return (	m_CreatureType == CREATURETYPE_SLAYER_OPERATOR
													||	m_CreatureType == CREATURETYPE_VAMPIRE_OPERATOR
													||	m_CreatureType == CREATURETYPE_OUSTERS_OPERATOR);}
		// 2004, 12, 7, sobeit add end
		//------------------------------------------------------
		// IndexSet을 정해준다.
		//------------------------------------------------------
		int			GetCreatureColorSet() const	{ return (*g_pCreatureTable)[m_CreatureType].ColorSet; }
		
		//------------------------------------------------------
		// Set Same Body
		//------------------------------------------------------
		void		SetSameBody(const MCreature* pCreature);

		//------------------------------------------------------
		// 색깔 관련... 
		//------------------------------------------------------
		void		SetBodyColor1(WORD set);
		void		SetBodyColor2(WORD set);
		void		SetChangeColorSet(WORD set)	{ m_ChangeColorSet = set; }
		WORD		GetBodyColor1() const		{ return m_ColorBody1; }
		WORD		GetBodyColor2() const		{ return m_ColorBody2; }		
		WORD		GetChangeColorSet() const	{ return m_ChangeColorSet; }
		void		SetBatColor(WORD set)		{ m_BatColorSet = set; }
		WORD		GetBatColor() const			{ return m_BatColorSet; }
		// 2005, 2, 21, sobeit add start
		void		SetMasterEffectType(BYTE bType)		{ m_MasterEffectType = bType; }
		BYTE		GetMasterEffectType() const			{ return m_MasterEffectType; }
		// 2005, 2, 21, sobeit add end
		//------------------------------------------------------
		// Move Type
		//------------------------------------------------------
		MOVE_TYPE	GetMoveType() const				{ return m_MoveType; }
		void	SetMoveType(MOVE_TYPE moveType)		{ m_MoveType = moveType; }
		void	SetUndergroundCreature();
		void	SetGroundCreature();
		void	SetFlyingCreature();
		void	SetFakeCreature()				{ m_MoveType = CREATURE_FAKE_NO_BLOCK; m_Z=(*g_pCreatureTable)[m_CreatureType].FlyingHeight; }
		bool	IsUndergroundCreature()	const	{ return m_MoveType == CREATURE_UNDERGROUND || m_MoveType == CREATURE_FAKE_UNDERGROUND; }
		bool	IsGroundCreature()	const		{ return m_MoveType == CREATURE_GROUND || m_MoveType == CREATURE_FAKE_GROUND; }
		bool	IsFlyingCreature()	const		{ return m_MoveType == CREATURE_FLYING || m_MoveType == CREATURE_FAKE_FLYING; }
		bool	IsFakeCreature()	const		{ return m_MoveType == CREATURE_FAKE_NO_BLOCK || m_MoveType == CREATURE_FAKE_UNDERGROUND || m_MoveType == CREATURE_FAKE_GROUND || m_MoveType == CREATURE_FAKE_FLYING; }

		//------------------------------------------------------
		// 성별
		//------------------------------------------------------
		//bool	IsMale() const					{ return (*g_pCreatureTable)[m_CreatureType].bMale; }
		//bool	IsFemale() const				{ return !(*g_pCreatureTable)[m_CreatureType].bMale; }
		void	SetMale(bool bMale=true)		{ m_bMale = bMale; }
		void	SetFemale()						{ m_bMale = false; }
		bool	IsMale() const					{ return m_bMale; }
		bool	IsFemale() const				{ return !m_bMale; }

		//------------------------------------------------------
		// 키 
		//------------------------------------------------------
		int		GetHeight() const				{ if (m_bAlive) return (*g_pCreatureTable)[m_CreatureType].Height; return (*g_pCreatureTable)[m_CreatureType].DeadHeight; }
		
		//------------------------------------------------------
		// 이동하는 방법
		//------------------------------------------------------	
		void		SetMoveDevice(MOVE_DEVICE md);
		MOVE_DEVICE	GetMoveDevice()	const			{ return m_MoveDevice; }

		//------------------------------------------------------
		// 움직이기 
		//------------------------------------------------------		
		virtual void	Action();			// 현재 행동을 취하도록 한다.
		virtual BOOL	IsStop();
		virtual void	SetStop();

		//------------------------------------------------------		
		// Buffering된 다음의 이동 정보를 가져온다.
		//------------------------------------------------------		
		virtual bool	AffectMoveBuffer();		
		virtual void	AffectMoveBufferAll();		
		void			ReleaseMoveBuffer();

		//----------------------------------------------------------
		// 상태 값 바꾸기
		//----------------------------------------------------------
		virtual void	SetStatus(DWORD n, DWORD value);

		//----------------------------------------------------------
		// 변신
		//----------------------------------------------------------
		virtual bool	ChangeToSlayer();
		virtual bool	ChangeToVampire();

		//----------------------------------------------------------
		// item사용 가능 여부 체크
		//----------------------------------------------------------
		virtual void	CheckAffectStatus(MItem* pItem);	// 특정 아이템

		//------------------------------------------------------
		//	set
		//-------------------------------------x-----------------
		void	SetName(const char* pName);
		bool	MovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);	// Zone의 Sector에서도 이동한다.
		void	SetZone(MZone* pZone)		{ m_pZone = pZone; }
		void	SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		void	SetDirection(BYTE d)		{ m_Direction = d; }
		void	SetCurrentDirection(BYTE d)		{ m_CurrentDirection = d; }
		void	SetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);		
		virtual void	SetAction(BYTE action);
		//void	SetFrame(BYTE f)			{ m_ActionCount = f; }
		//void	SetZ(short z)				{ m_Z = z; }	
		
		//----------------------------------------------------------
		// 방향 설정
		//----------------------------------------------------------
		bool	ChangeNearDirection();	// CurrentDirection을 Direction으로 자연스럽게 바꾼다.

		//----------------------------------------------------------
		// Server
		//----------------------------------------------------------
		virtual void		SetServerPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);

		// 다음에 할 행동 설정.. 
		virtual void	SetNextAction(BYTE action);				
		virtual void	SetNextActionToMove();


		//------------------------------------------------------
		// get
		//------------------------------------------------------		
		const char*	GetName() const				{ return m_pName; }
		int	GetCX()	const				{ return m_cX; }	// 한번에 이동하는 pixel x
		int	GetCY()	const				{ return m_cY; }	// 한번에 이동하는 pixel y
		int	GetSX()	const				{ return m_sX; }	// sector에서 떨어진 X값
		int	GetSY()	const				{ return m_sY; }	// sector에서 떨어진 Y값
		TYPE_SECTORPOSITION	GetServerX() const	{ return m_ServerX; }
		TYPE_SECTORPOSITION	GetServerY() const	{ return m_ServerY; }
		BYTE		GetAction()	const			{ return m_Action; }
		BYTE		GetNextAction()	const		{ return m_NextAction; }
		BYTE		GetDirection()	const		{ return m_CurrentDirection; }
		BYTE		GetServerDirection()	const		{ return m_Direction; }
		short		GetZ() const				{ return m_Z; }
		BYTE		GetActionCount() const		{ return m_ActionCount; }
		BYTE		GetActionCountMax() const;//{ return m_ActionCountMax; }
		BYTE		GetMoveCount() const		{ return m_MoveCount; }
		BYTE		GetMoveCountMax() const		{ return m_MoveCountMax; }
		BYTE		GetFrame() const			
		{ 
			// 특별한 action이 없이 움직이고 있는 경우
			if (m_ActionCount>=GetActionCountMax())
			{
				// 다 움직여서 정지하고 있는 경우는...action을 보여줘야 한다.
				return (m_MoveCount<m_MoveCountMax)?m_MoveCount:
						(m_ActionCount==0)? 0 : GetActionCountMax()-1; 
			}
			// action을 표현해주는 경우
			else
			{
				return (m_ActionCount<GetActionCountMax())?m_ActionCount:GetActionCountMax()-1; 				
			}
		}

		// nActionInfo에 적절한 action을 찾는다.
		int			GetActionInfoAction(TYPE_ACTIONINFO nActionInfo, bool IsSelfAction = false);
		int			GetActionInfoCastingStartFrame(TYPE_ACTIONINFO nActionInfo);
		int			GetActionInfoCastingFrames(TYPE_ACTIONINFO nActionInfo);
		int			GetActionInfoDelay(TYPE_ACTIONINFO nActionInfo);
		int			GetActionInfoActionEffectSpriteType(TYPE_ACTIONINFO nActionInfo);
		int			GetActionInfoRepeatStartFrame(TYPE_ACTIONINFO nActionInfo);
		int			GetActionInfoRepeatEndFrame(TYPE_ACTIONINFO nActionInfo);
		

		// nActionInfo에 적절한 casting sound를 찾는다.
		TYPE_SOUNDID	GetCastingSoundID(TYPE_ACTIONINFO nActionInfo);

		void		GetNextPosition(BYTE direction, POINT &next);
		//void		GetNextPosition(POINT &next);
		void		GetNextPosition(TYPE_SECTORPOSITION &sX, TYPE_SECTORPOSITION &sY);
		int			GetCounterDirection( int d );
		
		// LightSight나 MaxEffectLight 중에서 큰 값을 넘겨준다. (현재의 가장 큰 시야)
		//char		GetMaxLightSight() const			{ return (m_MaxEffectLight > m_LightSight)? m_MaxEffectLight : m_LightSight; }
		BYTE		GetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);

		// Pixel좌표
		int			GetPixelX() const;
		int			GetPixelY() const;

		// item count
		int			GetItemCount() const	{ return m_ItemCount; }
		void		SetItemCount(int c)		{ m_ItemCount = c; }

		// ( 기술ID, Effect DelayFrame )
		TYPE_ACTIONINFO	GetDelayActionInfo() const		{ return m_DelayActionInfo; }
		DWORD			GetEffectDelayFrame() const		{ return m_EffectDelayFrame; }
		void			SetEffectDelayFrame(TYPE_ACTIONINFO ai, DWORD f)	{ m_DelayActionInfo=ai; m_EffectDelayFrame = f; }


		//------------------------------------------------------
		// 캐릭터 정보
		//------------------------------------------------------
		TYPE_FRAMEID	GetCreatureFrameID(int index) const	{ return m_CreatureFrameIDs[index]; }
		int				GetCreatureFrameCount()	  const { return m_CreatureFrameIDs.GetSize();}

		BOOL		IsExistBody() const			{ return m_bExistBody; }
		
		//------------------------------------------------------
		// 기술
		//------------------------------------------------------
		void				SetBasicActionInfo( TYPE_ACTIONINFO n )		{ m_nBasicActionInfo = n; }
		TYPE_ACTIONINFO		GetBasicActionInfo() const;
		virtual void		SetSpecialActionInfo( TYPE_ACTIONINFO n )	{ m_nSpecialActionInfo = n; }
		void				SetSpecialActionInfoNULL()					{ m_nSpecialActionInfo = ACTIONINFO_NULL; }
		TYPE_ACTIONINFO		GetSpecialActionInfo() const				{ return m_nSpecialActionInfo; }		
		TYPE_ACTIONINFO		GetUsedActionInfo() const					{ return m_nUsedActionInfo; }

		//----------------------------------------------------------
		// 기술 목표는?
		//----------------------------------------------------------
		BOOL	IsSpecialActionInfoTargetSelf()	const	{ return (m_nSpecialActionInfo==ACTIONINFO_NULL)?FALSE : (*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetSelf(); }	
		BOOL	IsSpecialActionInfoTargetOther() const	{ return (m_nSpecialActionInfo==ACTIONINFO_NULL)?FALSE : (*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetOther(); }
		BOOL	IsSpecialActionInfoTargetZone() const	{ return (m_nSpecialActionInfo==ACTIONINFO_NULL)?FALSE : (*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetZone(); }
		BOOL	IsSpecialActionInfoTargetItem() const	{ return (m_nSpecialActionInfo==ACTIONINFO_NULL)?FALSE : (*g_pActionInfoTable)[m_nSpecialActionInfo].IsTargetItem(); }		
		

		// 복장
		virtual BOOL		IsWear() const					{ return FALSE; }

		//------------------------------------------------------
		// Chatting String
		//------------------------------------------------------
		void		SetChatString(char *str, COLORREF color = RGB(255, 255, 255));
		void		SetPersnalString(char *str, COLORREF color = RGB(255, 255, 255));
		const char*	GetChatString(BYTE i);
		void		ClearChatString();
		void		FadeChatString();
		COLORREF	GetChatColor()		{ return m_ChatColor; }
		COLORREF	GetOriChatColor()	{ return m_OriChatColor; }
		DWORD		GetChatTime() const	{ return m_ChatTime; }

		//------------------------------------------------
		//
		// Effect 관련...
		//
		// AttachEffect는 캐릭터 몸에 붙어있는 effect를 의미한다.
		// GroundAttachEffect는 캐릭터의 발 밑에 붙어있는 effect를 의미한다.
		// 근데, AttachEffect가 GroundAttachEffect도 포함하는 의미이기도 한다.
		// (기존에 기획에 없던 내용이라서 추가하면서 개가 된 수많은 코드 중의 하나이다. 1818!)
		//
		//------------------------------------------------
		bool		IsExistAttachEffect() const		{ return !m_listEffect.empty(); }
		bool		IsExistGroundAttachEffect() const		{ return !m_listGroundEffect.empty(); }
		int			GetAttachEffectSize() const		{ return m_listEffect.size(); }
		int			GetGroundAttachEffectSize() const		{ return m_listGroundEffect.size(); }
		void		ClearAttachEffect();
		

		// 여러가지 붙어있는 Effect를 표현하는 색깔 중에서 --> 현재 출력할려는 색
		WORD		GetAttachEffectColor() const	{ return m_AttachEffectColor; }

		// AttachEffect의 시야 추가/삭제
		//void		SetLightSightAttachEffect();
		//void		UnSetLightSightAttachEffect();

		// 외부에서 iterator로 작업을 할 수 있도록..
		ATTACHEFFECT_LIST::const_iterator GetAttachEffectIterator() const { return m_listEffect.begin(); }
		ATTACHEFFECT_LIST::const_iterator GetGroundAttachEffectIterator() const { return m_listGroundEffect.begin(); }

		// Add/Update
		MAttachEffect*		CreateAttachEffect(TYPE_EFFECTSPRITETYPE type, DWORD delayFrame, DWORD linkDelayFrame, BOOL bGroundEffect=FALSE, MEffect::EFFECT_TYPE eType=MEffect::EFFECT_ATTACH);
		virtual bool		AddEffectStatus(EFFECTSTATUS status, DWORD delayFrame);
		virtual bool		RemoveEffectStatus(EFFECTSTATUS status);		
		bool				HasEffectStatus(EFFECTSTATUS status) const		{ return m_bEffectStatus[status]; }
		bool				IsAvailableEffectStatus() const		{ return m_bEffectStatus != NULL; }

		virtual void		UpdateAttachEffect();

		void				RemoveGlacierEffect();
		void				Remove_Curse_Paralsis_Effect();

		//char		GetMaxEffectLight() const		{ return m_MaxEffectLight; }

		//------------------------------------------------------
		// 죽음..
		//------------------------------------------------------
		virtual void		SetDead();		// 죽는 모습 보여준다.
		virtual void		SetAlive();		// 살아난다.
		virtual void		SetCorpse();	// 죽어있는 시체 상태
		virtual bool		IsDead() const		{ return !m_bAlive; }
		virtual bool		IsAlive() const		{ return m_bAlive; }		
		
		//------------------------------------------------------
		// Invisible
		//------------------------------------------------------
		int					IsInvisible() const			{ return m_InvisibleCount; }
		void				SetInvisible();
		void				SetInvisibleSoon();	// 즉시 invisible로 만든다.
		void				SetVisible();
		void				SetVisibleSoon();
		int					GetInvisibleCount() const	{ return m_InvisibleCount; }
		void				UpdateInvisible();

		//------------------------------------------------------
		// Level Name
		//------------------------------------------------------
		void				SetLevelName(int ln);
		BOOL				HasLevelName() const		{ return m_LevelName; }
		const char*			GetLevelName() const;

		//------------------------------------------------------
		// Guild
		//------------------------------------------------------
		void				SetGuildNumber(int gn)		{ m_GuildNumber = gn; }
		int					GetGuildNumber() const		{ return m_GuildNumber; }		
		
		void				SetUnionGuildID(int gn)		{ m_nUnionGuildID = gn; }
		int					GetUnionGuildID() const		{ return m_nUnionGuildID; }		

		//------------------------------------------------------
		// Weapon Speed
		//------------------------------------------------------
		WEAPON_SPEED		GetWeaponSpeed() const			{ return m_WeaponSpeed; }
		void				SetWeaponSpeed(int speed);
		void				SetWeaponSpeed(WEAPON_SPEED ws)	{ m_WeaponSpeed = ws; }

		//------------------------------------------------------
		// Head
		//------------------------------------------------------
		bool				CanRemoveHead() const			{ return (*g_pCreatureTable)[m_CreatureType].bHeadCut; }
		void				RemoveHead()					{ if (CanRemoveHead()) m_bHasHead = false; }
		bool				HasHead() const					{ return m_bHasHead; }

		//------------------------------------------------------
		// HP 회복
		//------------------------------------------------------
		// rate만큼 times번 회복된다.
		inline BOOL			IsRecoveryHP();
		inline BOOL			IsRecoveryMP();
		void				SetRecoveryHP(int amount, int times, DWORD delay);
		void				SetRecoveryMP(int amount, int times, DWORD delay);
		void				StopRecoveryHP()	{ m_RecoveryHPTimes = 0; }
		void				StopRecoveryMP()	{ m_RecoveryMPTimes = 0; }
		DWORD				GetRecoveryHPDelayTime() const		{ return m_RecoveryHPDelayTime; }
		int					GetRecoveryHPTimes() const			{ return m_RecoveryHPTimes; }
		int					GetRecoveryHPAmmount() const		{ return m_RecoveryHPAmount; }
		DWORD				GetRecoveryMPDelayTime() const		{ return m_RecoveryMPDelayTime; }
		int					GetRecoveryMPTimes() const			{ return m_RecoveryMPTimes; }
		int					GetRecoveryMPAmmount() const		{ return m_RecoveryMPAmount; }

		void				SetRegen(int amount, DWORD delay);
		void				SetRegenBonus(int amount, DWORD delay);			
		void				CheckRegen();

		void				UpdateStatus();

		//------------------------------------------------------
		// 피 흘리기
		//------------------------------------------------------
		virtual void		CheckDropBlood();

		//------------------------------------------------------
		// 화면에 표시하는 HP bar의 길이
		//------------------------------------------------------
		int					GetHPBarWidth()	const				{ return (*g_pCreatureTable)[m_CreatureType].HPBarWidth; }

		//------------------------------------------------------
		// Fade
		//------------------------------------------------------
		// 몸 전체가 어둡게 표현되는 경우
		bool				IsFade() const						{ return m_bFade; }
		void				SetFade() 							{ m_bFade = true; }
		void				UnSetFade()							{ m_bFade = false; }

		// 캐릭터 동작을 따르는 그림자 모습 개수.. - -;
		int					GetShadowCount() const				{ return m_ShadowCount; }
		void				SetShadowCount(int n);

		//------------------------------------------------------
		// 냠..
		//------------------------------------------------------
		virtual int			FindEnemy();

		//------------------------------------------------------
		//
		//              Server Packet 처리
		//
		//------------------------------------------------------
		virtual void		PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp = 0);
		virtual void		PacketAttackNormal(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE direction);
		virtual void		PacketMove(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE direction);		
		virtual void		PacketSpecialActionToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, MActionResult* pActionResult);
		virtual void		PacketSpecialActionToOther(TYPE_ACTIONINFO nActionInfo, TYPE_OBJECTID id, MActionResult* pActionResult);
		virtual void		PacketSpecialActionToNobody(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);	// 상대가 zone에 없는 경우
		virtual void		PacketSpecialActionToSelf(TYPE_ACTIONINFO nActionInfo, MActionResult* pActionResult);
		virtual void		PacketSpecialActionToInventoryItem(TYPE_ACTIONINFO nActionInfo);
		
	
		//------------------------------------------------------
		// Fast Move
		//------------------------------------------------------
		bool				IsFastMove() const	{ return m_bFastMove; }
		virtual bool		FastMovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y,bool server = false);	// Zone의 Sector에서도 빨리(-_-;) 이동한다.
		void				StopFastMove();

		//------------------------------------------------------
		// Knock Back
		//------------------------------------------------------
		int					IsKnockBack() const			{ return m_bKnockBack>0; }
		virtual bool		KnockBackPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE Action = 0);
		void				StopKnockBack()			{ m_bKnockBack = 0; }
		
		//------------------------------------------------------
		// Casket
		//------------------------------------------------------
		bool				IsInCasket() const			{ return m_bInCasket; }
		int					GetCasketCount() const		{ return m_CasketCount; }
		int					GetCasketType() const		{ return m_CasketType; }
		void				AddCasket(int casketType);
		void				RemoveCasket();
		void				AddCasketSoon(int casketType);
		void				RemoveCasketSoon();
		void				UpdateCasket();		

		//--------------------------------------------------
		// CutHeight
		//--------------------------------------------------
		bool				IsCutHeight() const			{ return m_bCutHeight; }
		int					GetCutHeightCount() const	{ return m_CutHeightCount; }
		void				SetCutHeightFrom(int startCount, int lastCount, int inc);
		void				UpdateCutHeight();

		//--------------------------------------------------
		// Turning
		//--------------------------------------------------
		bool				IsTurning() const			{ return m_bTurning; }
		int					GetTurningCount() const		{ return m_TurningCount; }
		void				SetTurning(WORD count)		{ m_bTurning = true; m_TurningCount = count; }
		void				StopTurning()				{ m_bTurning = false; }
		void				UpdateTurning();
		
		//--------------------------------------------------
		// CauseCriticalWounds
		//--------------------------------------------------
		bool				IsCauseCriticalWounds() const			{ return m_bCauseCriticalWounds; }
		void				SetCauseCriticalWounds(int delay)		{ m_bCauseCriticalWounds = true; m_CauseCriticalWoundsCount = delay; }
		void				StopCauseCriticalWounds()				{ m_bCauseCriticalWounds = false; }
		void				UpdateCauseCriticalWounds();

		//--------------------------------------------------
		// BloodyZenith
		//--------------------------------------------------
		bool				IsBloodyZenith();
		void				SetBloodyZenith(int delay)		{ m_bBloodyZenith = true; m_BloodyZenithCount = delay; }
		void				StopBloodyZenith()				{ m_bBloodyZenith = false; }
		int					GetBloodyZenithCount();
		void				UpdateBloodyZenith();
		
		//--------------------------------------------------
		// GunShotGuidance
		//--------------------------------------------------
		bool				IsGunShotGuidance() const			{ return m_bGunShotGuidance; }
		void				SetGunShotGuidance(int delay)		{ m_bGunShotGuidance = true; m_GunShotGuidanceCount = delay; }
		void				StopGunShotGuidance()				{ m_bGunShotGuidance = false; }
		void				UpdateGunShotGuidance();

		//--------------------------------------------------
		// BurningSolCount
		//--------------------------------------------------
		bool				IsBurningSol() const			{ return m_bBurningSol; }
		void				SetBurningSol(int delay)		{ m_bBurningSol = true; m_bBurningSolCount = delay; }
		void				StopBurningSol()				{ m_bBurningSol = false; }
		bool				UpdateBurningSol();

		//--------------------------------------------------
		// InstallTurret
		//--------------------------------------------------
		BYTE				GetInstallTurretCount() const			{ return m_bInstallTurretCount; }
		void				SetInstallTurretCount(int Count)		{ m_bInstallTurretCount = Count; }
		BYTE				GetInstallTurretDirect() const			{ return m_bInstallTurretDirect; }
		void				SetInstallTurretDirect(int Count)		{ m_bInstallTurretDirect = Count; }
		bool				UpDateInstallTurret();
		void				InstallTurretStopAttack();


		//--------------------------------------------------
		// Armageddon
		//--------------------------------------------------
		bool				IsArmageddon() const			{ return m_bArmageddon; }
		void				SetArmageddon(int delay)		{ m_bArmageddon = true; m_ArmageddonCount = delay; }
		const int			GetArmageddon() const			{ if(IsArmageddon())return m_ArmageddonCount; return -1; }
		void				StopArmageddon()				{ m_bArmageddon = false; }
		void				UpdateArmageddon();

		//--------------------------------------------------
		// Teleport
		//--------------------------------------------------
		bool				IsGhost(BYTE flag) const			{ return (m_fGhost&flag)?true:false; }
		void				SetGhost(BYTE flag, int delay)		{ m_fGhost = flag; m_GhostCount = delay; }
		const int			GetGhost() const			{ if(IsGhost(0xFF))return m_GhostCount; return -1; }
		void				StopGhost()				{ m_fGhost = 0; }
		void				UpdateGhost();
		
		//--------------------------------------------------
		// FakeDie
		//--------------------------------------------------
		bool				IsFakeDie() const			{ return m_bFakeDie; }
		void				SetFakeDie();
		int					GetFakeDieCount()			{ return m_FakeDieCount; }
		void				StopFakeDie()				{ m_bFakeDie = false; }
		void				UpdateFakeDie();

		//----------------------------------------------------------
		// player가 Darkness안에 있는가?
		// DarknessCount는 몇타일이 보이는가?이다.
		//----------------------------------------------------------
		BOOL				IsInDarkness() const		{ return m_DarknessCount>=0; }
		BOOL				ShowInDarkness(int sX, int sY) const;
		int					GetDarknessCount() const	{ return m_DarknessCount; }
		void				UpdateDarkness();
		virtual void		CheckInDarkness();
		void				PlaceInDarkness(bool IsBlindness = false); // 2004, 6, 24 sobeit modify - blindness 처리 땜에
		void				PlaceNotInDarkness();

		BOOL				IsInGroundElemental() const;
		//--------------------------------------------------
		// 흡혈 delay를 없애기 위해서 임시로..
		//--------------------------------------------------
		void				SetStopBloodDrain()			{ m_bStopBloodDrain++; }
		void				UnSetStopBloodDrain()		{ if (m_bStopBloodDrain>0) m_bStopBloodDrain--; }
		void				ClearStopBloodDrain()		{ m_bStopBloodDrain = 0; }
		virtual void		StopBloodDrain();

		//--------------------------------------------------
		// 흡영 delay를 없애기 위해서 임시로..
		//--------------------------------------------------
		void				SetStopAbsorbSoul()			{ m_bStopAbsorbSoul++; }
		void				UnSetStopAbsorbSoul()		{ if (m_bStopAbsorbSoul>0) m_bStopAbsorbSoul--; }
		void				ClearStopAbsorbSoul()		{ m_bStopAbsorbSoul = 0; }
		virtual void		StopAbsorbSoul();
		
		//--------------------------------------------------
		// 파티 체크
		//--------------------------------------------------
		bool				IsPlayerParty() const		{ return m_bPlayerParty; }
		void				SetPlayerParty()			{ m_bPlayerParty = true; }
		void				UnSetPlayerParty()			{ m_bPlayerParty = false; }

		//--------------------------------------------------
		// Hallucination걸렸을때 몹으로 보이기 - -;
		//--------------------------------------------------		
		bool				IsHallu() const				{ return m_bHallu; }
		void				DetermineHalluActionFrame();
		void				SetHalluCreature(TYPE_CREATURETYPE type);
		void				UnSetHalluCreature()		{ m_bHallu = false; }
		TYPE_FRAMEID		GetHalluCreatureFrameID() const	{ return m_HalluCreatureFrameID; }
		TYPE_CREATURETYPE	GetHalluCreatureType() const { return m_HalluCreatureType; }
		WORD				GetHalluColor() const		{ return m_HalluColorBody; }
		BYTE				GetHalluAction() const		{ return m_HalluAction; }
		BYTE				GetHalluFrame() const		{ return m_HalluFrame; }
		const char*			GetHalluName() const;
		

		// 운영자
		void				SetCompetence(BYTE c)		{ m_Competence = c; }
		BYTE				GetCompetence() const		{ return m_Competence; }

		// 흡혈
		void				SetDrainCreatureID(TYPE_OBJECTID id)	{ m_DrainCreatureID = id; }
		const TYPE_OBJECTID	GetDrainCreatureID() const				{ return m_DrainCreatureID; }
		void				StopDrain();	// 나를 흡혈하고 있던 Creature의 흡혈동작을 멈춘다

		// 흡영
		void				SetAbsorbCreatureID(TYPE_OBJECTID id)	{ m_DrainCreatureID = id; }
		const TYPE_OBJECTID	GetAbsorbCreatureID() const				{ return m_DrainCreatureID; }
		void				StopAbsorb();	// 나를 흡혈하고 있던 Creature의 흡영동작을 멈춘다
		
		// PET
		void				SetPetID(TYPE_OBJECTID id)				{ m_PetID = id; }
		const TYPE_OBJECTID	GetPetID() const						{ return m_PetID; }
		
		// Elemental
		void				SetElementalID(TYPE_OBJECTID id)				{ m_ElementalID = id; }
		const TYPE_OBJECTID	GetElementalID() const						{ return m_ElementalID; }
		
		//--------------------------------------------------
		// HP Modify관련
		//--------------------------------------------------		
		const HPMODIFYLIST *GetHPModifyList() const			{ return &m_HPModifyList; }
		void				AddHPModify(const int modify);
		const bool			IsEmptyHPModifyList() const		{ return m_HPModifyList.empty(); }
		const int			GetHPModifyListSize() const		{ return m_HPModifyList.size(); }

		void			SetActionGrade( BYTE a ) { if( a != 0 ) m_GradeActionInfo = a; }
		void			ClearActionGrade() {m_GradeActionInfo = 0;}
		BYTE			GetActionGrade() { return m_GradeActionInfo; }
		
		virtual	void	SetTraceID( TYPE_OBJECTID id );

		void			SetActionDelay(short time)			{ m_DelayLastAction = time; }
		short			GetActionDelay()		const		{ return m_DelayLastAction; }

		void			SetNickName(BYTE bType, char* szNickName) { m_NickNameType = bType; m_NickName = szNickName;}
		string			GetNickName() { return m_NickName;}
		BYTE			GetNickNameType() { return m_NickNameType;}

		bool			GetIsInSiegeWar() { return (m_bSiegeWar>0)?true:false;}
		void			SetSiegeWar(BYTE bSiege) { m_bSiegeWar = bSiege; }
		bool			GetIsAttackerInSiegeWar() {return (m_bSiegeWar>2&&m_bSiegeWar<8)?true:false;}
		bool			GetIsDefenderInSiegeWar() {return (m_bSiegeWar>0&&m_bSiegeWar<3)?true:false;}
		
		BOOL			IsInFuryOfGnome() const;


		string			GetPersnalShopMessage() {return m_PersnalMessage; }
		void 			SetPersnalShopMessage(const char * str) {m_PersnalMessage = str; }

		void			SetPersnalShop(int curshop) { m_Persnal_flag = curshop; }
		int				CurPernalShop() {return m_Persnal_flag;}

		void			SetInputChatting(bool b_input) {  m_Input_Chat_Flag = b_input ;}
		bool			GetInputChatting() { return  m_Input_Chat_Flag; }

		BYTE			GetPersonalShopMsgTime() const { return m_PersonalShopOpenTime; }
		void			SetPersonalShopMsgTime( BYTE time ) { m_PersonalShopOpenTime = time;	}
		
		bool			IsAdvancementClass() const { return GetAdvancementClassLevel() > 0 && GetAdvancementClassLevel() != MODIFY_NULL; }


		// 2005, 1, 5, sobeit add start
		void		UpdateBikeCrash();
		// 2005, 1, 5, sobeit add end
		void			RemoveCauseCriticalWoundsEffect();

		// 2005, 2, 22, sobeit add start
		int 			GetMasterEffectType(DWORD Status);
		void			ChangeMasterEffectType(int MarketEffect);
		// 2005, 2, 22, sobeit add end
	protected :
		virtual void	ActionMove();
		virtual void	ActionEffect();
	
		virtual void	AffectUsedActionInfo(TYPE_ACTIONINFO nUsedActionInfo);
		virtual void	AttachCastingEffect(TYPE_ACTIONINFO nUsedActionInfo, BOOL bForceAttach=FALSE);

		virtual void	AffectBufferedActionInfo();
	

	protected :
		// Creature의 종류
		TYPE_CREATURETYPE		m_CreatureType;
		//TYPE_FRAMEID			m_CreatureFrameID;
		CTypeTable<TYPE_FRAMEID>			m_CreatureFrameIDs;
		BOOL					m_bExistBody;
		DWORD					m_TraceTimer;				// 따라가기 제한시간

		//add by viva
		BYTE					m_MoveCount;		// 이동한 회수
		//end

		//--------------------------------------------------
		// 색깔
		//--------------------------------------------------
		// PC Vampire : 1=피부, 2=옷		
		// Monster : 1=몸의 어딘가에 변하는 색.. - -; 2=Nothing
		WORD					m_ColorBody1;		
		WORD					m_ColorBody2;	
		WORD					m_ChangeColorSet;	// 몹 전체 색깔이 바뀌는 경우
		WORD					m_BatColorSet;
		// 2005, 2, 21, sobeit add start
		BYTE					m_MasterEffectType;
		// 2005, 2, 21, sobeit add end
		

		// Move 종류 : 지하,지상,공중
		MOVE_TYPE				m_MoveType;
		short					m_Z;			// 높이 Z

		// 이동하는 방법
		MOVE_DEVICE				m_MoveDevice;
		BYTE					m_MoveAction;

		MZone*					m_pZone;		// 이 Creature가 속하는 Zone


		// 다음 움직일 방향
		TYPE_SECTORPOSITION		m_NextX;
		TYPE_SECTORPOSITION		m_NextY;
		BYTE					m_NextDirection;

		//--------------------------------------------------
		// Move Buffering
		//--------------------------------------------------
		MOVENODE_LIST			m_listMoveBuffer;
	

		// sector사이의 smooth scroll을 위해서		
		int				m_sX, m_sY;		// 이동해야할 나머지 pixels
		int				m_cX, m_cY;		// 한번에 이동하는 가로 pixel
		
		// 이동하는데 필요한 정보
		static int		m_sXTable[MAX_DIRECTION];
		static int		m_sYTable[MAX_DIRECTION];
		static int		*m_cXTable[MAX_FRAME_MOVE][MAX_DIRECTION];
		static int		*m_cYTable[MAX_FRAME_MOVE][MAX_DIRECTION];		

		// 현재 frame에 대한 정보
		BYTE					m_Action;			// Action에 대한 ID
		//add by viva for no increase speed
		//BYTE					m_CurrentDirection;	// 현재의 Direction
		//BYTE					m_Direction;		// 바라봐야 하는 Direction
		//BYTE					m_DirectionMove;	// 이동할 방향
		//BYTE					m_DirectionMoved;	// 이동한 방향		
		//BYTE					m_ActionCount;		// ActionCount == Frame
		//BYTE					m_ActionCountMax;	// ActionCountMax
		//BYTE					m_MoveCount;		// 이동한 회수
		//BYTE					m_MoveCountMax;		// 이동하는 목표 회수
		//BYTE					m_MoveTableCount;	// cX,YTable의 count
		//BYTE					m_NextMoveCount;	// 다음에 이동해야하는 count수


		//BYTE					m_NextAction;
		//end
		// 기술 종류		
		TYPE_ACTIONINFO			m_nBasicActionInfo;		// 선택되어 있는 기본 기술 종류				
		TYPE_ACTIONINFO			m_nSpecialActionInfo;	// 선택되어 있는 특수 기술 종류
		TYPE_ACTIONINFO			m_nUsedActionInfo;		// 사용한 것
		
		// 다음에 사용하게되는 기술..
		TYPE_ACTIONINFO			m_nNextUsedActionInfo;

		// 공격하는 목표에 대한 정보
		TYPE_OBJECTID			m_TraceID;				// 따라가는 Object의 ID		
		TYPE_SECTORPOSITION		m_TraceX;				// 추적하는 목표 위치
		TYPE_SECTORPOSITION		m_TraceY;
		short					m_TraceZ;

		
		// Server에서 최근에 검증된 위치
		TYPE_SECTORPOSITION			m_ServerX;	
		TYPE_SECTORPOSITION			m_ServerY;		

		// 캐릭터에 붙어 있는 Effect 표현
		bool*					m_bAttachEffect;
		bool*					m_bEffectStatus;
		ATTACHEFFECT_LIST		m_listEffect;
		ATTACHEFFECT_LIST		m_listGroundEffect;
		WORD					m_AttachEffectColor;	// 현재 출력할려는 캐릭터 색깔 바꾸기 색 -_-;(말이 이상해..음냐)
		//BYTE					m_nAlphaEffect;		
		//char					m_MaxEffectLight;		// Effect의 최고 밝기(시야)

		// chat
		COLORREF				m_ChatColor;				// 현재의 채팅 색
		COLORREF				m_OriChatColor;				// fade되기 전의 채팅 색
		DWORD					m_NextChatFadeTime;		// 최근의 Chat string이 입력된 시간
		BYTE					m_ChatStringCurrent;
		char**					m_ChatString;
		DWORD					m_ChatTime;
		BYTE					m_PersonalShopOpenTime;
		
		// ActionInfo에 대한 결과(Server로부터 검증된 것)
		MActionResult*			m_pActionResult;

		// 캐릭터 이름
		char*					m_pName;

		// 살아있나?
		bool					m_bAlive;		

		// 다음 행동이 있나?
		bool					m_bNextAction;		

		// 가지고 있는 item 개수 [ TEST CODE ] - -;;
		int						m_ItemCount;
		//add by viva
		BYTE					m_DirectionMove;	// 이동할 방향
		//end
		// 기술의 지속 시간 설정
		TYPE_ACTIONINFO			m_DelayActionInfo;
		DWORD					m_EffectDelayFrame;

		// LevelName
		int						m_LevelName;

		// 길드
		int						m_GuildNumber;
		int						m_nUnionGuildID;
		//add by viva
		BYTE					m_Direction;		// 바라봐야 하는 Direction
		//end
		// fast move
		bool					m_bFastMove;

		// repeat count
		int						m_RepeatCount;

		// 무기 공격 속도
		WEAPON_SPEED			m_WeaponSpeed;

		static int				s_SlayerActionSpeed[ACTION_MAX_SLAYER][3];
		static int				s_VampireActionSpeed[ACTION_MAX_SLAYER][3];	//ACTION_MAX_VAMPIRE][3];
		static int				s_OustersActionSpeed[ACTION_MAX_SLAYER][3];	//ACTION_MAX_VAMPIRE][3];

		//--------------------------------------------------
		// 시간에 따른 HP 회복
		//--------------------------------------------------
		DWORD					m_RecoveryHPNextTime;
		DWORD					m_RecoveryHPDelayTime;
		int						m_RecoveryHPTimes;
		int						m_RecoveryHPAmount;
		DWORD					m_RecoveryMPNextTime;
		DWORD					m_RecoveryMPDelayTime;
		int						m_RecoveryMPTimes;
		int						m_RecoveryMPAmount;
		//DWORD					m_RecoveryPart;		// hp, mp?
		//add by viva
		BYTE					m_NextAction;
		//end
		// 자동 regen - -;
		DWORD					m_RegenDelayTime;
		DWORD					m_RegenNextTime;
		int						m_RegenAmount;
		DWORD					m_RegenBonusDelayTime;
		DWORD					m_RegenBonusNextTime;
		int						m_RegenBonusAmount;
		//add by viva
		BYTE					m_ActionCountMax;	// ActionCountMax
		//end
		//--------------------------------------------------
		// 머리가 있나?
		//--------------------------------------------------
		bool					m_bHasHead;

		//--------------------------------------------------
		// 어둡게 찍기
		//--------------------------------------------------
		bool					m_bFade;
		int						m_ShadowCount;
		//add by viva
		BYTE					m_NextMoveCount;	// 다음에 이동해야하는 count수
		//end
		//--------------------------------------------------
		// 성별
		//--------------------------------------------------
		bool					m_bMale;

		//--------------------------------------------------
		// 피 흘리기
		//--------------------------------------------------
		DWORD					m_NextBloodingTime;

		//--------------------------------------------------
		// KnockBack 처리
		//--------------------------------------------------
		int						m_bKnockBack;	// >0이면 KnockBack중.

		//--------------------------------------------------
		// 이것들 다 묶어야 되는데...  언제나~ --;;
		//--------------------------------------------------
		// 투명 되기
		//--------------------------------------------------
		int						m_InvisibleCount;	// 0이면 완전히 보인다. 64면 완전 투명
		int						m_InvisibleStep;
		
		//--------------------------------------------------
		// 관
		//--------------------------------------------------
		bool					m_bInCasket;
		int						m_CasketCount;
		int						m_CasketCountInc;
		int						m_CasketType;
		//add by viva
		BYTE					m_MoveTableCount;	// cX,YTable의 count
		//end
		//--------------------------------------------------
		// 다 그릴까 말까..
		//--------------------------------------------------
		bool					m_bCutHeight;
		int						m_CutHeightCount;
		int						m_CutHeightCountLast;
		int						m_CutHeightCountInc;

		//--------------------------------------------------
		// 빙빙 돌기
		//--------------------------------------------------
		bool					m_bTurning;
		int						m_TurningCount;

		//--------------------------------------------------
		// 흡혈 delay를 없애기 위한 임시 방편
		//--------------------------------------------------
		BYTE					m_bStopBloodDrain;
		BYTE					m_bStopAbsorbSoul;

		//----------------------------------------------------
		// CauseCriticalWounds에 묶여 있을때 흡혈 당하는 동작
		//----------------------------------------------------
		bool					m_bCauseCriticalWounds; 
		int						m_CauseCriticalWoundsCount; 
		//add by viva
		BYTE					m_ActionCount;		// ActionCount == Frame
		//end
		//----------------------------------------------------
		// Bloody Zenith에 묶여 있을때 흡혈 당하는 동작
		//----------------------------------------------------
		bool					m_bBloodyZenith; 
		int						m_BloodyZenithCount; 
		
		//----------------------------------------------------
		// GunShotGuidance에 묶여 있을때 총쏘는 동작
		//----------------------------------------------------
		bool					m_bGunShotGuidance; 
		int						m_GunShotGuidanceCount; 
		
		//----------------------------------------------------
		// Armageddon에 묶여 있을 때
		//----------------------------------------------------
		bool					m_bArmageddon; 
		int						m_ArmageddonCount; 
		//add by viva
		BYTE					m_CurrentDirection;	// 현재의 Direction
		//end
		//----------------------------------------------------
		// Teleport에 묶여 있을 때
		//----------------------------------------------------
		BYTE					m_fGhost; 
		int						m_GhostCount; 
		
		//----------------------------------------------------
		// 캐릭터 사라지는 동작
		//----------------------------------------------------
		bool					m_bFakeDie; 
		int						m_FakeDieCount; 
		
		//----------------------------------------------------
		// darkness
		//----------------------------------------------------
		int						m_DarknessCount;
		int						m_DarknessCountInc;

		//----------------------------------------------------
		// party - player의 체크를 빠르게 할려고..
		//----------------------------------------------------
		bool					m_bPlayerParty;

		//--------------------------------------------------
		// Hallucination걸렸을때 몹으로 보이기
		//--------------------------------------------------		
		bool					m_bHallu;
		TYPE_CREATURETYPE		m_HalluCreatureType;
		TYPE_FRAMEID			m_HalluCreatureFrameID;
		WORD					m_HalluColorBody;
		BYTE					m_HalluAction;
		BYTE					m_HalluFrame;
		WORD					m_HalluName;	// 일단 임시로...
		short					m_DelayLastAction;					// 해당 액션을 한 후에 일반 액션으로 돌아올때 딜레이
		//add by viva
		BYTE					m_DirectionMoved;	// 이동한 방향	
		//end
		//--------------------------------------------------		
		// 운영자
		//--------------------------------------------------		
		BYTE					m_Competence;

		//--------------------------------------------------		
		// 흡혈중에 흡혈상대가 죽으면 흡혈 동작 중지하기 위해.
		// 흡혈당하는 사람이 자신을 흡혈 하는 사람의 ID를 기억
		//--------------------------------------------------		
		TYPE_OBJECTID			m_DrainCreatureID;

		// PET ID
		TYPE_OBJECTID			m_PetID;
		TYPE_OBJECTID			m_ElementalID;
		
		//--------------------------------------------------
		// HP의 변화량을 기억
		//--------------------------------------------------
		HPMODIFYLIST			m_HPModifyList;		
		BYTE					m_GradeActionInfo;				// 동작하려는 Action 의 단계.
		
		int						m_HeadSkin;
		
		BYTE					m_NickNameType;
		string					m_NickName;
		
		//--------------------------------------------------
		// 공성전 참가 중인지.. 
		// 1,2 : 수비측
		// 3~7 : 공격측
		//--------------------------------------------------
		BYTE					m_bSiegeWar;	

		// 버닝 솔
		BYTE					m_bBurningSolCount;
		bool					m_bBurningSol;

		BYTE					m_bInstallTurretCount;
		BYTE					m_bInstallTurretDirect;
		int						m_current_sprite_index;

		//add by viva for no increase speed
		BYTE					m_MoveCountMax;		// 이동하는 목표 회수
		//end

		string					m_PersnalMessage;
		int						m_Persnal_flag;
		bool					m_Input_Chat_Flag;

		bool					m_IsSkipMotorStand;

};

#endif

		