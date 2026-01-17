//----------------------------------------------------------------------
// MSector.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include "MObject.h"
#include "MItem.h"
#include "MCreature.h"
#include "MImageObject.h"
#include "MEffect.h"
#include "MSector.h"
#include "MTopView.h"
#include "EffectSpriteTypeDef.h"
#include <fstream>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MSector::MSector(TYPE_SPRITEID	spriteID)
{
	m_SpriteID = spriteID;

	m_fProperty		= 0;

	// 현재 Sector에 존재하는 Object들의 개수
	m_nImageObject	= 0;

	//m_FilterSpriteID = SPRITEID_NULL;

	m_Light = 0;

	m_fProperty2 = 0;

	m_bDarkness = false;
	m_bSanctuary = false;	// [새기술]
	m_bGroundElemental = false;	// [새기술]
	m_bDarknessForbidden = false;	// [새기술]

	m_nGroundCreature = 0;
	m_nUndergroundCreature = 0;
	m_nFlyingCreature = 0;
	m_bVisitedFlag = false;
	m_bFuryOfGnome = false;
	m_bDisableTileImage = false;
}

MSector::~MSector()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
void	
MSector::Set(TYPE_SPRITEID spriteID, BYTE fObject)
{ 
	m_SpriteID	= spriteID;
	m_fProperty	= fObject;

	// m_mapObject.clear();
}

//----------------------------------------------------------------------
// Remove All Object
//----------------------------------------------------------------------
void	
MSector::RemoveAllObject()	
{ 
	m_mapObject.clear(); 
	m_nImageObject;
	m_fProperty = 0; 
	m_fProperty2 = 0;

	m_nGroundCreature = 0;
	m_nUndergroundCreature = 0;
	m_nFlyingCreature = 0;
}

//----------------------------------------------------------------------
// Save to File
//----------------------------------------------------------------------
// SpriteID와 BlockFlag, Light만 저장한다.
//----------------------------------------------------------------------
void	
MSector::SaveToFile(ofstream& file)
{
	BYTE flag = 0;

	// Underground BLOCK flag가 있을 경우
	if (m_fProperty & FLAG_SECTOR_BLOCK_UNDERGROUND)
	{
		// BLOCK flag 하나만 저장한다.
		flag |= FLAG_SECTOR_BLOCK_UNDERGROUND;
	}

	// Ground BLOCK flag가 있을 경우
	if (m_fProperty & FLAG_SECTOR_BLOCK_GROUND)
	{
		// BLOCK flag 하나만 저장한다.
		flag |= FLAG_SECTOR_BLOCK_GROUND;
	}

	// Flying BLOCK flag가 있을 경우
	if (m_fProperty & FLAG_SECTOR_BLOCK_FLYING)
	{
		// BLOCK flag 하나만 저장한다.
		flag |= FLAG_SECTOR_BLOCK_FLYING;
	}

	// Portal flag가 있을 경우
	if (m_fProperty & FLAG_SECTOR_PORTAL)
	{
		// BLOCK flag 하나만 저장한다.
		flag |= FLAG_SECTOR_PORTAL;
	}
	

	file.write((const char*)&m_SpriteID, SIZE_SPRITEID);
	file.write((const char*)&flag, 1);
	file.write((const char*)&m_Light, 1);
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// SpriteID와 Block Flag, Light만 Load된다.
//----------------------------------------------------------------------
void	
MSector::LoadFromFile(ifstream& file)
{
	//----------------------------------------
	// Object list를 초기화해야 되는데....
	// Load하고 Object추가하고
	// 다시 Load하는 경우가 없다고 보고..
	// 속도 문제상, 초기화하지 않는다.
	//----------------------------------------
	// m_listObject.clear();

	file.read((char*)&m_SpriteID, SIZE_SPRITEID);
	file.read((char*)&m_fProperty, 1);
	file.read((char*)&m_Light, 1);
}


//----------------------------------------------------------------------
// Assign Operator
//----------------------------------------------------------------------
// Tile의 모양과
// Block Flag만 대입된다.
//----------------------------------------------------------------------
void	
MSector::operator = (const MSector& sector)
{
	m_SpriteID = sector.m_SpriteID;

	// undeground block
	if (sector.m_fProperty & FLAG_SECTOR_BLOCK_UNDERGROUND)
		m_fProperty |= FLAG_SECTOR_BLOCK_UNDERGROUND;

	// ground block
	if (sector.m_fProperty & FLAG_SECTOR_BLOCK_GROUND)
		m_fProperty |= FLAG_SECTOR_BLOCK_GROUND;

	// flying block
	if (sector.m_fProperty & FLAG_SECTOR_BLOCK_FLYING)
		m_fProperty |= FLAG_SECTOR_BLOCK_FLYING;
}

//----------------------------------------------------------------------
// Can Stand UndergroundCreature
//----------------------------------------------------------------------
// 이 Sector에 UndergroundCreature가 들어갈 수 있는지에 대한 check
//----------------------------------------------------------------------
// Block이면 false
// UndergroundCreature가 있으면 false
// 아니면 true
//----------------------------------------------------------------------
bool
MSector::CanStandUndergroundCreature() const
{
	if (m_fProperty & FLAG_SECTOR_BLOCK_UNDERGROUND		
		|| m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE
		|| m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_UNDERGROUND)
		return false;

	return true;
}

//----------------------------------------------------------------------
// Can Stand Creature
//----------------------------------------------------------------------
// 이 Sector에 Creature가 들어갈 수 있는지에 대한 check
//----------------------------------------------------------------------
// Block이면 false
// Creature가 있으면 false
// 아니면 true
//----------------------------------------------------------------------
bool
MSector::CanStandGroundCreature() const
{
	if (m_fProperty & FLAG_SECTOR_BLOCK_GROUND	
		|| m_fProperty & FLAG_SECTOR_GROUNDCREATURE
		|| m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_GROUND)
		return false;

	return true;
}


//----------------------------------------------------------------------
// Can Stand FlyingCreature
//----------------------------------------------------------------------
// 이 Sector에 FlyingCreature가 들어갈 수 있는지에 대한 check
//----------------------------------------------------------------------
// Block이면 false
// FlyingCreature가 있으면 false
// 아니면 true
//----------------------------------------------------------------------
bool
MSector::CanStandFlyingCreature() const
{
	if (m_fProperty & FLAG_SECTOR_BLOCK_FLYING
		|| m_fProperty & FLAG_SECTOR_FLYINGCREATURE
		|| m_fProperty2 & FLAG_SECTOR_BLOCK_SERVER_FLYING)		
		return false;

	return true;
}



//----------------------------------------------------------------------
// 
//                        Add Object
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Add Item
//----------------------------------------------------------------------
// 이미 Item이 있으면 추가할 수 없다.
// Item을 추가할 수 있으면 pItem을 추가하고
// Item Flag에 check해준다.
//----------------------------------------------------------------------
bool	
MSector::AddItem(const MItem* pItem)
{
	// 이미 Item이 있으면.. 추가 불가능
	if (m_fProperty & FLAG_SECTOR_ITEM)
		return false;

	// Item의 위치에 Item을 추가한다.
	m_mapObject.insert( OBJECT_MAP::value_type(POSITION_ITEM, (MObject*)pItem) );

	// Item Flag에 check해준다.
	m_fProperty |= FLAG_SECTOR_ITEM;
	if( pItem->GetItemClass() == ITEM_CLASS_CORPSE )
	{
		const MCorpse* pCorpse = dynamic_cast<const MCorpse*>(pItem);
		if( pCorpse == NULL || pCorpse->GetCreature() == NULL )
			return true;

		const MCreature* pCreature = pCorpse->GetCreature();

		if( pCreature->GetCreatureType() == 672 )
			m_fProperty |= FLAG_SECTOR_BLOCK_GROUND;
	}

	return true;
}

//----------------------------------------------------------------------
// Add UndergroundCreature
//----------------------------------------------------------------------
// 이미 UndergroundCreature이 있으면 추가할 수 없다.
// UndergroundCreature을 추가할 수 있으면 pUndergroundCreature을 추가하고
// UndergroundCreature Flag에 check해준다.
//----------------------------------------------------------------------
bool	
MSector::AddUndergroundCreature(const MCreature* pUndergroundCreature)
{
	// 이미 UndergroundCreature이 있으면.. 추가 불가능
	//if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	//	return false;

	BYTE pos = POSITION_UNDERGROUNDCREATURE+m_nUndergroundCreature;

	if (pos > POSITION_UNDERGROUNDCREATURE_MAX)
		return false;
	

	int beforeSize = m_mapObject.size();

	// UndergroundCreature의 위치에 UndergroundCreature을 추가한다.
	m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pUndergroundCreature) );

	int afterSize = m_mapObject.size();

	// 추가가 안 됐다면..
	if (afterSize==beforeSize)
	{
		BYTE previousPos = POSITION_UNDERGROUNDCREATURE-1;

		// 다~ 검색해서 빈 곳을 찾아서 추가해야 한다.
		OBJECT_MAP::const_iterator iCreature = GetUndergroundCreatureIterator();
		BYTE currentPos;
		int i;

		for (i=0; i<beforeSize; i++)
		{
			currentPos = iCreature->first;

			// 연속적이지 않은 숫자이면... 빈 곳이 있다는 의미이다.
			// currentPos-1은 확실히 빈 곳이라고 할 수 있다.
			if (currentPos - previousPos > 1)
			{
				pos = currentPos - 1;
				m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pUndergroundCreature) );

				break;
			}

			previousPos = currentPos;

			iCreature ++;
		}

		// 맨 마지막꺼 다음에 넣는다.
		if (i==beforeSize)
		{
			pos = currentPos + 1;
			m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pUndergroundCreature) );
		}
	}

	// UndergroundCreature Flag에 check해준다.
	m_fProperty |= FLAG_SECTOR_UNDERGROUNDCREATURE;

	m_nUndergroundCreature ++;

	return true;
}

//----------------------------------------------------------------------
// Add Creature
//----------------------------------------------------------------------
// 이미 Creature이 있으면 추가할 수 없다.
// Creature을 추가할 수 있으면 pCreature을 추가하고
// Creature Flag에 check해준다.
//----------------------------------------------------------------------
bool	
MSector::AddGroundCreature(const MCreature* pCreature)
{
	// 이미 Creature이 있으면.. 추가 불가능
	//if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	//	return false;

	//----------------------------------------------------------
	// 같은게 이미 있는지 체크하기
	//----------------------------------------------------------
	/*
	if (m_nGroundCreature>0)
	{
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_GROUNDCREATURE);

		while (iObject!=m_mapObject.end() 
				&& iObject->first <= POSITION_GROUNDCREATURE_MAX)
		{
			// 이미 있는 경우.. pointer값 비교
			if (((MCreature*)iObject->second)==pCreature)
			{
				return true;
			}
			
			iObject ++;
		}
	}
	*/

	BYTE pos = POSITION_GROUNDCREATURE+m_nGroundCreature;

	if (pos > POSITION_GROUNDCREATURE_MAX)
		return false;

	int beforeSize = m_mapObject.size();

	// Creature의 위치에 Creature을 추가한다.
	m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pCreature) );

	int afterSize = m_mapObject.size();

	// 추가가 안 됐다면..
	if (afterSize==beforeSize)
	{
		BYTE previousPos = POSITION_GROUNDCREATURE-1;

		// 다~ 검색해서 빈 곳을 찾아서 추가해야 한다.
		OBJECT_MAP::const_iterator iCreature = GetGroundCreatureIterator();
		BYTE currentPos;
		int i;

		for (i=0; i<beforeSize; i++)
		{
			currentPos = iCreature->first;

			// 연속적이지 않은 숫자이면... 빈 곳이 있다는 의미이다.
			// currentPos-1은 확실히 빈 곳이라고 할 수 있다.
			if (currentPos - previousPos > 1)
			{
				pos = currentPos - 1;
				m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pCreature) );

				break;
			}

			previousPos = currentPos;

			iCreature ++;
		}

		// 맨 마지막꺼 다음에 넣는다.
		if (i==beforeSize)
		{
			pos = currentPos + 1;
			m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pCreature) );
		}
	}


	// Creature Flag에 check해준다.
	m_fProperty |= FLAG_SECTOR_GROUNDCREATURE;

	m_nGroundCreature ++;

	return true;
}

//----------------------------------------------------------------------
// Add FlyingCreature
//----------------------------------------------------------------------
// 이미 FlyingCreature이 있으면 추가할 수 없다.
// FlyingCreature을 추가할 수 있으면 pFlyingCreature을 추가하고
// FlyingCreature Flag에 check해준다.
//----------------------------------------------------------------------
bool	
MSector::AddFlyingCreature(const MCreature* pFlyingCreature)
{
	// 이미 FlyingCreature이 있으면.. 추가 불가능
	//if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	//	return false;

	BYTE pos = POSITION_FLYINGCREATURE+m_nFlyingCreature;

	if (pos > POSITION_FLYINGCREATURE_MAX)
		return false;	

	int beforeSize = m_mapObject.size();

	// FlyingCreature의 위치에 FlyingCreature을 추가한다.
	m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pFlyingCreature) );

	int afterSize = m_mapObject.size();

	// 추가가 안 됐다면..
	if (afterSize==beforeSize)
	{
		BYTE previousPos = POSITION_FLYINGCREATURE-1;

		// 다~ 검색해서 빈 곳을 찾아서 추가해야 한다.
		OBJECT_MAP::const_iterator iCreature = GetFlyingCreatureIterator();
		BYTE currentPos;
		int i;

		for (i=0; i<beforeSize; i++)
		{
			currentPos = iCreature->first;

			// 연속적이지 않은 숫자이면... 빈 곳이 있다는 의미이다.
			// currentPos-1은 확실히 빈 곳이라고 할 수 있다.
			if (currentPos - previousPos > 1)
			{
				pos = currentPos - 1;
				m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pFlyingCreature) );

				break;
			}

			previousPos = currentPos;

			iCreature ++;
		}

		// 맨 마지막꺼 다음에 넣는다.
		if (i==beforeSize)
		{
			pos = currentPos + 1;
			m_mapObject.insert( OBJECT_MAP::value_type(pos, (MObject*)pFlyingCreature) );
		}
	}

	// FlyingCreature Flag에 check해준다.
	m_fProperty |= FLAG_SECTOR_FLYINGCREATURE;

	m_nFlyingCreature ++;

	return true;
}

//----------------------------------------------------------------------
// Add ImageObject
//----------------------------------------------------------------------
// 이미 ImageObject가 있거나 없거나 무조건 추가하면 된다.
// 그런데, 이미 ImageObject가 있을 수도 있으므로 
// 현재 존재하는 개수를 알아보고 맨 끝에~ 넣으면 된다.
// 추가하고 m_nImageObject를 1 증가. 
//
// (단, ImageObject의 개수의 한계를 넘어갈수도 있지만... 확률이 희박하므로
//  그럴 경우는 없다고 보고 check하지 않는다..)
//
//----------------------------------------------------------------------
bool	
MSector::AddImageObject(const MImageObject* pImageObject)
{
	BYTE pos = POSITION_IMAGEOBJECT+m_nImageObject;

	if (pos >= POSITION_EFFECT)
		return false;

	// ImageObject의 위치에 ImageObject을 추가한다.
	m_mapObject.insert( 
		OBJECT_MAP::value_type(pos, (MObject*)pImageObject) 
	);

	// 개수를 증가시킨다.
	m_nImageObject ++;

	return true;
}


//----------------------------------------------------------------------
// 
//                         Get Object
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Get Item
//----------------------------------------------------------------------
// Item Flag를 보고 Item이 있다면 return해주면 된다.
//----------------------------------------------------------------------
MItem* const			
MSector::GetItem() const
{
	// Item이 있다면...
	if (m_fProperty & FLAG_SECTOR_ITEM)
	{
		// Item의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.find(POSITION_ITEM);

		// 없을 경우 NULL을 return한다.
		if (iObject == m_mapObject.end()) 
			return NULL;
		
		// 있으면 그 Object를 return한다.
		return (MItem* const)((*iObject).second);
	}

	return NULL;
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature가 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::const_iterator
MSector::GetCreatureIterator() const
{
	// UndergroundCreature이 있다면...
	if (IsExistAnyCreature())
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get UndergroundCreature
//----------------------------------------------------------------------
// UndergroundCreature Flag를 보고 UndergroundCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::const_iterator
MSector::GetUndergroundCreatureIterator() const
{
	// UndergroundCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		// 있으면 UndergroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_UNDERGROUNDCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::const_iterator
MSector::GetGroundCreatureIterator() const
{
	// Creature이 있다면...
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// Creature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_GROUNDCREATURE);

		// 있으면 GroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_GROUNDCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}
		
//----------------------------------------------------------------------
// Get FlyingCreature
//----------------------------------------------------------------------
// FlyingCreature Flag를 보고 FlyingCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::const_iterator
MSector::GetFlyingCreatureIterator() const
{
	// FlyingCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_FLYINGCREATURE);

		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature가 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::iterator
MSector::GetCreatureIterator()
{
	// UndergroundCreature이 있다면...
	if (IsExistAnyCreature())
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get UndergroundCreature
//----------------------------------------------------------------------
// UndergroundCreature Flag를 보고 UndergroundCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::iterator
MSector::GetUndergroundCreatureIterator()
{
	// UndergroundCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		// 있으면 UndergroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_UNDERGROUNDCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::iterator
MSector::GetGroundCreatureIterator()
{
	// Creature이 있다면...
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// Creature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.lower_bound(POSITION_GROUNDCREATURE);

		// 있으면 GroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_GROUNDCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}
		
//----------------------------------------------------------------------
// Get FlyingCreature
//----------------------------------------------------------------------
// FlyingCreature Flag를 보고 FlyingCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
OBJECT_MAP::iterator
MSector::GetFlyingCreatureIterator()
{
	// FlyingCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.lower_bound(POSITION_FLYINGCREATURE);

		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return iObject;
		}
	}

	return m_mapObject.end();
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature가 있다면 return해주면 된다.
//----------------------------------------------------------------------
MCreature * const
MSector::GetCreature() const
{
	// UndergroundCreature이 있다면...
	if (IsExistAnyCreature())
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return (MCreature* const)(iObject->second);
		}
	}
	
	return NULL;
}

//----------------------------------------------------------------------
// Get UndergroundCreature
//----------------------------------------------------------------------
// UndergroundCreature Flag를 보고 UndergroundCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
MCreature * const
MSector::GetUndergroundCreature() const
{
	// UndergroundCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_UNDERGROUNDCREATURE);
		
		// 있으면 UndergroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_UNDERGROUNDCREATURE_MAX)
		{
			return (MCreature* const)(iObject->second);
		}
	}
	
	return NULL;
}

//----------------------------------------------------------------------
// Get Creature
//----------------------------------------------------------------------
// Creature Flag를 보고 Creature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
MCreature * const
MSector::GetGroundCreature() const
{
	// Creature이 있다면...
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// Creature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_GROUNDCREATURE);
		
		// 있으면 GroundCreature가 맞을때..
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_GROUNDCREATURE_MAX)
		{
			return (MCreature* const)(iObject->second);
		}
	}
	
	return NULL;
}

//----------------------------------------------------------------------
// Get FlyingCreature
//----------------------------------------------------------------------
// FlyingCreature Flag를 보고 FlyingCreature이 있다면 return해주면 된다.
//----------------------------------------------------------------------
MCreature * const
MSector::GetFlyingCreature() const
{
	// FlyingCreature이 있다면...
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.lower_bound(POSITION_FLYINGCREATURE);
		
		if (iObject!=m_mapObject.end() 
			&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			return (MCreature* const)(iObject->second);
		}
	}
	
	return NULL;
}
	
//----------------------------------------------------------------------
// Get ImageObject
//----------------------------------------------------------------------
// ImageObject Flag를 보고 ImageObject가 있다면 
// 원하는 id를 가지는 ImageObject가 있으면 return해주면 된다.
//----------------------------------------------------------------------
MImageObject* const	
MSector::GetImageObject(TYPE_OBJECTID id) const
{
	// ImageObject이 있다면...
	if (m_nImageObject)
	{
		// ImageObject의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::const_iterator iObject = m_mapObject.find(POSITION_IMAGEOBJECT);

		// 없을 경우 NULL을 return한다.
		if (iObject == m_mapObject.end()) 
			return NULL;

		// 있으면 ImageObject 개수만큼 비교를 하고..		
		// 도중에 ID가 id인 ImageObject가 있으면 return한다.
		for (register BYTE i=0; i<m_nImageObject; i++)
		{
			// 같은 ID이면 return한다.
			if ( ((*iObject).second)->GetID() == id )
				return (MImageObject* const)((*iObject).second);

			// 다음 ImageObject
			iObject++;
		}
		
	}

	return NULL;
}


//----------------------------------------------------------------------
//
//                       Remove Object
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Remove Item
//----------------------------------------------------------------------
// Item이 있으면 제거하고 Flag도 없애고
// pItem으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveItem(MItem*& pItem)
{
	// Item이 있으면
	if (m_fProperty & FLAG_SECTOR_ITEM)
	{
		// Item의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_ITEM);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 그 Object를 넘겨줄 수 있도록 pItem에 저장한다.
		pItem =  (MItem*)((*iObject).second);

		// iObject를 제거한다.
		m_mapObject.erase( iObject );

		// Item Flag만 제거한다.
		if( pItem->GetItemClass() == ITEM_CLASS_CORPSE &&
			((MCorpse*)pItem)->GetCreature() != NULL &&
			((MCorpse*)pItem)->GetCreature()->GetCreatureType() == 672 )
			m_fProperty &= ~FLAG_SECTOR_BLOCK_GROUND;

		m_fProperty &= ~FLAG_SECTOR_ITEM;

		return true;
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove UndergroundCreature
//----------------------------------------------------------------------
// UndergroundCreature이 있으면 제거하고 Flag도 없애고
// pUndergroundCreature으로 넘겨준다.
//----------------------------------------------------------------------
/*
bool	
MSector::RemoveUndergroundCreature(MCreature*& pUndergroundCreature)
{
	// UndergroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_UNDERGROUNDCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 그 Object를 넘겨줄 수 있도록 pUndergroundCreature에 저장한다.
		pUndergroundCreature =  (MCreature*)((*iObject).second);

		// iObject를 제거한다.
		m_mapObject.erase( iObject );

		// UndergroundCreature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;

		return true;
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove Creature
//----------------------------------------------------------------------
// Creature이 있으면 제거하고 Flag도 없애고
// pCreature으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveGroundCreature(MCreature*& pCreature)
{
	// Creature이 있으면
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// Creature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_GROUNDCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 그 Object를 넘겨줄 수 있도록 pCreature에 저장한다.
		pCreature =  (MCreature*)((*iObject).second);

		// iObject를 제거한다.
		m_mapObject.erase( iObject );

		// Creature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;

		return true;
	}

	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove FlyingCreature
//----------------------------------------------------------------------
// FlyingCreature이 있으면 제거하고 Flag도 없애고
// pFlyingCreature으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveFlyingCreature(MCreature*& pFlyingCreature)
{
	// FlyingCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_FLYINGCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 그 Object를 넘겨줄 수 있도록 pFlyingCreature에 저장한다.
		pFlyingCreature =  (MCreature*)((*iObject).second);

		// iObject를 제거한다.
		m_mapObject.erase( iObject );

		// FlyingCreature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;

		return true;
	}

	// 없으면
	return false;
}
*/

//----------------------------------------------------------------------
// Remove Item(id, &Item)
//----------------------------------------------------------------------
// Item이 있으면 id를 비교해서 제거하고 Flag도 없애고
// pItem으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveItem(TYPE_OBJECTID id, MItem*& pItem)
{
	// Item이 있으면
	if (m_fProperty & FLAG_SECTOR_ITEM)
	{
		// Item의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_ITEM);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		pItem =  (MItem*)((*iObject).second);
			
		// 있으면 
		// ID를 비교해서 같은 경우,
		// 그 Object를 넘겨줄 수 있도록 pItem에 저장한다.
		if (((*iObject).second)->GetID() == id)
		{
			// iObject를 제거한다.
			m_mapObject.erase( iObject );

			// Item Flag만 제거한다.
			if( pItem->GetItemClass() == ITEM_CLASS_CORPSE &&
				((MCorpse*)pItem)->GetCreature() != NULL &&
				((MCorpse*)pItem)->GetCreature()->GetCreatureType() == 672 )
				m_fProperty &= ~FLAG_SECTOR_BLOCK_GROUND;

			m_fProperty &= ~FLAG_SECTOR_ITEM;

			return true;
		}		
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove Creature(id, &Creature)
//----------------------------------------------------------------------
// Creature가 있으면 id를 비교해서 제거하고 Flag도 없애고
// pCreature 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveCreature(TYPE_OBJECTID id, MCreature*& pCreature)
{
	if (IsExistAnyCreature())
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetCreatureIterator();

		while (iObject!=m_mapObject.end() 
				&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			pCreature =  (MCreature*)iObject->second;
				
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pCreature에 저장한다.
			if (pCreature->GetID() == id)
			{
				// Underground
				if (iObject->first <= POSITION_UNDERGROUNDCREATURE_MAX)
				{					
					if (--m_nUndergroundCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;
					}
				}
				// Ground
				else if (iObject->first <= POSITION_GROUNDCREATURE_MAX)
				{
					if (--m_nGroundCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;
					}
				}
				// Flying
				else 
				{
					if (--m_nFlyingCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;
					}
				}		
				
				// iObject를 제거한다.
				m_mapObject.erase( iObject );				

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	pCreature = NULL;

	return false;
}

//----------------------------------------------------------------------
// Remove UndergroundCreature(id, &UndergroundCreature)
//----------------------------------------------------------------------
// UndergroundCreature이 있으면 id를 비교해서 제거하고 Flag도 없애고
// pUndergroundCreature으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveUndergroundCreature(TYPE_OBJECTID id, MCreature*& pUndergroundCreature)
{
	// UndergroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetUndergroundCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nUndergroundCreature; i++)
		{
			pUndergroundCreature =  (MCreature*)iObject->second;
				
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pUndergroundCreature에 저장한다.
			if (pUndergroundCreature->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// UndergroundCreature Flag만 제거한다.
				if (--m_nUndergroundCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;				
				}

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	pUndergroundCreature = NULL;

	return false;
}

//----------------------------------------------------------------------
// Remove Creature(id, &Creature)
//----------------------------------------------------------------------
// Creature이 있으면 id를 비교해서 제거하고 Flag도 없애고
// pGroundCreature으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveGroundCreature(TYPE_OBJECTID id, MCreature*& pGroundCreature)
{
	// GroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// GroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetGroundCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nGroundCreature; i++)
		{
			pGroundCreature =  (MCreature*)iObject->second;
				
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pGroundCreature에 저장한다.
			if (pGroundCreature->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// GroundCreature Flag만 제거한다.				
				if (--m_nGroundCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;
				}

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	pGroundCreature = NULL;

	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove FlyingCreature(id, &FlyingCreature)
//----------------------------------------------------------------------
// FlyingCreature이 있으면 id를 비교해서 제거하고 Flag도 없애고
// pFlyingCreature으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveFlyingCreature(TYPE_OBJECTID id, MCreature*& pFlyingCreature)
{
	// FlyingCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetFlyingCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nFlyingCreature; i++)
		{
			pFlyingCreature =  (MCreature*)iObject->second;
				
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pFlyingCreature에 저장한다.
			if (pFlyingCreature->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// FlyingCreature Flag만 제거한다.				
				if (--m_nFlyingCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;
				}

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	pFlyingCreature = NULL;

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove ImageObject(id, &ImageObject)
//----------------------------------------------------------------------
// ImageObject이 있으면 id를 비교해서 같은 걸 제거하고 Flag도 없애고
// m_nImageObject를 1 감소 시키고 
// pImageObject으로 넘겨준다.
//----------------------------------------------------------------------
bool	
MSector::RemoveImageObject(TYPE_OBJECTID id, MImageObject*& pImageObject)
{
	// ImageObject이 있으면
	if (m_nImageObject)
	{
		// ImageObject의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_IMAGEOBJECT);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 모든 ImageObject와 ID를 비교해서..
		// 같은 경우, 그 Object를 넘겨줄 수 있도록 pImageObject에 저장한다.
		for (register BYTE i=0; i<m_nImageObject; i++)
		{
			// ID가 같은 경우..
			if (((*iObject).second)->GetID() == id)
			{
				pImageObject =  (MImageObject*)((*iObject).second);

				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// m_nImageObject가 0인 경우만 Flag를 제거한다.
				m_nImageObject--;				

				return true;
			}		

			// 다음 ImageObject
			iObject++;
		}
	}

	// 없으면
	return false;
}


//----------------------------------------------------------------------
// Remove Item(id)
//----------------------------------------------------------------------
// Item이 있으면 id를 비교해서 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveItem(TYPE_OBJECTID id)
{
	// Item이 있으면
	if (m_fProperty & FLAG_SECTOR_ITEM)
	{
		// Item의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_ITEM);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 
		// ID를 비교해서 같은 경우,
		// 그 Object를 넘겨줄 수 있도록 pItem에 저장한다.
		if (((*iObject).second)->GetID() == id)
		{
			// iObject를 제거한다.
			MItem* pItem = dynamic_cast<MItem*>(iObject->second);
			if( pItem->GetItemClass() == ITEM_CLASS_CORPSE &&
				((MCorpse*)pItem)->GetCreature() != NULL &&
				((MCorpse*)pItem)->GetCreature()->GetCreatureType() == 672 )
				m_fProperty &= ~FLAG_SECTOR_BLOCK_GROUND;

			m_mapObject.erase( iObject );

			// Item Flag만 제거한다.
			m_fProperty &= ~FLAG_SECTOR_ITEM;

			return true;
		}		
	}

	// 없으면
	return false;
}


//----------------------------------------------------------------------
// Remove Creature(id)
//----------------------------------------------------------------------
// Creature가 있으면 id를 비교해서 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveCreature(TYPE_OBJECTID id)
{	
	if (IsExistAnyCreature())
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetCreatureIterator();

		while (iObject!=m_mapObject.end() 
				&& iObject->first <= POSITION_FLYINGCREATURE_MAX)
		{
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pCreature에 저장한다.
			if (((MCreature*)iObject->second)->GetID() == id)
			{
				// Underground
				if (iObject->first <= POSITION_UNDERGROUNDCREATURE_MAX)
				{
					if (--m_nUndergroundCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;
					}					
				}
				// Ground
				else if (iObject->first <= POSITION_GROUNDCREATURE_MAX)
				{
					if (--m_nGroundCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;
					}
					
				}
				// Flying
				else 
				{
					if (--m_nFlyingCreature==0)
					{
						m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;
					}					
				}	
				
				// iObject를 제거한다.
				m_mapObject.erase( iObject );				

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove UndergroundCreature(id)
//----------------------------------------------------------------------
// UndergroundCreature이 있으면 id를 비교해서 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveUndergroundCreature(TYPE_OBJECTID id)
{
	// UndergroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetUndergroundCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nUndergroundCreature; i++)
		{
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pUndergroundCreature에 저장한다.
			if (((MCreature*)iObject->second)->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// UndergroundCreature Flag만 제거한다.
				if (--m_nUndergroundCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;
				}

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove Creature(id)
//----------------------------------------------------------------------
// Creature이 있으면 id를 비교해서 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveGroundCreature(TYPE_OBJECTID id)
{
	// GroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// GroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetGroundCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nGroundCreature; i++)
		{
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pGroundCreature에 저장한다.
			if (((MCreature*)iObject->second)->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// GroundCreature Flag만 제거한다.
				if (--m_nGroundCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;
				}				

				return true;
			}

			iObject ++;
		}		
	}

	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove FlyingCreature(id)
//----------------------------------------------------------------------
// FlyingCreature이 있으면 id를 비교해서 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveFlyingCreature(TYPE_OBJECTID id)
{
	// FlyingCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = GetFlyingCreatureIterator();

		for (int i=0; iObject!=m_mapObject.end() && i<m_nFlyingCreature; i++)
		{
			// 있으면 
			// ID를 비교해서 같은 경우,
			// 그 Object를 넘겨줄 수 있도록 pFlyingCreature에 저장한다.
			if (((MCreature*)iObject->second)->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );

				// FlyingCreature Flag만 제거한다.
				if (--m_nFlyingCreature==0)
				{
					m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;
				}
				

				return true;
			}

			iObject ++;
		}		
	}


	// 없으면
	return false;
}
		
//----------------------------------------------------------------------
// Remove ImageObject(id)
//----------------------------------------------------------------------
// ImageObject이 있으면 id를 비교해서 
// 같은게 있으면 제거
//----------------------------------------------------------------------
bool	
MSector::RemoveImageObject(TYPE_OBJECTID id)
{
	// ImageObject이 있으면
	if (m_nImageObject)
	{
		// ImageObject의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_IMAGEOBJECT);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 있으면 
		// ID를 비교해서 같은 경우,
		// 그 Object를 넘겨줄 수 있도록 pImageObject에 저장한다.
		for (register BYTE i=0; i<m_nImageObject; i++)
		{
			if (((*iObject).second)->GetID() == id)
			{
				// iObject를 제거한다.
				m_mapObject.erase( iObject );
				
				m_nImageObject--;				

				return true;
			}		

			// 다음 ImageObject
			iObject ++;
		}
	}

	// 없으면
	return false;
}


//----------------------------------------------------------------------
// Remove Item
//----------------------------------------------------------------------
// Item이 있으면 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveItem()
{
	// Item이 있으면
	if (m_fProperty & FLAG_SECTOR_ITEM)
	{
		// Item의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_ITEM);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;

		MItem* pItem = dynamic_cast<MItem*>(iObject->second);
		if( pItem->GetItemClass() == ITEM_CLASS_CORPSE && 
			((MCorpse*)pItem)->GetCreature() != NULL &&
			((MCorpse*)pItem)->GetCreature()->GetCreatureType() == 672 )
			m_fProperty &= ~FLAG_SECTOR_BLOCK_GROUND;
		
		// 제거하고
		m_mapObject.erase( iObject );

		// Item Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_ITEM;

		return true;
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove UndergroundCreature
//----------------------------------------------------------------------
// UndergroundCreature이 있으면 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
/*
bool	
MSector::RemoveUndergroundCreature()
{
	// UndergroundCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_UNDERGROUNDCREATURE)
	{
		// UndergroundCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_UNDERGROUNDCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 제거하고
		m_mapObject.erase( iObject );

		// UndergroundCreature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_UNDERGROUNDCREATURE;

		return true;
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove Creature
//----------------------------------------------------------------------
// Creature이 있으면 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveGroundCreature()
{
	// Creature이 있으면
	if (m_fProperty & FLAG_SECTOR_GROUNDCREATURE)
	{
		// Creature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_GROUNDCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 제거하고
		m_mapObject.erase( iObject );

		// Creature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_GROUNDCREATURE;

		return true;
	}

	// 없으면
	return false;
}

//----------------------------------------------------------------------
// Remove FlyingCreature
//----------------------------------------------------------------------
// FlyingCreature이 있으면 제거하고 Flag를 없앤다.
//----------------------------------------------------------------------
bool	
MSector::RemoveFlyingCreature()
{
	// FlyingCreature이 있으면
	if (m_fProperty & FLAG_SECTOR_FLYINGCREATURE)
	{
		// FlyingCreature의 위치에 있는 Object를 찾는다.
		OBJECT_MAP::iterator iObject = m_mapObject.find(POSITION_FLYINGCREATURE);

		// 없을 경우 return false
		if (iObject == m_mapObject.end()) 
			return false;
		
		// 제거하고
		m_mapObject.erase( iObject );

		// FlyingCreature Flag만 제거한다.
		m_fProperty &= ~FLAG_SECTOR_FLYINGCREATURE;

		return true;
	}

	// 없으면
	return false;
}
*/

//----------------------------------------------------------------------
//
//                      iterator를 넘겨준다.
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ImageObject Iterator
//----------------------------------------------------------------------
// ImageObject가 있는 경우에.. 최초의 ImageObject에 대한
// Iterator를 넘겨준다.
//----------------------------------------------------------------------
OBJECT_MAP::const_iterator	
MSector::GetImageObjectIterator() const
{
	if (m_nImageObject)
	{
		// ImageObject의 위치에 있는 Object를 찾는다.
		return m_mapObject.find(POSITION_IMAGEOBJECT);
	}

	return m_mapObject.end();
}


//----------------------------------------------------------------------
//
//                       Effect List 처리
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Add Effect
//----------------------------------------------------------------------
// Effect List의 끝에 Effect를 추가한다.
//----------------------------------------------------------------------
void
MSector::AddEffect(const MEffect* pEffect)
{
	// NULL이 아닐 경우 추가..(그냥.. - -;;)
	if (pEffect!=NULL)
	{
		m_listEffect.push_back((MEffect*)pEffect);

		// Darkness인지 체크한다.
		if (!m_bDarkness || !m_bSanctuary)	// [새기술]
		{
			int fid = pEffect->GetFrameID();
			int est = g_pTopView->GetEffectSpriteType( (BLT_TYPE)pEffect->GetBltType(), fid );

//			if(est >= EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE &&
//				est >= EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3)
//				m_bDarkness = true;
			// darkness 있는 경우			
			if (est>=EFFECTSPRITETYPE_DARKNESS_2_1
					&& est<=EFFECTSPRITETYPE_DARKNESS_2_5)
			{
				m_bDarkness = true;
			}				
			
			if( est>= EFFECTSPRITETYPE_GRAY_DARKNESS_2_1 &&
				est<= EFFECTSPRITETYPE_GRAY_DARKNESS_2_5 )
			{
				m_bDarkness = true;
			}

			// sanctuary 있는 경우			
			if (est>=EFFECTSPRITETYPE_SANCTUARY_1
					&& est<=EFFECTSPRITETYPE_SANCTUARY_3)
			{
				m_bSanctuary = true;
			}

		}
	}
}

//----------------------------------------------------------------------
// Get Effect
//----------------------------------------------------------------------
// Effect ID가 id인 effect를 찾는다.
//----------------------------------------------------------------------
MEffect*	
MSector::GetEffect(TYPE_OBJECTID id) const
{
	EFFECT_LIST::const_iterator iEffect = m_listEffect.begin();

	while (iEffect != m_listEffect.end())
	{
		// 원하는 id인지 check
		if ((*iEffect)->GetID()==id)
		{
			return *iEffect;
		}

		iEffect++;
	}

	return NULL;
}


//----------------------------------------------------------------------
// Remove Effect(id)
//----------------------------------------------------------------------
// Effect List에서 ID가 id인 것을 찾아서 list에서 제거한다.
//----------------------------------------------------------------------
bool		
MSector::RemoveEffect(TYPE_OBJECTID id)
{
	EFFECT_LIST::iterator iEffect = m_listEffect.begin();

	while (iEffect != m_listEffect.end())
	{
		MEffect* pEffect = *iEffect;				

		// 원하는 id인지 check
		if (pEffect->GetID()==id)
		{
			// 제거~
			m_listEffect.erase( iEffect );

			// Darkness인지 체크한다.
			if (m_bDarkness || m_bSanctuary)	// [새기술]
			{
				int fid = pEffect->GetFrameID();
				int est = g_pTopView->GetEffectSpriteType( (BLT_TYPE)pEffect->GetBltType(), fid );

//				if(est >= EFFECTSPRITETYPE_MAP_BLACK_LARGE_SMOKE &&
//					est <= EFFECTSPRITETYPE_MAP_BLACK_SMALL_SMOKE_3
//					)
//				{
//					m_bDarkness = false;
//				}
				// darkness 있는 경우			
				if (est>=EFFECTSPRITETYPE_DARKNESS_2_1
						&& est<=EFFECTSPRITETYPE_DARKNESS_2_5)
				{
					m_bDarkness = false;
				}				

				if (est>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1
						&& est<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5)
				{
					m_bDarkness = false;
				}				

				// sanctuary 있는 경우			
				if (est>=EFFECTSPRITETYPE_SANCTUARY_1
						&& est<=EFFECTSPRITETYPE_SANCTUARY_3)
				{
					m_bSanctuary = false;
				}
			}

			return true;
		}

		iEffect++;
	}

	return false;
}

//----------------------------------------------------------------------
// Remove Effect(id, &pEffect)
//----------------------------------------------------------------------
// Effect List에서 ID가 id인 것을 찾아서 list에서 제거하고
// 그 Effect를 넘겨준다.
//----------------------------------------------------------------------
bool		
MSector::RemoveEffect(TYPE_OBJECTID id, MEffect*& pEffect)
{
	EFFECT_LIST::iterator iEffect = m_listEffect.begin();

	while (iEffect != m_listEffect.end())
	{
		MEffect* pEffect = *iEffect;

		// 원하는 id인지 check
		if (pEffect->GetID()==id)
		{
			// 제거할 Effect를 넘겨준다.
			pEffect = *iEffect;

			// 제거~
			m_listEffect.erase( iEffect );

			// Darkness인지 체크한다.
			if (m_bDarkness || m_bSanctuary)	// [새기술]
			{
				int fid = pEffect->GetFrameID();
				int est = g_pTopView->GetEffectSpriteType( (BLT_TYPE)pEffect->GetBltType(), fid );

				// darkness 있는 경우			
				if (est>=EFFECTSPRITETYPE_DARKNESS_2_1
						&& est<=EFFECTSPRITETYPE_DARKNESS_2_5)
				{
					m_bDarkness = false;
				}				

				if (est>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1
						&& est<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5)
				{
					m_bDarkness = false;
				}				

				// sanctuary 있는 경우			
				if (est>=EFFECTSPRITETYPE_SANCTUARY_1
						&& est<=EFFECTSPRITETYPE_SANCTUARY_3)
				{
					m_bSanctuary = false;
				}
			}

			return true;
		}

		iEffect++;
	}

	return false;
}

//----------------------------------------------------------------------
// Add Portal ( zoneID )
//----------------------------------------------------------------------
// 이미 있는거면 추가하지 않는다.
// cut&paste .. 우헤헤
//----------------------------------------------------------------------
bool					
MSector::AddPortal(int type, int zoneID)
{
	PORTAL_LIST::iterator iData = m_listPortal.begin();

	// 이미 list에 들어있는 모든 Data과 비교해보고
	// 같은게 없을 때 추가한다.
	while (iData != m_listPortal.end())
	{
		// 현재 들어있는게 추가할려는것보다 클 경우,
		// 현재 위치에 추가하면 된다.
		if (iData->ZoneID > zoneID)
		{
			m_listPortal.insert(iData, PORTAL_INFO(type, zoneID));

			m_fProperty |= FLAG_SECTOR_PORTAL;

			return true;
		}

		// 이미 있는 값이면
		// 추가하지 않는다.
		if (iData->ZoneID==zoneID)
		{
			return false;
		}

		iData++;
	}	

	// list의 모든 원소들보다 크므로 
	// list의 끝에 추가한다.
	m_listPortal.push_back( PORTAL_INFO(type, zoneID) );

	m_fProperty |= FLAG_SECTOR_PORTAL;

	return true;
}

//----------------------------------------------------------------------
// Clear Portal ( zoneID )
//----------------------------------------------------------------------
void					
MSector::ClearPortal()
{
	m_listPortal.clear();

	m_fProperty &= ~FLAG_SECTOR_PORTAL;
}

//----------------------------------------------------------------------
// Add SectorSound (SECTORSOUND_INFO&)
//----------------------------------------------------------------------
bool								
MSector::AddSectorSound(const SECTORSOUND_INFO& info)
{
	return AddSectorSound( info.ZoneSoundID, info.X, info.Y );
}

//----------------------------------------------------------------------
// Add SectorSound ( ZoneSoundID, Xo, Yo )
//----------------------------------------------------------------------
// 이미 있는거면 추가하지 않는다.
// cut&paste .. 우헤헤
//----------------------------------------------------------------------
bool					
MSector::AddSectorSound(int zoneSoundID, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	SECTORSOUND_LIST::iterator iData = m_listSectorSound.begin();

	// 이미 list에 들어있는 모든 Data과 비교해보고
	// 같은게 없을 때 추가한다.
	while (iData != m_listSectorSound.end())
	{
		// 현재 들어있는게 추가할려는것보다 클 경우,
		// 현재 위치에 추가하면 된다.
		if (iData->ZoneSoundID > zoneSoundID)
		{
			m_listSectorSound.insert(iData, SECTORSOUND_INFO(zoneSoundID, x, y));

			return true;
		}

		// 이미 있는 값이면
		// 추가하지 않는다.
		if (iData->ZoneSoundID==zoneSoundID)
		{
			return false;
		}

		iData++;
	}	

	// list의 모든 원소들보다 크므로 
	// list의 끝에 추가한다.
	m_listSectorSound.push_back( SECTORSOUND_INFO(zoneSoundID, x, y) );

	return true;
}

//----------------------------------------------------------------------
// Clear SectorSound ( zoneID )
//----------------------------------------------------------------------
void					
MSector::ClearSectorSound()
{
	m_listSectorSound.clear();
}