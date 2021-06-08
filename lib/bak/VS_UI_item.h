/*-----------------------------------------------------------------------------

	VS_UI_item.h

	Item을 관리하는 객체.

	2000.2.29. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_ITEM_H__
#define __VS_UI_ITEM_H__

#include "VS_UI_Util.h"
#include "VS_UI_base.h"

#include "MItemTable.h"
#include "MItem.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MQuickSlot.h"
#include "MInventory.h"

/*-----------------------------------------------------------------------------
  Class VS UI Item
-----------------------------------------------------------------------------*/
class C_VS_UI_ITEM
{
private:
	CIndexSpritePack 			m_item_ispk;

	//C_SPRITE_PACK *			m_pC_axs_spk;
	//CSpritePackList *			m_pC_spk_list; // Item Sprite list
	//C_CONVERT_TABLE			m_ct;

	// Item frame
	//C_SPRITE_FRAME_ARRAY *	m_pC_sprite_frame_array;
	//C_CONVERT_TABLE			m_sf_ct;

public:
	C_VS_UI_ITEM();
	~C_VS_UI_ITEM();

	void	Blt(POINT &point, data_id_t id);
	void	Blt(int x, int y, data_id_t id);
	void	BltColor(int x, int y, data_id_t id, int rgb);
	void	BltDarkness(int x, int y, data_id_t id, int dark);
	void	BltOutline(int x, int y, int color, data_id_t id);
	void	BltOutlineOnly(int x, int y, int color, data_id_t id);
	void	BltColorSet(int x, int y, data_id_t id, int color_set);

	// by sigi
	void	BltLocked(POINT &point, data_id_t id);
	void	BltLocked(int x, int y, data_id_t id);
	void	BltLockedColor(int x, int y, data_id_t id, int rgb);
	void	BltLockedDarkness(int x, int y, data_id_t id, int dark);
	void	BltLockedOutline(int x, int y, int color, data_id_t id);
	void	BltLockedOutlineOnly(int x, int y, int color, data_id_t id);
	void	BltLockedColorSet(int x, int y, data_id_t id, int color_set);

	int	GetWidth(data_id_t id);
	int	GetHeight(data_id_t id);
	//CSprite * GetSprite(data_id_t id) const;
	//int	GetRX(data_id_t id) const;
	//int	GetRY(data_id_t id) const;
};

extern C_VS_UI_ITEM *	gpC_item;

#endif