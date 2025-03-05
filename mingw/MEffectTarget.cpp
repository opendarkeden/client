//----------------------------------------------------------------------
// MEffectTarget.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffectTarget.h"
#include "MZoneTable.h"
#include "MPlayer.h"
#include "DebugInfo.h"

//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
BYTE		MEffectTarget::s_EffectID	= 0;

//----------------------------------------------------------------------
//
//						MEffectTarget
//
//----------------------------------------------------------------------
MEffectTarget::MEffectTarget(const MEffectTarget& target)			
{
	m_pResult = NULL; 

	if (&target!=NULL) 
	{
		*this = target; 
	}

	// 객체 ID를 할당한다.
	//m_InstanceID = s_InstanceID++;
	m_EffectID = target.m_EffectID;
	
	m_ServerID = OBJECTID_NULL;

	m_bResultTime = false;
}	

MEffectTarget::MEffectTarget(BYTE max)
{			
	m_MaxPhase		= max;
	m_CurrentPhase	= 0;

	// 결과
	//m_nResultActionInfo = ACTIONINFO_NULL;
	m_pResult		= NULL;

	// 객체 ID를 할당한다.
	//m_InstanceID = s_InstanceID++;
	m_EffectID = 0;

	m_bResultTime = false;

	m_DelayFrame	= 0;
}

MEffectTarget::~MEffectTarget() 
{ 
	DEBUG_ADD_FORMAT("delete EffectTarget. id=%d", (int)m_EffectID);

	if (m_pResult!=NULL) 
	{
		delete m_pResult; 
		m_pResult = NULL;
	}

	DEBUG_ADD("del res");

	// 죽음의 코드 - -;
	if (g_pPlayer!=NULL)
	{
		g_pPlayer->RemoveEffectTarget( m_EffectID );
	}

	DEBUG_ADD("del ok");
}

//----------------------------------------------------------------------
// Set Result
//----------------------------------------------------------------------
void			
MEffectTarget::SetResult(MActionResult* pResult)
{ 
	if (m_pResult != NULL)
	{
		delete m_pResult; 
	}

	m_pResult = pResult;
}
		
//----------------------------------------------------------------------
// assign
//----------------------------------------------------------------------
void	
MEffectTarget::operator = (const MEffectTarget& target)
{
	m_MaxPhase		= target.m_MaxPhase;
	m_CurrentPhase	= target.m_CurrentPhase;
	m_DelayFrame	= target.m_DelayFrame;
	m_X				= target.m_X;
	m_Y				= target.m_Y;
	m_Z				= target.m_Z;
	m_ID			= target.m_ID;
	m_ServerID		= target.m_ServerID;
}



//----------------------------------------------------------------------
//
//					MPortalEffectTarget
//
//----------------------------------------------------------------------
MPortalEffectTarget::MPortalEffectTarget(const MEffectTarget& target)			
: MEffectTarget(target)
{
	m_ZoneID = OBJECTID_NULL;
	m_ZoneX = SECTORPOSITION_NULL;
	m_ZoneY = SECTORPOSITION_NULL;
}	

MPortalEffectTarget::MPortalEffectTarget(BYTE max)
: MEffectTarget(max)
{
	m_ZoneID = OBJECTID_NULL;
	m_ZoneX = SECTORPOSITION_NULL;
	m_ZoneY = SECTORPOSITION_NULL;
}

MPortalEffectTarget::~MPortalEffectTarget()
{
}

//------------------------------------------------------------------
// assign
//------------------------------------------------------------------
void	
MPortalEffectTarget::operator = (const MEffectTarget& target)
{
	(MEffectTarget)*this = target;

	if (target.GetEffectTargetType()==EFFECT_TARGET_PORTAL)
	{
		const MPortalEffectTarget& portalTarget = (const MPortalEffectTarget&)target;

		m_OwnerName = portalTarget.m_OwnerName;
		m_ZoneID = portalTarget.m_ZoneID;
		m_ZoneX = portalTarget.m_ZoneX;
		m_ZoneY = portalTarget.m_ZoneY;
	}
}

//------------------------------------------------------------------
// Get ZoneName
//------------------------------------------------------------------
const char*				
MPortalEffectTarget::GetZoneName() const
{
	if (g_pZoneTable!=NULL)
	{
		ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( m_ZoneID );

		if (pZoneInfo!=NULL)
		{
			return pZoneInfo->Name.GetString();
		}
	}
	
	return NULL;
}