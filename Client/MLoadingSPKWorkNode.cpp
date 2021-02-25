//----------------------------------------------------------------------
// MLoadingSPKWorkNode.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MLoadingSPKWorkNode.h"

extern bool		FileOpenBinary(const char* filename, class ifstream& file);

//----------------------------------------------------------------------
// MLoadingSPKWorkNode1 :: Execute
//----------------------------------------------------------------------
//		= FilePosition에서 SFPArray를 읽어서 SPK에 load한다.
//		CSpritePack*	SPK
//		long			FilePosition
//
//		[Remain] --> Node 2
//----------------------------------------------------------------------
BOOL
MLoadingSPKWorkNode1::Execute(MWorkNode*& pRemainNode)
{
	// 실행중
	//m_bExecute = TRUE;

	//---------------------------------------------------
	//
	// Load SpriteFilePositionArray 
	//
	//---------------------------------------------------
	class ifstream file;
	if (!FileOpenBinary(m_SFPAFilename, file))
	{
		// -_-;;
		m_bExecute = FALSE;
		pRemainNode = NULL;
		return FALSE;
	}
	CSpriteFilePositionArray SFPA;
	SFPA.LoadFromFile( file );
	file.close();

	//---------------------------------------------------
	//
	// SPK Load
	//
	//---------------------------------------------------
	class ifstream spkFile;
	if (!FileOpenBinary(m_SPKFilename, spkFile))
	{
		// -_-;;
		m_bExecute = FALSE;
		pRemainNode = NULL;
		return FALSE;
	}

	int size = SFPA.GetSize();
	for (int i=0; i<size; i++)
	{
		//---------------------------------------------------
		// 계속 load중인 경우..
		//---------------------------------------------------
		if (m_bExecute)
		{
			// Load할려는 위치까지 FilePosition을 이동한다.
			spkFile.seekg( SFPA[i].FilePosition, ios::beg );

			CSprite& sprite = (*m_pSPK)[SFPA[i].SpriteID];

			//---------------------------------------------------
			// Sprite를 Load한다.
			//---------------------------------------------------
			// 아직 Load되지 않은 경우에만 Load한다.
			if (sprite.IsInit())
				continue;

			sprite.LoadFromFile( spkFile );
		}
		//---------------------------------------------------
		// Stop 된 경우..
		//---------------------------------------------------
		else
		{
			//---------------------------------------------------
			// 중지된 경우이다.
			//---------------------------------------------------
			// 남은 SFPA를 새로운 SFPA로 생성한다.
			// --> Node2	
			CSpriteFilePositionArray* pRemainSFPA = new CSpriteFilePositionArray;

			int remainSize = size - i;

			pRemainSFPA->Init( remainSize );			

			// m_SFPA의 남은 부분을 복사한다.
			for (int j=0; j<remainSize; j++)
			{
				(*pRemainSFPA)[j] = SFPA[i + j];
			}

			MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pRemainSFPA );

			pNode->SetSPK( m_pSPK, m_SPKFilename );
			pNode->SetType( m_Type );

			// 남은 일을 넘겨준다.
			spkFile.close();
			pRemainNode = pNode;
			return FALSE;
		}
	}
	spkFile.close();

	// 실행 끝
	m_bExecute = FALSE;

	// 정상적인 종료..
	pRemainNode = NULL;
	return TRUE;
}


//----------------------------------------------------------------------
// MLoadingSPKWorkNode2 :: Execute
//----------------------------------------------------------------------
//		= SFPArray를 이용해서 SPK에다 load한다.
//
//		SFPArray*		SFPA
//
//		[Remain] --> Node 2
//----------------------------------------------------------------------
BOOL
MLoadingSPKWorkNode2::Execute(MWorkNode*& pRemainNode)
{
	//m_bExecute = TRUE;

	//---------------------------------------------------
	//
	// SPK Load
	//
	//---------------------------------------------------
	class ifstream spkFile;
	if (!FileOpenBinary(m_SPKFilename, spkFile))
	{
		// -_-;;
		m_bExecute = FALSE;
		pRemainNode = NULL;
		return FALSE;
	}

	int size = m_pSFPA->GetSize();
	for (int i=0; i<size; i++)
	{
		SPRITE_FILEPOSITION_NODE& node = (*m_pSFPA)[i];

		//---------------------------------------------------
		// 계속 load중인 경우..
		//---------------------------------------------------
		if (m_bExecute)
		{
			// Load할려는 위치까지 FilePosition을 이동한다.
			spkFile.seekg( node.FilePosition, ios::beg );

			CSprite& sprite = (*m_pSPK)[node.SpriteID];

			//---------------------------------------------------
			// Sprite를 Load한다.
			//---------------------------------------------------
			// 아직 Load되지 않은 경우에만 Load한다.
			if (sprite.IsInit())
				continue;

			sprite.LoadFromFile( spkFile );
		}
		//---------------------------------------------------
		// Stop 된 경우..
		//---------------------------------------------------
		else
		{
			CSpriteFilePositionArray* pRemainSFPA = new CSpriteFilePositionArray;

			int remainSize = size - i;

			pRemainSFPA->Init( remainSize );			

			// m_SFPA의 남은 부분을 복사한다.
			for (int j=0; j<remainSize; j++)
			{
				(*pRemainSFPA)[j] = (*m_pSFPA)[i + j];
			}

			MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pRemainSFPA );

			pNode->SetSPK( m_pSPK, m_SPKFilename );
			pNode->SetType( m_Type );

			// 남은 일을 넘겨준다.
			spkFile.close();
			pRemainNode = pNode;
			return FALSE;
		}
	}
	spkFile.close();

	// 실행 끝
	m_bExecute = FALSE;

	// 정상적으로 끝난 경우
	pRemainNode = NULL;
	return TRUE;
}

//----------------------------------------------------------------------
// MLoadingSPKWorkNode3 :: Execute
//----------------------------------------------------------------------
//		TYPE_SPRITEID	SpriteID
//		long			FilePosition
//
//		[Remain] --> NULL
//----------------------------------------------------------------------
BOOL
MLoadingSPKWorkNode3::Execute(MWorkNode*& pRemainNode)
{
	//m_bExecute = TRUE;

	CSprite& sprite = (*m_pSPK)[m_SpriteID];

	//-------------------------------------------------------------
	// 아직 Loading되지 않은 경우에만 loading한다.
	//-------------------------------------------------------------
	if (sprite.IsNotInit())
	{	
		class ifstream spkFile;
		if (!FileOpenBinary(m_SPKFilename, spkFile))
		{
			// -_-;;
			m_bExecute = FALSE;
			pRemainNode = NULL;
			return FALSE;
		}

		// loading할려는 sprite에 접근한다.
		spkFile.seekg(m_FilePosition, ios::beg);
		
		//--------------------------------------------------
		// Sprite 하나 Loading
		//--------------------------------------------------
		if (sprite.LoadFromFile( spkFile ))
		{
		}
		else
		{
			// -_-;;
			// 뭐지.. 이럴 수 있나..
			int a =0;
		}
		spkFile.close();
	}

	m_bExecute = FALSE;	

	pRemainNode = NULL;
	return TRUE;
}


//----------------------------------------------------------------------
// MLoadingSPKWorkNode4 :: Execute
//----------------------------------------------------------------------
//		TYPE_SPRITEID	FirstSpriteID
//		TYPE_SPRITEID	LastSpriteID
//		long			FilePosition
//
//		[Remain] --> Node 4
//----------------------------------------------------------------------
BOOL
MLoadingSPKWorkNode4::Execute(MWorkNode*& pRemainNode)
{
	//m_bExecute = TRUE;

	class ifstream spkFile;
	if (!FileOpenBinary(m_SPKFilename, spkFile))
	{
		// -_-;;
		m_bExecute = FALSE;
		pRemainNode = NULL;
		return FALSE;
	}

	//-------------------------------------------------------
	// FirstSprite에 접근..
	//-------------------------------------------------------
	spkFile.seekg(m_FilePosition, ios::beg);

	//-------------------------------------------------------
	// FirstSpriteID ~ LastSpriteID를 Loading한다.
	//-------------------------------------------------------
	for (TYPE_SPRITEID i=m_FirstSpriteID; i<m_LastSpriteID; i++)
	{
		//---------------------------------------------------
		// 계속 load중인 경우..
		//---------------------------------------------------
		if (m_bExecute)
		{
			CSprite& sprite = (*m_pSPK)[i];

			//---------------------------------------------------
			// 아직 loading되지 않은 경우에만 loading한다.
			// (!) 안된다.. file position을 증가시켜야 되기 때메..
			// 무조건 loading!
			//---------------------------------------------------
			//if (sprite.IsNotInit())
			{
				sprite.LoadFromFile( spkFile );
			}
		}
		//---------------------------------------------------
		// Stop된 경우
		//---------------------------------------------------
		else
		{
			//---------------------------------------------------
			// firstSpriteID를 현재의 i로 설정해서 node를 생성한다.
			//---------------------------------------------------
			MLoadingSPKWorkNode4* pNode = new MLoadingSPKWorkNode4( 
													i,					// first SpriteID
													m_LastSpriteID,		// last SpriteID
													spkFile.tellg()		// file position
												);
			pNode->SetSPK( m_pSPK, m_SPKFilename );
			pNode->SetType( m_Type );

			spkFile.close();
			
			pRemainNode = pNode;
			return FALSE;
		}
	}

	m_bExecute = FALSE;

	spkFile.close();
	pRemainNode = NULL;
	return TRUE;
}

