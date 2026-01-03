// VS_UI_SKILL_VIEW.cpp: implementation of the C_VS_UI_SKILL_VIEW class.
//
//////////////////////////////////////////////////////////////////////

#include "client_PCH.h"

/*
#include "VS_UI_SKILL_VIEW.h"
#include "VS_UI_GameCommon.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI.h"

//domain의 순서가 그래픽의 순서와 서버상의 순서가 틀려서 변환용 테이블
int temp_domain[5] = {1,0,2,3,4};

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_VIEW::C_VS_UI_SKILL_VIEW()
{
	m_pC_button_group = NULL;
	m_p_skill_view_etc_spk = NULL;
	m_p_skill_view_spk = NULL;
	m_p_skill_view_grade_spk = NULL; 
	m_pC_skill_view_domain_spk = NULL;

	g_RegisterWindow(this);
	AttrPin(true);

	m_pC_button_group = new ButtonGroup(this);

	m_p_skill_view_etc_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_ETC);

// member data init
//	m_iGrade = 0;
//	m_iDomain = 0;
//	m_iScroll = 0;

	if(gbl_vampire_interface)
	{
//		if(m_skill_domain != SKILLDOMAIN_VAMPIRE)
//		{
			m_skill_domain = SKILLDOMAIN_VAMPIRE;
			m_iDomain = 0;
			m_iGrade = 0;
			m_iScroll = 0;
//		}
	}
	else
	{
//		if(m_skill_domain == SKILLDOMAIN_VAMPIRE)
//		{
		int level = -1;
		for(int i = 0; i < 5; i++)
		{
			int level_temp = (*g_pSkillManager)[(SKILLDOMAIN)i].GetDomainLevel();
			if(level_temp > level)
			{
				level = level_temp;
				m_skill_domain = (SKILLDOMAIN)i;
				m_iDomain = temp_domain[i];
			}
		}
			m_iGrade = 0;
			m_iScroll = 0;
//		}
	}

	if(gbl_vampire_interface)
	{
		m_p_skill_view_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE);
		m_p_skill_view_grade_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE_GRADE);
		m_pC_skill_view_domain_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE_DOMAIN);
	}
	else
	{
		m_p_skill_view_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER);
		m_p_skill_view_grade_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER_GRADE);
		m_pC_skill_view_domain_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER_DOMAIN);
	}

	Set(RESOLUTION_X/2-m_p_skill_view_spk->GetWidth(SKILL_VIEW_BACK)/2, RESOLUTION_Y/2-m_p_skill_view_spk->GetHeight(SKILL_VIEW_BACK)/2, m_p_skill_view_spk->GetWidth(SKILL_VIEW_BACK), m_p_skill_view_spk->GetHeight(SKILL_VIEW_BACK));


// button
	int close_button_x, close_button_y, help_button_x, help_button_y;
	int scroll_x, scroll_up_y, scroll_down_y;
	int grade_y, grade_prev_x, grade_next_x;

	if(gbl_vampire_interface)
	{
		// Domain_ICON load
//		ifstream	file(SPK_SKILLVIEW_VAMPIRE_DOMAIN, ios::binary|ios::nocreate);
//		if (!file)
//			_Error(FILE_OPEN);
//		m_pC_skill_view_domain_spk.LoadFromFile(file);
//		file.close();

		help_button_y = close_button_y = 417;
		help_button_x = 220;
		close_button_x = 288;
		scroll_x = 319-7;
		scroll_up_y = 143-4;
		scroll_down_y = 316;
		grade_prev_x = 129-9;
		grade_next_x = 316-9;
		grade_y = 102;
	}
	else
	{
		// Domain_ICON load
//		ifstream	file(SPK_SKILLVIEW_SLAYER_DOMAIN, ios::binary|ios::nocreate);
//		if (!file)
//			_Error(FILE_OPEN);
//		m_pC_skill_view_domain_spk.LoadFromFile(file);
//		file.close();
		
		help_button_y = close_button_y = 287;
		help_button_x = 189;
		close_button_x = 257;
		scroll_x = 319;
		scroll_up_y = 143;
		scroll_down_y = 255;
		grade_prev_x = 129;
		grade_next_x = 316;
		grade_y = 108;
	}


	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_x, help_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, close_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x, scroll_up_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_UP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_UP), SCROLL_UP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_UP));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x, scroll_down_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), SCROLL_DOWN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_DOWN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(grade_prev_x, grade_y, m_p_skill_view_etc_spk->GetWidth(GRADE_PREV_PUSHED), m_p_skill_view_etc_spk->GetHeight(GRADE_PREV_PUSHED), GRADE_PREV_ID, this, GRADE_PREV_PUSHED));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(grade_next_x, grade_y, m_p_skill_view_etc_spk->GetWidth(GRADE_NEXT_PUSHED), m_p_skill_view_etc_spk->GetHeight(GRADE_NEXT_PUSHED), GRADE_NEXT_ID, this, GRADE_NEXT_PUSHED));

	if(gbl_vampire_interface)
	{
		m_skill_x = 112;
		m_skill_y = 140;
		m_skill_x_gap = 64;
		m_skill_y_gap = 65;
		

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103, 342, m_pC_skill_view_domain_spk->GetWidth(POISON_BUTTON), m_pC_skill_view_domain_spk->GetHeight(POISON_BUTTON), POISON_BUTTON_ID, this, POISON_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103+80, 342, m_pC_skill_view_domain_spk->GetWidth(ACID_BUTTON), m_pC_skill_view_domain_spk->GetHeight(ACID_BUTTON), ACID_BUTTON_ID, this, ACID_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103+80*2, 342, m_pC_skill_view_domain_spk->GetWidth(CURSE_BUTTON), m_pC_skill_view_domain_spk->GetHeight(CURSE_BUTTON), CURSE_BUTTON_ID, this, CURSE_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103, 342+29, m_pC_skill_view_domain_spk->GetWidth(SUMMON_BUTTON), m_pC_skill_view_domain_spk->GetHeight(SUMMON_BUTTON), SUMMON_BUTTON_ID, this, SUMMON_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103+80, 342+29, m_pC_skill_view_domain_spk->GetWidth(BLOOD_BUTTON), m_pC_skill_view_domain_spk->GetHeight(BLOOD_BUTTON), BLOOD_BUTTON_ID, this, BLOOD_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(103+80*2, 342+29, m_pC_skill_view_domain_spk->GetWidth(INNATE_BUTTON), m_pC_skill_view_domain_spk->GetHeight(INNATE_BUTTON), INNATE_BUTTON_ID, this, INNATE_BUTTON));
	}
	else
	{
		m_skill_x = 120;
		m_skill_y = 144;
		m_skill_x_gap = 66;
		m_skill_y_gap = 63;

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(41, 120, m_pC_skill_view_domain_spk->GetWidth(SWORD_BUTTON), m_pC_skill_view_domain_spk->GetHeight(SWORD_BUTTON), SWORD_BUTTON_ID, this, SWORD_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(41, 120+36, m_pC_skill_view_domain_spk->GetWidth(BLADE_BUTTON), m_pC_skill_view_domain_spk->GetHeight(BLADE_BUTTON), BLADE_BUTTON_ID, this, BLADE_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(41, 120+36*2, m_pC_skill_view_domain_spk->GetWidth(GUN_BOMB_BUTTON), m_pC_skill_view_domain_spk->GetHeight(GUN_BOMB_BUTTON), GUN_BOMB_BUTTON_ID, this, GUN_BOMB_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(41, 120+36*3, m_pC_skill_view_domain_spk->GetWidth(HEAL_BUTTON), m_pC_skill_view_domain_spk->GetHeight(HEAL_BUTTON), HEAL_BUTTON_ID, this, HEAL_BUTTON));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(41, 120+36*4, m_pC_skill_view_domain_spk->GetWidth(ENCHANT_BUTTON), m_pC_skill_view_domain_spk->GetHeight(ENCHANT_BUTTON), ENCHANT_BUTTON_ID, this, ENCHANT_BUTTON));
	}
	SetRect(&m_rcSkillDesciption, m_skill_x, m_skill_y, m_skill_x + m_skill_x_gap*3, m_skill_y + m_skill_y_gap*(gbl_vampire_interface?3:2));

#ifndef _LIB
	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetDomainLevel(1);
	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetDomainExp(990);
	(*g_pSkillManager)[SKILLDOMAIN_BLADE].SetDomainLevel(30);
	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetDomainLevel(60);
	(*g_pSkillManager)[SKILLDOMAIN_HEAL].SetDomainLevel(80);
	(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].SetDomainLevel(100);
	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetDomainLevel(21);

	MSkillDomain& swordDomain = (*g_pSkillManager)[SKILLDOMAIN_SWORD];

	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_DOUBLE_IMPACT );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_TRIPLE_SLASHER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_THUNDER_SPARK );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_RAINBOW_SLASHER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_SNAKE_COMBO );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_CROSS_COUNTER );
	swordDomain.SetNewSkill();
	(*g_pSkillInfoTable)[SKILL_DOUBLE_IMPACT].SetExpLevel( 00 );
	(*g_pSkillInfoTable)[SKILL_TRIPLE_SLASHER].SetExpLevel( 20 );
	(*g_pSkillInfoTable)[SKILL_THUNDER_SPARK].SetExpLevel( 40 );
	(*g_pSkillInfoTable)[SKILL_RAINBOW_SLASHER].SetExpLevel( 60 );
	(*g_pSkillInfoTable)[SKILL_SNAKE_COMBO].SetExpLevel( 80 );
	(*g_pSkillInfoTable)[SKILL_CROSS_COUNTER].SetExpLevel( 100 );

	(*g_pSkillInfoTable)[SKILL_DOUBLE_IMPACT].SetSkillExp( 149 );
	(*g_pSkillInfoTable)[SKILL_TRIPLE_SLASHER].SetSkillExp( 19999 );
	(*g_pSkillInfoTable)[SKILL_THUNDER_SPARK].SetSkillExp( 40 );
	(*g_pSkillInfoTable)[SKILL_RAINBOW_SLASHER].SetSkillExp( 60 );
	(*g_pSkillInfoTable)[SKILL_SNAKE_COMBO].SetSkillExp( 80 );
	(*g_pSkillInfoTable)[SKILL_CROSS_COUNTER].SetSkillExp( 100 );

	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetNewSkill();
	(*g_pSkillManager)[SKILLDOMAIN_GUN].LearnSkill(SKILL_FAST_RELOAD);

	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetNewSkill();
	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].LearnSkill(MAGIC_ACID_TOUCH);
	
#endif
	m_domain_level[0] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_SWORD].GetDomainLevel()/100;
	m_domain_level[1] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_BLADE].GetDomainLevel()/100;
	m_domain_level[2] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_GUN].GetDomainLevel()/100;
	m_domain_level[3] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_HEAL].GetDomainLevel()/100;
	m_domain_level[4] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].GetDomainLevel()/100;
	m_domain_level[5] = 29-29*(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel()/100;

//			EFFECT_DARKER = 0,
//			EFFECT_GRAYSCALE,
//			EFFECT_LIGHTEN,
//			EFFECT_DARKEN,
//			EFFECT_COLORDODGE,
//			EFFECT_SCREEN,
//			EFFECT_DODGEBURN,
//			EFFECT_DIFFERENT,


// button groups
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(status_x_offset, status_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_UP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_UP), STATUS_UP, this, C_GLOBAL_RESOURCE::AB_BUTTON_UP));

//	m_skill_domain = MAX_SKILLDOMAIN;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_SKILL_VIEW
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SKILL_VIEW::~C_VS_UI_SKILL_VIEW()
{
	g_UnregisterWindow(this);

	DeleteNew(m_p_skill_view_etc_spk); 
	DeleteNew(m_p_skill_view_spk); 
	DeleteNew(m_p_skill_view_grade_spk); 
	DeleteNew(m_pC_skill_view_domain_spk); 

	DeleteNew(m_pC_button_group);
}	

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL_VIEW::Show()
{
	CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);

	m_p_skill_view_spk->Blt(x, y, SKILL_VIEW_BACK);

	int grade_x, grade_y, skill_ico_x, skill_ico_y;
	int scroll_size = (gbl_vampire_interface?9:6);

	int scroll_x, scroll_y, scroll_height;

	if(gbl_vampire_interface)
	{
		grade_x = x + 142;
		grade_y = y + 101;

		skill_ico_x = x+m_skill_x + 13;
		skill_ico_y = y+m_skill_y + 14;

		scroll_x = x + 313;
		scroll_y = y + 160;
		scroll_height = 134;

	}
	else
	{
		grade_x = x + 151;
		grade_y = y + 108;

		skill_ico_x = x+m_skill_x + 14;
		skill_ico_y = y+m_skill_y + 13;

		scroll_x = x + 319;
		scroll_y = y + 162;
		scroll_height = 73;
	}

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if(gbl_vampire_interface)
		{
			if(m_iGrade >= 0 && m_iGrade < 3)
				m_p_skill_view_grade_spk->BltLocked(grade_x, grade_y, m_iGrade);
		}
		else
		{
			if(m_iGrade >= 0 && m_iGrade < 5)
				m_p_skill_view_grade_spk->BltLocked(grade_x, grade_y, m_iGrade);
		}
		
		// SKILL STEP이 있으면 스킬들을 출력한다
		if(!gbl_vampire_interface || m_iGrade == 0)
		{
			if((gbl_vampire_interface && m_iDomain >=0 && m_iDomain < 6 && m_iScroll >= 0 && m_skill_domain == SKILLDOMAIN_VAMPIRE) || 
				(!gbl_vampire_interface && m_iDomain >=0 && m_iDomain < 5 && m_iScroll >= 0 && (m_skill_domain == SKILLDOMAIN_SWORD || m_skill_domain == SKILLDOMAIN_BLADE || m_skill_domain == SKILLDOMAIN_GUN || m_skill_domain == SKILLDOMAIN_HEAL || m_skill_domain == SKILLDOMAIN_ENCHANT)))
			{
				SKILL_STEP step = (SKILL_STEP)(gbl_vampire_interface?m_iDomain+SKILL_STEP_VAMPIRE_POISON:m_iGrade+SKILL_STEP_APPRENTICE);
				if((*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
				{
					MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step));
					
					MSkillDomain::SKILL_STEP_LIST::iterator ss = list.begin()+m_iScroll*3;
					
					for(int skill = 0; skill < min( scroll_size, list.size() - m_iScroll*3); skill++)
					{
						MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus((ACTIONINFO)*ss);
						int sprID = (*g_pSkillInfoTable)[(ACTIONINFO)*ss].GetSpriteID();
						//---------------------------------------
						// status는 다음과 같다. 
						//---------------------------------------
						//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
						//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
						//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
						//---------------------------------------
						m_p_skill_view_spk->BltLocked(x+m_skill_x + skill%3 * m_skill_x_gap, y+m_skill_y + skill/3*m_skill_y_gap, SKILL_GUARD);
						
						POINT p = {skill_ico_x + skill%3 * m_skill_x_gap, skill_ico_y + skill/3*m_skill_y_gap};
						
						switch(status)
						{
						case MSkillDomain::SKILLSTATUS_LEARNED:
							{
								int level = (*g_pSkillInfoTable)[(ACTIONINFO)*ss].GetExpLevel();
								RECT rect = {p.x, p.y, p.x + 36, p.y+36-36*level/100};
								
								if((*g_pSkillInfoTable)[(ACTIONINFO)*ss].IsEnable() == false)
								{
									if(sprID < C_VS_UI_SKILL::m_pC_spk.GetSize() && C_VS_UI_SKILL::m_pC_spk[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_pC_spk[sprID]);
								}
								if((*g_pSkillInfoTable)[(ACTIONINFO)*ss].IsPassive())
								{
									if(sprID < C_VS_UI_SKILL::m_pC_spk.GetSize() && C_VS_UI_SKILL::m_pC_spk[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&p, &C_VS_UI_SKILL::m_pC_spk[sprID], 315);
								}
								else
								{
									POINT p2 = p;
									if(sprID < C_VS_UI_SKILL::m_pC_spk.GetSize() && C_VS_UI_SKILL::m_pC_spk[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSprite(&p2, &C_VS_UI_SKILL::m_pC_spk[sprID]);
									
									if(!gbl_vampire_interface)
									{
										// 배운부분만 밝게 표시
										//									gpC_base->m_p_DDSurface_back->ChangeBrightnessBit(&rect, 1);
										if(rect.bottom > 0 && rect.right > 0 && rect.left < 800 && rect.top < 600)
											DrawAlphaBox(&rect, 0, 0, 0, 15);
										
										// EXP BAR
										int max_exp = (*g_pSkillInfoTable)[(ACTIONINFO)*ss].maxExp;
										int exp = (*g_pSkillInfoTable)[(ACTIONINFO)*ss].GetSkillExp();
										int exp_percent;
										if(max_exp > 0)
											exp_percent = (exp % (max_exp/100))*100/(max_exp/100);
										else exp_percent = 0;

										if(exp == max_exp)exp_percent = 100;
										Rect clip_rect(0, 0, 50*exp_percent/100, 4);
										m_p_skill_view_etc_spk->BltLockedClip(p.x-7, p.y+40, clip_rect, EXP_BAR);
									}
								}
							}
							break;
							
						default:
							int domain_level;
							if(gbl_vampire_interface)
								domain_level = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel();
							else
							{
								if((*g_pSkillInfoTable)[(ACTIONINFO)*ss].DomainType >= 0)
									domain_level = (*g_pSkillManager)[(*g_pSkillInfoTable)[(ACTIONINFO)*ss].DomainType].GetDomainLevel();
								else
									domain_level = 0;
							}
							
							if((*g_pSkillInfoTable)[(ACTIONINFO)*ss].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[(ACTIONINFO)*ss].GetLearnLevel() >= 0)
							{
								if(sprID < C_VS_UI_SKILL::m_pC_spk.GetSize() && C_VS_UI_SKILL::m_pC_spk[sprID].IsInit())
									gpC_base->m_p_DDSurface_back->BltSpriteColor(&p, &C_VS_UI_SKILL::m_pC_spk[sprID], rgb_GREEN);
							}
							else
								
							{
								if(sprID < C_VS_UI_SKILL::m_pC_spk.GetSize() && C_VS_UI_SKILL::m_pC_spk[sprID].IsInit())
									gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_pC_spk[sprID]);
							}
						}
						
						ss++;
					}
//					if(list.size() > scroll_size)
//						gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(scroll_x, scroll_y + scroll_height*(m_iScroll)/((list.size()-scroll_size+2)/3), (gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG));
				}
			}
		}
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_button_group->Show();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::Run
//
// id는 ACTIONINFO이다.
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL_VIEW::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
//		gpC_base->SendMessage(UI_CLOSE_SKILL_VIEW);
//		gC_vs_ui.HotKey_SkillInfo();
		break;

	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)(gbl_vampire_interface?C_VS_UI_DESC_DIALOG::VAMPIRESKILLINFO:C_VS_UI_DESC_DIALOG::SLAYERSKILLINFO));
		m_pC_button_group->UnacquireMouseFocus();
		break;

	case SCROLL_UP_ID:
		if(m_iScroll > 0) m_iScroll--;
		break;

	case SCROLL_DOWN_ID:
		{
			if(!((gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 6 && m_iScroll >= 0 && m_skill_domain == SKILLDOMAIN_VAMPIRE) || 
				(!gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 5 && m_iScroll >= 0 && (m_skill_domain == SKILLDOMAIN_SWORD || m_skill_domain == SKILLDOMAIN_BLADE || m_skill_domain == SKILLDOMAIN_GUN || m_skill_domain == SKILLDOMAIN_HEAL || m_skill_domain == SKILLDOMAIN_ENCHANT))))
			if(gbl_vampire_interface && m_iGrade != 0)break;

			SKILL_STEP step = (SKILL_STEP)(gbl_vampire_interface?m_iDomain+SKILL_STEP_VAMPIRE_POISON:m_iGrade+SKILL_STEP_APPRENTICE);
			if((*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
			{
				MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step));
				if(m_iScroll*3 +(gbl_vampire_interface?9:6) < list.size())m_iScroll++;
			}
		}
		break;

	case GRADE_PREV_ID:
		if(m_iGrade > 0)
		{
			m_iGrade--;
			m_iScroll = 0;
		}
		break;
		
	case GRADE_NEXT_ID:
		if(gbl_vampire_interface)
		{
			if(m_iGrade < 1)
				m_iGrade++;
		}
		else
		{
			if(m_iGrade < 4)
				m_iGrade++;
		}
		m_iScroll = 0;
		break;

	default:
		m_iDomain = id - SWORD_BUTTON_ID;
		m_iGrade = 0;
		m_iScroll = 0;

		if(gbl_vampire_interface)
			m_skill_domain = SKILLDOMAIN_VAMPIRE;
		else 
			switch(id)
			{
			case SWORD_BUTTON_ID:
				m_skill_domain = SKILLDOMAIN_SWORD;
				break;

			case BLADE_BUTTON_ID:
				m_skill_domain = SKILLDOMAIN_BLADE;
				break;

			case GUN_BOMB_BUTTON_ID:
				m_skill_domain = SKILLDOMAIN_GUN;
				break;

			case HEAL_BUTTON_ID:
				m_skill_domain = SKILLDOMAIN_HEAL;
				break;

			case ENCHANT_BUTTON_ID:
				m_skill_domain = SKILLDOMAIN_ENCHANT;
				break;
			}
		break;
	}
//
//		default:
//			//
//			// 배울 수 있는 것만 선택해서 Client로 보내준다.
//			//
//			assert(m_skill_domain != MAX_SKILLDOMAIN);
//
//			// 새로운 skill을 배울 수 있는 상태인지 체크.. by sigi
//			if ((*g_pSkillManager)[m_skill_domain].HasNewSkill())
//			{
//				MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus((ACTIONINFO)id);
//
//				assert(status != MSkillDomain::SKILLSTATUS_NULL);
//
//				if (status == MSkillDomain::SKILLSTATUS_NEXT)
//				{
//					gpC_base->SendMessage(UI_LEARN_SLAYER_SKILL, id);
//				}
//			}
//			break;
//	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL_VIEW::IsPixel(int _x, int _y)
{
	return m_p_skill_view_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), SKILL_VIEW_BACK);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{

	int button_hilight = (gbl_vampire_interface)? 6:5;
	int button_pushed = 0;
	static char m_default_string[2][30] = 
	{
		"SkillInfo창을 닫습니다",
		"SkillInfo창의 도움말을 봅니다",
	};

	static char m_grade_string[2][30] = 
	{
		"이전 단계를 봅니다",
		"다음 단계를 봅니다",
	};

	static char m_skill_string[5][30] = 
	{
		"검 계열",
		"도 계열",
		"총/폭탄 계열",
		"회복 계열",
		"축복 계열",
	};

	static char m_magic_string[6][30] = 
	{
		"독 계열",
		"산 계열",
		"저주 계열",
		"소환 계열",
		"피 계열",
		"고유 계열",
	};

	if (p_button->GetFocusState())
	{
		switch(p_button->GetID())
		{
		case CLOSE_ID:
		case HELP_ID:
			g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, m_default_string[p_button->GetID() - CLOSE_ID]);

		case SCROLL_UP_ID:
		case SCROLL_DOWN_ID:
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			break;
			
		case GRADE_PREV_ID:
		case GRADE_NEXT_ID:
			if(p_button->GetPressState())
				m_p_skill_view_etc_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index);
			g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, m_grade_string[p_button->GetID() - GRADE_PREV_ID]);
			break;
			
		default:
			{
				if(gbl_vampire_interface)
				{
					g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, m_magic_string[p_button->GetID() - POISON_BUTTON_ID]);
				}
				else
				{
					static char temp_str[5][30];
					static LPSTR str[5] = { temp_str[0], temp_str[1], temp_str[2], temp_str[3], temp_str[4] };
					int domain = p_button->GetID() - SWORD_BUTTON_ID;

					strcpy(temp_str[0], m_skill_string[domain]);
					if(g_pSkillManager)
					{
						int level = (*g_pSkillManager)[temp_domain[domain]].GetDomainLevel();
						int exp = (*g_pSkillManager)[temp_domain[domain]].GetDomainExp();
//						int prev_exp;
//						if(level == 0)
//						{
//							prev_exp = 0;
//						}
//						else
//						{
//							prev_exp = (*g_pSkillManager)[temp_domain[domain]].GetExpInfo(level).AccumExp;
//						}

						if(level >= 0 && exp >= 0)
						{
							int next_exp = (*g_pSkillManager)[temp_domain[domain]].GetExpInfo(level).AccumExp;
							
							wsprintf(temp_str[1], "Level : %d", level);
							wsprintf(temp_str[2], "현재경험치 : %d", exp);
							wsprintf(temp_str[3], "다음  레벨 : %d", next_exp);
							wsprintf(temp_str[4], "남은경험치 : %d", next_exp - exp);
//							strcat(temp_str[2], "%");
							
							g_descriptor_manager.Set(DID_STRINGS, x+p_button->x, y+p_button->y, (void *)str, 5);//3);
						}
					}
					
					

				}

				if(p_button->GetPressState())
				{
					m_pC_skill_view_domain_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index+button_pushed);
					if(!gbl_vampire_interface)
					{
						RECT rect = {x+p_button->x, y+p_button->y, x+p_button->x + 69, y+p_button->y+m_domain_level[p_button->m_image_index]};
						if(rect.bottom > 0 && rect.right > 0 && rect.left < 800 && rect.top < 600)
							DrawAlphaBox(&rect, 0, 0, 0, 15);
					}
				}
				else
				{
					if(gbl_vampire_interface)
					{
						if(p_button->GetID() == m_iDomain + POISON_BUTTON_ID)
							m_pC_skill_view_domain_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index+button_hilight);
					}
					else
					{
						if(p_button->GetID() == m_iDomain + SWORD_BUTTON_ID)
							m_pC_skill_view_domain_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index+button_hilight);
						RECT rect = {x+p_button->x, y+p_button->y, x+p_button->x + 69, y+p_button->y+m_domain_level[p_button->m_image_index]};
						if(rect.bottom > 0 && rect.right > 0 && rect.left < 800 && rect.top < 600)
							DrawAlphaBox(&rect, 0, 0, 0, 15);
					}
				}
			}
			break;
		}
	}
	else if(p_button->GetID() == m_iDomain + SWORD_BUTTON_ID)
	{
		m_pC_skill_view_domain_spk->Blt(x+p_button->x, y+p_button->y, p_button->m_image_index+button_hilight);
		if(!gbl_vampire_interface)
		{
			RECT rect = {x+p_button->x, y+p_button->y, x+p_button->x + 69, y+p_button->y+m_domain_level[p_button->m_image_index]};
			if(rect.bottom > 0 && rect.right > 0 && rect.left < 800 && rect.top < 600)
				DrawAlphaBox(&rect, 0, 0, 0, 15);
		}
	}
	else if(p_button->GetID() >= POISON_BUTTON_ID && p_button->GetID() <= INNATE_BUTTON_ID)
	{
//		gpC_base->m_p_DDSurface_back->BltSprite(&p, &m_pC_skill_view_domain_spk[p_button->m_image_index]);
		if(!gbl_vampire_interface)
		{
			RECT rect = {x+p_button->x, y+p_button->y, x+p_button->x + 69, y+p_button->y+m_domain_level[p_button->m_image_index]};
			if(rect.bottom > 0 && rect.right > 0 && rect.left < 800 && rect.top < 600)
				DrawAlphaBox(&rect, 0, 0, 0, 15);
//			gpC_base->m_p_DDSurface_back->ChangeBrightnessBit(&rect, 1);
		}
	}
//	else
//		gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL_VIEW::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x;_y-=y;//창이동 가능하게 하기 위한
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch (message)
	{
	case M_MOVING:
		{
			if(gbl_vampire_interface && m_iGrade != 0)break;
			if(!((gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 6 && m_iScroll >= 0 && m_skill_domain == SKILLDOMAIN_VAMPIRE) || 
				(!gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 5 && m_iScroll >= 0 && (m_skill_domain == SKILLDOMAIN_SWORD || m_skill_domain == SKILLDOMAIN_BLADE || m_skill_domain == SKILLDOMAIN_GUN || m_skill_domain == SKILLDOMAIN_HEAL || m_skill_domain == SKILLDOMAIN_ENCHANT))))
				break;
			SKILL_STEP step = (SKILL_STEP)(gbl_vampire_interface?m_iDomain+SKILL_STEP_VAMPIRE_POISON:m_iGrade+SKILL_STEP_APPRENTICE);
			
			if(g_pSkillManager && (*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
				if(_x >= m_rcSkillDesciption.left && _y >= m_rcSkillDesciption.top && _x < m_rcSkillDesciption.right && _y < m_rcSkillDesciption.bottom)
				{
					int num = (_y - m_skill_y)/m_skill_y_gap*3 + (_x - m_skill_x)/m_skill_x_gap + m_iScroll*3;
					
					if(num >= (*((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step))).size())break;
					
					int id = ( *( (*g_pSkillManager)[m_skill_domain].GetSkillStepList( step ) ) )[num];
					
					g_descriptor_manager.Set(DID_SKILLTREE, x+_x, y+_y, (void *)&m_skill_domain, id, 0);
				}
		}
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			if((gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 6 && m_iScroll >= 0 && m_skill_domain == SKILLDOMAIN_VAMPIRE) || 
				(!gbl_vampire_interface && m_iDomain >=0 && m_iDomain <= 5 && m_iScroll >= 0 && (m_skill_domain == SKILLDOMAIN_SWORD || m_skill_domain == SKILLDOMAIN_BLADE || m_skill_domain == SKILLDOMAIN_GUN || m_skill_domain == SKILLDOMAIN_HEAL || m_skill_domain == SKILLDOMAIN_ENCHANT)))
			if(!gbl_vampire_interface || m_iGrade == 0)
			{
				SKILL_STEP step = (SKILL_STEP)(gbl_vampire_interface?m_iDomain+SKILL_STEP_VAMPIRE_POISON:m_iGrade+SKILL_STEP_APPRENTICE);
				
				if(g_pSkillManager && (*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
					if(_x >= m_rcSkillDesciption.left && _y >= m_rcSkillDesciption.top && _x < m_rcSkillDesciption.right && _y < m_rcSkillDesciption.bottom)
					{
						int num = (_y - m_skill_y)/m_skill_y_gap*3 + (_x - m_skill_x)/m_skill_x_gap + m_iScroll*3;
						
						if(num < (*((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step))).size())
						{
							
							int id = ( *( (*g_pSkillManager)[m_skill_domain].GetSkillStepList( step ) ) )[num];
							
							gC_vs_ui.RunDescDialog(DID_SKILL, (void *)id);
//							g_descriptor_manager.Set(DID_SKILLTREE, _x, _y, (void *)&m_skill_domain, id, 0);
							break;
						}
					}
			}
			
			if (gpC_mouse_pointer->GetPickUpItem() == false && re)
			{
				MoveReady();
				SetOrigin(_x, _y);
				break;
			}
				
		}
		break;

	case M_WHEEL_UP:
		Run(SCROLL_UP_ID);
		break;

	case M_WHEEL_DOWN:
		Run(SCROLL_DOWN_ID);
		break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::KeyboardControl(UINT message, UINT key, long extra)
{
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_VIEW::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);

	DeleteNew(m_p_skill_view_grade_spk);
	DeleteNew(m_p_skill_view_etc_spk);
	DeleteNew(m_p_skill_view_spk);
	DeleteNew(m_pC_skill_view_domain_spk);

// SKILL_ICON load
//	ifstream	file(SPK_SKILL_ICON, ios::binary|ios::nocreate);
//	if (!file)
//		_Error(FILE_OPEN);
//	C_VS_UI_SKILL::m_pC_spk.LoadFromFile(file);
//	file.close();


	m_p_skill_view_etc_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_ETC);
	if(gbl_vampire_interface)
	{
		m_p_skill_view_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE);
		m_p_skill_view_grade_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE_GRADE);
		m_pC_skill_view_domain_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_VAMPIRE_DOMAIN);
		// Domain_ICON load
//		ifstream	file(SPK_SKILLVIEW_VAMPIRE_DOMAIN, ios::binary|ios::nocreate);
//		if (!file)
//			_Error(FILE_OPEN);
//		m_pC_skill_view_domain_spk.LoadFromFile(file);
//		file.close();

	}
	else
	{
		m_p_skill_view_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER);
		m_p_skill_view_grade_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER_GRADE);
		m_pC_skill_view_domain_spk = new C_SPRITE_PACK(SPK_SKILLVIEW_SLAYER_DOMAIN);


		// Domain_ICON load
//		ifstream	file(SPK_SKILLVIEW_SLAYER_DOMAIN, ios::binary|ios::nocreate);
//		if (!file)
//			_Error(FILE_OPEN);
//		m_pC_skill_view_domain_spk.LoadFromFile(file);
//		file.close();
		
	}

	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL_TREE::Finish
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL_VIEW::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);

//	C_VS_UI_SKILL::m_pC_spk.Release();

	DeleteNew(m_pC_skill_view_domain_spk);
	DeleteNew(m_p_skill_view_grade_spk);
	DeleteNew(m_p_skill_view_etc_spk);
	DeleteNew(m_p_skill_view_spk);
}

*/