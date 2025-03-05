//----------------------------------------------------------------------
// MCreatureWear.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MCreatureWear.h"
#include "MItem.h"
#include "AddonDef.h"
#include "SkillDef.h"
#include "DebugInfo.h"
#include "CIndexSprite.h"
#include "ClientConfig.h"
#include "MEffectGeneratorTable.h"
#include "MEffectSpriteTypeTable.h"
#include "EffectSpriteTypeDef.h"
#include "MItemOptionTable.h"
#include "PacketFunction.h"
#include "MinTR.H"
#include "MTopView.h"

int defaultSkinColor		= 410;					
int defaultCoatColor		= 377;//368;	
int defaultTrouserColor	= 377;//489;	


#define	ATTACHEFFECTCOLOR_NULL		0xFFFF


//----------------------------------------------------------------------
// static member
//----------------------------------------------------------------------
// Creature의 현재 방향에 따라서...
// 옷을 출력해주는 순서가 다를 수 있다.
// 각 방향별로 옷 출력해주는 순서를 정하는 부분이다.
int			MCreatureWear::s_AddonOrder[DIRECTION_MAX][ADDON_MAX] = 
{
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// LEFT
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// LEFTDOWN
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// DOWN	
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHTDOWN	
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHT
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHTUP
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// UP
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND }	// LEFTUP
};

int	MCreatureWear::s_ACAddonOrder[ DIRECTION_MAX ][ AC_ADDON_MAX ] =
{
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_RIGHTHAND, AC_ADDON_LEFTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_RIGHTHAND, AC_ADDON_LEFTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_LEFTHAND, AC_ADDON_RIGHTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_LEFTHAND, AC_ADDON_RIGHTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_LEFTHAND, AC_ADDON_RIGHTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_LEFTHAND, AC_ADDON_RIGHTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_RIGHTHAND, AC_ADDON_LEFTHAND },
	{ AC_ADDON_MOTOR, AC_ADDON_BODY, AC_ADDON_HELM, AC_ADDON_SHOULDER, AC_ADDON_RIGHTHAND, AC_ADDON_LEFTHAND },
};


//----------------------------------------------------------------------
//
//  constructor/destructor
//
//----------------------------------------------------------------------
MCreatureWear::MCreatureWear()
{
	for (int i=0; i<ADDON_MAX; i++)
	{
		ClearAddonInfo( i );
	}

	m_HairFrameID = ADDONID_HAIR1_MALE;
	m_HairColorSet = 0;

	m_ShoulderAddon.bAddon = FALSE;
	m_ShoulderAddon.FrameID	= FRAMEID_NULL;		// 복장

	m_ShoulderAddon.ItemClass	= ITEM_CLASS_NULL;	// item class
	m_ShoulderAddon.ItemType	= ITEMTYPE_NULL;			// item type
	m_ShoulderAddon.ColorSet1	= 0;						// colorset1
	m_ShoulderAddon.ColorSet2	= 0;						// colorset2
	m_ShoulderAddon.bEffectColor = FALSE;					// effect color로 보여지는 부위인가?
	m_ShoulderAddon.EffectColorSet	= 0;				// effectcolorset
}

MCreatureWear::~MCreatureWear()
{
}

//----------------------------------------------------------------------
//
//  member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Clear AddonInfo
//----------------------------------------------------------------------
void			
MCreatureWear::ClearAddonInfo(int Addon)
{
	ADDON_INFO& addon = m_Addon[Addon];

	// 초기화
	addon.bAddon	= FALSE;			// 착용했나?	
	addon.FrameID	= FRAMEID_NULL;		// 복장

	addon.ItemClass	= ITEM_CLASS_NULL;	// item class
	addon.ItemType	= ITEMTYPE_NULL;			// item type
	addon.ColorSet1	= 0;						// colorset1
	addon.ColorSet2	= 0;						// colorset2
	addon.bEffectColor = FALSE;					// effect color로 보여지는 부위인가?
	addon.EffectColorSet	= 0;				// effectcolorset
}

//----------------------------------------------------------------------
// Set Same Wear
//----------------------------------------------------------------------
void				
MCreatureWear::SetSameWear(const MCreatureWear* pCreature)
{
	// 복장의 정보	
	for (int i=0; i<ADDON_MAX; i++)
	{
		m_Addon[i] = pCreature->m_Addon[i];
	}

	m_HairFrameID = pCreature->m_HairFrameID;
	m_HairColorSet = pCreature->m_HairColorSet;
}

//----------------------------------------------------------------------
// Set Addon ColorSet1
//----------------------------------------------------------------------
void			
MCreatureWear::SetAddonColorSet1(int Addon, WORD colorSet)	
{ 
	if (Addon >= ADDON_MAX)
	{
		return;
	}
	
	// 2004, 5, 4 sobeit add start - 다른 캐릭의 아우스터즈 부츠는 세팅하는 코드가 없어서 간단히 추가
	if(Addon == ADDON_TROUSER)
		m_Addon[Addon].bAddon = true;
	// 2004, 5, 4 sobeit add end

	if (colorSet < MAX_COLORSET || colorSet == UNIQUE_ITEM_COLOR || colorSet == QUEST_ITEM_COLOR)
	{	
		m_Addon[Addon].ColorSet1 = colorSet; 		
	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] Exceed Maximum ColorSet. addon=%d, set=%d", Addon, colorSet);		
	}
}

//----------------------------------------------------------------------
// Set Addon ColorSet2
//----------------------------------------------------------------------
void			
MCreatureWear::SetAddonColorSet2(int Addon, WORD colorSet)	
{ 
	if (Addon >= ADDON_MAX)
	{
		return;
	}

	// 2004, 5, 4 sobeit add start - 다른 캐릭의 아우스터즈 부츠는 세팅하는 코드가 없어서 간단히 추가
	if(Addon == ADDON_TROUSER)
		m_Addon[Addon].bAddon = true;
	// 2004, 5, 4 sobeit add end
	
	if (colorSet < MAX_COLORSET || colorSet == UNIQUE_ITEM_COLOR || colorSet == QUEST_ITEM_COLOR)
	{	
		// 운영자는 set1이다. T_T;;
		if (m_CreatureType==CREATURETYPE_SLAYER_OPERATOR
			&& Addon==ADDON_COAT)
		{
			m_ColorBody1 = m_ColorBody2 = colorSet;
		}
		
		m_Addon[Addon].ColorSet2 = colorSet; 		
	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] Exceed Maximum ColorSet. addon=%d, set=%d", Addon, colorSet);
	}
}

//----------------------------------------------------------------------
// New Item From AddonInfo
//----------------------------------------------------------------------
// Addon번째 정보로 Item을 생성한다.
// 외부에서 delete해줘야 한다.
//----------------------------------------------------------------------
MItem*
MCreatureWear::NewItemFromAddonInfo(int Addon)
{
	if (Addon >= ADDON_MAX)
	{
		return false;
	}

	ADDON_INFO& addon = m_Addon[Addon];

	//--------------------------------------------------
	// item을 착용한 경우
	//--------------------------------------------------
	if (!addon.bAddon || addon.ItemClass==ITEM_CLASS_NULL)
	{
		return NULL;
	}
	
	//--------------------------------------------------
	// item의 정보를 알기위해서 생성해서 제거한다.
	//--------------------------------------------------
	MItem* pItem = MItem::NewItem( addon.ItemClass );
	pItem->SetItemType( addon.ItemType );

	return pItem;
}

//----------------------------------------------------------------------
// Remove Addon
//----------------------------------------------------------------------
// Addon의 위치의 복장을 없앤다.
//----------------------------------------------------------------------
bool		
MCreatureWear::RemoveAddon(int Addon)
{ 
	DEBUG_ADD("MCreatureWear::RemoveAddon");
	
	if (Addon >= ADDON_MAX)
	{
		return false;
	}

	ADDON_INFO& addon = m_Addon[Addon];

	//--------------------------------------------------
	// 장착한게 있을 경우에만 벗긴다. - -;
	//--------------------------------------------------
	if (addon.bAddon)
	{
		MItem* pItem = NewItemFromAddonInfo( Addon );
		
		if (pItem!=NULL)
		{
			//--------------------------------------------------
			// addon Item을 복장에서 제거한다.
			//--------------------------------------------------
			bool bRemove = RemoveAddonItem( pItem );

			delete pItem;

			return bRemove;
		}

		//--------------------------------------------------
		// 정보 제거..
		//--------------------------------------------------
		ClearAddonInfo( Addon );

		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Set AddonItem
//---------------------------------------------------------------------------
// 머리카락을 설정한다.
//---------------------------------------------------------------------------
void			
MCreatureWear::SetAddonHair(TYPE_FRAMEID id, WORD cs1)
{
	ADDON_INFO& addon = m_Addon[ADDON_HAIR];

	// 초기화
	addon.bAddon	= TRUE;			// 착용했나?	
	addon.FrameID	= id;		// 복장

	addon.ItemClass	= ITEM_CLASS_NULL;	// item class

	addon.ItemType	= ITEMTYPE_NULL;			// item type
	addon.ColorSet1	= cs1;						// colorset1
	addon.ColorSet2	= 0;						// colorset2
	addon.bEffectColor = FALSE;
	addon.EffectColorSet = 0;

	// 저장해 둔다.
	m_HairFrameID = id;
	m_HairColorSet = cs1;
}

//---------------------------------------------------------------------------
// Set AddonItem
//---------------------------------------------------------------------------
// 장착해서 모양이 바뀌는 item(AddonItem)인 경우
// Creature에 장착하고 상태를 바꾼다.
//---------------------------------------------------------------------------
bool
MCreatureWear::SetAddonItem(MItem* pItem)
{
	DEBUG_ADD("MCreatureWear::SetAddonItem");
	
	if (pItem==NULL)
	{
		DEBUG_ADD("The Item is NULL");
		
		return false;
	}

	//-------------------------------------------------
	//
	// 정지한 상태인 경우만 복장을 바꿀 수 있다.
	//
	//-------------------------------------------------
	// player가 아닌 경우나..
	// 오토바이를 타는 경우는..
	// 그냥 정지하게 해 버린다. - -;
	if (GetClassType()!=MCreature::CLASS_PLAYER
		|| pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
	{
		SetAction( (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );
	}

	if (m_Action==ACTION_STAND 
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND
		|| IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND )
	{
		//-------------------------------------------------
		// 장착해서 모양이 바뀌는 Item인 경우에만 처리한다.
		//-------------------------------------------------
		if (pItem->IsAddonItem() && pItem->IsAffectStatus())
		{
			TYPE_FRAMEID	fid;
			ADDON			Addon = pItem->GetAddonSlot();

			//-------------------------------------------------
			// 복장이 바뀌는 위치가 있는 경우
			//-------------------------------------------------
			if (pItem->GetAddonSlot()==ADDON_NULL)
			{			
				DEBUG_ADD_FORMAT("[Error] Item doesn't have Addon Slot. id=%d, class=%d, type=%d", 
														pItem->GetID(), (int)pItem->GetItemClass(), (int)pItem->GetItemType());
				
			}
			else
			{	
				//-------------------------------------------------
				//
				//			장착하는 위치에 따라서 처리..
				//
				//-------------------------------------------------
				switch (Addon)
				{
					//-------------------------------------------------
					// 오토바이를 장착한 경우
					//-------------------------------------------------
					case ADDON_MOTOR :
						//------------------------------------------
						// 움직이는 방법 바꿈
						//------------------------------------------					
						SetMoveDevice( MCreature::MOVE_DEVICE_RIDE );
					break;

					//-------------------------------------------------
					// 장착할려는게 왼손무기?인 경우
					//-------------------------------------------------
					case ADDON_LEFTHAND :
					{
						// 방패밖에 없다. - -;
						// 필살 하드코딩.. - -;;;;

						//-------------------------------------------------
						// 기존에 있던 오른손무기가 양손무기이면 
						// 오른손Addon을 제거한다.
						//-------------------------------------------------
						MItem* pOldItem = NewItemFromAddonInfo( ADDON_RIGHTHAND );

						// 오른손 item이 있는 경우 제거한다.
						if (pOldItem!=NULL)
						{	
							if (pOldItem->IsGearSlotTwoHand())
							{
								RemoveAddon( ADDON_RIGHTHAND );
							}

							delete pOldItem;
						}
					}
					break;

					//-------------------------------------------------
					// 장착할려는게 오른손무기?인 경우
					//-------------------------------------------------
					case ADDON_RIGHTHAND :
					{
						// 양손무기이면 왼손을 없앤다.
						if (pItem->IsGearSlotTwoHand())
						{
							RemoveAddon( ADDON_LEFTHAND );
						}				
					}	
					break;

					//-------------------------------------------------
					// 모자
					//-------------------------------------------------
					// 모자인 경우는
					// 짧은 머리카락이라면 없앤다.
					// 짧은 머리 : 여1, 남123
					//-------------------------------------------------
					case ADDON_HELM :
						if (m_HairFrameID==ADDONID_HAIR1_FEMALE
							|| m_HairFrameID==ADDONID_HAIR1_MALE
							|| m_HairFrameID==ADDONID_HAIR2_MALE
							|| m_HairFrameID==ADDONID_HAIR3_MALE)
						{
							RemoveAddon( ADDON_HAIR );
						}
					break;

					//-------------------------------------------------
					// 운영자인 경우.. 옷 바꾸면 몸 색깔 바뀐다.
					//-------------------------------------------------
					case ADDON_COAT :
						if (m_CreatureType==CREATURETYPE_SLAYER_OPERATOR
							|| m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR
							|| m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR)
						{
//							if(pItem->IsUniqueItem())
//								m_ColorBody1 = m_ColorBody2 = UNIQUE_ITEM_COLOR;
//							else
								m_ColorBody1 = m_ColorBody2 = pItem->GetItemOptionColorSet();
						}
					break;
				}
					
				//-------------------------------------------------
				//
				//			성별에 따른 FrameID
				//
				//-------------------------------------------------	
				if (IsMale())
				{
					fid = pItem->GetAddonMaleFrameID();
				}			
				else //if (pCreature->IsFemale())
				{
					fid = pItem->GetAddonFemaleFrameID();			
				}
			
				//-------------------------------------------------	
				// Slayer이면서 fid가 없는 경우..
				//-------------------------------------------------					
				if (fid == FRAMEID_NULL && IsSlayer())
				{					
					DEBUG_ADD_FORMAT("[Error] Item doesn't have FrameID. id=%d, class=%d, type=%d", 
										pItem->GetID(), (int)pItem->GetItemClass(), (int)pItem->GetItemType());					
				}
				//-------------------------------------------------					
				// 정상적이거나.. Vampire인 경우
				//-------------------------------------------------					
				else
				{	
					//-------------------------------------------------
					// 장착한다.
					//-------------------------------------------------
					ADDON_INFO& addon = m_Addon[Addon];

					// 초기화
					addon.bAddon	= TRUE;
					addon.FrameID	= fid;

					addon.ItemClass	= pItem->GetItemClass();
					addon.ItemType	= pItem->GetItemType();

					// 뱀파 옷추가
					if(pItem->GetItemClass() == ITEM_CLASS_VAMPIRE_COAT)
					{
						unsigned short nSpecialActionInfo = GetSpecialActionInfo();

						if(m_CreatureType != CREATURETYPE_VAMPIRE_OPERATOR)
						{
							if (m_CreatureType != CREATURETYPE_WOLF && m_CreatureType != CREATURETYPE_BAT )
							{
								if( m_CreatureType == CREATURETYPE_WER_WOLF )
								{
									SetCreatureType( CREATURETYPE_WER_WOLF );
								}								
								else
								{
									if (IsMale())
									{
										SetCreatureType(pItem->GetAddonMaleFrameID());
									}			
									else //if (pCreature->IsFemale())
									{
										SetCreatureType(pItem->GetAddonFemaleFrameID());
										//SetCreatureType(812);
									}
								}
							}
							addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 피부
//							if(pItem->IsUniqueItem())
//								m_ColorBody2 = UNIQUE_ITEM_COLOR;
//							else
							m_ColorBody2	= pItem->GetItemOptionColorSet();			// option
							if( m_CreatureType == CREATURETYPE_WER_WOLF )
								m_ColorBody1 = m_ColorBody2;
								
						}
						else
						{
							m_ColorBody1 = m_ColorBody2 = pItem->GetItemOptionColorSet();
						}

						SetSpecialActionInfo(nSpecialActionInfo);
					}

					else 
					if (Addon==ADDON_COAT || Addon==ADDON_TROUSER)
					{
						{
							addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 피부
							//						if(pItem->IsUniqueItem())
							//							addon.ColorSet2	= UNIQUE_ITEM_COLOR;			// option
							//						else
							addon.ColorSet2	= pItem->GetItemOptionColorSet();			// option
							addon.bEffectColor = FALSE;
							addon.EffectColorSet = 0;
							
							// 상의인 경우만 옷이 바뀐다.
							//if (Addon==ADDON_COAT)
							{
								//-------------------------------------------------
								// Vampire만 적용된다.
								//-------------------------------------------------
								if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR)
								{
									//								if(pItem->IsUniqueItem())
									//									m_ColorBody1 = m_ColorBody2 = UNIQUE_ITEM_COLOR;
									//								else
									m_ColorBody1 = m_ColorBody2 = pItem->GetItemOptionColorSet();
								}
								else
								{
									//								if(pItem->IsUniqueItem())
									//									m_ColorBody2 = UNIQUE_ITEM_COLOR;
									//								else
									m_ColorBody2 = pItem->GetItemOptionColorSet();
								}
							}
						}
					}
					else
					{
//						if(pItem->IsUniqueItem())
//						{
//							addon.ColorSet1	= UNIQUE_ITEM_COLOR;	// option
//							addon.ColorSet2	= UNIQUE_ITEM_COLOR;//pItem->GetItemColorSet();			// null
//						}
//						else
						{
							addon.ColorSet1	= pItem->GetItemOptionColorSet();	// option
							addon.ColorSet2	= pItem->GetItemOptionColorSet();//pItem->GetItemColorSet();			// null
						}
						addon.bEffectColor = FALSE;
						addon.EffectColorSet = 0;
					}					
				}				
			}
		}			
		else
		{
			DEBUG_ADD_FORMAT("[Error] Item is Not Addon Item. id=%d, class=%d, type=%d", 
									pItem->GetID(), (int)pItem->GetItemClass(), (int)pItem->GetItemType());			
		}

		//-------------------------------------------------
		// 장착한 Item이 
		// 기본공격용 아이템이라면...
		// 기본공격ActionInfo가 바뀌어야 한다.
		//-------------------------------------------------
		if (pItem->IsBasicWeapon() && pItem->IsAffectStatus())
		{
			TYPE_ACTIONINFO ai = pItem->GetUseActionInfo();

			//-------------------------------------------------
			// 설정된 actionInfo가 없는 경우
			//-------------------------------------------------
			if (ai==ACTIONINFO_NULL)
			{
				// 기본 맨손 공격 동작으로 전환한다.
				SetBasicActionInfo( SKILL_ATTACK_MELEE );
				
				DEBUG_ADD("[Empty ActionInfo]This Creature's basic attack is Melee");				
			}
			//-------------------------------------------------
			// 설정된 actionInfo가 있다면..
			//-------------------------------------------------
			else
			{
				// 기본 공격 동작으로 설정한다.
				SetBasicActionInfo( ai );
				
				DEBUG_ADD_FORMAT("[Set BasicActionInfo] %d", ai);
			}
		}

		//-------------------------------------------------
		// 오른손이 비어있으면..
		// 공격은 맨손이라고 보면 된다..
		// 하드 코딩.. - -;;
		//-------------------------------------------------
		if (!m_Addon[ADDON_RIGHTHAND].bAddon)
		{
			DEBUG_ADD("[Empty RightHand]This Creature's basic attack is Melee");
			
			// 기본 맨손 공격 동작으로 전환한다.
			SetBasicActionInfo( SKILL_ATTACK_MELEE );			
		}

		return true;	
		
	}

	//-------------------------------------------------
	//
	// 정지한 상태가 아닌 경우
	//
	//-------------------------------------------------
	DEBUG_ADD_FORMAT("[Error] Not STAND action. id=%d, action=%d", m_ID, m_Action);
	
	// 다음에 정지할 때, 복장을 바꾸도록 한다..
	// 그래야 되는데.. 귀찮다. - -;;

	return false;	
}

//---------------------------------------------------------------------------
// Remove AddonItem
//---------------------------------------------------------------------------
// AddonItem을 장착해제 한다.
//---------------------------------------------------------------------------
bool
MCreatureWear::RemoveAddonItem( MItem* pItem )
{
	DEBUG_ADD("MCreatureWear::RemoveAddonItem");

	if (pItem==NULL)
	{
		DEBUG_ADD("The Item is NULL");
		
		return false;
	}

	//-------------------------------------------------
	//
	// 정지한 상태인 경우만 복장을 바꿀 수 있다.
	//
	//-------------------------------------------------
	// player가 아닌 경우나..
	// 오토바이를 타는 경우는..
	// 그냥 정지하게 해 버린다. - -;
	if (GetClassType()!=MCreature::CLASS_PLAYER
		|| pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
	{
		SetAction( (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );
	}


	if (m_Action==ACTION_STAND
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND
		|| IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND )
	{
		// 뱀파 옷추가
		if(pItem->GetItemClass() == ITEM_CLASS_VAMPIRE_COAT)
		{
			if(m_CreatureType != CREATURETYPE_VAMPIRE_OPERATOR)
			{
				if (m_CreatureType != CREATURETYPE_WOLF && m_CreatureType != CREATURETYPE_BAT )
				{
					if( m_CreatureType == CREATURETYPE_WER_WOLF )
					{
						SetCreatureType( CREATURETYPE_WER_WOLF );
						m_ColorBody1 = defaultCoatColor;
					} else
					{
						if (IsMale())	// 아씨 하드 코딩 하면 안대는데-ㅅ-
						{
							SetCreatureType(2);
						}			
						else //if (pCreature->IsFemale())
						{
							SetCreatureType(3); //by viva
						}
					}
				}
				//						addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 피부

				m_ColorBody2	= defaultCoatColor;
			}
			else
			{
				m_ColorBody1	= m_ColorBody2	= defaultCoatColor;
			}
		}

		//-------------------------------------------------
		// 착용해서 모양이 바뀌는 Item인가?
		//-------------------------------------------------
		// --> 장착 부위를 알아내서 
		//    그 부위의 Addon을 없애야 한다.
		//-------------------------------------------------
//		else
		if (pItem->IsAddonItem())
		{						
			enum ADDON add = pItem->GetAddonSlot();

			//-------------------------------------------------
			// 장착 부위의 Addon을 없앤다.
			//-------------------------------------------------
			if (add == ADDON_NULL)
			{				
				DEBUG_ADD_FORMAT("[Error] Item doesn't have Addon Slot. id=%d, class=%d, type=%d", 
													pItem->GetID(), (int)pItem->GetItemClass(), (int)pItem->GetItemType());				
			}
			else
			{
				//--------------------------------------------------
				// 정보를 제거 한다.
				//--------------------------------------------------
				ClearAddonInfo( add );

				//-------------------------------------------------
				//
				//			장착하는 부위에 따라서
				//
				//-------------------------------------------------	
				switch (add)
				{
					//-------------------------------------------------
					// 상의 : 맨몸일 경우에는 기본몸으로..
					//-------------------------------------------------
					case ADDON_COAT :
					{
						//MItem* pItem = MItem::NewItem( ITEM_CLASS_TROUSER );
						//pItem->SetItemType( 0 );

						//SetAddonItem( pItem );	

						//delete pItem;
						//-------------------------------------------------
						// 장착한다.
						//-------------------------------------------------						
						ADDON_INFO& addon = m_Addon[ADDON_COAT];

						// 초기화
						addon.bAddon	= TRUE;
						if(GetRace() == RACE_OUSTERS)
						{
							addon.FrameID = 1;
							addon.ColorSet1 = defaultCoatColor;
						}
						else
						{
							addon.FrameID	= IsMale()? ADDONID_COAT0_MALE : ADDONID_COAT0_FEMALE; 
							addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
						}

						addon.ItemClass	= ITEM_CLASS_COAT;
						addon.ItemType	= 0;	//by viva
						addon.ColorSet2	= defaultCoatColor;
						addon.bEffectColor = FALSE;
						addon.EffectColorSet = 0;
						
						//-------------------------------------------------	
						// Vampire만 적용된다.
						// 운영자도 적용되겠지..
						//-------------------------------------------------													
						if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR
							|| m_CreatureType==CREATURETYPE_SLAYER_OPERATOR
							|| m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR
							)
						{
							m_ColorBody1 = m_ColorBody2 = defaultSkinColor;	
						}
						else
						{
							m_ColorBody2 = defaultCoatColor;
						}
						
					}
					break;

					//-------------------------------------------------
					// 하의 : 맨몸일 경우에는 기본몸으로..
					//-------------------------------------------------
					case ADDON_TROUSER :
					{
						//MItem* pItem = MItem::NewItem( ITEM_CLASS_TROUSER );
						//pItem->SetItemType( 0 );

						//SetAddonItem( pItem );

						//delete pItem;
						//-------------------------------------------------
						// 장착한다.
						//-------------------------------------------------
						ADDON_INFO& addon = m_Addon[ADDON_TROUSER];

						// 초기화
						addon.bAddon	= TRUE;
						addon.FrameID	= IsMale()? ADDONID_TROUSER0_MALE : ADDONID_TROUSER0_FEMALE;

						addon.ItemClass	= ITEM_CLASS_TROUSER;
						addon.ItemType	= 0;
						addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
						addon.ColorSet2	= defaultTrouserColor;
						addon.bEffectColor = FALSE;
						addon.EffectColorSet = 0;
					}
					break;

					//-------------------------------------------------
					// 오른손은 무기..이므로..
					//-------------------------------------------------				
					case ADDON_RIGHTHAND :
						// 기본 맨손 공격 동작으로 전환한다.
						SetBasicActionInfo( SKILL_ATTACK_MELEE );

						// 특정 무기에만 걸리는 Effect
						switch (pItem->GetItemClass())
						{
							//-------------------------------------------------	
							// 도
							//-------------------------------------------------	
							case ITEM_CLASS_BLADE :
								if (HasEffectStatus(EFFECTSTATUS_BERSERKER))
								{
									RemoveEffectStatus(EFFECTSTATUS_BERSERKER);
								}
							break;

							//-------------------------------------------------	
							// 십자가
							//-------------------------------------------------	
							case ITEM_CLASS_CROSS :
							case ITEM_CLASS_MACE :
								if (HasEffectStatus(EFFECTSTATUS_HYPNOSIS))
								{
									RemoveEffectStatus(EFFECTSTATUS_HYPNOSIS);
								}
							break;
						}
					break;

					//-------------------------------------------------
					// 오토바이 장착 해제
					//-------------------------------------------------
					case ADDON_MOTOR :
						//------------------------------------------
						// 움직이는 방법 바꿈
						//------------------------------------------					
						SetMoveDevice( MCreature::MOVE_DEVICE_WALK );
					break;

					//-------------------------------------------------
					// 모자
					//-------------------------------------------------
					// 모자를 제거한 경우는 
					// 짧은 머리카락을 다시 붙여준다.
					//-------------------------------------------------
					case ADDON_HELM :
						if (m_HairFrameID==ADDONID_HAIR1_FEMALE
							|| m_HairFrameID==ADDONID_HAIR1_MALE
							|| m_HairFrameID==ADDONID_HAIR2_MALE
							|| m_HairFrameID==ADDONID_HAIR3_MALE)
						{
							SetAddonHair( m_HairFrameID, m_HairColorSet );
						}
					break;
				}
				
			}

			return true;
		}

		// addon item이 아닌 경우
		return true;
	}

	//-------------------------------------------------
	//
	// 정지한 상태가 아닌 경우
	//
	//-------------------------------------------------
	DEBUG_ADD_FORMAT("[Error] Not STAND action. id=%d, action=%d", m_ID, m_Action);
	
	// 다음에 정지할 때, 복장을 바꾸도록 한다..
	// 그래야 되는데.. 귀찮다. - -;;


	return false;
}

//---------------------------------------------------------------------------
// Set Creature Type
//---------------------------------------------------------------------------
// 기본 복장을 입힌다.
//---------------------------------------------------------------------------
void
MCreatureWear::SetCreatureType(TYPE_CREATURETYPE type)
{
	//-------------------------------------------------
	// type설정
	//-------------------------------------------------
	MCreature::SetCreatureType( type );

	//-------------------------------------------------
	// 다 벗긴다.
	//-------------------------------------------------
	for (int i=0; i<ADDON_MAX; i++)
	{
		ClearAddonInfo( i );
	}

	if(IsVampire())
	{
		//-------------------------------------------------
		// 상의 : 맨몸일 경우에는 기본몸으로..
		//-------------------------------------------------
		ADDON_INFO& addon = m_Addon[ADDON_COAT];

		// 초기화
		addon.bAddon	= TRUE;
//		addon.FrameID	= IsMale()? ADDONID_COAT0_MALE : ADDONID_COAT0_FEMALE;
		
		addon.ItemClass	= ITEM_CLASS_VAMPIRE_COAT;
		addon.ItemType	= 0;
		addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
		addon.ColorSet2	= defaultCoatColor;	
		addon.bEffectColor = FALSE;
		addon.EffectColorSet = 0;
	}
	else
	if(IsOusters())
	{
		//-------------------------------------------------
		// 상의 : 맨몸일 경우에는 기본몸으로..
		//-------------------------------------------------
		ADDON_INFO& addon = m_Addon[ADDON_COAT];
		
		// 초기화
		addon.bAddon	= TRUE;
		addon.FrameID	= 1;
		//		addon.FrameID	= IsMale()? ADDONID_COAT0_MALE : ADDONID_COAT0_FEMALE;
		
		addon.ItemClass	= ITEM_CLASS_OUSTERS_COAT;
		addon.ItemType	= 0;
		addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
		addon.ColorSet2	= defaultCoatColor;	
		addon.bEffectColor = FALSE;
		addon.EffectColorSet = 0;
	}
	else
	{
		//-------------------------------------------------
		// 상의 : 맨몸일 경우에는 기본몸으로..
		//-------------------------------------------------
		ADDON_INFO& addon = m_Addon[ADDON_COAT];

		// 초기화
		addon.bAddon	= TRUE;
		addon.FrameID	= IsMale()? ADDONID_COAT0_MALE : ADDONID_COAT0_FEMALE;
		
		addon.ItemClass	= ITEM_CLASS_COAT;
		addon.ItemType	= 0;
		addon.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
		addon.ColorSet2	= defaultCoatColor;	
		addon.bEffectColor = FALSE;
		addon.EffectColorSet = 0;
		
		//-------------------------------------------------
		// 하의 : 맨몸일 경우에는 기본몸으로..
		//-------------------------------------------------
		ADDON_INFO& addon2 = m_Addon[ADDON_TROUSER];
		
		// 초기화
		addon2.bAddon	= TRUE;
		addon2.FrameID	= IsMale()? ADDONID_TROUSER0_MALE : ADDONID_TROUSER0_FEMALE;
		
		addon2.ItemClass	= ITEM_CLASS_TROUSER;
		addon2.ItemType	= 0;
		addon2.ColorSet1	= m_ColorBody1;//defaultSkinColor;	// 기본 피부색
		addon2.ColorSet2	= defaultTrouserColor;
		addon2.bEffectColor = FALSE;
		addon2.EffectColorSet = 0;
	}
}

//----------------------------------------------------------------------
// Remove EffectStatus
//----------------------------------------------------------------------
// 특정한 EffectSpriteType을 제거한다.
//----------------------------------------------------------------------
bool
MCreatureWear::RemoveEffectStatus(EFFECTSTATUS status)
{
	DEBUG_ADD_FORMAT("[RemoveEffectStatus] id=%d, status=%d", m_ID, (int)status);
	
	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	if (!m_bEffectStatus[status])
	{
		return false;
	}

	m_bEffectStatus[status] = false;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;
	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
	//------------------------------------------------------------
	// effectStatus에 따라서.
	//------------------------------------------------------------
	switch (status)
	{
		case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
		case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
		case EFFECTSTATUS_SUMMON_SYLPH_RED :
		case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
		case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
		case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
		case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
		case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
		case EFFECTSTATUS_SUMMON_SYLPH:
			SetMoveDevice(MOVE_DEVICE_WALK);
			if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
			{
				type = EFFECTSPRITETYPE_OUSTERS_FASTMOVE_LOOP;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_END,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, 8, NULL, false);
			}
		break;
		case EFFECTSTATUS_BIKE_CRASH:
			SetMoveDevice(MOVE_DEVICE_WALK);
			break;
		//------------------------------------------------------------
		// 마비 풀릴 때
		//------------------------------------------------------------
		// add by Coffee 2007-3-21
		case EFFECTSTATUS_SATELLITE_BOMB_AIM :
		// end	
		case EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM :
			if (m_bAlive)
			{
				SetStop();
				StopGunShotGuidance();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[GSG] Stop");
	#endif
			}
		break;

		//------------------------------------------------------------
		// 마비 풀릴 때
		//------------------------------------------------------------
		case EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS :
		case EFFECTSTATUS_EXPLOSION_WATER :
			if (m_bAlive)
			{
				SetStop();
				StopCauseCriticalWounds();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[CCW] Stop");
	#endif
			}
		break;

		case EFFECTSTATUS_CURSE_PARALYSIS :
			if (m_bAlive)
			{
				SetStop();
				Remove_Curse_Paralsis_Effect();
			}
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [새기술]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// 바로 invisible로 만든다.
			RemoveCasket();
		break;

		//------------------------------------------------------------
		// invisible 제거
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
		case EFFECTSTATUS_SNIPPING_MODE :
			// invisible을 제거한다.
			SetVisible();
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_COMA
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetAlive();
			if(IsVampire())
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_RESURRECT,										// 사용 기술 번호
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 사용 방향
							
							m_ID,												// 목표에 대한 정보
							m_X, m_Y, 0,
							
							2*16, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}

		case EFFECTSTATUS_GHOST:
			if (!(*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 박쥐인 경우
				SetGroundCreature();
			break;
		break;
		case EFFECTSTATUS_GLACIER:	
			MCreature::RemoveGlacierEffect();
			break;

		case EFFECTSTATUS_INSTALL_TURRET:
			SetAction( ACTION_STAND );
			SetInstallTurretCount(0);
			SetInstallTurretDirect(2);
			SetDirection( 2 );
			SetCurrentDirection( 2 );
			break;
	}

	//------------------------------------------------------------
	//
	//			EffectSprite로 표현하는 경우
	//
	//------------------------------------------------------------
	if (bUseEffectSprite)
	{
		//TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
		TYPE_EFFECTSPRITETYPE type2 = EFFECTSPRITETYPE_NULL;

		if(type == EFFECTSPRITETYPE_FAST_MOVE_STOP)
			type2 = EFFECTSPRITETYPE_FAST_MOVE_FLY;

		_MinTrace("remove effectsprite\n");

		//-------------------------------------------------------
		// Creature에 붙어 있는 Effect중에서
		// EffectSpriteType이 type인 것을 제거한다.
		//-------------------------------------------------------
		if (type < g_pEffectSpriteTypeTable->GetSize())
		{
			if (IsFemale()
				&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
			{
				type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;
			}
			

			if (m_bAttachEffect[type])
			{
				//-------------------------------------------------------
				// 몸에 붙은 경우
				//-------------------------------------------------------			
				ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

				bool re = false;

				while (iEffect != m_listEffect.end())
				{
					MAttachEffect* pEffect = *iEffect;
					
					//-------------------------------------------------------
					// 같은 type을 찾는다.
					//-------------------------------------------------------
					if (pEffect->IsEffectSprite() 
						&& (pEffect->GetEffectSpriteType() == type ||
						type2 != EFFECTSPRITETYPE_NULL && pEffect->GetEffectSpriteType() == type2 ) )
					{						
						// 메모리 제거
						delete pEffect;
						pEffect = NULL;
						
						ATTACHEFFECT_LIST::iterator dEffect = iEffect;
						iEffect--;

						// list에서 제거
						m_listEffect.erase( dEffect );

						m_bAttachEffect[type] = false;
						
						re = true;
					}					
					
					iEffect++;
				}

				if(re)
					return true;

				//-------------------------------------------------------
				// 바닥에 붙은 경우
				//-------------------------------------------------------
				iEffect = m_listGroundEffect.begin();

				while (iEffect != m_listGroundEffect.end())
				{
					MAttachEffect* pEffect = *iEffect;
					
					//-------------------------------------------------------
					// 같은 type을 찾는다.
					//-------------------------------------------------------
					if (pEffect->IsEffectSprite() && 
						( pEffect->GetEffectSpriteType() == type || type2 != EFFECTSPRITETYPE_NULL && 
						type2 == pEffect->GetEffectSpriteType()) )
					{
						// 메모리 제거
						delete pEffect;

						// list에서 제거
						m_listGroundEffect.erase( iEffect );

						m_bAttachEffect[type] = false;
				
						return true;
					} 
					iEffect++;
				}
			}
		}
	}
	//------------------------------------------------------------
	//
	//			EffectColor로 표현하는 경우
	//
	//------------------------------------------------------------
	else
	{
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		//-------------------------------------------------------
		// Creature에 붙어 있는 Effect중에서
		// EffectColor가 color인 것을 제거한다.
		//-------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		while (iEffect != m_listEffect.end())
		{
			MAttachEffect* pEffect = *iEffect;
			
			//-------------------------------------------------------
			// 같은 type을 찾는다.
			//-------------------------------------------------------
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				ADDON	part = pEffect->GetEffectColorPart();

				//------------------------------------------------------------
				// 특정한 부위만 색깔이 바뀌는 경우도 있다.
				//------------------------------------------------------------
				if (part < ADDON_MAX)
				{
					m_Addon[part].bEffectColor = FALSE;
				}
				//------------------------------------------------------------
				// 모든 부위를 원래대로...
				//------------------------------------------------------------
				else
				{
					for (int p=0; p<ADDON_MAX; p++)
					{
						m_Addon[p].bEffectColor = FALSE;
					}
				}

				// 메모리 제거
				delete pEffect;

				// list에서 제거
				m_listEffect.erase( iEffect );

				return true;
			}
			
			iEffect++;
		}		
	}
	
	return false;
}

//----------------------------------------------------------------------
// Update Effect
//----------------------------------------------------------------------
// Creature에 붙은 모든 Effect의 Frame을 바꿔주고..
// 끝나는게 있으면 list와 memory에서 삭제한다.
//----------------------------------------------------------------------
void		
MCreatureWear::UpdateAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	ATTACHEFFECT_LIST::iterator iEffectTemp;

	// 밝기는 0
	//m_MaxEffectLight = 0;

	//---------------------------------------------------------------------
	// 캐릭터 색깔 바꾸는 색..
	//---------------------------------------------------------------------
	// NULL값이당.. 외부에서 체크해야함..
	m_AttachEffectColor = m_ChangeColorSet;	//ATTACHEFFECTCOLOR_NULL;
	int bShowColor = (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS) 
						|| g_CurrentFrame % g_pClientConfig->FRAME_DRAW_ORIGINAL_SPRITE);	// 몇 frame마다 한번씩은 원래 색깔을 보여준다.

	int bShowColorPart = (g_CurrentFrame & 0x00000004);	// 4 frame씩 번갈아가며 색 출력
	int numColors = 0;
	BOOL bChangeColor = FALSE;

	BOOL	bAddonColorPart[ADDON_MAX] = { FALSE, };
	WORD	addonColor[ADDON_MAX] = { m_ChangeColorSet, };

	//---------------------------------------------------------------------
	// 모든 Effect를 Update한다.
	//---------------------------------------------------------------------
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		bool			bErase = false;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif

		if( pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_FLY ||
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_STOP ||
			(
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP
			)
			)
		{		
			BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].BltType;
			TYPE_EFFECTSPRITETYPE		EffectSpriteType = pEffect->GetEffectSpriteType();
			bool			bNewSummonSylphEffect = false;
			bool			bNewSummonSylphEffectStop = false;
			
			if( EffectSpriteType >= EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
				EffectSpriteType <= EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP )
			{
				bNewSummonSylphEffect = true;

				if( (EffectSpriteType - EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY) & 0x1 )
					bNewSummonSylphEffectStop = true;
			}
			
			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_STAND && (
				EffectSpriteType != EFFECTSPRITETYPE_FAST_MOVE_STOP ||
				( bNewSummonSylphEffect && !bNewSummonSylphEffectStop )
				)
			  )
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].FrameID;
				
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType + 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			
			
				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_STOP );
				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType + 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
			else
			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_MOVE && 
				( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_FAST_MOVE_FLY ||
				( bNewSummonSylphEffect && bNewSummonSylphEffectStop )
			  ) )
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_FLY].FrameID;
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType - 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			

				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_FLY );

				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType - 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
		} 
		else if( pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW )
		{
			if(!(  GetAction() == ACTION_SLAYER_SWORD_2 || GetAction() == ACTION_SLAYER_SWORD_2_SLOW || GetAction() == ACTION_SLAYER_SWORD_2_FAST ) 
				&& m_RepeatCount <= 0 )
			{
				pEffect->SetCount(0);
			}
		}
		else if(		
					(pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_START &&
					pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_END &&
					!HasEffectStatus( EFFECTSTATUS_TRYING ) 
					)
			)
		{
			bErase = true;			
		}

		if (bErase == false && pEffect->Update())
		{
			// -_- 흡영은 방향 안바꾼다..
			if( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_ABSORB_SOUL )
				pEffect->SetDirection( m_CurrentDirection );
			// 최고 밝기를 가진 Effect의 밝기를 저장한다.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 위치 설정
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 위치가 바뀌었거나
			// 빛의 크기(시야)가 바뀐 경우
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 시야 바꾸기
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
			}

			//---------------------------------------------------------------------
			// 캐릭터 색깔 바꾸는 Effect이면
			// 하나를 선택해야 한다.
			//---------------------------------------------------------------------
			if (pEffect->IsEffectColor())
			{
				ADDON	part = pEffect->GetEffectColorPart();

				//--------------------------------------------------------
				// 부분적으로 색깔 바뀌는 effect
				//--------------------------------------------------------
				if (part < ADDON_MAX)
				{
					if (bShowColorPart)
					{
						bAddonColorPart[part] = TRUE;
						addonColor[part] = pEffect->GetEffectColor();

						bChangeColor = TRUE;
					}
				}
				//--------------------------------------------------------
				// 전체 색깔 바뀌는 effect
				//--------------------------------------------------------
				else if (bShowColor)
				{
					numColors++;
					//if (m_AttachEffectColor == ATTACHEFFECTCOLOR_NULL || rand()%numColors==0)
					if (m_AttachEffectColor == m_ChangeColorSet || rand()%numColors==0)
					{
						m_AttachEffectColor = pEffect->GetEffectColor();

						bChangeColor = TRUE;
					}
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
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}

			// 제대로 된 경우
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 시간이 다 돼서 끝나는 경우
		//---------------------------------------------------------------------
		else
		{
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag제거
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
			}


			ADDON	part = pEffect->GetEffectColorPart();

			//------------------------------------------------------------
			// 특정한 부위만 색깔이 바뀌는 경우도 있다.
			//------------------------------------------------------------
			if (part < ADDON_MAX)
			{
				m_Addon[part].bEffectColor = FALSE;
			}
			//------------------------------------------------------------
			// 원래대로 모두 돌린다.
			//------------------------------------------------------------
			else
			{
				for (int p=0; p<ADDON_MAX; p++)
				{
					m_Addon[p].bEffectColor = FALSE;
				}
			}
		
			//---------------------------------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//---------------------------------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{				
				g_pEffectGeneratorTable->GenerateNext( pEffect );
			}

			// 빛나는 Effect면 시야를 사라지게 해야한다.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 모든 빛나는Effect가 사라졌으면 시야를 제거시킨다.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			
			// memory삭제, list삭제						
			delete pEffect;						// memory제거
			
			DEBUG_ADD("[DeleteAttachEffect] OK");
			
			// list에서 삭제하기 위해서.. 임시로 저장
			iEffectTemp = iEffect;

			iEffect++;
			m_listEffect.erase( iEffectTemp );	// list에서 제거
		}		
	}

	//---------------------------------------------------------------------
	// 몸에 붙는 effect
	//---------------------------------------------------------------------
	// 모든 Effect를 Update한다.
	//---------------------------------------------------------------------
	iEffect = m_listGroundEffect.begin();
	
	while (iEffect != m_listGroundEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif

		if (pEffect->Update())
		{
			// 최고 밝기를 가진 Effect의 밝기를 저장한다.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 위치 설정
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 위치가 바뀌었거나
			// 빛의 크기(시야)가 바뀐 경우
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 시야 바꾸기
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
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
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}

			// 제대로 된 경우
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 시간이 다 돼서 끝나는 경우
		//---------------------------------------------------------------------
		else
		{
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag제거
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
			}

			//---------------------------------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//---------------------------------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{				
				g_pEffectGeneratorTable->GenerateNext( pEffect );
			}

			// 빛나는 Effect면 시야를 사라지게 해야한다.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 모든 빛나는Effect가 사라졌으면 시야를 제거시킨다.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			

			// memory삭제, list삭제						
			delete pEffect;						// memory제거
			
			
			DEBUG_ADD("[DeleteAttachEffect] OK");
			

			// list에서 삭제하기 위해서.. 임시로 저장
			iEffectTemp = iEffect;

			iEffect++;
			m_listGroundEffect.erase( iEffectTemp );	// list에서 제거
		}		
	}

	//---------------------------------------------------------------------
	// 색깔 바뀌는 거 보여주는 시점이면..
	//---------------------------------------------------------------------
	if (bChangeColor)
	{
		//---------------------------------------------------------------------
		// 전체 색깔 바뀌는 Effect가 없는 경우
		//---------------------------------------------------------------------	
		if (m_AttachEffectColor == m_ChangeColorSet)//ATTACHEFFECTCOLOR_NULL)
		{
			for (int p=0; p<ADDON_MAX; p++)
			{
				if (bAddonColorPart[p])
				{
					// 그 부분만 바뀌는 색깔로..
					m_Addon[p].bEffectColor		= TRUE;
					m_Addon[p].EffectColorSet	= addonColor[p];
				}		
				else
				{
					// 원래 색깔로..
					m_Addon[p].bEffectColor		= FALSE;
				}
			}
		}
		//---------------------------------------------------------------------
		// 전체 색깔 바뀌는 Effect가 있는 경우
		//---------------------------------------------------------------------
		else
		{
			for (int p=0; p<ADDON_MAX; p++)
			{
				m_Addon[p].bEffectColor		= TRUE;

				if (bAddonColorPart[p])
				{
					// 그 부분만 바뀌는 색깔로..			
					m_Addon[p].EffectColorSet	= addonColor[p];
				}		
				else
				{
					// 전체 바뀌는 색깔로..				
					m_Addon[p].EffectColorSet	= m_AttachEffectColor;
				}
			}
		}
	}
	//---------------------------------------------------------------------
	// 색깔 바뀌는거 안 보여주는 시점...
	//---------------------------------------------------------------------
	else
	{
		for (int p=0; p<ADDON_MAX; p++)
		{
			m_Addon[p].bEffectColor		= FALSE;
		}
	}
}

const MCreatureWear::ADDON_INFO&	MCreatureWear::GetACAddonInfo(int Addon) const
{ 
	switch( Addon )
	{
	case AC_ADDON_BODY :
		return m_Addon[ ADDON_COAT ];
		
	case AC_ADDON_HELM :
		return m_Addon[ ADDON_HELM ];

	case AC_ADDON_LEFTHAND :
		return m_Addon[ ADDON_LEFTHAND ];

	case AC_ADDON_RIGHTHAND :
		return m_Addon[ ADDON_RIGHTHAND ];

	case AC_ADDON_MOTOR :
		return m_Addon[ ADDON_MOTOR ];
	}

	return m_ShoulderAddon;
}
