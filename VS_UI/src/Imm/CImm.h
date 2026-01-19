// CImm.h: interface for the CImm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __IFC_H__
#define __IFC_H__



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



	CImmProject					*m_ProjectAction;
	CImmProject					*m_ProjectSkill;
	CImmProject					*m_ProjectInventory;
	CImmProject					*m_ProjectUseItem;

	std::vector<CImmPeriodic*>	m_vUI;
//	std::string					m_strAction[FORCE_ACTION_MAX];

public:
	CImm();
	~CImm();

	void	Enable();
	void	Disable();
	bool	IsDevice()	{ return m_pDevice != NULL; }


	void	ForceUI(const unsigned int ID) const;						//UI
	void	ForceAction(const int sound_id) const;
	void	ForceSkill(const int sound_id) const;
	void	ForceInventory(const int sound_id) const;
	void	ForceUseItem(const int sound_id) const;


};

extern CImm *gpC_Imm;
extern HWND g_hWnd;

#endif // !defined(AFX_CIMM_H__F8627A31_A1CE_4724_9B35_8601BDDC598E__INCLUDED_)
