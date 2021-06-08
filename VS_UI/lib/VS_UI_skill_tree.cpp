// VS_UI_skill_tree.cpp

#include "client_PCH.h"

//#include "VS_UI_skill_tree.h"
//#include "VS_UI_filepath.h"
//#include "VS_UI_Description.h"
/*
// close button
#define ADD_CLOSE_BUTTON	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(Right()-94, Down()-66, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), SKILLTREE_CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));

//
// ! SkillTree에 skill 위치를 지정하는 방법은 다음과 같다.
//   skill을 배우는 순서를 따라가되 skill tree가 나오면 좌에서 우로 번호를
//   매긴다.
//
// ! 아직없는 것은 나중에 Sprite Pack 뒤에 추가된다. (중간에 있는것들도)
//
SKILLTREE_SKILL C_VS_UI_SKILL_TREE_ENCHANT::m_skill_icon_tab[ENCHANT_SKILL_COUNT] = {
	MAGIC_CREATE_HOLY_WATER,
	MAGIC_LIGHT,
	MAGIC_DETECT_HIDDEN,
	MAGIC_AURA_RING,
	MAGIC_BLESS_GROUND,
	MAGIC_STRIKING,
	MAGIC_BLESS,
	MAGIC_CONTINUAL_LIGHT,
	MAGIC_AURA_PRISM,
	MAGIC_DETECT_INVISIBILITY,
	MAGIC_VISIBLE,
	MAGIC_CHAIN_AURA,
	MAGIC_CHRIST_AURA,
	
};

SKILLTREE_SKILL C_VS_UI_SKILL_TREE_HEAL::m_skill_icon_tab[HEAL_SKILL_COUNT] = {
	MAGIC_CURE_LIGHT_WOUNDS,
	MAGIC_PROTECTION_FROM_POISON,
	MAGIC_CURE_POISON,
	MAGIC_AURA_BALL,
	MAGIC_PROTECTION_FROM_PARALYSIS,
	MAGIC_CURE_PARALYSIS,
	MAGIC_CURE_CRITICAL_WOUNDS,
	MAGIC_PROTECTION_FROM_CURSE,
	MAGIC_AURA_SHIELD,
	MAGIC_REGENERATION,
	MAGIC_CURE_ALL,
	MAGIC_AURA_CROSS,	//AURA STORM
	MAGIC_SAINT_AURA,
};

SKILLTREE_SKILL C_VS_UI_SKILL_TREE_BLADE::m_skill_icon_tab[BLADE_SKILL_COUNT] = {
	SKILL_SINGLE_BLOW,
	SKILL_SPIRAL_SLAY,
	SKILL_TRIPLE_BREAK,
	SKILL_WILD_SMASH,
	SKILL_HURRICANE_COMBO,
	SKILL_POTENTIAL_EXPLOSION,
	SKILL_SHADOW_WALK,
	SKILL_CHARGING_POWER,
	SKILL_TORNADO_SEVER,
	SKILL_ARMAGEDDON_SLASHER,
	SKILL_SOUL_SHOCK,
	SKILL_SAINT_BLADE,
};

SKILLTREE_SKILL C_VS_UI_SKILL_TREE_SWORD::m_skill_icon_tab[SWORD_SKILL_COUNT] = {
	SKILL_DOUBLE_IMPACT,
	SKILL_TRIPLE_SLASHER,
	SKILL_RAINBOW_SLASHER,
	SKILL_THUNDER_SPARK,
	SKILL_SNAKE_COMBO,
	SKILL_CROSS_COUNTER,
	SKILL_FLASH_SLIDING,
	SKILL_LIGHTNING_HAND,
	SKILL_SWORD_WAVE,
	SKILL_DRAGON_RISING,
	SKILL_FIVE_STORM_CRASH,
	SKILL_HEAVENS_SWORD,
};

SKILLTREE_SKILL C_VS_UI_SKILL_TREE_VAMPIRE::m_skill_icon_tab[VAMPIRE_SKILL_COUNT] = {
	MAGIC_HIDE,
	MAGIC_DARKNESS,
	MAGIC_POISONOUS_HANDS,
	MAGIC_ACID_TOUCH,
	MAGIC_GREEN_POISON,
	MAGIC_YELLOW_POISON,
	MAGIC_PURPLE_WALL,
	MAGIC_ACID_BOLT,
	MAGIC_CURSE_LEVEL1,
	MAGIC_GREEN_STALKER,
	MAGIC_CURSE_LEVEL2,
	MAGIC_INVISIBILITY,
	MAGIC_DARKBLUE_POISON,
	MAGIC_ACID_BALL,
	MAGIC_CURSE_LEVEL3,
	MAGIC_ACID_SWAMP,
	MAGIC_BLOODY_NAIL,
	MAGIC_BLOODY_KNIFE,
	MAGIC_BLOODY_BALL,
	MAGIC_BLOODY_WALL,
	MAGIC_BLOODY_SPEAR,
	MAGIC_SUMMON,
};

SKILLTREE_SKILL C_VS_UI_SKILL_TREE_GUN::m_skill_icon_tab[GUN_SKILL_COUNT] = {
	SKILL_SHARP_SHOOTING,
	SKILL_FAST_RELOAD,
	SKILL_SCOPE_SHOOTING,
	SKILL_CRITICAL_SHOOTING,
	SKILL_KNOCKS_TARGET_BACK,
	SKILL_HOLY_SHOOTING,
	SKILL_DETECT_MINE,
	SKILL_MAKE_BOMB,
	SKILL_DISARM_MINE,
//	SKILL_INSTALL_MINE,
	SKILL_MAKE_MINE,
/*	
	MINE_VIPER,
	MINE_DIAMONDBACK,
	MINE_SIDEWINDER,
	MINE_COBRA,

	BOMB_ACER,
	BOMB_BULLS,
	BOMB_PYLONE,
	BOMB_CYCLONE,
	BOMB_CROSSBOW,
	BOMB_TWISTER,*//*
};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE::C_VS_UI_SKILL_TREE()
{
	g_RegisterWindow(this);

	AttrPin(true);

	m_pC_button_group = new ButtonGroup(this);

	m_skill_domain = MAX_SKILLDOMAIN;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_SKILL_TREE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE::~C_VS_UI_SKILL_TREE()
{
	g_UnregisterWindow(this);

	DeleteNew(m_p_etc_image_spk);
	DeleteNew(m_pC_button_group);
}	

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL_TREE::Show()
{
	m_skill_tree_back_spk.Blt(x, y);
	m_pC_button_group->Show();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Run
//
// id는 ACTIONINFO이다.
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL_TREE::Run(id_t id)
{
	switch (id)
	{
		case SKILLTREE_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_SKILL_TREE);
			break;

		default:
			//
			// 배울 수 있는 것만 선택해서 Client로 보내준다.
			//
			assert(m_skill_domain != MAX_SKILLDOMAIN);

			// 새로운 skill을 배울 수 있는 상태인지 체크.. by sigi
			if ((*g_pSkillManager)[m_skill_domain].HasNewSkill())
			{
				MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus((ACTIONINFO)id);

//				assert(status != MSkillDomain::SKILLSTATUS_NULL);

				if (status == MSkillDomain::SKILLSTATUS_NEXT)
				{
					gpC_base->SendMessage(UI_LEARN_SLAYER_SKILL, id);
				}
			}
			break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL_TREE::IsPixel(int _x, int _y)
{
	return m_skill_tree_back_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if (p_button->GetID() == SKILLTREE_CLOSE_ID)
	{
		if (p_button->GetFocusState() && p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
	}
	else if(p_button->m_image_index != -1)
	{
		//*
		assert(m_skill_domain != MAX_SKILLDOMAIN); // 설정되었는가?

		MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus((ACTIONINFO)p_button->GetID());

		assert(status != MSkillDomain::SKILLSTATUS_NULL);

		if (status == MSkillDomain::SKILLSTATUS_LEARNED)
		{
			// 이미 배운 skill은 highlight된 icon으로 출력한다.
			m_skill_icon_bright_image_spk.Blt(p_button->x+m_extra_offset_x, p_button->y+m_extra_offset_y, p_button->m_image_index);
			m_p_etc_image_spk->BltColor(p_button->x, p_button->y, ICON_GUARD, rgb_BLUE);
		}
		else if (status == MSkillDomain::SKILLSTATUS_NEXT
					// 새로운 skill을 배울 수 있는 상태인지 체크.. by sigi
					&& (*g_pSkillManager)[m_skill_domain].HasNewSkill() )
		{
			// 배울 수 있는 skill은 guard색으로 구별시킨다.
			m_p_etc_image_spk->BltColor(p_button->x, p_button->y, ICON_GUARD, rgb_GREEN);

			// 배우기 위해 click하는가?
			if (p_button->GetFocusState() && p_button->GetPressState())
			{
				m_skill_icon_bright_image_spk.Blt(p_button->x+m_extra_offset_x, p_button->y+m_extra_offset_y, p_button->m_image_index);
			}
		}
		//*/

		// test ----------------------------------------------------
		/*
		if (p_button->GetPressState())
		{
		m_p_etc_image_spk->Blt(p_button->x, p_button->y, ICON_GUARD);
		m_skill_icon_bright_image_spk.Blt(p_button->x+4, p_button->y+4, p_button->m_image_index);
		}
		*/
		// end of test ---------------------------------------------

		/*
		MSkillDomain& domain = (*g_pSkillManager)[m_skill_domain];
		domain.SetBegin();
		while (domain.IsNotEnd())
		{
			// skill의 id와 status
			ACTIONINFO						id			= domain.GetSkillID();
			MSkillDomain::SKILLSTATUS	status	= domain.GetSkillStatus();

			//---------------------------------------
			// status는 다음과 같다. 
			//---------------------------------------
			//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
			//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
			//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
			//---------------------------------------
			
			//---------------------------------------
			// id를 알면 g_SkillInfoTable에서 
			// 그 id의 skill에 대한 정보를 얻을 수 있다.
			//---------------------------------------
			//logFile << "[" << id << "] " << g_SkillInfoTable[id].GetName()
			//		<< " = " << (int)status << endl;
			if (p_button->GetPressState())
			{
			m_skill_icon_bright_image_spk.Blt(p_button->x+4, p_button->y+4, p_button->m_image_index);

			// guard
			m_p_etc_image_spk->Blt(p_button->x, p_button->y, ICON_GUARD);
			}

			// 다음
			domain.Next();
		}*//*
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL_TREE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	m_pC_button_group->MouseControl(message, _x, _y);

	switch (message)
	{
		case M_MOVING:
			{
				C_VS_UI_EVENT_BUTTON * p_button;
				p_button = m_pC_button_group->IsInRect(_x, _y);
				if (p_button != NULL && p_button->GetID() != SKILLTREE_CLOSE_ID)
				{
					g_descriptor_manager.Set(DID_SKILLTREE, p_button->x, p_button->y, (void *)&m_skill_domain, p_button->GetID(), SKILL_GUARD_Y);
				}
			}
			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::KeyboardControl(UINT message, UINT key, long extra)
{
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Finish
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_TREE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_ENCHANT
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_ENCHANT::C_VS_UI_SKILL_TREE_ENCHANT()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_SLAYER);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;

	m_skill_domain = SKILLDOMAIN_ENCHANT;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_ENCHANT);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_ENCHANT);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 5, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

	//x = y = 0;

	ADD_CLOSE_BUTTON;

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+33, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+89, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+140, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+190, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+240, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+290, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+340, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+391, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+442, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+442, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+391, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[10].id, this, 10));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+340, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[11].id, this, 11));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+290, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[12].id, this, 12));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& enchantDomain = (*g_pSkillManager)[SKILLDOMAIN_ENCHANT];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_CREATE_HOLY_WATER );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_LIGHT );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_DETECT_HIDDEN );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_AURA_RING );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_BLESS_GROUND );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_STRIKING );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_BLESS );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_CONTINUAL_LIGHT );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_AURA_PRISM );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_DETECT_INVISIBILITY );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_VISIBLE );
	enchantDomain.SetNewSkill();
	enchantDomain.LearnSkill( MAGIC_CHAIN_AURA );
	enchantDomain.SetNewSkill();
//	enchantDomain.LearnSkill( MAGIC_CHRIST_AURA );

#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_SKILL_TREE_ENCHANT
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_ENCHANT::~C_VS_UI_SKILL_TREE_ENCHANT()
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_SWORD
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_SWORD::C_VS_UI_SKILL_TREE_SWORD()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_SLAYER);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;

	m_skill_domain = SKILLDOMAIN_SWORD;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_SWORD);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_SWORD);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 5, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

//	x = y = 0;

	ADD_CLOSE_BUTTON;

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+33, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+78, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+66, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+175, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+170, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+216, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+66, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+175, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+308, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+354, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+400, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[10].id, this, 10));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+446, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[11].id, this, 11));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& swordDomain = (*g_pSkillManager)[SKILLDOMAIN_SWORD];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_DOUBLE_IMPACT );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_TRIPLE_SLASHER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_RAINBOW_SLASHER );	
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_THUNDER_SPARK );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_SNAKE_COMBO );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_CROSS_COUNTER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_FLASH_SLIDING );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_LIGHTNING_HAND );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_SWORD_WAVE );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_DRAGON_RISING );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_FIVE_STORM_CRASH );
	swordDomain.SetNewSkill();
//	swordDomain.LearnSkill( SKILL_HEAVENS_SWORD );

#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_SKILL_TREE_SWORD
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_SWORD::~C_VS_UI_SKILL_TREE_SWORD()
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_VAMPIRE::C_VS_UI_SKILL_TREE_VAMPIRE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_VAMPIRE::C_VS_UI_SKILL_TREE_VAMPIRE()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_VAMPIRE);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;

	m_skill_domain = SKILLDOMAIN_VAMPIRE;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_VAMPIRE);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_VAMPIRE);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 0, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

	//x = y = 0;

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+305, y+454, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), SKILLTREE_CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+96, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+96, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+96, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+96, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+154, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+154, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+154, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+154, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+212, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+212, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+212, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[10].id, this, 10));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+212, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[11].id, this, 11));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+270, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[12].id, this, 12));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+270, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[13].id, this, 13));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+270, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[14].id, this, 14));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+270, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[15].id, this, 15));

	// 요사이에 두개가 앞으로 추가될 것임.
	//m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+328, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[16].id, this, 16));
	//m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+328, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[17].id, this, 17));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+328, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[16].id, this, 16));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+328, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[17].id, this, 17));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+282, y+386, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[18].id, this, 18));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+223, y+386, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[19].id, this, 19));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+163, y+386, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[20].id, this, 20));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+104, y+386, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[21].id, this, 21));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& vampireDomain = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	vampireDomain.SetNewSkill();
	vampireDomain.LearnSkill( MAGIC_HIDE );
	vampireDomain.SetNewSkill();
	vampireDomain.LearnSkill( MAGIC_DARKNESS );
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_VAMPIRE::~C_VS_UI_SKILL_TREE_VAMPIRE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_VAMPIRE::~C_VS_UI_SKILL_TREE_VAMPIRE()
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_BLADE::C_VS_UI_SKILL_TREE_BLADE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_BLADE::C_VS_UI_SKILL_TREE_BLADE()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_SLAYER);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;

	m_skill_domain = SKILLDOMAIN_BLADE;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_BLADE);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_BLADE);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 5, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

//	x = y = 0;

	ADD_CLOSE_BUTTON;

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+33, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+78, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+66, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+175, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+170, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+216, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+66, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+175, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+308, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+354, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+400, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[10].id, this, 10));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+446, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[11].id, this, 11));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// blade Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& bladeDomain = (*g_pSkillManager)[SKILLDOMAIN_BLADE];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_SINGLE_BLOW );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_SPIRAL_SLAY );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_TRIPLE_BREAK );	
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_WILD_SMASH );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_HURRICANE_COMBO );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_POTENTIAL_EXPLOSION );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_SHADOW_WALK );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_CHARGING_POWER );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_TORNADO_SEVER );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_ARMAGEDDON_SLASHER );
	bladeDomain.SetNewSkill();
	bladeDomain.LearnSkill( SKILL_SOUL_SHOCK );
	bladeDomain.SetNewSkill();
//	bladeDomain.LearnSkill( SKILL_SAINT_BLADE );

#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_BLADE::~C_VS_UI_SKILL_TREE_BLADE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_BLADE::~C_VS_UI_SKILL_TREE_BLADE()
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_HEAL::C_VS_UI_SKILL_TREE_HEAL
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_HEAL::C_VS_UI_SKILL_TREE_HEAL()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_SLAYER);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;

	m_skill_domain = SKILLDOMAIN_HEAL;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_HEAL);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_HEAL);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 5, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

	//x = y = 0;

	ADD_CLOSE_BUTTON;

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+33, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+89, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+140, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+190, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+240, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+290, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+340, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+391, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+442, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+442, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+391, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[10].id, this, 10));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+340, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[11].id, this, 11));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+57, y+290, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[12].id, this, 12));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& healDomain = (*g_pSkillManager)[SKILLDOMAIN_HEAL];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_PROTECTION_FROM_POISON );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_CURE_LIGHT_WOUNDS );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_CURE_POISON );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_AURA_BALL );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_PROTECTION_FROM_PARALYSIS );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_CURE_PARALYSIS );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_CURE_CRITICAL_WOUNDS );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_PROTECTION_FROM_CURSE );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_AURA_SHIELD );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_REGENERATION );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_CURE_ALL );
	healDomain.SetNewSkill();
	healDomain.LearnSkill( MAGIC_AURA_CROSS );
	healDomain.SetNewSkill();
//	healDomain.LearnSkill( MAGIC_SAINT_AURA );
	
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_HEAL::~C_VS_UI_SKILL_TREE_HEAL
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_HEAL::~C_VS_UI_SKILL_TREE_HEAL()
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_GUN::C_VS_UI_SKILL_TREE_GUN
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_GUN::C_VS_UI_SKILL_TREE_GUN()
{
	m_p_etc_image_spk = new C_SPRITE_PACK(SPK_SKILLTREE_ETC_SLAYER);
	m_extra_offset_x = 4;
	m_extra_offset_y = 4;


	m_skill_domain = SKILLDOMAIN_GUN;

	m_skill_tree_back_spk.Open(SPK_SKILLTREE_BACK_GUN);
	m_skill_icon_bright_image_spk.Open(SPK_SKILLTREE_ICO_GUN);

	Set(RESOLUTION_X/2-m_skill_tree_back_spk.GetWidth()/2, 5, m_skill_tree_back_spk.GetWidth(), m_skill_tree_back_spk.GetHeight());

	//x = y = 0;

	ADD_CLOSE_BUTTON;

	// add skill icon button!
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+33, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[0].id, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+78, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[1].id, this, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[2].id, this, 2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+170, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[3].id, this, 3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+216, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[4].id, this, 4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[5].id, this, 5));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+308, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[6].id, this, 6));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+354, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[7].id, this, 7));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+400, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[8].id, this, 8));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+120, y+446, SKILL_GUARD_X, SKILL_GUARD_Y, m_skill_icon_tab[9].id, this, 9));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+41, y+310, SKILL_GUARD_X, SKILL_GUARD_Y, MINE_COBRA, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+41, y+356, SKILL_GUARD_X, SKILL_GUARD_Y, MINE_SIDEWINDER, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+41, y+402, SKILL_GUARD_X, SKILL_GUARD_Y, MINE_DIAMONDBACK, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+41, y+448, SKILL_GUARD_X, SKILL_GUARD_Y, MINE_VIPER, this, -1));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+124, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_ACER, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+170, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_BULLS, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+216, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_PYLONE, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+262, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_CYCLONE, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+308, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_CROSSBOW, this, -1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+201, y+354, SKILL_GUARD_X, SKILL_GUARD_Y, BOMB_TWISTER, this, -1));

#ifndef _LIB 
	//---------------------------------------------------------------------
	//
	// (*g_pSkillManager)에서
	// Sword Domain의 Skill들을 가지고 작업한다.
	//
	//---------------------------------------------------------------------
	MSkillDomain& gunDomain = (*g_pSkillManager)[SKILLDOMAIN_GUN];

	//---------------------------------------------------------------------
	// 몇가지 skill을 배웠다고 표시한다.
	//---------------------------------------------------------------------
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_SHARP_SHOOTING );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_FAST_RELOAD );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_SCOPE_SHOOTING );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_CRITICAL_SHOOTING );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_KNOCKS_TARGET_BACK );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_HOLY_SHOOTING );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_DETECT_MINE );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_MAKE_BOMB );
	gunDomain.SetNewSkill();
	gunDomain.LearnSkill( SKILL_DISARM_MINE );
	gunDomain.SetNewSkill();
//	gunDomain.LearnSkill( SKILL_MAKE_MINE );
	
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE_GUN::~C_VS_UI_SKILL_TREE_GUN
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_TREE_GUN::~C_VS_UI_SKILL_TREE_GUN()
{

}
*/