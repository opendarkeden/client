//----------------------------------------------------------------------
// MEffectTarget.h
//----------------------------------------------------------------------
//
//  = 각각의 Effect마다 필요한 정보들
//
//  = 먼저 설정된 목표에 먼저 도달해야 한다. (queue로 구현)
//
//  = EffectTargetNode
//    - 목표(pX,pY,pZ)
//    - CreatureID
//
//----------------------------------------------------------------------

#ifndef	__MEFFECTTARGET_H__
#define	__MEFFECTTARGET_H__

#pragma warning(disable:4786)


#include "MTypeDef.h"
#include "MActionResult.h"
#include "MString.h"

//----------------------------------------------------------------------
// EFFECT_TARGET_NODE의 list (queue로 하면 좋겠지만.. 문제가.. - -;) 
//----------------------------------------------------------------------
class MEffectTarget {
	public :
		enum EFFECT_TARGET_TYPE
		{
			EFFECT_TARGET_NORMAL,
			EFFECT_TARGET_PORTAL
		};

	public :
		//--------------------------------
		// assign
		//--------------------------------		
		MEffectTarget(const MEffectTarget& target);
		MEffectTarget(BYTE max);
		virtual ~MEffectTarget();

		virtual EFFECT_TARGET_TYPE	GetEffectTargetType() const	{ return EFFECT_TARGET_NORMAL; }
	
		//-------------------------------------------------------
		// Instance ID
		//-------------------------------------------------------
		void		NewEffectID()			{ m_EffectID = s_EffectID++; }
		BYTE		GetEffectID() const		{ return m_EffectID; }

		//-------------------------------------------------------
		// Set
		//-------------------------------------------------------
		void		Set(int x, int y, int z, TYPE_OBJECTID id)
		{			
			m_X				= x;
			m_Y				= y;
			m_Z				= z;
			m_ID			= id;
		}

		void		SetServerID(TYPE_OBJECTID id)	{ m_ServerID = id; }

		void		SetDelayFrame(DWORD df)		{ m_DelayFrame = df; }
	
		//-------------------------------------------------------
		// assign operator
		//-------------------------------------------------------
		virtual void	operator = (const MEffectTarget& target);		


		//-------------------------------------------------------
		// 진행중인 단계
		//-------------------------------------------------------
		void			NextPhase()				{ m_CurrentPhase++; }
		bool			IsEnd() const			{ return m_CurrentPhase>=m_MaxPhase; }
		
		// 결과를 출력해줘야 하는 시간이 지났다는 의미..
		bool			IsResultTime()	const	{ return m_bResultTime; }
		void			SetResultTime()			{ m_bResultTime = true; }

		//-------------------------------------------------------
		// Get
		//-------------------------------------------------------		
		BYTE			GetMaxPhase() const		{ return m_MaxPhase; }
		BYTE			GetCurrentPhase() const	{ return m_CurrentPhase; }
		DWORD			GetDelayFrame() const	{ return m_DelayFrame; }	
		int				GetX() const			{ return m_X; }
		int				GetY() const			{ return m_Y; }
		int				GetZ() const			{ return m_Z; }
		TYPE_OBJECTID	GetID() const			{ return m_ID; }
		TYPE_OBJECTID	GetServerID() const		{ return m_ServerID; }

		//-------------------------------------------------------		
		// 연결되는 결과에 대한 정보
		//-------------------------------------------------------		
		bool			IsResultEmpty() const					{ return m_pResult==NULL; }
		bool			IsExistResult() const					{ return m_pResult!=NULL; }
		MActionResult*	GetResult() const						{ return m_pResult; }
		void			SetResult(MActionResult* pResult);
		void			SetResultNULL()							{ m_pResult = NULL; }
		//TYPE_ACTIONINFO	GetResultActionInfo() const					{ return m_nResultActionInfo; }
		//void			SetResultActionInfo(TYPE_ACTIONINFO rai)	{ m_nResultActionInfo = rai; }
		
	public :
		BYTE			m_MaxPhase;			// 전체 단계수
		BYTE			m_CurrentPhase;		// 현재 단계

		bool			m_bResultTime;		// 결과 시간이 지났는가?

		DWORD			m_DelayFrame;		// 지속 시간

		//-------------------------------------------------------		
		// 목표에 대한 정보
		//-------------------------------------------------------		
		int				m_X;				
		int				m_Y;
		int				m_Z;
		TYPE_OBJECTID	m_ID;
		TYPE_OBJECTID	m_ServerID;

		//-------------------------------------------------------		
		// 결과에 대한 정보
		//-------------------------------------------------------		
		//TYPE_ACTIONINFO	m_nResultActionInfo;	// 결과를 표현하는 ActionInfoTableID
		MActionResult*	m_pResult;				// 결과들


		//-------------------------------------------------------		
		// Instance ID
		//-------------------------------------------------------		
		BYTE			m_EffectID;
		static BYTE		s_EffectID;
};

//----------------------------------------------------------------------
// MPortalEffectTarget
//----------------------------------------------------------------------
class MPortalEffectTarget : public MEffectTarget {
	public :
		MPortalEffectTarget(const MEffectTarget& target);
		MPortalEffectTarget(BYTE max);
		~MPortalEffectTarget();

		virtual EFFECT_TARGET_TYPE	GetEffectTargetType() const	{ return EFFECT_TARGET_PORTAL; }

		//------------------------------------------------------------------
		// Set
		//------------------------------------------------------------------
		void					SetOwnerName(const char* name)	{ m_OwnerName = name; }
		void					SetPortal(int id, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
		{
			m_ZoneID = id;
			m_ZoneX = x;
			m_ZoneY = y;
		}

		//------------------------------------------------------------------
		// Get
		//------------------------------------------------------------------
		const char*				GetOwnerName() const	{ return m_OwnerName.GetString(); }
		int						GetZoneID() const		{ return m_ZoneID; }
		TYPE_SECTORPOSITION		GetZoneX() const		{ return m_ZoneX; }
		TYPE_SECTORPOSITION		GetZoneY() const		{ return m_ZoneY; }
		const char*				GetZoneName() const;

		//------------------------------------------------------------------
		// assign
		//------------------------------------------------------------------
		virtual void	operator = (const MEffectTarget& target);		

	protected :
		MString					m_OwnerName;
		int						m_ZoneID;
		TYPE_SECTORPOSITION		m_ZoneX;
		TYPE_SECTORPOSITION		m_ZoneY;
};

typedef	std::list<MEffectTarget*>		EFFECTTARGET_LIST;

#endif

