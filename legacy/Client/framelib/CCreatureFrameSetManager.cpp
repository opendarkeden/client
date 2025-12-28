//----------------------------------------------------------------------
// CCreatureFrameSetManager.cpp
//----------------------------------------------------------------------
#include "Frame_PCH.h"
#include "CFrame.h"
#include "CCreatureFrameSetManager.h"
#include "CFramePack.h"
#include "CSpriteSetManager.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CCreatureFrameSetManager::CCreatureFrameSetManager()
{
}

CCreatureFrameSetManager::~CCreatureFrameSetManager()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Save SpriteSet Index
//----------------------------------------------------------------------
// CreatureFrameSet과 관련된 SpriteID를 선택해서
// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
//----------------------------------------------------------------------
bool		
CCreatureFrameSetManager::SaveSpriteSetIndex(CCreatureFramePack* pCreatureFramePack, 
											 ofstream& setIndex, ifstream& packIndex)
{
	CSpriteSetManager ssm;

	
	ACTION_FRAME_ARRAY*		pActionArray;
	DIRECTION_FRAME_ARRAY*	pDirectionArray ;
	FRAME_ARRAY*			pFrameArray;

	DATA_LIST::iterator iData = m_List.begin();

	//------------------------------------------------------------------
	// 선택된 FrameID에 관련된 모든 SpriteID를 알아내야 한다.	
	//------------------------------------------------------------------
	while (iData != m_List.end())
	{
		// FRAME_ARRAY를 읽어온다.
		pActionArray = &((*pCreatureFramePack)[*iData]);

		//--------------------------------------------------------------
		// 각각의 ACTION_FRAME_ARRAY에 속한 ..
		// DIRECTION_FRAME_ARRAY에 속한..
		// FRAME_ARRAY에 속한 Frame의 SpriteID를 
		// CSpriteSetManager에 저장한다. - -;;
		//--------------------------------------------------------------
		for (int action=0; action<pActionArray->GetSize(); action++)
		{
			pDirectionArray = &((*pActionArray)[action]);
			for (int direction=0; direction<pDirectionArray->GetSize(); direction++)
			{
				pFrameArray = &((*pDirectionArray)[direction]);
				for (int frame=0; frame<pFrameArray->GetSize(); frame++)
				{
					ssm.Add( (*pActionArray)[action][direction][frame].GetSpriteID() );
				}
			}
		}

		iData++;
	}

	//------------------------------------------------------------------
	// ThingFrameSet과 관련된 모든 SpriteID를 
	// SpriteSetManager에 저장했으므로 
	// SpriteSetManager를 이용해 SpriteSetIndex를 생성하면된다.
	//------------------------------------------------------------------
	return ssm.SaveSpriteSetIndex(setIndex, packIndex);
}

