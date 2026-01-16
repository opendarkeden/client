//----------------------------------------------------------------------
// CSpriteTexturePartManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#ifdef PLATFORM_WINDOWS
#include "DX3D.h"
#else
#include "CDirect3D.h"
#endif
#include "CFileIndexTable.h"
#include "CSpriteTexturePartManager.h"

#include "DebugInfo.h"
//#include "MFileDef.h"
#include "Properties.h"

//----------------------------------------------------------------------
//
// contructor/destructor
//
//----------------------------------------------------------------------
//CNormalSpriteTexturePartManager::CSpriteTexturePartManager(CAlphaSpritePack* pSPK, WORD partSize)
CNormalSpriteTexturePartManager::CNormalSpriteTexturePartManager(CSpritePack* pSPK, WORD partSize)
{
	m_pSPK = NULL;
	m_pWidth = NULL;
	m_pHeight = NULL;

//	m_bOpen = false;
	Init(pSPK, partSize);	

	//Init(pSPK, partSize);	
}

CNormalSpriteTexturePartManager::~CNormalSpriteTexturePartManager()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init(spkFilename, indexFilename)
//----------------------------------------------------------------------
// spkFilename은 AlphaSpritePack이다.
// indexFilename은 AlphaSpritePack Index이다.
//
// spkFilename은 화일을 열어서 나중에 읽을 수 있도록 해두고
// indexFilename은 모두 읽어서 기억해둔다.
//----------------------------------------------------------------------
void
CNormalSpriteTexturePartManager::Init(CSpritePack* pSPK, WORD partSize)
//CNormalSpriteTexturePartManager::Init(const char* spkFilename, WORD partSize)
{
	Release();

	///*
	// Index File을 Load한다.
//	std::ifstream indexFile(indexFilename, ios::binary);
//	m_SPKIndex.LoadFromFile( indexFile );
//	indexFile.close();
//
//	// SPK를 초기화 한다. (개수만큼..)
//	m_SPK.Init( m_SPKIndex.GetSize());//, CDirectDraw::Is565() );
//
//	// SPK File을 열어둔다.
//	m_SPKFile.open(spkFilename, ios::binary);
//	
//	TYPE_SPRITEID size;
//	m_SPKFile.read((char*)&size, SIZE_SPRITEID);
//
//	m_SPK.LoadFromFileRunning(spkFilename);
	m_pSPK = pSPK;
	int allSize = m_pSPK->GetSize(); 
	//*/


	//m_pSPK = pSPK;

	//int allSize = pSPK->GetSize();

	// Base class의 Init를 이용해서..
	// ( 전체 개수, 메모리 허용 개수 ) 
	CPartManager<WORD, WORD, CSpriteSurface*>::Init( allSize, partSize );
	
	// NULL로 초기화
	for (int i=0; i<m_nPart; i++)
	{
		m_pData[i] = NULL;
	}

	
	m_pWidth = new int [allSize];
	m_pHeight = new int [allSize];

	for (int i=0; i<allSize; i++)
	{
		m_pWidth[i] = 0;
		m_pHeight[i] = 0;
	}

	/*
	int spWidth, spHeight;
	int width, height;

	if (CDirect3D::IsTexturePow2())
	{
		for (int i=0; i<allSize; i++)
		{
			if ((*pSPK)[i].IsInit())
			{
				spWidth = (*pSPK)[i].GetWidth();
				spHeight = (*pSPK)[i].GetHeight();
				width = 1;
				height = 1;

				// width와 height는 spWidth와 spHeight보다 커야 한다.
				// square로 맞춰준다.
				while (width < spWidth || height < spHeight)
				{
					width <<= 1;
					height <<= 1;
				}

				m_pWidth[i] = width;
				m_pHeight[i] = height;
			}
			else
			{
				m_pWidth[i] = 0;
				m_pHeight[i] = 0;
			}
		}
	}
	else
	{
		for (int i=0; i<allSize; i++)
		{
			if ((*pSPK)[i].IsInit())
			{
				spWidth = (*pSPK)[i].GetWidth();
				spHeight = (*pSPK)[i].GetHeight();
				width = spWidth;
				height = spHeight;

				// 큰 크기로 맞추어서 Square로 만든다.
				if (CDirect3D::IsTextureSquareOnly())
				{				
					if (width > height)
					{
						height = width;				
					}
					else if (width < height)
					{
						width = height;				
					}	
				}

				m_pWidth[i] = width;
				m_pHeight[i] = height;
			}
			else
			{
				m_pWidth[i] = 0;
				m_pHeight[i] = 0;
			}
		}
	}
	*/

// 	m_bOpen = true;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
// 열려진 SPK file을 닫는다.
//----------------------------------------------------------------------
void	
CNormalSpriteTexturePartManager::Release()
{
	///*
//	if (m_bOpen)
//	{
//		m_SPKFile.close();
//		m_bOpen = false;
//	}
	//*/	
	if (m_pData!=NULL)
	{
		for (int i=0; i<m_nPart; i++)
		{
			if (m_pData[i]!=NULL)
			{
				delete m_pData[i];
				m_pData[i] = NULL;
			}
		}
	}

	if (m_pWidth != NULL)
	{
		delete [] m_pWidth;
		m_pWidth = NULL;
	}

	if (m_pHeight != NULL)
	{
		delete [] m_pHeight;
		m_pHeight = NULL;
	}

	DeleteRemoved();
}

//----------------------------------------------------------------------
// Delete Removed
//----------------------------------------------------------------------
// 지워야 할 것들
//----------------------------------------------------------------------
void				
CNormalSpriteTexturePartManager::DeleteRemoved()
{
	SPRITESURFACE_LIST::iterator iSurface = m_listRemoved.begin();

	while (iSurface != m_listRemoved.end())
	{
		CSpriteSurface*	pSurface = *iSurface;

		delete pSurface;
		
		iSurface++;
	}

	m_listRemoved.clear();
}

//----------------------------------------------------------------------
// Clear
//----------------------------------------------------------------------
// Video Memory의 내용은 clear한다.
// 실제로는... 다 날려버리는 것이다.
//----------------------------------------------------------------------
void
CNormalSpriteTexturePartManager::Clear()
{	
	DeleteRemoved();

	// video memory를 날려~준다.
	for (int i=0; i<m_nPart; i++)
	{
		if (m_pData[i] != NULL)			
		{
			m_pData[i]->Restore();
			delete m_pData[i];
			m_pData[i] = NULL;
		}
	}

	// Base class의 Init를 이용해서..
	// ( 전체 개수, 메모리 허용 개수 ) 
	CPartManager<WORD, WORD, CSpriteSurface*>::Init( m_nIndex, m_nPart );

	// NULL로 초기화
	for (int i=0; i<m_nPart; i++)
	{
		m_pData[i] = NULL;
	}

}

//----------------------------------------------------------------------
// GetTexture( id )
//----------------------------------------------------------------------
// SPK에서 ID가 id인 AlphaSprite가 Load된 Surface를 넘겨줘야 하는데,
//
// Load되지 않았으면,
// Index를 참고해서 열려진 SPK File에서 
// id에 해당하는 AlphaSprite를 Load해서 Surface를 하나 생성하고
// 그 Surface에 AlphaSprite를 출력해준다.
//
// 이 부분에서 속도가 상당히 느려지지 않을까... 
//----------------------------------------------------------------------
CSpriteSurface*		
CNormalSpriteTexturePartManager::GetTexture(TYPE_SPRITEID id)
{
	//static int reload	= 0;
	//static int reuse	= 0;

	//-----------------------------------------------------------
	// 없으면 --> Load & return
	//-----------------------------------------------------------
	if (IsDataNotNULL(id))
	{
#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD_FORMAT("CNormalSpriteTexturePartManager::GetTexture(%d)", id);
		}
#endif
		
		CSpriteSurface* pTextureSurface;
		if (GetData(id, pTextureSurface))
		{
			return pTextureSurface;
		}
		
#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD("No Exist!");
		}
#endif
		
		//reuse ++;
	}
	else
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("CNormalSpriteTexturePartManager::GetTexture New(%d)", id);
			}
		#endif

		CSprite* pSprite = &(*m_pSPK)[id];
		//CAlphaSprite* pSprite = &(*m_pSPK)[id];

		//// 다 Load되어 있다고 가정한다..
		///*
		//----------------------------------------
		// 이미 Load된 것이면..
		//----------------------------------------
//		if (pSprite->IsInit())
//		{
//		}
//		//----------------------------------------
//		// Load해야 되는 경우
//		//----------------------------------------
//		else
//		{
//			//-----------------------------------------------------------
//			// AlphaSprite를 Load한다.
//			//-----------------------------------------------------------
//			// id에 맞는 적절한 FilePointer로 이동해서
//			int temp = m_SPKIndex[id];
//			m_SPKFile.seekg(m_SPKIndex[id], ios::beg);
//			pSprite->LoadFromFile( m_SPKFile );
//		}
		//*/
		// Texture의 Size를 결정한다.
		int spWidth, spHeight;
		int width, height;
		BYTE smallShift = 0;

		spWidth = pSprite->GetWidth();
		spHeight = pSprite->GetHeight();

		//-----------------------------------------------------
		// 길이가 0인 경우..
		//-----------------------------------------------------
		if (spWidth==0 || spHeight==0)
		{
			return NULL;
		}

		//-----------------------------------------------------------
		// (다시) 생성한다.
		//-----------------------------------------------------------
		CSpriteSurface* pTextureSurface = new CSpriteSurface;
	

		width = spWidth;
		height = spHeight;
		CDirect3D::GetTextureSize(width, height);		

		//-----------------------------------------------------------
		// 2의 승수만 지원하는 경우..
		//-----------------------------------------------------------
		bool bDifferentSize;	// sprite와 texture의 크기가 다른가?
		if (CDirect3D::IsTexturePow2())
		{
			bDifferentSize = true;

			// width와 height가 다른 경우...
			// 작은 쪽에 맞춰서 똑같이 한다.
			// Square로 맞춘다..고 할 수 있지.. 음하..
			
			// 큰 쪽에 맞추면... detail은 높아지는데.. 
			// 뭔가 문제가 있었던거 같기도 한데(-_-;)
			// 지금은 생각하기 싫어서... - -;;;
			// 작은 쪽에 맞추면 detail이 떨어진다.
			// 암튼 이 부분 체크를 해야한다.			
			if (width > height)
			{
				height = width;
			}
			else //if (width < height)
			{
				width = height;
			}

			//-----------------------------------------------------------
			// Texture크기보다 Sprite 크기가 더 큰 경우..
			//-----------------------------------------------------------
			// 즉, 하드웨어에서 Sprite크기만큼의 Texture를 지원하지 못하는 경우이다.		
			// shift를 이용해서 크기를 줄인다.
			while (spWidth > width || spHeight > height)
			{
				smallShift ++;

				spWidth >>= 1;
				spHeight >>= 1;
			}
		}
		//-----------------------------------------------------------
		// 아무런 size나 관계 없는 경우
		//-----------------------------------------------------------
		else
		{
			if (CDirect3D::IsTextureSquareOnly())
			{
				// width와 height가 다른 경우...
				// 큰 쪽에 맞춰서 똑같이 한다.
				// Square로 맞춘다..고 할 수 있지.. 음하..
				if (width > height)
				{
					height = width;
					bDifferentSize = true;
				}
				else if (width < height)
				{
					width = height;
					bDifferentSize = true;
				}				
			}
			else
			{
				bDifferentSize = false;
			}			
		}		

		//---------------------------------------------------
		// TextureSurface 생성
		//---------------------------------------------------		
		pTextureSurface->InitTextureSurface(width, height, 0, CDirect3D::GetPixelFormat1555());

		POINT point;
		point.x = 0;
		point.y = 0;
		
		if (!pTextureSurface->Lock())
		{
			delete pTextureSurface;
			return NULL;
		}

		int i;

		//---------------------------------------------------
		// 검게 칠하는 부분..
		//---------------------------------------------------
		//if (bDifferentSize)
		{
			//---------------------------------------------------
			// Texture Surface 초기화
			//---------------------------------------------------
			WORD *pSurface = (WORD*)pTextureSurface->GetSurfacePointer();
					//,	*pSurfaceTemp;
			long pitch	= pTextureSurface->GetSurfacePitch();

			int width2 = width << 1;

			for (int i=0; i<height; i++)
			{
				memset(pSurface, 0, width2);
				pSurface = (WORD*)((BYTE*)pSurface + pitch);
			}
			
			/*
			// Sprite가 차지하는 영역을 제외한 부분을 검게~~
			DWORD width2 = (width - spWidth) << 1;	// *2 
			pSurface += spWidth;

			// 오른쪽 옆부분
			if (width2 > 0)
			{
				i = spHeight;		
				do
				{			
					memset(pSurface, 0, width2);
					pSurface = (WORD*)((BYTE*)pSurface + pitch);
				} while (--i);
			}

			// 아래쪽
			pSurface -= spWidth;
			width2 = width << 1;
			i = height - spHeight;

			if (i>0)
			{
				do
				{			
					memset(pSurface, 0, width2);
					pSurface = (WORD*)((BYTE*)pSurface + pitch);
				} while (--i);
			}
			*/
		}

		//---------------------------------------------------
		// AlphaSprite출력
		//---------------------------------------------------
		// Effect의 크기를 고려해서..
		if (smallShift==0)
		{
			//pTextureSurface->BltAlphaSprite4444NotTrans(&point, pSprite);
			pTextureSurface->BltSprite1555NotTrans(&point, pSprite);
		}
		else
		{
			pTextureSurface->BltSprite1555SmallNotTrans(&point, pSprite, smallShift);
		}
		
		pTextureSurface->Unlock();


		//-----------------------------------------------------------
		// 출력할때 사용할 크기
		//-----------------------------------------------------------
		// 이전에 출력할 Texture Size가 정해져 있지 않은 경우
		//-----------------------------------------------------------
		if (m_pWidth[id]==0 || m_pHeight[id]==0)
		{
			spWidth = pSprite->GetWidth();
			spHeight = pSprite->GetHeight();
			
			if (CDirect3D::IsTexturePow2())
			{	
				// texture에서 출력되는 size를 구한다.
				for (int i=0; i<smallShift; i++)
				{
					spWidth >>= 1;
					spHeight >>= 1;
				}

				// 실제 크기
				width = width * pSprite->GetWidth() / spWidth;
				height = height * pSprite->GetHeight() / spHeight;		
			}
			else
			{
				width = spWidth;
				height = spHeight;

				// 큰 크기로 맞추어서 Square로 만든다.
				if (CDirect3D::IsTextureSquareOnly())
				{				
					if (width > height)
					{
						height = width;				
					}
					else if (width < height)
					{
						width = height;				
					}	
				}
			}			

			m_pWidth[id] = width;
			m_pHeight[id] = height;		
		}
		
		//---------------------------------------------------
		// Replace됐으면 원래것을 메모리에서 지운다.
		//---------------------------------------------------
		CSpriteSurface* pOld = NULL;
		WORD oldIndex = SetData( id, pTextureSurface, pOld );
		
		if (oldIndex!=m_IndexNULL)
		{
			if (pOld != NULL)
			{
				//------------------------------------------------------------
				// D3D의 Texture는 바로 사용되지 않는 경우도 있다.
				// BeginScene() ~ EndScene() 까지 Surface는 살아있어야 하므로
				// 여기서 지우면 안된다.
				//------------------------------------------------------------
				//delete pOld;
				//------------------------------------------------------------
				m_listRemoved.push_back( pOld );
			}

			// loading되어 있던 sprite를 제거한다... 메모리 때문에..
			//m_SPK[oldIndex].Release();
		}

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("OK");
			}
		#endif

		// return
		return pTextureSurface;

		//reload++;
	}
	
	return NULL;
}
