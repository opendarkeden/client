//----------------------------------------------------------------------
// MSkillManager.h
//----------------------------------------------------------------------
//
// < SKILLINFO_NODE >
//
//		하나의 Skill에 대한 정보로서
//			- SkillID, Skill이름
//			- 출력 좌표, 출력SpriteID
//			- 다음에 배울 수 있는 기술들...등의 정보가 있다.
//
//
// < SkillInfoTable >
//
//		SKILLINFO_NODE의 array이다.
//		ACTIONINFO에 정해진 순서(ID)에 따른다.
//
//
// < MSkillDomain >
//
//		SkillID(==ACTIONINFO)들의 집합이다.
//		SkillDef.h에 지정된 Skill들에 대해서 
//		배운것/배울것/못배운것..으로 나누기 위한 것.
//		stl의 map을 이용해서 SkillID만 저장한다.
//		(!) SkillID에 따라서 MSkillInfoTable을 참조해서 정보를 얻을 수 있다.
//
//
// < MSkillManager >
//
//		SkillDomain의 array이다.
//
//----------------------------------------------------------------------
//
/*
	//---------------------------------------------------------------------
	//
	// Skill Tree 전체를 초기화 하는 방법
	//
	//---------------------------------------------------------------------
	g_SkillManager.Init( MAX_SKILLDOMAIN );

	//---------------------------------------------------------------------
	// 기본 기술로부터 skill tree를 초기화한다.
	//---------------------------------------------------------------------
	g_SkillManager[SKILLDOMAIN_BLADE].SetRootSkill( SKILL_SINGLE_BLOW );
	g_SkillManager[SKILLDOMAIN_SWORD].SetRootSkill( SKILL_DOUBLE_IMPACT );
	g_SkillManager[SKILLDOMAIN_GUN].SetRootSkill( SKILL_SNIPPING );
	g_SkillManager[SKILLDOMAIN_GUN].SetRootSkill( SKILL_SHARP_SHOOTING );
	g_SkillManager[SKILLDOMAIN_ENCHANT].SetRootSkill( MAGIC_CREATE_HOLY_WATER );
	g_SkillManager[SKILLDOMAIN_HEAL].SetRootSkill( MAGIC_CURE_POISON );
	g_SkillManager[SKILLDOMAIN_VAMPIRE].SetRootSkill( MAGIC_HIDE );	


	// 내부에서 하므로..	  2001.7.10
	 g_SkillManager.Init()만 하면 된다.


	class ofstream logFile("log\\sword.txt");

	//---------------------------------------------------------------------
	//
	// g_SkillManager에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& swordDomain = g_SkillManager[SKILLDOMAIN_SWORD];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	swordDomain.LearnSkill( SKILL_DOUBLE_IMPACT );
	swordDomain.LearnSkill( SKILL_TRIPLE_SLASHER );
	swordDomain.LearnSkill( SKILL_SCREW_SLASHER );
	swordDomain.LearnSkill( SKILL_RAINBOW_SLASHER );
	swordDomain.LearnSkill( SKILL_CRUSH_COMBO );
	swordDomain.LearnSkill( SKILL_CROSS_COUNTER );

	//---------------------------------------------------------------------
	// Sword Domain의 모든 기술들을 출력한다.
	//---------------------------------------------------------------------
	swordDomain.SetBegin();
	
	while (swordDomain.IsNotEnd())
	{
		// skill의 id와 status
		ACTIONINFO					id		= swordDomain.GetSkillID();
		MSkillDomain::SKILLSTATUS	status	= swordDomain.GetSkillStatus();

		//---------------------------------------
		// status는 다음과 같다. 
		//---------------------------------------
		//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
		//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
		//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
		//---------------------------------------
		
		//---------------------------------------
		// id를 알면 g_SkillInfoTable에서 
		// 그 id의 skill에 대한 정보를 얻을 수 있다.
		//---------------------------------------
		logFile << "[" << id << "] " << g_SkillInfoTable[id].GetName()
				<< " = " << (int)status << endl;

		// 다음
		swordDomain.Next();
	}

	logFile.close();
*/
//----------------------------------------------------------------------

#ifndef	__MSKILLMANAGER_H__
#define	__MSKILLMANAGER_H__

#pragma warning(disable:4786)

#include "SkillDef.h"
#include "MString.h"
#include "CTypeTable.h"
#include "DrawTypeDef.h"
#include "ExpInfo.h"
#include "RaceType.h"

#include <fstream>
using namespace std;



//----------------------------------------------------------------------
//
//		SKILLINFO_NODE
//
//----------------------------------------------------------------------
class SKILLINFO_NODE {
	public :
		enum ELEMENTAL_DOMAIN
		{
			ELEMENTAL_DOMAIN_NO_DOMAIN = -1,     // 무속성
			ELEMENTAL_DOMAIN_FIRE,              // 불계열
			ELEMENTAL_DOMAIN_WATER,             // 물계열
			ELEMENTAL_DOMAIN_EARTH,             // 대지계열
			ELEMENTAL_DOMAIN_WIND,              // 바람계열
			ELEMENTAL_DOMAIN_COMBAT,            // 전투 일반 계열
			ELEMENTAL_DOMAIN_ELEMENTAL_COMBAT,  // 전투 정령 계열
			ELEMENTAL_DOMAIN_ETC,               // 기타(계열구분없음)
			
			ELEMENTAL_DOMAIN_MAX
		};

		typedef	std::list<ACTIONINFO>		SKILLID_LIST;				
		typedef	std::list<int>				SKILLTYPE_LIST;

	public :
		SKILLINFO_NODE();
		SKILLINFO_NODE(int level, const char* name, int x, int y, TYPE_SPRITEID sid, const char* hname)
		{
			Set(level, name, x,y, sid, hname);
		}

		~SKILLINFO_NODE() {}

		//------------------------------------------------------
		// Set
		//------------------------------------------------------
		void			Set(int level, const char* name, int x, int y, TYPE_SPRITEID sid, const char* hname)
		{
			m_Level		= level;
			m_Name		= name;
			m_X			= x;
			m_Y			= y;
			m_SpriteID	= sid;
			m_HName		= hname;
		}

		void			SetMP(int mp)				{ m_MP = mp; }

		//------------------------------------------------------
		// Add NextSkill
		//------------------------------------------------------
		bool			AddNextSkill(ACTIONINFO id);
		//bool			RemoveNextSkill(ACTIONINFO id);	// 필요 없는 함수당..

		//------------------------------------------------------
		// Get
		//------------------------------------------------------
		int					GetLevel() const		{ return m_Level; }		
		const char*			GetName() const				{ return m_Name.GetString(); }
		const char*			GetHName() const			{ return m_HName.GetString(); }
		int					GetX() const				{ return m_X; }
		int					GetY() const				{ return m_Y; }
		TYPE_SPRITEID		GetSpriteID() const			{ return m_SpriteID; }
		const SKILLID_LIST& GetNextSkillList() const	{ return m_listNextSkill; }
		int					GetMP() const				{ return m_MP; }

		//------------------------------------------------------
		// 현재 Player의 skill상태에 대한 정보
		//------------------------------------------------------
		void				SetExpLevel(int lev)		{ m_ExpLevel = lev; }
		int					GetExpLevel() const			{ return m_ExpLevel; }
		
		//------------------------------------------------------
		// Passive Skill인가? - 적용은 되지만 사용 불가 스킬
		//------------------------------------------------------
		bool			IsPassive() const	{ return m_bPassive; }
		void			SetPassive()		{ m_bPassive = true; }
		void			UnSetPassive()		{ m_bPassive = false; }

		//------------------------------------------------------
		// (Always) Active Skill인가? - 무기에 구애받지 않고 항상 사용 가능
		//------------------------------------------------------
		bool			IsActive() const	{ return m_bActive; }
		void			SetActive()			{ m_bActive = true; }
		void			UnSetActive()		{ m_bActive = false; }

		//------------------------------------------------------
		// skill exp
		//------------------------------------------------------
		void				SetSkillExp(int exp)		{ m_SkillExp = exp; }
		int					GetSkillExp() const			{ return m_SkillExp; }		

		//------------------------------------------------------
		// skill step
		//------------------------------------------------------
		void				SetSkillStep(SKILL_STEP step)	{ m_SkillStep = step; }
		SKILL_STEP			GetSkillStep() const			{ return m_SkillStep; }				

		//------------------------------------------------------
		// Available Level
		//------------------------------------------------------
		void				SetLearnLevel(int lv)	{ m_LearnLevel = lv; }
		int					GetLearnLevel() const	{ return m_LearnLevel; }						

		//------------------------------------------------------
		// 종족 스킬 구분
		//------------------------------------------------------
		void				SetVampireSkill()		{ m_eSkillRace = RACE_VAMPIRE; }
		bool				IsVampireSkill()		{ return m_eSkillRace == RACE_VAMPIRE; }
		void				SetSlayerSkill()		{ m_eSkillRace = RACE_SLAYER; }
		bool				IsSlayerSkill()			{ return m_eSkillRace == RACE_SLAYER; }
		void				SetOustersSkill()		{ m_eSkillRace = RACE_OUSTERS; }
		bool				IsOustersSkill()		{ return m_eSkillRace == RACE_OUSTERS; }
		
		//------------------------------------------------------
		// 기술 사용 Delay
		//------------------------------------------------------
		void				SetDelayTime(DWORD delay);		// 기술 사용후 다시 사용할 수 있는 delay시간 설정
		DWORD				GetDelayTime() const	{ return m_DelayTime; }

		bool				IsAvailableTime() const;		// 지금 사용 가능한가?
		void				SetAvailableTime(int delay = 0);				// 지금 바로 사용 가능하다.
		DWORD				GetAvailableTimeLeft() const;	// 남은 사용 시간
		
		void				SetNextAvailableTime();			// 다음 사용 가능한 시간을 결정한다.

		//------------------------------------------------------
		// 사용 가능한 기술인가?
		//------------------------------------------------------
		bool				IsEnable() const		{ return m_bEnable; }
		void				SetEnable(bool enable=true);
		//void				SetDisable();

		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);

		//------------------------------------------------------
		// Load / Save SERVER_SKILLINFO
		//------------------------------------------------------
		void		LoadFromFileServerSkillInfo(ifstream& file);
		void        SaveFromFileServerSkillInfo(ofstream& file);

	protected :
		
		int				m_Level;			// 기술의 level		

		MString			m_Name;				// 기술 이름
		MString			m_HName;			// 기술의 영문 이름.

		int				m_X, m_Y;			// 화면에서의 출력 시작 위치
		TYPE_SPRITEID	m_SpriteID;			// 기술의 Icon Sprite

		int				m_MP;				// MP 소비량

		SKILLID_LIST	m_listNextSkill;	// 다음에 배울 수 있는 기술들		

		bool			m_bPassive;			// passive skill인가?

		bool			m_bActive;			// 항상 사용가능한 skill인가?

		//------------------------------------------------------
		// 현재 Player의 skill상태에 대한 정보
		//------------------------------------------------------
		int				m_ExpLevel;			// 스킬 레벨
		int				m_SkillExp;			// 스킬 경험치

		//------------------------------------------------------
		// Skill의 단계
		//------------------------------------------------------
		SKILL_STEP		m_SkillStep;

		//------------------------------------------------------
		// 기술 배울 수 있는 레벨
		//------------------------------------------------------
		int				m_LearnLevel;

		//------------------------------------------------------
		// 종족 기술 구분
		//------------------------------------------------------
		Race			m_eSkillRace;

		//------------------------------------------------------
		// 기술 사용 Delay
		//------------------------------------------------------
		DWORD			m_DelayTime;		// 기술 사용후 다시 사용가능한 delay
		DWORD			m_AvailableTime;	// 다시 사용 가능한 시간
		
		//------------------------------------------------------
		// 사용가능한 기술인가?
		//------------------------------------------------------
		bool			m_bEnable;	

	public :
		int			DomainType;		// 그 기술이 어느 도메인에 속하는가.
		int			minDamage;		// 최소 데미지 또는 효과치.
		int			maxDamage;		// 최대 데미지 또는 효과치.
		int			minDelay;		// 최소 사용 딜레이.
		int			maxDelay;		// 최대 사용 딜레이.
		int			minCastTime;	// 최소 캐스팅 타임.
		int			maxCastTime;	// 최대 캐스팅 타임.		
		int			minDuration;	// 최소 지속 시간
		int			maxDuration;	// 최대 지속 시간
		int			minRange;		// 최소 사정거리
		int			maxRange;		// 최대 사정거리
		int			maxExp;			// 그 기술의 100% 경험치. 1 회당 + 1 씩 올라감
		
		// 아우스터즈용
	public:
		int			SkillPoint;
		int			LevelUpPoint;
		SKILLTYPE_LIST		SkillTypeList;
		int			Fire;
		int			Water;
		int			Earth;
		int			Wind;
		int			Sum;
		int			Wristlet;
		int			Stone1;
		int			Stone2;
		int			Stone3;
		int			Stone4;
		int			ElementalDomain;
		BYTE		CanDelete; // 스킬을 아예 삭제 할수 있는지..
		
};

//----------------------------------------------------------------------
//
//	MSkillSet
//
//----------------------------------------------------------------------
// SkillID(==ACTIONINFO)들의 집합이다.
//----------------------------------------------------------------------
const BYTE	FLAG_SKILL_ENABLE		= 1;		// 현재(!) 사용가능한가?

class SKILLID_NODE {
	public :
		ACTIONINFO		SkillID;	
		BYTE			Flag;

	public :
		SKILLID_NODE(ACTIONINFO sid, BYTE f=FLAG_SKILL_ENABLE)
		{
			SkillID = sid;
			Flag	= f;
		}

		void	SetEnable()			{ Flag |= FLAG_SKILL_ENABLE; }
		void	SetDisable()		{ Flag &= ~FLAG_SKILL_ENABLE; }
		BYTE	IsEnable() const	{ return Flag & FLAG_SKILL_ENABLE; }

		void	operator = (const SKILLID_NODE& node)
		{
			SkillID		= node.SkillID;
			Flag		= node.Flag;
		}

};
		
class MSkillSet : public std::map<ACTIONINFO, SKILLID_NODE> {
	public :
		// <SkillID>
		typedef	std::map<ACTIONINFO, SKILLID_NODE>		SKILLID_MAP;
	

	public :
		MSkillSet() {}
		~MSkillSet() {}

		//------------------------------------------------------
		// Skill
		//------------------------------------------------------
		bool			AddSkill(ACTIONINFO id, BYTE flag=FLAG_SKILL_ENABLE);		
		bool			RemoveSkill(ACTIONINFO id);		// SkillID 제거
		
		//------------------------------------------------------
		// Enable?
		//------------------------------------------------------
		bool			IsEnableSkill(ACTIONINFO id) const;	// id의 skill이 사용 가능한가?
		bool			EnableSkill(ACTIONINFO id);		// 사용가능
		bool			DisableSkill(ACTIONINFO id);	// 사용 불가능

		//------------------------------------------------------
		// 현재 사용가능한 skill들 체크
		//------------------------------------------------------
		// SetAvailableSkills하면 자동으로 CheckMP도 된다.
		//------------------------------------------------------
		void			SetAvailableSkills();	// 전체 체크
		void			CheckMP();				// mp check

		void			SetAvailableVampireSkills();

	
	protected :
};

//----------------------------------------------------------------------
//
//	MSkillDomain
//
//----------------------------------------------------------------------
// 한 SkillDomain을 관리한다.
//----------------------------------------------------------------------
class MSkillDomain {
	public :
		enum SKILLSTATUS
		{
			SKILLSTATUS_NULL = 0,		// 없는 Skill이다.
			SKILLSTATUS_LEARNED,		// 배웠다.
			SKILLSTATUS_NEXT,			// 다음에 배울 수 있다.
			SKILLSTATUS_OTHER			// 아직은 배울 수 없다.			
		};

		// <SkillID>와 <Skill습득 상태>
		typedef	std::map<ACTIONINFO, SKILLSTATUS>		SKILLID_MAP;
	
		// Skill Step List
		typedef std::vector<ACTIONINFO>					SKILL_STEP_LIST;

		// Skill Step Map
		typedef std::map<SKILL_STEP, SKILL_STEP_LIST*>	SKILL_STEP_MAP;


	public :
		MSkillDomain();
		~MSkillDomain();

		//----------------------------------------------------------------------
		// Add Skill - 하위의 skill들을 모조리 domain에 추가한다.
		//----------------------------------------------------------------------
		bool		AddSkill(ACTIONINFO id);

		//------------------------------------------------------
		// Clear
		//------------------------------------------------------
		void			Clear();
		void			ClearSkillList();

		//------------------------------------------------------
		// Set Root Skill - domain의 최상위 skill부터 
		//					하위 skill들을 추가한다.
		//------------------------------------------------------
		bool			SetRootSkill(ACTIONINFO id, bool reset = true);		

		//------------------------------------------------------
		// Set SkillStatus
		//------------------------------------------------------
		// Skill의 상태 변경
		//bool			SetSkillStatus(ACTIONINFO id, SKILLSTATUS status);	

		//------------------------------------------------------
		// Get SkillStatus
		//------------------------------------------------------
		// Skill의 상태는?
		SKILLSTATUS		GetSkillStatus(ACTIONINFO id) const;	
		
		//bool			RemoveSkill(ACTIONINFO id);				// Skill 제거

		//------------------------------------------------------
		// New Skill
		//------------------------------------------------------
		bool			HasNewSkill() const		{ return m_bNewSkill; }
		void			SetNewSkill()			{ m_bNewSkill = true; }	// 새로 배울 skill point가 생겼다.
		void			UnSetNewSkill()			{ m_bNewSkill = false; }
		
		//------------------------------------------------------
		// Learn Skill
		//------------------------------------------------------
		bool			LearnSkill(ACTIONINFO id);		// skill을 배운다.
		bool			UnLearnSkill(ACTIONINFO id);	// 배운 skill을 못 배운걸로 한다.

		//------------------------------------------------------
		// Iterator
		//------------------------------------------------------
		void			SetBegin()			{ m_iterator = m_mapSkillID.begin(); }
		ACTIONINFO		GetSkillID()		{ return (*m_iterator).first; }
		SKILLSTATUS		GetSkillStatus()	{ return (*m_iterator).second; }
		void			Next()				{ m_iterator++; }		
		bool			IsEnd() const		{ return m_iterator==m_mapSkillID.end(); }
		bool			IsNotEnd() const	{ return m_iterator!=m_mapSkillID.end(); }
		int				GetSize() const		{ return m_mapSkillID.size(); }

		//------------------------------------------------------
		// Level / Exp
		//------------------------------------------------------
		int			GetDomainLevel() const		{ return m_DomainLevel; }
		void		SetDomainLevel(int dl)		{ m_DomainLevel = dl; }
		DWORD		GetDomainExpRemain() const		{ return m_DomainExpRemain; }
		void		SetDomainExpRemain(DWORD de)		{ m_DomainExpRemain = de; }

		//------------------------------------------------------
		// Skill Step 
		//------------------------------------------------------
		BOOL								IsExistSkillStep(SKILL_STEP ss) const;
		const SKILL_STEP_LIST*				GetSkillStepList(SKILL_STEP ss) const;		
		

		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);

		//------------------------------------------------------
		// Load SERVER_SKILLINFO
		//------------------------------------------------------
		void		LoadFromFileServerDomainInfo(ifstream& file);

		//------------------------------------------------------
		// Exp
		//------------------------------------------------------
		const ExpInfo&	GetExpInfo(int level) const;

		// 2004, 11, 9, sobeit add start - 강제로 배울수 있다고 세팅한다.-_-
		void		AddNextSkillForce(ACTIONINFO id);
		bool		IsAvailableDeleteSkill(ACTIONINFO id);
		// 2004, 11, 9, sobeit add end
	protected :	

		//----------------------------------------------------------------------
		// Remove NextSkill - 다음에 배울 수 있게 표시된 기술들을 제거한다.
		//----------------------------------------------------------------------
		void		RemoveNextSkill(ACTIONINFO id);
		void		AddNextSkill(ACTIONINFO id);

		//------------------------------------------------------
		// Level
		//------------------------------------------------------
		void		SetMaxLevel();

		//------------------------------------------------------
		// Add Skill Step
		//------------------------------------------------------
		// ss에 ai를 추가한다.
		void		AddSkillStep(SKILL_STEP ss, ACTIONINFO ai);


	protected :
		SKILLID_MAP::const_iterator	m_iterator;
		SKILLID_MAP					m_mapSkillID;		
		
		int							m_MaxLevel;			// domain의 최고 기술 level
		int							m_MaxLearnedLevel;	// 현재 배운것 중에서 MAX skill ID
		ACTIONINFO*					m_pLearnedSkillID;	// 배운 skill ID		

		// 새로운 skill을 배울 수 있는가?
		bool				 		m_bNewSkill;

		// Domain Level
		int							m_DomainLevel;
		DWORD						m_DomainExpRemain;

		// SkillStep map
		SKILL_STEP_MAP				m_mapSkillStep;

		//
		ExpTable					m_DomainExpTable;
};


//----------------------------------------------------------------------
//
//	SkillInfoTable
//
//----------------------------------------------------------------------
class MSkillInfoTable : public CTypeTable<SKILLINFO_NODE> {
	public :
		MSkillInfoTable();
		~MSkillInfoTable();

		// 바뀌는 정보만 초기화
		void			Init();

		void			LoadFromFileServerSkillInfo(ifstream& file);
		void			SaveFromFileServerSkillInfo(ofstream& file);
};

extern MSkillInfoTable*		g_pSkillInfoTable;


//----------------------------------------------------------------------
//
// SkillManager
//
//----------------------------------------------------------------------
class MSkillManager : public CTypeTable<MSkillDomain>
{
	public :
		MSkillManager();
		~MSkillManager();

		void			Init();
		void			InitSkillList();

		//------------------------------------------------------
		// Load SERVER_SKILLINFO
		//------------------------------------------------------
		void		LoadFromFileServerDomainInfo(ifstream& file);
};

//----------------------------------------------------------------------
//
//	global
//
//----------------------------------------------------------------------
extern MSkillManager*	g_pSkillManager;

extern MSkillSet*		g_pSkillAvailable;

#define					HOLYLAND_BONUS_MAX	12
#define					SWEEPER_BONUS_MAX	12

extern bool				g_abHolyLandBonusSkills[HOLYLAND_BONUS_MAX];
extern bool				g_abSweeperBonusSkills[SWEEPER_BONUS_MAX];
#endif

