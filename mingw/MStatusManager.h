//----------------------------------------------------------------------
// MStatusManager.h
//----------------------------------------------------------------------


#ifndef __MSTATUSMANAGER_H__
#define	__MSTATUSMANAGER_H__

#include "DllInfo.h"

class __EX MStatusManager {
	public :
		MStatusManager();
		~MStatusManager();

		//--------------------------------------------------------------
		// 계산을 위한 값 설정
		//--------------------------------------------------------------
		//Edit by Sonic Start 2006.9.6
		void		Set(int str,int dex, int intel);
		void		SetCurrentWeaponDomain(int domain, int level);
		/*
		void		Set(int str, int dex, int intel)		
		{

			m_STR = str;
			m_DEX = dex;
			m_INT = intel;
		}
		

		//--------------------------------------------------------------
		// 현재 사용하는 무기의 domain level을 설정한다.
		// Vampire는 domain level을 설정한다.
		//--------------------------------------------------------------
		void		SetCurrentWeaponDomain(int domain, int level)
		{
			m_Domain	 = domain;
			m_DomainLevel = level;
		}
		*/
		//Edit by sonic End 2006.9.6
		//--------------------------------------------------------------
		// 특정한 값을 얻어낸다.
		//--------------------------------------------------------------
		int			GetTOHIT();
		int			GetMinDAM();		// min
		int			GetMaxDAM();		// max
		int			GetDefense();
		int			GetProtection();
		int			GetAttackSpeed();
		//int			GetCC();

	protected :
		int			m_STR;
		int			m_DEX;
		int			m_INT;

		int			m_Domain;
		int			m_DomainLevel;
};

extern __EX MStatusManager		g_StatusManager;

#endif

