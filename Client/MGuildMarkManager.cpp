//----------------------------------------------------------------------
// MGuildMarkManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MGuildMarkManager.h"
#include "MGuildInfoMapper.h"
#include "CSprite555.h"
#include "CSprite565.h"
#include "UtilityFunction.h"

#if defined(__GAME_CLIENT__) || defined(__GUILD_MANAGER_TOOL__)
	#include "UtilityFunction.h"
	#include "AppendPatchInfo.h"
	#include "DebugInfo.h"
#endif

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
MGuildMarkManager* g_pGuildMarkManager = NULL;

extern bool g_bUseProgressBar;


//----------------------------------------------------------------------
//
//						MGuildMarkManager
//
//----------------------------------------------------------------------
MGuildMarkManager::MGuildMarkManager(const char* pGuildMarkSPKFilename, 
									const char* pGuildInfoMapperFilename,
									const char* pSlayerGradeMarkSPKFilename,
									const char* pVampireGradeMarkSPKFilename,
									const char* pOustersGradeMarkSPKFilename,
									const char* pLevelMarkSPKFilename)
{
	m_GuildMarkSPKFilename = pGuildMarkSPKFilename;

//	m_SlayerGradeMarkSPKFilename = pSlayerGradeMarkSPKFilename;
//	m_VampireGradeMarkSPKFilename = pVampireGradeMarkSPKFilename;

	// SpriteIDMapper를 화일에서 읽어들인다.
	if (pGuildInfoMapperFilename!=NULL)
	{
		m_GuildMarkSpriteMapperFilename = pGuildInfoMapperFilename;		
	}

//	m_GradeMarks.Init(GRADE_MARK_MAX<<2, CDirectDraw::Is565());
	m_GradeMarks[RACE_SLAYER].LoadFromFileRunning(pSlayerGradeMarkSPKFilename);
	m_GradeMarks[RACE_VAMPIRE].LoadFromFileRunning(pVampireGradeMarkSPKFilename);
	m_GradeMarks[RACE_OUSTERS].LoadFromFileRunning(pOustersGradeMarkSPKFilename);
	m_LevelMarks.LoadFromFileRunning(pLevelMarkSPKFilename);
}

MGuildMarkManager::~MGuildMarkManager()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void		
MGuildMarkManager::Release()
{
	GUILDMARK_MAP::iterator iMark = m_GuildMarks.begin();

	while (iMark != m_GuildMarks.end())
	{
		CSprite* pSprite = iMark->second.pSprite;
		CSprite* pSpriteSmall = iMark->second.pSpriteSmall;

		if (pSprite!=NULL)
		{
			delete pSprite;
		}

		if (pSpriteSmall!=NULL)
		{
			delete pSpriteSmall;
		}

		iMark++;
	}
	m_GuildMarks.clear();
}

//----------------------------------------------------------------------
// Create GuildMark( guildID, pFilename )
//
// pFilename은 *.bmp이거나 *.jpg
//
// image화일인 pFilename을 읽어서
// CSprite로 만든 후, 
// guildMarkSPK(+SPKIndex)에 추가하고 그 때의 ID를
// guildID와 연결해서 guildMarkSpriteMapper에 추가해두면 된다.
//----------------------------------------------------------------------
bool		
MGuildMarkManager::CreateGuildMark(WORD guildID, const char* pFilename)
{
#if defined(__GAME_CLIENT__) || defined(__GUILD_MANAGER_TOOL__)
	if (pFilename==NULL)
	{
		return false;
	}

	//-----------------------------------------------------
	// pFilename을 Surface에 load한다.
	//-----------------------------------------------------
	CDirectDrawSurface bmpSurface;
	if (!LoadImageToSurface(pFilename, bmpSurface))
	{
		return false;
	}
	
	const POINT bigSize = { 40, 40 };
	const POINT smallSize = { 20, 20 };
	
	CDirectDrawSurface surface;
	surface.InitOffsurface( bigSize.x, bigSize.y, DDSCAPS_SYSTEMMEMORY );
		
	RECT destBigRect = { 0, 0, bigSize.x, bigSize.y };
	RECT destSmallRect = { 0, 0, smallSize.x, smallSize.y };
	
	//-----------------------------------------------------
	// CSprite생성
	//-----------------------------------------------------
	CSprite* pSprite = NULL;
	CSprite* pSpriteSmall = NULL;

	if (CDirectDraw::Is565())
	{
		pSprite = new CSprite565;
		pSpriteSmall = new CSprite565;
	}
	else
	{
		pSprite = new CSprite555;
		pSpriteSmall = new CSprite555;
	}

	//-----------------------------------------------------
	// surface --> CSprite
	//-----------------------------------------------------
	WORD* lpSurface;
	unsigned short pitch;

	RECT bmpRect = { 0, 0, bmpSurface.GetWidth(), bmpSurface.GetHeight() };

	// 파란색이 투명색이다.
	CSprite::SetColorkey( 0x001F );

	// BigSize
	surface.FillSurface( 0 );
	surface.Blt(&destBigRect, &bmpSurface, &bmpRect);
	surface.LockW(lpSurface, pitch);
	pSprite->SetPixel(lpSurface, pitch, bigSize.x, bigSize.y);
	surface.Unlock();	

	// SmallSize
	surface.FillSurface( 0 );
	surface.Blt(&destSmallRect, &bmpSurface, &bmpRect);
	surface.LockW(lpSurface, pitch);				
	pSpriteSmall->SetPixel(lpSurface, pitch, smallSize.x, smallSize.y);
	surface.Unlock();	

	//-----------------------------------------------------
	// add
	//-----------------------------------------------------
	AddGuildMark(guildID, pSprite, pSpriteSmall);
#endif
	return true;
}

//----------------------------------------------------------------------
// A GuildMark
//----------------------------------------------------------------------
bool		
MGuildMarkManager::AddGuildMark(WORD guildID, CSprite* pSprite, CSprite* pSpriteSmall)
{
	if (!SaveGuildMark(guildID, pSprite, pSpriteSmall))
	{
		// 저장 안돼도 일단 memory에 있으면 쓸 수 있다.		
		//return false;
	}

	GUILDMARK_MAP::iterator iMark = m_GuildMarks.find(guildID);

	// 이미 있다면 기존에꺼 delete해야 한다.
	if (iMark != m_GuildMarks.end())
	{
		CSprite* pSprite = iMark->second.pSprite;
		CSprite* pSpriteSmall = iMark->second.pSpriteSmall;

		if (pSprite!=NULL)
		{
			delete pSprite;
		}

		if (pSpriteSmall!=NULL)
		{
			delete pSpriteSmall;
		}
	}

	m_GuildMarks[guildID] = GUILD_SPRITES( pSprite, pSpriteSmall );

	return true;
}

//----------------------------------------------------------------------
// Get GuildMark
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetGuildMark(WORD guildID)
{
	GUILDMARK_MAP::iterator iMark = m_GuildMarks.find(guildID);

	if (iMark != m_GuildMarks.end())
	{
		return m_GuildMarks[guildID].pSprite;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Get GuildMarkSmall
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetGuildMarkSmall(WORD guildID)
{
	GUILDMARK_MAP::iterator iMark = m_GuildMarks.find(guildID);

	if (iMark != m_GuildMarks.end())
	{
		return m_GuildMarks[guildID].pSpriteSmall;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Has GuildMark
//----------------------------------------------------------------------
bool			
MGuildMarkManager::HasGuildMark(WORD guildID) const
{
	GUILDMARK_MAP::const_iterator iMark = m_GuildMarks.find(guildID);

	if (iMark != m_GuildMarks.end())
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Load GuildMark
//----------------------------------------------------------------------
// 길드ID를 guildMarkSpriteMapper에서 찾으면.. 관련된 SpriteID를 알 수 있다.
// guildMarkSPKIndex에서 그 SpriteID로 그 길드Sprite의 SpriteIndex를 
// 알 수 있으므로.. 바로 Load해올 수 있다.
//----------------------------------------------------------------------
bool		
MGuildMarkManager::LoadGuildMark(WORD guildID)
{
	// 이미 있으면 load 안한다.
	GUILDMARK_MAP::iterator iMark = m_GuildMarks.find(guildID);

	if (iMark != m_GuildMarks.end())
	{
		return true;
	}

	MGuildInfoMapper::const_iterator iMapper = g_pGuildInfoMapper->find( guildID );

	if (iMapper!=g_pGuildInfoMapper->end())
	{
		GUILD_INFO* pInfo		= iMapper->second;
		TYPE_SPRITEID spriteID	= pInfo->GetSpriteID();

		//---------------------------------------------------------
		// file이름 준비..
		//---------------------------------------------------------
		char spkiFilename[256];

		// filename.spki
		strcpy(spkiFilename, m_GuildMarkSPKFilename.GetString());
		strcat(spkiFilename, "i");

		std::ifstream spkFile(m_GuildMarkSPKFilename.GetString(), ios::binary );
		std::ifstream spkiFile(spkiFilename, ios::binary );

		TYPE_SPRITEID maxSpkSize = 0;

		if (spkFile.is_open()
			&& spkiFile.is_open())
		{			
			//---------------------------------------------------------
			// Sprite의 개수를 알아낸다.
			//---------------------------------------------------------
			spkiFile.seekg( 0, ios::beg );
			spkiFile.read((char*)&maxSpkSize, 2);			

			if (spriteID < maxSpkSize)	// spriteID체크
			{
				long fp = 0;

				//-----------------------------------------------------
				// 어디 들었는지 찾기
				//-----------------------------------------------------
				spkiFile.seekg( 2 + spriteID*sizeof(long), ios::beg );
				spkiFile.read((char*)&fp, 4);				
				spkiFile.close();

				spkFile.seekg( fp, ios::beg );

				//-----------------------------------------------------
				// CSprite생성
				//-----------------------------------------------------
				CSprite* pSprite = NULL;
				CSprite* pSpriteSmall = NULL;

				if (CDirectDraw::Is565())
				{
					pSprite = new CSprite565;
					pSpriteSmall = new CSprite565;
				}
				else
				{
					pSprite = new CSprite555;
					pSpriteSmall = new CSprite555;
				}

				//-----------------------------------------------------
				// Load 해서 
				//-----------------------------------------------------
				pSprite->LoadFromFile( spkFile );
				pSpriteSmall->LoadFromFile( spkFile );	
				
				spkFile.close();

				m_GuildMarks[guildID] = GUILD_SPRITES( pSprite, pSpriteSmall );

				return true;
			}
		}		
	}

	// 화일에 없다는 얘기니까..
	// 다시 체크안하도록 NULL로 넣어둔다.
	m_GuildMarks[guildID] = GUILD_SPRITES( NULL, NULL );

	return false;
}

//----------------------------------------------------------------------
// Save GuildMark
//----------------------------------------------------------------------
// m_GuildMarkSPKFilename에서 max SpriteID를 찾아보고..
// 화일의 끝에 CSprite를 추가한다.
// m_GuildMarkSPKIndexFilename에도 추가해야된다..
//
// 방금 추가된 spriteID를 return하면 된다.
//----------------------------------------------------------------------
bool		
MGuildMarkManager::SaveGuildMark(WORD guildID, CSprite* pSprite, CSprite* pSpriteSmall)
{
	if (IsInit()
		&& pSprite!=NULL
		&& pSpriteSmall!=NULL)
	{
		//---------------------------------------------------------
		// file이름 준비..
		//---------------------------------------------------------
		char spkiFilename[256];

		// filename.spki
		strcpy(spkiFilename, m_GuildMarkSPKFilename.GetString());
		strcat(spkiFilename, "i");

		
		//---------------------------------------------------------
		// file에 sprite를 추가한다.
		// index도 추가해야 한다.
		//---------------------------------------------------------
		std::ifstream spkInputFile(m_GuildMarkSPKFilename.GetString(), ios::binary );

		TYPE_SPRITEID maxSpkSize = 0;

		if (spkInputFile.is_open())
		{			
			//---------------------------------------------------------
			// Sprite의 개수를 알아낸다.
			//---------------------------------------------------------
			spkInputFile.seekg( 0, ios::beg );
			spkInputFile.read((char*)&maxSpkSize, 2);
			spkInputFile.close();
		}

		
		std::ofstream spkFile(m_GuildMarkSPKFilename.GetString(), ios::binary | ios::ate);
		std::ofstream spkiFile(spkiFilename, ios::binary | ios::ate);
		
		if (spkFile.is_open()
			&& spkiFile.is_open())
		{
			//---------------------------------------------------------
			//
			//					SpritePack
			//
			//---------------------------------------------------------			

			//---------------------------------------------------------
			// sprite개수 수정
			//---------------------------------------------------------
			TYPE_SPRITEID newSpkSize = maxSpkSize + 2;
			
			spkFile.seekp( 0, ios::beg );
			spkFile.write((const char*)&newSpkSize, 2);
			
			//---------------------------------------------------------
			// Sprite화일의 끝에 CSprite를 추가한다.			
			//---------------------------------------------------------
			spkFile.seekp( 0, ios::end );

			long fp = spkFile.tellp();		// index file에 저장할 fp			
			pSprite->SaveToFile( spkFile );

			long fpSmall = spkFile.tellp();		// index file에 저장할 fp
			pSpriteSmall->SaveToFile( spkFile );

			
			//---------------------------------------------------------
			//
			//					SpritePack Index
			//
			//---------------------------------------------------------					
			// SpriteIndex file 개수 수정
			//---------------------------------------------------------
			spkiFile.seekp( 0, ios::beg );
			spkiFile.write((const char*)&newSpkSize, 2);

			//---------------------------------------------------------
			// SpriteIndex file에 fp를 저장한다.
			//---------------------------------------------------------
			spkiFile.seekp( 0, ios::end );
			spkiFile.write((const char*)&fp, 4);
			spkiFile.write((const char*)&fpSmall, 4);
	
			spkFile.close();
			spkiFile.close();

			//---------------------------------------------------------
			//
			//					GuildSpriteIDMapper
			//
			//---------------------------------------------------------
			// BigSpriteID = maxSpkSize
			// SmallSpriteID = maxSpkSize + 1
			GUILD_INFO* pInfo = g_pGuildInfoMapper->Get(guildID);

			if (pInfo!=NULL)
			{
				pInfo->SetSpriteID( maxSpkSize );
			}
			else
			{
				// 없으면 생성			
				pInfo = new GUILD_INFO;

				// default
//				pInfo->SetGuildName("Guild");
//				pInfo->SetLeaderName("Leader");				
				pInfo->SetSpriteID( maxSpkSize );

				g_pGuildInfoMapper->Set(guildID, pInfo);
			}			
			
			// file에서 guildID부분만 newSpriteID로 바꾸면 된다.
			//class fstream mapperFile(m_GuildMarkSpriteMapperFilename.GetString(), ios::binary | ios::ate | ios::in | ios::out);
			//g_pGuildInfoMapper->ChangeValueToFile(m_GuildMarkSpriteMapperFilename.GetString(), guildID);

			std::ofstream file(m_GuildMarkSpriteMapperFilename.GetString(), ios::binary);

			if (file.is_open())
			{
				g_pGuildInfoMapper->SaveToFile(file);
			}

			// GuildSpriteIDMapper 개수 수정
			
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------
// MergeGuildMark
//----------------------------------------------------------------------
// org에 app를 추가하고
// app를 지운다.
//----------------------------------------------------------------------
bool		
MGuildMarkManager::MergeGuildMark(const char* pSPKFilenameOrg, 
									const char* pIDMapperFilenameOrg,
									const char* pSPKFilenameApp, 
									const char* pIDMapperFilenameApp)
{
	#if defined(__GAME_CLIENT__) || defined(__GUILD_MANAGER_TOOL__)
		if (pSPKFilenameOrg==NULL
			|| pIDMapperFilenameOrg==NULL
			|| pSPKFilenameApp==NULL
			|| pIDMapperFilenameApp==NULL)
		{
			return false;
		}

		//---------------------------------------------------------
		// file이름 준비..
		//---------------------------------------------------------
		char pSPKIFilenameOrg[256];
		char pSPKIFilenameApp[256];

		sprintf(pSPKIFilenameOrg, "%si", pSPKFilenameOrg);
		sprintf(pSPKIFilenameApp, "%si", pSPKFilenameApp);
		

		//---------------------------------------------------------
		// SPKOrg의 maxSprite개수를 기억해둔다.
		// (SPKApp의 0 --> orgSize가 된다)
		// (SPKAppIndex의 0 --> fpOrg-2(size부분)가 된다.)
		//---------------------------------------------------------
		std::ifstream spkInputFile(pSPKFilenameOrg, ios::binary );
		std::ifstream spkInputFileApp(pSPKFilenameApp, ios::binary );
		std::ifstream spkiInputFileApp(pSPKIFilenameApp, ios::binary );

		TYPE_SPRITEID orgSize = 0;			// 원래 개수
		TYPE_SPRITEID appSize = 0;			// 추가할 개수
		TYPE_SPRITEID orgiSize = 0;			// 원래 개수
		TYPE_SPRITEID appiSize = 0;			// 추가할 개수
		long orgFP = 0;						// 원래 file의 크기
		long appFP = 0;						// 원래 file의 크기

		if (!spkInputFileApp.is_open()
			|| !spkiInputFileApp.is_open())	// 추가할게 없는 경우
		{
			return false;
		}

		//---------------------------------------------------------
		// Sprite의 개수를 알아낸다.
		//---------------------------------------------------------
		if (spkInputFile.is_open())
		{				
			spkInputFile.read((char*)&orgSize, 2);		

			spkInputFile.seekg( 0, ios::end );		
			orgFP = spkInputFile.tellg();

			spkInputFile.close();		
		}

		//---------------------------------------------------------
		// Sprite App의 개수를 알아낸다.
		//---------------------------------------------------------
		spkInputFileApp.read((char*)&appSize, 2);
		spkiInputFileApp.read((char*)&appiSize, 2);

		spkInputFileApp.seekg( 0, ios::end );		
		appFP = spkInputFileApp.tellg();

		spkInputFileApp.close();
		spkiInputFileApp.close();

		if (appSize <= 0
			|| appSize!=appiSize)		
		{
			return false;
		}

		//-----------------------------------------------------------------------------
		// SpritePack Append
		//-----------------------------------------------------------------------------
		AppendPatch ap;
		
		ap.SetOriginalInfo( pSPKFilenameOrg, orgFP, orgSize );
		ap.AddAppendInfo( pSPKFilenameApp, appFP, appSize );
		ap.SetFinalInfo( orgFP+appFP-2, orgSize+appSize );

		int errorCode = 0;	
		
		g_bUseProgressBar = false;	// execute patch할때 dialog 안 띄우기 - -;

		if (ap.ExecutePatch())
		{
			if (!ap.CheckFinalInfo())
			{
				errorCode = 2;
			}
		}
		else
		{
			errorCode = 1;
		}

		g_bUseProgressBar = true;

		if (errorCode)
		{
			DEBUG_ADD_FORMAT("[Error] GuildMarkPatch Terrible! code=%d", errorCode); 
		}

		//-----------------------------------------------------------------------------
		// GuildMark.spki 를 수정해서 추가한다.
		//-----------------------------------------------------------------------------
		std::ofstream spkiFileOrg(pSPKIFilenameOrg, ios::binary | ios::ate);
		std::ifstream spkiFileApp(pSPKIFilenameApp, ios::binary );

		TYPE_SPRITEID newSize = orgSize + appSize;

		// 개수 수정
		spkiFileOrg.seekp( 0, ios::beg );
		spkiFileOrg.write((char*)&newSize, 2);
		
		spkiFileOrg.seekp( 0, ios::end );		// 끝으로
		spkiFileApp.seekg( 2 );					// size부분 건너띄고 fp 부분의 처음으로

		long fp;
		long addFP = orgFP - 2;					// size부분 빼고 원래 화일(org) 만큼 뒤로.
		for (int i=0; i<appiSize; i++)
		{
			spkiFileApp.read((char*)&fp, 4);

			fp += addFP;

			spkiFileOrg.write((char*)&fp, 4);
		}

		spkiFileOrg.close();
		spkiFileApp.close();
		
		//-----------------------------------------------------------------------------
		// MapperOrg, MapperApp를 load하고
		//
		// MapperApp의 각 spriteID를 spriteID + maxSpkSize로 수정해서
		// MapperOrg에 추가(수정)한다.
		//-----------------------------------------------------------------------------
		MGuildInfoMapper mapperOrg;
		MGuildInfoMapper mapperApp;

		std::ifstream mapperFileOrg(pIDMapperFilenameOrg, ios::binary );
		std::ifstream mapperFileApp(pIDMapperFilenameApp, ios::binary );

		if (mapperFileOrg.is_open())
		{
			mapperOrg.LoadFromFile( mapperFileOrg );
			mapperFileOrg.close();
		}

		if (mapperFileApp.is_open())
		{
			mapperApp.LoadFromFile( mapperFileApp );
			mapperFileApp.close();
		}

		// app를 org에 추가한다.
		MGuildInfoMapper::const_iterator iMapper = mapperApp.begin();

		while (iMapper != mapperApp.end())
		{
			WORD guildID				= iMapper->first;
			GUILD_INFO*		pAppInfo	= iMapper->second;
			TYPE_SPRITEID	newSpriteID = pAppInfo->GetSpriteID() + orgSize;

			// 있는지 체크..
			GUILD_INFO* pInfo = mapperOrg.Get(guildID);

			if (pInfo!=NULL)
			{
				// sprite ID 변경
				pInfo->SetSpriteID( newSpriteID );
			}
			else
			{
				// 없으면 생성			
				pInfo = new GUILD_INFO;

				*pInfo = *pAppInfo;

				// sprite ID 변경
				pInfo->SetSpriteID( newSpriteID );

				mapperOrg.Set( guildID, pInfo );
			}		

			iMapper ++;
		}

		// MapperOrg를 다시 저장한다.
		std::ofstream mapperOutputFileOrg(pIDMapperFilenameOrg, ios::binary);
		mapperOrg.SaveToFile( mapperOutputFileOrg );
		mapperOutputFileOrg.close();

		//-----------------------------------------------------------------------------
		// SPKApp, MapperApp를 지운다.
		//-----------------------------------------------------------------------------
		//remove(pSPKFilenameApp);		// append하면서 지워진다.
		remove(pSPKIFilenameApp);
		remove(pIDMapperFilenameApp);
	#endif

	return true;
}


////----------------------------------------------------------------------
//// Load GradeMark
////----------------------------------------------------------------------
//bool		
//MGuildMarkManager::LoadGradeMark(WORD gradeSpriteID)
//{
//	// 이미 있는 경우는 리턴
//	if(m_GradeMarks[gradeSpriteID].IsInit())
//		return true;
//
//	MString *filename = NULL;
//	int fileSpriteID = 0;
//
//	// 뱀파이어와 슬레이어의 GradeMark가 따로따로 되있다.
//	if(gradeSpriteID < GRADE_MARK_MAX*2)
//	{
//		filename = &m_SlayerGradeMarkSPKFilename;
//		fileSpriteID = gradeSpriteID;
//	}
//	else
//	{
//		filename = &m_VampireGradeMarkSPKFilename;
//		fileSpriteID = gradeSpriteID - GRADE_MARK_MAX*2;
//	}
//
//	// 파일이름이 없으면 retuen false;
//	if(filename->GetLength() == 0)
//		return false;
//	
//	//---------------------------------------------------------
//	// file이름 준비..
//	//---------------------------------------------------------
//	char spkiFilename[256];
//	
//	// filename.spki
//	strcpy(spkiFilename, filename->GetString());
//	strcat(spkiFilename, "i");
//	
//	m_GradeMarks.LoadFromFileSprite(gradeSpriteID, fileSpriteID, filename->GetString(), spkiFilename);
//
//	return true;
//}

//----------------------------------------------------------------------
// Get GradeMark
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetGradeMark(WORD gradeID, Race race)
{
	if(race >= RACE_MAX)
		return NULL;

	return &m_GradeMarks[race][gradeID*2];
}

//----------------------------------------------------------------------
// Get GradeMarkSmall
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetGradeMarkSmall(WORD gradeID, Race race)
{
	if(race >= RACE_MAX)
		return NULL;
	
	return &m_GradeMarks[race][gradeID*2+1];
}

//----------------------------------------------------------------------
// Get LevelMark
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetLevelMark(WORD level)
{
	return &m_LevelMarks[level*2];
}

//----------------------------------------------------------------------
// Get LevelMarkSmall
//----------------------------------------------------------------------
CSprite*	
MGuildMarkManager::GetLevelMarkSmall(WORD level)
{
	return &m_LevelMarks[level*2+1];
}



bool		
MGuildMarkManager::CreateGuildMark(const char* pFilename, CSprite *&pSprite, CSprite *&pSpriteSmall)
{
	if (pFilename==NULL)
	{
		return false;
	}

	//-----------------------------------------------------
	// pFilename을 Surface에 load한다.
	//-----------------------------------------------------
	CDirectDrawSurface bmpSurface;
	if (!LoadImageToSurface(pFilename, bmpSurface))
	{
		return false;
	}
	
	const POINT bigSize = { 40, 40 };
	const POINT smallSize = { 20, 20 };
	
	CDirectDrawSurface surface;
	surface.InitOffsurface( bigSize.x, bigSize.y, DDSCAPS_SYSTEMMEMORY );
		
	RECT destBigRect = { 0, 0, bigSize.x, bigSize.y };
	RECT destSmallRect = { 0, 0, smallSize.x, smallSize.y };
	
	//-----------------------------------------------------
	// CSprite생성
	//-----------------------------------------------------
	if (CDirectDraw::Is565())
	{
		pSprite = new CSprite565;
		pSpriteSmall = new CSprite565;
	}
	else
	{
		pSprite = new CSprite555;
		pSpriteSmall = new CSprite555;
	}

	//-----------------------------------------------------
	// surface --> CSprite
	//-----------------------------------------------------
	WORD* lpSurface;
	unsigned short pitch;

	RECT bmpRect = { 0, 0, bmpSurface.GetWidth(), bmpSurface.GetHeight() };

	// 파란색이 투명색이다.
	CSprite::SetColorkey( 0x001F );

	// BigSize
	surface.FillSurface( 0 );
	surface.Blt(&destBigRect, &bmpSurface, &bmpRect);
	surface.LockW(lpSurface, pitch);
	pSprite->SetPixel(lpSurface, pitch, bigSize.x, bigSize.y);
	surface.Unlock();	

	// SmallSize
	surface.FillSurface( 0 );
	surface.Blt(&destSmallRect, &bmpSurface, &bmpRect);
	surface.LockW(lpSurface, pitch);				
	pSpriteSmall->SetPixel(lpSurface, pitch, smallSize.x, smallSize.y);
	surface.Unlock();	

	//-----------------------------------------------------
	// add
	//-----------------------------------------------------
//	AddGuildMark(guildID, pSprite, pSpriteSmall);
	return true;
}