//----------------------------------------------------------------------
// MObjectSelector
//----------------------------------------------------------------------
// mouse로 뭔가를 선택할때..
// 선택해도 되나?마나?를 결정한다.
//
// -er인가 -or인가 - -;
//----------------------------------------------------------------------

#ifndef __MOBJECTSELECTOR__
#define __MOBJECTSELECTOR__

class MCreature;

class MObjectSelector {
	public :
		enum SELECT_TARGET
		{
			SELECT_ALL,
			SELECT_FRIEND,
			SELECT_ENEMY
		};

		enum SELECT_BY
		{
			SELECT_BY_RACE,
			SELECT_BY_GUILD,
		};


	public :
		MObjectSelector();
		~MObjectSelector();

		//--------------------------------------------------------------
		// 선택 범위
		//--------------------------------------------------------------
		void	SelectAll()			{ m_SelectTarget = SELECT_ALL; }		
		void	SelectFriend()		{ m_SelectTarget = SELECT_FRIEND; }
		void	SelectEnemy()		{ m_SelectTarget = SELECT_ENEMY; }

		//--------------------------------------------------------------
		// 선택 공격
		//--------------------------------------------------------------
		void	SelectByRace()		{ m_SelectBy = SELECT_BY_RACE; }
		void	SelectByGuild()		{ m_SelectBy = SELECT_BY_GUILD; }

		//--------------------------------------------------------------
		// Can Select - 선택할 수 있는가?
		//--------------------------------------------------------------
		BOOL	CanSelect(MCreature* pCreature);

		//--------------------------------------------------------------
		// Can Attack - 공격할 수 있는가?
		//--------------------------------------------------------------
		BOOL	CanAttack(MCreature* pCreature);

		//--------------------------------------------------------------
		// IsWarEnemy - 전쟁에 관련된 적인가?
		//--------------------------------------------------------------
		BOOL	IsWarEnemy(MCreature* pCreature);

		// pk가 가능한 zone 인지..-> player에만 한해서
//		BOOL	IsAblePKbyZone(MCreature* pCreature);

	protected :
		SELECT_TARGET		m_SelectTarget;
		SELECT_BY			m_SelectBy;
};

extern MObjectSelector* g_pObjectSelector;

#endif

