//----------------------------------------------------------------------
// CFramePack.h
//----------------------------------------------------------------------
// 아래의 FramePack들을 정의해서 사용한다.
//
// typedef CFramePack<FRAME_ARRAY>				CThingFramePack;
// typedef	CFramePack<DIRECTION_FRAME_ARRAY>	CEffectFramePack;
// typedef	CFramePack<ACTION_FRAME_ARRAY>		CCreatureFramePack;
//
//
// - ThingFramePack    : 방향성이 없는 Thing을 표현
// - EffectFramePack   : 방향성이 있는 Effect를 표현
// - CreatureFramePack : Action과 방향성이 있는 Creature를 표현
//
//----------------------------------------------------------------------

#ifndef	__CFRAMEPACK_H__
#define	__CFRAMEPACK_H__


#include "DrawTypeDef.h"
#include "CFrame.h"

template <class Type>
class CFramePack : public TArray<Type, TYPE_FRAMEID> {
	public :
		CFramePack();
		~CFramePack();

		
		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		bool		SaveToFile(class ofstream& packFile, class ofstream& indexFile);		

	protected :

};


//----------------------------------------------------------------------
// CFramePack<Type>.cpp
//----------------------------------------------------------------------

//#include "CFramePack.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
CFramePack<Type>::CFramePack<Type>()
{
}

template <class Type>
CFramePack<Type>::~CFramePack<Type>()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// CreatureFramePack File을 저장하고
// CreatureFramePack IndexFile도 저장해야 한다.
//----------------------------------------------------------------------
template <class Type>
bool
CFramePack<Type>::SaveToFile(class ofstream& packFile, class ofstream& indexFile)
{
	//--------------------------------------------------
	// Size저장 : 0이라도 개수는 저장한다.
	//--------------------------------------------------
	packFile.write((const char*)&m_Size, s_SIZEOF_SizeType);
	indexFile.write((const char *)&m_Size, s_SIZEOF_SizeType); 

	// 아무것도 없으면..
	if (m_pData==NULL || m_Size==0) 
		return false;

	//--------------------------------------------------
	// index file을 생성하기 위한 정보
	//--------------------------------------------------
	long*	pIndex = new long [m_Size];

	//--------------------------------------------------
	//
	// CreatureFramePack 모두 저장
	//
	//--------------------------------------------------
	for (TYPE_FRAMEID i=0; i<m_Size; i++)
	{
		// CreatureFrame이 file에 쓰여지는 index를 저장
		pIndex[i] = packFile.tellp();

		// CreatureFrame저장
		m_pData[i].SaveToFile(packFile);
	}

	//--------------------------------------------------
	// index 저장
	//--------------------------------------------------
	for (i=0; i<m_Size; i++)
	{
		indexFile.write((const char*)&pIndex[i], 4);
	}

	delete [] pIndex;

	return true;
}

//----------------------------------------------------------------------
// FramePack을 define한다.
//----------------------------------------------------------------------
typedef CFramePack<CFrame>							CImageFramePack;
typedef CFramePack<FRAME_ARRAY>						CAnimationFramePack;
typedef	CFramePack<DIRECTION_FRAME_ARRAY>			CDirectionFramePack;
typedef	CFramePack<DIRECTION_EFFECTFRAME_ARRAY>		CEffectFramePack;
//typedef	CFramePack<ACTION_FRAME_ARRAY>				CCreatureFramePack;

/*
class CEffectFramePack : public CFramePack<DIRECTION_EFFECTFRAME_ARRAY> {
	public :
		void		InfoToFile(const char* filename);
};
*/

class CCreatureFramePack : public CFramePack<ACTION_FRAME_ARRAY> {
	public :
		void		InfoToFile(const char* filename);
};

#endif

