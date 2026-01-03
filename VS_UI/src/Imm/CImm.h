// CImm.h: interface for the CImm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __IFC_H__
#define __IFC_H__


//Logitech iFeel Mouse 지원코드
#include "ifc.h"
#include <vector>

class CImm  
{
public:
	enum FORCE_UI_ID
	{
		FORCE_UI_DRAG,
		FORCE_UI_WINDOW,
		FORCE_UI_BUTTON,
		FORCE_UI_GRID,

		FORCE_UI_MAX,
	};

//	enum FORCE_ACTION_ID
//	{
//		FORCE_ACTION_DAMAGED,
//		FORCE_ACTION_SWORD,
//		FORCE_ACTION_BLADE,
//		FORCE_ACTION_CROSS,
//		FORCE_ACTION_MACE,
//		FORCE_ACTION_SR,
//		FORCE_ACTION_SMG,
//		FORCE_ACTION_SG,
//		FORCE_ACTION_AR,
//		FORCE_ACTION_BIKE_UP,
//		FORCE_ACTION_BIKE_DOWN,
//		FORCE_ACTION_BIKE_ING,
//
//		FORCE_ACTION_MAX,
//	};

private:
	bool						m_bPlay;
	CImmDevice					*m_pDevice;

	//UI는 몇개 안되고 자주 사용되므로 미리 로딩해 놓는다.
	//나머지는 파일에서 직접 사용
	CImmProject					*m_ProjectAction;		//특정동작 할때
	CImmProject					*m_ProjectSkill;		//스킬쓸때
	CImmProject					*m_ProjectInventory;	//아이템 집을때
	CImmProject					*m_ProjectUseItem;		//아이템 사용할때

	std::vector<CImmPeriodic*>	m_vUI;
//	std::string					m_strAction[FORCE_ACTION_MAX];

public:
	CImm();
	~CImm();

	void	Enable();
	void	Disable();
	bool	IsDevice()	{ return m_pDevice != NULL; }

	//Force함수들
	void	ForceUI(const unsigned int ID) const;						//UI
	void	ForceAction(const int sound_id) const;					//특정동작할때
	void	ForceSkill(const int sound_id) const;			//스킬쓸때 wav파일이름 넣어줄것
	void	ForceInventory(const int sound_id) const;		//아이템 집을때
	void	ForceUseItem(const int sound_id) const;		//아이템 사용할때


};

extern CImm *gpC_Imm;
extern HWND g_hWnd;

#endif // !defined(AFX_CIMM_H__F8627A31_A1CE_4724_9B35_8601BDDC598E__INCLUDED_)
