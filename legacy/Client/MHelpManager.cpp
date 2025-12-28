//---------------------------------------------------------------------------
// MHelpManager.cpp
//---------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MHelpManager.h"

#ifdef __GAME_CLIENT__
	#include "ClientDef.h"
	#include "MCompareManager.h"
	#include "MHelpDisplayer.h"
#include "VS_UI.h"
#endif


//---------------------------------------------------------------------------
// Global
//---------------------------------------------------------------------------
MHelpManager*		g_pHelpManager = NULL;

//-----------------------------------------------------------------------------
// Execute Help Event
//-----------------------------------------------------------------------------
void		
ExecuteHelpEvent(HELP_EVENT he)
{
	#if defined(__GAME_CLIENT__)// && defined(OUTPUT_DEBUG) && defined(_DEBUG)
		if (g_Mode==MODE_GAME)
		{
			gC_vs_ui.AddHelpMail(he);//, g_pPlayer->IsInSafeSector());
		//	g_pHelpManager->ExecuteEvent( he );
		}
	#endif
}

//---------------------------------------------------------------------------
// ItemClass에 맞게 메모리를 잡아주는 class table을 설정..
//---------------------------------------------------------------------------
MHelpNode::FUNCTION_NEWNODE
MHelpNode::s_NewNodeClassTable[MAX_NODE_TYPE] =
{
	NULL,
	MCompareHelpNode::NewNode,
	MOutputHelpNode::NewNode
};

//---------------------------------------------------------------------------
//
//							MHelpNode
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// New Node
//---------------------------------------------------------------------------
// nodeType에 맞는 class의 객체를 생성해서(new) 넘겨준다.
//---------------------------------------------------------------------------
MHelpNode*		
MHelpNode::NewNode(MHelpNode::NODE_TYPE nodeType)
{
	if (s_NewNodeClassTable[nodeType]==NULL)
	{
		return NULL;
	}

	return (MHelpNode*)(*s_NewNodeClassTable[nodeType])();
};


//---------------------------------------------------------------------------
//
//							MCompareHelpNode
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Get Next
//---------------------------------------------------------------------------
MHelpNode*
MCompareHelpNode::GetNext() const
{
	// 현재의 조건을 비교해서
	// m_pTrue나 m_pFalse를 넘겨준다.

#ifdef __GAME_CLIENT__
	if (g_pCompareManager!=NULL 
		&& g_pCompareManager->Compare( m_pCompareType ))
	{
		return m_pTrue;
	}
#endif	
	
	return m_pFalse;	
}

//---------------------------------------------------------------------------
// Set True
//---------------------------------------------------------------------------
void				
MCompareHelpNode::SetTrue(MHelpNode* pNode)
{
	if (m_pTrue!=NULL)
	{
		delete m_pTrue;
	}

	m_pTrue = pNode;
}

//---------------------------------------------------------------------------
// Set False
//---------------------------------------------------------------------------
void				
MCompareHelpNode::SetFalse(MHelpNode* pNode)
{
	if (m_pFalse!=NULL)
	{
		delete m_pFalse;
	}

	m_pFalse = pNode;
}

//---------------------------------------------------------------------------
// Save To File
//---------------------------------------------------------------------------
void		
MCompareHelpNode::SaveToFile(class ofstream& file)
{
	//---------------------------------------------------
	// 검사해볼려는 조건
	//---------------------------------------------------
	WORD compareType = m_pCompareType;			
	file.write((const char*)&compareType, 2);
	
	BYTE classType;

	//---------------------------------------------------
	// 조건을 만족하는 경우
	//---------------------------------------------------
	if (m_pTrue==NULL)
	{
		classType = 0;
		file.write((const char*)&classType, 1);
	}
	else
	{
		classType = m_pTrue->GetType();
		file.write((const char*)&classType, 1);

		m_pTrue->SaveToFile( file );
	}
	
	//---------------------------------------------------
	// 조건을 만족하지 못하는 경우
	//---------------------------------------------------
	if (m_pFalse==NULL)
	{
		classType = 0;
		file.write((const char*)&classType, 1);
	}
	else
	{
		classType = m_pFalse->GetType();
		file.write((const char*)&classType, 1);

		m_pFalse->SaveToFile( file );
	}
}

//---------------------------------------------------------------------------
// Load From File
//---------------------------------------------------------------------------
void		
MCompareHelpNode::LoadFromFile(class ifstream& file)
{
	//---------------------------------------------------
	// 검사해볼려는 조건
	//---------------------------------------------------
	WORD compareType;
	file.read((char*)&compareType, 2);
	m_pCompareType = (HELP_COMPARE)compareType;

	BYTE classType;

	//---------------------------------------------------
	// 기존에 있던거 제거
	//---------------------------------------------------
	if (m_pTrue!=NULL)	{ delete m_pTrue; }
	if (m_pFalse!=NULL)	{ delete m_pFalse; }

	//---------------------------------------------------
	// 조건을 만족하는 경우
	//---------------------------------------------------
	file.read((char*)&classType, 1);

	m_pTrue = MHelpNode::NewNode( (MHelpNode::NODE_TYPE)classType );

	if (m_pTrue!=NULL)
	{
		m_pTrue->LoadFromFile( file );
	}
	
	//---------------------------------------------------
	// 조건을 만족하지 못하는 경우
	//---------------------------------------------------
	file.read((char*)&classType, 1);

	m_pFalse = MHelpNode::NewNode( (MHelpNode::NODE_TYPE)classType );

	if (m_pFalse!=NULL)
	{
		m_pFalse->LoadFromFile( file );
	}
}

//---------------------------------------------------------------------------
//
//							MOutputHelpNode
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Get OutputType
//---------------------------------------------------------------------------
HELP_OUTPUT			
MOutputHelpNode::GetOutputType() const
{
	int numOutputID = size();

	if (numOutputID == 0)
	{
		return HELP_OUTPUT_NULL;
	}

	int select = rand()%numOutputID;
	
	const_iterator	iID = begin();

	// select번째 id를 선택한다.
	for (int i=0; i<select; i++)
	{
		iID++;
	}

	return *iID;
}

//---------------------------------------------------------------------------
// Save To File
//---------------------------------------------------------------------------
void		
MOutputHelpNode::SaveToFile(class ofstream& file)
{
	//-----------------------------------------------
	// ID 개수 저장
	//-----------------------------------------------
	int numID = size();
	file.write((const char*)&numID, 4);

	//-----------------------------------------------
	// 모든 OutputHelp ID저장
	//-----------------------------------------------
	iterator iID = begin();
	while (iID != end())
	{
		WORD id = *iID;

		file.write((const char*)&id, 2);

		iID++;
	}
}

//---------------------------------------------------------------------------
// Load From File
//---------------------------------------------------------------------------
void		
MOutputHelpNode::LoadFromFile(class ifstream& file)
{
	//-----------------------------------------------
	// ID 개수 
	//-----------------------------------------------
	int numID;
	file.read((char*)&numID, 4);

	//-----------------------------------------------
	// 모든 OutputHelp ID로딩
	//-----------------------------------------------
	WORD id;

	clear();

	for (int i=0; i<numID; i++)
	{		
		file.read((char*)&id, 2);

		push_back( (HELP_OUTPUT)id );
	}
}

//---------------------------------------------------------------------------
//
//							MHelpManager
//
//---------------------------------------------------------------------------
MHelpManager::MHelpManager()
{
}

MHelpManager::~MHelpManager()
{
	Release();
}

//---------------------------------------------------------------------------
// Init
//---------------------------------------------------------------------------
void		
MHelpManager::Init(int size)
{
	//-------------------------------------------------------
	// base class's Init
	//-------------------------------------------------------
	CTypeTable<MHelpNode*>::Init( size );

	//-------------------------------------------------------
	// Event 발생 기록
	//-------------------------------------------------------
	m_EventOccured.Init( size );

	//-------------------------------------------------------
	// 초기화
	//-------------------------------------------------------
	for (int i=0; i<m_Size; i++)
	{
		m_pTypeInfo[i] = NULL;

		m_EventOccured[i] = false;
	}
	
}

//---------------------------------------------------------------------------
// Release
//---------------------------------------------------------------------------
void		
MHelpManager::Release()
{
	for (int i=0; i<m_Size; i++)
	{
		if (m_pTypeInfo[i] != NULL)
		{
			delete m_pTypeInfo[i];
			m_pTypeInfo[i] = NULL;
		}
	}

	// base class's Release
	CTypeTable<MHelpNode*>::Release();

	m_EventOccured.Release();
}

//---------------------------------------------------------------------------
// Clear EventOccured
//---------------------------------------------------------------------------
void		
MHelpManager::ClearEventOccured()
{
	for (int i=0; i<m_EventOccured.GetSize(); i++)
	{
		m_EventOccured[i] = false;
	}
}

//---------------------------------------------------------------------------
// Save To File
//---------------------------------------------------------------------------
void		
MHelpManager::SaveToFile(class ofstream& file)
{
	//---------------------------------------------------------
	// size 저장
	//---------------------------------------------------------
	file.write((const char*)&m_Size, 4);

	// 아무 것도 없는 경우
	if (m_pTypeInfo==NULL)
		return;

	//---------------------------------------------------------
	// 각각의 정보 저장
	//---------------------------------------------------------
	BYTE classType;

	for (int i=0; i<m_Size; i++)
	{
		MHelpNode* pNode = m_pTypeInfo[i];

		if (pNode==NULL)
		{
			classType = 0;
			file.write((const char*)&classType, 1);
		}
		else
		{
			classType = pNode->GetType();
			file.write((const char*)&classType, 1);

			pNode->SaveToFile( file );
		}		
	}
}

//---------------------------------------------------------------------------
// Load From File
//---------------------------------------------------------------------------
void		
MHelpManager::LoadFromFile(class ifstream& file)
{
	int size;

	//---------------------------------------------------------
	// size 읽어오기
	//---------------------------------------------------------
	file.read((char*)&size, 4);

	// 현재 잡혀있는 메모리와 다르면 다시 메모리를 잡는다.
	if (m_Size != size)
	{
		// 메모리 해제
		Release();

		// 메모리 잡기
		Init( size );
	}

	//---------------------------------------------------------
	// file에서 각각의 정보를 읽어들인다.
	//---------------------------------------------------------
	BYTE classType;
	for (int i=0; i<m_Size; i++)
	{
		file.read((char*)&classType, 1);

		MHelpNode* pNode = MHelpNode::NewNode( (MHelpNode::NODE_TYPE)classType );

		if (pNode!=NULL)
		{
 			pNode->LoadFromFile( file );

			m_pTypeInfo[i] = pNode;
		}
	}
}


//---------------------------------------------------------------------------
// Execute Event ( HELP_EVENT )
//---------------------------------------------------------------------------
void
MHelpManager::ExecuteEvent(HELP_EVENT he)
{
	if (m_pTypeInfo[he]!=NULL)
	{
		MHelpNode* pHelpNode = m_pTypeInfo[he];

		while (pHelpNode!=NULL)
		{
			switch (pHelpNode->GetType())
			{
				//------------------------------------------------------------
				// 조건 비교 node
				//------------------------------------------------------------
				case MHelpNode::TYPE_COMPARE :
				{
					// 조건비교해서.. 다음 node를 얻는다.
					pHelpNode = ((MCompareHelpNode*)pHelpNode)->GetNext();			
				}
				break;

				//------------------------------------------------------------
				// 도움말 출력 node
				//------------------------------------------------------------
				case MHelpNode::TYPE_OUTPUT :
				{
					HELP_OUTPUT ho = ((MOutputHelpNode*)pHelpNode)->GetOutputType();

					#ifdef __GAME_CLIENT__
						g_pHelpDisplayer->OutputHelp( ho );					
					#endif

					pHelpNode = NULL;	// while loop를 빠지기 위해서.
				}
				break;
			}
		}
	}
}

