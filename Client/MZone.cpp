//----------------------------------------------------------------------
// MZone.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include <math.h>
#include "MZoneDef.h"
#include "MSector.h"
#include "MCreature.h"
#include "MFakeCreature.h"
#include "MPlayer.h"
#include "MImageObject.h"
#include "MShadowObject.h"
#include "MAnimationObject.h"
#include "MShadowAnimationObject.h"
#include "MInteractionObject.h"
#include "MPortal.h"
#include "MItem.h"
#include "MEffect.h"
#include "MZone.h"
#include "MTopView.h"
#include "UserOption.h"
#include "MEffectGeneratorTable.h"
#include "MZoneTable.h"
#include "CMessageArray.h"
#include "DebugInfo.h"
#include "ServerInfo.h"
#include "Client.h"
#include "EffectSpriteTypeDef.h"
#include "SoundDef.h"
#include "MEffectSpriteTypeTable.h"
#include "MHelicopterManager.h"
#include "MParty.h"
#include "MTestDef.h"
#include <fstream>
#include "SkillDef.h"
#include "mintr.h"
#include "MNPC.h"
#include "MGuildType.h"
#include "MEffectStatusDef.h"

#include "VS_UI.h"
#include "MHelpDef.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MZone*				g_pZone				= NULL;

BOOL g_bPlayPropeller = FALSE;

extern HWND					g_hWnd;
extern DWORD				g_ZoneRandomSoundTime;

extern void		SendPositionInfoToParty();
extern void		SendStatusInfoToParty();
//GameNew Mode  add by sonc 2006.9.27
extern	LONG g_SECTOR_WIDTH;
extern	LONG g_SECTOR_HEIGHT;
extern	LONG g_SECTOR_WIDTH_HALF;
extern	LONG g_SECTOR_HEIGHT_HALF;
extern	LONG g_SECTOR_SKIP_PLAYER_LEFT;
extern	LONG g_SECTOR_SKIP_PLAYER_UP;
extern	LONG g_TILESURFACE_SECTOR_WIDTH;
extern	LONG g_TILESURFACE_SECTOR_HEIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_DOWN;
extern	LONG g_TILESURFACE_WIDTH;
extern	LONG g_TILESURFACE_HEIGHT;
extern	LONG g_TILESURFACE_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_OUTLINE_DOWN;
//end 
#ifdef __METROTECH_TEST__
	extern int		g_iSpeed;
#endif

#ifdef	OUTPUT_DEBUG
	//extern CMessageArray		g_DebugMessage;
	#ifdef __METROTECH_TEST__
//		#define OUTPUT_DEBUG_UPDATE_ZONE
	#endif
//	#define OUTPUT_DEBUG_UPDATE_EFFECT
#endif

// ImageObject에 대한 정보 출력
//#ifdef __OUTPUT_IMAGEOBJECT__

#define	VISION_SECTOR_WIDTH_HALF				12
#define	VISION_SECTOR_HEIGHT_HALF				13

#define NEW_VISION_SECTOR_WIDTH_HALF			10
#define NEW_VISION_SECTOR_HEIGHT_HALF_UP		14
#define NEW_VISION_SECTOR_HEIGHT_HALF_DOWN		14

//----------------------------------------------------------------------
// Is RelicTable [성물수정]
//----------------------------------------------------------------------
bool
IsRelicTable( MItem* pItem )
{
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();
		
		return pCreature->GetCreatureType()>=371 && pCreature->GetCreatureType()<=376 ||
				pCreature->GetCreatureType()>=560 && pCreature->GetCreatureType()<=563 ||
				pCreature->GetCreatureType()>=526 && pCreature->GetCreatureType()<=549;
	}

	return false;
}

//----------------------------------------------------------------------
// define function ㅋㅋ
//----------------------------------------------------------------------
#define	CheckCreatureInDarkness( sector, x, y )						\
		{															\
			if (g_pPlayer->GetX()==x && g_pPlayer->GetY()==y)		\
			{														\
				g_pPlayer->CheckInDarkness();						\
			}														\
																	\
			int numCreature = sector.GetCreatureSize();				\
																	\
			if (numCreature > 0)									\
			{														\
				OBJECT_MAP::iterator iCreature = sector.GetCreatureIterator();	\
																		\
				for (int i=0; i<numCreature; i++, iCreature ++)			\
				{														\
					if (iCreature==sector.GetObjectEnd()					\
						|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)	\
						break;												\
																			\
					MCreature* pCreature = (MCreature*)iCreature->second;	\
																		\
					if (pCreature!=NULL)								\
					{													\
						pCreature->CheckInDarkness();					\
					}													\
				}														\
			}															\
																		\
			if (sector.IsExistItem())									\
			{															\
				MItem* pItem = sector.GetItem();						\
																		\
				if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)	\
				{														\
					MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();	\
																		\
					if (pCreature!=NULL)								\
					{													\
						pCreature->CheckInDarkness();					\
					}													\
				}														\
			}															\
		}
				
//----------------------------------------------------------------------
// RemoveSectorEffect [새기술9]
//----------------------------------------------------------------------
#define RemoveSectorEffect( sX, sY, id )						\
		{														\
			MSector& sector = m_ppSector[sY][sX];				\
																\
			bool bSanctuary = sector.HasSanctuary();			\
			sector.RemoveEffect( id );							\
																\
			if (bSanctuary && !sector.HasSanctuary())			\
			{													\
				for (int i=-1; i<=1; i++)						\
				{												\
					for (int j=-1; j<=1; j++)					\
					{											\
						int x0 = sX + i;						\
						int y0 = sY + j;						\
																\
						if (i==0 && j==0						\
							|| x0<0 || x0>=m_Width				\
							|| y0<0 || y0>=m_Height)			\
						{										\
							continue;							\
						}										\
																\
						MSector& sector2 = m_ppSector[y0][x0];	\
						sector2.UnSetSanctuary();				\
					}											\
				}												\
			}													\
		}


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

MZone::MZone(TYPE_SECTORPOSITION width, TYPE_SECTORPOSITION height)
{
	m_Width = 0;
	m_Height =0;

	m_ppSector	= NULL;
	//m_pPlayer	= NULL;

	Init(width, height);

	m_bZoneSoundLoaded = false;

	DEBUG_ADD("Load HornInfo");
	int zoneNum, portalNum;
	UI_PORTAL_FLAG portal;
	UI_PORTAL_LIST portalList;

	ifstream hornInfoFile(g_pFileDef->getProperty("FILE_INFO_HORN").c_str(), ios::binary);
	hornInfoFile.read((char *)&zoneNum, sizeof(int));
	for(int zone = 0; zone < zoneNum; zone++)
	{
		hornInfoFile.read((char *)&portalNum, sizeof(int));
		for(int p = 0; p < portalNum; p++)
		{
			hornInfoFile.read((char *)&portal.zone_id, sizeof(int));
			hornInfoFile.read((char *)&portal.x, sizeof(int));
			hornInfoFile.read((char *)&portal.y, sizeof(int));
			hornInfoFile.read((char *)&portal.portal_x, sizeof(int));
			hornInfoFile.read((char *)&portal.portal_y, sizeof(int));
			portalList.push_back(portal);
		}
		m_horn.push_back(portalList);
		portalList.clear();
	}
	hornInfoFile.close();
	DEBUG_ADD("Load HornInfo OK");

}

MZone::~MZone()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
MZone::Init(TYPE_SECTORPOSITION width, TYPE_SECTORPOSITION height)
{
	// 0 이 있으면 return
	if (width==0 || height==0) 
		return;

	// memory해제
	Release();

	m_Width	 = width;
	m_Height = height;

	m_ppSector = new MSector* [m_Height];

	int i;
	for (i=0; i<m_Height; i++)
	{
		m_ppSector[i] = new MSector [m_Width];
	}

	//for (i=0; i<m_Height; i++)
	//{
	//	for (int j=0; j<m_Width; j++)
	//	{
	//		m_ppSector[i][j].SetFilterSpriteID( 0 );
	//	}
	//}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MZone::Release()
{
	//---------------------------------
	// player 위치 제거
	//---------------------------------
	//m_pPlayer = NULL;

	DEBUG_ADD( "Release ImageObject" );
	
	//---------------------------------
	// ImageObject 제거
	//---------------------------------
	IMAGEOBJECT_MAP::iterator iImageObject = m_mapImageObject.begin();

	MImageObject *pImageObject = NULL;
	while (iImageObject != m_mapImageObject.end())
	{
		pImageObject = (*iImageObject).second;
		if (pImageObject!=NULL)
		{
			delete pImageObject;					
		}
		iImageObject++;
	}
	m_mapImageObject.clear();


	DEBUG_ADD( "Start Release Object" );
	
	//---------------------------------
	// object들 제거
	//---------------------------------
	ReleaseObject();

	//---------------------------------
	// Obstacle 제거
	//---------------------------------
	/*
	OBSTACLE_LIST::iterator iObstacle = m_listObstacle.begin();

	MObstacle *pObstacle = NULL;
	while (iObstacle != m_listObstacle.end())
	{
		pObstacle = *iObstacle;
		if (pObstacle != NULL)
			delete pObstacle;
		iObstacle++;
	}
	m_listObstacle.clear();
	*/

	DEBUG_ADD_FORMAT( "MZone::Release() - %d x %d", m_Width, m_Height);

	//---------------------------------
	// sector 제거
	//---------------------------------	
	if (m_ppSector!=NULL)
	{
		int i;
		for (i=0; i<m_Height; i++)
		{
			delete [] m_ppSector[i];			
		}

		delete [] m_ppSector;		
		
		m_ppSector=NULL;
		m_Width = 0;
		m_Height =0;
	}


	m_bZoneSoundLoaded = false;
	DEBUG_ADD("zone rel ok");
}

//----------------------------------------------------------------------
// Release Object
//----------------------------------------------------------------------
// instance object들을 제거한다.
//----------------------------------------------------------------------
void
MZone::ReleaseObject()
{
	m_HelicopterManager.Release();

	//---------------------------------
	// map에 있는 Creature제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseCreature. size=%d", m_mapCreature.size());
	
	CREATURE_MAP::iterator iCreature;
	iCreature = m_mapCreature.begin();

	MCreature *pCreature = NULL;
	while (iCreature != m_mapCreature.end())
	{
		pCreature = (*iCreature).second;
		// player는 지우지 않는다.
		if (pCreature!=NULL && pCreature->GetClassType()!=MCreature::CLASS_PLAYER)
		{
			DEBUG_ADD_FORMAT("RemoveCreature: [%s] id=%d, (%d, %d)", pCreature->GetName(), pCreature->GetID(), pCreature->GetX(), pCreature->GetY());
			
			/*
			if (g_pParty!=NULL && pCreature->IsPlayerParty())
			{
				PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );

				if (pInfo!=NULL)
				{
					pInfo->bInSight = false;					
					pInfo->zoneID = 60002;
				}
			}
			*/
			
			// sector에서 제거
			if (m_ppSector!=NULL)
			{
				int id = pCreature->GetID();
				int x = pCreature->GetX();
				int y = pCreature->GetY();
				int serverX = pCreature->GetServerX();
				int serverY = pCreature->GetServerY();

				UnSetServerBlock(pCreature->GetMoveType(), serverX, serverY);

				//if (x>=0 && y>=0 && x<m_Width && y<m_Height) 
				{
					//------------------------------------------------
					// sector에서 제거시킨다.
					//------------------------------------------------
					if (!m_ppSector[y][x].RemoveCreature(id))
					{
						if (!m_ppSector[serverY][serverX].RemoveCreature(id))
						{	
							DEBUG_ADD_FORMAT("Can't RemoveCreatureWhenRelease! ID=%d client(%d,%d), server(%d,%d)", pCreature->GetID(), x,y,serverX,serverY);
						}
					}

					/*
					bool removed = m_ppSector[y][x].RemoveGroundCreature( id );
					if (!removed)
					{
						removed = m_ppSector[serverY][serverX].RemoveGroundCreature( id );

						if (!removed)
						{
							removed = m_ppSector[y][x].RemoveFlyingCreature( id );

							if (!removed)
							{
								removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( id );

								if (!removed)
								{
									removed = m_ppSector[y][x].RemoveUndergroundCreature( id );

									if (!removed)
									{
										removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( id );

										DEBUG_ADD_FORMAT("[Error] Can't Remove Creature. ID=%d. xy=(%d, %d), sxy=(%d, %d)", id, x, y, serverX, serverY);
									}
								}
							}
						}
					}
					*/
				}
			}
		
			DEBUG_ADD("RemoveCreature: OK");
			
			delete pCreature;

			DEBUG_ADD("DeleteCreature: OK");
		}
		iCreature++;
	}
	m_mapCreature.clear();

	//---------------------------------
	// map에 있는 FakeCreature제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseFakeCreature. size=%d", m_mapFakeCreature.size());
	
	CREATURE_MAP::iterator iFakeCreature= m_mapFakeCreature.begin();

	while (iFakeCreature != m_mapFakeCreature.end())
	{
		pCreature = (*iFakeCreature).second;
		if (pCreature!=NULL)
		{
			delete pCreature;					
		}
		iFakeCreature++;
	}
	m_mapFakeCreature.clear();


	//---------------------------------
	// map에 있는 Item제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseItem. size=%d", m_mapItem.size());
	
	ITEM_MAP::iterator iItem = m_mapItem.begin();

	MItem *pItem = NULL;
	while (iItem != m_mapItem.end())
	{
		pItem = (*iItem).second;
		if (pItem!=NULL)
		{
			if (m_ppSector!=NULL)
			{
				// [성물수정]
				MSector& sector = m_ppSector[pItem->GetY()][pItem->GetX()];
				sector.RemoveItem();

				// 혹시 문제가 있을까봐.. 시체 정리..
				if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
				{
					sector.UnSetBlockServerGround();

					// [성물수정]
					if (IsRelicTable(pItem))
					{
						sector.UnSetBlockGround();
						sector.UnSetBlockFlying();
					}
				}
			}

			delete pItem;					
		}
		iItem++;
	}
	m_mapItem.clear();

	//---------------------------------
	// Effect 제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseEffect. size=%d", m_mapEffect.size());
	
	EFFECT_MAP::iterator iEffect = m_mapEffect.begin();

	MEffect *pEffect = NULL;
	while (iEffect != m_mapEffect.end())
	{
		pEffect = iEffect->second;
		if (pEffect != NULL)
		{
			if (m_ppSector!=NULL)
			{
				// [새기술9]
				RemoveSectorEffect( pEffect->GetX(), pEffect->GetY(), pEffect->GetID() );				
			}

			delete pEffect;			
		}
		iEffect++;
	}
	m_mapEffect.clear();

	//---------------------------------
	// Ground Effect 제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseGroundEffect. size=%d", m_mapGroundEffect.size());
	
	iEffect = m_mapGroundEffect.begin();

	while (iEffect != m_mapGroundEffect.end())
	{
		pEffect = iEffect->second;
		if (pEffect != NULL)
		{	
			delete pEffect;			
		}
		iEffect++;
	}
	m_mapGroundEffect.clear();


	//---------------------------------
	// Sound 제거
	//---------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseSound. size=%d", m_listSoundNode.size());
	
	SOUND_NODE_LIST::iterator iSound = m_listSoundNode.begin();

	while (iSound != m_listSoundNode.end())
	{
		SOUND_NODE*	pNode = *iSound;

		// 지운다.
		delete pNode;
		
		iSound++;			
	}

	m_listSoundNode.clear();

	// 2004, 11, 29, sobeit add start
	//-----------------------------------------------
	// wait effect list 삭제...
	//-----------------------------------------------
	DEBUG_ADD_FORMAT("MZone::ReleaseWaitEffect. size=%d", m_listWaitEffect.size());
	WAIT_EFFECT_LIST::iterator iWaitEffect = m_listWaitEffect.begin();

	while (iWaitEffect != m_listWaitEffect.end())
	{
		MEffect*	pNode = *iWaitEffect;

		delete pNode;

		iWaitEffect ++;
	}
	m_listWaitEffect.clear();
	// 2004, 11, 29, sobeit add end
	
	DEBUG_ADD("MZone::Release OK");
}

//----------------------------------------------------------------------
// Save To File SectorSound
//----------------------------------------------------------------------
// Width*Height
// SectorSoundInfo
// //MZoneSoundTable
//----------------------------------------------------------------------
bool		
MZone::SaveToFileSectorSound(std::ofstream& file)
{
	//-----------------------------------------------------------
	// 크기
	//-----------------------------------------------------------
	file.write((const char*)&m_Width, SIZE_SECTORPOSITION);
	file.write((const char*)&m_Height, SIZE_SECTORPOSITION);

	//-----------------------------------------------------------
	// SectorSoundInfo
	//-----------------------------------------------------------
	for (int x=0; x<m_Width; x++)
	{
		for (int y=0; y<m_Height; y++)
		{
			const SECTORSOUND_LIST& listSound = m_ppSector[y][x].GetSectorSoundList();

			SECTORSOUND_LIST::const_iterator iSound = listSound.begin();

			int num = listSound.size();

			// 한 Sector에 있는 sound의 개수 저장
			file.write((const char*)&num, 4);

			// 각 sound저장
			while (iSound != listSound.end())
			{
				const SECTORSOUND_INFO& info = *iSound;

				info.SaveToFile( file );

				iSound++;
			}			
		}
	}

	/*
	//-----------------------------------------------------------
	// MZoneSoundTable
	//-----------------------------------------------------------
	// 좀.. 뭐하지만.. - -;
	// 암튼 편의상(-_-;) 같이 넣어버린다. ㅋㅋ..
	//-----------------------------------------------------------
	bool bExistZoneTable = (g_pZoneTable!=NULL);

	// 체크용..
	file.write((const char*)&bExistZoneTable, 1);

	if (bExistZoneTable)
	{
		g_pZoneTable->SaveToFile( file );

		return true;
	}	

	return false;
	*/

	return true;
}

//----------------------------------------------------------------------
// Load From File SectorSound
//----------------------------------------------------------------------
// Width*Height로 크기 체크하고.
// SectorSoundInfo를 Loading하고
// //MZoneSoundTable을 Loading한다.
//----------------------------------------------------------------------
bool		
MZone::LoadFromFileSectorSound(std::ifstream& file)
{
	m_bZoneSoundLoaded = false;

	/*
	//---------------------------------------------------------------
	// Sector에 사운드 정보를 Loading한다.
	//---------------------------------------------------------------
	// test code
	for (int x=0; x<m_Width; x++)
	{
		for (int y=0; y<m_Height; y++)
		{
			// ZoneSound 1번이 50,50에서 소리난다고 모든 sector에 정보 추가
			// 사실 어느 정도 거리가 안되면 추가할 필요는 없다.
			// 일단 귀찮아서 테스트로.. - -;;
			//		DEBUG_ADD_FORMAT("AddSectorSound[%d][%d]", y, x);
			
		
			m_ppSector[y][x].AddSectorSound( 1, 30, 30 );		
		}
	}
	*/

	//-----------------------------------------------------------
	// 크기
	//-----------------------------------------------------------
	//TYPE_SECTORPOSITION width, height;

	//file.read((char*)&width, SIZE_SECTORPOSITION);
	//file.read((char*)&height, SIZE_SECTORPOSITION);

	//if (m_Width!=width || m_Height!=height)
	{
		// Zone크기와 SectorSound정보의 크기가 다른 경우
		// 절망이다 - -;
	//	return false;
	}

	//-----------------------------------------------------------
	// SectorSoundInfo
	//-----------------------------------------------------------
	//
	// 아래와 같이 정의하고..
	//
	//	 -         : 소리가 나지 않는 Sector
	//	 A,C,D     : 1개의 SECTORSOUND_INFO가 있는 Sector (각각 소리는 다름)
	//   B		   : 2개의 SECTORSOUND_INFO가 있는 Sector
	//   E         : 3개의 SECTORSOUND_INFO가 있는 Sector
	//   a~z       : SECTORSOUND_INFO
	//
	// 한줄을 예로 들자면..
	//
	// ex) -----AAABBBAAC--DEEEEE---
	//
	//	   2						// 한 줄에서의 반복회수 2회(-----AAABBBAAC, --DEEEEE )
	//                              // 한줄의 끝부분의 소리없는 sector는 무시하면 된다.
	//
	//      5						// 첫번째 반복. 소리없는 sector수(-----)
	//       4						// sound종류 ( AAA, BBB, AA, C )
	//        {3 { 1 a }}			// 3개의 sector( AAA )에  1개의 info
	//        {3 { 2 b, b' }}		// 3개의 sector( BBB )에  2개의 info
	//        {2 { 1 a }}			// 2개의 sector( AA )에  1개의 info
	//        {1 { 1 c }}			// 1개의 sector( C )에  1개의 info
	//
	//      2                       // 두번째 반복. 소리없는 sector수(--)
	//       2						// sound종류 ( D, EEEE )
	//        {1 { 1 d }}			// 1개의 sector( D )에  1개의 info
	//        {5 { 3 e, e', e" }}   // 5개의 sector( EEEEE )에 3개의 info	
	//
	//-----------------------------------------------------------
	BYTE				num;
	SECTORSOUND_INFO	info;

	for (int y=0; y<m_Height; y++)
	{
		//-----------------------------------------------------------
		// 반복 회수 : { 0의 개수, 사운드 종류수 * { info * 개수 } }
		//-----------------------------------------------------------
		file.read((char*)&num, 1);

		int x = 0;
		int numCount = num;
		
		//-----------------------------------------------------------
		// 반복회수만큼..
		//-----------------------------------------------------------
		for (int i=0; i<numCount; i++)
		{
			//-----------------------------------------------------
			// 소리 안나는 sector수
			//-----------------------------------------------------
			file.read((char*)&num, 1);	
			x += num;	// 좌표 skip

			//-----------------------------------------------------
			// 이번 반복에서 소리나는 sector들에서 
			// 연속하지 않은 SECTORSOUND_INFO의 종류수
			//-----------------------------------------------------
			file.read((char*)&num, 1);	
			int numSoundType = num;

			//-----------------------------------------------------
			// SECTORSOUND_INFO의 종류수만큼 반복
			//-----------------------------------------------------
			for (int j=0; j<numSoundType; j++)
			{
				//-----------------------------------------------------
				// 같은 info를 가진 sector가 몇개나 연속해서 있는가?
				//-----------------------------------------------------
				file.read((char*)&num, 1);		// assert( num > 0 );
				int numSector = num;

				//-----------------------------------------------------
				// sector에 SECTORSOUND_INFO는 몇개나 있는가?
				//-----------------------------------------------------
				file.read((char*)&num, 1);		// assert( num > 0 );
				int numSound = num;

				//-----------------------------------------------------
				// sound를 읽으면서 sector들에 연속해서 info를 추가한다.
				//-----------------------------------------------------
				for (int n=0; n<numSound; n++)
				{
					//-----------------------------------------------------
					// 하나의 SECTORSOUND_INFO를 load
					//-----------------------------------------------------
					info.LoadFromFile( file );
					
					int xPlusNumSector = x + numSector;
					for (int k=x; k<xPlusNumSector; k++)
					{
						m_ppSector[y][k].AddSectorSound( info );
					}
				}

				x += numSector;
			}
		}
	}

	/*
	for (int x=0; x<m_Width; x++)
	{
		for (int y=0; y<m_Height; y++)
		{
			file.read((char*)&num, 1);

			#ifdef _DEBUG
				if (file.eof())
				{
					int a = 0;
				}
			#endif

			intNum = num;

			for (int n=0; n<intNum; n++)
			{
				info.LoadFromFile( file );

				#ifdef _DEBUG
					if (file.eof())
					{
						int a = 0;
					}
				#endif

				m_ppSector[y][x].AddSectorSound( info );			
			}
		}
	}
	*/
	

	//-----------------------------------------------------------
	// MZoneSoundTable
	//-----------------------------------------------------------
	// 좀.. 뭐하지만.. - -;
	// 암튼 편의상(-_-;) 같이 넣어버린다. ㅋㅋ..
	//-----------------------------------------------------------
	/*
	bool bExistZoneTable;

	// 체크용..
	file.read((char*)&bExistZoneTable, 1);

	if (bExistZoneTable)
	{
		g_pZoneTable->LoadFromFile( file );
		
		m_bZoneSoundLoaded = true;

		return true;
	}

	return false;
	*/

	m_bZoneSoundLoaded = true;
	return true;

}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
//   가로 size, 세로 size
//   Zone ID
//   속성
//   가로size * 세로size 개의 Sector정보(TileSpriteID, Flag)
//   Obstacle수,  Obstacle수 * Obstacle 정보
//   ImageObject수,  ImageObject수 * (ImageObject, ImageObjectSectorInfo)
//----------------------------------------------------------------------
bool		
MZone::LoadFromFile(std::ifstream& file)
{
	//-------------------------------------------------
	// Header
	//-------------------------------------------------
	m_Info.LoadFromFile( file );
	

	//-------------------------------------------------
	//
	// 5월 11일 version
	//
	//-------------------------------------------------
	if (m_Info.ZoneVersion==MAP_VERSION_2000_05_10)
	{
		//-------------------------------------------------
		// ZoneID 
		// 속성
		//-------------------------------------------------	
		file.read((char*)&m_fpTile, 4);				// Tile FP
		file.read((char*)&m_fpImageObject, 4);		// ImageObject FP

		//-------------------------------------------------
		// 이미 있던 것 제거
		//-------------------------------------------------
		Release();

		//-------------------------------------------------
		// Zone의 가로 Size, 
		//        세로 Size
		//-------------------------------------------------
		file.read((char*)&m_Width, 2);
		file.read((char*)&m_Height, 2);

		// 아무것도 없는 경우
		if (m_Width==0 || m_Height==0)
			return false;

		//-------------------------------------------------
		// Zone의 각 Sector들을 Load
		//-------------------------------------------------
		// memory잡기
		Init(m_Width, m_Height);

		///*
		int i,j;

		for (i=0; i<m_Height; i++)
		{
			for (j=0; j<m_Width; j++)
			{
				m_ppSector[i][j].LoadFromFile(file);			
			}
		}

		//*/
		/*
		struct MSector_st
		{
			TYPE_SPRITEID	spriteID;
			BYTE			property;
			BYTE			light;
		};

		MSector_st* tempSector = new MSector_st[m_Height*m_Width];

		file.read((char*)tempSector, sizeof(MSector_st)*m_Height*m_Width);

		int i,j;
		
		MSector_st* tempSectorPtr = tempSector;

		for (i=0; i<m_Height; i++)
		{
			for (j=0; j<m_Width; j++)
			{
				m_ppSector[i][j].Set( tempSectorPtr->spriteID, tempSectorPtr->property );
				// light는 의미없다.

				tempSectorPtr++;
			}
		}
		
		delete [] tempSector;
		*/

		//-------------------------------------------------
		// 빛에 따른 Filter 모양 생성
		//-------------------------------------------------
		// 이부분을 MapEditor에서??
		/*
		for (i=0; i<m_Height; i++)
		{
			for (j=0; j<m_Width; j++)
			{
				if (m_ppSector[i][j].GetLight()!=0)	
				{
					SetLight(j,i, 0);
				}
			}
		}
		*/
		

		int size;

		//-------------------------------------------------
		// Portal을 읽어들인다.
		//-------------------------------------------------
		// 2001.7.11에 제거
		/*
		file.read((char *)&size, 4);

		// Client에서는 필요없는 정보이므로 읽어들이기만 한다.
		// [!] 아예 file position을 기억해서 건너띌수도 있겠다.
		MPortal portal;
		for (i=0; i<size; i++)
		{
			portal.LoadFromFile( file );
		}
		*/

		//-------------------------------------------------
		// ImageObject 개수를 Load
		//-------------------------------------------------	
		file.read((char *)&size, 4);

		//-------------------------------------------------
		// Zone의 ImageObject들을 Load
		//-------------------------------------------------
		MImageObject				*pImageObject;
		IMAGEOBJECT_POSITION_LIST	ImageObjectPositionList;
		BYTE						ObjectType;
		for (i=0; i<size; i++)	
		{
			#ifdef __OUTPUT_IMAGEOBJECT__
				char str[1024];
			#endif

			// ImageObject memory를 잡고 Load한다.
			file.read((char*)&ObjectType, 1);

			switch (ObjectType)
			{
				case MObject::TYPE_IMAGEOBJECT :
					pImageObject = new MImageObject;
					
					#ifdef __OUTPUT_IMAGEOBJECT__	
						strcpy(str, "ImageObject : ");
					#endif
				break;

				case MObject::TYPE_SHADOWOBJECT :
					pImageObject = new MShadowObject;

					#ifdef __OUTPUT_IMAGEOBJECT__
						strcpy(str, "ShadowObject : ");
					#endif
				break;

				case MObject::TYPE_ANIMATIONOBJECT :
					pImageObject = new MAnimationObject;
					
					#ifdef __OUTPUT_IMAGEOBJECT__
						strcpy(str, "AnimationObject : ");
					#endif
				break;

				case MObject::TYPE_SHADOWANIMATIONOBJECT :
					pImageObject = new MShadowAnimationObject;
					
					#ifdef __OUTPUT_IMAGEOBJECT__
						strcpy(str, "ShadowAnimationObject : ");
					#endif
				break;

				case MObject::TYPE_INTERACTIONOBJECT :
					pImageObject = new MInteractionObject;
					
					#ifdef __OUTPUT_IMAGEOBJECT__
						strcpy(str, "InteractionObject : ");
					#endif
				break;
			}

			pImageObject->LoadFromFile(file);	
			
			#ifdef __OUTPUT_IMAGEOBJECT__
				sprintf(str, "%s[%d] vp=%d. ", str, pImageObject->GetImageObjectID(), pImageObject->GetViewpoint());
			#endif

			// ImageObject을 IMAGEOBJECT_MAP에 추가한다.
			AddImageObject(pImageObject);

			//-------------------------------------------------
			// 방금 Load한 ImageObject이 존재하는 
			// Sector들의 좌표를 Load해야한다.
			// (*) 이 정보는 더 이상 저장하고 있지 않는다.
			//-------------------------------------------------		
			ImageObjectPositionList.LoadFromFile(file);		
			
			//-------------------------------------------------------
			// Load한 ImageObjectPositionList의 각 Position에 대해서 
			// Zone에 ImageObjectSector를 표시해야 한다.
			//-------------------------------------------------------
			IMAGEOBJECT_POSITION_LIST::POSITION_LIST::const_iterator 
				iImageObjectPosition = ImageObjectPositionList.GetIterator();


			/*
			// File로 SpriteID가 61번인 것의 정보 출력
			char str[256];
			char str2[80];
			if (pImageObject->GetSpriteID()==61)
			{
				sprintf(str, "IO[%d] Pixel=(%d, %d)  View=%d  Position=", pImageObject->GetID(), pImageObject->GetPixelX(), pImageObject->GetPixelY(), pImageObject->GetViewpoint());
			}
			*/

			// 각 Sector에 ImageObject표시
			for (int j=0; j<ImageObjectPositionList.GetSize(); j++)
			{
				#ifdef __OUTPUT_IMAGEOBJECT__
					sprintf(str, "%s(%d,%d) ", str, (int)(*iImageObjectPosition).X, (int)(*iImageObjectPosition).Y);
				#endif

				SetImageObjectSector((*iImageObjectPosition).X, (*iImageObjectPosition).Y, pImageObject->GetID());

				/*
				// File로 SpriteID가 61번인 것의 정보 출력
				if (pImageObject->GetSpriteID()==61)
				{
					sprintf(str2, "(%d, %d)  ", (*iImageObjectPosition).X, (*iImageObjectPosition).Y);
					strcat(str, str2);			
				}
				*/

				iImageObjectPosition++;			
			}			

			#ifdef __OUTPUT_IMAGEOBJECT__
				if (g_pDebugMessage!=NULL)
					g_pDebugMessage->AddToFile( str );
			#endif
			/*
			// File로 SpriteID가 61번인 것의 정보 출력
			if (pImageObject->GetSpriteID()==61)
			{
				strcpy(g_pDebugMessage->GetCurrent(), str);
				g_pDebugMessage->Next();
			}
			*/
		}					
	}
	//-----------------------------------------------------------------
	// 다른 버전??
	//-----------------------------------------------------------------
	else
	{
		return false;
	}



	return true;
}

//----------------------------------------------------------------------
// Set Player 
//
// Player는 Zone의 중심에서 출력된다.
// Player를 Zone의 Sector에 위치시킨다.
//----------------------------------------------------------------------
void		
MZone::SetPlayer()//MPlayer* pPlayer)
{
	// Player Setting
	//m_pPlayer = pPlayer; 

	// Sector Setting
	//m_ppSector[m_pPlayer->GetY()][m_pPlayer->GetX()].AddGroundCreature(pPlayer);
	//m_ppSector[g_pPlayer->GetY()][g_pPlayer->GetX()].AddGroundCreature(&g_pPlayer->;

	// Player의 시야 안의 Sector를 보이게 한다.
	//SetSight(g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetLightSight());

	AddCreature( g_pPlayer );
	
	//---------------------------------------------------------------
	// 내 정보를 파티에게 보내준다.
	//---------------------------------------------------------------
	SendPositionInfoToParty();
	SendStatusInfoToParty();
}

//----------------------------------------------------------------------
// Set Player 
//
// Player는 Zone의 중심에서 출력된다.
// Player를 Zone의 Sector에 위치시킨다.
//----------------------------------------------------------------------
void		
MZone::RemovePlayer()//MPlayer* pPlayer)
{
	// Player Setting
	//m_pPlayer = pPlayer; 

	// Sector Setting
	//m_ppSector[m_pPlayer->GetY()][m_pPlayer->GetX()].AddGroundCreature(pPlayer);
	//m_ppSector[g_pPlayer->GetY()][g_pPlayer->GetX()].RemoveGroundCreature(&g_pPlayer->;

	// Player의 시야 안의 Sector를 보이게 한다.
	//SetSight(g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetLightSight());

	RemoveCreature( g_pPlayer->GetID() );	
}


//----------------------------------------------------------------------
// (x,y) sector에 들어갈 수 있는가?
//----------------------------------------------------------------------
// 갈 수 없는 경우를 모두 check하여 
// return false를 해줘야 한다.
//----------------------------------------------------------------------
bool		
MZone::CanMove(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	// Zone의 영역을 벗어날 경우
	if (x<0 || x>=m_Width || y<0 || y>=m_Height)
		return false;

	const MSector& sector = m_ppSector[y][x];

	//------------------------------------------------------
	// [새기술] Sanctuary 로는 못 움직인다.
	//------------------------------------------------------
	if (sector.HasSanctuary()
		&& creatureType != MCreature::CREATURE_FAKE_NO_BLOCK
		&& creatureType != MCreature::CREATURE_FAKE_UNDERGROUND
		&& creatureType != MCreature::CREATURE_FAKE_GROUND
		&& creatureType != MCreature::CREATURE_FAKE_FLYING
		)
	{
		return false;
	}


	// 이미 다른 Object가 존재하는 경우
	//if (m_ppSector[y][x].IsExistObject())
	{
		// objectType에 따라서 달리 check해 준다.
		switch (creatureType)
		{
			// UNDERGROUND CREATURE
			case MCreature::CREATURE_UNDERGROUND :
			case MCreature::CREATURE_FAKE_UNDERGROUND :
				return sector.CanStandUndergroundCreature();// || sector.IsPortal();

			// CREATURE
			case MCreature::CREATURE_GROUND :
			case MCreature::CREATURE_FAKE_GROUND :
				return sector.CanStandGroundCreature();// || sector.IsPortal();

			// FLYING CREATURE
			case MCreature::CREATURE_FLYING :
			case MCreature::CREATURE_FAKE_FLYING :
				return sector.CanStandFlyingCreature();// || sector.IsPortal();

			// fake는 무조건 움직인다.
			case MCreature::CREATURE_FAKE_NO_BLOCK :
				return true;
		}		
	}

	return true;
}

//----------------------------------------------------------------------
// Set ServerBlock
//----------------------------------------------------------------------
void		
MZone::SetServerBlock(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	// Zone의 영역을 벗어날 경우
	if (x<0 || x>=m_Width || y<0 || y>=m_Height)
		return;

	// 이미 다른 Object가 존재하는 경우
	//if (m_ppSector[y][x].IsExistObject())
	{
		// objectType에 따라서 달리 check해 준다.
		switch (creatureType)
		{
			// UNDERGROUND CREATURE
			case MCreature::CREATURE_UNDERGROUND :
				m_ppSector[y][x].SetBlockServerUnderground();
			break;

			// CREATURE
			case MCreature::CREATURE_GROUND :
				m_ppSector[y][x].SetBlockServerGround();
			break;

			// FLYING CREATURE
			case MCreature::CREATURE_FLYING :
				m_ppSector[y][x].SetBlockServerFlying();
			break;
		}		
	}
}

//----------------------------------------------------------------------
// UnSet ServerBlock
//----------------------------------------------------------------------
void		
MZone::UnSetServerBlock(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	// Zone의 영역을 벗어날 경우
	if (x<0 || x>=m_Width || y<0 || y>=m_Height)
		return;

	// 이미 다른 Object가 존재하는 경우
	//if (m_ppSector[y][x].IsExistObject())
	{
		// objectType에 따라서 달리 check해 준다.
		
		// 2002.3.29
		// block은 일단 다 푼다. 뭔가 이상해서 - -;
		/*
		switch (creatureType)
		{
			// UNDERGROUND CREATURE
			case MCreature::CREATURE_UNDERGROUND :
				m_ppSector[y][x].UnSetBlockServerUnderground();
			break;

			// CREATURE
			case MCreature::CREATURE_GROUND :
				m_ppSector[y][x].UnSetBlockServerGround();
			break;

			// FLYING CREATURE
			case MCreature::CREATURE_FLYING :
				m_ppSector[y][x].UnSetBlockServerFlying();
			break;
		}
		*/
		MSector& sector = m_ppSector[y][x];
		sector.UnSetBlockServerUnderground();
		sector.UnSetBlockServerGround();
		sector.UnSetBlockServerFlying();
	}
}


//----------------------------------------------------------------------
// Move Ground Creature
//----------------------------------------------------------------------
// (xo,yo)에 있던 Creature를 (xn,yn)으로 옮긴다.
//----------------------------------------------------------------------
bool		
MZone::MoveGroundCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (xn<0 || yn<0 || xn>=m_Width || yn>=m_Height) return false;

	// Player인 경우 --> 이동하지 않는다.
	if (pCreature->GetClassType()==MCreature::CLASS_PLAYER
		// fake는 이동의 의미가 없다.
		|| pCreature->IsFakeCreature())
	{
		return true;
	}

	// 이미 이동해있는 경우..
	//if (xo==xn && yo==yn)
	//	return false;

	
	//MCreature* pExistCreature = NULL;

	int id = pCreature->GetID();

	//------------------------------------------------
	// sector에서 제거시킨다.
	//------------------------------------------------
	if (!m_ppSector[yo][xo].RemoveCreature(id))
	{
		int xs = pCreature->GetServerX();
		int ys = pCreature->GetServerY();

		if (!m_ppSector[ys][xs].RemoveCreature(id))
		{	
			DEBUG_ADD_FORMAT("Can't Remove! [Ground] ID=%d (%d,%d) --> (%d,%d)", pCreature->GetID(), xo,yo,xn,yn);			
		}
	}

	//------------------------------------------------
	// sector에 추가시킨다.
	//------------------------------------------------
	if (!m_ppSector[yn][xn].AddGroundCreature( pCreature ))
	{
		DEBUG_ADD_FORMAT("Can't Add GroundCreature! ID=%d (%d,%d)", pCreature->GetID(), xn,yn);			
	}

	
	/*
	if (!m_ppSector[yo][xo].RemoveGroundCreature(id, pExistCreature))
	{	
		if (!m_ppSector[yo][xo].RemoveFlyingCreature(id, pExistCreature))
		{
			if (!m_ppSector[yo][xo].RemoveUndergroundCreature(id, pExistCreature))
			{
				int xs = pCreature->GetServerX();
				int ys = pCreature->GetServerY();

				if (!m_ppSector[ys][xs].RemoveFlyingCreature(id, pExistCreature))
				{	
					if (!m_ppSector[ys][xs].RemoveGroundCreature(id, pExistCreature))
					{
						if (!m_ppSector[ys][xs].RemoveUndergroundCreature(id, pExistCreature))
						{

							int id = 0;
							if (pExistCreature!=NULL)
								id = pExistCreature->GetID();

							// remove 실패
							//MessageBox(g_hWnd, "Already Removed![Flying]", NULL, MB_OK);
							DEBUG_ADD_FORMAT("Already Removed! [Ground] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, id);
						}
					}
				}
			}
		}
	}	

	MCreature* pOldCreature = m_ppSector[yn][xn].GetGroundCreature();

	//------------------------------------------------
	// 아무도 없는 경우
	//------------------------------------------------
	if (pOldCreature == NULL)
	{
		// add								
		if (!m_ppSector[yn][xn].AddGroundCreature( pCreature ))
		{
			DEBUG_ADD_FORMAT("Can't Add GroundCreature! ID=%d (%d,%d)", pCreature->GetID(), xn,yn);			
		}
	}
	//------------------------------------------------
	// 새로운 자리에 이미 다른 캐릭터가 있는 경우...
	//------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("Already ExistCreature! [Ground] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, pOldCreature->GetID());				
		
		// 먼저 있는 캐릭터가 Player일 경우
		//if (pOldCreature->GetID()==g_pPlayer->GetID())
		//{
			// 추가 하지 않는다.
			// 이동하기 전으로 되돌린다.
			// add								
		//	m_ppSector[xo][yo].AddGroundCreature( pCreature );

		//	return false;
		//}
		//else
		{
			//------------------------------------------------
			// 먼저 있던 캐릭터 제거..
			//------------------------------------------------						
			m_ppSector[yn][xn].RemoveGroundCreature( pOldCreature );

			#ifdef OUTPUT_DEBUG
				if (pOldCreature!=NULL)
					DEBUG_ADD_FORMAT("Remove Creature by New Creature! ID=%d (%d,%d)", pOldCreature->GetID(), xn,yn);
			#endif

			//------------------------------------------------
			// add								
			//------------------------------------------------
			m_ppSector[yn][xn].AddGroundCreature( pCreature );

			// 확인용
			pCreature->SetX( xn );
			pCreature->SetY( yn );
			
			//------------------------------------------------
			// 제거된 캐릭터를 제대로된 위치에 추가한다.
			//------------------------------------------------
			if (pOldCreature!=NULL)
			{
				int x = pOldCreature->GetServerX();
				int y = pOldCreature->GetServerY();
				
				// 살아있고.. 좌표가 다른 경우..
				if (x!=xn || y!=yn)
				{
					if (pOldCreature->IsAlive())					
					{				
						pOldCreature->SetStop();
						pOldCreature->MovePosition(	x, y );
					}
					else
					{
						pOldCreature->MovePosition(	x, y );
					}
				}
			}
		}
	}		
	*/

	return true;
}

//----------------------------------------------------------------------
// Move Flying Creature
//----------------------------------------------------------------------
// (xo,yo)에 있던 FlyingCreature를 (xn,yn)으로 옮긴다.
//----------------------------------------------------------------------
bool		
MZone::MoveFlyingCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (xn<0 || yn<0 || xn>=m_Width || yn>=m_Height) return false;

	// Player인 경우 --> 이동하지 않는다.
	if (pCreature->GetClassType()==MCreature::CLASS_PLAYER
		// fake는 이동의 의미가 없다.
		|| pCreature->IsFakeCreature())
	{
		return true;
	}

	// 이미 이동해있는 경우..
	//if (xo==xn && yo==yn)
	//	return false;

	//MCreature* pExistCreature = NULL;

	int id = pCreature->GetID();

	//------------------------------------------------
	// sector에서 제거시킨다.
	//------------------------------------------------
	if (!m_ppSector[yo][xo].RemoveCreature(id))
	{
		int xs = pCreature->GetServerX();
		int ys = pCreature->GetServerY();

		if (!m_ppSector[ys][xs].RemoveCreature(id))
		{	
			DEBUG_ADD_FORMAT("Can't Remove! [Flying] ID=%d (%d,%d) --> (%d,%d)", pCreature->GetID(), xo,yo,xn,yn);			
		}
	}

	//------------------------------------------------
	// sector에 추가시킨다.
	//------------------------------------------------
	if (!m_ppSector[yn][xn].AddFlyingCreature( pCreature ))
	{
		DEBUG_ADD_FORMAT("Can't Add FlyingCreature! ID=%d (%d,%d)", pCreature->GetID(), xn,yn);			
	}

	/*
	// remove가 안 된 경우...
	if (!m_ppSector[yo][xo].RemoveFlyingCreature(id, pExistCreature))
	{	
		if (!m_ppSector[yo][xo].RemoveGroundCreature(id, pExistCreature))
		{
			if (!m_ppSector[yo][xo].RemoveUndergroundCreature(id, pExistCreature))
			{
				int xs = pCreature->GetServerX();
				int ys = pCreature->GetServerY();

				if (!m_ppSector[ys][xs].RemoveFlyingCreature(id, pExistCreature))
				{	
					if (!m_ppSector[ys][xs].RemoveGroundCreature(id, pExistCreature))
					{
						if (!m_ppSector[ys][xs].RemoveUndergroundCreature(id, pExistCreature))
						{

							int id = 0;
							if (pExistCreature!=NULL)
								id = pExistCreature->GetID();

							// remove 실패
							//MessageBox(g_hWnd, "Already Removed![Flying]", NULL, MB_OK);
							DEBUG_ADD_FORMAT("Already Removed! [Flying] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, id);
						}
					}
				}
			}
		}
	}

	// test
	MCreature* pOldCreature = m_ppSector[yn][xn].GetFlyingCreature();

	// 아무도 없는 경우
	if (pOldCreature == NULL)
	{
		// add								
		m_ppSector[yn][xn].AddFlyingCreature( pCreature );
	}
	// 새로운 자리에 이미 다른 캐릭터가 있는 경우...
	else
	{
		DEBUG_ADD_FORMAT("Already ExistCreature! [Flying] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, pOldCreature->GetID());				

		// 먼저 있는 캐릭터가 Player일 경우
		//if (pOldCreature->GetID()==g_pPlayer->GetID())
		//{
			// 추가 하지 않는다.
			// 이동하기 전으로 되돌린다.
			// add								
		//	m_ppSector[xo][yo].AddFlyingCreature( pCreature );

		//	return false;
		//}
		//else
		{
			// 먼저 있던 캐릭터 제거..
			m_ppSector[yn][xn].RemoveFlyingCreature( pOldCreature );

			#ifdef OUTPUT_DEBUG
				if (pOldCreature!=NULL)
					DEBUG_ADD_FORMAT("[Flying]Remove Creature by New Creature! ID=%d (%d,%d)", pOldCreature->GetID(), xn,yn);
			#endif

			// add								
			m_ppSector[yn][xn].AddFlyingCreature( pCreature );

			// 확인용
			pCreature->SetX( xn );
			pCreature->SetY( yn );

			//------------------------------------------------
			// 제거된 캐릭터를 제대로된 위치에 추가한다.
			//------------------------------------------------
			if (pOldCreature!=NULL)
			{
				int x = pOldCreature->GetServerX();
				int y = pOldCreature->GetServerY();
				
				// 살아있고.. 좌표가 다른 경우..
				if (x!=xn || y!=yn)
				{
					if (pOldCreature->IsAlive())					
					{				
						pOldCreature->SetStop();
						pOldCreature->MovePosition(	x, y );
					}
					else
					{
						pOldCreature->MovePosition(	x, y );
					}
				}
			}
		}
	}		
	*/

	return true;
}

//----------------------------------------------------------------------
// Move Ungerground Creature
//----------------------------------------------------------------------
// (xo,yo)에 있던 UndergroundCreature를 (xn,yn)으로 옮긴다.
//----------------------------------------------------------------------
bool		
MZone::MoveUndergroundCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (xn<0 || yn<0 || xn>=m_Width || yn>=m_Height) return false;

	// Player인 경우 --> 이동하지 않는다.
	if (pCreature->GetClassType()==MCreature::CLASS_PLAYER
		// fake는 이동의 의미가 없다.
		|| pCreature->IsFakeCreature())
	{
		return true;
	}

	// 이미 이동해있는 경우..
	//if (xo==xn && yo==yn)
	//	return false;

	
	//MCreature* pExistCreature = NULL;

	int id = pCreature->GetID();

	//------------------------------------------------
	// sector에서 제거시킨다.
	//------------------------------------------------
	if (!m_ppSector[yo][xo].RemoveCreature(id))
	{
		int xs = pCreature->GetServerX();
		int ys = pCreature->GetServerY();

		if (!m_ppSector[ys][xs].RemoveCreature(id))
		{	
			DEBUG_ADD_FORMAT("Can't Remove! [Underground] ID=%d (%d,%d) --> (%d,%d)", pCreature->GetID(), xo,yo,xn,yn);			
		}
	}

	//------------------------------------------------
	// sector에 추가시킨다.
	//------------------------------------------------
	if (!m_ppSector[yn][xn].AddUndergroundCreature( pCreature ))
	{
		DEBUG_ADD_FORMAT("Can't Add UndergroundCreature! ID=%d (%d,%d)", pCreature->GetID(), xn,yn);			
	}

	/*
	// remove가 안 된 경우...
	if (!m_ppSector[yo][xo].RemoveUndergroundCreature(id, pExistCreature))
	{	
		if (!m_ppSector[yo][xo].RemoveGroundCreature(id, pExistCreature))
		{
			if (!m_ppSector[yo][xo].RemoveFlyingCreature(id, pExistCreature))
			{
				int xs = pCreature->GetServerX();
				int ys = pCreature->GetServerY();

				if (!m_ppSector[ys][xs].RemoveFlyingCreature(id, pExistCreature))
				{	
					if (!m_ppSector[ys][xs].RemoveGroundCreature(id, pExistCreature))
					{
						if (!m_ppSector[ys][xs].RemoveUndergroundCreature(id, pExistCreature))
						{

							int id = 0;
							if (pExistCreature!=NULL)
								id = pExistCreature->GetID();

							// remove 실패
							//MessageBox(g_hWnd, "Already Removed![Flying]", NULL, MB_OK);
							DEBUG_ADD_FORMAT("Already Removed! [Under] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, id);
						}
					}
				}
			}
		}
	}

	// test
	MCreature* pOldCreature = m_ppSector[yn][xn].GetUndergroundCreature();

	// 아무도 없는 경우
	if (pOldCreature == NULL)
	{
		// add								
		m_ppSector[yn][xn].AddUndergroundCreature( pCreature );
	}
	// 새로운 자리에 이미 다른 캐릭터가 있는 경우...
	else
	{
		DEBUG_ADD_FORMAT("Already ExistCreature! [Underground] ID=%d (%d,%d) --> (%d,%d) ExistID = %d", pCreature->GetID(), xo,yo,xn,yn, pOldCreature->GetID());				

		// 먼저 있는 캐릭터가 Player일 경우
		//if (pOldCreature->GetID()==g_pPlayer->GetID())
		//{
			// 추가 하지 않는다.
			// 이동하기 전으로 되돌린다.
			// add								
		//	m_ppSector[xo][yo].AddUndergroundCreature( pCreature );

		//	return false;
		//}
		//else
		{
			//------------------------------------------------
			// 먼저 있던 캐릭터 제거..
			//------------------------------------------------						
			m_ppSector[yn][xn].RemoveUndergroundCreature( pOldCreature );

			#ifdef OUTPUT_DEBUG
				if (pOldCreature!=NULL)
					DEBUG_ADD_FORMAT("Remove Creature by New Creature! ID=%d (%d,%d)", pOldCreature->GetID(), xn,yn);
			#endif

			//------------------------------------------------
			// add								
			//------------------------------------------------
			m_ppSector[yn][xn].AddUndergroundCreature( pCreature );
			
			//------------------------------------------------
			// 제거된 캐릭터를 제대로된 위치에 추가한다.
			//------------------------------------------------
			if (pOldCreature!=NULL)
			{
				int x = pOldCreature->GetServerX();
				int y = pOldCreature->GetServerY();
				
				// 살아있고.. 좌표가 다른 경우..
				if (x!=xn || y!=yn)
				{
					if (pOldCreature->IsAlive())					
					{				
						pOldCreature->SetStop();
						pOldCreature->MovePosition(	x, y );
					}
					else
					{
						pOldCreature->MovePosition(	x, y );
					}
				}
			}
		}
	}	
	*/

	return true;
}

//----------------------------------------------------------------------
// Get CreatureID
//----------------------------------------------------------------------
// 이름으로 ID를 찾는다.
// flag : 1 npc뺴고
//----------------------------------------------------------------------
TYPE_OBJECTID		
MZone::GetCreatureID(const char* pName, int flag) const
{
	CREATURE_MAP::const_iterator	iCreature = m_mapCreature.begin();

	MCreature* pCreature;

	//------------------------------------------------------
	// 모든 Creature에 대해서 Action
	//------------------------------------------------------
	while (iCreature != m_mapCreature.end())
	{
		pCreature = iCreature->second;

		// player가 아닌 경우에..
		if (pCreature->GetID()!=g_pPlayer->GetID()
			&& strcmp(pCreature->GetName(), pName)==0
			&& (flag == 0 || flag == 1 && !pCreature->IsNPC() )
			)
		{
			return pCreature->GetID();
		}

		iCreature++;
	}	

	return OBJECTID_NULL;
}

//----------------------------------------------------------------------
// Sector (x,y)에 있는 Creature의 ID를 넘겨준다.
//----------------------------------------------------------------------

TYPE_OBJECTID
MZone::GetCreatureID(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (x<0 || y<0 
		|| x>=m_Width || y>=m_Height) return OBJECTID_NULL;

	// 뭔가 있는 sector이면	
	MCreature*	pCreature = m_ppSector[y][x].GetGroundCreature();

	// Creature가 있으면
	if (pCreature != NULL)
	{			
		return pCreature->GetID();
	}

	return OBJECTID_NULL;

}

//----------------------------------------------------------------------
// Sector (x,y)에 있는 Creature를 넘겨준다.
//----------------------------------------------------------------------
MCreature*
MZone::GetCreatureBySector(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (x<0 || y<0 
		|| x>=m_Width || y>=m_Height) return NULL;

	// 뭔가 있는 sector이면	
	MCreature*	pCreature = m_ppSector[y][x].GetGroundCreature();

	// Creature가 있으면
	if (pCreature != NULL)
	{			
		return pCreature;
	}

	return NULL;

}
//----------------------------------------------------------------------
// Sector (x,y)에 있는 FlyingCreature의 ID를 넘겨준다.
//----------------------------------------------------------------------
TYPE_OBJECTID
MZone::GetFlyingCreatureID(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (x<0 || y<0 
		|| x>=m_Width || y>=m_Height) return OBJECTID_NULL;

	// 뭔가 있는 sector이면	
	MCreature*	pCreature = (MCreature*)m_ppSector[y][x].GetFlyingCreature();

	// Creature가 있으면
	if (pCreature != NULL)
	{			
		return pCreature->GetID();
	}

	return OBJECTID_NULL;

}


//----------------------------------------------------------------------
// Sector (x,y)에 있는 UndergroundCreature의 ID를 넘겨준다.
//----------------------------------------------------------------------
TYPE_OBJECTID
MZone::GetUndergroundCreatureID(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (x<0 || y<0 
		|| x>=m_Width || y>=m_Height) return OBJECTID_NULL;

	// 뭔가 있는 sector이면	
	MCreature*	pCreature = (MCreature*)m_ppSector[y][x].GetUndergroundCreature();

	// Creature가 있으면
	if (pCreature != NULL)
	{			
		return pCreature->GetID();
	}

	return OBJECTID_NULL;

}


//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
void
MZone::Update()
{
	//------------------------------------------
	// 다른 Creature를 움직인다.
	//------------------------------------------
	UpdateAllCreature();

	//------------------------------------------
	// fake Creature를 움직인다.
	//------------------------------------------
	UpdateFakeCreature();

	//------------------------------------------
	// UpdateItem
	//------------------------------------------
	UpdateItem();

	#ifdef OUTPUT_DEBUG_UPDATE_ZONE
		DEBUG_ADD("MZone::before Effect");
	#endif

	//------------------------------------------
	// Effect를 움직인다.
	//------------------------------------------
	UpdateWaitEffects();
	UpdateEffects();
	UpdateGroundEffects();
	//------------------------------------------
	// Helicopter
	//------------------------------------------
	m_HelicopterManager.Update();

	//------------------------------------------
	// Sound 처리
	//------------------------------------------
	UpdateSound();

	#ifdef OUTPUT_DEBUG_UPDATE_ZONE
		DEBUG_ADD("MZone::Update OK");
	#endif
}

//----------------------------------------------------------------------
// Move All Creature
//----------------------------------------------------------------------
// Zone에 속한 모든 Creature를 행동하게 한다.
// Player는 제외
// Player의 시야를 벗어나면 제거 시켜야 한다.
//----------------------------------------------------------------------
void
MZone::UpdateAllCreature()
{
	CREATURE_MAP::iterator	iCreature = m_mapCreature.begin();

	MCreature* pCreature;

	//	DEBUG_ADD_FORMAT("[UpdateAllCreature] %d Creature(s)", m_mapCreature.size());

	static DWORD nextResetTime = g_CurrentTime + g_pUserOption->persnalShopupdatetime;
	bool bResetPersonalShopValue = false;
	
	if( g_CurrentTime > nextResetTime )
	{
		nextResetTime = g_CurrentTime+ g_pUserOption->persnalShopupdatetime;
		bResetPersonalShopValue = true;
	}	
	
	//------------------------------------------------------
	// 모든 Creature에 대해서 Action
	//------------------------------------------------------
	while (iCreature != m_mapCreature.end())
	{
		pCreature = ((*iCreature).second);

		#ifdef OUTPUT_DEBUG_UPDATE_ZONE
				DEBUG_ADD_FORMAT("[UpdateCreature] id=%d, cType=%d, x=%d, y=%d, action=%d", 
									(int)pCreature->GetID(), (int)pCreature->GetCreatureType(),
									(int)pCreature->GetX(), (int)pCreature->GetY(), (int)pCreature->GetAction());
		#endif

		if( pCreature->CurPernalShop() == 1 && bResetPersonalShopValue )
			pCreature->SetPersonalShopMsgTime( rand() & 0x7F );
		
		//------------------------------------------------------
		//
		// Player인 경우
		//
		//------------------------------------------------------
		if (pCreature->GetClassType()==MCreature::CLASS_PLAYER)
		{
#ifdef __METROTECH_TEST__
//			for(int i = 0; i < abs(g_iSpeed)-1; i++)
//			{
//				g_pPlayer->Action();
////				g_pPlayer->UpdateAttachEffect();
//			}
#endif
			if(g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
				||g_pPlayer->HasEffectStatus( EFFECTSTATUS_DRAGON_EYES )
				)
				g_pPlayer->Action();
			
			g_pPlayer->Action();

			#ifdef OUTPUT_DEBUG_UPDATE_ZONE
				DEBUG_ADD_FORMAT("PlAok");
			#endif

			g_pPlayer->UpdateAttachEffect();

			#ifdef OUTPUT_DEBUG_UPDATE_ZONE
				DEBUG_ADD_FORMAT("PlEok");
			#endif			
		}
		//------------------------------------------------------
		//
		// 다른 Creature인 경우
		//
		//------------------------------------------------------
		else
		{
			int invisibleCount = pCreature->GetInvisibleCount();
			if(pCreature->HasEffectStatus( EFFECTSTATUS_DRAGON_EYES ))
				pCreature->Action();
			pCreature->Action();
			pCreature->UpdateAttachEffect();

			//------------------------------------------------------
			// Creature가 죽은 경우
			//------------------------------------------------------
			// Zone에서 제거하고 MCorpse로 바꾼다.
			//------------------------------------------------------
			/*
			if (pCreature->IsDead())
			{
				#ifdef CONNECT_SERVER
					// 죽는 동작이 끝난 경우
					// 또, AttachEffect가 없는 경우..
					if (pCreature->GetActionCount()==pCreature->GetActionCountMax()
						&& !pCreature->IsExistAttachEffect())
					{	
						// creature를 시체로 바꾼다.
						AddCorpseFromCreature( pCreature );

						// map에서 creature를 제거한다.
						CREATURE_MAP::iterator	iCreatureTemp = iCreature;
						iCreature++;

						m_mapCreature.erase( iCreatureTemp );

						continue;
					}
				#endif
			}
			*/

			//------------------------------------------------------
			// invisibleCount가 64면 이 캐릭터는 Zone에서 없어져야 한다.
			//------------------------------------------------------
			if (pCreature->GetInvisibleCount()==64
				&& invisibleCount!=64)
			{
				//-------------------------------------------------------
				// snipping mode이면 무조건 없어진다(아무도 볼 수 없으므로)
				//
				// invisibility이면 player가 slayer이고
				//					detect_hidden이 없다면 없어진다.
				//-------------------------------------------------------
				if (//pCreature->HasEffectStatus(EFFECTSTATUS_SNIPPING_MODE)|| 
					pCreature->HasEffectStatus(EFFECTSTATUS_INVISIBILITY)
						&& !g_pPlayer->IsVampire()
						&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_DETECT_INVISIBILITY)
						&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_OBSERVING_EYE)
 						&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_GNOME_WHISPER)
//						&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
						)
				{
					//-------------------------------------------------------
					// player가 아닌 경우
					//-------------------------------------------------------
					int id			= pCreature->GetID();
					int x			= pCreature->GetX();
					int y			= pCreature->GetY();
					int serverX		= pCreature->GetServerX();
					int serverY		= pCreature->GetServerY();

					//----------------------------------------
					// 헬기 제거
					//----------------------------------------
					if (pCreature->IsSlayer())
					{
						m_HelicopterManager.RemoveHelicopter( pCreature->GetID() );
					}				
					
					// 찾은 경우 --> 제거		
					bool removed = true;
					
					//------------------------------------------------
					// sector에서 제거시킨다.
					//------------------------------------------------
					if (!m_ppSector[y][x].RemoveCreature(id))
					{
						if (!m_ppSector[serverY][serverX].RemoveCreature(id))
						{	
							DEBUG_ADD_FORMAT("Can't RemoveCreature! ID=%d client(%d,%d), server(%d,%d)", pCreature->GetID(), x,y,serverX,serverY);

							removed = false;
						}
					}

					UnSetServerBlock( pCreature->GetMoveType(), pCreature->GetServerX(), pCreature->GetServerY() );

					// 제거
					delete pCreature;	

					// map에서 creature를 제거한다.
					CREATURE_MAP::iterator	iCreatureTemp = iCreature;
					iCreature++;

					m_mapCreature.erase( iCreatureTemp );			


					continue;
				}
			}

		}

		iCreature++;
	}

#ifdef __METROTECH_TEST__
	extern int	g_iSpeed;
	if(g_iSpeed < 0)
	{
		CREATURE_MAP::iterator itr = m_mapCreature.begin();
		CREATURE_MAP::iterator endItr = m_mapCreature.end();
		
		while(itr != endItr)
		{
			MCreature *pCreature = itr->second;
			if(pCreature->IsPlayer() && pCreature->GetID() != g_pPlayer->GetID() && !pCreature->IsPlayerParty())
				break;
			
			itr++;
		}
		
		if(itr == endItr)
		{
			g_iSpeed = -4;
		}
		else
		{
			g_iSpeed = -1;
		}
	}
#endif
	
	#ifdef OUTPUT_DEBUG_UPDATE_ZONE
		DEBUG_ADD("UpdateAllCreature OK");
	#endif
}

//----------------------------------------------------------------------
// Create Corpse  From Creature
//----------------------------------------------------------------------
// 시체(MCorpse)를 생성하는데.. Creature를 참조한다..
//----------------------------------------------------------------------
/*
bool
MZone::AddCorpseFromCreature(MCreature* pCreature)
{
	DEBUG_ADD_FORMAT("Add Corpse From Creature id=%d", pCreature->GetID());

	int id = pCreature->GetID();
	int sX = pCreature->GetX();
	int sY = pCreature->GetY();

	//----------------------------------------
	// 채팅을 없애준다.
	//----------------------------------------
	pCreature->ClearChatString();

	//----------------------------------------
	// Sector에서 Creature 제거
	//----------------------------------------
	// player가 아닌 경우..
	//----------------------------------------
	bool removed = false;
	if (id != g_pPlayer->GetID())
	{
		int serverX = pCreature->GetServerX();
		int serverY = pCreature->GetServerY();

		removed = m_ppSector[sY][sX].RemoveGroundCreature( id );
		if (!removed)
		{
			removed = m_ppSector[serverY][serverX].RemoveGroundCreature( id );

			if (!removed)
			{
				removed = m_ppSector[sY][sX].RemoveFlyingCreature( id );

				if (!removed)
				{
					removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( id );

					if (!removed)
					{
						removed = m_ppSector[sY][sX].RemoveUndergroundCreature( id );

						if (!removed)
						{
							removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( id );

							DEBUG_ADD_FORMAT("[Error] Can't Remove Creature to corpse. ID=%d. xy=(%d, %d), sxy=(%d, %d)", id, sX, sY, serverX, serverY);
						}
					}
				}
			}
		}
	}

	// Creature가 제거가 안됐으면...
	#ifdef OUTPUT_DEBUG
		if (!removed)
		{			
			DEBUG_ADD_FORMAT("[Create Corpse] Can't Remove Creature from Sector: %d, (%d,%d)", id, sX, sY);
		}
	#endif
	
	//----------------------------------------
	//
	// 시체를 생성하고 Creature를 설정한다.
	//
	//----------------------------------------
	// server에서 받은 위치
	int sSX = pCreature->GetServerX();
	int sSY = pCreature->GetServerY();

	MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
	pCorpse->SetID( id );
	pCorpse->SetItemType( 0 );
	pCorpse->SetCreature( pCreature );
	pCorpse->SetPosition( sSX, sSY );

	// item 개수 설정
	pCorpse->SetNumber( pCreature->GetItemCount() );


	//----------------------------------------	
	// 이미 다른 Item이 있다면?? 제거한다.
	//----------------------------------------
	if (m_ppSector[sSY][sSX].IsExistItem())
	{
		MItem* pOldItem = m_ppSector[sSY][sSX].GetItem();
		
		#ifdef OUTPUT_DEBUG
			if (pOldItem!=NULL)
			{
				DEBUG_ADD_FORMAT("Already Exist Item. Remove Old Item: %d, (%d,%d)", pOldItem->GetID(), sSX, sSY);
			}
		#endif

		bool bRemove = RemoveItem( pOldItem->GetID() );

		#ifdef OUTPUT_DEBUG
			if (!bRemove)
			{
					DEBUG_ADD_FORMAT("Can't Remove Old Item");
			}
		#endif
	}

	//----------------------------------------
	// Zone에 Item추가
	//----------------------------------------
	#ifdef OUTPUT_DEBUG
		if (!AddItem( pCorpse ))
		{
			// 음.. 불가능한 경우라고 할 수 있다. - -;
			DEBUG_ADD_FORMAT("[Error] Can't Add Corpse to Sector id=%d (%d,%d)", pCorpse->GetID(), sSX, sSY);
		
			delete pCorpse;

			return false;
		}

		return true;
	#else
		return AddItem( pCorpse );
	#endif
}
*/

//----------------------------------------------------------------------
// Create Corpse  From Creature
//----------------------------------------------------------------------
// 시체(MCorpse)를 생성하는데.. Creature를 참조한다..
//----------------------------------------------------------------------
bool
MZone::AddCorpseFromCreature(TYPE_OBJECTID id)
{
	DEBUG_ADD_FORMAT("Add Corpse From Creature id=%d", id);

	if (id==g_pPlayer->GetID())
	{
		DEBUG_ADD("No Player Corpse");
		return false;
	}

	CREATURE_MAP::iterator iCreature = m_mapCreature.find( id );

	if (iCreature==m_mapCreature.end())
	{
		DEBUG_ADD("No such creature");
		return false;
	}

	MCreature* pCreature = iCreature->second;

	UnSetServerBlock( pCreature->GetMoveType(), pCreature->GetServerX(), pCreature->GetServerY() );

	// Zone에서 제거한다.
	m_mapCreature.erase( iCreature );	
	
	int sX = pCreature->GetX();
	int sY = pCreature->GetY();

	//----------------------------------------
	// 채팅을 없애준다.
	//----------------------------------------
	if(pCreature->GetCreatureType() != 723) // 각성 질드레..^^: 여기서 지우면 죽으면서 하는 대사가 안나옴..
		pCreature->ClearChatString();

	//----------------------------------------
	// Sector에서 Creature 제거
	//----------------------------------------
	// player가 아닌 경우..
	//----------------------------------------
	bool removed = false;
	if (id != g_pPlayer->GetID()
		&& !pCreature->IsFakeCreature())
	{
		//------------------------------------------------
		// sector에서 제거시킨다.
		//------------------------------------------------
		removed = true;
		if (!m_ppSector[sY][sX].RemoveCreature(id))
		{
			int serverX = pCreature->GetServerX();
			int serverY = pCreature->GetServerY();

			if (!m_ppSector[serverY][serverX].RemoveCreature(id))
			{	
				removed = false;
				DEBUG_ADD_FORMAT("Can't RemoveCreatureToCorpse! ID=%d client(%d,%d), server(%d,%d)", pCreature->GetID(), sX,sY,serverX,serverY);
			}			
		}
		

		/*
		removed = m_ppSector[sY][sX].RemoveGroundCreature( id );
		if (!removed)
		{
			removed = m_ppSector[serverY][serverX].RemoveGroundCreature( id );

			if (!removed)
			{
				removed = m_ppSector[sY][sX].RemoveFlyingCreature( id );

				if (!removed)
				{
					removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( id );

					if (!removed)
					{
						removed = m_ppSector[sY][sX].RemoveUndergroundCreature( id );

						if (!removed)
						{
							removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( id );

							DEBUG_ADD_FORMAT("[Error] Can't Remove Creature to corpse. ID=%d. xy=(%d, %d), sxy=(%d, %d)", id, sX, sY, serverX, serverY);
						}
					}
				}
			}
		}
		*/
	}

	// Creature가 제거가 안됐으면...
	#ifdef OUTPUT_DEBUG
		if (!removed)
		{			
			DEBUG_ADD_FORMAT("[Create Corpse] Can't Remove Creature from Sector: %d, (%d,%d)", id, sX, sY);
		}
	#endif
	
	//----------------------------------------
	//
	// 시체를 생성하고 Creature를 설정한다.
	//
	//----------------------------------------
	// server에서 받은 위치
	int sSX = pCreature->GetServerX();
	int sSY = pCreature->GetServerY();

	MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
	pCorpse->SetID( id );
	pCorpse->SetItemType( 0 );
	pCorpse->SetCreature( pCreature );
	pCorpse->SetPosition( sSX, sSY );

	// item 개수 설정
	pCorpse->SetNumber( pCreature->GetItemCount() );


	//----------------------------------------	
	// 이미 다른 Item이 있다면?? 제거한다.
	//----------------------------------------
	if (m_ppSector[sSY][sSX].IsExistItem())
	{
		MItem* pOldItem = m_ppSector[sSY][sSX].GetItem();
		
		#ifdef OUTPUT_DEBUG
			if (pOldItem!=NULL)
			{
				DEBUG_ADD_FORMAT("Already Exist Item. Remove Old Item: %d, (%d,%d)", pOldItem->GetID(), sSX, sSY);
			}
		#endif

		bool bRemove = RemoveItem( pOldItem->GetID() );

		#ifdef OUTPUT_DEBUG
			if (!bRemove)
			{
					DEBUG_ADD_FORMAT("Can't Remove Old Item");
			}
		#endif
	}

	//----------------------------------------
	// Zone에 Item추가
	//----------------------------------------
	#ifdef OUTPUT_DEBUG
		if (!AddItem( pCorpse ))
		{
			// 음.. 불가능한 경우라고 할 수 있다. - -;
			DEBUG_ADD_FORMAT("[Error] Can't Add Corpse to Sector id=%d (%d,%d)", pCorpse->GetID(), sSX, sSY);
		
			delete pCorpse;

			return false;
		}

		return true;
	#else
		return AddItem( pCorpse );
	#endif

}


//----------------------------------------------------------------------
// Add Portal
//----------------------------------------------------------------------
// rect영역은 zoneID로 이동하는 portal이다.
//----------------------------------------------------------------------
void						
MZone::AddPortal(int type, int zoneID, const RECT& rect)
{
	int top = rect.top;
	int left = rect.left;
	int bottom = rect.bottom;
	int right = rect.right;

	if (top < 0) top = 0;
	if (left < 0) left = 0;
	if (bottom >= m_Height) bottom = m_Height-1;
	if (right >= m_Width) right = m_Width-1;

	if (top > bottom) { int temp=top; top=bottom; bottom=temp; }
	if (left > right) { int temp=left; left=right; right=left; }

	for (int i=top; i<=bottom; i++)
	{
		for (int j=left; j<=right; j++)
		{
			m_ppSector[i][j].AddPortal( type, zoneID );
		}
	}
}

//----------------------------------------------------------------------
// Keep Object In PlayerSight
//----------------------------------------------------------------------
// Zone에 속한 모든 Creature/Item중에서
// Player의 시야 안에 있는 것만 남기고 
// 나머지는 제거시킨다.
// Server의 시야 범위와 맞추어야 한다.
//----------------------------------------------------------------------
void
MZone::KeepObjectInSight(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE sight)
{	
//	int sight15 = sight + (sight>>1);

	//------------------------------------------------------
	// Player의 시야 범위를 지정한다.
	//------------------------------------------------------
//	int minX = min( sight, VISION_SECTOR_WIDTH_HALF );
//	int minY = min( sight, VISION_SECTOR_HEIGHT_HALF );
//	int minY2 = min( sight, VISION_SECTOR_HEIGHT_HALF );//-2 );

	int minX = NEW_VISION_SECTOR_WIDTH_HALF;
	int minY = NEW_VISION_SECTOR_HEIGHT_HALF_UP;
	int minY2 = NEW_VISION_SECTOR_HEIGHT_HALF_DOWN;

	int sX1 = x-minX;
	int sY1 = y-minY;
	int sX2 = x+minX;
	int sY2 = y+minY2;

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{					
		sX1 = 0;	
	}

	if (sX2 >= m_Width)
	{
		sX2 = m_Width-1;
	}

	if (sY1 < 0)
	{
		sY1 = 0;	
	}

	if (sY2 >= m_Height)
	{
		sY2 = m_Height-1;
	}
	
	//------------------------------------------------------
	//
	//				모든 Creature에 대해서....
	//
	//------------------------------------------------------
	CREATURE_MAP::iterator	iCreature = m_mapCreature.begin();

	MCreature* pCreature;

	while (iCreature != m_mapCreature.end())
	{
		pCreature = iCreature->second;

		//-----------------------------------------------------
		// Player가 아닌 경우에만 체크하고 제거한다.
		//-----------------------------------------------------
		if (pCreature->GetClassType()==MCreature::CLASS_PLAYER || pCreature->GetID() < 10000)	// 클라이언트 크리쳐는 제거 안한다
		{	
			iCreature++;
		}		
		else
		{
			//-----------------------------------------------------
			// Creature의 Server좌표가 Player의 시야에 
			// 포함되지 않은 경우만 제거시킨다.
			//-----------------------------------------------------			
			int cX = pCreature->GetServerX();
			int cY = pCreature->GetServerY();
//			int dist = abs(x - cX) + abs(y - cY);

			DEBUG_ADD_FORMAT("[KeepObjectInSight] %d : %d : %d , %d : %d : %d",
				sX1, cX, sX2, sY1, cY, sY2 );
			//-----------------------------------------------------			
			// 시야의 범위 안에 속하는 경우는 넘어간다..
			//-----------------------------------------------------			
			if (cX>=sX1 && cX<=sX2 &&
				cY>=sY1 && cY<=sY2 
//				&& dist <= sight15
				)
			{
				iCreature++;			
			}
			//-----------------------------------------------------			
			// 시야의 범위 안에 속하지 않으면... 제거~~
			//-----------------------------------------------------			
			else
			{
				// iTemp를 지우면 된다.
				CREATURE_MAP::iterator iTemp = iCreature;

				// 다음 것..
				iCreature++;

				DEBUG_ADD_FORMAT("[Remove Creature by Sight] id=%d xy=(%d, %d)", pCreature->GetID(), cX, cY);						
				
				MCreature* pCreature = (*iTemp).second;
				
				//----------------------------------------
				// Sector에서 제거
				//----------------------------------------
				// player가 아닌 경우..
				//----------------------------------------
				int id = pCreature->GetID();
				int x = pCreature->GetX();
				int y = pCreature->GetY();
				
				//------------------------------------------------
				// sector에서 제거시킨다.
				//------------------------------------------------
				if (!m_ppSector[y][x].RemoveCreature(id))
				{
					if (!m_ppSector[cY][cX].RemoveCreature(id))
					{	
						DEBUG_ADD_FORMAT("Can't RemoveCreature! ID=%d client(%d,%d), server(%d,%d)", pCreature->GetID(), x,y,cX,cY);
					}
				}

				/*
				bool removed = m_ppSector[y][x].RemoveGroundCreature( id );
				if (!removed)
				{
					removed = m_ppSector[serverY][serverX].RemoveGroundCreature( id );

					if (!removed)
					{
						removed = m_ppSector[y][x].RemoveFlyingCreature( id );

						if (!removed)
						{
							removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( id );

							if (!removed)
							{
								removed = m_ppSector[y][x].RemoveUndergroundCreature( id );

								if (!removed)
								{
									removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( id );

									DEBUG_ADD_FORMAT("[Error] Can't Remove Creature by sight. ID=%d. xy=(%d, %d), sxy=(%d, %d)", id, x, y, serverX, serverY);
								}
							}
						}
					}
				}
				*/

				//-------------------------------------------------------
				// 지금 제거 될려는 애가 player의 파티인 경우
				//-------------------------------------------------------
				/*
				if (pCreature->IsPlayerParty()
					&& g_pParty!=NULL)
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( pCreature->GetName() );
						
					if (pInfo!=NULL)
					{
						pInfo->bInSight = false;
						pInfo->zoneID = 60002;
					}
				}
				*/
				
				// memory에서 제거
				UnSetServerBlock( pCreature->GetMoveType(), pCreature->GetServerX(), pCreature->GetServerY() );

				delete pCreature;				
				
				m_mapCreature.erase(iTemp);			
			}			
		}		
	}

	//------------------------------------------------------
	//
	//				모든 Item에 대해서....
	//
	//------------------------------------------------------
	ITEM_MAP::iterator	iItem = m_mapItem.begin();

	MItem* pItem;

	while (iItem != m_mapItem.end())
	{
		pItem = iItem->second;
	
		//-----------------------------------------------------
		// Item의 좌표가 Player의 시야에 
		// 포함되지 않은 경우만 제거시킨다.
		//-----------------------------------------------------			
		int iX = pItem->GetX();
		int iY = pItem->GetY();
//		int dist = abs(x - iX) + abs(y - iY);

		//-----------------------------------------------------			
		// 시야의 범위 안에 속하는 경우는 넘어간다..
		//-----------------------------------------------------			
		if (iX>=sX1 && iX<=sX2 &&
			iY>=sY1 && iY<=sY2 
//			&& dist <= sight15
			)
		{
			iItem++;			
		}
		//-----------------------------------------------------			
		// 시야의 범위 안에 속하지 않으면... 제거~~
		//-----------------------------------------------------			
		else
		{
			// iTemp를 지우면 된다.
			ITEM_MAP::iterator iTemp = iItem;

			// 다음 것..
			iItem++;

			DEBUG_ADD_FORMAT("[Remove Item by Sight] id=%d xy=(%d, %d)", pItem->GetID(), iX, iY);					
				
			MItem* pItem = (*iTemp).second;

			// [성물수정]
			MSector& sector = m_ppSector[iY][iX];				
			sector.RemoveItem( pItem->GetID() );

			// [성물수정]
			if (IsRelicTable(pItem))
			{
				sector.UnSetBlockGround();
				sector.UnSetBlockFlying();
			}
				
			// memory에서 제거
			//delete pItem;				
			SAFE_DELETE ( pItem );
				
			m_mapItem.erase(iTemp);	
		}					
	}
	
	//------------------------------------------------------
	// Effect 지우기
	//------------------------------------------------------
	EFFECT_MAP::iterator	iEffect = m_mapEffect.begin();

	MEffect* pEffect;

	while (iEffect != m_mapEffect.end())
	{
		pEffect = iEffect->second;
	
		//-----------------------------------------------------
		// Item의 좌표가 Player의 시야에 
		// 포함되지 않은 경우만 제거시킨다.
		//-----------------------------------------------------			
		int iX = pEffect->GetX();
		int iY = pEffect->GetY();
//		int dist = abs(x - iX) + abs(y - iY);

		//-----------------------------------------------------			
		// 시야의 범위 안에 속하는 경우는 넘어간다..
		//-----------------------------------------------------			
		if (iX>=sX1 && iX<=sX2 &&
			iY>=sY1 && iY<=sY2 
//			&& dist <= sight15
			)
		{
			iEffect++;			
		}
		//-----------------------------------------------------			
		// 시야의 범위 안에 속하지 않으면... 제거~~
		//-----------------------------------------------------			
		else if (pEffect->GetEffectType()!=MEffect::EFFECT_CHASE &&
				(pEffect->GetFrameID() < EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE ||
				pEffect->GetFrameID() > EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3)&&
				pEffect->GetFrameID() != EFFECTSPRITETYPE_GDR_LAIR_POTAL)
		{
			// iTemp를 지우면 된다.
			EFFECT_MAP::iterator iTemp = iEffect;

			// 다음 것..
			iEffect++;

			DEBUG_ADD_FORMAT("[Remove Effect by Sight] id=%d xy=(%d, %d)", pEffect->GetID(), iX, iY);					
				
			MEffect* pEffect = iTemp->second;

			// [새기술9]
			RemoveSectorEffect( iX, iY, pEffect->GetID() );
				
			// memory에서 제거
			delete pEffect;				
				
			m_mapEffect.erase(iTemp);	
		}	
		else
		{
			iEffect++;
		}
	}
}

//----------------------------------------------------------------------
// Zone에 Creature 추가 
//----------------------------------------------------------------------
// 외부에서 new해줘야 한다.
// 이미 있는지 확인을 하고 없으면 추가해야 한다.
//----------------------------------------------------------------------
bool		
MZone::AddCreature(MCreature* pCreature)
{
	DEBUG_ADD_FORMAT("MZone::AddCreature id = %d", pCreature->GetID());

	if (pCreature->GetX()==SECTORPOSITION_NULL
		|| pCreature->GetY()==SECTORPOSITION_NULL)
	{
		DEBUG_ADD_FORMAT("[Error] Position Not Set");

		return false;
	}

	CREATURE_MAP::iterator	theIterator;

	theIterator = m_mapCreature.find(pCreature->GetID());
	
	// 아직 없는 Creature이면 추가	
	if (theIterator == m_mapCreature.end())
	{		
		// 우후훗
		if (g_pParty!=NULL 
			&& pCreature->GetName()!=NULL
			&& g_pParty->HasMember( pCreature->GetName() )
			&& !pCreature->IsNPC() )
		{
			pCreature->SetPlayerParty();
				
			//-------------------------------------------------------
			// 지금 들어온 애가 player의 파티인 경우
			//-------------------------------------------------------
			PARTY_INFO* pInfo = g_pParty->GetMemberInfo( pCreature->GetName() );
				
			if (pInfo!=NULL)
			{
				pInfo->bInSight = true;
				pInfo->HP = pCreature->GetHP();
				pInfo->MaxHP = pCreature->GetMAX_HP();
				pInfo->guildID = pCreature->GetGuildNumber();
			}			
		}

		//----------------------------------------
		// player가 Hallu인 경우
		//----------------------------------------
		if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
		{
			pCreature->SetHalluCreature( g_pTopView->GetRandomMonsterTypeInZone() );
		}

		//----------------------------------------
		// 이동방법에 따라.. 다시 설정
		//----------------------------------------
		if (pCreature->IsUndergroundCreature())
		{
			// 그냥 묻어둠.. - -;
		}
		else
		{
			if ((*g_pCreatureTable)[pCreature->GetCreatureType()].IsFlyingCreature())
			{
				pCreature->SetFlyingCreature();
			}			
			else
			{
				pCreature->SetGroundCreature();
			}
		}

		bool bAdd;
		//----------------------------------------
		// Sector Setting
		//----------------------------------------
		// Player인 경우
		//----------------------------------------
		if (pCreature->GetClassType()==MCreature::CLASS_PLAYER)
		{
			//m_mapCreature.insert(CREATURE_MAP::value_type(pCreature->GetID(), pCreature));
			bAdd = true;
		}
		//----------------------------------------
		// Player가 아닌 경우
		//----------------------------------------
		else
		{			
			int x = pCreature->GetX();
			int y = pCreature->GetY();

			switch (pCreature->GetMoveType())
			{
				case MCreature::CREATURE_UNDERGROUND :
					bAdd = m_ppSector[y][x].AddUndergroundCreature(pCreature);

					//if (!bAdd)
					{
					//	m_ppSector[pCreature->GetY()][pCreature->GetX()].RemoveUndergroundCreature();
					//	bAdd = m_ppSector[pCreature->GetY()][pCreature->GetX()].AddUndergroundCreature(pCreature);
					}
				break;

				case MCreature::CREATURE_GROUND :
					bAdd = m_ppSector[y][x].AddGroundCreature(pCreature);		

					//if (!bAdd)
					{
					//	m_ppSector[pCreature->GetY()][pCreature->GetX()].RemoveGroundCreature();
					//	bAdd = m_ppSector[pCreature->GetY()][pCreature->GetX()].AddGroundCreature(pCreature);		
					}
				break;

				case MCreature::CREATURE_FLYING :
					bAdd = m_ppSector[y][x].AddFlyingCreature(pCreature);		

					//if (!bAdd)
					{
					//	m_ppSector[pCreature->GetY()][pCreature->GetX()].RemoveFlyingCreature();
					//	bAdd = m_ppSector[pCreature->GetY()][pCreature->GetX()].AddFlyingCreature(pCreature);		
					}
				break;
			}
		}

		if (bAdd)
		{
			m_mapCreature.insert(CREATURE_MAP::value_type(pCreature->GetID(), pCreature));

			pCreature->SetZone( this );
			
			return true;
		}		

		DEBUG_ADD_FORMAT("[Error] AddCreature Failed!");

		return false;
	}

	// 이미 있는 Creature이면,
	DEBUG_ADD_FORMAT("Add Failed - Already Exist in Zone");

	return false;
}

	

//----------------------------------------------------------------------
// Zone에서 Creature 제거
//----------------------------------------------------------------------
// 내부에서 delete해준다.
//----------------------------------------------------------------------
bool
MZone::RemoveCreature(TYPE_OBJECTID id)
{
	DEBUG_ADD_FORMAT("Remove Creature. ID=%d", id);

	CREATURE_MAP::iterator	theIterator;

	// ID가 id인 Creature를 찾는다.
	theIterator = m_mapCreature.find(id);
    
	// 그런 id를 가진 Creature는 없다.	
	if (theIterator == m_mapCreature.end())
		return false;


	MCreature* pCreature = (*theIterator).second;

	
	//-------------------------------------------------------
	// 지금 제거 될려는 애가 player의 파티인 경우
	//-------------------------------------------------------
	/*
	if (pCreature->IsPlayerParty()
		&& g_pParty!=NULL)
	{
		PARTY_INFO* pInfo = g_pParty->GetMemberInfo( pCreature->GetName() );
			
		if (pInfo!=NULL)
		{
			pInfo->bInSight = false;
			pInfo->zoneID = 60002;
		}
	}
	*/
	
	//----------------------------------------
	// Sector에서 제거
	//----------------------------------------
	// player인 경우
	//----------------------------------------
	if (pCreature==NULL || pCreature->GetClassType()==MCreature::CLASS_PLAYER)
	{
		m_mapCreature.erase(theIterator);
		
		return true;
	}
	
	//----------------------------------------
	// 헬기 제거
	//----------------------------------------
	if (pCreature->IsSlayer())
	{
		m_HelicopterManager.RemoveHelicopter( id );
	}

	//----------------------------------------
	// player가 아닌 경우
	//----------------------------------------	
	int x			= pCreature->GetX();
	int y			= pCreature->GetY();
	int serverX		= pCreature->GetServerX();
	int serverY		= pCreature->GetServerY();

	// move type에 관계없이 ID를 다 찾아보자..
	/*
	switch (pCreature->GetMoveType())
	{
		case MCreature::CREATURE_GROUND :					
			removed = m_ppSector[y][x].RemoveGroundCreature( creatureID );
			if (!removed)
			{
				removed = m_ppSector[serverY][serverX].RemoveGroundCreature( creatureID );
			}
		break;

		case MCreature::CREATURE_FLYING :
			removed = m_ppSector[y][x].RemoveFlyingCreature( creatureID );
			if (!removed)
			{
				removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( creatureID );
			}
		break;

		case MCreature::CREATURE_UNDERGROUND :
			removed = m_ppSector[y][x].RemoveUndergroundCreature( creatureID );
			if (!removed)
			{
				removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( creatureID );
			}
		break;
	}
	*/

	// 찾은 경우 --> 제거		
	bool removed = true;
	
	//------------------------------------------------
	// sector에서 제거시킨다.
	//------------------------------------------------
	if (!m_ppSector[y][x].RemoveCreature(id))
	{
		if (!m_ppSector[serverY][serverX].RemoveCreature(id))
		{	
			DEBUG_ADD_FORMAT("Can't RemoveCreature! ID=%d client(%d,%d), server(%d,%d)", pCreature->GetID(), x,y,serverX,serverY);

			removed = false;
		}
	}

	/*
	removed = m_ppSector[y][x].RemoveGroundCreature( id );
	if (!removed)
	{
		removed = m_ppSector[serverY][serverX].RemoveGroundCreature( id );

		if (!removed)
		{
			removed = m_ppSector[y][x].RemoveFlyingCreature( id );

			if (!removed)
			{
				removed = m_ppSector[serverY][serverX].RemoveFlyingCreature( id );

				if (!removed)
				{
					removed = m_ppSector[y][x].RemoveUndergroundCreature( id );

					if (!removed)
					{
						removed = m_ppSector[serverY][serverX].RemoveUndergroundCreature( id );

						DEBUG_ADD_FORMAT("[Error] Can't Remove Creature. ID=%d. xy=(%d, %d), sxy=(%d, %d)", id, x, y, serverX, serverY);
					}
				}
			}
		}
	}
	*/

	// 서버좌표는 그냥 지워준다. 2002.3.29
	UnSetServerBlock( pCreature->GetMoveType(), pCreature->GetServerX(), pCreature->GetServerY() );


	// memory에서 제거 : Player가 아닐 경우에만 memory에서 제거
	if (removed)
	{
		//UnSetServerBlock( pCreature->GetMoveType(), pCreature->GetServerX(), pCreature->GetServerY() );

		delete pCreature;

		m_mapCreature.erase(theIterator);
	}			

	return removed;
}


//----------------------------------------------------------------------
// Zone의 Creature 읽어오기
//----------------------------------------------------------------------
// item에서도 찾는다. -_-;
//----------------------------------------------------------------------
MCreature*	
MZone::GetCreature(TYPE_OBJECTID id)
{
	CREATURE_MAP::iterator	theIterator;

	// ID가 id인 Creature를 찾는다.
	theIterator = m_mapCreature.find(id);

	// 없을 경우 NULL을 return한다.
	if (theIterator == m_mapCreature.end()) 
	{
		ITEM_MAP::iterator iItem = m_mapItem.find(id);

		if (iItem==m_mapItem.end())
		{
			return NULL;
		}

		MItem* pItem = iItem->second;

		if (pItem!=NULL
			&& pItem->GetItemClass()==ITEM_CLASS_CORPSE)
		{
			MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();

			if (pCreature!=NULL
				&& pCreature->GetActionCount() < pCreature->GetActionCountMax())
			{
				return pCreature;
			}
		}
		
		return NULL;
	}

	// 있으면 그 Creature를 return한다.
	return (*theIterator).second;
}

//----------------------------------------------------------------------
// Zone의 Creature 읽어오기
//----------------------------------------------------------------------
MCreature*	
MZone::GetCreatureOnly(TYPE_OBJECTID id)
{
	CREATURE_MAP::iterator	theIterator;

	// ID가 id인 Creature를 찾는다.
	theIterator = m_mapCreature.find(id);

	// 없을 경우 NULL을 return한다.
	if (theIterator == m_mapCreature.end()) 
	{
		return NULL;
	}

	// 있으면 그 Creature를 return한다.
	return theIterator->second;
}

//----------------------------------------------------------------------
// Zone에 ImageObject 추가 
//----------------------------------------------------------------------
// 외부에서 new해줘야 한다.
// 이미 있는지 확인을 하고 없으면 추가해야 한다.
//----------------------------------------------------------------------
bool		
MZone::AddImageObject(MImageObject* pImageObject)
{
	if(pImageObject != NULL)
	{
		IMAGEOBJECT_MAP::iterator	theIterator;

		theIterator = m_mapImageObject.find(pImageObject->GetID());
		
		// 아직 없는 ImageObject이면 추가	
		if (theIterator == m_mapImageObject.end())
		{
			m_mapImageObject.insert(IMAGEOBJECT_MAP::value_type(pImageObject->GetID(), pImageObject));

			return true;
		}

		// 이미 있는 ImageObject이면,
	}
	return false;
}

	

//----------------------------------------------------------------------
// Zone에서 ImageObject 제거
//----------------------------------------------------------------------
// 내부에서 delete해준다.
//----------------------------------------------------------------------
bool
MZone::RemoveImageObject(TYPE_OBJECTID	id)
{
	IMAGEOBJECT_MAP::iterator	theIterator;

	// ID가 id인 ImageObject를 찾는다.
	theIterator = m_mapImageObject.find(id);
    
	// 그런 id를 가진 ImageObject는 없다.	
	if (theIterator == m_mapImageObject.end())
		return false;


	// 찾은 경우 --> 제거	
	MImageObject* pImageObject = (*theIterator).second;	


	//???????????????????????????????????????????????????????
	//
	// ImageObject이 존재하는 Sector도 제거해야하지 않나??
	//
	//???????????????????????????????????????????????????????

	m_mapImageObject.erase(theIterator);

	// memory에서 제거
	delete pImageObject;	

	return true;
}

//----------------------------------------------------------------------
// Zone의 ImageObject 읽어오기
//----------------------------------------------------------------------
MImageObject*	
MZone::GetImageObject(TYPE_OBJECTID id)
{
	IMAGEOBJECT_MAP::iterator	theIterator;

	// ID가 id인 ImageObject를 찾는다.
	theIterator = m_mapImageObject.find(id);

	// 없을 경우 NULL을 return한다.
	if (theIterator == m_mapImageObject.end()) 
		return NULL;

	// 있으면 그 ImageObject를 return한다.
	return (*theIterator).second;
}

//----------------------------------------------------------------------
// SetImageObjectSector(sX,sY)
//----------------------------------------------------------------------
// Zone의 Sector에 ImageObject을 set하기
//----------------------------------------------------------------------
void		
MZone::SetImageObjectSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id)
{
	IMAGEOBJECT_MAP::iterator	theIterator;

	// ID가 id인 ImageObject를 찾는다.
	theIterator = m_mapImageObject.find(id);

	// 없을 경우 
	if (theIterator == m_mapImageObject.end()) 
		return;
	
	// (sX,sY) Sector에 ImageObject을 set한다.
	m_ppSector[sY][sX].AddImageObject((*theIterator).second);
}

//----------------------------------------------------------------------
// UnSetImageObjectSector(sX,sY)
//----------------------------------------------------------------------
// Zone의 Sector에 ImageObject을 UnSet하기
//----------------------------------------------------------------------
void		
MZone::UnSetImageObjectSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id)
{
	//-------------------------------------------------------
	// (sX,sY) Sector에 ImageObject을 set한다.
	//-------------------------------------------------------
	m_ppSector[sY][sX].RemoveImageObject(id);
}

//----------------------------------------------------------------------
// Zone에 Obstacle 추가
//----------------------------------------------------------------------
// 외부에서 new를 해줘야한다.
// Sector에 추가할 수 있으면 m_listObstacle에 추가한다.
//----------------------------------------------------------------------
/*
bool		
MZone::AddObstacle(MObstacle* pObstacle)
{	
	if (!m_ppSector[pObstacle->GetY()][pObstacle->GetX()].AddObstacle(pObstacle))
	{
		// 제대로 추가가 되지 않은 경우
		return false;
	}

	// Sector에 추가가 되었으니까 list에도 추가한다.
	// 나중에 memory에서 지우기 쉽도록 하기 위해서이다.
	m_listObstacle.push_front(pObstacle);

	return true;
}
*/

//----------------------------------------------------------------------
// Zone에서 Obstacle 제거		
//----------------------------------------------------------------------
// Sector(sX,sY)에 있는 Obstacle을 제거한다.
// map에서도 제거해야 한다.
//----------------------------------------------------------------------
// 게임 실행중에는 실제로 이 함수가 호출될 이유가 없다.
// Obstacle은 고정된 사물이기 때문이다.
//----------------------------------------------------------------------
//bool		
//MZone::RemoveObstacle(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
//{
	/*
	MObstacle* pObstacle;

	// 제대로 제거된 경우
	if (m_ppSector[sY][sX].RemoveObstacle(pObstacle))
	{
		// pObstacle에 제거된 MObstacle이 담겨있다.
		// ID가 같은 Obstacle을 mapObstacle에서 제거한다.
		OBSTACLE_MAP::iterator	iObstacle = m_mapObstacle.find(pObstacle->GetID());
    
		// 그런 id를 가진 ImageObjectPositionList는 없다.	
		if (iObstacle == m_mapObstacle.end())
			return false;

		// map에서 삭제
		m_mapObstacle.erase( iObstacle );

		// memory에서 제거
		delete pObstacle;
		
		return true;
	}

	// 제대로 제거되지 않은 경우 (왜일까?) - 없어서?
	*/
	//return false;
//}


//----------------------------------------------------------------------
// Change Frame Obstacle
//----------------------------------------------------------------------
// 임시 함수  :  모든 Obstacle의 Frame을 변화시킨다.
//----------------------------------------------------------------------
/*
void
MZone::ChangeFrameObstacle()
{
	OBSTACLE_LIST::iterator iObstacle = m_listObstacle.begin();

	MObstacle* pObstacle;
	while (iObstacle != m_listObstacle.end())
	{
		pObstacle = *iObstacle;

		pObstacle->NextFrame();

		iObstacle++;
	}
	
}
*/

//----------------------------------------------------------------------
// Sector (x,y)에 있는 Item의 ID를 넘겨준다.
//----------------------------------------------------------------------
TYPE_OBJECTID
MZone::GetItemID(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (//x<0 || y<0 || 
		x>=m_Width || y>=m_Height) return OBJECTID_NULL;

	// 뭔가 있는 sector이면
	MItem*	pItem = m_ppSector[y][x].GetItem();

	// Item가 있으면
	if (pItem != NULL)
	{			
		return pItem->GetID();
	}

	return OBJECTID_NULL;

	
}

//----------------------------------------------------------------------
// Zone에 Item 추가 
//----------------------------------------------------------------------
// 외부에서 new해줘야 한다.
// 이미 있는지 확인을 하고 없으면 추가해야 한다.
//----------------------------------------------------------------------
bool		
MZone::AddItem(MItem* pItem, BOOL bDropping)
{
	//--------------------------------------------------------	
	// 떨어뜨리는 Frame이 있는 경우...
	//--------------------------------------------------------	
	if (pItem->GetItemClass()!=ITEM_CLASS_MOTORCYCLE)	// 오토바이는 안 떨어진다. 소리도 안 난다.
	{
		if (bDropping 
			&& pItem->GetDropFrameID()!=FRAMEID_NULL)				// 떨어지는 그림이 없는 경우
		{
			pItem->SetDropping();
		}
		else
		{
			//--------------------------------------------------------	
			// 바로 떨어지는 경우는 sound출력
			//--------------------------------------------------------	
			//PlaySound( pItem->GetTileSoundID(),
			//					false,
			//					g_pPlayer->GetX(), g_pPlayer->GetY());
		}
	}

	//--------------------------------------------------------	
	// zone에 item추가
	//--------------------------------------------------------	
	ITEM_MAP::iterator	theIterator;

	theIterator = m_mapItem.find(pItem->GetID());
	
	// 아직 없는 Item이면 추가	
	if (theIterator == m_mapItem.end())
	{
		// Sector Setting [성물수정]
		MSector& sector = m_ppSector[pItem->GetY()][pItem->GetX()];

		if (sector.AddItem(pItem))
		{		
			m_mapItem.insert(ITEM_MAP::value_type(pItem->GetID(), pItem));

			// [성물수정] - 아이템인데 강제로 block시킨다.
			if (IsRelicTable(pItem))
			{
				sector.SetBlockGround();
				sector.SetBlockFlying();
			}

			bool bSpecialItem = false;

			// 아담의 성지 강제 이펙트 붙임
			if(pItem->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE)
			{
				bSpecialItem = true;
				ExecuteActionInfoFromMainNode(
					ARMEGA_TILE+pItem->GetItemType(),		// 값 자체가 RESULT_ACTIONINFO이다.
					
					pItem->GetX(), pItem->GetY(), 0,
					DIRECTION_DOWN,
					
					OBJECTID_NULL,
					pItem->GetX(), pItem->GetY(), 0,
					
					0xFFFF,	
					
					NULL
					);
			}

			// 아담의 성지 강제 이펙트 붙임
			if(pItem->GetItemClass() == ITEM_CLASS_CASTLE_SYMBOL)
			{
				int TempEffetType[6] = {	AMBER_OF_GUARD_TILE,
											AMBER_OF_AVENGER_TILE,
											AMBER_OF_IMMORTAL_TILE,
											AMBER_OF_CURSE_TILE,
											AMBER_OF_LIFE_TILE,
											AMBER_OF_LIGHT_TILE,
										};
				int TempItemType = pItem->GetItemType();

				if(TempItemType >5 ) TempItemType = 0;
				bSpecialItem = true;

				ExecuteActionInfoFromMainNode(
					TempEffetType[TempItemType],		// 값 자체가 RESULT_ACTIONINFO이다.
					
					pItem->GetX(), pItem->GetY(), 0,
					DIRECTION_DOWN,
					
					OBJECTID_NULL,
					pItem->GetX(), pItem->GetY(), 0,
					
					0xFFFF,	
					
					NULL
					);
			}

			if(bSpecialItem)
			{
				ExecuteActionInfoFromMainNode(
					DROP_BLOOD_BIBLE,		// 값 자체가 RESULT_ACTIONINFO이다.
					
					pItem->GetX(), pItem->GetY(), 0,
					DIRECTION_DOWN,
					
					OBJECTID_NULL,
					pItem->GetX(), pItem->GetY(), 0,
					
					0xFFFF,	
					
					NULL
					);
			}
			
			// 2004, 10, 25, sobeit add start - 드레곤 아이 이펙트 붙임
			if(pItem->GetItemClass() == ITEM_CLASS_WAR_ITEM)
			{
				int TempItemType = pItem->GetItemType();
				if(0 == TempItemType) // 드레곤 아이 일 때
				{
					ExecuteActionInfoFromMainNode(
						SKILL_CLIENT_DRAGON_EYES,		// 값 자체가 RESULT_ACTIONINFO이다.
						
						pItem->GetX(), pItem->GetY(), 0,
						DIRECTION_DOWN,
						
						OBJECTID_NULL,
						pItem->GetX(), pItem->GetY(), 0,
						
						0xFFFF,	
						
						NULL
						);
				}
			}
			// 2004, 10, 25, sobeit add end - 드레곤 아이 이펙트 붙임

			return true;
		}

		DEBUG_ADD_FORMAT("[Error] Already Exist Item id=%d, xy(%d,%d), cl=%d, ty=%d",
								pItem->GetID(), 
								pItem->GetX(), pItem->GetY(), 
								(int)pItem->GetItemClass(), 
								(int)pItem->GetItemType());
		

		return false;
	}

	// 이미 있는 Item이면,
	return false;
}
	

//----------------------------------------------------------------------
// Zone에서 Item 제거
//----------------------------------------------------------------------
// 내부에서 delete해준다.
//----------------------------------------------------------------------
bool
MZone::RemoveItem(TYPE_OBJECTID id)
{
	DEBUG_ADD_FORMAT("Remove Item. ID=%d", id);
	
	ITEM_MAP::iterator	theIterator;

	ITEM_MAP::iterator tempItr = m_mapItem.begin();
	TYPE_OBJECTID idd;
	while( tempItr != m_mapItem.end() )
	{
		 idd = (*tempItr).first;
		tempItr++;
	}

	// ID가 id인 Item를 찾는다.
	theIterator = m_mapItem.find(id);
    
	// 그런 id를 가진 Item는 없다.	
	if (theIterator == m_mapItem.end())
		return false;


	// 찾은 경우 --> 제거	

	// Sector에서 제거
	MItem* pItem = NULL;
	// [성물수정]

	int sector_y = (*theIterator).second->GetY(), sector_x = (*theIterator).second->GetX();
	MSector& sector = m_ppSector[sector_y][sector_x];
	bool removed = sector.RemoveItem(pItem);

	if (removed && pItem!=NULL)
	{
		// [성물수정]
		if (IsRelicTable(pItem))
		{
			sector.UnSetBlockGround();
			sector.UnSetBlockFlying();
		}
		bool bSpecialItem = false;
		
		// 아담의 성지 강제 이펙트 붙임
		if(pItem->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE)
		{
			bSpecialItem = true;
						
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)(EFFECTSPRITETYPE_ARMEGA_TILE + pItem->GetItemType()) ) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (ARMEGA_TILE HardCoding)");
			}
		}
		
		// 2004, 10, 25, sobeit add start - 드레곤 아이 이펙트 떼자
		if(pItem->GetItemClass() == ITEM_CLASS_WAR_ITEM)
		{
			if(0 == pItem->GetItemType())
			{
				if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)(EFFECTSPRITETYPE_DRAGON_EYE)) )
				{
					DEBUG_ADD("[Error] Can't Remove Effect From Tile (ITEM_CLASS_WAR_ITEM)");
				}
			}
		}
		// 2004, 10, 25, sobeit add end
		// 아담의 성지 강제 이펙트 붙임
		if(pItem->GetItemClass() == ITEM_CLASS_CASTLE_SYMBOL)
		{
			bSpecialItem = true;

			int TempEffetType[6] = {	EFFECTSPRITETYPE_AMBER_OF_GUARD_TILE,
										EFFECTSPRITETYPE_AMBER_OF_AVENGER_TILE,
										EFFECTSPRITETYPE_AMBER_OF_IMMORTAL_TILE,
										EFFECTSPRITETYPE_AMBER_OF_CURSE_TILE,
										EFFECTSPRITETYPE_AMBER_OF_LIFE,
										EFFECTSPRITETYPE_AMBER_OF_LIGHT,
										};
				int TempItemType = pItem->GetItemType();
				if(TempItemType >5 ) TempItemType = 0;
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,  (TYPE_EFFECTSPRITETYPE)(TempEffetType[TempItemType]) ) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (AMBER_OF_GUARD_TILE HardCoding)");
			}
		}
		
		if(bSpecialItem)
		{
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)EFFECTSPRITETYPE_DROP_BLOOD_BIBLE) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (DropBloodBible HardCoding)");
			}
		}

		m_mapItem.erase(theIterator);

		//------------------------------------------------------
		// Item Check Buffer에 있는 경우 지워줘야 한다. 2001.8.27
		//------------------------------------------------------
		MItem* pCheckItem = g_pPlayer->GetItemCheckBuffer();

		if (pCheckItem!=NULL)
		{
			DEBUG_ADD_FORMAT("Remove Item. ID=%d", id);

			if (pItem->GetID()==pCheckItem->GetID())
			{
				//---------------------------------------------
				// item check buffer를 없애준다.
				//---------------------------------------------
				g_pPlayer->ClearItemCheckBuffer();
			}
		}
		else
		{
			DEBUG_ADD("Check Item is NULL");
		}

		// memory에서 제거
		//delete pItem;	
		SAFE_DELETE ( pItem );
	}

	return removed;
}

//----------------------------------------------------------------------
// Zone에서 Item 제거
//----------------------------------------------------------------------
// zone에서 제거만 하고 delete하지 않는다.
//----------------------------------------------------------------------
bool
MZone::PickupItem(TYPE_OBJECTID id)
{
	ITEM_MAP::iterator	theIterator;

	// ID가 id인 Item를 찾는다.
	theIterator = m_mapItem.find(id);
    
	// 그런 id를 가진 Item는 없다.	
	if (theIterator == m_mapItem.end())
		return false;


	// 찾은 경우 --> 제거	

	// Sector에서 제거
	MItem* pItem = NULL;
	int sector_y = (*theIterator).second->GetY(), sector_x = (*theIterator).second->GetX();
	bool removed = m_ppSector[sector_y][sector_x].RemoveItem(pItem);

	bool bSpecialItem = false;

	if(pItem != NULL && removed)
	{
		// 아담의 성지 강제 이펙트 붙임
		if(pItem->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE)
		{
			bSpecialItem = true;
						
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)(EFFECTSPRITETYPE_ARMEGA_TILE + pItem->GetItemType() )) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (ARMEGA_TILE HardCoding)");
			}
		}
		
		// 2004, 10, 25, sobeit add start - 드레곤 아이 이펙트 떼자
		if(pItem->GetItemClass() == ITEM_CLASS_WAR_ITEM)
		{
			if(0 == pItem->GetItemType())
			{
				if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)(EFFECTSPRITETYPE_DRAGON_EYE)) )
				{
					DEBUG_ADD("[Error] Can't Remove Effect From Tile (ITEM_CLASS_WAR_ITEM)");
				}
			}
		}
		// 2004, 10, 25, sobeit add end
		
		// 아담의 성지 강제 이펙트 붙임
		if(pItem->GetItemClass() == ITEM_CLASS_CASTLE_SYMBOL)
		{
			bSpecialItem = true;

			int TempEffetType[6] = {	EFFECTSPRITETYPE_AMBER_OF_GUARD_TILE,
									EFFECTSPRITETYPE_AMBER_OF_AVENGER_TILE,
									EFFECTSPRITETYPE_AMBER_OF_IMMORTAL_TILE,
									EFFECTSPRITETYPE_AMBER_OF_CURSE_TILE,
									EFFECTSPRITETYPE_AMBER_OF_LIFE,
									EFFECTSPRITETYPE_AMBER_OF_LIGHT,
									};
			int TempItemType = pItem->GetItemType();
			if(TempItemType >5 ) TempItemType = 0;
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,  (TYPE_EFFECTSPRITETYPE)(TempEffetType[TempItemType] )) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (AMBER_OF_GUARD_TILE HardCoding)");
			}
		}
		
		if(bSpecialItem)
		{
			if (!g_pZone->RemoveTileEffect( sector_x,sector_y,   (TYPE_EFFECTSPRITETYPE)EFFECTSPRITETYPE_DROP_BLOOD_BIBLE) )
			{
				DEBUG_ADD("[Error] Can't Remove Effect From Tile (DropBloodBible HardCoding)");
			}
		}
	}


	if (removed)
	{
		m_mapItem.erase(theIterator);
	}

	return removed;
}

//----------------------------------------------------------------------
// Zone의 Item 읽어오기
//----------------------------------------------------------------------
MItem*	
MZone::GetItem(TYPE_OBJECTID id)
{
	ITEM_MAP::iterator	theIterator;

	// ID가 id인 Item를 찾는다.
	theIterator = m_mapItem.find(id);

	// 없을 경우 NULL을 return한다.
	if (theIterator == m_mapItem.end()) 
		return NULL;

	// 있으면 그 Item를 return한다.
	return (*theIterator).second;
}

//----------------------------------------------------------------------
// Zone에 Effect 추가
//----------------------------------------------------------------------
// m_mapEffect에 추가한다.
// Sector에 listEffect를 추가한다.
//----------------------------------------------------------------------
bool		
MZone::AddEffect(MEffect* pNewEffect, DWORD dwWaitCount)
{	
	DEBUG_ADD("AddEffect");

	if(dwWaitCount)
	{
		m_listWaitEffect.push_back( pNewEffect );
		return true;
	}

	int x = pNewEffect->GetX();
	int y = pNewEffect->GetY();

	bool bDarkNess = pNewEffect->GetFrameID() >= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_DARKNESS_1_1].FrameID &&
		pNewEffect->GetFrameID() <= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_DARKNESS_3_5].FrameID ||
		pNewEffect->GetFrameID() >= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_GRAY_DARKNESS_1_1].FrameID &&
		pNewEffect->GetFrameID() <= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_GRAY_DARKNESS_3_5].FrameID;

	bool bAcidSwamp = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_ACID_SWAP_1].FrameID <= pNewEffect->GetFrameID() &&
		(*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_ACID_SWAP_3].FrameID >= pNewEffect->GetFrameID();

	bool bProminence = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_PROMINENCE_ING].FrameID <= pNewEffect->GetFrameID() &&
		(*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_PROMINENCE_START].FrameID >= pNewEffect->GetFrameID() ||
		(*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_PROMINENCE2_ING].FrameID <= pNewEffect->GetFrameID() &&
		(*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_PROMINENCE3_START].FrameID >= pNewEffect->GetFrameID();


	if(bDarkNess)
	{
		const MSector& sector= m_ppSector[y][x];
		if(sector.HasDarknessForbidden() == true )
		{
			delete pNewEffect;
			return false;
		}
	}
	// 깃대 주변에는 다크니스가 깔리지 않는다.
	if( bDarkNess )
	{
		if( !( x < 0 || x >= GetWidth() || y < 0 || y >=GetHeight() ) )
		{
			const MSector& sector= m_ppSector[y][x];
			const MItem* pItem = sector.GetItem();
			
			// 깃대가 있는곳은 다크니스가 안찍힌다.
			if( pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_CORPSE &&
				((MCorpse*)pItem)->GetCreature() != NULL &&
				((MCorpse*)pItem)->GetCreature()->GetCreatureType() == 670 )
			{
				delete pNewEffect;
				return false;
			}
		}		
	}

	// 머시 그라운드가 이미 깔려 있으면 다크니스, 잔디, 불바다는 깔지 않는다.	
	if( bDarkNess || bAcidSwamp || bProminence )
	{
		EFFECT_MAP::iterator iGroundEffect = m_mapGroundEffect.begin();
		
		while (iGroundEffect != m_mapGroundEffect.end())
		{
			MEffect* pEffect = iGroundEffect->second;
			
			MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();
			TYPE_FRAMEID	frameID_s = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_MERCY_GROUND_1].FrameID;
			TYPE_FRAMEID	frameID_e = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_MERCY_GROUND_9].FrameID;
			TYPE_FRAMEID	regenTowerTile = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_REGEN_TOWER_GROUND].FrameID;
			TYPE_FRAMEID	summonClay = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_SUMMON_CLAY_LOOP].FrameID;
			
			// Mercy Ground 가 깔려 있으면 다크니스 찍지 않는다.
			if( pEffect->GetFrameID() >= frameID_s && pEffect->GetFrameID() <= frameID_e )
			{
				int fix_y =   ( pEffect->GetFrameID() - frameID_s) /3 -1;
				int fix_x = ( ( pEffect->GetFrameID() - frameID_s) %3) -1;

				if( pEffect->GetX() + fix_x == x &&	pEffect->GetY() + fix_y == y )
				{
					delete pNewEffect;
					return false;
				}
			} 
			else if( pEffect->GetFrameID() == regenTowerTile && x == pEffect->GetX() && y == pEffect->GetY() )
			{
				delete pNewEffect;
				return false;
			}
// 			// add by Coffee 藤속침쥣皐랬槻벎꼇콘굳페劍槻벎헌뇜
// 			else if( pEffect->GetFrameID() == summonClay && x == pEffect->GetX() && y == pEffect->GetY() )
// 			{
// 				delete pNewEffect;
// 				return false;
// 			}
			
			iGroundEffect++;
		}
	}
		

	//---------------------------------------------------------
	// Zone의 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x<0 || x>=m_Width
		|| y<0 || y>=m_Height)
	{
		DEBUG_ADD("OuterZone");

		// chase effect 는 넘어가도 괜찮다.
		if (pNewEffect->GetEffectType()==MEffect::EFFECT_CHASE)
		{
			TYPE_OBJECTID id = pNewEffect->GetID();

			// 없는 경우만 추가한다.
			if (m_mapEffect.find( id )==m_mapEffect.end())
			{
				// list에 추가
				m_mapEffect[id] = pNewEffect;				

				DEBUG_ADD("ChaseEffect");

				return true;
			}
		}

		DEBUG_ADD("deleteEff");

		delete pNewEffect;
		
		return false;
	}

	MSector &sector = m_ppSector[y][x];

	//---------------------------------------------------------
	// 기존의 Effect와 비교..
	//
	// Sector에 고정된 Effect인 경우만...
	//---------------------------------------------------------
	BOOL AddOK = TRUE;

	if (pNewEffect->GetEffectType()==MEffect::EFFECT_SECTOR && !pNewEffect->IsMulti())
	{
		DEBUG_ADD("getSector");

		
		if (sector.IsExistEffect())
		{
			EFFECT_LIST::const_iterator iEffect = sector.GetEffectIterator();
			
			int num = sector.GetEffectSize();

			DEBUG_ADD_FORMAT("secEffNum=%d", num);

			for (int i=0; i<num; i++)
			{
				MEffect* pEffect = *iEffect;

				//---------------------------------------------------------
				// 그림이 같다면 같은 Effect로 취급한다.
				// 시간이 긴 쪽을 잡는다.
				//---------------------------------------------------------
				// 같은 type을 찾는다.
				if (pEffect->GetEffectType()==MEffect::EFFECT_SECTOR)
				{
					int fid1 = pNewEffect->GetFrameID();
					int fid2 = pEffect->GetFrameID();

					int est1 = g_pTopView->GetEffectSpriteType( (BLT_TYPE)pNewEffect->GetBltType(), fid1 );
					int est2 = g_pTopView->GetEffectSpriteType( (BLT_TYPE)pEffect->GetBltType(), fid2 );
					
//					// 2004, 10, 19, sobeit add start 도슬 140 스킬 땜에
//					if(est1 == EFFECTSPRITETYPE_SPIT_STREAM ||
//						est1 == EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW ||
//						est1 == EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW_SHADOW ||
//						est1 == EFFECTSPRITETYPE_GREAT_RUFFIAN_1_AXE_GROUND ||
//						est1 == EFFECTSPRITETYPE_GREAT_RUFFIAN_1_AXE_WAVE
//						)
//					{
//						AddOK = TRUE; // 이 스킬은 같은 그림이 한타일에 두개 이상 다른위치에 나타날 수도 있다..-_-
//					}
//					else
//					// 2004, 10, 19, sobeit add end
					if (fid1 == fid2)
					{
						// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
						if (pNewEffect->GetEndFrame() > pEffect->GetEndFrame() &&
							est1 != EFFECTSPRITETYPE_DRAGON_FIRE_CRACKER) 
						{
							int count = pNewEffect->GetEndFrame() - g_CurrentFrame;
							
							pEffect->SetCount( count );
						}

						// 같은게 있다면 추가안함..
						AddOK = FALSE;
						
						if( est1 ==	EFFECTSPRITETYPE_DRAGON_FIRE_CRACKER )
							AddOK = TRUE;

						break;
					}
					// 또는 Darkness인 경우... 으흠~~ -_-;;
					else
					{					
						if (
							( est1>=EFFECTSPRITETYPE_DARKNESS_1_1
							&& est1<=EFFECTSPRITETYPE_DARKNESS_3_5
							&& est2>=EFFECTSPRITETYPE_DARKNESS_1_1
							&& est2<=EFFECTSPRITETYPE_DARKNESS_3_5

							&& (est1>=EFFECTSPRITETYPE_DARKNESS_2_1
								&& est1<=EFFECTSPRITETYPE_DARKNESS_2_5
								&& est2>=EFFECTSPRITETYPE_DARKNESS_2_1
								&& est2<=EFFECTSPRITETYPE_DARKNESS_2_5
								|| est1>=EFFECTSPRITETYPE_DARKNESS_1_1
								&& est1<=EFFECTSPRITETYPE_DARKNESS_1_5
								&& est2>=EFFECTSPRITETYPE_DARKNESS_1_1
								&& est2<=EFFECTSPRITETYPE_DARKNESS_1_5
								|| est1>=EFFECTSPRITETYPE_DARKNESS_3_1
								&& est1<=EFFECTSPRITETYPE_DARKNESS_3_5
								&& est2>=EFFECTSPRITETYPE_DARKNESS_3_1
								&& est2<=EFFECTSPRITETYPE_DARKNESS_3_5) 
								)||

							(	est1>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1
							&& est1<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5
							&& est2>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1
							&& est2<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5

							&& (est1>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1
								&& est1<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5
								&& est2>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1
								&& est2<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5
								|| est1>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1
								&& est1<=EFFECTSPRITETYPE_GRAY_DARKNESS_1_5
								&& est2>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1
								&& est2<=EFFECTSPRITETYPE_GRAY_DARKNESS_1_5
								|| est1>=EFFECTSPRITETYPE_GRAY_DARKNESS_3_1
								&& est1<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5
								&& est2>=EFFECTSPRITETYPE_GRAY_DARKNESS_3_1
								&& est2<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5)
								)								
							)
						{
							// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
							if (pNewEffect->GetEndFrame() > pEffect->GetEndFrame()) 
							{
								int count = pNewEffect->GetEndFrame() - g_CurrentFrame;
								
								pEffect->SetCount( count );
							}

							// 같은게 있다면 추가안함..
							AddOK = FALSE;

							// 반복 darkness인 경우에만..
							if (est1>=EFFECTSPRITETYPE_DARKNESS_2_1 &&
								est1<=EFFECTSPRITETYPE_DARKNESS_2_5 ||
								est1>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1 &&
								est1<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5)
							{
								sector.SetDarkness();
							}

						
							CheckCreatureInDarkness( sector, x, y );

							break;
						}	// [새기술]
							// sanctuary 있는 경우			
						else if (est1>=EFFECTSPRITETYPE_SANCTUARY_1
								&& est1<=EFFECTSPRITETYPE_SANCTUARY_3)
						{
							// 근처 타일들에도 체크한다. [새기술9]
							for (int i=-1; i<=1; i++)
							{							
								for (int j=-1; j<=1; j++)
								{	
									int x0 = x + i;
									int y0 = y + j;

									if (x0<0 || x0>=m_Width
										|| y0<0 || y0>=m_Height)
									{
										continue;
									}

									MSector& sector2 = m_ppSector[y0][x0];
									sector2.SetSanctuary();
								}
							}
						}

					}
				}

				iEffect++;
			}
		}
	}
			
	//---------------------------------------------------------
	// 추가해도 되는 경우
	//---------------------------------------------------------
	if (AddOK)
	{
		DEBUG_ADD("addOK");

		TYPE_OBJECTID id = pNewEffect->GetID();

		// 없는 경우만 추가한다.
		if (m_mapEffect.find( id )==m_mapEffect.end())
		{
			// list에 추가
			m_mapEffect[id] = pNewEffect;

			// sector에 추가
			MSector& sector = m_ppSector[y][x];

			DEBUG_ADD("secAddEff");
			sector.AddEffect( pNewEffect );

			DEBUG_ADD("chkDark");
			CheckCreatureInDarkness( sector, x, y );
			DEBUG_ADD("chkDarkOK");

			return true;
		}
	}

	// 시야 설정
//	if (pEffect->GetBltType()==BLT_EFFECT)
//	{
//		SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());		
//	}

	DEBUG_ADD("delEff");
	delete pNewEffect;

	DEBUG_ADD("reF");
	return false;
}

//----------------------------------------------------------------------
// Get Effect
//----------------------------------------------------------------------
MEffect*	
MZone::GetEffect(TYPE_OBJECTID id) const
{
	EFFECT_MAP::const_iterator iEffect = m_mapEffect.find( id );

	if (iEffect==m_mapEffect.end())
	{
		return NULL;
	}

	return iEffect->second;
}

//----------------------------------------------------------------------
// Remove Effect ( id )
//----------------------------------------------------------------------
// effect의 id는 거의 의미가 없지만..
// 억지로라도 지워야하는 경우가 있어서리.. - -;
//----------------------------------------------------------------------
bool		
MZone::RemoveEffect(TYPE_OBJECTID id)
{
	EFFECT_MAP::iterator iEffect = m_mapEffect.find( id );

	if (iEffect==m_mapEffect.end())
	{
		return false;
	}

	MEffect* pEffect = iEffect->second;

	int x = pEffect->GetX();
	int y = pEffect->GetY();

	// 경계를 넘지 않는 경우
	if (x>=0 && x<m_Width
		&& y>=0 && y<m_Height)
	{
		MSector& sector = m_ppSector[y][x];
		
		// [새기술9]
		RemoveSectorEffect( x, y, pEffect->GetID() );

		CheckCreatureInDarkness( sector, x, y );
	}

	delete pEffect;

	m_mapEffect.erase( iEffect );

	return true;
}

//----------------------------------------------------------------------
// Remove Tile Effect
//----------------------------------------------------------------------
// tile에 있는 effect를 지운다.
//----------------------------------------------------------------------
bool		
MZone::RemoveTileEffect(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, int effectStatus, int serverID)
{
	if (effectStatus >= g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus is NULL or Over: %d", effectStatus);

		return false;
	}

	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[effectStatus].EffectSpriteType;

	//----------------------------------------------------------------------
	// EffectStatus에 따라서 특별한것 체크
	//----------------------------------------------------------------------
	switch (effectStatus)
	{
		case EFFECTSTATUS_TRYING_POSITION :
		case EFFECTSTATUS_TRAP_INSTALLED :
		{
			bool bDelete = false;
			
			EFFECT_MAP::iterator iGroundEffect = m_mapGroundEffect.begin();
			
			while (iGroundEffect != m_mapGroundEffect.end())
			{
				MEffect* pEffect = iGroundEffect->second;
				
				MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[type].FrameID;

				if( pEffect->GetFrameID() == frameID &&
					pEffect->GetX() == sX &&
					pEffect->GetY() == sY )
				{
					delete pEffect;
					m_mapGroundEffect.erase( iGroundEffect );
					break;
				}
				
				iGroundEffect++;
			}
			
			DEBUG_ADD("EffectSpriteType of EffectStatus is NULL");
			
			return bDelete;
		}
		break;

		//----------------------------------------------------------------
		// vampire 포탈은 m_mapGroundEffect에 추가되어 있당..
		//----------------------------------------------------------------		
		case EFFECTSTATUS_VAMPIRE_PORTAL :
		{
			bool bDelete = false;

			EFFECT_MAP::iterator iGroundEffect = m_mapGroundEffect.begin();

			while (iGroundEffect != m_mapGroundEffect.end())
			{
				MEffect* pEffect = iGroundEffect->second;

				MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();

				if (pEffectTarget!=NULL)
				{
					//------------------------------------------------------------------
					// Portal인 경우
					//------------------------------------------------------------------
					if (pEffectTarget->GetEffectTargetType()==MEffectTarget::EFFECT_TARGET_PORTAL)
					{
						MPortalEffectTarget* pPortalEffectTarget = (MPortalEffectTarget*)pEffectTarget;

						int portalServerID = pPortalEffectTarget->GetServerID();
						
						if (serverID==portalServerID)
						{
							delete pEffect;

							m_mapGroundEffect.erase( iGroundEffect );

							break;
						}
					}
				}

				iGroundEffect++;
			}

			DEBUG_ADD("EffectSpriteType of EffectStatus is NULL");

			return bDelete;
		}
		break;	// - -;
		
		case EFFECTSTATUS_GROUND_ELEMENTAL_AURA:
			if( g_pZone != NULL )
			{
				const_cast<MSector &>(g_pZone->GetSector(sX, sY)).UnSetGroundElemental();
			}
			break;
		
		case EFFECTSTATUS_DARKNESS_FORBIDDEN:
			if( g_pZone != NULL )
			{
				const_cast<MSector &>(g_pZone->GetSector(sX, sY)).UnSetDarknessForbidden();
			}
			break;
			
		case EFFECTSTATUS_FURY_OF_GNOME:
			if( g_pZone != NULL )
			{
				const_cast<MSector &>(g_pZone->GetSector(sX, sY)).UnSetFuryOfGnome();
			}
			break;	
		case EFFECTSTATUS_DRAGON_TORNADO:
		case EFFECTSTATUS_DRAGON_TORNADO_CHILD:
			{
				return RemoveFakeCreature(serverID);
			}
			
		case EFFECTSTATUS_DELETE_TILE:
			if( g_pZone != NULL )
			{
				const_cast<MSector &>(g_pZone->GetSector(sX, sY)).UnSetDisableTileImage();
				if(g_pTopView != NULL)
					g_pTopView->SetFirstDraw();
			}
			break;	
			return true;
	}


	return RemoveTileEffect(sX, sY, type, serverID);

}



//----------------------------------------------------------------------
// Remove Tile Effect
//----------------------------------------------------------------------
// tile에 있는 effect를 지운다.
//----------------------------------------------------------------------
bool		
MZone::RemoveTileEffect(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_EFFECTSPRITETYPE type, int serverID)
{
	if (type==EFFECTSPRITETYPE_NULL)
	{
		DEBUG_ADD("[Error]EffectSpriteType of EffectStatus is NULL");

		return false;
	}

	TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;

	if (frameID==FRAMEID_NULL)
	{
		DEBUG_ADD("[Error]FrameID of EffectSpriteType is NULL");

		return false;
	}


	MSector& sector = m_ppSector[sY][sX];

	//--------------------------------------------------
	// effect가 있는 sector인지 확인
	//--------------------------------------------------
	if (sector.IsExistEffect())
	{
		int num = sector.GetEffectSize();

		bool isRemoveDarknessEffect = (frameID>=EFFECTSPRITETYPE_DARKNESS_1_1
								&& frameID<=EFFECTSPRITETYPE_DARKNESS_3_5) ||
								(frameID>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1 
								&& frameID <= EFFECTSPRITETYPE_GRAY_DARKNESS_3_5 
//								||	frameID >= EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE &&
//								frameID <= EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3
								);

		EFFECT_LIST::const_iterator iEffect = sector.GetEffectIterator();

		for (int i=0; i<num; i++)
		{
			MEffect* pEffect = *iEffect;

			//--------------------------------------------------
			// sector에 존재하는 effect만 체크해서 지운다.
			//--------------------------------------------------
			if (pEffect->GetEffectType() == MEffect::EFFECT_SECTOR)
			{
				bool isExistDarknessEffect = ((pEffect->GetFrameID()>=EFFECTSPRITETYPE_DARKNESS_1_1
											&& pEffect->GetFrameID()<=EFFECTSPRITETYPE_DARKNESS_3_5) ||
											pEffect->GetFrameID() >= EFFECTSPRITETYPE_GRAY_DARKNESS_1_1 &&
											pEffect->GetFrameID() <= EFFECTSPRITETYPE_GRAY_DARKNESS_3_5 
//											||	pEffect->GetFrameID() <= EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE &&
//											pEffect->GetFrameID() >= EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3;
											);
				//--------------------------------------------------
				// 같은 effect type인 경우
				//--------------------------------------------------
				if (pEffect->GetFrameID() == frameID

					// Darkness인 경우... 하드코딩.. - -;;
					|| isRemoveDarknessEffect
					&& isExistDarknessEffect					
					)
				{
					// sector에서 제거 [새기술9]
					RemoveSectorEffect( sX, sY, pEffect->GetID() );

					
					//--------------------------------------------------
					// zone의 list에서도 체크해서 지운다.
					//--------------------------------------------------
					// delete
					EFFECT_MAP::iterator iZoneEffect = m_mapEffect.begin();

					BOOL found = FALSE;
					while (iZoneEffect != m_mapEffect.end())
					{
						MEffect* pZoneEffect = iZoneEffect->second;

						// 같은 Effect이면..
						if (pZoneEffect->GetID()==pEffect->GetID())
						{
							m_mapEffect.erase( iZoneEffect );

							found = TRUE;

							break;
						}

						iZoneEffect++;
					}

					if (!found)
					{
						EFFECT_MAP::iterator iGroundEffect = m_mapGroundEffect.begin();

						BOOL found = FALSE;
						while (iGroundEffect != m_mapGroundEffect.end())
						{
							MEffect* pGroundEffect = iGroundEffect->second;

							// 같은 Effect이면..
							if (pGroundEffect->GetID()==pEffect->GetID())
							{
								m_mapGroundEffect.erase( iGroundEffect );

								found = TRUE;

								break;
							}

							iGroundEffect++;
						}
					}

					// 다크니스인 경우 그 타일에 있던 캐릭터를 보여준다.
					if (isRemoveDarknessEffect)
					{
						CheckCreatureInDarkness( sector, sX, sY );
					}

					delete pEffect;
					
					return true;
				}
			}

			iEffect++;
		}
	}

	DEBUG_ADD("EffectSpriteType of EffectStatus is NULL");
	
	return false;
}


//----------------------------------------------------------------------
// Change Frame Effect
//----------------------------------------------------------------------
// 임시 함수  :  모든 Effect의 Frame을 변화시킨다.
//----------------------------------------------------------------------
void
MZone::UpdateEffects()
{
	EFFECT_MAP::iterator iEffect = m_mapEffect.begin();

	EFFECT_MAP::iterator iTemp;
	
	MEffect* pEffect;
	int count = m_mapEffect.size();


	int		sX, sY;		// 변경 전의 좌표
	int		nX, nY;		// 변경 후의 좌표
	int		light;
	int		id;
	
//	bool bChangeSight;

	for (int i=0; i<count; i++)	
	{
		pEffect = iEffect->second;

		if(pEffect->IsDelayFrame() == true)
		{
			iEffect++;
			continue;
		}

		// 이전 좌표 기억
		id		= pEffect->GetID();
		sX		= pEffect->GetX();
		sY		= pEffect->GetY();
		light	= pEffect->GetLight();

		// 시야 바꿔야 하는가?
//		bChangeSight = pEffect->GetBltType()==BLT_EFFECT;

		BOOL sXYinZone = (sX>=0 && sX<m_Width && sY>=0 && sY<m_Height);
		
		//---------------------------------------
		//
		// 제대로 움직인 경우
		//
		//---------------------------------------
		if (pEffect->Update())
		{
			nX = pEffect->GetX();
			nY = pEffect->GetY();

			BOOL nXYinZone = (nX>=0 && nX<m_Width && nY>=0 && nY<m_Height);

			//---------------------------------------
			// chase effect가 아니고..
			// Zone의 경계를 넘어간 경우.. --> 제거한다.
			//---------------------------------------
			if (pEffect->GetEffectType()!=MEffect::EFFECT_CHASE
//				&& (pEffect->GetFrameID() <= EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE ||
//					pEffect->GetFrameID() >= EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3)
				&& !nXYinZone)
			{
				// 시야 제거
//				if (bChangeSight)
//				{
//					UnSetLight(sX, sY, light);
//				}

				if (sXYinZone)
				{
					// sector에서 제거 [새기술9]
					RemoveSectorEffect( sX, sY, id );
				}
										
				#ifdef OUTPUT_DEBUG_UPDATE_EFFECT
					DEBUG_ADD("delete pEffect0");
					// memory삭제							
					delete pEffect;			
					DEBUG_ADD("delete OK");
				#else
					delete pEffect;
				#endif
				
				iTemp = iEffect;
				iEffect++;

				// list에서 제거한다.
				m_mapEffect.erase(iTemp);	

				continue;
			}

			//-----------------------------------------------------------
			// Effect의 종류에 따라서 처리를 해준다.
			//-----------------------------------------------------------
			// Sector에 고정되어 있는 Effect
			//-----------------------------------------------------------
			if (pEffect->GetEffectType() == MEffect::EFFECT_SECTOR)
			{
				// 빛나는 Effect이고, 빛의 크기가 달라졌으면..
//				if (bChangeSight && light!=pEffect->GetLight())
//				{
//					UnSetLight(sX, sY, light);
//					SetLight(sX, sY, pEffect->GetLight());
//				}				
			}
			//-----------------------------------------------------------
			// 움직이는 Effect
			//-----------------------------------------------------------
			else
			{
				// 직선 Effect인 경우 --> Sector좌표에 따라 Sector에 체크~해야한다.
				// 이전 좌표와 비교하여 
				//case MEffect::EFFECT_LINEAR :
				//case MEffect::EFFECT_PARABOLA :
				//case MEffect::EFFECT_GUIDANCE :
				//case MEffect::EFFECT_ATTACH :

				//------------------------------------------------------
				// 좌표가 달라졌으면..
				//------------------------------------------------------
				if (sX!=nX || sY!=nY)
				{
					//------------------------------------------------------
					// Block되는 Sector에 들어갔을 경우 제거한다.
					//------------------------------------------------------
					//
					// Block의 종류에 따라 다르지 않을까??
					//
					// 공중만 ...
					if (0)
						// 서버에서 block을 체크하지 않기 때문에
						// block을 무시한다.
						//
						//pEffect->GetEffectType()!=MEffect::EFFECT_CHASE
						//&& nXYinZone && m_ppSector[nY][nX].IsBlockFlying())						
					{
						// 시야 제거
//						if (bChangeSight)
//						{
//							UnSetLight(sX, sY, light);
//						}

						// sector에서 제거	
						if (sXYinZone)
						{
							// [새기술9]
							RemoveSectorEffect(sX, sY, id);
						}
													
						// memory삭제							
						#ifdef OUTPUT_DEBUG_UPDATE_EFFECT
							DEBUG_ADD("delete pEffect1");
							// memory삭제							
							delete pEffect;			
							DEBUG_ADD("delete OK");
						#else
							delete pEffect;
						#endif
						
						iTemp = iEffect;
						iEffect++;

						// list에서 제거한다.
						m_mapEffect.erase(iTemp);	

						continue;
					}

					// 시야 다시 설정
//					if (bChangeSight)
//					{
//						UnSetLight(sX, sY, light);
//						SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());
//					}

					// 이전 좌표에서 빼서 
					if (sXYinZone)
					{
						// [새기술9]
						RemoveSectorEffect(sX, sY, id);
					}

					//새로운 좌표에 추가한다.
					if (nXYinZone)
					{
						m_ppSector[nY][nX].AddEffect( pEffect );
					}
				}		
				//------------------------------------------------------
				// 빛의 크기(시야)가 바뀌었으면..
				//------------------------------------------------------
				else if (light != pEffect->GetLight())
				{
					// 시야 다시 설정
//					if (bChangeSight)
//					{
//						UnSetLight(sX, sY, light);
//						SetLight(sX, sY, pEffect->GetLight());
//					}
				}
			}

			//-----------------------------------------------
			//
			// 이 Effect가 끝나기 전에 LinkCount에 의해서
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			// 현재Frame이 EndLinkFrame을 넘어간 경우
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext에서 
				// pEffect의 EffectTarget을 NULL로 만들어주기 때문에
				// 여기서 지울 필요 없다.
				#ifdef OUTPUT_DEBUG_UPDATE_EFFECT
					DEBUG_ADD("GenerateNext1");
					g_pEffectGeneratorTable->GenerateNext( pEffect );
					DEBUG_ADD("GenerateNext1 OK");
				#else
					g_pEffectGeneratorTable->GenerateNext( pEffect );
				#endif

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}
			
			
			iEffect++;
		}
		//---------------------------------------
		//
		// 다 움직인 경우 = 제거해야 하는 경우 
		//
		//---------------------------------------
		else
		{
			//-----------------------------------------------
			//
			//     Effect 제거
			//
			//-----------------------------------------------
			// 시야 제거			
			//if (bChangeSight)
			//{
				//UnSetLight(sX, sY, light);
			//}			
			//-----------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//-----------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{
				#ifdef OUTPUT_DEBUG_UPDATE_EFFECT
					DEBUG_ADD("GenerateNext2");
					g_pEffectGeneratorTable->GenerateNext( pEffect );
					DEBUG_ADD("GenerateNext2 OK");
				#else
					g_pEffectGeneratorTable->GenerateNext( pEffect );
				#endif
			}		
			
			// sector에서 제거	
			if (sXYinZone)
			{
				MSector& sector = m_ppSector[sY][sX];
				
				// [새기술9]
				RemoveSectorEffect( sX, sY, id );

				CheckCreatureInDarkness( sector, sX, sY );				
			}
							
			#ifdef OUTPUT_DEBUG_UPDATE_EFFECT
				DEBUG_ADD("delete pEffect2");
				// memory삭제							
				delete pEffect;			
				DEBUG_ADD("delete OK");
			#else
				delete pEffect;
			#endif

			iTemp = iEffect;
			iEffect++;

			// list에서 제거한다.
			m_mapEffect.erase(iTemp);	
		}		
	}	
}



//----------------------------------------------------------------------
// Zone에 Effect 추가
//----------------------------------------------------------------------
// m_mapGroundEffect에 추가한다.
// GroundEffect는 pixel좌표만을 가지므로 
// Sector좌표를 체크할 필요가 없다.
//----------------------------------------------------------------------
bool		
MZone::AddGroundEffect(MEffect* pEffect)
{	
	TYPE_OBJECTID id = pEffect->GetID();
	BOOL	AddOK = TRUE;
	
	if( m_mapGroundEffect.empty() )
		AddOK = TRUE;
	else
	{
		EFFECT_MAP::iterator Itr = m_mapGroundEffect.begin();
		EFFECT_MAP::iterator endItr = m_mapGroundEffect.end();
		
		int NewEffectX = pEffect->GetX();
		int NewEffectY = pEffect->GetY();
		
		while( Itr != endItr )
		{
			MEffect *pOldEffect = Itr->second;			
			
			if(NewEffectX == pOldEffect->GetX() && NewEffectY == pOldEffect->GetY())
			{
				int est1 = g_pTopView->GetEffectSpriteType( (BLT_TYPE) pEffect->GetBltType(), pEffect->GetFrameID() );
				int est2 = g_pTopView->GetEffectSpriteType( (BLT_TYPE) pOldEffect->GetBltType(), pOldEffect->GetFrameID() );

				if( est1 == EFFECTSPRITETYPE_SUMMON_GROUND_ELEMENTAL_END && est2 == EFFECTSPRITETYPE_SUMMON_GROUND_ELEMENTAL_ING)
				{
					pOldEffect->SetCount( 0 );
				}

				if( est1 == EFFECTSPRITETYPE_MAGIC_ELUSION_ING && est2 == EFFECTSPRITETYPE_MAGIC_ELUSION_ING ||
					est1 >= EFFECTSPRITETYPE_MERCY_GROUND_1 && est1 <= EFFECTSPRITETYPE_MERCY_GROUND_9 &&
					est1 == est2)
				{
					if( pEffect->GetEndFrame() > pOldEffect->GetEndFrame() )
					{
						int count = pEffect->GetEndFrame() - g_CurrentFrame;
						pOldEffect->SetCount( count );
					}
					AddOK = FALSE;
				}
				else if ( est2 == EFFECTSPRITETYPE_REGEN_TOWER_GROUND && 
					(est1 == EFFECTSPRITETYPE_ICE_FIELD_2 ||
					est1 == EFFECTSPRITETYPE_ICE_FIELD_1 ||
					est1 == EFFECTSPRITETYPE_ICE_FIELD_3
					)
					)
				{
					AddOK = FALSE;
				}
// 				else if ( est1 == EFFECTSPRITETYPE_SUMMON_CLAY_LOOP || 
// 						  est1 == EFFECTSPRITETYPE_SUMMON_CLAY_START ||
// 						  est1 == EFFECTSPRITETYPE_SUMMON_CLAY_END)
// 				{
// 					AddOK = FALSE;
// 				}
			}
			if(AddOK == FALSE)
				break;			
			Itr++;
		}
	}
	if(AddOK == TRUE)
	{
		m_mapGroundEffect[id] = pEffect;
	}		
		/*
		//---------------------------------------------------------
		// 선택가능한 Effect는 sector좌표가 바뀌지 않는다고 가정.
		//---------------------------------------------------------
		if (pEffect->IsSelectable())
		{	
			int x = pEffect->GetX();
			int y = pEffect->GetY();

			//---------------------------------------------------------
			// Zone의 경계를 넘지 않는 경우만 Sector에 추가한다.
			//---------------------------------------------------------
			if (x<0 || x>=m_Width
				|| y<0 || y>=m_Height)
			{		
			}
			else
			{
				m_ppSector[y][x].AddEffect( pEffect );
			}
		}
		*/

	return true;
//	return false;
}

//----------------------------------------------------------------------
// Get GroundEffect
//----------------------------------------------------------------------
MEffect*	
MZone::GetGroundEffect(TYPE_OBJECTID id) const
{
	EFFECT_MAP::const_iterator iEffect = m_mapGroundEffect.find( id );

	if (iEffect==m_mapGroundEffect.end())
	{
		return NULL;
	}

	return iEffect->second;
}

//----------------------------------------------------------------------
// Remove GroundEffect ( id )
//----------------------------------------------------------------------
// effect의 id는 거의 의미가 없지만..
// 억지로라도 지워야하는 경우가 있어서리.. - -;
//----------------------------------------------------------------------
bool		
MZone::RemoveGroundEffect(TYPE_OBJECTID id)
{
	EFFECT_MAP::iterator iEffect = m_mapGroundEffect.find( id );

	if (iEffect==m_mapGroundEffect.end())
	{
		return false;
	}

	MEffect* pEffect = iEffect->second;

	/*
	//---------------------------------------------------------
	// 선택가능한 Effect는 sector좌표가 바뀌지 않는다고 가정.
	//---------------------------------------------------------
	if (pEffect->IsSelectable())
	{
		int x = pEffect->GetX();
		int y = pEffect->GetY();

		//---------------------------------------------------------
		// Zone의 경계를 넘지 않는 경우만 Sector에 추가한다.
		//---------------------------------------------------------
		if (x<0 || x>=m_Width
			|| y<0 || y>=m_Height)
		{		
		}
		else
		{
			m_ppSector[y][x].RemoveEffect( pEffect->GetID() );
		}
	}
	*/

	delete pEffect;

	m_mapGroundEffect.erase( iEffect );

	return true;
}


//----------------------------------------------------------------------
// Update GroundEffects
//----------------------------------------------------------------------
// 임시 함수  :  모든 GroundEffect의 Frame을 변화시킨다.
//----------------------------------------------------------------------
void
MZone::UpdateGroundEffects()
{
	EFFECT_MAP::iterator iEffect = m_mapGroundEffect.begin();

	EFFECT_MAP::iterator iTemp;
	
	MEffect* pEffect;
	int count = m_mapGroundEffect.size();


	int		light;
	int		id;
	
	for (int i=0; i<count; i++)	
	{
		pEffect = iEffect->second;

		// 이전 좌표 기억
		id		= pEffect->GetID();
		light	= pEffect->GetLight();

		//---------------------------------------
		//
		// 제대로 움직인 경우
		//
		//---------------------------------------
		if (pEffect->Update())
		{
			//-----------------------------------------------
			//
			// 이 Effect가 끝나기 전에 LinkCount에 의해서
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			// 현재Frame이 EndLinkFrame을 넘어간 경우
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext에서 
				// pEffect의 EffectTarget을 NULL로 만들어주기 때문에
				// 여기서 지울 필요 없다.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}

			// 다음..	
			iEffect++;
		}
		//---------------------------------------
		//
		// 다 움직인 경우 = 제거해야 하는 경우 
		//
		//---------------------------------------
		else
		{
			//-----------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//-----------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{
				g_pEffectGeneratorTable->GenerateNext( pEffect );
			}				
			
			//---------------------------------------------------------
			// 선택가능한 Effect는 sector좌표가 바뀌지 않는다고 가정.
			//---------------------------------------------------------
			/*
			if (pEffect->IsSelectable())
			{
				int x = pEffect->GetX();
				int y = pEffect->GetY();

				//---------------------------------------------------------
				// Zone의 경계를 넘지 않는 경우만 Sector에서 제거
				//---------------------------------------------------------
				if (x<0 || x>=m_Width
					|| y<0 || y>=m_Height)
				{		
				}
				else
				{
					m_ppSector[y][x].RemoveEffect( pEffect->GetID() );
				}
			}
			*/
												
			// memory삭제							
			delete pEffect;			

			iTemp = iEffect;
			iEffect++;

			// list에서 제거한다.
			m_mapGroundEffect.erase(iTemp);	
		}		
	}	
}

//----------------------------------------------------------------------
// Get Near SpriteSet
//----------------------------------------------------------------------
// (sX,sY) 근처의 Tile과 ImageObject의 SpriteSet을 검색해서 넘겨준다.
//----------------------------------------------------------------------
void						
MZone::GetNearSpriteSet(CSpriteSetManager& TileSSM, CSpriteSetManager& ImageObjectSSM, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY) const
{
	//----------------------------------------------------------------------
	//
	//                         ImageObject 검색
	//
	//----------------------------------------------------------------------	
	POINT firstSector;
	firstSector.x = sX + g_SECTOR_SKIP_PLAYER_LEFT*3;
	firstSector.y = sY + g_SECTOR_SKIP_PLAYER_UP*3;
	int sX1 = firstSector.x + SECTOR_SKIP_LEFT*3;
	int sY1 = firstSector.y + SECTOR_SKIP_UP*3;
	int sX2 = firstSector.x + g_SECTOR_WIDTH*3;
	int sY2 = firstSector.y + g_SECTOR_HEIGHT*3;
	
	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{					
		//tilePoint.x += -sX1 * TILE_X;
		sX1 = 0;	
	}

	if (sX2 >= m_Width)
	{
		sX2 = m_Width-1;
	}

	if (sY1 < 0)
	{
		//tilePoint.y += -sY1 * TILE_Y;
		sY1 = 0;	
	}

	if (sY2 >= m_Height)
	{
		sY2 = m_Height-1;
	}
		
	DEBUG_ADD_FORMAT("GetNearSpriteSet: (%d, %d) ~ (%d, %d)", sX1, sY1, sX2, sY2);

	//------------------------------------------------------
	// 각 Sector의 ImageObject검색
	//------------------------------------------------------
	int y,x,i;

	int maxTile = g_pTopView->m_TileSPK.GetSize();

	for (y=sY1; y<=sY2; y++)
	{				
		for (x=sX1; x<=sX2; x++)
		{				
			const MSector& sector = m_ppSector[y][x];

			//--------------------------------------------
			// Tile SpriteID를 저장한다.
			//--------------------------------------------
			int spriteID = sector.GetSpriteID();

			if (spriteID < maxTile)
			{
				TileSSM.Add( spriteID );
			}

			//--------------------------------------------
			// ImageObject가 있다면.. 
			//--------------------------------------------
			if (sector.IsExistImageObject())
			{
				OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

				//--------------------------------------------				
				// Sector에 있는 모든 ImageObject들을 검색한다.
				// 각 ImageObject의 SpriteID를 모두 추가한다.
				//--------------------------------------------
				for (i=0; i<sector.GetImageObjectSize(); i++)
				{
					MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
					
					if (pImageObject->GetSpriteID() != SPRITEID_NULL)
					{
						ImageObjectSSM.Add( pImageObject->GetSpriteID() );
					}
					
					// Frame이 있는 경우... Frame의 SpriteID도 검색 
					if (pImageObject->IsAnimation())
					{
						DEBUG_ADD_FORMAT("AnimationObject: (%d, %d)",x, y);

						int fid = ((MAnimationObject*)pImageObject)->GetFrameID();

						if(fid < g_pTopView->m_ImageObjectFPK.GetSize())
						{
							FRAME_ARRAY* pFrameArray = &(g_pTopView->m_ImageObjectFPK[ fid ]);
							
							// 각 Frame마다 SpriteID를 검색한다.
							for (int j=0; j<pFrameArray->GetSize(); j++)
							{
								if ((*pFrameArray)[j].GetSpriteID() != SPRITEID_NULL)
								{
									ImageObjectSSM.Add( (*pFrameArray)[j].GetSpriteID() );
								}
							}
						}
					}
					
					
					iImageObject++;
				}
			}
		}
	}
}


//----------------------------------------------------------------------
// Add Sound
//----------------------------------------------------------------------
// 어느 시점에 출력할 Sound를 저장해둔다.
//----------------------------------------------------------------------
void
MZone::AddSound(SOUND_NODE* pNode)
{
	m_listSoundNode.push_back( pNode );
}

//----------------------------------------------------------------------
// Update Sound
//----------------------------------------------------------------------
// 어느 시점에 출력할 Sound를 체크해서 소리를 낸다.
//----------------------------------------------------------------------
void
MZone::UpdateSound()
{
	SOUND_NODE_LIST::iterator iSound = m_listSoundNode.begin();

	while (iSound != m_listSoundNode.end())
	{
		SOUND_NODE*	pNode = *iSound;

		//-------------------------------------------------
		// 소리를 출력할 시간이 지났으면.. PlaySound
		//-------------------------------------------------
		if (pNode->GetPlayTime() < g_CurrentTime)
		{
			// 한번만 소리를 낸다.
			PlaySound( pNode->GetSoundID(), false, pNode->GetX(), pNode->GetY() );

			// 지운다.
			delete pNode;

			// 잠시 기억해뒀다가 지운다.
			SOUND_NODE_LIST::iterator iTemp = iSound;
			iSound++;
			m_listSoundNode.erase( iTemp );
		}
		//-------------------------------------------------
		// 아닌 경우.. 다음 소리 체크..
		//-------------------------------------------------
		else
		{
			iSound++;
		}	
	}

	//----------------------------------------------------------------
	// Zone에서 random으로 나는 소리
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	// 헬기장 소리..
	// 하드코딩.. 우헤헤.. 나중에 빼야된다.
	//----------------------------------------------------------------
	int zoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);
			
	if (zoneID==2106 || zoneID==2004 || zoneID==2014 || zoneID==2024)
	{
		if (!g_bPlayPropeller)// && g_CurrentTime > g_ZoneRandomSoundTime)
		{
			int x, y;
			x = g_pPlayer->GetX();// + ((rand()%2)? 1 : -1) * (rand()%7 + 4);
			y = g_pPlayer->GetY();// + ((rand()%2)? 1 : -1) * (rand()%6 + 2);

			PlaySound( SOUND_WORLD_PROPELLER, true, x, y );
			g_bPlayPropeller = TRUE;

			//g_ZoneRandomSoundTime = 0x0FFFFFFF;		// -_-;;
		}
	}
	else
	{
		if (g_bPlayPropeller)
		{
			StopSound( SOUND_WORLD_PROPELLER );

			g_bPlayPropeller = FALSE;
		}

		if (g_CurrentTime > g_ZoneRandomSoundTime)
		{
			ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( zoneID );

			if (pZoneInfo!=NULL)
			{
				int soundID = pZoneInfo->GetRandomSoundID();			

				int x, y;

				x = g_pPlayer->GetX() + ((rand()%2)? 1 : -1) * (rand()%15 + 13);
				y = g_pPlayer->GetY() + ((rand()%2)? 1 : -1) * (rand()%12 + 10);

				PlaySound( soundID, false, x, y );
			}
			
			// 10~30초 후에 다시 소리 낸다
			g_ZoneRandomSoundTime = g_CurrentTime + ((rand()%10)+6)*1000;			
		}
	}
}

//----------------------------------------------------------------------
// Update Item
//----------------------------------------------------------------------
// Item이 시체인 경우.. - -;
//----------------------------------------------------------------------
void
MZone::UpdateItem()
{
	ITEM_MAP::iterator iItem = m_mapItem.begin();

	while (iItem!= m_mapItem.end())
	{
		MItem*	pItem = (*iItem).second;

		//----------------------------------------------------------------------
		// 시체인 경우.. 시체에 붙어있는 Effect를 처리해준다.
		//----------------------------------------------------------------------
		if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
		{
			MCorpse* pCorpse = (MCorpse*)pItem;

			MCreature* pCreature = pCorpse->GetCreature();

			if (pCreature!=NULL)
			{
				if (pCreature->GetNextAction()!=ACTION_STAND
					|| pCreature->GetActionCount()<pCreature->GetActionCountMax()
					|| pCreature->GetCreatureType() == 672 )
				{
					pCreature->Action();
				}
				pCreature->UpdateAttachEffect();
			}
		}

		//----------------------------------------------------------------------
		// 떨어지고 있는 Item인 경우..
		//----------------------------------------------------------------------
		else if (pItem->IsDropping())
		{
			pItem->NextDropFrame();

			//--------------------------------------------------------
			// 다 떨어진 경우
			//--------------------------------------------------------
			if (!pItem->IsDropping())
			{
				PlaySound( pItem->GetTileSoundID(),
							false,
							g_pPlayer->GetX(), g_pPlayer->GetY());
			}
		}
		
		iItem++;		
	}

}


//----------------------------------------------------------------------
// Zone에 FakeCreature 추가 
//----------------------------------------------------------------------
// 외부에서 new해줘야 한다.
// 이미 있는지 확인을 하고 없으면 추가해야 한다.
//----------------------------------------------------------------------
bool		
MZone::AddFakeCreature(MCreature* pCreature)
{
	DEBUG_ADD_FORMAT("MZone::AddFakeCreature id = %d", pCreature->GetID());

	CREATURE_MAP::iterator	theIterator;

	theIterator = m_mapFakeCreature.find(pCreature->GetID());
	
	// 아직 없는 Creature이면 추가	
	if (theIterator == m_mapFakeCreature.end())
	{
		m_mapFakeCreature.insert(CREATURE_MAP::value_type(pCreature->GetID(), pCreature));
		
		return true;
	}		

	// 이미 있는 Creature이면,
	DEBUG_ADD_FORMAT("Add Failed - Already Exist in Zone");

	return false;
}

	

//----------------------------------------------------------------------
// Zone에서 FakeCreature 제거
//----------------------------------------------------------------------
// 내부에서 delete해준다.
//----------------------------------------------------------------------
bool
MZone::RemoveFakeCreature(TYPE_OBJECTID id)
{
	CREATURE_MAP::iterator	theIterator;

	// ID가 id인 Creature를 찾는다.
	theIterator = m_mapFakeCreature.find(id);
    
	// 그런 id를 가진 Creature는 없다.	
	if (theIterator == m_mapFakeCreature.end())
		return false;


	MFakeCreature* pFakeCreature = (MFakeCreature *)theIterator->second;

	if(pFakeCreature != NULL)
	{
		// 펫인경우
		if(pFakeCreature->GetOwnerID() != OBJECTID_NULL)
		{
			MCreature *pCreature = GetCreature(pFakeCreature->GetOwnerID());

			if(pCreature != NULL)
			{
				if(pCreature->GetPetID() == pFakeCreature->GetID())
					pCreature->SetPetID(OBJECTID_NULL);
				else if(pCreature->GetElementalID() == pFakeCreature->GetID())
					pCreature->SetElementalID(OBJECTID_NULL);
			}
		}

		delete pFakeCreature;
	}

	m_mapFakeCreature.erase(theIterator);

	return true;
}


//----------------------------------------------------------------------
// Zone의 Creature 읽어오기
//----------------------------------------------------------------------
MCreature*	
MZone::GetFakeCreature(TYPE_OBJECTID id)
{
	CREATURE_MAP::iterator	theIterator;

	// ID가 id인 Creature를 찾는다.
	theIterator = m_mapFakeCreature.find(id);

	// 없을 경우 NULL을 return한다.
	if (theIterator == m_mapFakeCreature.end()) 
	{
		return NULL;
	}

	// 있으면 그 Creature를 return한다.
	return (*theIterator).second;
}

//----------------------------------------------------------------------
// Update FakeCreature
//----------------------------------------------------------------------
void		
MZone::UpdateFakeCreature()
{
	CREATURE_MAP::iterator	iCreature = m_mapFakeCreature.begin();

	MCreature* pCreature;
	
	//	DEBUG_ADD_FORMAT("[UpdateAllCreature] %d Creature(s)", m_mapCreature.size());
	
	//------------------------------------------------------
	// 모든 Creature에 대해서 Action
	//------------------------------------------------------
	while (iCreature != m_mapFakeCreature.end())
	{
		pCreature = iCreature->second;

		//------------------------------------------------------
		// MFakeCreature인 경우만..
		//------------------------------------------------------
		if (pCreature->GetClassType()==MCreature::CLASS_FAKE)
		{
			MFakeCreature* pFakeCreature = (MFakeCreature*)pCreature;

			pFakeCreature->Action();
			pFakeCreature->UpdateAttachEffect();

			//------------------------------------------------------
			// FakeCreature가 사라지는 경우
			//------------------------------------------------------
			if (pFakeCreature->IsFakeEnd())
			{	
				CREATURE_MAP::iterator	iCreatureTemp = iCreature;
				iCreature++;

				m_mapFakeCreature.erase( iCreatureTemp );

				delete pFakeCreature;

				continue;
			}
		}

		iCreature++;
	}
}

//----------------------------------------------------------------------
// Add Helicopter
//----------------------------------------------------------------------
bool
MZone::AddHelicopter(TYPE_OBJECTID creatureID, int x0, int y0)
{
	MHelicopter* pHelicopter = new MHelicopter;

	if (!pHelicopter->SetChaseCreature( creatureID, x0, y0 )
		|| !m_HelicopterManager.AddHelicopter( pHelicopter ))
	{
		DEBUG_ADD("Can't Add Helicopter");

		delete pHelicopter;

		return false;
	}

	return true;
}

//----------------------------------------------------------------------
// Get Helicopter
//----------------------------------------------------------------------
MHelicopter*	
MZone::GetHelicopter(TYPE_OBJECTID creatureID) const
{
	return m_HelicopterManager.GetHelicopter(creatureID);
}

//----------------------------------------------------------------------
// Remove Helicopter
//----------------------------------------------------------------------
void		
MZone::RemoveHelicopter(TYPE_OBJECTID creatureID)
{
	m_HelicopterManager.RemoveHelicopter( creatureID );
}

//----------------------------------------------------------------------
// Change To HalluCreature
//----------------------------------------------------------------------
void		
MZone::ChangeToHalluCreature()
{
	CREATURE_MAP::iterator	iCreature = m_mapCreature.begin();

	MCreature* pCreature;

	DEBUG_ADD("Zone::ChangeToHalluCreature");
	
	//------------------------------------------------------
	// 모든 Creature에 대해서 Action
	//------------------------------------------------------
	while (iCreature != m_mapCreature.end())
	{
		pCreature = iCreature->second;

		// player가 아닌 경우에...
		if (pCreature!=g_pPlayer)
		{
			pCreature->SetHalluCreature( g_pTopView->GetRandomMonsterTypeInZone() );
		}

		iCreature ++;
	}

	DEBUG_ADD("Zone::ChangeToHalluCreature OK");

}

//----------------------------------------------------------------------
// Remove HalluCreature
//----------------------------------------------------------------------
void		
MZone::RemoveHalluCreature()
{
	CREATURE_MAP::iterator	iCreature = m_mapCreature.begin();

	MCreature* pCreature;

	DEBUG_ADD("Zone::RemoveHalluCreature");
	
	//------------------------------------------------------
	// 모든 Creature에 대해서 Action
	//------------------------------------------------------
	while (iCreature != m_mapCreature.end())
	{
		pCreature = iCreature->second;

		// player가 아닌 경우에...
		if (pCreature!=g_pPlayer)
		{
			pCreature->UnSetHalluCreature();
		}

		iCreature ++;
	}

	DEBUG_ADD("Zone::RemoveHalluCreature OK");
/*
	ITEM_MAP::iterator	iItem = m_mapItem.begin();

	MItem *pItem;

	while (iItem != m_mapItem.end())
	{
		pItem = iItem->second;

		if (pItem != NULL && pItem->GetItemClass()==ITEM_CLASS_CORPSE)
		{
			MCorpse* pCorpse = (MCorpse*)pItem;
			
			MCreature* pCreature = pCorpse->GetCreature();
			
			if (pCreature!=NULL && pCreature != g_pPlayer)
			{
				pCreature->UnSetHalluCreature();
			}
		}

		iItem++;
	}

	DEBUG_ADD("Zone::RemoveHalluCorpse OK");
*/
}

//----------------------------------------------------------------------
// Set Safe Sector
//----------------------------------------------------------------------
void						
MZone::SetSafeSector( const RECT& rect, BYTE fSafe )
{
	int top = rect.top;
	int left = rect.left;
	int bottom = rect.bottom;
	int right = rect.right;

	if (top < 0) top = 0;
	if (left < 0) left = 0;
	if (bottom >= m_Height) bottom = m_Height-1;
	if (right >= m_Width) right = m_Width-1;

	if (top > bottom) { int temp=top; top=bottom; bottom=temp; }
	if (left > right) { int temp=left; left=right; right=left; }

	for (int i=top; i<=bottom; i++)
	{
		for (int j=left; j<=right; j++)
		{
			m_ppSector[i][j].SetSafe( fSafe );
		}
	}
}

MItem *
MZone::GetCorpseKilledByMe(int limitItemCount)
{
	ITEM_MAP::iterator itr = m_mapItem.begin();
	ITEM_MAP::iterator endItr = m_mapItem.end();

	while(itr != endItr)
	{
		MItem* pItem = itr->second;

		if (pItem!=NULL
			&& pItem->GetItemClass()==ITEM_CLASS_CORPSE && pItem->GetNumber() > limitItemCount)
		{
			MCreature* pCreature = ((MCorpse*)pItem)->GetCreature();

			if (pCreature!=NULL
				&& pCreature->GetDrainCreatureID() == g_pPlayer->GetID())
			{
				return pItem;
			}
		}

		itr++;
	}

	return NULL;
}

BYTE			
MZone::GetPKType()
{
	if(g_pZoneTable)
	{
		ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( GetID() );
		if(NULL != pZoneInfo)
			return pZoneInfo->PKType;
	}

	return 0;
}


// 2004, 8, 30, sobeit add start - sweep vice test
//----------------------------------------------------------------------
// ChangeSwapViceType
//----------------------------------------------------------------------
// tile에 있는 SwapViceType을 변경 한다.
//----------------------------------------------------------------------
void		
MZone::ChangeSwapViceType(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_EFFECTSPRITETYPE type, WORD wDelay)
{
	EFFECT_MAP::iterator iEffect = m_mapGroundEffect.begin();

	MEffect *pEffect = NULL;
	while (iEffect != m_mapGroundEffect.end())
	{
		pEffect = iEffect->second;
		if (pEffect != NULL)
		{
			if( pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_1 ||
					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_2 ||
					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_3 )
				{
					pEffect->SetFrameID(type, pEffect->GetMaxFrame());
					pEffect->SetDelayFrame(wDelay);
					return;
			}
		}
		iEffect++;
	}

//	
//
//	MSector& sector = m_ppSector[sY][sX];
//
//	//--------------------------------------------------
//	// effect가 있는 sector인지 확인
//	//--------------------------------------------------
//	if (sector.IsExistEffect())
//	{
//		int num = sector.GetEffectSize();
//
//		EFFECT_LIST::const_iterator iEffect = sector.GetEffectIterator();
//
//		for (int i=0; i<num; i++)
//		{
//			MEffect* pEffect = *iEffect;
//
//			//--------------------------------------------------
//			// sector에 존재하는 effect만 체크해서 지운다.
//			//--------------------------------------------------
//			if (pEffect->GetEffectType() == MEffect::EFFECT_SECTOR)
//			{
//				if( pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_1 ||
//					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_2 ||
//					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_3 )
//				{
//					pEffect->SetFrameID(type, pEffect->GetMaxFrame());
//					pEffect->SetDelayFrame(wDelay);
//					return;
//				}
//			}
//
//			iEffect++;
//		}
//	}
	return ;
}

void		
MZone::RemoveSwapViceType()
{
	EFFECT_MAP::iterator iEffect = m_mapGroundEffect.begin();

	MEffect *pEffect = NULL;
	while (iEffect != m_mapGroundEffect.end())
	{
		pEffect = iEffect->second;
		if (pEffect != NULL)
		{
			if( pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_1 ||
					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_2 ||
					pEffect->GetFrameID() == EFFECTSPRITETYPE_SWEEP_VICE_PRECASTING_3 )
				{
					
//				// list에서 제거
					iEffect = m_mapGroundEffect.erase( iEffect );
					// 메모리 제거
					delete pEffect;

					continue;
			}
		}
		iEffect++;
	}

}
// 2004, 8, 30, sobeit add end - sweep vice test

// 2004, 8, 31, sobeit add start
void
MZone::UpdateWaitEffects()
{
	WAIT_EFFECT_LIST::iterator iWaitEffect = m_listWaitEffect.begin();

	while (iWaitEffect != m_listWaitEffect.end())
	{
		MEffect*	pNode = *iWaitEffect;
		if( !pNode->IsWaitFrame())
		{
			AddEffect( pNode );
			iWaitEffect = m_listWaitEffect.erase(iWaitEffect);
		}
		else
			iWaitEffect ++;
	}
}
// 2004, 8, 31, sobeit add end
