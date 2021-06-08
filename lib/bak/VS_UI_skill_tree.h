/*-----------------------------------------------------------------------------

	VS_UI_skill_tree.h

	2000.11.16. KJTINC

-----------------------------------------------------------------------------*/
/*
#ifndef __VS_UI_SKILL_TREE_H__
#define __VS_UI_SKILL_TREE_H__

#include "VS_UI_Widget.h"
#include "VS_UI_Util.h"

#include "MSkillManager.h"
/*
#define SKILLTREE_CLOSE_ID				0xFFFFFFFF

struct SKILLTREE_SKILL
{
	ACTIONINFO				id;
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE - base
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE : public Window, public Exec, public ButtonVisual
{
protected:
	C_SPRITE_PACK				m_skill_tree_back_spk;
	C_SPRITE_PACK				m_skill_icon_bright_image_spk;
	C_SPRITE_PACK *			m_p_etc_image_spk; // 상속받는 곳에서 load할 것.

	ButtonGroup *				m_pC_button_group;

	SKILLDOMAIN					m_skill_domain;

	int							m_extra_offset_x, m_extra_offset_y; // 상속받는 곳에서 set할 것.

	enum
	{
		SKILL_GUARD_X = 44,
		SKILL_GUARD_Y = 44,
	};

	enum ETC_IMAGE_SPK_INDEX
	{
		ICON_GUARD,
	};

public:
	C_VS_UI_SKILL_TREE();
	virtual ~C_VS_UI_SKILL_TREE();

	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	Process();

	void	AcquireDisappear() {}
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	virtual void	Show();

	void	Start();
	void	Finish();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_ENCHANT
//
// Enchant skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_ENCHANT : public C_VS_UI_SKILL_TREE
{
private:
	enum { ENCHANT_SKILL_COUNT = 13 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[ENCHANT_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_ENCHANT();
	~C_VS_UI_SKILL_TREE_ENCHANT();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_BLADE
//
// Blade skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_BLADE : public C_VS_UI_SKILL_TREE
{
private:
	enum { BLADE_SKILL_COUNT = 12 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[BLADE_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_BLADE();
	~C_VS_UI_SKILL_TREE_BLADE();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_SWORD
//
// Sword skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_SWORD : public C_VS_UI_SKILL_TREE
{
private:
	enum { SWORD_SKILL_COUNT = 12 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[SWORD_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_SWORD();
	~C_VS_UI_SKILL_TREE_SWORD();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_GUN
//
// Gun skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_GUN : public C_VS_UI_SKILL_TREE
{
private:
	enum { GUN_SKILL_COUNT = 10 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[GUN_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_GUN();
	~C_VS_UI_SKILL_TREE_GUN();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_HEAL
//
// Heal skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_HEAL : public C_VS_UI_SKILL_TREE
{
private:
	enum { HEAL_SKILL_COUNT = 13 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[HEAL_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_HEAL();
	~C_VS_UI_SKILL_TREE_HEAL();
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_VAMPIRE
//
// Heal skill tree
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL_TREE_VAMPIRE : public C_VS_UI_SKILL_TREE
{
private:
	enum { VAMPIRE_SKILL_COUNT = 22 }; // skill tree 상에 있는 것의 개수.

	static SKILLTREE_SKILL	m_skill_icon_tab[VAMPIRE_SKILL_COUNT];

public:
	C_VS_UI_SKILL_TREE_VAMPIRE();
	~C_VS_UI_SKILL_TREE_VAMPIRE();
};

#endif
  */