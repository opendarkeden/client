//----------------------------------------------------------------------
// MCreatureTable.h
//----------------------------------------------------------------------
// Creature의 종류에 따른 정보를 저장하는 class
//----------------------------------------------------------------------
//
// [ 종류별로 있는 정보 ] 
//
// - 동작 FrameID
// - Sprite FilePosition
// - First SpriteID
// - 이름... 등등...
// 
//----------------------------------------------------------------------

#ifndef	__MCREATURETABLE_H__
#define	__MCREATURETABLE_H__

#pragma warning(disable:4786)

#include "MObject.h"
#include "SpriteLib/DrawTypeDef.h"
#include "CTypeTable.h"
#include "MString.h"
#include <list>
#include <vector>
#include <fstream>
using namespace std;

//----------------------------------------------------------------------
// Creature의 종족
//----------------------------------------------------------------------
enum CREATURETRIBE
{	
	CREATURETRIBE_SLAYER,			// 슬레이어
	CREATURETRIBE_VAMPIRE,			// 뱀파이어
	CREATURETRIBE_NPC,				// NPC
	CREATURETRIBE_SLAYER_NPC,		// 슬레이어 스프라이트를 사용하는 NPC
	CREATURETRIBE_OUSTERS,
	CREATURETRIBE_OUSTERS_NPC,		// 아우스터즈 스프라이트를 사용하는 NPC
	MAX_CREATURETRIBE
};

//----------------------------------------------------------------------
// CreatureType
//----------------------------------------------------------------------
#define	CREATURETYPE_SLAYER_MALE		0
#define	CREATURETYPE_SLAYER_FEMALE		1

#define	CREATURETYPE_VAMPIRE_MALE1		2
#define	CREATURETYPE_VAMPIRE_FEMALE1	3

#define	CREATURETYPE_VAMPIRE_MALE2		514
#define	CREATURETYPE_VAMPIRE_FEMALE2	515

#define	CREATURETYPE_VAMPIRE_MALE3		558
#define	CREATURETYPE_VAMPIRE_FEMALE3	559
// add by Coffee 2006.11.24  藤속뱁痢饑쮸近蹶
#define	CREATURETYPE_VAMPIRE_MALE4		807
#define	CREATURETYPE_VAMPIRE_FEMALE4	808
//add by viva  195
#define	CREATURETYPE_VAMPIRE_MALE5		809
#define	CREATURETYPE_VAMPIRE_FEMALE5	810
//add by viva  215
#define	CREATURETYPE_VAMPIRE_MALE6		811
#define	CREATURETYPE_VAMPIRE_FEMALE6	812

// end
#define	CREATURETYPE_BAT				185
#define	CREATURETYPE_WOLF				186
#define	CREATURETYPE_SLAYER_OPERATOR	367
#define	CREATURETYPE_VAMPIRE_OPERATOR	369//805//765//804//369
//#define	CREATURETYPE_VAMPIRE_OPERATOR	810//add by viva
#define	CREATURETYPE_OUSTERS_OPERATOR	652
#define CREATURETYPE_OUSTERS			651
#define CREATURETYPE_WER_WOLF			671
#define CREATURETYPE_GHOST				716
#define CREATURETYPE_INSTALL_TURRET		742
#define CREATURETYPE_ROCKET_LUNCHER		767
#define CREATURETYPE_WILD_WOLF			787
//----------------------------------------------------------------------
// 슬레이어 NPC의 복장에 대한 정보
//----------------------------------------------------------------------
class ITEM_WEARINFO
{
	public :
		WORD		skinColor;
		WORD		hairColor;		
		WORD		jacketColor;
		WORD		pantsColor;
		WORD		helmetColor;
		WORD		weaponColor;
		WORD		shieldColor;
		WORD		motorcycleColor;
		
		BYTE		hair;		
		BYTE		jacket;
		BYTE		pants;
		BYTE		helmet;
		BYTE		weapon;
		BYTE		shield;
		BYTE		motorcycle;

	public :
		ITEM_WEARINFO();

		void		operator = (const ITEM_WEARINFO& info);
		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);
};


//----------------------------------------------------------------------
//
// 한 종류의 Creature에 대한 정보
//
//----------------------------------------------------------------------
class CREATURETABLE_INFO {
	public :		
		MString					Name;					// 캐릭터 이름
		CTypeTable<int>			SpriteTypes;			// SpriteTable에서의 ID
		bool					bMale;					// 남자인가?		
		BYTE					MoveTimes;				// 이동 회수
		BYTE					MoveRatio;				// Frame수/이동 회수... 그림은 12frame이라도 6번만 이동할 수도 있다.
		BYTE					MoveTimesMotor;			// 오토바이 탔을 때 이동 회수
		int						Height;					// 키 (채팅 출력 위치를 위해서)
		int						DeadHeight;				// 죽었을때의 키
		TYPE_ACTIONINFO			DeadActionInfo;			// 죽을때의 actionInfo
		int						ColorSet;				// 바뀌는 색깔 번호
		bool					bFlyingCreature;		// 날아다니는 캐릭인가?
		int						FlyingHeight;			// 날아다니는 높이
		bool					bHeadCut;				// 머리 잘리는가? -_-;
		int						HPBarWidth;				// HP bar 크기
		ITEM_WEARINFO*			pItemWearInfo;			// 슬레이어 NPC인 경우
		WORD					ChangeColorSet;			// 캐릭터 전체가 바뀌는 경우
		int						ShadowCount;			// 늘 따라다니는 몸의 잔상 개수

	protected :
		CREATURETRIBE			m_CreatureTribe;		// 종족
		TYPE_SOUNDID*			m_pActionSound;			// Sound ID
		int*					m_pActionCount;			// Action Count
		
	public :
		CREATURETABLE_INFO();
		~CREATURETABLE_INFO();

		//-------------------------------------------------------
		// Set / Get
		//-------------------------------------------------------
		void				SetCreatureTribe(enum CREATURETRIBE ct, int nMaxAction = 0);
		bool				IsSlayer() const						{ return m_CreatureTribe==CREATURETRIBE_SLAYER || m_CreatureTribe==CREATURETRIBE_SLAYER_NPC; }
		bool				IsVampire() const						{ return m_CreatureTribe==CREATURETRIBE_VAMPIRE; }
		bool				IsOusters() const						{ return m_CreatureTribe==CREATURETRIBE_OUSTERS || m_CreatureTribe==CREATURETRIBE_OUSTERS_NPC; }
		bool				IsNPC() const							{ return m_CreatureTribe==CREATURETRIBE_NPC || m_CreatureTribe==CREATURETRIBE_SLAYER_NPC|| m_CreatureTribe==CREATURETRIBE_OUSTERS_NPC; }
		void				SetActionSound(int n, int count, TYPE_SOUNDID sid)	{ m_pActionCount[n] = count;  m_pActionSound[n] = sid; }
		void				SetActionSound(int n, TYPE_SOUNDID sid)	{ m_pActionSound[n] = sid; }
		void				SetActionCount(int n, int count)		{ m_pActionCount[n] = count; }

		enum CREATURETRIBE	GetCreatureTribe() const				{ return m_CreatureTribe; }
		int					GetActionCount(int n)					{ return m_pActionCount[n]; }		
		TYPE_SOUNDID		GetActionSound(int n)					{ return m_pActionSound[n]; }		
		int					GetActionMax() const;
		
		bool				IsFlyingCreature() const				{ return bFlyingCreature; }

		//-------------------------------------------------------
		// assign
		//-------------------------------------------------------
		void				operator = (const CREATURETABLE_INFO& creatureInfo);

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void				SaveToFile(ofstream& file);
		void				LoadFromFile(ifstream& file);	
		
	protected :
		void				InitActionType(int nMaxAction = 0);				
};

//----------------------------------------------------------------------
// CreatureSpriteTypeMapper
//----------------------------------------------------------------------
// SpriteID로 CreatureType을 찾는 경우에 사용
//----------------------------------------------------------------------
class CreatureSpriteTypeMapper {
	public :
		typedef std::vector<WORD>				CREATURE_TYPES;		// WORD.. 귀차나..
		typedef std::vector<CREATURE_TYPES*>	CREATURE_SPRITE_TYPES;

	public :
		CreatureSpriteTypeMapper();
		~CreatureSpriteTypeMapper();

		void	Init(int num);
		void	Release();

		void	AddCreatureType(TYPE_SPRITEID spriteID, WORD creatureType);
		int		GetRandomCreatureType(TYPE_SPRITEID spriteID) const;

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void				SaveToFile(ofstream& file);
		void				LoadFromFile(ifstream& file);	

	protected :
		CREATURE_SPRITE_TYPES	m_CreatureSpriteTypes;
};


typedef CTypeTable<CREATURETABLE_INFO>	CREATURE_TABLE;
extern CREATURE_TABLE* g_pCreatureTable;

extern CreatureSpriteTypeMapper* g_pCreatureSpriteTypeMapper;

#endif


