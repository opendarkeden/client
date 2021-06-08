//----------------------------------------------------------------------
// CAnimationFrameSetManager.cpp
//----------------------------------------------------------------------
#include "Frame_PCH.h"
#include "CFrame.h"
#include "CAnimationFrameSetManager.h"
#include "CFramePack.h"
#include "CSpriteSetManager.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CAnimationFrameSetManager::CAnimationFrameSetManager()
{
}

CAnimationFrameSetManager::~CAnimationFrameSetManager()
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
// AnimationFrameSet과 관련된 SpriteID를 선택해서
// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
//----------------------------------------------------------------------
bool		
CAnimationFrameSetManager::SaveSpriteSetIndex(CAnimationFramePack* pAnimationFramePack, 
										  ofstream& setIndex, ifstream& packIndex)
{
	CSpriteSetManager ssm;

	
	FRAME_ARRAY*	pFrameArray;

	DATA_LIST::iterator iData = m_List.begin();

	//------------------------------------------------------------------
	// 선택된 FrameID에 관련된 모든 SpriteID를 알아내야 한다.	
	//------------------------------------------------------------------
	while (iData != m_List.end())
	{
		// FRAME_ARRAY를 읽어온다.
		pFrameArray = &((*pAnimationFramePack)[*iData]);

		//--------------------------------------------------------------
		// 각각의 FRAME_ARRAY에 속한 Frame의 SpriteID를 
		// CSpriteSetManager에 저장한다.
		//--------------------------------------------------------------
		for (int i=0; i<pFrameArray->GetSize(); i++)
		{
			ssm.Add( (*pFrameArray)[i].GetSpriteID() );
		}

		iData++;
	}

	//------------------------------------------------------------------
	// AnimationFrameSet과 관련된 모든 SpriteID를 
	// SpriteSetManager에 저장했으므로 
	// SpriteSetManager를 이용해 SpriteSetIndex를 생성하면된다.
	//------------------------------------------------------------------
	return ssm.SaveSpriteSetIndex(setIndex, packIndex);
}

