//----------------------------------------------------------------------
// MLoadingSPKWorkNode.h
//----------------------------------------------------------------------
// class 이름 붙이기가 골치가 아파서 숫자 붙였음. - -;;
//
// 암튼.. SPK를 Loading하는데.. 그 방법에 따라서 몇개의 Node로 나뉨
//
// [Remain]의 의미는 Node가 실행중에 중단되었을 경우
// 남은 작업을 다시 하나의 Node로 만들어야 되는데..
// 그 때 생성할 Node의 종류이다.
/*

  [ 공통 ]
  		CSpritePack*	SPK
		char			SPKFilename


  [Node 1]
		= FilePosition에서 SFPArray를 읽어서 SPK에 load한다.
		
		char			SFPFilename
		long			FilePosition

		[Remain] --> Node 2


  [Node 2]
		= SFPArray를 이용해서 SPK에다 load한다.

		SFPArray*		SFPA

		[Remain] --> Node 2
		

  [Node 3]
		= FilePosition에서 하나의 Sprite를 읽어서 SPK의 ID번째에 저장한다.

		TYPE_SPRITEID	SpriteID
		long			FilePosition

		[Remain] --> NULL

  [Node 4]
		= FilePosition에서 SPK의 FirstID ~ LastID까지를 읽어들인다.

		TYPE_SPRITEID	FirstSpriteID
		TYPE_SPRITEID	LastSpriteID
		long			FilePosition

		[Remain] --> Node 4


*/
//----------------------------------------------------------------------

#ifndef __MLOADINSPKWORKNODE_H__
#define __MLOADINSPKWORKNODE_H__

#include "MWorkNode.h"
#include "CSpriteDef.h"
//#include "CSpritePack.h"
#include "CSpritePack.h"


#define	MAX_FILENAME		256

//----------------------------------------------------------------------
// MLoadingSPKWorkNode
//----------------------------------------------------------------------
// SPKFile에서 어떻게~해서 SPK를 loading한다.
//----------------------------------------------------------------------
class MLoadingSPKWorkNode : public MWorkNode {
	public :
		MLoadingSPKWorkNode()	{ m_pSPK = NULL; m_bExecute = TRUE; }
		~MLoadingSPKWorkNode() {}

		//---------------------------------------------------
		// pSPK에 SPKFilename의 뭔가를(?) Loading하는 것이다.
		//---------------------------------------------------
		void			SetSPK(CSpritePack* pSPK, const char* filename)
		{
			m_pSPK = pSPK;
			strcpy(m_SPKFilename, filename);
		}

	protected :
		CSpritePack*			m_pSPK;
		char					m_SPKFilename[MAX_FILENAME];
};

//----------------------------------------------------------------------
// Node 1
//----------------------------------------------------------------------
class MLoadingSPKWorkNode1 : public MLoadingSPKWorkNode {
	public :
		MLoadingSPKWorkNode1(const char* sfpaFilename, long filePosition) { strcpy(m_SFPAFilename, sfpaFilename); m_FilePosition = filePosition; }
		~MLoadingSPKWorkNode1() {}

		BOOL		Execute(MWorkNode*& pRemainNode);

	protected :
		char							m_SFPAFilename[MAX_FILENAME];
		long							m_FilePosition;
};

//----------------------------------------------------------------------
// Node 2
//----------------------------------------------------------------------
// m_pSFPA는 내부에서 delete한다.
//----------------------------------------------------------------------
class MLoadingSPKWorkNode2 : public MLoadingSPKWorkNode {
	public :
		MLoadingSPKWorkNode2(CSpriteFilePositionArray* pSFPA) { m_pSFPA = pSFPA; }
		~MLoadingSPKWorkNode2() { if (m_pSFPA!=NULL) delete m_pSFPA; }

		BOOL		Execute(MWorkNode*& pRemainNode);
		
	protected :
		CSpriteFilePositionArray*		m_pSFPA;
};

//----------------------------------------------------------------------
// Node 3
//----------------------------------------------------------------------
class MLoadingSPKWorkNode3 : public MLoadingSPKWorkNode {
	public :
		MLoadingSPKWorkNode3(TYPE_SPRITEID spriteID, long fp) { m_SpriteID = spriteID; m_FilePosition = fp; }
		~MLoadingSPKWorkNode3() {}

		BOOL		Execute(MWorkNode*& pRemainNode);
		
	protected :	
		TYPE_SPRITEID			m_SpriteID;
		long					m_FilePosition;
};

//----------------------------------------------------------------------
// Node 4
//----------------------------------------------------------------------
class MLoadingSPKWorkNode4 : public MLoadingSPKWorkNode {
	public :
		MLoadingSPKWorkNode4(TYPE_SPRITEID fID, TYPE_SPRITEID lID, long fp) { m_FirstSpriteID = fID; m_LastSpriteID = lID; m_FilePosition = fp; }
		~MLoadingSPKWorkNode4() {}

		BOOL		Execute(MWorkNode*& pRemainNode);
		

	protected :
		TYPE_SPRITEID			m_FirstSpriteID;
		TYPE_SPRITEID			m_LastSpriteID;
		long					m_FilePosition;
};

#endif

