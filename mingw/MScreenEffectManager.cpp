//-----------------------------------------------------------------------------
// MScreenEffectManager.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MScreenEffectManager.h"
#include "MEffect.h"
#include "MScreenEffect.h"
#include "MEffectGeneratorTable.h"
#include "UIFunction.h"
#include "MInventory.h"

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void		
MScreenEffectManager::Update()
{
	EFFECT_LIST::iterator iEffect = m_listEffect.begin();

	EFFECT_LIST::iterator iTemp;
	
//	POINT point;

	MEffect* pEffect;
	int count = m_listEffect.size();

	for (int i=0; i<count; i++)	
	{
		pEffect = *iEffect;

/*		if (pEffect->GetEffectType()==MEffect::EFFECT_SCREEN)
		{
			MScreenEffect* pScreenEffect = (MScreenEffect*)pEffect;
			
			const int invenX = pEffect->GetX(), invenY = pEffect->GetY();

			POINT grid_plus = { 0, 0 };

			const MItem* pItem = g_pInventory->GetItem( invenX, invenY );
			
			//---------------------------------------------------------------
			// 아이템이 있으면 크기에 따라서 이펙트가 바뀐다.
			//---------------------------------------------------------------
			if (pItem!=NULL)
			{
				grid_plus.x = (pItem->GetGridWidth()-1)*30/2;	// 헉ㅋㅋㅋ 하드코딩
				grid_plus.y = (pItem->GetGridHeight()-1)*30/2;	// 헉ㅋㅋㅋ 하드코딩
			}
			//---------------------------------------
			// 현재 좌표에 맞게 설정
			//---------------------------------------
			point = UI_GetInventoryGridPosition( invenX, invenY );
			pScreenEffect->SetScreenPosition(point.x, point.y);			
		}
*/
		//---------------------------------------
		//
		// update --> 정상적으로 된 경우
		//
		//---------------------------------------
		if (pEffect->Update())
		{
			
			iEffect++;

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
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}			
		}
		//---------------------------------------
		//
		// 다 돼서 끝난 경우
		//
		//---------------------------------------
		else
		{
			iTemp = iEffect;
			iEffect++;
			
			//-----------------------------------------------
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//-----------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}
/*			else if(pEffect->GetEffectTarget() != NULL && pEffect->GetEffectTarget()->IsEnd())
			{
				//------------------------------------------------------------
				// pEffect의 정보를 참고해서 Generate한다.
				//------------------------------------------------------------
				MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();
				
				if (pEffectTarget->IsExistResult())
				{
					MActionResult* pResult = pEffectTarget->GetResult();
					
					pEffectTarget->SetResultNULL();
					
#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
					DEBUG_ADD("Execute Result0");
#endif
					
					// 결과 실행
					pResult->Execute();
					
					// 메모리 제거
					delete pResult;			
				}
				
			}
*/
			//-----------------------------------------------
			// 제거
			//-----------------------------------------------
			delete pEffect;

			// list에서 제거한다.
			m_listEffect.erase(iTemp);
		}
	
	}
}

