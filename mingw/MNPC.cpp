//----------------------------------------------------------------------
// MNPC.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MNPC.h"
#include "MNPCTable.h"
#include "MShopTemplateTable.h"
#include "MShopShelf.h"
#include "MPlayer.h"

//----------------------------------------------------------------------
// 
// constructor / destructor
//
//----------------------------------------------------------------------
MNPC::MNPC()
{
	m_NPCID = 0;

	m_pShop = NULL;
}

MNPC::~MNPC()
{
	if (m_pShop!=NULL)
	{
		delete m_pShop;
	}
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Get NPC Info
//----------------------------------------------------------------------
NPC_INFO*			
MNPC::GetNPCInfo(TYPE_OBJECTID id) const
{
	return (*g_pNPCTable).GetData( id ); 
}

//----------------------------------------------------------------------
// Set Shop
//----------------------------------------------------------------------
void				
MNPC::SetShop(MShop* pShop)
{
	// 기존에 있던걸 지운다.
	if (m_pShop!=NULL)
	{
		delete m_pShop;
	}

	m_pShop = pShop;
}

//-----------------------------------------------------------------------------
// CreateItemFromShopTemplate
//-----------------------------------------------------------------------------
BOOL
MNPC::CreateFixedShelf(bool bMysterious)
{
	NPC_INFO* pInfo = (*g_pNPCTable).GetData( m_NPCID );

	//-----------------------------------------------------------
	// NPC의 상점을 얻는다.
	//-----------------------------------------------------------
	MShop* pShop = m_pShop;

	if (pShop==NULL)
	{
		// 상점이 없으면 상점을 만든다.
		pShop = new MShop;
		pShop->Init( MShopShelf::MAX_SHELF );

		// NPC에 상점 설정..
		m_pShop = pShop;
	}

	MShopShelf::SHELF_TYPE shelfType;

	if (bMysterious)
	{
		shelfType = MShopShelf::SHELF_UNKNOWN;
	}
	else
	{
		shelfType = MShopShelf::SHELF_FIXED;
	}

	//-----------------------------------------------------------
	// NPC정보가 있는가?
	//-----------------------------------------------------------
	if (pInfo==NULL)
	{
		// 없으면... 그냥 빈 상점 생성
		MShopShelf* pShopShelf = MShopShelf::NewShelf( shelfType );

		pShop->SetShelf( shelfType, pShopShelf );	

		return false;
	}

	//-----------------------------------------------------------
	// Normal Item 선반을 얻는다.
	//-----------------------------------------------------------
	MShopShelf* pShopShelf = pShop->GetShelf( shelfType );

	//-----------------------------------------------------------
	// 없으면 생성한다.
	//-----------------------------------------------------------
	if (pShopShelf==NULL)
	{
		// normal shelf를 생성한다.
		pShopShelf = MShopShelf::NewShelf( shelfType );

		pShop->SetShelf( shelfType, pShopShelf );	
	}
	else
	{
		pShopShelf->Release();
	}

	bool bEnable = false;

	//-----------------------------------------------------------
	// NPC가 가진 ShopTemplate ID들을 얻어서 처리한다.
	//-----------------------------------------------------------
	//
	// Item정보들을 이용해서 pShopShelf에 Item을 추가하면 된다.
	//
	//-----------------------------------------------------------
	NPC_INFO::SHOPTEMPLATEID_LIST::iterator iID = pInfo->ListShopTemplateID.begin();

	while (iID != pInfo->ListShopTemplateID.end())
	{
		unsigned int id = *iID;

		//-----------------------------------------------------------
		// id의 ShopTemplate을 찾는다.
		//-----------------------------------------------------------
		MShopTemplate* pShopTemplate = (*g_pShopTemplateTable).GetData( id );

		if (pShopTemplate!=NULL)
		{
			//-----------------------------------------------------------
			// Normal Item인 경우만 처리한다.
			//-----------------------------------------------------------
			if ((MShopShelf::SHELF_TYPE)pShopTemplate->Type == shelfType)
			{	
				int minType	= pShopTemplate->MinType;	
				int	maxType	= pShopTemplate->MaxType;

				//-----------------------------------------------------------
				// min~max Type의 item들을 생성한다. (min, max포함)
				//-----------------------------------------------------------
				for (int type=minType; type<=maxType; type++)
				{
					// item 생성
					ITEM_CLASS itemClass = (ITEM_CLASS)pShopTemplate->Class;
					MItem* pItem = MItem::NewItem( itemClass );
					bEnable = true;

					if(bMysterious && g_pPlayer->IsFemale() && 
						(
							itemClass == ITEM_CLASS_COAT ||
							itemClass == ITEM_CLASS_TROUSER ||
							itemClass == ITEM_CLASS_VAMPIRE_COAT
						)
					)
						type++;

					pItem->SetItemType( type );
					pItem->SetGrade( 4 );
					if(bMysterious)
						pItem->UnSetIdentified();

					// durability max
					pItem->SetCurrentDurability( pItem->GetMaxDurability() );

					//-----------------------------------------------------------
					// Charge된 item인 경우 --> 꽉 채운다.
					//-----------------------------------------------------------
					if (pItem->IsChargeItem())
					{
						pItem->SetNumber( pItem->GetMaxNumber() );
					}

					//-----------------------------------------------------------
					// vampire portal인 경우 default값 설정
					//-----------------------------------------------------------
					if (pItem->GetItemClass()==ITEM_CLASS_VAMPIRE_PORTAL_ITEM)
					{
						switch(type)
						{
						case 3:
						case 4:
						case 5:
							{
								MVampirePortalItem* pPortalItem = (MVampirePortalItem*)pItem;
								
								// 바토리 마을
								pPortalItem->SetZone( 1003, 50, 70 );
								
							}
							break;

						case 6:
						case 7:
						case 8:
							{
								MVampirePortalItem* pPortalItem = (MVampirePortalItem*)pItem;
								
								// 테페즈 마을
								pPortalItem->SetZone( 1007, 62, 65 );
								
							}
							break;
							
						case 9:
						case 10:
						case 11:
							{
								MVampirePortalItem* pPortalItem = (MVampirePortalItem*)pItem;
								
								// 페로나 마을
								pPortalItem->SetZone( 61, 102, 220 );
								
							}
							break;
						}
					}


					// pItem->SetItemOption( 0 ); // default로 0이므로 할 필요 없다.
	
					// 무시  - -;;				
					// item option
					//pShopTemplate->MinOption;
					//pShopTemplate->MaxOption;

					//------------------------------------------------------
					// 생성된 item을 shelf에 추가한다.
					//------------------------------------------------------
					// 순서대로 추가시키면 된다.
					//------------------------------------------------------
					pShopShelf->AddItem( pItem );
				}				
			}
		}


		iID++;
	}

//	if(bMysterious)
	{
		if(bEnable)
			pShopShelf->SetEnable();
		else
			pShopShelf->SetDisable();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Action
//-----------------------------------------------------------------------------
void
MNPC::Action()
{
	MCreature::Action();

	// 바토리인 경우
	if (m_CreatureType==217
		// 테페즈인 경우
		|| m_CreatureType==366)
	{
		//--------------------------------------------------------
		// 방향을 바꿀 필요가 없던 경우에..
		// 심심할때마다 한번씩 방향 바꿔주기.. - -;
		//--------------------------------------------------------			
		if (//Player가 아니고
			m_CreatureType >= 4
			// 살아 있고..
			&& m_bAlive
			// 정지상태이고
			&& m_Action==ACTION_STAND
			// 움직일곳이 없고
			&& m_listMoveBuffer.size()==0
			// 정지동작의 끝에..
			&& m_ActionCount>=m_ActionCountMax-1
			// random하게.. - -;
			&& (rand() % 5)==0)
		{
			// 랜덤하게 player를 바라본다.
			SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );
		}
	}
	if(m_CreatureType >= 636 && m_CreatureType <= 639 )
	{
		if(m_ActionCount == 0 && !m_listEffect.empty())
		{
			ATTACHEFFECT_LIST::iterator itr = m_listEffect.begin();
			ATTACHEFFECT_LIST::iterator endItr = m_listEffect.end();
			
			while(itr != endItr )
			{
				MAttachEffect *pEffect =  *itr;
				
				pEffect->SetFrameID( pEffect->GetFrameID(), pEffect->GetMaxFrame() );
				itr++;
			}
		}
	}	
}