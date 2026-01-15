//----------------------------------------------------------------------
// MSector.h
//----------------------------------------------------------------------
//
// - Object Map을 사용한다.
//
// - Effect는 Object이긴 하지만, 구조상..
//   Effect만 List로 따로 관리하는게 낫다..
//
//----------------------------------------------------------------------
// Flag를 이용해서 Sector에 현재 존재하는 Object들의 정보를 저장해두면,
// 속도가 빠를 것이다!
//----------------------------------------------------------------------
// 한 Sector에는 여러개의 ImageObject가 존재할 수 있다.
// 단, Map의 구조상.. 게임 실행시에 추가된 것이 삭제가 되면 안된다.
// 
// 
// list보다는 map이 낫지 않을까??
//
// 특정 key값을 통해서 map에 추가/삭제가 가능하다.
//----------------------------------------------------------------------
//
// Map에서 사용될 Key값은 다음과 같이 정의한다.
//
//	1			POSITION_ITEM = 1,					// Item이 있는 Node
//	2			POSITION_UNDERGROUNDCREATURE,		// 지하 캐릭터가 있는 Node
//	3			POSITION_GROUNDCREATURE,			// 지상 캐릭터가 있는 Node
//	4			POSITION_FLYINGCREATURE,			// 공중 캐릭터가 있는 Node
//	5			POSITION_PORTAL,					// Zone 이동이 되는 곳이다.
//	100 ~255	POSITION_IMAGEOBJECT				// ImageObject들이 있는 곳
//
//
//----------------------------------------------------------------------
// 
// [시야처리에 관련된 것]
//
// m_Light는 이 Sector에 표현되는 빛의 밝기를 의미한다.
// m_Light가 0이면 m_FilterSpriteID를 이용해서 Fog를 표현해야한다.
// m_Light가 1 이상일때는 Fog가 없어야 한다.
//
// m_Light가 단순히 true/false가 아닌 이유는..
// 한 Sector에는 여러개의 빛이 동시에 존재할 수 있으므로 
// 중첩됐다가 제거할 때.. 여러번 제거하게 되면.. 
// 아직 빛이 존재하는데도 0이 될 수가 있기 때문이다.
// 
// 예) [추가]고정된 사물 + Player시야 + 마법Effect  = 3개의 빛
//     1. 고정된 사물 제거 = 2개의 빛
//     2. 마법Effect 제거 = 1개의 빛
//     3. Player시야 제거 = 0개의 빛
//     4. 이제부터는 FilterSpriteID를 사용해서 Fog를 출력해야 한다.
//
//
// = Sector와 MTopView와 creature의 light관련 부분을 BYTE에서 short로 고쳐야할까?
//   - 한 Sector에 너무 많은 빛이 겹쳐지게 되면 문제가 생긴다.
//
//----------------------------------------------------------------------


#ifndef	__MSECTOR_H__
#define	__MSECTOR_H__

#pragma warning(disable:4786)

#include "DrawTypeDef.h"
#include "MTypeDef.h"
#include "MObject.h"
#include "SectorSoundInfo.h"

#include <fstream>
using namespace std;

class MItem;
class MCreature;
class MImageObject;
class MEffect;

//----------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------
#define	FLAG_SECTOR_BLOCK_UNDERGROUND		0x01	// 지하로 지나갈 수 없는 Sector인가?
#define	FLAG_SECTOR_BLOCK_GROUND			0x02	// 지상으로 지나갈 수 없는 Sector인가?
#define	FLAG_SECTOR_BLOCK_FLYING			0x04	// 공중으로 지나갈 수 없는 Sector인가?
#define	FLAG_SECTOR_BLOCK_ALL				0x07	// Block ALL
#define	FLAG_SECTOR_ITEM					0x08	// Item이 존재한다.
#define	FLAG_SECTOR_UNDERGROUNDCREATURE		0x10	// 지하 캐릭터가 존재한다.
#define	FLAG_SECTOR_GROUNDCREATURE			0x20	// 지상 캐릭터가 존재한다.
#define	FLAG_SECTOR_FLYINGCREATURE			0x40	// 공중 캐릭터가 존재한다.
#define	FLAG_SECTOR_PORTAL					0x80	// Zone 이동이 되는 곳이다.

//----------------------------------------------------------------------
// Flag2
//----------------------------------------------------------------------
#define	FLAG_SECTOR_SAFE_COMMON				0x01	// 전부 다 안전한 곳
#define	FLAG_SECTOR_SAFE_SLAYER				0x02	// slayer만 안전한 곳
#define	FLAG_SECTOR_SAFE_VAMPIRE			0x04	// vampire만 안전한 곳
#define FLAG_SECTOR_SAFE_NO_PK_ZONE			0x08	// 노 피케존 
#define	FLAG_SECTOR_SAFE_OUSTERS			0x10	// ousters만 안전한 곳
#define FLAG_SECTOR_SAFE_ZONE				0x17
// server에 block된 것
#define FLAG_SECTOR_BLOCK_SERVER_UNDERGROUND	0x10
#define FLAG_SECTOR_BLOCK_SERVER_GROUND			0x20
#define FLAG_SECTOR_BLOCK_SERVER_FLYING			0x40


// MObject Map
typedef std::map<BYTE, MObject*>		OBJECT_MAP;

// MEffect List
typedef std::list<MEffect*>		EFFECT_LIST;


//----------------------------------------------------------------------
// PORTAL_INFO
//----------------------------------------------------------------------
class PORTAL_INFO
{
	// type은 MPortal의 Type을 사용한다.
	public :
		int		Type;
		int		ZoneID;	

	public :
		PORTAL_INFO(int type, int zoneID)
		{
			Type = type;
			ZoneID = zoneID;
		}

	public :
		void		operator = (PORTAL_INFO& info)
		{
			Type = info.Type;
			ZoneID = info.ZoneID;
		}
};

// ZoneID List
typedef std::list<PORTAL_INFO>			PORTAL_LIST;

//----------------------------------------------------------------------
// 
// MSector class는 하나의 Object가 존재할 수 있는
//                 한 영역에 대한 정보를 가지고 있는다.
//                 Sector는 하나의 바닥 타일로서 표현된다.
//
//----------------------------------------------------------------------
class MSector {
	public :
		MSector(TYPE_SPRITEID spriteID=0);
		~MSector();


		//------------------------------------------------
		//
		// file I/O
		//
		//------------------------------------------------
		void	SaveToFile(ofstream& file);
		void	LoadFromFile(ifstream& file);

		//------------------------------------------------
		//
		// set SpriteID
		//
		//------------------------------------------------
		void				Set(TYPE_SPRITEID spriteID, BYTE fObject=0);
		// get Sprite Info.
		TYPE_SPRITEID		GetSpriteID() const	{ return m_bDisableTileImage?SPRITEID_NULL:m_SpriteID; }

		bool				IsEmpty() const		{ return m_fProperty==0; }

		//------------------------------------------------
		//
		// Block
		//
		//------------------------------------------------
		BYTE	IsBlockAny() const { return (m_fProperty & FLAG_SECTOR_BLOCK_GROUND) ||
											(m_fProperty & FLAG_SECTOR_BLOCK_UNDERGROUND) ||
											(m_fProperty & FLAG_SECTOR_BLOCK_FLYING); }
		// all
		BYTE	IsBlockAll() const	{ return m_fProperty & FLAG_SECTOR_BLOCK_ALL; }
		void	SetBlockAll()		{ m_fProperty |= FLAG_SECTOR_BLOCK_ALL; }
		void	UnSetBlockAll()		{ m_fProperty &= ~FLAG_SECTOR_BLOCK_ALL; }

		// underground
		BYTE	IsBlockUnderground() const	{ return m_fProperty & FLAG_SECTOR_BLOCK_UNDERGROUND; }
		void	SetBlockUnderground()		{ m_fProperty |= FLAG_SECTOR_BLOCK_UNDERGROUND; }
		void	UnSetBlockUnderground()		{ m_fProperty &= ~FLAG_SECTOR_BLOCK_UNDERGROUND; }

		// ground
		BYTE	IsBlockGround() const		{ return m_fProperty & FLAG_SECTOR_BLOCK_GROUND; }
		void	SetBlockGround()			{ m_fProperty |= FLAG_SECTOR_BLOCK_GROUND; }
		void	UnSetBlockGround()			{ m_fProperty &= ~FLAG_SECTOR_BLOCK_GROUND; }

		// flying
		BYTE	IsBlockFlying() const		{ return m_fProperty & FLAG_SECTOR_BLOCK_FLYING; }
		void	SetBlockFlying()			{ m_fProperty |= FLAG_SECTOR_BLOCK_FLYING; }
		void	UnSetBlockFlying()			{ m_fProperty &= ~FLAG_SECTOR_BLOCK_FLYING; }

		// server block underground
		BYTE	IsBlockServerUnderground() const	{ return m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_UNDERGROUND; }
		void	SetBlockServerUnderground()			{ m_fProperty2 |= FLAG_SECTOR_BLOCK_SERVER_UNDERGROUND; }
		void	UnSetBlockServerUnderground()		{ m_fProperty2 &= ~FLAG_SECTOR_BLOCK_SERVER_UNDERGROUND; }

		// server block Ground
		BYTE	IsBlockServerGround() const	{ return m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_GROUND; }
		void	SetBlockServerGround()			{ m_fProperty2 |= FLAG_SECTOR_BLOCK_SERVER_GROUND; }
		void	UnSetBlockServerGround()		{ m_fProperty2 &= ~FLAG_SECTOR_BLOCK_SERVER_GROUND; }

		// server block Flying
		BYTE	IsBlockServerFlying() const	{ return m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_FLYING; }
		void	SetBlockServerFlying()			{ m_fProperty2 |= FLAG_SECTOR_BLOCK_SERVER_FLYING; }
		void	UnSetBlockServerFlying()		{ m_fProperty2 &= ~FLAG_SECTOR_BLOCK_SERVER_FLYING; }

		
		//------------------------------------------------
		//
		// Sector에 Object가 존재하는가?
		//
		//------------------------------------------------
		BYTE	IsExistObject() const	{ return !m_mapObject.empty(); }
		BYTE	IsObjectEmpty() const	{ return m_mapObject.empty(); }

		BYTE	IsExistItem() const					{ return m_fProperty & FLAG_SECTOR_ITEM; }
		BYTE	IsExistUndergroundCreature() const	{ return m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE; }
		BYTE	IsExistGroundCreature() const		{ return m_fProperty & FLAG_SECTOR_GROUNDCREATURE; }
		BYTE	IsExistFlyingCreature() const		{ return m_fProperty & FLAG_SECTOR_FLYINGCREATURE; }		
		BYTE	IsExistImageObject() const			{ return m_nImageObject; }		
		BYTE	IsExistAnyCreature() const			{ return (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE) ||
																(m_fProperty & FLAG_SECTOR_GROUNDCREATURE) ||
																(m_fProperty & FLAG_SECTOR_FLYINGCREATURE); }

		// ImageObject가 보이기는 하는 곳인데 Block이 아닌 곳인가? (반투명 처리)
		//BYTE	IsImageObjectAndNotBlock()	const	{ return (m_nImageObject && (m_fProperty & FLAG_SECTOR_BLOCK)); }
		

		//------------------------------------------------
		//
		// Add Object to Sector
		//
		//------------------------------------------------		
		bool	AddItem(const MItem* pItem);
		bool	AddUndergroundCreature(const MCreature* pCreature);
		bool	AddGroundCreature(const MCreature* pCreature);
		bool	AddFlyingCreature(const MCreature* pCreature);
		bool	AddImageObject(const MImageObject* pImageObject);


		//------------------------------------------------
		//
		// Remove Object from Sector
		//
		//------------------------------------------------				
		bool	RemoveItem(MItem*& pItem);
		//bool	RemoveUndergroundCreature(MCreature*& pCreature);
		//bool	RemoveGroundCreature(MCreature*& pCreature);
		//bool	RemoveFlyingCreature(MCreature*& pCreature);		

		// ID check and remove		
		bool	RemoveItem(TYPE_OBJECTID id, MItem*& pItem);
		bool	RemoveUndergroundCreature(TYPE_OBJECTID id, MCreature*& pCreature);
		bool	RemoveGroundCreature(TYPE_OBJECTID id, MCreature*& pCreature);
		bool	RemoveFlyingCreature(TYPE_OBJECTID id, MCreature*& pCreature);
		bool	RemoveImageObject(TYPE_OBJECTID id, MImageObject*& pImageObject);		
		bool	RemoveCreature(TYPE_OBJECTID id, MCreature*& pCreature);

		// ID check and just remove		
		bool	RemoveItem(TYPE_OBJECTID id);
		bool	RemoveUndergroundCreature(TYPE_OBJECTID id);
		bool	RemoveGroundCreature(TYPE_OBJECTID id);
		bool	RemoveFlyingCreature(TYPE_OBJECTID id);
		bool	RemoveCreature(TYPE_OBJECTID id);
		bool	RemoveImageObject(TYPE_OBJECTID id);		

		// just remove		
		bool	RemoveItem();
		//bool	RemoveUndergroundCreature();
		//bool	RemoveGroundCreature();
		//bool	RemoveFlyingCreature();		

		// remove all
		void	RemoveAllObject();


		//------------------------------------------------
		//
		// get Object		
		//
		//------------------------------------------------			
		OBJECT_MAP::iterator		GetObjectEnd()			{ return m_mapObject.end(); }
		OBJECT_MAP::const_iterator	GetObjectEnd() const	{ return m_mapObject.end(); }

		// Object*
		MItem*	const		GetItem() const;

		BYTE					GetUndergroundCreatureSize() const	{ return m_nUndergroundCreature; }
		BYTE					GetGroundCreatureSize() const		{ return m_nGroundCreature; }
		BYTE					GetFlyingCreatureSize() const		{ return m_nFlyingCreature; }
		int						GetCreatureSize() const				{ return (int)m_nGroundCreature+m_nUndergroundCreature+m_nFlyingCreature; }
		OBJECT_MAP::const_iterator	GetCreatureIterator() const;
		OBJECT_MAP::const_iterator	GetUndergroundCreatureIterator() const;
		OBJECT_MAP::const_iterator	GetGroundCreatureIterator() const;
		OBJECT_MAP::const_iterator	GetFlyingCreatureIterator() const;
		OBJECT_MAP::iterator	GetCreatureIterator();
		OBJECT_MAP::iterator	GetUndergroundCreatureIterator();
		OBJECT_MAP::iterator	GetGroundCreatureIterator();
		OBJECT_MAP::iterator	GetFlyingCreatureIterator();

		MCreature* const		GetCreature() const;
		MCreature* const		GetUndergroundCreature() const;
		MCreature* const		GetGroundCreature() const;
		MCreature* const		GetFlyingCreature() const;
		MImageObject* const		GetImageObject(TYPE_OBJECTID id) const;		

		// 개수		
		BYTE			GetImageObjectSize() const		{ return m_nImageObject; }

		// iterator를 넘겨준다.
		OBJECT_MAP::const_iterator	GetImageObjectIterator() const;		
		

		// assign operator
		void	operator = (const MSector& sector);

		//------------------------------------------------
		//
		// Effect 관련...
		//
		//------------------------------------------------
		bool		IsExistEffect() const		{ return !m_listEffect.empty(); }
		void		ClearEffect()				{ m_listEffect.clear(); }
		int			GetEffectSize() const		{ return m_listEffect.size(); }

		// 외부에서 iterator로 작업을 할 수 있도록..
		EFFECT_LIST::const_iterator GetEffectIterator() const { return m_listEffect.begin(); }

		// Add/Get/Remove
		void		AddEffect(const MEffect* pEffect);
		MEffect*	GetEffect(TYPE_OBJECTID id) const;
		bool		RemoveEffect(TYPE_OBJECTID id, MEffect*& pEffect);
		bool		RemoveEffect(TYPE_OBJECTID id);
		
		// 계산을 빨리 할려고 darkness만 따로 처리한다.
		bool		HasDarkness() const			{ return m_bDarkness; }
		void		SetDarkness()				{ m_bDarkness = true; }
		void		UnSetDarkness()				{ m_bDarkness = false; }
		
		// 계산을 빨리 할려고 대지정령도 따로 처리한다.-ㅅ-;;;
		bool		HasGroundElemental() const			{ return m_bGroundElemental; }
		void		SetGroundElemental()				{ m_bGroundElemental = true; }
		void		UnSetGroundElemental()				{ m_bGroundElemental = false; }
		
		// 계산을 빨리 할려고 DARKNESS_FORBIDDEN마저 따로 처리한다.-ㅅ-;;;
		bool		HasDarknessForbidden() const			{ return m_bDarknessForbidden; }
		void		SetDarknessForbidden()				{ m_bDarknessForbidden = true; }
		void		UnSetDarknessForbidden()				{ m_bDarknessForbidden = false; }
		
		// [새기술] sanctuary도 추가 --;
		bool		HasSanctuary() const		{ return m_bSanctuary; }
		void		SetSanctuary()				{ m_bSanctuary = true; }
		void		UnSetSanctuary()			{ m_bSanctuary = false; }
		
		//------------------------------------------------
		// Can Stand ?
		//------------------------------------------------
		bool		CanStandUndergroundCreature() const;
		bool		CanStandGroundCreature() const;
		bool		CanStandFlyingCreature() const;


		//------------------------------------------------
		//
		// 시야/Filter 관련
		//
		//------------------------------------------------
		//void			SetFilterSpriteID(TYPE_SPRITEID id)	{ m_FilterSpriteID = id; }
		//TYPE_SPRITEID	GetFilterSpriteID() const			{ return m_FilterSpriteID; }

		// 빛 관련..
		BYTE			GetLight() const					{ return m_Light; }
		void			AddLight()							{ if (m_Light<0xFF) m_Light++; }
		void			RemoveLight() 						{ if (m_Light>0) m_Light--; }
		
		//------------------------------------------------
		//
		// Portal
		//
		//------------------------------------------------
		BYTE	IsPortal() const		{ return m_fProperty & FLAG_SECTOR_PORTAL; }
		void	SetPortal()				{ m_fProperty |= FLAG_SECTOR_PORTAL; }
		void	UnSetPortal()			{ m_fProperty &= ~FLAG_SECTOR_PORTAL; }
		int							GetPortalSize() const	{ return m_listPortal.size(); }
		PORTAL_LIST::const_iterator	GetPortalBegin() const	{ return m_listPortal.begin(); }
		bool						AddPortal(int type, int zoneID);
		void						ClearPortal();

		//------------------------------------------------
		//
		// SectorSound
		//
		//------------------------------------------------
		const SECTORSOUND_LIST&				GetSectorSoundList() const	{ return m_listSectorSound; }
		int									GetSectorSoundSize() const	{ return m_listSectorSound.size(); }
		SECTORSOUND_LIST::const_iterator	GetSectorSoundBegin() const	{ return m_listSectorSound.begin(); }
		bool								AddSectorSound(int zoneSoundID, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		bool								AddSectorSound(const SECTORSOUND_INFO& info);
		void								ClearSectorSound();

		//------------------------------------------------
		//
		// Safe - 안전지대
		//
		//------------------------------------------------
		void				SetSafe(BYTE flag)		{ m_fProperty2 = flag; }
		void				AddSafe(BYTE flag)		{ m_fProperty2 |= flag; }
		void				RemoveSafe(BYTE flag)	{ m_fProperty2 &= ~flag; }
		BYTE				GetSafe() const			{ return m_fProperty2; }
		BYTE				IsSafeSlayer() const	{ return (m_fProperty2 & FLAG_SECTOR_SAFE_COMMON) || (m_fProperty2 & FLAG_SECTOR_SAFE_SLAYER); }
		BYTE				IsSafeVampire() const	{ return (m_fProperty2 & FLAG_SECTOR_SAFE_COMMON) || (m_fProperty2 & FLAG_SECTOR_SAFE_VAMPIRE); }
		BYTE				IsSafeOusters() const	{ return (m_fProperty2 & FLAG_SECTOR_SAFE_COMMON) || (m_fProperty2 & FLAG_SECTOR_SAFE_OUSTERS); }
		BYTE				IsSafeComplete() const	{ return (m_fProperty2 & FLAG_SECTOR_SAFE_COMMON); }

		//------------------------------------------------
		//
		// Visited Flag - 길찾기용
		//
		//------------------------------------------------
		void				SetVisitedFlag()		{ m_bVisitedFlag = true; }
		void				UnSetVisitedFlag()		{ m_bVisitedFlag = false; }
		const bool			IsVisitedFlag() const	{ return m_bVisitedFlag; }


		// 대지 아우 140 스킬 퓨리 오브 놈 관련 fury of gnome
		bool		HasFuryOfGnome() const			{ return m_bFuryOfGnome; }
		void		SetFuryOfGnome()				{ m_bFuryOfGnome = true; }
		void		UnSetFuryOfGnome()				{ m_bFuryOfGnome = false; }

		// 퀘스트 관련 - 타일 보여줄지 안보여줄지..-_-
		bool		HasDisableTileImage() const			{ return m_bDisableTileImage; }
		void		SetDisableTileImage()				{ m_bDisableTileImage = true; }
		void		UnSetDisableTileImage()				{ m_bDisableTileImage = false; }
	public :
		//----------------------------------------------------------------------
		// Key Value of Map
		//----------------------------------------------------------------------
		// 값이 약간 불안한데.. ㅡ.ㅡ;;
		enum POSITION_VALUE 
		{				
			POSITION_ITEM = 1,					// Item이 있는 Node

			POSITION_UNDERGROUNDCREATURE=2,		// 지하 캐릭터가 있는 Node (2~14)
			POSITION_UNDERGROUNDCREATURE_MAX=14,

			POSITION_GROUNDCREATURE=15,			// 지상 캐릭터가 있는 Node (15~49)
			POSITION_GROUNDCREATURE_MAX=49,

			POSITION_FLYINGCREATURE=50,			// 공중 캐릭터가 있는 Node (50~79)
			POSITION_FLYINGCREATURE_MAX=79,

			POSITION_PORTAL=80,					// Zone 이동이 되는 곳이다.
			POSITION_IMAGEOBJECT = 81,			// ImageObject들이 존재하는 Node(81~149)
			POSITION_EFFECT	= 150				// Effect들이 존재하는 Node (150~255)	// 안쓰네 - -;
		};




	protected :
		// tile sprite에 대한 정보
		TYPE_SPRITEID		m_SpriteID;

		// 한 Sector에 있을 수 있는 Object들에 대한 pointer
		OBJECT_MAP			m_mapObject;

		// 한 Sector에 있을 수 있는 Effect들에 대한 pointer
		EFFECT_LIST			m_listEffect;

		// Filer로 사용할 SpriteID
		//TYPE_SPRITEID	m_FilterSpriteID;
	
		PORTAL_LIST			m_listPortal;

		SECTORSOUND_LIST	m_listSectorSound;

		// 캐릭터 마리수
		BYTE				m_nGroundCreature;
		BYTE				m_nUndergroundCreature;
		BYTE				m_nFlyingCreature;

		// 존재하는 ImageObject 개수에 대한 정보
		BYTE				m_nImageObject;			

		// 정보 Flag
		BYTE				m_fProperty;

		// 안전지대
		BYTE				m_fProperty2;

		// 빛에 대한 값
		BYTE				m_Light;


		bool				m_bDarkness;
		bool				m_bSanctuary;		// [새기술]
		bool				m_bGroundElemental;		// [새기술]
		bool				m_bDarknessForbidden;
		bool				m_bVisitedFlag;

		bool				m_bFuryOfGnome;
		bool				m_bDisableTileImage;
};

#endif


