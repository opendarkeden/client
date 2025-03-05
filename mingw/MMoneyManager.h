//-----------------------------------------------------------------------------
// MMoneyManager.h
//-----------------------------------------------------------------------------
// Money는 MoneyLimit를 넘으면 안된다.
//-----------------------------------------------------------------------------

#ifndef __MMONEYMANAGER_H__
#define __MMONEYMANAGER_H__

class MMoneyManager {

	public :
		MMoneyManager();
		MMoneyManager(const MMoneyManager& mm); 	
		~MMoneyManager();

		//-------------------------------------------------------
		// Money
		//-------------------------------------------------------		
		bool		SetMoney(int money);
		int			GetMoney() const		{ return m_Money; }

		bool		AddMoney(int money);
		bool		UseMoney(int money);

		// money만큼 돈을 add/use할 수 있나?
		bool		CanAddMoney(int money);
		bool		CanUseMoney(int money);

		//-------------------------------------------------------		
		// Money Limit
		//-------------------------------------------------------		
		void		SetMoneyLimit(int limit)	{ m_MoneyLimit = limit; }
		int			GetMoneyLimit() const		{ return m_MoneyLimit;}
		int			GetMaxAddMoney() const		{ return m_MoneyLimit - m_Money; }

	protected :
		int			m_MoneyLimit;	// 가질 수 있는 돈의 한계
		int			m_Money;		// 돈 - -;
};

extern MMoneyManager*		g_pMoneyManager;

#endif

