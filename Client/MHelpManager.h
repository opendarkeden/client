//---------------------------------------------------------------------------
// MHelpManager.h
//---------------------------------------------------------------------------
/*

	//---------------------------------------------------------------------------
	// 하나의 HelpEvent 생성 예제
	//---------------------------------------------------------------------------
	pHelpNode = new MCompareHelpNode( HC_PICKED_UP_ITEM );
		// 아이템 주운적이 있다면
		pHelpNode->SetTrue( new MCompareHelpNode( HC_USE_ALT_FOR_ITEM ) );
			// ALT를 사용한 적이 없는 경우
			pHelpNode->GetTrue()->SetFalse( new MOutputHelpNode( HO_ALT_TO_PICKUP ) );

		// 아이템 주운적이 없다면
		pHelpNode->SetFalse( new MCompareHelpNode( HC_USE_ALT_FOR_ITEM ) );
			// ALT를 사용한 적이 있다면
			pHelpNode->GetFalse()->SetTrue( new MOutputHelpNode( HO_CLICK_TO_PICKUP ) );
			// ALT를 사용한적이 없다면
			pHelpNode->GetFalse()->SetFalse( new MOutputHelpNode( HO_ALT_TO_PICKUP ) );

	g_pHelpManager[HE_APPEAR_ITEM] = pHelpNode;


	//---------------------------------------------------------------------------
	// event가 생겼을 때, event실행..
	//---------------------------------------------------------------------------
	g_pHelpManager->ProcessEvent( HE_APPEAR_ITEM );

*/
//---------------------------------------------------------------------------

#ifndef __MHELPMANAGER_H__
#define	__MHELPMANAGER_H__

#pragma warning(disable:4786)

#include "CTypeTable.h"
#include "MHelpDef.h"
#include "Client_PCH.h"
#ifdef __GAME_CLIENT__
	#include "UserOption.h"
#endif


//---------------------------------------------------------------------------
//
//							MHelpNode
//
//---------------------------------------------------------------------------
class MHelpNode {
	public :
		enum NODE_TYPE
		{
			TYPE_NULL = 0,
			TYPE_COMPARE,
			TYPE_OUTPUT,

			MAX_NODE_TYPE
		};

	public :
		MHelpNode() {}
		virtual ~MHelpNode() {}
	
		virtual NODE_TYPE	GetType() const	= 0;

		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		virtual void		SaveToFile(std::ofstream& file) = 0;
		virtual void		LoadFromFile(ifstream& file) = 0;

	public :
		// NewItem()에 대한 functions pointer
		typedef MHelpNode*	(*FUNCTION_NEWNODE)();

		//-------------------------------------------------------
		// 원하는 class의 Item의 객체를 생성한다.
		//-------------------------------------------------------
		static MHelpNode*	NewNode(MHelpNode::NODE_TYPE nodeType);

		// 각 class별로 NewItem()에 대한 function pointer
		static FUNCTION_NEWNODE		s_NewNodeClassTable[MAX_NODE_TYPE];
};

//---------------------------------------------------------------------------
//
//							MCompareHelpNode
//
//---------------------------------------------------------------------------
// CompareType에 따라서 True / False로 나뉜다.
//---------------------------------------------------------------------------
class MCompareHelpNode : public MHelpNode {
	public :
		MCompareHelpNode(HELP_COMPARE ct=HC_RANDOM)
		{
			m_pCompareType = ct;
			m_pTrue = m_pFalse = NULL;
		}	

		virtual ~MCompareHelpNode()
		{
			if (m_pTrue!=NULL)	{ delete m_pTrue; }
			if (m_pFalse!=NULL)	{ delete m_pFalse; }
		}

		//-----------------------------------------------------------
		// Get Type
		//-----------------------------------------------------------
		virtual MHelpNode::NODE_TYPE	GetType() const	{ return MHelpNode::TYPE_COMPARE; }

		//-----------------------------------------------------------
		// Get Next
		//-----------------------------------------------------------
		MHelpNode*			GetNext() const;

		//-----------------------------------------------------------
		// Get/Set Compare Node
		//-----------------------------------------------------------
		HELP_COMPARE		GetCompareType() const		{ return m_pCompareType; }
		void				SetTrue(MHelpNode* pNode);
		void				SetFalse(MHelpNode* pNode);
		MHelpNode*			GetTrue() const				{ return m_pTrue; }
		MHelpNode*			GetFalse() const			{ return m_pFalse; }

		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		virtual void		SaveToFile(std::ofstream& file);
		virtual void		LoadFromFile(ifstream& file);	

	public :
		static MHelpNode*	NewNode()					{ return new MCompareHelpNode; }

	protected :
		HELP_COMPARE		m_pCompareType;			// 검사해볼려는 조건
		MHelpNode*			m_pTrue;				// 조건을 만족하는 경우
		MHelpNode*			m_pFalse;				// 조건을 만족하지 못하는 경우
};

//---------------------------------------------------------------------------
//
//							MOutputHelpNode
//
//---------------------------------------------------------------------------
// 출력할 도움말을 나타낸다.
//---------------------------------------------------------------------------
class MOutputHelpNode : public MHelpNode, public std::list<HELP_OUTPUT> {
	public :
		typedef std::list<HELP_OUTPUT>		OUTPUT_LIST;

	public :
		MOutputHelpNode() {}
		virtual ~MOutputHelpNode() {}

		//-----------------------------------------------------------
		// Get Type
		//-----------------------------------------------------------
		virtual MHelpNode::NODE_TYPE	GetType() const		{ return MHelpNode::TYPE_OUTPUT; }

		//-----------------------------------------------------------
		// Get 
		//-----------------------------------------------------------
		HELP_OUTPUT			GetOutputType() const;
		
		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		virtual void		SaveToFile(std::ofstream& file);
		virtual void		LoadFromFile(ifstream& file);	

	public :
		static MHelpNode*	NewNode()					{ return new MOutputHelpNode; }
};


//---------------------------------------------------------------------------
//
//							MHelpManager
//
//---------------------------------------------------------------------------
class MHelpManager : public CTypeTable<MHelpNode*> {
	public :
		MHelpManager();
		~MHelpManager();

		//------------------------------------------------------
		// Init / Release
		//------------------------------------------------------
		void		Init(int size);		
		void		Release();
		
		//------------------------------------------------------
		// Event Occured
		//------------------------------------------------------
		void		ClearEventOccured();
		bool		IsEventOccured(HELP_EVENT he)		{ return m_EventOccured[he]; }

		//------------------------------------------------------
		// Execute Event
		//------------------------------------------------------
		void		ExecuteEvent(HELP_EVENT he);

		//------------------------------------------------------
		// File I/O
		//------------------------------------------------------
		void		SaveToFile(std::ofstream& file);
		void		LoadFromFile(ifstream& file);	

	protected :
		CTypeTable<bool>		m_EventOccured;
};

//---------------------------------------------------------------------------
// Global
//---------------------------------------------------------------------------
extern MHelpManager*		g_pHelpManager;

//-----------------------------------------------------------------------------
// Execute Help Event
//-----------------------------------------------------------------------------
extern void		ExecuteHelpEvent(HELP_EVENT he);

#if defined(__GAME_CLIENT__) && defined(OUTPUT_DEBUG)
	#define __BEGIN_HELP_EVENT 		if (g_pUserOption!=NULL && g_pUserOption->UseHelpEvent) {
	#define __END_HELP_EVENT		}
	#define __USE_HELP_EVENT
#else
	#define __BEGIN_HELP_EVENT 		((void)0);
	#define __END_HELP_EVENT 		((void)0);
#endif

#endif

