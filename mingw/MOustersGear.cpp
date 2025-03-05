//----------------------------------------------------------------------
// MOustersGear.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MItem.h"
#include "MOustersGear.h"
#include "MQuickSlot.h"

#ifdef __GAME_CLIENT__
	#include "UIFunction.h"
	#include "ClientFunction.h"
	#include "MPlayer.h"
#endif
#include "MTimeItemManager.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MOustersGear*		g_pOustersGear = NULL;

//----------------------------------------------------------------------
// static
//----------------------------------------------------------------------
MOustersGear::FUNCTION_GEARSLOT_CHECK
MOustersGear::s_GearSlotCheckTable[MAX_GEAR_OUSTERS] =
{
	IsGearSlotCirclet,			// 서클릿
	IsGearSlotCoat,				// 옷
	IsGearSlotLeftHand,			// 왼손
	IsGearSlotRightHand,			// 오른손
	IsGearSlotBoots,				// 신발
	IsGearSlotArmsBand,			// 암스밴드1
	IsGearSlotArmsBand,			// 암스밴드2
	IsGearSlotRing,				// 링1
	IsGearSlotRing,				// 링2
	IsGearSlotPendent,			// 목걸이1
	IsGearSlotPendent,			// 목걸이2
	IsGearSlotPendent,			// 목걸이3
	IsGearSlotStone,			// 정령석1
	IsGearSlotStone,			// 정령석2
	IsGearSlotStone,			// 정령석3
	IsGearSlotStone,			// 정령석4
	IsGearSlotCoreZap,			// 코어잽1
	IsGearSlotCoreZap,			// 코어잽2
	IsGearSlotCoreZap,			// 코어잽3
	IsGearSlotCoreZap,			// 코어잽4
	IsGearSlotFascia,			// 허리장식
	IsGearSlotMitten,			// 장갑
	IsGearSlotBloodBible,		// 블러드 바이블 1
	IsGearSlotBloodBible,		// 블러드 바이블 2
	IsGearSlotBloodBible,		// 블러드 바이블 3
	IsGearSlotBloodBible,		// 블러드 바이블 4
	IsGearSlotBloodBible,		// 블러드 바이블 5
	IsGearSlotBloodBible,		// 블러드 바이블 6

};


//----------------------------------------------------------------------
// 특정 부위에 pItem을 장착할 수 있는지를 check한다.
//----------------------------------------------------------------------
bool		MOustersGear::IsGearSlotCirclet(MItem* pItem)		{ return pItem->IsGearSlotOustersCirclet(); }
bool		MOustersGear::IsGearSlotCoat(MItem* pItem)			{ return pItem->IsGearSlotOustersCoat(); }
bool		MOustersGear::IsGearSlotLeftHand(MItem* pItem)		{ return pItem->IsGearSlotOustersLeftHand(); }
bool		MOustersGear::IsGearSlotRightHand(MItem* pItem)		{ return pItem->IsGearSlotOustersRightHand(); }
bool		MOustersGear::IsGearSlotBoots(MItem* pItem)			{ return pItem->IsGearSlotOustersBoots(); }
bool		MOustersGear::IsGearSlotArmsBand(MItem* pItem)		{ return pItem->IsGearSlotOustersArmsBand(); }
bool		MOustersGear::IsGearSlotRing(MItem* pItem)			{ return pItem->IsGearSlotOustersRing(); }
bool		MOustersGear::IsGearSlotPendent(MItem* pItem)		{ return pItem->IsGearSlotOustersPendent(); }
bool		MOustersGear::IsGearSlotStone(MItem* pItem)			{ return pItem->IsGearSlotOustersStone(); }
bool		MOustersGear::IsGearSlotCoreZap(MItem* pItem)		{ return pItem->IsGearSlotOustersCoreZap(); }	// 코어잽
bool		MOustersGear::IsGearSlotFascia(MItem* pItem)		{ return pItem->IsGearSlotOustersFascia(); }	// 코어잽
bool		MOustersGear::IsGearSlotMitten(MItem* pItem)		{ return pItem->IsGearSlotOustersMitten(); }	// 코어잽
bool		MOustersGear::IsGearSlotBloodBible(MItem* pItem)	{ return pItem->IsGearSlotOustersBloodBible(); }	// 블러드 바이블

//----------------------------------------------------------------------
//
// constructor
// 
//----------------------------------------------------------------------
MOustersGear::MOustersGear()
{
	g_pArmsBand1 = NULL;
	g_pArmsBand2 = NULL;
	m_bBloodBibleOpenCount = 0;
}

MOustersGear::~MOustersGear()
{
	g_pArmsBand1 = NULL;
	g_pArmsBand2 = NULL;
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init 
//----------------------------------------------------------------------
// MAX_GEAR_OUSTERS만큼의 Slot을 생성한다.
//----------------------------------------------------------------------
void
MOustersGear::Init()
{
	// slot 생성
	MPlayerGear::Init(MAX_GEAR_OUSTERS);
	m_Gilles_CoreZap = 4;
	m_bBloodBibleOpenCount = 0;
}

//----------------------------------------------------------------------
// Check AffectStatus
//----------------------------------------------------------------------
// 특정 아이템의 착용 가능 여부를 체크한다.
//----------------------------------------------------------------------
void			
MOustersGear::CheckAffectStatus(MItem* pItem)
{
	#ifdef __GAME_CLIENT__

		g_pPlayer->CheckAffectStatus( pItem );

	#endif
}

//----------------------------------------------------------------------
// Add Item ( pItem )
//----------------------------------------------------------------------
// pItem을 적절한(?) slot에 추가시킨다.
//----------------------------------------------------------------------
bool			
MOustersGear::AddItem(MItem* pItem)
{
	for (int i=0; i<MAX_GEAR_OUSTERS; i++)
	{
		// 추가했으면..
		if (AddItem( pItem, (enum GEAR_OUSTERS)i ))
		{
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Can Replace Item ( pItem, n )
//----------------------------------------------------------------------
// slot(n)에 pItem을 추가한다.
// 추가할 수 있는 slot인지 확인해야 한다.
//
// pOldItem에는 교체될 것이 있는 경우에 그게 들어간다.
//----------------------------------------------------------------------
bool			
MOustersGear::CanReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem)
{	
	// 수치 적용되지 않는 아이템은 착용되지 않는다
	if(pItem->IsAffectStatus() == false&& !pItem->IsQuestItem())
		return false;

	pOldItem = NULL;

	//---------------------------------------------------------
	// ItemSlot 범위를 넘어가는 경우..
	//---------------------------------------------------------	
	if (n>=m_Size || !pItem->IsOustersItem())
	{
		return false;
	}
	if(n>=GEAR_OUSTERS_BLOOD_BIBLE1 && n<=GEAR_OUSTERS_BLOOD_BIBLE6) // 블러드 바이블은 집을수 없다.
		return false;

	
		
	// bycsm 2004.12.31

		if(n == GEAR_OUSTERS_FASCIA)
	{
		if(pItem->IsGearSlotOustersFascia() == true)
		{
			if (m_ItemSlot[GEAR_OUSTERS_FASCIA]==NULL)
				return true;
			else
			{
				pOldItem = m_ItemSlot[GEAR_OUSTERS_FASCIA];
				return true;
			}
		}

	}

	if(n == GEAR_OUSTERS_MITTEN)
	{
		if(pItem->IsGearSlotOustersMitten()== true)
		{
			
			if (m_ItemSlot[GEAR_OUSTERS_MITTEN]==NULL)
				return true;
			else
			{
				pOldItem = m_ItemSlot[GEAR_OUSTERS_MITTEN];
				return true;
			}
		}
		
	}


	
	//-------------------------------------------------
	//
	// 손에 장착할려는 경우
	//
	//-------------------------------------------------
	if (n==GEAR_OUSTERS_LEFTHAND || n==GEAR_OUSTERS_RIGHTHAND)	
	{
		//-------------------------------------------------
		// [ 양손 Item ] 인 경우
		//-------------------------------------------------
		if (pItem->IsGearSlotTwoHand())
		{			
			//-------------------------------------------------
			// 양손의 Slot이 모두 비어있는 경우 
			//-------------------------------------------------			
			if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]==NULL
					&& m_ItemSlot[GEAR_OUSTERS_LEFTHAND]==NULL)
			{
				return true;
			}
			
			//-------------------------------------------------
			// 양손에 다 뭔가 있는 경우
			//-------------------------------------------------
			if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]!=NULL 
				&& m_ItemSlot[GEAR_OUSTERS_LEFTHAND]!=NULL)
			{
				//-------------------------------------------------
				// 양손에 있는게 같다면..
				//-------------------------------------------------
				if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]==m_ItemSlot[GEAR_OUSTERS_LEFTHAND])
				{
					pOldItem = m_ItemSlot[GEAR_OUSTERS_RIGHTHAND];
					return true;
				}
				
				return false;				
			}

			//-------------------------------------------------
			// 오른손에만 뭔가 있는 경우
			//-------------------------------------------------
			else if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]!=NULL)
			{
				pOldItem = m_ItemSlot[GEAR_OUSTERS_RIGHTHAND];
				return true;
			}

			//-------------------------------------------------
			// 왼손에만뭔가 있는 경우
			//-------------------------------------------------
			else if (m_ItemSlot[GEAR_OUSTERS_LEFTHAND]!=NULL)
			{			
				pOldItem = m_ItemSlot[GEAR_OUSTERS_LEFTHAND];
				return true;
			}

			// 이런 경우가 있을 수 있을까? - -;;
			return false;
		}

		//-------------------------------------------------
		// [ 오른손 Item ] 인 경우
		//-------------------------------------------------				
		else if (pItem->IsGearSlotRightHand())
		{
			// NULL이거나 말거나 그냥 넣어주면 된다.
			pOldItem = m_ItemSlot[GEAR_OUSTERS_RIGHTHAND];
			return true;
		}
		
		//-------------------------------------------------				
		// [ 왼손 Item ] 인 경우
		//-------------------------------------------------
		else if (pItem->IsGearSlotLeftHand())
		{
			pOldItem = m_ItemSlot[GEAR_OUSTERS_LEFTHAND];
			return true;
		}

		// 손에 장착하는게 아닌 경우
		return false;
	}

	/***********************************************************	
				코어잽 관련 처리
	***********************************************************/
	if (n >= GEAR_OUSTERS_STONE1 && n <= GEAR_OUSTERS_STONE4)	
	{
		if (pItem->IsGearSlotOustersStone())
		{			
			if (m_ItemSlot[n+m_Gilles_CoreZap]==NULL) // 코어잽이 없어야 정령석을 놓을 수 있음
			{
				pOldItem = m_ItemSlot[n];
				return true;
			}
		}
		else
		if (pItem->IsGearSlotOustersCoreZap())
		{			
			if (m_ItemSlot[n]!=NULL) // 코어잽은 링이 있어야 놓을 수 있다. 
			{
				if (m_ItemSlot[n]!=NULL) // 코어잽은 링이 있어야 놓을 수 있다. 
				{
					if(m_ItemSlot[n+m_Gilles_CoreZap]!=NULL)
						pOldItem = m_ItemSlot[n+m_Gilles_CoreZap];
					else
						pOldItem = NULL;
					return true;
				}
			}
		}
		return false;
	}
	else if(n >= GEAR_OUSTERS_ZAP1 && n <= GEAR_OUSTERS_ZAP4) // 코어잽 슬롯은 보이지 않는다. -_-?? 
		return false;
	//----------------------------------------------------
	// 적절한 Slot인지 확인해야 한다.
	//----------------------------------------------------	
	if (pItem->IsGearItem()	&&				// pItem이 GearItem인지 확인..
		(*s_GearSlotCheckTable[n])(pItem))	// 적절한 Slot인지 확인
	{
		// n번째 slot에 pItem을 추가한다.
		pOldItem = m_ItemSlot[n];
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Add Item ( pItem, n )
//----------------------------------------------------------------------
// slot(n)에 pItem을 추가한다.
// 추가할 수 있는 slot인지 확인해야 한다.
//----------------------------------------------------------------------
bool			
MOustersGear::AddItem(MItem* pItem, GEAR_OUSTERS n)
{	
	//---------------------------------------------------------
	// ItemSlot 범위를 넘어가는 경우..
	//---------------------------------------------------------	
	if (n>=m_Size || !pItem->IsOustersItem())
	{
		return false;
	}

	//-------------------------------------------------
	// 양손 Item인 경우는 따로 체크해준다.
	//-------------------------------------------------
	if ((n==GEAR_OUSTERS_LEFTHAND || n==GEAR_OUSTERS_RIGHTHAND) &&
		pItem->IsGearSlotTwoHand())
	{
		// 양손의 Slot이 모두 비어있는 상태에만 추가 가능하다.
		if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]==NULL
				&& m_ItemSlot[GEAR_OUSTERS_LEFTHAND]==NULL)
		{
			// 오른손에 추가한다.			
			if (MPlayerGear::AddItem( pItem, GEAR_OUSTERS_RIGHTHAND ))
			{
				// 왼손에도 추가해준다.
				m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = pItem;

				//-------------------------------------------------
				// 제대로 추가된 경우 --> sound출력
				//-------------------------------------------------
				#ifdef __GAME_CLIENT__
					PlaySound( pItem->GetGearSoundID() );
					g_pPlayer->CalculateStatus();	
				#endif

				return true;
			}

			return false;
		}

		return false;
	}
	/***********************************************************	
				코어잽 관련 처리
	***********************************************************/
	if (n >= GEAR_OUSTERS_STONE1 && n <= GEAR_OUSTERS_STONE4)	
	{
		if (pItem->IsGearSlotOustersStone())
		{			
			if (m_ItemSlot[n+m_Gilles_CoreZap]==NULL&&m_ItemSlot[n]==NULL) // 정령석도 없고 코어잽이 없어야 링을 놓을 수 있음
			{
				if (MPlayerGear::AddItem( pItem, n ))	
				{
					//-------------------------------------------------
					// 제대로 추가된 경우 --> sound출력
					//-------------------------------------------------
					#ifdef __GAME_CLIENT__
						PlaySound( pItem->GetGearSoundID() );
						g_pPlayer->CalculateStatus();	
					#endif

					return true;
				}
			}
		}
		else 
		if (pItem->IsGearSlotOustersCoreZap())
		{
			if (m_ItemSlot[n-m_Gilles_CoreZap]!=NULL && m_ItemSlot[n]==NULL) // 해당위치에 링이 있고 코어잽이 없을 경우만 코어잽 추가
			{		
				if (MPlayerGear::AddItem( pItem, n + m_Gilles_CoreZap ))// 코어잽 위치에 추가
				{
					//-------------------------------------------------
					// 제대로 추가된 경우 --> sound출력
					//-------------------------------------------------
					#ifdef __GAME_CLIENT__
						PlaySound( pItem->GetGearSoundID() );
						g_pPlayer->CalculateStatus();	
					#endif

					return true;
				}

			}
		}

		return false;
	}
//	else if(n >= GEAR_OUSTERS_ZAP1 && n <= GEAR_OUSTERS_ZAP4) // 코어잽 슬롯은 보이지 않는다. -_-?? 
//		return false;
	//----------------------------------------------------
	// 적절한 Slot인지 확인해야 한다.
	//----------------------------------------------------	
	if (m_ItemSlot[n]==NULL &&				// n번째 slot이 비어있는지 확인
		pItem->IsGearItem()	&&				// pItem이 GearItem인지 확인..
		(*s_GearSlotCheckTable[n])(pItem))	// 적절한 Slot인지 확인
	{
		// n번째 slot에 pItem을 추가한다.
		bool bAdd = MPlayerGear::AddItem( pItem, n );

		//-------------------------------------------------
		// 제대로 추가된 경우 --> sound출력
		//-------------------------------------------------
		if (bAdd)
		{
			#ifdef __GAME_CLIENT__		
				PlaySound( pItem->GetGearSoundID() );
				g_pPlayer->CalculateStatus();
			#endif

			if( n == GEAR_OUSTERS_ARMSBAND1	)
			{
				g_pArmsBand1 = (MOustersArmsBand*)pItem;
				#ifdef __GAME_CLIENT__		
				UI_ResetQuickItemSlot();
				#endif
			}
			else if ( n == GEAR_OUSTERS_ARMSBAND2 )
			{
				g_pArmsBand2 = (MOustersArmsBand*)pItem;
				#ifdef __GAME_CLIENT__		
				UI_ResetQuickItemSlot();
				#endif
			}
			
			#ifdef __GAME_CLIENT__		
			PlaySound( pItem->GetGearSoundID() );
			g_pPlayer->CalculateStatus();	
			#endif

			return true;
		}		

		return false;
	}

	return false;
}

//----------------------------------------------------------------------
// Remove Item ( n)
//----------------------------------------------------------------------
MItem*			
MOustersGear::RemoveItem(GEAR_OUSTERS n)
{ 
	MItem* pItem = m_ItemSlot[n];

	//-----------------------------------------------------
	// 없는 경우
	//-----------------------------------------------------
	if (pItem==NULL)
	{
		return NULL;
	}

	//-----------------------------------------------------
	// 양손 무기이면 오른손으로 설정한다.
	//-----------------------------------------------------
	if (pItem->IsGearSlotTwoHand())
	{
		n = GEAR_OUSTERS_RIGHTHAND;
	}
	/***********************************************************	
				코어잽 관련 처리
	***********************************************************/
	if(n>=GEAR_OUSTERS_STONE1 && n<= GEAR_OUSTERS_STONE4) 
	{
		if(NULL != m_ItemSlot[n+m_Gilles_CoreZap]) // 코어잽이 붙어 있으면 코어잽을 뗀다
			n = (GEAR_OUSTERS)(n + m_Gilles_CoreZap); // 코어잽 위치
	}
//	else if(n >= GEAR_OUSTERS_ZAP1 && n <= GEAR_OUSTERS_ZAP4) // 코어잽 슬롯은 보이지 않는다. -_-?? 
//		return false;

	pItem = MPlayerGear::RemoveItem( (BYTE)n );

	//-----------------------------------------------------
	// 없는 경우
	//-----------------------------------------------------
	if (pItem==NULL)
	{
		return NULL;
	}

	if( n == GEAR_OUSTERS_ARMSBAND1 || n == GEAR_OUSTERS_ARMSBAND2 )
	{
		if( n == GEAR_OUSTERS_ARMSBAND1 )
			g_pArmsBand1 = NULL;
		else
			g_pArmsBand2 = NULL;
		
	}

	//-----------------------------------------------------
	// 양손 Item인 경우 양손의 Slot을 모두 제거해줘야 한다.
	//-----------------------------------------------------
	if (pItem->IsGearSlotTwoHand())
	{
		m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = NULL;
		m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = NULL;
	}
	
	#ifdef __GAME_CLIENT__
		g_pPlayer->CalculateStatus();
	#endif

	// 있으면...
	return pItem;
}

//----------------------------------------------------------------------
// RemoveItem : id가 같은 item을 제거한다.
//----------------------------------------------------------------------
MItem*			
MOustersGear::RemoveItem(TYPE_OBJECTID id)
{ 
	MItem* pItem = MPlayerGear::RemoveItem( id ); 

	//-----------------------------------------------------
	// 없는 경우
	//-----------------------------------------------------
	if (pItem==NULL)
	{
		return NULL;
	}

	//-----------------------------------------------------
	// 양손 Item인 경우 양손의 Slot을 모두 제거해줘야 한다.
	//-----------------------------------------------------
	if (pItem->IsGearSlotTwoHand())
	{
		m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = NULL;
		m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = NULL;
	}
	if( pItem->GetItemClass() == ITEM_CLASS_OUSTERS_ARMSBAND )
	{
		if( pItem->GetItemSlot()== GEAR_OUSTERS_ARMSBAND1 )
			g_pArmsBand1 = NULL;
		else
			g_pArmsBand2 = NULL;		
	}


	#ifdef __GAME_CLIENT__
		g_pPlayer->CalculateStatus();
	#endif

	// 있~으면...
	return pItem;
}

//----------------------------------------------------------------------
// ReplaceItem : pItem을 추가하고 딴게 있다면 Item교환
//----------------------------------------------------------------------
bool			
MOustersGear::ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem)
{
	//---------------------------------------------------------
	// ItemSlot 범위를 넘어가는 경우..
	//---------------------------------------------------------	

	if (n>=m_Size || !pItem->IsOustersItem())
	{
		return false;
	}


	if(n == GEAR_OUSTERS_FASCIA)
	{
		if(pItem->IsGearSlotOustersFascia() == true)
		{
			if (m_ItemSlot[GEAR_OUSTERS_FASCIA]==NULL)
			{
				MPlayerGear::ReplaceItem( pItem, GEAR_OUSTERS_FASCIA, pOldItem );
				return true;
			}
			else
				return false;
		}
	}
	
	if(n == GEAR_OUSTERS_MITTEN)
	{
		if(pItem->IsGearSlotOustersMitten() == true)
		{
			if (m_ItemSlot[GEAR_OUSTERS_MITTEN]==NULL)
			{
				MPlayerGear::ReplaceItem( pItem, GEAR_OUSTERS_MITTEN, pOldItem );
				return true;
			}
			else
				return false;
		}
	}
	//-------------------------------------------------
	//
	// 손에 장착할려는 경우
	//
	//-------------------------------------------------
	if (n==GEAR_OUSTERS_LEFTHAND || n==GEAR_OUSTERS_RIGHTHAND)	
	{
		bool bAdd = false;
			
		//-------------------------------------------------
		// [ 양손 Item ] 인 경우
		//-------------------------------------------------
		if (pItem->IsGearSlotTwoHand())
		{			
			//-------------------------------------------------
			// 양손의 Slot이 모두 비어있는 경우
			//-------------------------------------------------
			// 그냥 추가한다.
			//-------------------------------------------------
			if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]==NULL
					&& m_ItemSlot[GEAR_OUSTERS_LEFTHAND]==NULL)
			{
				// 그냥 추가한다.
				pOldItem = NULL;
				if (MPlayerGear::AddItem( pItem, GEAR_OUSTERS_RIGHTHAND ))
				{
					//-------------------------------------------------
					// item의 내구성 상태 설정
					//-------------------------------------------------
					CheckItemStatus( pItem, GEAR_OUSTERS_RIGHTHAND );

					pItem->SetItemSlot( GEAR_OUSTERS_RIGHTHAND );
					m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = pItem;
					m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = pItem;

					//-------------------------------------------------
					// 제대로 추가된 경우 --> sound출력
					//-------------------------------------------------
					#ifdef __GAME_CLIENT__
						PlaySound( pItem->GetGearSoundID() );						
						g_pPlayer->CalculateStatus();			
					#endif

					return true;
				}

				return false;
			}
			
			//-------------------------------------------------
			// 양손에 다 뭔가 있는 경우
			//-------------------------------------------------
			if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]!=NULL 
				&& m_ItemSlot[GEAR_OUSTERS_LEFTHAND]!=NULL)
			{
				//-------------------------------------------------
				// 양손에 있는게 같다면..
				//-------------------------------------------------
				if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]==m_ItemSlot[GEAR_OUSTERS_LEFTHAND])
				{
					// 양손 무기와 교체하는 경우이다.
					if (MPlayerGear::ReplaceItem( pItem, 
													GEAR_OUSTERS_RIGHTHAND, 
													pOldItem ))
					{
						//-------------------------------------------------
						// item의 내구성 상태 설정
						//-------------------------------------------------
						CheckItemStatus( pItem, GEAR_OUSTERS_RIGHTHAND );

						//pItem->SetItemSlot( GEAR_OUSTERS_RIGHTHAND );
						m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = pItem;
						//m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = pItem;

						//-------------------------------------------------
						// 제대로 추가된 경우 --> sound출력
						//-------------------------------------------------
						#ifdef __GAME_CLIENT__
							PlaySound( pItem->GetGearSoundID() );
							g_pPlayer->CalculateStatus();	
						#endif	

						return true;
					}

					return false;
				}
				
				//-------------------------------------------------
				// 양손에 있는게 다르다면..
				//-------------------------------------------------
				pOldItem = NULL;
				return false;
				
			}

			//-------------------------------------------------
			// 오른손에 뭔가 있는 경우
			//-------------------------------------------------
			// 오른손의 Item과 Replace한다.
			//-------------------------------------------------
			else if (m_ItemSlot[GEAR_OUSTERS_RIGHTHAND]!=NULL)
			{
				bAdd = MPlayerGear::ReplaceItem( pItem, 
													GEAR_OUSTERS_RIGHTHAND, 
													pOldItem );
			}		

			//-------------------------------------------------
			// 왼손에 뭔가 있는 경우
			//-------------------------------------------------
			// 왼손의 Item과 Replace한다.
			//-------------------------------------------------
			else if (m_ItemSlot[GEAR_OUSTERS_LEFTHAND]!=NULL)
			{
				bAdd = MPlayerGear::ReplaceItem( pItem, 
													GEAR_OUSTERS_LEFTHAND, 
													pOldItem );					
			}

			//-----------------------------------------------------
			// 양손의 Slot을 모두 설정..
			//-----------------------------------------------------
			if (bAdd)
			{
				//-------------------------------------------------
				// item의 내구성 상태 설정
				//-------------------------------------------------
				CheckItemStatus( pItem, GEAR_OUSTERS_RIGHTHAND );
				
				pItem->SetItemSlot( GEAR_OUSTERS_RIGHTHAND );
				m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = pItem;
				m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = pItem;
		
				//-------------------------------------------------
				// 제대로 추가된 경우 --> sound출력
				//-------------------------------------------------
				#ifdef __GAME_CLIENT__
					PlaySound( pItem->GetGearSoundID() );
					g_pPlayer->CalculateStatus();
				#endif

				return true;
			}

			return false;
		}
		//-------------------------------------------------
		// [ 오른손 Item ] 인 경우
		//-------------------------------------------------		
		else if (pItem->IsGearSlotRightHand())
		{
			bAdd = MPlayerGear::ReplaceItem( pItem, 
													GEAR_OUSTERS_RIGHTHAND, 
													pOldItem );

			//-------------------------------------------------
			// 교환되어 나온 Item이 양손인 경우
			//-------------------------------------------------
			if (pOldItem!=NULL)
			{
				if (pOldItem->IsGearSlotTwoHand())
				{
					// 왼손을 비워둔다.
					m_ItemSlot[GEAR_OUSTERS_LEFTHAND] = NULL;
				}

			}

			if (bAdd)
			{
				//-------------------------------------------------
				// item의 내구성 상태 설정
				//-------------------------------------------------
				CheckItemStatus( pItem, GEAR_OUSTERS_RIGHTHAND );

				//-------------------------------------------------
				// 제대로 추가된 경우 --> sound출력
				//-------------------------------------------------
				#ifdef __GAME_CLIENT__
					PlaySound( pItem->GetGearSoundID() );
					g_pPlayer->CalculateStatus();
				#endif

				return true;
			}

			return false;
		}
		//-------------------------------------------------
		// [ 왼손 Item ] 인 경우
		//-------------------------------------------------
		else if (pItem->IsGearSlotLeftHand())
		{
			bAdd = MPlayerGear::ReplaceItem( pItem, 
													GEAR_OUSTERS_LEFTHAND, 
													pOldItem );
			//-------------------------------------------------
			// 교환되어 나온 Item이 양손인 경우
			//-------------------------------------------------
			if (pOldItem!=NULL)
			{
				if (pOldItem->IsGearSlotTwoHand())
				{
					// 오른손을 비워둔다.
					m_ItemSlot[GEAR_OUSTERS_RIGHTHAND] = NULL;
				}

			}

			if (bAdd)
			{
				//-------------------------------------------------
				// item 상태 설정
				//-------------------------------------------------
				CheckItemStatus( pItem, GEAR_OUSTERS_LEFTHAND );
				
				//-------------------------------------------------
				// 제대로 추가된 경우 --> sound출력
				//-------------------------------------------------
				#ifdef __GAME_CLIENT__
					PlaySound( pItem->GetGearSoundID() );
					g_pPlayer->CalculateStatus();
				#endif	

				return true;
			}

			return false;
		}
		
		//-------------------------------------------------
		// 손에 장착할려는데
		// 손에 장착할 수 있는 Item이 아닌 경우
		//-------------------------------------------------
		pOldItem = NULL;
		return false;
		
	}
	/***********************************************************	
				코어잽 관련 처리
	***********************************************************/
	if (n >= GEAR_OUSTERS_STONE1 && n <= GEAR_OUSTERS_STONE4)	
	{
		if (pItem->IsGearSlotOustersStone())
		{			
			if (m_ItemSlot[n+m_Gilles_CoreZap]!=NULL) // 코어잽이 있으면 못 놓음
				return false;
		}
		else
		if (pItem->IsGearSlotOustersCoreZap())
		{			
			if (m_ItemSlot[n]!=NULL) // 코어잽은 링이 있어야 놓을 수 있다. 
			{
				bool bAdd = false;
				if(m_ItemSlot[n+m_Gilles_CoreZap] == NULL)// 기존 코어잽이 없으면
				{
					pOldItem = NULL;
					bAdd = MPlayerGear::AddItem( pItem, n+m_Gilles_CoreZap );	// 걍 추가
				}
				else // 기존 코어잽이 있으면 교환
				{
					bAdd = MPlayerGear::ReplaceItem( pItem, n+m_Gilles_CoreZap, pOldItem );	
					
					if (bAdd)
					{
						CheckItemStatus( pItem, n+m_Gilles_CoreZap );
					}			
				}

				if (bAdd) // 잘 추가 됐으면 
				{
					#ifdef __GAME_CLIENT__
						PlaySound( pItem->GetGearSoundID() );
						g_pPlayer->CalculateStatus();
					#endif
					return true;
				}
				else 
					return false;
			}
			return false;
		}
	//	return false;
	}
//	else if(n >= GEAR_OUSTERS_ZAP1 && n <= GEAR_OUSTERS_ZAP4) // 코어잽 슬롯은 보이지 않는다. -_-?? 
//		return false;
	
	//----------------------------------------------------
	// 적절한 Slot인지 확인해야 한다.
	//----------------------------------------------------	
	if (//m_ItemSlot[n]==NULL &&				// n번째 slot이 비어있는지 확인
		pItem->IsGearItem()	&&				// pItem이 GearItem인지 확인..
		(*s_GearSlotCheckTable[n])(pItem))	// 적절한 Slot인지 확인
	{
		// n번째 slot에 pItem을 추가한다.
		if (MPlayerGear::ReplaceItem( pItem, n, pOldItem ))
		{
			//-------------------------------------------------
			// item 상태 설정
			//-------------------------------------------------
			CheckItemStatus( pItem, n );			

			//-------------------------------------------------
			// 제대로 추가된 경우 --> sound출력
			//-------------------------------------------------
			#ifdef __GAME_CLIENT__
				PlaySound( pItem->GetGearSoundID() );
			#endif

			#ifdef __GAME_CLIENT__
				g_pPlayer->CalculateStatus();
			#endif

			if( n == GEAR_OUSTERS_ARMSBAND1	)
			{
				g_pArmsBand1 = (MOustersArmsBand*)pItem;
				#ifdef __GAME_CLIENT__		
				UI_ResetQuickItemSlot();
				#endif
			}
			else if ( n == GEAR_OUSTERS_ARMSBAND2 )
			{
				g_pArmsBand2 = (MOustersArmsBand*)pItem;
				#ifdef __GAME_CLIENT__		
				UI_ResetQuickItemSlot();
				#endif
			}
			return true;
		}

		return false;
	}

	return false;
}

MItem*	MOustersGear::GetGearItem_PickUp(int &n) const	
{
	int Slot = n;
//	if(Slot>=GEAR_OUSTERS_BLOOD_BIBLE1 && Slot<=GEAR_OUSTERS_BLOOD_BIBLE6) // 블러드 바이블은 집을수 없다.
//		return NULL;
	if(Slot>=GEAR_OUSTERS_STONE1 && Slot<=GEAR_OUSTERS_STONE4)
	{
		if(m_ItemSlot[Slot+m_Gilles_CoreZap]!=NULL)
		{
			n = n+m_Gilles_CoreZap;
			return MSlotItemManager::GetItem( (BYTE)(Slot+m_Gilles_CoreZap) ); 
		}
	}
	
	return MSlotItemManager::GetItem( (BYTE)Slot ); 
}


MItem*	MOustersGear::GetGearCoreZapItem(int slot) const	
{
	if(slot>=GEAR_OUSTERS_STONE1 && slot<=GEAR_OUSTERS_STONE4)
	{
		if(m_ItemSlot[slot+m_Gilles_CoreZap]!=NULL)
		{
			if(m_ItemSlot[slot+m_Gilles_CoreZap]->GetItemClass() == ITEM_CLASS_CORE_ZAP)
				return MSlotItemManager::GetItem( (BYTE)(slot+m_Gilles_CoreZap) ); 
		}
	}
	
	return NULL;
}


MItem*	MOustersGear::GetGearCoreZapedItem(int slot) const	
{
	if(slot>=GEAR_OUSTERS_ZAP1 && slot<=GEAR_OUSTERS_ZAP4)
	{
		if(slot-m_Gilles_CoreZap<0)
			return NULL;
		if(m_ItemSlot[slot-m_Gilles_CoreZap]!=NULL)
		{
			if(m_ItemSlot[slot-m_Gilles_CoreZap]->GetItemClass() == ITEM_CLASS_OUSTERS_STONE)
				return MSlotItemManager::GetItem( (BYTE)(slot-m_Gilles_CoreZap) ); 
		}
	}
	
	return NULL;
}

const bool	MOustersGear::IsHasAllCoreZap(int CoreZapType) const	
{
	BYTE bType[4] = {0,};
	if(CoreZapType == 0) // 질드레 코어 잽 일 경우
	{
		MItem* pItem;
		for(int i = 0; i<4; i++)
		{
			pItem = m_ItemSlot[GEAR_OUSTERS_ZAP1+i];
			if(pItem != NULL)
			{
				if(pItem->GetItemType()>3)
					return false;
				if(bType[pItem->GetItemType()]!= 0)
					return false;
				bType[pItem->GetItemType()] = 1;
			}
			else
				return false;
		}
		if(bType[0]+bType[1]+bType[2]+bType[3] == 4)
			return true;
	}
	return false;
}

const bool	MOustersGear::IsCloseBloodBibleSlot(int slot) const	
{
	if(slot>=GEAR_OUSTERS_BLOOD_BIBLE1 && slot<=GEAR_OUSTERS_BLOOD_BIBLE6)
	{
		int TempSlot = slot - GEAR_OUSTERS_BLOOD_BIBLE1 + 1;
		if(TempSlot > m_bBloodBibleOpenCount)
			return true;
	}
	return false;
}
