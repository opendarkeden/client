//----------------------------------------------------------------------
// MZone.h
//----------------------------------------------------------------------
//
// Creature : ID로 관리된다. 마구 움직인다. - -;;
// Item     : ID로 관리된다. 사라졌다가(creature가 pickup) 생기기도 한다(drop)
// Obstacle : Sector에 고정된다. 움직이지 않는다.
// ImageObject : ??
//
//----------------------------------------------------------------------
// File에 저장된 정보
//----------------------------------------------------------------------
//
// [ MapData : Filename.MD ]   - Zone에서 Load한다.
//   가로 size, 세로 size
//   Zone ID
//   속성
//   가로size * 세로size 개의 Sector정보(TileSpriteID, Flag)
//   Obstacle수, Obstacle수 * Obstacle 정보
//   ImageObject수, ImageObject수 * (ImageObject, ImageObjectSectorInfo)
//
//
//----------------------------------------------------------------------
// On-line Update
//----------------------------------------------------------------------
//
// File을 하나로 관리하게 되면, 
// File의 중간에 data를 추가할 수 없으므로 File전체를 Load했다가
// Memory에서 Update하고 File전체를 Save해야한다.
// (!)속도 문제가 있을 수 있다.
//
// 그렇다고, 각각의 Data부분들을 따로 file로 두면 지저분하당. - -;
// Zone 하나가 생성되었을 때, 더 이상 data가 바뀌지 않는다고 하면..
// 하나의 File로 사용해도 문제가 없을 것이다.
//
//----------------------------------------------------------------------

#ifndef	__MZONE_H__
#define	__MZONE_H__

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include <list>
#include <map>
#include <vector>
#include "MTypeDef.h"
class ofstream;
class ifstream;
#include "MSector.h"
class MObstacle;
class MItem;
class MCreature;
class MEffect;
class MPlayer;
class MImageObject;
class CSpriteSetManager;
#include "MHelicopterManager.h"
#include "CPositionList.h"
#include "ZoneFileHeader.h"
#include "SoundNode.h"

struct UI_PORTAL_FLAG
{
	int zone_id, x, y, portal_x, portal_y;
};
typedef std::list<UI_PORTAL_FLAG> UI_PORTAL_LIST;
typedef std::vector<UI_PORTAL_LIST> UI_PORTAL_VECTOR;

//----------------------------------------------------------------------
//
// Zone class는 Sector의 2차원 배열이다.
//
//----------------------------------------------------------------------
class MZone {
	//--------------------------------------------------------------		
	// typedef
	//--------------------------------------------------------------	
	public :
		// building map
		typedef std::map<TYPE_OBJECTID, MImageObject*>				IMAGEOBJECT_MAP;

		// IMAGEOBJECT_POSITION_LIST를 정의한다.
		typedef	CPositionList<TYPE_SECTORPOSITION>							IMAGEOBJECT_POSITION_LIST;

		// IMAGEOBJECT_POSITION_LIST의 map이다.
		typedef	std::map<TYPE_OBJECTID, IMAGEOBJECT_POSITION_LIST*>	IMAGEOBJECT_POSITION_LIST_MAP;


		// creature map
		typedef std::map<TYPE_OBJECTID, MCreature*>		CREATURE_MAP;

		// Item map	
		typedef std::map<TYPE_OBJECTID, MItem*>			ITEM_MAP;

		// Obstacle list
		//typedef std::list<MObstacle*>					OBSTACLE_LIST;

		// Effect list
		typedef std::map<TYPE_OBJECTID, MEffect*>		EFFECT_MAP;

		// Sound list
		typedef std::list<SOUND_NODE*>					SOUND_NODE_LIST;

		// 2004, 8, 31, sobeit add start - WaitEffectList
		// Wiait Effect list
		typedef std::list<MEffect*>						WAIT_EFFECT_LIST;
		// 2004, 8, 31, sobeit add end - WaitEffectList
	public :
		MZone(TYPE_SECTORPOSITION width=0, TYPE_SECTORPOSITION height=0);
		~MZone();

		//--------------------------------------------------------------		
		// init / release
		//--------------------------------------------------------------		
		void		Init(TYPE_SECTORPOSITION width, TYPE_SECTORPOSITION height);
		void		Release();
		
		//--------------------------------------------------------------
		// Update
		//--------------------------------------------------------------
		void		Update();

		//--------------------------------------------------------------
		// Release Object
		//--------------------------------------------------------------
		void		ReleaseObject();


		//--------------------------------------------------------------
		// file I/O
		//--------------------------------------------------------------		
		bool		LoadFromFile(class ifstream& file);

		bool		SaveToFileSectorSound(class ofstream& file);
		bool		LoadFromFileSectorSound(class ifstream& file);

		//--------------------------------------------------------------		
		// Zone File Header 부분
		//--------------------------------------------------------------		
		const MString&	GetVersion() const			{ return m_Info.ZoneVersion; } // 최종 업데이트된 날짜
		WORD			GetID() const				{ return m_Info.ZoneID; }			//	존 아이디
		WORD			GetGroupID() const			{ return m_Info.ZoneGroupID; }	//	존 그룹 아이디
		const MString&	GetName() const				{ return m_Info.ZoneName; }		// 존 이름
		BYTE			GetType() const				{ return m_Info.ZoneType; }		//	존의 타입
		BYTE			GetLevel() const			{ return m_Info.ZoneLevel; }		//	존에 나오는 몬스터의 레벨. 1-10 사이.
		const MString&	GetDescription() const		{ return m_Info.Description; }	//	존	



		//--------------------------------------------------------------
		//
		// Creature
		//
		//--------------------------------------------------------------
		void		SetPlayer();//MPlayer* pPlayer);
		void		RemovePlayer();
		
		// Creature Map : Zone에 존재하는 전체 creature 수
		int			GetCreatureNumber() const		{ return m_mapCreature.size(); }
		CREATURE_MAP::const_iterator GetCreatureBegin()		{ return m_mapCreature.begin(); }

		// Move
		void		UpdateAllCreature();

		// player의 시야를 벗어나는 Object들을 제거한다.
		void		KeepObjectInSight(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE sight);

		// (x,y) sector에 들어갈 수 있는가?
		bool		CanMove(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		void		SetServerBlock(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		void		UnSetServerBlock(BYTE creatureType, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);

		// (xo,yo)에 있던 Creature를 (xn,yn)으로 옮긴다.		
		bool		MoveGroundCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn);
		bool		MoveFlyingCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn);
		bool		MoveUndergroundCreature(MCreature* pCreature, TYPE_SECTORPOSITION xo, TYPE_SECTORPOSITION yo, TYPE_SECTORPOSITION xn, TYPE_SECTORPOSITION yn);

		// (x,y) Sector에 있는 Creature의 ID를 넘겨준다.
		
		TYPE_OBJECTID		GetCreatureID(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		TYPE_OBJECTID		GetFlyingCreatureID(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);
		TYPE_OBJECTID		GetUndergroundCreatureID(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);

		TYPE_OBJECTID		GetCreatureID(const char* pName, int flag = 0) const;

		// other creatures : Add, Get, Remove
		bool		AddCreature(MCreature* pCreature);	// Zone에 Creature 추가
		bool		RemoveCreature(TYPE_OBJECTID	id);			// Zone에서 Creature 제거
		MCreature*	GetCreature(TYPE_OBJECTID id);				// Zone의 Creature 읽어오기
		MCreature*	GetCreatureOnly(TYPE_OBJECTID id);

		// 2004, 8, 12 sobeit add start
		MCreature*  GetCreatureBySector(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		// 2004, 8, 12 sobeit add end
		//MCreature*	GetUndergroundCreature(TYPE_OBJECTID id);				// Zone의 UndergroundCreature 읽어오기
		//MCreature*	GetFlyingCreature(TYPE_OBJECTID id);				// Zone의 FlyingCreature 읽어오기

		// Sector에서 Creature 제거
		void		RemoveUndergroundCreature(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id)	{ m_ppSector[sY][sX].RemoveUndergroundCreature(id); }
		void		RemoveGroundCreature(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id)		{ m_ppSector[sY][sX].RemoveGroundCreature(id); }
		void		RemoveFlyingCreature(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id)		{ m_ppSector[sY][sX].RemoveFlyingCreature(id); }

		// Hallu관련
		void		ChangeToHalluCreature();
		void		RemoveHalluCreature();


		//--------------------------------------------------------------
		//
		//							Item
		//
		//--------------------------------------------------------------
		bool		AddItem(MItem* pItem, BOOL bDropping=false);				// Zone에 Item 추가, bDropping은 떨어지기 시작하는것인가?를 나타냄..
		bool		RemoveItem(TYPE_OBJECTID	id);	// Zone에서 Item 제거(delete해줌)
		bool		PickupItem(TYPE_OBJECTID	id);	// Zone에서 Item 제거
		MItem*		GetItem(TYPE_OBJECTID id);			// Zone의 Item 읽어오기
		MItem*		GetCorpseKilledByMe(int limitItemCount = 0);				// Zone에서 내가 죽인 몬스터 하나 넘겨준다.
		
		// (x,y) Sector에 있는 Item의 ID를 넘겨준다.
		TYPE_OBJECTID		GetItemID(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);	

		// Creature가 시체로 변하는 것... - -;
		//bool		AddCorpseFromCreature(MCreature* pCreature);
		bool		AddCorpseFromCreature(TYPE_OBJECTID id);

		// Item Map : Zone에 존재하는 전체 Item 수
		int			GetItemNumber() const		{ return m_mapItem.size(); }
		ITEM_MAP::const_iterator GetItemBegin()		{ return m_mapItem.begin(); }

		// update item
		void		UpdateItem();


		//--------------------------------------------------------------
		//
		//							ImageObject
		//
		//--------------------------------------------------------------
		bool		AddImageObject(MImageObject* pImageObject);	// Zone에 ImageObject 추가
		bool		RemoveImageObject(TYPE_OBJECTID	id);			// Zone에서 ImageObject 제거
		MImageObject*	GetImageObject(TYPE_OBJECTID id);				// Zone의 ImageObject 읽어오기
		void		SetImageObjectSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id);	// ImageObject Sector
		void		UnSetImageObjectSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_OBJECTID id);	// Unset ImageObject Sector	

		//--------------------------------------------------------------
		//
		//							Obstacle
		//
		//--------------------------------------------------------------
		//bool		AddObstacle(MObstacle* pObstacle);	// Zone에 Obstacle 추가
		//bool		RemoveObstacle(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY);	// Zone에서 Obstacle 제거
		//void		ChangeFrameObstacle();				// Frame변화 (임시함수!!!)

		//--------------------------------------------------------------
		//
		//							 Effect
		//
		//--------------------------------------------------------------
		bool		AddEffect(MEffect* pEffect, DWORD dwWaitCount = 0);	// Zone에 Effect 추가	
		bool		RemoveEffect(TYPE_OBJECTID id);
		void		UpdateEffects();				// Frame변화 (임시함수!!!)
		// 2004, 8, 31, sobeit add start
		void		UpdateWaitEffects();
		// 2004, 8, 31, sobeit add end
		MEffect*	GetEffect(TYPE_OBJECTID id) const;
		int			GetEffectCount()				{ return m_mapEffect.size(); }
		EFFECT_MAP::const_iterator GetEffects()		{ return m_mapEffect.begin(); }
		bool		RemoveTileEffect(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, int effectStatus, int serverID=0);
		bool		RemoveTileEffect(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_EFFECTSPRITETYPE type, int serverID=0);
		void		ChangeSwapViceType(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_EFFECTSPRITETYPE type, WORD wDelay = 0xffff);
		void		RemoveSwapViceType();
		//--------------------------------------------------------------
		//						Ground Effect - Tile과 붙음..
		//--------------------------------------------------------------
		bool		AddGroundEffect(MEffect* pEffect);	// Zone에 Effect 추가	
		bool		RemoveGroundEffect(TYPE_OBJECTID id);
		MEffect*	GetGroundEffect(TYPE_OBJECTID id) const;
		void		UpdateGroundEffects();				// Frame변화 (임시함수!!!)
		int			GetGroundEffectCount()				{ return m_mapGroundEffect.size(); }
		EFFECT_MAP::const_iterator GetGroundEffects()	{ return m_mapGroundEffect.begin(); }
		EFFECT_MAP::const_iterator GetGroundEffectsEnd()	{ return m_mapGroundEffect.end(); }

		//--------------------------------------------------------------
		//
		//						 Item
		//
		//--------------------------------------------------------------
		void		AddSound(SOUND_NODE* pNode);	// 소리 추가
		void		UpdateSound();					// 소리 출력


		//--------------------------------------------------------------
		//
		//							Sector
		//
		//--------------------------------------------------------------		
		bool		IsPositionInZone(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)	{ return x<m_Width && y<m_Height; }

		void		SetSector(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, const MSector& sector)	{ m_ppSector[y][x]=sector; }

		void		SetBlockAllSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)				{ m_ppSector[sY][sX].SetBlockAll(); }		// All Block sector로 만든다.
		void		UnSetBlockAllSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)			{ m_ppSector[sY][sX].UnSetBlockAll(); }	// All Block sector로 만든다.

		void		SetBlockUndergroundSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)		{ m_ppSector[sY][sX].SetBlockUnderground(); }		// UndergroundBlock sector 설정
		void		UnSetBlockUndergroundSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	{ m_ppSector[sY][sX].UnSetBlockUnderground(); }	// UndergroundBlock sector 해제

		void		SetBlockGroundSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)			{ m_ppSector[sY][sX].SetBlockGround(); }	// Ground Block sector 설정
		void		UnSetBlockGroundSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)		{ m_ppSector[sY][sX].UnSetBlockGround(); }	// Ground Block sector 해제

		void		SetBlockFlyingSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	{ m_ppSector[sY][sX].SetBlockFlying(); }					// Flying Block sector로 만든다.
		void		UnSetBlockFlyingSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	{ m_ppSector[sY][sX].UnSetBlockFlying(); }			// Flying Block sector로 만든다.

		void		SetVisitedFlag(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)				{ m_ppSector[sY][sX].SetVisitedFlag(); }
		void		UnSetVisitedFlag(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)			{ m_ppSector[sY][sX].UnSetVisitedFlag(); }
		const bool	IsVisitedFlag(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY) const		{ return m_ppSector[sY][sX].IsVisitedFlag(); }
		
		//--------------------------------------------------------------		
		//
		//		get
		//
		//--------------------------------------------------------------		
		TYPE_SECTORPOSITION			GetWidth() const				{ return m_Width; }
		TYPE_SECTORPOSITION			GetHeight()	const				{ return m_Height; }
		const MSector&	GetSector(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y) const	{ return m_ppSector[y][x]; }
		//const MPlayer&	GetPlayer()	const				{ return *m_pPlayer; }
	
		long						GetTileFilePosition() const			{ return m_fpTile; }
		long						GetImageObjectFilePosition() const	{ return m_fpImageObject; }

		//--------------------------------------------------------------
		// Tile SpriteID 얻기
		//--------------------------------------------------------------
		void						GetNearSpriteSet(CSpriteSetManager& TileSSM, CSpriteSetManager& ImageObjectSSM, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY) const;


		//--------------------------------------------------------------		
		//
		// portal
		//
		//--------------------------------------------------------------		
		void						AddPortal(int type, int zoneID, const RECT& rect);

		//--------------------------------------------------------------		
		//
		// Safe
		//
		//--------------------------------------------------------------		
		void						SetSafeSector( const RECT& safeRect, BYTE fSafe );


		//--------------------------------------------------------------
		//
		// Fake Creature
		//
		//--------------------------------------------------------------
		// Creature Map : Zone에 존재하는 전체 creature 수
		int			GetFakeCreatureNumber() const		{ return m_mapFakeCreature.size(); }
		CREATURE_MAP::const_iterator GetFakeCreatureBegin()		{ return m_mapFakeCreature.begin(); }

		// Move
		void		UpdateFakeCreature();

		// other creatures : Add, Get, Remove
		bool		AddFakeCreature(MCreature* pCreature);	// Zone에 Creature 추가
		bool		RemoveFakeCreature(TYPE_OBJECTID id);			// Zone에서 Creature 제거
		MCreature*	GetFakeCreature(TYPE_OBJECTID id);				// Zone의 Creature 읽어오기		
	
		//--------------------------------------------------------------
		// ZoneSound
		//--------------------------------------------------------------
		bool		IsZoneSoundLoaded() const		{ return m_bZoneSoundLoaded; }

		//--------------------------------------------------------------
		// HelicopterManager
		//--------------------------------------------------------------
		bool			AddHelicopter(TYPE_OBJECTID creatureID, int x0, int y0);
		MHelicopter*	GetHelicopter(TYPE_OBJECTID creatureID) const;
		void			RemoveHelicopter(TYPE_OBJECTID creatureID);
		UI_PORTAL_VECTOR &	GetHorn() { return m_horn; }
		
		void			SetCurrentZoneID( WORD id ) { m_Info.ZoneID = id; }			// 아아아 왜 map 에 zoneid 가 들어있는거얏!!

		BYTE			GetPKType();

		
	protected :
		// Zone File Header
		FILEINFO_ZONE_HEADER	m_Info;

		TYPE_SECTORPOSITION					m_Width;		// Zone의 가로 크기
		TYPE_SECTORPOSITION					m_Height;		// Zone의 세로 크기
		MSector**				m_ppSector;		// 각각의 Sector

		// Other Creature
		CREATURE_MAP			m_mapCreature;	// other creatures	

		// Other Creature
		ITEM_MAP				m_mapItem;		// Item map

		// ImageObjects
		IMAGEOBJECT_MAP			m_mapImageObject;	// ImageObject map

		// Obstacle
		//OBSTACLE_LIST			m_listObstacle;	// Obstacle List

		// Effects
		EFFECT_MAP				m_mapEffect;	// Effect map

		// Effects
		EFFECT_MAP				m_mapGroundEffect;	// Effect map

		// Zone에 필요한 그림 정보에 대한 File Position
		long					m_fpTile;
		long					m_fpImageObject;

		SOUND_NODE_LIST			m_listSoundNode;

		// client에서만 돌아가는 서비스용 creature - -;;
		CREATURE_MAP			m_mapFakeCreature;

		// sound가 load되었나.. 케케..
		bool					m_bZoneSoundLoaded;

		MHelicopterManager		m_HelicopterManager;
		UI_PORTAL_VECTOR		m_horn;
		
		// 2004, 8, 31, sobeit add start
		WAIT_EFFECT_LIST		m_listWaitEffect;
		// 2004, 8, 31, sobeit add end
};

// Zone
extern MZone*						g_pZone;

#endif


