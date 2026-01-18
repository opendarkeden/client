// CImm.cpp: implementation of the CImm class.
//
//////////////////////////////////////////////////////////////////////

#include "client_PCH.h"

#include "CImm.h"
#include "VS_UI_Filepath.h"
#include "CSoundPartManager.h"
#include "MSoundTable.h"

extern CSoundPartManager*		g_pSoundManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CImm::CImm()
{

	m_bPlay = false;
	m_pDevice = CImmDevice::CreateDevice(NULL, g_hWnd);

//	if (m_pDevice)
//	{
//		m_strAction[FORCE_ACTION_DAMAGED] = "Damaged";
//		m_strAction[FORCE_ACTION_SWORD] = "Sword";
//		m_strAction[FORCE_ACTION_BLADE] = "Blade";
//		m_strAction[FORCE_ACTION_CROSS] = "Cross";
//		m_strAction[FORCE_ACTION_MACE] = "Mace";
//		m_strAction[FORCE_ACTION_SR] = "Sr";
//		m_strAction[FORCE_ACTION_SMG] = "Smg";
//		m_strAction[FORCE_ACTION_SG] = "Sg";
//		m_strAction[FORCE_ACTION_AR] = "Ar";
//		m_strAction[FORCE_ACTION_BIKE_UP] = "BikeUp";
//		m_strAction[FORCE_ACTION_BIKE_DOWN] = "BikeDown";
//		m_strAction[FORCE_ACTION_BIKE_ING] = "BikeIng";
//	}
	m_ProjectAction = NULL;
	m_ProjectSkill = NULL;
	m_ProjectInventory = NULL;
	m_ProjectUseItem = NULL;

}

CImm::~CImm()
{


	Disable();
	delete m_pDevice; // Release device

}


void CImm::Enable()
{
	if(!IsDevice())return;
	Disable();
	m_bPlay = true; 
	
	BOOL bRes;


	CImmProject ImmProject;
	if (ImmProject.OpenFile(IFR_UI, m_pDevice))
	{
		// Square Wave Vibration from Immersion Studio resource file 
		
		CImmPeriodic *m_pTemp;
		
		m_pTemp = new CImmPeriodic;
		bRes = m_pTemp->InitializeFromProject(ImmProject, "Drag");
		if (!bRes)
		{
			delete m_pTemp;
			m_pTemp = NULL;
		}
		m_vUI.push_back(m_pTemp);
		
		m_pTemp = new CImmPeriodic;
		bRes = m_pTemp->InitializeFromProject(ImmProject, "Window");
		if (!bRes)
		{
			delete m_pTemp;
			m_pTemp = NULL;
		}
		m_vUI.push_back(m_pTemp);
		
		m_pTemp = new CImmPeriodic;
		bRes = m_pTemp->InitializeFromProject(ImmProject, "Button");
		if (!bRes)
		{
			delete m_pTemp;
			m_pTemp = NULL;
		}
		m_vUI.push_back(m_pTemp);
		
		m_pTemp = new CImmPeriodic;
		bRes = m_pTemp->InitializeFromProject(ImmProject, "Grid");
		if (!bRes)
		{
			delete m_pTemp;
			m_pTemp = NULL;
		}
		m_vUI.push_back(m_pTemp);
		
		ImmProject.Close();
	}

	m_ProjectAction = new CImmProject;
	if(!m_ProjectAction->OpenFile(IFR_ACTION, m_pDevice))
	{
		delete m_ProjectAction;
		m_ProjectAction = NULL;
	}
	
	m_ProjectSkill = new CImmProject;
	if(!m_ProjectSkill->OpenFile(IFR_SKILL, m_pDevice))
	{
		delete m_ProjectSkill;
		m_ProjectSkill = NULL;
	}
	
	m_ProjectInventory = new CImmProject;
	if(!m_ProjectInventory->OpenFile(IFR_INVENTORY, m_pDevice))
	{
		delete m_ProjectInventory;
		m_ProjectInventory = NULL;
	}
	
	m_ProjectUseItem = new CImmProject;
	if(!m_ProjectUseItem->OpenFile(IFR_USE_ITEM, m_pDevice))
	{
		delete m_ProjectUseItem;
		m_ProjectUseItem = NULL;
	}
	
}

void CImm::Disable()
{
	if(!IsDevice())return;
	m_bPlay = false; 


	for(unsigned int i = 0; i < m_vUI.size(); i++)
	{
		if(m_vUI[i] != NULL)
			delete m_vUI[i];
	}
	m_vUI.clear();

	if(m_ProjectAction != NULL)
	{
		m_ProjectAction->Close();
		delete m_ProjectAction;
		m_ProjectAction = NULL;
	}

	if(m_ProjectSkill != NULL)
	{
		m_ProjectSkill->Close();
		delete m_ProjectSkill;
		m_ProjectSkill = NULL;
	}

	if(m_ProjectInventory != NULL)
	{
		m_ProjectInventory->Close();
		delete m_ProjectInventory;
		m_ProjectInventory = NULL;
	}

	if(m_ProjectUseItem != NULL)
	{
		m_ProjectUseItem->Close();
		delete m_ProjectUseItem;
		m_ProjectUseItem = NULL;
	}

}

//UI
void CImm::ForceUI(const unsigned int ID) const
{
	if(m_bPlay && m_pDevice)
		if(ID < m_vUI.size() && m_vUI[ID] != NULL)
			m_vUI[ID]->Start();
}


void CImm::ForceAction(const int sound_id) const
{
	if(m_bPlay && m_pDevice && m_ProjectAction && g_pSoundTable && sound_id < g_pSoundTable->GetSize())
	{
		const char *pFilename = strrchr((*g_pSoundTable)[sound_id].Filename.GetString(), '\\');
		if(pFilename != NULL)
		{
			m_ProjectAction->Start(pFilename+1);
		}
	}
}


void CImm::ForceSkill(const int sound_id) const
{
	if(m_bPlay && m_pDevice && m_ProjectSkill && g_pSoundTable && sound_id < g_pSoundTable->GetSize())
	{
		const char *pFilename = strrchr((*g_pSoundTable)[sound_id].Filename.GetString(), '\\');
		if(pFilename != NULL)
		{
			m_ProjectSkill->Start(pFilename+1);
		}
	}
}


void CImm::ForceUseItem(const int sound_id) const
{
	if(m_bPlay && m_pDevice && m_ProjectUseItem && g_pSoundTable && sound_id < g_pSoundTable->GetSize())
	{
		const char *pFilename = strrchr((*g_pSoundTable)[sound_id].Filename.GetString(), '\\');
		if(pFilename != NULL)
		{
			m_ProjectUseItem->Start(pFilename+1);
		}
	}
}


void CImm::ForceInventory(const int sound_id) const
{
	if(m_bPlay && m_pDevice && m_ProjectInventory && g_pSoundTable && sound_id < g_pSoundTable->GetSize())
	{
		const char *pFilename = strrchr((*g_pSoundTable)[sound_id].Filename.GetString(), '\\');
		if(pFilename != NULL)
		{
			m_ProjectInventory->Start(pFilename+1);
		}
	}
}

