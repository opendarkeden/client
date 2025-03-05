//----------------------------------------------------------------------
// MActionResult.h
//----------------------------------------------------------------------
//
// 어떤 ActionInfo의 결과로 표현되는 것들에 대한 정보
//
// ACTIONRESULT_NODE를 생성해서(new) Add해주고
// 필요할때, GetIterator로.. 읽어야 한다.
//
// delete는 내부에서 한다.
//
//----------------------------------------------------------------------
/*

- 순간적인 Effect표시 
  사용 --> Server검증 --> Effect표시

- 지속적으로 Tile에 존재하는 Effect
  사용 --> Server검증 --> Tile에 Effect표시

- 효과 마법
  사용 --> Server검증 --> 대상에 효과 표현

- 타인 공격 마법
  사용 --> Server검증 --> 대상 데미지

- 사인예
  사용 --> Server검증 --> 대상 데미지, 캐릭터 이동



  = 내가 사용한 기술 : 
    시작ActionInfo를 보여주고 
	Server에서 결과가 오면 결과를 붙여서 결과ActionInfo를 실행한다.

  = 타인이 사용한 기술 :
    시작과 결과가 같이 존재하는데...
	시작ActionInfo에 결과ActionInfo+결과를 붙여야 한다.

  = 시작ActionInfo에는 결과ActionInfo에 대한 pointer
	결과 ActionInfo에는 결과에 대한 list


*/
//----------------------------------------------------------------------


#ifndef	__MACTIONRESULT_H__
#define	__MACTIONRESULT_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include "MTypeDef.h"
#include "MStatus.h"
#include "MActionResultDef.h"
#include "MEffectStatusDef.h"
#include <list>

//----------------------------------------------------------------------
// 하나의 결과에 대한 정보를 담고 있는 node의 base class
//----------------------------------------------------------------------
class MActionResultNode {
	public :
		MActionResultNode()		{}
		virtual ~MActionResultNode()	{}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		virtual TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_NULL; }
		
		//------------------------------------------------------
		// 어떤 종류인가?
		//------------------------------------------------------
		virtual bool		IsNodeActionInfo() const		{ return false; }		
		virtual bool		IsNodeChangePosition() const	{ return false; }
		virtual bool		IsNodeChangeStatus() const		{ return false; }
		virtual bool		IsNodeCreatureDie() const		{ return false; }
		virtual bool		IsNodeCreatureBurrow() const	{ return false; }
		virtual bool		IsNodeCreatureInvisible() const	{ return false; }
		virtual bool		IsNodeCreatureVisible() const	{ return false; }
		virtual bool		IsNodeCreatureSnipping() const	{ return false; }
		virtual bool		IsNodeAddEffectStatus() const		{ return false; }
		virtual bool		IsNodeChangeCreatureType() const	{ return false; }
		virtual bool		IsNodeSummonCasket() const			{ return false; }
		virtual bool		IsNodeOpenCasket() const			{ return false; }
		virtual bool		IsNodeCreatureTurning() const		{ return false; }
		virtual bool		IsNodeRemoveItemInInventory() const			{ return false; }
		virtual bool		IsNodeChangeItemOptionInInventory() const		{ return false; }
		virtual bool		IsNodeFakeDie() const			{ return false; }
		virtual bool		IsNodeEvent() const			{ return false; }


		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		virtual void		Execute() = 0;
};

//----------------------------------------------------------------------
// 누가(UserID) 누군가(TargetID)에게 ActionInfo를 사용한 결과node
//----------------------------------------------------------------------
class MActionResultNodeActionInfo : public MActionResultNode {
	public :
		MActionResultNodeActionInfo(TYPE_ACTIONINFO nActionInfo=0, TYPE_OBJECTID uid=0, TYPE_OBJECTID tid=0, TYPE_SECTORPOSITION sX=0, TYPE_SECTORPOSITION sY=0, DWORD delayFrame=0, BYTE temp=0);
		~MActionResultNodeActionInfo()		{};

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_ACTIONINFO; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeActionInfo() const		{ return true; }		

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetUserID(TYPE_OBJECTID uid)	{ m_UserID = uid; }
		void		SetTargetID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetActionInfo(TYPE_ACTIONINFO ai)	{ m_nActionInfo = ai; }
		void		SetDelayFrame(DWORD df)			{ m_DelayFrame = df; }
		void		SetSX(TYPE_SECTORPOSITION sX)	{ m_sX = sX; }
		void		SetSY(TYPE_SECTORPOSITION sY)	{ m_sY = sY; }
	
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();

	public :
		TYPE_ACTIONINFO			m_nActionInfo;
		
		TYPE_OBJECTID			m_UserID;
		TYPE_OBJECTID			m_TargetID;
		TYPE_SECTORPOSITION		m_sX;
		TYPE_SECTORPOSITION		m_sY;

		DWORD					m_DelayFrame;		
		BYTE					m_Temp;
};


//----------------------------------------------------------------------
// 누군가(ID)의 좌표(sX,sY)를 바꾸게 하는 결과node
//----------------------------------------------------------------------
class MActionResultNodeChangePosition : public MActionResultNode {
	public :
		MActionResultNodeChangePosition(TYPE_OBJECTID uid=0, TYPE_SECTORPOSITION sX=0, TYPE_SECTORPOSITION sY=0);
		~MActionResultNodeChangePosition()		{};

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CHANGE_POSITION; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeChangePosition() const	{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetUserID(TYPE_OBJECTID uid)		{ m_UserID = uid; }
		void		SetX(TYPE_SECTORPOSITION x)	{ m_X = x; }
		void		SetY(TYPE_SECTORPOSITION y)	{ m_Y = y; }
	
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_UserID;
		TYPE_SECTORPOSITION		m_X;
		TYPE_SECTORPOSITION		m_Y;
};

//----------------------------------------------------------------------
// 누군가(ID)의 상태를 바꾸게 하는 결과node
//----------------------------------------------------------------------
class MActionResultNodeChangeStatus : public MActionResultNode {
	public :
		MActionResultNodeChangeStatus(TYPE_OBJECTID tid=0, MStatus* pStatus=NULL);
		~MActionResultNodeChangeStatus();

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CHANGE_STATUS; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeChangeStatus() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetTargetID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetStatus(MStatus* pStatus)		{ m_pStatus = pStatus; }

		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
		MStatus*				m_pStatus;
};

//----------------------------------------------------------------------
// 누군가(ID)를 죽게 하는 결과
//----------------------------------------------------------------------
class MActionResultNodeCreatureDie : public MActionResultNode {
	public :
		MActionResultNodeCreatureDie(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeCreatureDie() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_DIE; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureDie() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 누군가(ID)를 땅 속에 숨게 하는 결과
//----------------------------------------------------------------------
class MActionResultNodeCreatureBurrow : public MActionResultNode {
	public :
		MActionResultNodeCreatureBurrow(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeCreatureBurrow() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_BURROW; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureBurrow() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 누군가(ID)를 사라지게 하는 결과 
//----------------------------------------------------------------------
class MActionResultNodeCreatureInvisible : public MActionResultNode {
	public :
		MActionResultNodeCreatureInvisible(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeCreatureInvisible() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_INVISIBLE; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureInvisible() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 누군가(ID)를 보이게 하는 결과 
//----------------------------------------------------------------------
class MActionResultNodeCreatureVisible : public MActionResultNode {
	public :
		MActionResultNodeCreatureVisible(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeCreatureVisible() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_VISIBLE; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureVisible() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 누군가(ID)를 땅 속에 숨게 하는 결과
//----------------------------------------------------------------------
class MActionResultNodeAddEffectStatus : public MActionResultNode {
	public :
		MActionResultNodeAddEffectStatus(TYPE_OBJECTID tid, EFFECTSTATUS es, DWORD df) { m_TargetID = tid; m_EffectStatus=es; m_DelayFrame=df; }
		~MActionResultNodeAddEffectStatus() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_ADD_EFFECTSTATUS; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeAddEffectStatus() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetEffectStatus(EFFECTSTATUS es)	{ m_EffectStatus = es; }
		void		SetCreatureID(DWORD df)				{ m_DelayFrame = df; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID		m_TargetID;				// 대상
		EFFECTSTATUS		m_EffectStatus;		// 붙이는 effect
		DWORD				m_DelayFrame;		// 지속 시간		
};

//----------------------------------------------------------------------
// 누군가(ID)를 snipping mode로..
//----------------------------------------------------------------------
class MActionResultNodeCreatureSnipping : public MActionResultNode {
	public :
		MActionResultNodeCreatureSnipping(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeCreatureSnipping() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_SNIPPING; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureSnipping() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// CreatureType을 바꾸는 Result
//----------------------------------------------------------------------
class MActionResultNodeChangeCreatureType : public MActionResultNode {
	public :
		MActionResultNodeChangeCreatureType(TYPE_OBJECTID tid=0, int ctype=0);
		~MActionResultNodeChangeCreatureType();

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CHANGE_CREATURE_TYPE; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeChangeCreatureType() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetTargetID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetCreatureType(int ctype)		{ m_CreatureType = ctype; }

		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
		int						m_CreatureType;
};

//----------------------------------------------------------------------
// 관 소환 (tid가 (x,y)에서 관 소환)
//----------------------------------------------------------------------
class MActionResultNodeSummonCasket : public MActionResultNode {
	public :
		MActionResultNodeSummonCasket(TYPE_OBJECTID tid=0, int casketType=0);
		~MActionResultNodeSummonCasket();

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_SUMMON_CASKET; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeSummonCasket() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetTargetID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetType(int type)				{ m_CasketType = type; }

		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
		int						m_CasketType;
};

//----------------------------------------------------------------------
// 관에서 나가기
//----------------------------------------------------------------------
class MActionResultNodeOpenCasket : public MActionResultNode {
	public :
		MActionResultNodeOpenCasket(TYPE_OBJECTID tid=0);
		~MActionResultNodeOpenCasket();

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_OPEN_CASKET; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeOpenCasket() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetTargetID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;		
};

//----------------------------------------------------------------------
// 누군가(ID)를 사라지게 하는 결과 
//----------------------------------------------------------------------
class MActionResultNodeCreatureTurning : public MActionResultNode {
	public :
		MActionResultNodeCreatureTurning(TYPE_OBJECTID tid=0, WORD frame=0) { m_TargetID = tid; m_TurnFrame = frame; }
		~MActionResultNodeCreatureTurning() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CREATURE_TURNING; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeCreatureTurning() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
		WORD					m_TurnFrame;
};

//----------------------------------------------------------------------
// 인벤토리 아이템(ID)을 없앰
//----------------------------------------------------------------------
class MActionResultNodeRemoveItemInInventory : public MActionResultNode {
	public :
		MActionResultNodeRemoveItemInInventory(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeRemoveItemInInventory() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_REMOVE_ITEM_IN_INVENTORY; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeRemoveItemInInventory() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetItemID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 인벤토리 아이템(ID)의 옵션을 바꿈
//----------------------------------------------------------------------
class MActionResultNodeChangeItemOptionInInventory : public MActionResultNode {
	public :
		MActionResultNodeChangeItemOptionInInventory(TYPE_OBJECTID tid=0, DWORD type = 0) { m_TargetID = tid; m_Type = type; }
		~MActionResultNodeChangeItemOptionInInventory() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_CHANGE_ITEM_OPTION_IN_INVENTORY; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeChangeItemOptionInInventory() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetItemID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		void		SetOption(DWORD type)	{ m_Type = type; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
		DWORD			m_Type;
};

class MActionResultNodeChangeItemGenderInInventory : public MActionResultNode 
{
public :
	MActionResultNodeChangeItemGenderInInventory( TYPE_OBJECTID tid=0, DWORD type = 0 ) { m_TargetID = tid; m_Type = type; }
	~MActionResultNodeChangeItemGenderInInventory() {}

	TYPE_ACTIONRESULTID		GetType() const { return ACTIONRESULTNODE_CHANGE_ITEM_GENDER; }
	bool					IsNodeChangeItemOptionInInventory() const { return false; }

	void					SetItemID(TYPE_OBJECTID tid) { m_TargetID = tid; }
	void					SetOption(DWORD type) { m_Type = type; }

	void					Execute();

public :
	TYPE_OBJECTID			m_TargetID;
	DWORD					m_Type;
};

//----------------------------------------------------------------------
// 누군가(ID)를 사라지게(가짜 캐릭터 죽을때) 하는 결과
//----------------------------------------------------------------------
class MActionResultNodeFakeDie : public MActionResultNode {
	public :
		MActionResultNodeFakeDie(TYPE_OBJECTID tid=0) { m_TargetID = tid; }
		~MActionResultNodeFakeDie() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_FAKE_DIE; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeFakeDie() const		{ return true; }

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void		SetCreatureID(TYPE_OBJECTID tid)	{ m_TargetID = tid; }
		
		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		TYPE_OBJECTID			m_TargetID;
};

//----------------------------------------------------------------------
// 이벤트를 추가한다
//----------------------------------------------------------------------
class MActionResultNodeEvent : public MActionResultNode {
	public :
		MActionResultNodeEvent(int skillID) { m_skillID = skillID; }
		~MActionResultNodeEvent() {}

		//------------------------------------------------------
		// ActionResult의 종류
		//------------------------------------------------------
		TYPE_ACTIONRESULTID		GetType() const		{ return ACTIONRESULTNODE_EVENT; }

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		bool		IsNodeEvent() const		{ return true; }

		//------------------------------------------------------
		// 결과를 적용시킨다.
		//------------------------------------------------------
		void		Execute();
		
	public :
		int			m_skillID;
};


//----------------------------------------------------------------------
// ActionInfo에 따른 Server에서 검증받은 결과들의 list
//----------------------------------------------------------------------
typedef	std::list<MActionResultNode*>	ACTIONRESULTNODE_LIST;


//----------------------------------------------------------------------
// 어떤 ActionInfo에 의해서 발생한 모든 결과를 
// Server로부터 검증을 받은 후에.. 그것들을 다~ 저장하고 있는 class
//
// destructor에서 Execute해버리기 때문에
// 실행할 필요가 없다면 Release해야한다.
//----------------------------------------------------------------------
class MActionResult {
	public :
		MActionResult();
		~MActionResult();

		//--------------------------------------------------------
		// Release
		//--------------------------------------------------------
		void		Release();

		//--------------------------------------------------------
		// 결과 하나를 추가한다.
		//--------------------------------------------------------
		void		Add(MActionResultNode* pNode);

		//--------------------------------------------------------
		// Get
		//--------------------------------------------------------
		bool		IsEmpty() const		{ return m_List.empty(); }
		int			GetSize() const		{ return m_List.size(); }
		ACTIONRESULTNODE_LIST::const_iterator GetIterator() const	{ return m_List.begin(); }

		//--------------------------------------------------------
		// 결과를 실행한다.
		//--------------------------------------------------------
		void		Execute();

	protected :
		ACTIONRESULTNODE_LIST		m_List;
};

#endif

