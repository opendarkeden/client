// VS_UI_Item.cpp

#include "client_PCH.h"
#include "VS_UI_item.h"
#include "VS_UI_filepath.h"

#ifndef _LIB
#include "MItemManager.h"
#include "MItemOptionTable.h"
#endif

#ifndef _LIB
MItemManager	g_item_manager;
int				g_item_list_size = 0;
#endif

//int ITEM_COUNT = 150;

//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------

//
// gpC_item
//
// C_VS_UI_ITEM::C_VS_UI_ITEM()에서 전역 object를
// 사용하므로 C_VS_UI_ITEM은 C_VS_UI::Init()에서 할당한다.
//
C_VS_UI_ITEM *	gpC_item;

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetSprite
//
// 
//-----------------------------------------------------------------------------
/*CSprite * C_VS_UI_ITEM::GetSprite(SPRITE_ID id) const
{
	assert(m_pC_spk_list);

	CSprite *p_sprite = m_pC_spk_list->GetSprite(m_ct.Index(id));

	return p_sprite;
}*/

/*-----------------------------------------------------------------------------
- Blt
- Item을 출력한다.
-----------------------------------------------------------------------------*/
void C_VS_UI_ITEM::Blt(POINT &point, SPRITE_ID id)
{
//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif
	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltIndexSprite(&point, &m_item_ispk[id]);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// Blt
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::Blt(int x, int y, SPRITE_ID id)
{
	POINT point = {x, y};

	Blt(point, id);
}

//-----------------------------------------------------------------------------
// BltOutlineSprite
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltOutline(int x, int y, int color, SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif
	//if (p_sprite == NULL)
	//{
	//	// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
	//	m_pC_axs_spk->BltOutline(x, y, color);
	//	return;
	//}
	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_item_ispk[id]);
	outline_o.Generate();

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteOutline(&outline_o,  color);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	outline_o.Clear();
}

//-----------------------------------------------------------------------------
// BltOutlineOnly
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltOutlineOnly(int x, int y, int color, SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif
	//if (p_sprite == NULL)
	//{
	//	// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
	//	m_pC_axs_spk->BltOutline(x, y, color);
	//	return;
	//}

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_item_ispk[id]);
	outline_o.Generate();

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteOutlineOnly(&outline_o,  color);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	outline_o.Clear();
}

//-----------------------------------------------------------------------------
// GetWidth
//
//-----------------------------------------------------------------------------
int C_VS_UI_ITEM::GetWidth(SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return 0;
//#endif
	if(id >= m_item_ispk.GetSize() || id < 0)
		return 0;
	return m_item_ispk[id].GetWidth();
}

//-----------------------------------------------------------------------------
// GetHeight
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_ITEM::GetHeight(SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return 0;
//#endif
	if(id >= m_item_ispk.GetSize() || id < 0)
		return 0;
	return m_item_ispk[id].GetHeight();
}

//-----------------------------------------------------------------------------
// GetRX
//
// 
//-----------------------------------------------------------------------------
//int C_VS_UI_ITEM::GetRX(SPRITE_ID id) const
//{
//	return m_pC_sprite_frame_array->GetRX(m_sf_ct.Index(id));
//}

//-----------------------------------------------------------------------------
// GetRY
//
// 
//-----------------------------------------------------------------------------
//int C_VS_UI_ITEM::GetRY(SPRITE_ID id) const
//{
//	return m_pC_sprite_frame_array->GetRY(m_sf_ct.Index(id));
//}

/*-----------------------------------------------------------------------------
- C_VS_UI_ITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_ITEM::C_VS_UI_ITEM()
{
	//*
#ifndef _LIB
	
	// Item option table load.
	ifstream file("data\\info\\itemoption.inf",ios::binary);
	g_pItemOptionTable->LoadFromFile( file );
	file.close();

	// 으아악 나중에 서버에 earring들가면 밑에 있는 -1 뺄것
	int i = 0;// MAX_ITEM_CLASS;
	for (i=0; i<MAX_ITEM_CLASS; i++)
		g_item_list_size += (*g_pItemTable)[i].GetSize();

//	int n = 0;
	extern int g_ui_item_max;
	for (i=0; i<MAX_ITEM_CLASS; i++)
	{
		srand(GetTickCount());

		int classSize = (*g_pItemTable)[i].GetSize();
		for (int t=0; t<classSize; t++)
		{
			MItem * p_item = MItem::NewItem((enum ITEM_CLASS)i);
			p_item->SetItemType( t );	
			
			while(1)
			{
				int option = (rand()%(g_pItemOptionTable->GetSize()-1))+1;
				if((*g_pItemOptionTable)[option].Name.GetLength() != 0)
				{
					if(rand()&0x1) {p_item->AddItemOption( option );}
					option = (rand()%(g_pItemOptionTable->GetSize()-1))+1;
					if(rand()&0x1) p_item->AddItemOption( option );
					break;
				}
			}

			p_item->SetID( g_ui_item_max++ );

			g_item_manager.AddItem( p_item );

			// make list to access easy
			//g_p_item_list = (MItem **)realloc(g_p_item_list, sizeof(MItem *)+(g_item_list_size+1));			
		}
	}
	
#endif
//*/
	//if (CDirectDraw::Is565())
		//m_pC_spk_list = new CSpritePackList565;
	//else
	//	m_pC_spk_list = new CSpritePackList555;

	//m_pC_sprite_frame_array = new C_SPRITE_FRAME_ARRAY;

	//Read_rbt(RBT_ITEM, m_pC_spk_list, &m_ct);	
	//Read_rbt(RBT_ITEM_FRAME, m_pC_sprite_frame_array, &m_sf_ct);

	//m_pC_axs_spk = new C_SPRITE_PACK(SPK_AXS);

	// item image loading
//	ifstream	item_ispk(ISPK_ITEM, ios::binary|ios::nocreate);
//	if (!item_ispk)
//		_Error(FILE_OPEN);
	m_item_ispk.LoadFromFileRunning(ISPK_ITEM);
//	item_ispk.close();

//	ITEM_COUNT = m_item_ispk.GetSize();
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_ITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_ITEM::~C_VS_UI_ITEM()
{
#ifndef _LIB

	//for (int i = 0; i < g_item_list_size; i++)
	//	DeleteNew(g_p_item_list[i]);

	//DeleteNewArray(g_p_item_list);
	
#endif
	m_item_ispk.Release();

	//DeleteNew(m_pC_sprite_frame_array);
	//DeleteNew(m_pC_spk_list);
	//DeleteNew(m_pC_axs_spk);
}

//-----------------------------------------------------------------------------
// BltColor
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltColor(int x, int y, SPRITE_ID id, int rgb)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	POINT point;
	point.x = x;
	point.y = y;

//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltIndexSpriteColor(&point, &m_item_ispk[id], rgb);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltDarkness
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltDarkness(int x, int y, SPRITE_ID id, int dark)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	POINT point;
	point.x = x;
	point.y = y;

//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltIndexSpriteDarkness(&point, &m_item_ispk[id], dark);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltColorSet
//
// ColorSet을 적용하여 출력
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltColorSet(int x, int y, SPRITE_ID id, int color_set)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	POINT point;
	point.x = x;
	point.y = y;

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltIndexSpriteColorSet(&point, &m_item_ispk[id], color_set);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}


/*-----------------------------------------------------------------------------
- BltLocked
- Item을 출력한다.
-----------------------------------------------------------------------------*/
void C_VS_UI_ITEM::BltLocked(POINT &point, SPRITE_ID id)
{
//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	gpC_base->m_p_DDSurface_back->BltIndexSprite(&point, &m_item_ispk[id]);
}

//-----------------------------------------------------------------------------
// BltLocked
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLocked(int x, int y, SPRITE_ID id)
{
	POINT point = {x, y};

	BltLocked(point, id);
}

//-----------------------------------------------------------------------------
// BltLockedOutlineSprite
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLockedOutline(int x, int y, int color, SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif
	//if (p_sprite == NULL)
	//{
	//	// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
	//	m_pC_axs_spk->BltLockedOutline(x, y, color);
	//	return;
	//}

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_item_ispk[id]);
	outline_o.Generate();

	gpC_base->m_p_DDSurface_back->BltSpriteOutline(&outline_o,  color);

	outline_o.Clear();
}

//-----------------------------------------------------------------------------
// BltLockedOutlineOnly
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLockedOutlineOnly(int x, int y, int color, SPRITE_ID id)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif
	//if (p_sprite == NULL)
	//{
	//	// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
	//	m_pC_axs_spk->BltOutline(x, y, color);
	//	return;
	//}

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_item_ispk[id]);
	outline_o.Generate();

	gpC_base->m_p_DDSurface_back->BltSpriteOutlineOnly(&outline_o,  color);

	outline_o.Clear();
}

//-----------------------------------------------------------------------------
// BltLockedColor
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLockedColor(int x, int y, SPRITE_ID id, int rgb)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	POINT point;
	point.x = x;
	point.y = y;

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}

	gpC_base->m_p_DDSurface_back->BltIndexSpriteColor(&point, &m_item_ispk[id], rgb);
}

//-----------------------------------------------------------------------------
// BltLockedDarkness
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLockedDarkness(int x, int y, SPRITE_ID id, int dark)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	POINT point;
	point.x = x;
	point.y = y;

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

//	if (p_sprite == NULL)
//	{
//		// Item이 "없음"을 표시하기 위해서 이 Sprite를 출력한다.
//		m_pC_axs_spk->Blt(point);
//		return;
//	}

	gpC_base->m_p_DDSurface_back->BltIndexSpriteDarkness(&point, &m_item_ispk[id], dark);
}

//-----------------------------------------------------------------------------
// BltColorSet
//
// ColorSet을 적용하여 출력
//-----------------------------------------------------------------------------
void C_VS_UI_ITEM::BltLockedColorSet(int x, int y, SPRITE_ID id, int color_set)
{
//#ifndef _LIB
//	if (id >= ITEM_COUNT)
//		return;
//#endif

	if(id >= m_item_ispk.GetSize() || id < 0)
		return;

	POINT point;
	point.x = x;
	point.y = y;

	gpC_base->m_p_DDSurface_back->BltIndexSpriteColorSet(&point, &m_item_ispk[id], color_set);
}

