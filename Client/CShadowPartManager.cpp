//----------------------------------------------------------------------
// CShadowPartManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#ifdef PLATFORM_WINDOWS
#include "DX3D.h"
#endif
#include "CFileIndexTable.h"
#include "CShadowPartManager.h"
#include "MGameTime.h"
#include "MWeather.h"

#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

extern bool FileOpenBinary(const char* filename, class ifstream& fil);

extern bool		g_bZonePlayerInLarge;

// 이거 128하면 memory가 장난이 아니다.
// detail이 문제인데... 흠냐..
#define	MAX_SIZE_TEXTURE_SHADOW		128


#ifdef OUTPUT_DEBUG
	#undef OUTPUT_DEBUG
#endif


//----------------------------------------------------------------------
//
// contructor/destructor
//
//----------------------------------------------------------------------
//CShadowPartManager::CShadowPartManager(CShadowSpritePack* pSSPK, WORD partSize)
CShadowPartManager::CShadowPartManager(const char* SSPKFilename, WORD partSize)
{
	//m_pSSPK		= NULL;
	m_pWidth	= NULL;
	m_pHeight	= NULL;

//	m_bOpen = false;
//	Init(SSPKFilename, indexFilename, partSize);	
	Init(SSPKFilename, partSize);	

	//Init(pSSPK, partSize);
}

CShadowPartManager::~CShadowPartManager()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init(SSPKFilename, indexFilename)
//----------------------------------------------------------------------
// SSPKFilename은 ShadowSpritePack이다.
// indexFilename은 ShadowSpritePack Index이다.
//
// SSPKFilename은 화일을 열어서 나중에 읽을 수 있도록 해두고
// indexFilename은 모두 읽어서 기억해둔다.
//----------------------------------------------------------------------
void
//CShadowPartManager::Init(CShadowSpritePack* pSSPK, WORD partSize)
CShadowPartManager::Init(const char* SSPKFilename, WORD partSize)
{
	Release();
	///*
	// Index File을 Load한다.
	class ifstream indexFile;//(indexFilename, ios::binary);
//	if (!FileOpenBinary(indexFilename, indexFile))
//		return;
//
//	m_SSPKIndex.LoadFromFile( indexFile );
//	indexFile.close();
//
//	// SSPK를 초기화 한다. (개수만큼..)
//	m_SSPK.Init( m_SSPKIndex.GetSize() );
//
//	// SSPK File을 열어둔다.
//	m_SSPKFile.open(SSPKFilename, ios::binary);
//	
//	TYPE_SPRITEID size;
//	m_SSPKFile.read((char*)&size, SIZE_SPRITEID);
	m_SSPK.LoadFromFileRunning(SSPKFilename);

	int allSize = m_SSPK.GetSize();
	//*/

	//m_pSSPK = pSSPK;

	//int allSize = pSSPK->GetSize();

	// Base class의 Init를 이용해서..
	// ( 전체 개수, 메모리 허용 개수 ) 
	CPartManager<WORD, WORD, CSpriteSurface*>::Init( allSize, partSize );

	// NULL로 초기화
	for (int i=0; i<m_nPart; i++)
	{
		m_pData[i] = NULL;
	}

	
	// texture size를 설정한다.
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
			if ((*pSSPK)[i].IsInit())
			{
				spWidth = (*pSSPK)[i].GetWidth();
				spHeight = (*pSSPK)[i].GetHeight();
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
			if ((*pSSPK)[i].IsInit())
			{
				spWidth = (*pSSPK)[i].GetWidth();
				spHeight = (*pSSPK)[i].GetHeight();
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

//	m_bOpen = true;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
// 열려진 SSPK file을 닫는다.
//----------------------------------------------------------------------
void	
CShadowPartManager::Release()
{
	///*
//	if (m_bOpen)
//	{
//		m_SSPKFile.close();
//		m_bOpen = false;
//	}
	//*/	
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

	DeleteRemoved();
}

//----------------------------------------------------------------------
// Delete Removed
//----------------------------------------------------------------------
// 지워야 할 것들
//----------------------------------------------------------------------
void				
CShadowPartManager::DeleteRemoved()
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
CShadowPartManager::Clear()
{
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

	DeleteRemoved();
}

//----------------------------------------------------------------------
// GetTexture( id )
//----------------------------------------------------------------------
// SSPK에서 ID가 id인 ShadowSprite가 Load된 Surface를 넘겨줘야 하는데,
//
// Load되지 않았으면,
// Index를 참고해서 열려진 SSPK File에서 
// id에 해당하는 ShadowSprite를 Load해서 Surface를 하나 생성하고
// 그 Surface에 ShadowSprite를 출력해준다.
//
// 이 부분에서 속도가 상당히 느려지지 않을까... 
//----------------------------------------------------------------------
CSpriteSurface*		
CShadowPartManager::GetTexture(TYPE_SPRITEID id)
{
	//static int reload	= 0;
	//static int reuse	= 0;

	//-----------------------------------------------------------
	// 없으면 --> Load & return
	//-----------------------------------------------------------
	if (IsDataNULL(id))
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("CShadowPartManager::GetTexture New(%d)", id);
			}
		#endif	

		CShadowSprite* pSprite = &m_SSPK[id];
		//CShadowSprite* pSprite = &(*m_pSSPK)[id];


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
//			// ShadowSprite를 Load한다.
//			//-----------------------------------------------------------
//			// id에 맞는 적절한 FilePointer로 이동해서
//			int temp = m_SSPKIndex[id];
//			m_SSPKFile.seekg(m_SSPKIndex[id], ios::beg);
//			pSprite->LoadFromFile( m_SSPKFile );
//		}
		//*/
		
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Loading ShadowSprite OK");
			}
		#endif	

		
		// shift값을 적절히 정해야 한다.
		// width와 height는 256을 넘어가면 안된다.
		// Texture의 Size를 결정한다.
		int spWidth, spHeight;
		int width, height;


		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Determine Texture GetSize OK");
			}
		#endif	

		spWidth = pSprite->GetWidth();
		spHeight = pSprite->GetHeight();
		
		//-----------------------------------------------------
		// 길이가 0인 경우..
		//-----------------------------------------------------
		if (spWidth==0 || spHeight==0)
		{
			return NULL;
		}

		width = spWidth;
		height = spHeight;


		int shift = 0;
		// 가로,세로 어느 하나라도 MAX_SIZE_TEXTURE_SHADOW보다 크면..
		while (width>MAX_SIZE_TEXTURE_SHADOW || height>MAX_SIZE_TEXTURE_SHADOW)
		{
			width >>= 1;
			height >>= 1;
			shift ++;
		}
		
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

			// 큰 쪽에 맞추면?..
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
			shift = 0;		
			while (spWidth > width || spHeight > height)
			{
				shift ++;

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

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Determine Texture Size OK");
			}
		#endif

		//-----------------------------------------------------------
		// (다시) 생성한다.
		//-----------------------------------------------------------
		CSpriteSurface* pTextureSurface = new CSpriteSurface;

		if (pTextureSurface==NULL)
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] Texture Surface is NULL");
				}
			#endif

			return false;
		}
		
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("New Texture Surface OK");
			}
		#endif
		

		//---------------------------------------------------
		// TextureSurface 생성
		//---------------------------------------------------	
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Before Get Pixel Format");
			}
		#endif
		const LPDDPIXELFORMAT pixel4444 = CDirect3D::GetPixelFormat4444();

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Before InitTexture Surface");
			}
		#endif

		pTextureSurface->InitTextureSurface(width, height, 0, pixel4444);
		POINT point;
		point.x = 0;
		point.y = 0;

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("Init Texture OK. num=%d, size=(%d, %d)", m_nPartUsed, pTextureSurface->GetWidth(), pTextureSurface->GetHeight());
			}
		#endif

		if (!pTextureSurface->Lock())
		{
			delete pTextureSurface;
			return false;
		}
			/*
		DDSURFACEDESC2 ddsd;

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		while (1)
		{
			HRESULT hr = pTextureSurface->GetSurface()->Lock(NULL, &ddsd,  DDLOCK_WAIT, NULL);

			if (hr==DD_OK)
			{
				break;
			}

			if( hr == DDERR_SURFACELOST )
			{
				bool ret = pTextureSurface->Restore();
				if (ret == false)
					break;
				else
					continue;
			}

			if( hr != DDERR_WASSTILLDRAWING )
			{
				#ifdef OUTPUT_DEBUG
					if (g_pDebugMessage)
					{
						DEBUG_ADD("Lock Failed");
					}
				#endif
				
				return false;
			}			
		}
		*/
		

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Lock OK");
			}
		#endif

		int i;

		//---------------------------------------------------
		// Texture Surface 초기화
		//---------------------------------------------------
		//static WORD trans[1024] = { 0, };

		WORD *pSurface = (WORD*)pTextureSurface->GetSurfacePointer();
				//,	*pSurfaceTemp;
		long pitch	= pTextureSurface->GetSurfacePitch();

		// 검게 칠하는 부분..
		//if (bDifferentSize)
		{
			int width2 = width << 1;
			i = pTextureSurface->GetHeight();

			if (i>0)
			{
				do
				{			
					memset(pSurface, 0, width2);
					//memcpy(pSurface, trans, width);
					pSurface = (WORD*)((BYTE*)pSurface + pitch);
				} while (--i);
			}
		}
	
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Make Texture OK");
			}
		#endif
		
		//---------------------------------------------------
		// 시간에 따른 그림자 출력
		//---------------------------------------------------
		const int timeShadow[] =
		{
			0x4000,		// 0시
			0x4000,		// 1시
			0x4000,		// 2시
			0x4000,		// 3시
			0x4000,		// 4시
			0x4000,		// 5시
			0x4000,		// 6시
			0x5000,		// 7시
			0x6000,		// 8시
			0x7000,		// 9시
			0x8000,		// 10시
			0x9000,		// 11시
			0xA000,		// 12시
			0xB000,		// 13시
			0xC000,		// 14시
			0xB000,		// 15시
			0xA000,		// 16시
			0x9000,		// 17시
			0x8000,		// 18시
			0x7000,		// 19시
			0x6000,		// 20시
			0x5000,		// 21시
			0x4000,		// 22시
			0x4000		// 23시			
		};

		WORD shadowColor = timeShadow[(*g_pGameTime).GetHour()];


		//---------------------------------------------------
		// 건물 안에는 무조건 0x8000
		//---------------------------------------------------
		if (!g_bZonePlayerInLarge)
		{
			shadowColor = 0x8000;
		}
		else
		{
			//---------------------------------------------------
			// 비가 오면 그림자가 더 엷어진다.
			//---------------------------------------------------
			if (g_pWeather->GetWeatherType()==MWeather::WEATHER_RAIN)
			{
				int test = (int)shadowColor - (int)0x4000;

				if (test < 0x3000)
				{
					test = 0x3000;
				}

				shadowColor = test;
			}
		}


		//---------------------------------------------------
		// ShadowSprite출력
		//---------------------------------------------------
		pTextureSurface->BltShadowSpriteSmall4444(&point, pSprite, shadowColor, shift);
		
		
	  
		pTextureSurface->Unlock();

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Blt Small4444 OK");
			}
		#endif
	
			
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
				for (int i=0; i<shift; i++)
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

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Set Data OK");
			}
		#endif

		if (oldIndex != m_IndexNULL)
		{
			if (pOld != NULL)
			{
				#ifdef OUTPUT_DEBUG
					if (g_pDebugMessage)
					{
						DEBUG_ADD_FORMAT("Delete Old Surface(%d), (%d,%d)", oldIndex, m_SSPK[oldIndex].GetWidth(), m_SSPK[oldIndex].GetHeight());
					}
				#endif
				
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
			// 그림자는 괜찮을려나? 흠.. 
			//m_SSPK[oldIndex].Release();			
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
	//-----------------------------------------------------------
	// 있는 경우 --> return
	//-----------------------------------------------------------
	else
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("CShadowPartManager::GetTexture(%d)", id);
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

	return NULL;
}


//----------------------------------------------------------------------
// Get Sprite
//----------------------------------------------------------------------
CShadowSprite&		
CShadowPartManager::GetSprite(TYPE_SPRITEID id)
{
	//------------------------------------------------------
	// Load가 안 됐으면 load해서 넘겨준다.
	//------------------------------------------------------
//	if (m_SSPK[id].IsNotInit())
//	{
//		int temp = m_SSPKIndex[id];
//		m_SSPKFile.seekg(m_SSPKIndex[id], ios::beg);
//		m_SSPK[id].LoadFromFile( m_SSPKFile );
//	}
//
	return m_SSPK[id];
}