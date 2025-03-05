//----------------------------------------------------------------------
// MGuildMarkManager.h
//----------------------------------------------------------------------
/*

  [ file 에서 ]

	CSpritePack			guildMarkSPK;			// 길드 마크 그림들
	SpriteIDMapper		guildMarkSpriteMapper;	// 길드ID --> SpriteID로 mapping

	* 길드ID를 guildMarkSpriteMapper에서 찾으면.. 관련된 SpriteID를 알 수 있다.
	  guildMarkSPKIndex에서 그 SpriteID로 그 길드Sprite의 SpriteIndex를 
	  알 수 있으므로.. 바로 Load해올 수 있다.

	* 저장할때는.. guildMarkSPK(+SPKIndex)에 추가하고 그 때의 ID를
	  그 길드ID와 연결해서 guildMarkSpriteMapper에 추가해두면 된다.

	* guildMarkSpriteMapper에 mapping이 되지 않는 guildMarkSprite는 버려진다.
	  용량이 그리 크지 않으므로 정리하는 시간을 줄이는게 낫다고 판단해서이다.

	* 클라이언트마다 guildMarkSPK와 guildMarkSpriteMapper는 다를 수 있다.



  [ memory 에서 ]

	map< 길드ID, 길드마크Sprite* >

	// 길드 마크를 생성하는 경우(
	bool		CreateGuildMark(길드ID, *.bmp || *.jpg);

	// 길드 마크를 추가하는 경우
	bool		AddGuildMark(길드ID, CSprite*);

	// 길드 마크를 출력하려고 sprite를 얻는 경우
	CSprite*	GetGuildMark(길드ID);

	// 길드 마크를 화일에서 읽고/쓰기
	bool		LoadGuildMark(길드ID);
	bool		SaveGuildMark(길드ID);



	//-------------------------------------------------
	// guild mark 출력 예제
	//-------------------------------------------------
	int guildID = ...

	//-------------------------------------------------
	// load되어 있는지 본다.
	//-------------------------------------------------
	CSprite* pSprite = g_pGuildMarkManager->GetGuildMark(guildID);

	if (pSprite==NULL)
	{		
		//-------------------------------------------------
		// file에 있는지 본다.
		//-------------------------------------------------
		g_pGuildMarkManager->LoadGuildMark(guildID);

		//-------------------------------------------------
		// file에서 load되었는지 다시 체크
		//-------------------------------------------------
		pSprite = g_pGuildMarkManager->GetGuildMark(guildID);

		//-------------------------------------------------
		// file에도 없는 경우..
		// guildMark관리서버?에서 받아온다.
		//-------------------------------------------------
		if (pSprite==NULL)
		{
			// 여기서는.. 임시로 화일에서 생성한다.
			char str[256];
			sprintf(str, "Data\\Guild\\Guild%d.bmp", guildID);
			g_pGuildMarkManager->CreateGuildMark( guildID, str );
		}
	}
	else
	{			
		// 작은거도 걍 함 찍어본다.
		CSprite* pSpriteSmall = g_pGuildMarkManager->GetGuildMarkSmall(guildID);

		m_pSurface->Lock();

		POINT point = { 100, 100 };

		m_pSurface->BltSprite(&point, pSprite);

		if (pSpriteSmall!=NULL)
		{
			point.x = 150;
			point.y = 100;
			m_pSurface->BltSprite(&point, pSpriteSmall);
		}

		m_pSurface->Unlock();
	}	
*/
//----------------------------------------------------------------------

#ifndef __MGUILD_MARK_MANAGER_H__
#define __MGUILD_MARK_MANAGER_H__

#pragma warning(disable:4786)

#include "CSprite.h"
#include "CSpritePack.h"
#include "MString.h"
#include "RaceType.h"

#define GRADE_MARK_MAX 50
//----------------------------------------------------------------------
// MGuildMarkManager
//----------------------------------------------------------------------
class MGuildMarkManager {
	public :
		struct GUILD_SPRITES
		{
			GUILD_SPRITES(CSprite* _pSprite=NULL, CSprite* _pSpriteSmall=NULL)
			{
				pSprite = _pSprite;
				pSpriteSmall = _pSpriteSmall;
			}

			CSprite* pSprite;
			CSprite* pSpriteSmall;
		};

		typedef std::map<WORD, GUILD_SPRITES>		GUILDMARK_MAP;
		
	public :
		MGuildMarkManager(	const char* pGuildMarkSPKFilename, 
							const char* pGuildMarkSpriteIDMapperFilename,
							const char *pSlayerGradeMarkSPKFilename,
							const char *pVampireGradeMarkSPKFilename,
							const char *pOustersGradeMarkSPKFilename,
							const char *pLevelMarkSPKFilename
							);
		~MGuildMarkManager();

		bool			IsInit() const		{ return (m_GuildMarkSPKFilename.GetLength()+m_GuildMarkSpriteMapperFilename.GetLength())!=0; }

		//----------------------------------------------------------
		// Release
		//----------------------------------------------------------
		void			Release();

		//----------------------------------------------------------
		// Create( guildID, pFilename은 *.bmp || *.jpg )
		//----------------------------------------------------------
		bool			CreateGuildMark(WORD guildID, const char* pFilename);
		static bool		CreateGuildMark(const char* pFilename, CSprite *&pSprite, CSprite *&pSpriteSmall);

		//----------------------------------------------------------
		// Add / Get
		//----------------------------------------------------------
		bool			HasGuildMark(WORD guildID) const;
		bool			AddGuildMark(WORD guildID, CSprite* pSprite, CSprite* pSpriteSmall);
		CSprite*		GetGuildMark(WORD guildID);
		CSprite*		GetGuildMarkSmall(WORD guildID);

		CSprite*		GetGradeMark(WORD gradeID, Race race);
		CSprite*		GetGradeMarkSmall(WORD gradeID, Race race);

		CSprite*		GetLevelMark(WORD level);
		CSprite*		GetLevelMarkSmall(WORD level);

		//----------------------------------------------------------
		// File I/O
		//----------------------------------------------------------
		bool			LoadGuildMark(WORD guildID);
		bool			SaveGuildMark(WORD guildID, CSprite* pSprite, CSprite* pSpriteSmall);
		static bool		MergeGuildMark(const char* pSPKFilenameOrg, const char* pIDMapperFilenameOrg,
										const char* pSPKFilenameApp, const char* pIDMapperFilenameApp);
//		bool			LoadGradeMark(WORD gradeSpriteID);

	protected :
		MString						m_GuildMarkSPKFilename;
		MString						m_GuildMarkSpriteMapperFilename;
		GUILDMARK_MAP				m_GuildMarks;

//		MString						m_SlayerGradeMarkSPKFilename;
//		MString						m_VampireGradeMarkSPKFilename;
		CSpritePack					m_GradeMarks[RACE_MAX];
		CSpritePack					m_LevelMarks;
};

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
extern MGuildMarkManager* g_pGuildMarkManager;

#endif

