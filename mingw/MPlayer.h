//----------------------------------------------------------------------
// MPlayer.h
//----------------------------------------------------------------------
// 특별히 Player만 다루는 class
// 길찾기 algorithm 구현
//----------------------------------------------------------------------

#ifndef	__MPLAYER_H__
#define	__MPLAYER_H__

#pragma warning(disable:4786)

#include <list>
#include <queue>

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------
// 추적하는 것에 대한 flag
#define	FLAG_TRACE_NULL					0
#define	FLAG_TRACE_CREATURE_BASIC		0x0001
#define	FLAG_TRACE_CREATURE_SPECIAL		0x0002
#define	FLAG_TRACE_SECTOR_BASIC			0x0004
#define	FLAG_TRACE_SECTOR_SPECIAL		0x0008
#define	FLAG_TRACE_ITEM					0x0010
#define	FLAG_TRACE_INTERACTIONOBJECT	0x0020
#define	FLAG_TRACE_SELF					0x0040
#define	FLAG_TRACE_INVENTORY			0x0080
#define	FLAG_TRACE_EFFECT				0x0100


//----------------------------------------------------------------------
// class 선언
//----------------------------------------------------------------------
class MCreature;
class MItem;
class MInteractionObject;
class Effect;
class MSlayerGear;
class MVampireGear;
class MOustersGear;

extern MSlayerGear*		g_pSlayerGear;
extern MVampireGear*	g_pVampireGear;
extern MOustersGear*	g_pOustersGear;

#include "MCreatureWear.h"
#include "MEffectTarget.h"
#include "MRequestMode.h"

extern DWORD	g_CurrentTime;

#ifndef __DNODE_DEFINE__
#define __DNODE_DEFINE__

//----------------------------------------------------------------------
//
// 길찾기에 이용할 Direction Node
//
//----------------------------------------------------------------------
class DNode {
	public :
		DNode(int _x=0, int _y=0, int _d=0, int _ds=0, int _s=0)
		{ 
			x=_x;
			y=_y;
			direction=_d;
			distance=_ds;
			step = _s;
			pParent=NULL;
		}

		void	SetParent(DNode* pDNode)	{ pParent = pDNode; }

		int		x;
		int		y;
		int		direction;
		int		distance;
		int		step;
		DNode*	pParent;
};


//----------------------------------------------------------------------
//
// 길찾기할 때 더 나은 Node를 선택하는 비교 연산
//
//----------------------------------------------------------------------
// [1] 거리가 가까운 것
// [2] 움직인 회수가 적은 것
// [3] 이전 방향과 같은 것
class Comparison {
	public :
		// distance가 적을 걸 선택해야 한다.		
		// true : right를 선택한다.
		// false : left를 선택한다.
		bool operator () (DNode * left, DNode * right) const
		{ 
			int diff = left->distance - right->distance;			
			int s = left->step - right->step;

			// 거리가(diff) 같은 경우..
			if (diff==0)
			{			
				// 움직인 회수가 같은 경우
				if (s==0)
				{
					if (left->pParent!=NULL && right->pParent!=NULL)
					{
						if (right->pParent->direction == right->direction)
						{
							return true;	// right선택
						}
						
						return false;	// left선택						
					}
				}
				// 움직인 회수가 적은 것
				else if (s>0) 
				{
					return true;
				}
				
				return false;				
			}			
			// 거리가 적은 것
			else if (diff>0) return true; 

			return false;
		}
};
#endif

//----------------------------------------------------------------------
//
// MPlayer class
//
//----------------------------------------------------------------------
class MPlayer : public MCreatureWear, public MRequestMode {
	public :
		typedef	std::list<BYTE>		DIRECTION_LIST;

		enum REQUEST_MODE
		{
			REQUEST_NULL,
			REQUEST_TRADE,
			REQUEST_PARTY
		};

		enum ATTACK_MODE 
		{
			ATTACK_MODE_PEACE,		// 아무도 공격 안 함
			ATTACK_MODE_NORMAL,		// 적(!)만 공격함
			ATTACK_MODE_AGGRESS,	// 아무나 공격함
			ATTACK_MODE_GUILD,		// 우리 길드만 우리편
		};

		enum ITEM_CHECK_BUFFER
		{
			ITEM_CHECK_BUFFER_NULL = 0,

			// item줍기
			ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY,
			ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE,
			ITEM_CHECK_BUFFER_PICKUP_TO_QUICKSLOT,
			ITEM_CHECK_BUFFER_PICKUP_MONEY,

			// item 사용 
			ITEM_CHECK_BUFFER_USE_FROM_INVENTORY,
			ITEM_CHECK_BUFFER_USE_FROM_QUICKSLOT,
			
			// item을 다른 item에 추가
			ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY,	
			ITEM_CHECK_BUFFER_INSERT_FROM_GEAR,	
			ITEM_CHECK_BUFFER_INSERT_FROM_QUICKSLOT,			
			
			ITEM_CHECK_BUFFER_DROP_TO_INVENTORY,
			ITEM_CHECK_BUFFER_PICKUP_FROM_INVENTORY,

			ITEM_CHECK_BUFFER_PICKUP_SOME_FROM_INVENTORY,	// 아이템 몇개만 들기

			// trade할 때
			ITEM_CHECK_BUFFER_TRADE_ADD,
			ITEM_CHECK_BUFFER_TRADE_REMOVE,

			// item에 기술 사용
			ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY,

			// item으로 기술 사용
			ITEM_CHECK_BUFFER_SKILL_FROM_ITEM,

			// mouse에서 zone으로 버린다.
			ITEM_CHECK_BUFFER_DROP_TO_ZONE,			

			ITEM_CHECK_BUFFER_DROP_TO_RELICTABLE,			

			// item을 다른 아이템에 더하기
			ITEM_CHECK_BUFFER_ITEM_TO_ITEM,

			// 트리 합체
			ITEM_CHECK_BUFFER_TREE_MERGE,
			ITEM_CHECK_BUFFER_USE_FROM_GEAR,
			
			ITEM_CHECK_BUFFER_USE_MIXING_ITEM,
			ITEM_CHECK_BUFFER_DROP_TO_CREATURE,			
			
			ITEM_CHECK_BUFFER_USE_FROM_GQUEST_INVENTORY,
		};

		// server에서 검증받기..
		enum WAIT_VERIFY
		{
			WAIT_VERIFY_NULL,
			WAIT_VERIFY_MOTORCYCLE_GETON,
			WAIT_VERIFY_MOTORCYCLE_GETOFF,
			WAIT_VERIFY_SKILL_SUCCESS,
			WAIT_VERIFY_ITEM_USE,
			WAIT_VERIFY_NPC_ASK,
			WAIT_VERIFY_TRADE,
			WAIT_VERIFY_PARTY,
			WAIT_VERIFY_PARTY_LEAVE,
			WAIT_VERIFY_RESURRECT,
			WAIT_VERIFY_LOVE_CHAIN,
			WAIT_VERIFY_LOGOUT,
			WAIT_VERIFY_SYLPH_SUMMON_GETOFF,
		};

	public :
		MPlayer();
		~MPlayer();

		void* operator new( size_t size )
		{
			return ::operator new( size );
		}

		void operator delete( void* pmem )
		{
			::operator delete ((MPlayer*)pmem);
		}
		
		virtual CLASS_TYPE	GetClassType() const	{ return CLASS_PLAYER; }

		class MPlayerGear*	GetGear() const 
		{
			switch(GetRace())
			{
			case RACE_SLAYER:
				return (MPlayerGear *)g_pSlayerGear;
				
			case RACE_VAMPIRE:
				return (MPlayerGear *)g_pVampireGear;
				
			case RACE_OUSTERS:
				return (MPlayerGear *)g_pOustersGear;
			}

			return NULL;
		}

		// Zone을 set한다.
		void	SetZone(MZone* pZone);

		virtual void	SetCreatureType(TYPE_CREATURETYPE type);		

		// 
		virtual void		SetServerPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);

		// 시야 관련
		void	CalculateLightSight();
		const int	GetLightSight() const			{ return m_LightSight; }
		char	GetTimeLightSight()	const		{ return m_TimeLightSight; }
		void	SetTimeLightSight(char s);
		void	SetItemLightSight(int s);
		int		GetItemLightSight() const			{ return m_ItemLightSight; }
		void	SetSight(char s)				{ m_Sight = s; }
		char	GetSight()	const			{ return m_Sight; }
		
		// 한 행동~
		void	Action();

		// 다음에 할 행동 설정.. 
		void	SetNextAction(BYTE action);	
		void	SetNextActionToMove();

		// 
		void	SetAction(BYTE action);

		// 흡혈 delay를 없애기 위해서 임시로..
		void	StopBloodDrain();
		void	StopAbsorbSoul();	// 흡영

		//----------------------------------------------------------
		// EffectStatus
		//----------------------------------------------------------
		bool	AddEffectStatus(EFFECTSTATUS status, DWORD delayFrame);
		bool	RemoveEffectStatus(EFFECTSTATUS status);		

		//----------------------------------------------------------
		// 상태 값 바꾸기
		//----------------------------------------------------------
		void	SetStatus(DWORD n, DWORD value);
		void	CalculateStatus();
		
		void	CalculateSight();
		
		//----------------------------------------------------------
		// 변신
		//----------------------------------------------------------
		bool	ChangeToSlayer();
		bool	ChangeToVampire();

//		//----------------------------------------------------------
//		// 시야
//		//----------------------------------------------------------
//		void	SetLightSight(char LightSight);
//		void	UnSetLightSight();	
		
		//----------------------------------------------------------
		// 안전지대에 있나?
		//----------------------------------------------------------
		BOOL	IsInSafeSector() const;
		
		//----------------------------------------------------------
		// 살기/죽기
		//----------------------------------------------------------
		void	SetAlive();		// 살아난다? - -;
		void	SetDead();		// 죽는다.

		//----------------------------------------------------------
		// 복장 바꾸기
		//----------------------------------------------------------
		bool	SetAddonItem( MItem* pItem );
		bool	RemoveAddonItem( MItem* pItem );

		//----------------------------------------------------------
		// SoulChain 실패
		//----------------------------------------------------------
		void	SetCannotTrace();
		

		//----------------------------------------------------------
		//
		//			 Item 처리
		//
		//----------------------------------------------------------
		void	PickupItem(MItem* pItem);	// item 줍기 처리
		void	PickupItemToInventory(MItem* pItem);
		void	PickupItemToMouse(MItem* pItem);
		bool	PickupItemToQuickslot(MItem* pItem);
		void	PickupMoney(MMoney* pItem);
		void	RideMotorcycle(MMotorcycle* pMotorcycle);		

		//----------------------------------------------------------
		//
		//            Server Packet과 관련
		//
		//----------------------------------------------------------
		// 기다리는 Move Packet은 없다.
		BYTE	GetSendMove() const		{ return m_SendMove; }
		void	ResetSendMove();
		
		//----------------------------------------------------------
		// server로부터 검증을 기다리는 상태
		//----------------------------------------------------------
		BOOL	IsWaitVerifyNULL() const	{ return m_WaitVerify==WAIT_VERIFY_NULL; }
		BOOL	IsWaitVerify() const		{ return m_WaitVerify!=WAIT_VERIFY_NULL; }
		WAIT_VERIFY	GetWaitVerify() const	{ return m_WaitVerify; }
		TYPE_ACTIONINFO GetWaitVerifyActionInfo() const { return m_WaitVerifyActionInfo; }
		void	SetWaitVerifyNULL()			{ m_WaitVerify=WAIT_VERIFY_NULL; m_WaitVerifyActionInfo=ACTIONINFO_NULL; }
		void	SetWaitVerify(WAIT_VERIFY wv, TYPE_ACTIONINFO ai=ACTIONINFO_NULL)	{ m_WaitVerify=wv; m_WaitVerifyActionInfo=ai; }

		//----------------------------------------------------------
		// packet처리
		//----------------------------------------------------------
		//void	PacketBack();
		void	PacketMoveOK(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE direction);
		void	PacketMoveNO();
		void	PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp=0);
		bool	PacketAddActionResult(WORD effectID, MActionResult* pResult);	// 결과 설정

		bool	FastMovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, bool server = false);	// Zone의 Sector에서도 빨리(-_-;) 이동한다.

		bool	KnockBackPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);

		//----------------------------------------------------------
		//
		//           추적 
		//
		//----------------------------------------------------------
		bool	SelfSpecialAction();
		// 다른 Creature를 추적해서 Action을 취한다.
		bool	TraceCreatureToBasicAction(TYPE_OBJECTID id, bool bForceAttack=false, bool	bClick = false);
		bool	TraceCreatureToSpecialAction(TYPE_OBJECTID id, bool bForceAttack=false);
		bool	TraceSectorToBasicAction(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		bool	TraceSectorToSpecialAction(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		bool	TraceItem(TYPE_OBJECTID id);
		bool	TraceInteractionObject(TYPE_OBJECTID id, BYTE currentAction);
		bool	TraceEffect(TYPE_OBJECTID id);
		bool	TraceInventoryItem(TYPE_OBJECTID id);

		// creature계속 추적
		void	KeepTraceCreature();

		// 모든 추적 중지
		void	TraceNULL()				{ m_fTrace=FLAG_TRACE_NULL; m_fNextTrace=FLAG_TRACE_NULL; }
		void	TraceNextNULL()			{ m_fNextTrace=FLAG_TRACE_NULL; } 

		// nActionInfo의 사용 가능 거리는?
		int		GetActionInfoRange(TYPE_ACTIONINFO nActionInfo);
		
		//----------------------------------------------------------
		//
		//         길찾기
		//
		//----------------------------------------------------------
		// Zone상에서의 목표위치를 설정
		bool	SetMovePosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		bool	SetNextDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		bool	SetDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);		
		bool	IsNextDirectionNULL()		{ return m_listDirection.empty(); }
		void	SetStop();		
		
		// get
		void	GetDestination(POINT &dest);
		void	GetNextDestination(POINT &dest);
		bool	GetNextPosition(POINT &next);
		BYTE	GetNextDirection()	const			{ return m_listDirection.front(); }

		//----------------------------------------------------------
		// 추적
		//----------------------------------------------------------
		BYTE			GetBasicAttackDistance() const	{ return m_BasicAttackDistance; }
		void			SetBasicAttackDistance(BYTE bad) { m_BasicAttackDistance = (bad==0)?1:bad; }
		TYPE_OBJECTID	GetTraceID() const				{ return m_TraceID; }
		BOOL			IsTraceNULL()	const			{ return m_fTrace==0; }
		BOOL			IsTraceCreature()	const		{ return (m_fTrace & FLAG_TRACE_CREATURE_BASIC) || (m_fTrace & FLAG_TRACE_CREATURE_SPECIAL); }
		BOOL			IsTraceSector()	const			{ return (m_fTrace & FLAG_TRACE_SECTOR_BASIC) || (m_fTrace & FLAG_TRACE_SECTOR_SPECIAL); }
		BOOL			IsTraceItem()	const			{ return m_fTrace & FLAG_TRACE_ITEM; }
		BOOL			IsTraceInteractionObject() const { return m_fTrace & FLAG_TRACE_INTERACTIONOBJECT; }
		BOOL			IsTraceEffect() const			{ return m_fTrace & FLAG_TRACE_EFFECT; }

		//----------------------------------------------------------
		// 계속 Creature를 추적하는가?
		//----------------------------------------------------------
		bool			IsKeepTraceCreature() const		{ return m_bKeepTraceCreature; }
		void			SetKeepTraceCreature()			{ m_bKeepTraceCreature = true; }
		void			UnSetKeepTraceCreature()		{ m_bKeepTraceCreature = false; }

		//----------------------------------------------------------
		// 행동 반복하기
		//----------------------------------------------------------
		BOOL			IsRepeatAction() const			{ return m_bRepeatAction; }
		void			SetRepeatAction()				{ m_bRepeatAction = TRUE; m_RepeatCount = 0; m_RepeatTimer = GetTickCount(); }		
		void			UnSetRepeatAction();

		// 기술 변경
		void			SetSpecialActionInfo( TYPE_ACTIONINFO n );

		// 공격 중인가?
		bool			OnAttacking() const;
				
		//----------------------------------------------------------
		// Delay시간을 지나서 delay가 끝났는가?
		//----------------------------------------------------------
		bool			IsNotDelay() const		{ return m_DelayTime==0 || m_DelayTime <= g_CurrentTime; }
		bool			IsNotDeadDelay() const	{ return m_DeadDelayTime <= g_CurrentTime; }
		void			SetDelay(DWORD delay)	{ m_DelayTime = g_CurrentTime + delay; }
		DWORD			GetDeadDelayLast() const;

		//----------------------------------------------------------
		// 뱀파이어로 변하는데 남은 시간
		//----------------------------------------------------------
		void			SetConversionDelay(DWORD last)		{ m_ConversionDelayTime = g_CurrentTime + last; }
		DWORD			GetConversionDelayTime() const		{ return m_ConversionDelayTime; }
		void			UnSetConversionDelay()				{ m_ConversionDelayTime=0; }
		void			UpdateConversionTime();

		//----------------------------------------------------------
		// 진행중인 기술..
		//----------------------------------------------------------
		void			RemoveEffectTarget(BYTE id);
		void			AddEffectTarget(MEffectTarget* pEffectTarget);
		
		//----------------------------------------------------------
		// Player의 Attack Mode
		//----------------------------------------------------------
		ATTACK_MODE	GetAttackMode() const	{ return m_AttackMode; }
		bool	IsAttackModePeace() const	{ return m_AttackMode == ATTACK_MODE_PEACE; }
		bool	IsAttackModeNormal() const	{ return m_AttackMode == ATTACK_MODE_NORMAL; }
		bool	IsAttackModeAggress() const	{ return m_AttackMode == ATTACK_MODE_AGGRESS; }
		bool	IsAttackModeGuild() const	{ return m_AttackMode == ATTACK_MODE_GUILD; }
		void	SetAttackModePeace()		{ m_AttackMode = ATTACK_MODE_PEACE; }
		void	SetAttackModeNormal()		{ m_AttackMode = ATTACK_MODE_NORMAL; }
		void	SetAttackModeAggress()		{ m_AttackMode = ATTACK_MODE_AGGRESS; }
		void	SetAttackModeGuild()		{ m_AttackMode = ATTACK_MODE_GUILD; }

		// to종족을 공격할 수 있는 AttackMode인가?
		bool	CanAttackTribe( enum CREATURETRIBE to ) const;
		bool	CanAttackTribe( const MCreature* pCreature ) const	{ return CanAttackTribe( pCreature->GetCreatureTribe() ); }
		bool	CanAttackGuild( const MCreature* pCreature ) const;

		//----------------------------------------------------------
		// Item Check Buffer
		//----------------------------------------------------------
		bool	IsItemCheckBufferNULL()				{ return m_pItemCheckBuffer==NULL; }

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void	SetItemCheckBuffer(MItem* pItem, enum ITEM_CHECK_BUFFER status, TYPE_OBJECTID SubItem = OBJECTID_NULL);
	#else
		void	SetItemCheckBuffer(MItem* pItem, enum ITEM_CHECK_BUFFER status);
	#endif
		
		void	ClearItemCheckBuffer();		// 제거
		MItem*	GetItemCheckBuffer() const				{ return m_pItemCheckBuffer; }
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		TYPE_OBJECTID	GetItemIDCheckBufferSubInventory() const	{ return m_dwSubItemIDCheckBuffer; }
	#endif
		enum ITEM_CHECK_BUFFER	GetItemCheckBufferStatus() const	{ return m_ItemCheckBufferStatus; }

		// 괜히 만들어본 함수들 - -;
		bool	IsItemCheckBufferPickupToInventory() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY; }
		bool	IsItemCheckBufferPickupToMouse() const			{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE; }
		bool	IsItemCheckBufferPickupToQuickslot() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_TO_QUICKSLOT; }
		bool	IsItemCheckBufferPickupMoney() const			{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_MONEY; }
		bool	IsItemCheckBufferUseFromInventory() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_USE_FROM_INVENTORY; }
		bool	IsItemCheckBufferUseFromQuickSlot() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_USE_FROM_QUICKSLOT; }
		bool	IsItemCheckBufferInsertFromInventory() const	{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY; }
		bool	IsItemCheckBufferInsertFromQuickSlot() const	{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_INSERT_FROM_QUICKSLOT; }
		bool	IsItemCheckBufferDropToInventory() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_DROP_TO_INVENTORY; }
		bool	IsItemCheckBufferPickupSomeFromInventory() const	{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_SOME_FROM_INVENTORY; }		
		bool	IsItemCheckBufferPickupFromInventory() const	{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_PICKUP_FROM_INVENTORY; }
		bool	IsItemCheckBufferTradeAdd() const				{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_TRADE_ADD; }
		bool	IsItemCheckBufferTradeRemove() const			{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_TRADE_REMOVE; }
		bool	IsItemCheckBufferSkillToInventory() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY; }
		bool	IsItemCheckBufferSkillFromItem() const			{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_SKILL_FROM_ITEM; }
		bool	IsItemCheckBufferDropToZone() const				{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_DROP_TO_ZONE; }
		bool	IsItemCheckBufferDropToRelicTable() const		{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_DROP_TO_RELICTABLE; }
		bool	IsItemCheckBufferItemToItem() const				{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_ITEM_TO_ITEM; }
		bool	IsItemCheckBufferDropToCreature() const			{ return m_ItemCheckBufferStatus==ITEM_CHECK_BUFFER_DROP_TO_CREATURE; }
		
		//----------------------------------------------------------
		// Lock Mode
		//----------------------------------------------------------
		bool	IsLockMode() const		{ return m_bLockMode; }
		void	SetLockMode()			{ m_bLockMode = true; m_LockTimer = GetTickCount(); }
		void	UnSetLockMode()			{ m_bLockMode = false; }
		int		FindEnemy();

		//----------------------------------------------------------
		// player가 Darkness안에 있는가?
		// DarknessCount는 몇타일이 보이는가?이다.
		//----------------------------------------------------------
		void	CheckInDarkness();

		void	SetUndergroundCreature();
		void	SetGroundCreature();
		void	SetFlyingCreature();

		// 2004, 5, 7, sobeit add start
		void	SetPetDelay(DWORD dwDelay) {m_PetDelayTime = dwDelay;}
		// 2004, 5, 7, sobeit add end

		// 2004, 8, 21, sobeit add start
		void	SetSweepViewValue(BYTE n ) {m_SweepVice_Value = n;}
		BYTE	GetSweepViewValue() { return m_SweepVice_Value;}
		bool	CheckRbuttonUpSkill();
		void	SetSweepPos(int x, int y) { m_SweepViceX = x; m_SweepViceY = y;	}
		void	SetTempSelectPos(int x, int y) { m_TempSelectPosX = x; m_TempSelectPosY = y;	}

		bool	IsShowAdamCloud() { return m_bShowAdamCloud;}
		void	SetShowAdamCloud(bool bShow) { m_bShowAdamCloud = bShow;}
		// 2004, 8, 21, sobeit add end
		
		// 2005, 1, 4, sobeit add start
		void	UseWildWolf_Corpse(MItem *pItem);
		// 2005, 1, 4, sobeit add end

		void	SetResurrectZoneID(int ZoneID) { m_ResurrectZoneID = ZoneID; }
		int		GetResurrectZoneID() { return m_ResurrectZoneID; }

	protected :				
		void	ActionMove();			// move
		void	ActionEffect();			// effect
		void	ActionToSendPacket();		// packet을 보낸다..
		bool	ActionInTraceDistance();	// 행동 가능 거리에 있을 때
		void	AffectUsedActionInfo(TYPE_ACTIONINFO nUsedActionInfo);		// m_nUsedActionInfo를 적용시킨다.
		void	AttachCastingEffect(TYPE_ACTIONINFO nUsedActionInfo, BOOL bForceAttach=FALSE);

		// 추적한 creature에게 기본 action 적용
		void	BasicActionToCreature();

		// 다음 행동을 결정
		bool	CheckBufferAction();

		// 반복 행동 결정
		bool	CheckRepeatAction();

		// list node를 지운다.
		void	RemoveNodes();

		// 목표위치까지의 거리 계산
		int		CalculateDistance(int x, int y);
		
		// sweep vice update
		void	UpdateSweepVice();
		bool	UpdateBurningSol();
		bool	UpDateInstallTurret();

	protected :		
		DWORD						m_RepeatTimer;				// Repeat모드 제한시간
		DWORD						m_LockTimer;				// Lock모드 제한시간		

		TYPE_SECTORPOSITION			m_DestX;
		TYPE_SECTORPOSITION			m_DestY;					// 목표위치(Sector)

		TYPE_SECTORPOSITION			m_NextDestX; 
		TYPE_SECTORPOSITION			m_NextDestY;				// 다음 목표위치(Sector)

		TYPE_SECTORPOSITION			m_BlockDestX;				// 못 갔던 곳.. 
		TYPE_SECTORPOSITION			m_BlockDestY;				// 

		// 기다리는 move packet수
		BYTE					m_SendMove;

		// Basic공격의 공격 가능 거리 : 0이 되어서는 안된다. 최소 1이다.
		BYTE					m_BasicAttackDistance;	

		// 추적하기
		WORD					m_fTrace;				// 추적의 종류		
		WORD					m_fTraceBuffer;			// 추적의 종류(잠시 기억해두는 것)
		TYPE_OBJECTID			m_TraceIDBuffer;		//
		BYTE					m_TraceDistance;		// 사정 거리		
		BYTE					m_TraceObjectAction;	// 추적하는 Object의 상태
		bool					m_bTraceCreatureToForceAttack;
		bool					m_bNextForceAttack;

		// 다음 추적 - Buffering
		WORD					m_fNextTrace;
		TYPE_OBJECTID			m_NextTraceID;
		TYPE_SECTORPOSITION		m_NextTraceX;
		TYPE_SECTORPOSITION		m_NextTraceY;
		short					m_NextTraceZ;
		BYTE					m_NextTraceObjectAction;

		// 행동을 반복하는가?
		BOOL					m_bRepeatAction;
		
		// 어떤 Creature를 계속 추적하는가?
		bool					m_bKeepTraceCreature;

		// 움직이는데 필요한 것들..
		typedef std::priority_queue<DNode*, std::vector<DNode*>, Comparison>	DNODE_PQ;
		typedef	std::list<DNode*>												DNODE_LIST;

		DNODE_PQ			m_pqDNodes;
		DNODE_LIST			m_listDNodes;

		DIRECTION_LIST	m_listDirection;
		DIRECTION_LIST	m_listSendDirection;

		// 기술 사용 후의 delay
		DWORD						m_DelayTime;

		// 펫 루팅 사용후의 delay
		DWORD						m_PetDelayTime;

		// 현재 진행 중인 사용 기술
		EFFECTTARGET_LIST			m_listEffectTarget;

		// 현재 전투 Mode
		ATTACK_MODE					m_AttackMode;

		// 죽고 나서의 delay시간 - 언제까지..
		DWORD						m_DeadDelayTime;

		// Item을 기억하고 있자..
		ITEM_CHECK_BUFFER			m_ItemCheckBufferStatus;
		MItem*						m_pItemCheckBuffer;

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		DWORD						m_dwSubItemIDCheckBuffer; // add 2005, 3, 2, sobeit added  - 어떤 subinventory 에서 사용 했는지
	#endif

		// 기술 사용의 목표 : effect target
		MEffectTarget*				m_pEffectTarget;

		// server에서 검증을 기다리는 상태
		WAIT_VERIFY					m_WaitVerify;
		TYPE_ACTIONINFO				m_WaitVerifyActionInfo;

		// 뱀파이어로 변하는데 남은 delay frame - 언제까지
		DWORD						m_ConversionDelayTime;

		// packet을 보내지 않는 actioninfo 
		// flash sliding때문에 추가됐다. - -;;
		TYPE_ACTIONINFO					m_nNoPacketUsedActionInfo;

		//----------------------------------------------------
		// Lock Mode
		//----------------------------------------------------
		// 제자리에 정지해서 근처에 있는 적을 공격한다.
		bool						m_bLockMode;

		// 빛의 크기 
		char					m_TimeLightSight;
//		TYPE_SECTORPOSITION		m_TimeLightSightX;	// 최근에 시야를 Set한 위치
//		TYPE_SECTORPOSITION		m_TimeLightSightY;		
		int						m_ItemLightSight;
		int						m_LightSight;

		// 시야의 크기
		char					m_Sight;		

		// 2004, 8, 21, sobeit add start
		// 인챈 130 스킬 관련
		BYTE					m_SweepVice_Value;
		TYPE_SECTORPOSITION		m_TempSelectPosX; // Temporary 하게 쓰려고..-_-
		TYPE_SECTORPOSITION		m_TempSelectPosY;
		TYPE_SECTORPOSITION		m_SweepViceX;
		TYPE_SECTORPOSITION		m_SweepViceY;
		
		bool					m_bShowAdamCloud;
		// 2004, 8, 21, sobeit add end

		int						m_ResurrectZoneID;

};


extern MPlayer*		g_pPlayer;

#endif


