#include "client_PCH.h"

#include "VS_UI_progress.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "UserInformation.h"
#include "VS_UI.h"
#include <time.h>

#include "MGameStringTable.H"

#define PROGRESS_MAX 25
extern	BOOL g_MyFull;

//-----------------------------------------------------------------------------
// C_VS_UI_PROGRESS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PROGRESS::C_VS_UI_PROGRESS()
{
	g_RegisterWindow(this);

	AttrTopmost(true);

	srand(time(0));

	static bool Check[PROGRESS_MAX+25];
	static int	CheckNum = 0;
	int num;
	int primium_plus = 25;
	
	if( g_pUserInformation->bChinese )
		primium_plus = 0;

//	if(g_pUserInformation->IsNetmarble)
//		primium_plus = 0;

	if(CheckNum == 0)
	{
		for(int i = 0; i < PROGRESS_MAX+primium_plus; i++)
			Check[i] = false;
		CheckNum = PROGRESS_MAX+primium_plus;

		if(g_pUserInformation->GoreLevel == false)
		{
			// 틴버전 하드코딩
			Check[4] = true;
			Check[9] = true;
			Check[11] = true;
			Check[13] = true;
			Check[14] = true;
			Check[15] = true;
			Check[21] = true;
			Check[22] = true;
			Check[23] = true;
			
			CheckNum -= 9;
		}
		
	}
	
	num = rand()%CheckNum;
	for(int i = 0; i <= num; i++)
	{
		if(Check[i] == true)num++;
	}
	Check[num] = true;
	CheckNum--;
	
	m_pC_progress = NULL;
	m_pC_event_progress = NULL;
	m_pC_character = NULL;
	// add by coffee 2006-12-21
	//num=PROGRESS_MAX;
	// end 2006-12-21
	if(num >= PROGRESS_MAX)
	{
		m_Event_Num = 0;
		if (g_MyFull)
		{
			m_pC_event_progress = new C_SPRITE_PACK(SPK_PROGRESS_EVENT_1024);
		}
		else
		{
			m_pC_event_progress = new C_SPRITE_PACK(SPK_PROGRESS_EVENT);
		}

		m_Event_Num = (rand() % m_pC_event_progress->GetSize())/2;
		Set(0, 0, m_pC_event_progress->GetWidth(), m_pC_event_progress->GetHeight());
	}
	else
	{
		m_pC_progress = new CSpritePack;
		char* pSpk_progressPathc;
		if(g_MyFull)
		{
			pSpk_progressPathc =SPK_PROGRESS_1024;
			//ifstream progress_file(SPK_PROGRESS_1024, ios::binary);
		}
		else
		{
			pSpk_progressPathc =SPK_PROGRESS;
			//ifstream progress_file(SPK_PROGRESS, ios::binary);
		}
		ifstream progress_file(pSpk_progressPathc, ios::binary);
		m_pC_progress->LoadFromFile(progress_file);
		progress_file.close();
		
		m_pC_character = new CSpritePack;
		
		m_pC_character->Init(2);
		m_pC_character->LoadFromFileData(0, num*2, SPK_PROGRESS_CHARACTER, SPKI_PROGRESS_CHARACTER);
		m_pC_character->LoadFromFileData(1, num*2+1, SPK_PROGRESS_CHARACTER, SPKI_PROGRESS_CHARACTER);
		
		m_pack_file.SetRAR(RPK_PROGRESS, RPK_PASSWORD);
		
		PrintInfo pi, title_pi;
		COLORREF color, title_color;
		
		title_pi = gpC_base->m_char_value_pi;
		title_color = RGB(178,140,121);
		pi = gpC_base->m_char_name_pi;
		color = RGB(200,200,200);
		
		static char *Monster[PROGRESS_MAX] =
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_TNDEAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_ARKHAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_ESTROIDER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_GOLEMER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_DARKSCREAMER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_DEADBODY].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_MODERAS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_VANDALIZER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_DIRTYSTRIDER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_HELLWIZARD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_TNSOUL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_IRONTEETH].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_REDEYE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_CRIMSONSLAUGTHER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_HELLGUARDIAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_SOLDIER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_RIPPER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_BIGFANG].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_LORDCHAOS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_CHAOSGUARDIAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_HOBBLE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_CHAOSNIGHT].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_WIDOWS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_KID].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_NAME_SHADOWWING].GetString(),
		};
		
		static char *MonsterFilename[PROGRESS_MAX] =
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_TNDEAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_ARKHAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_ESTROIDER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_GOLEMER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_DARKSCREAMER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_DEADBODY].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_MODERAS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_VANDALIZER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_DIRTYSTRIDER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_HELLWIZARD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_TNSOUL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_IRONTEETH].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_REDEYE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_CRIMSONSLAUGTHER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_HELLGUARDIAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_SOLDIER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_RIPPER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_BIGFANG].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_LORDCHAOS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_CHAOSGUARDIAN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_HOBBLE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_CHAOSNIGHT].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_WIDOWS].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_KID].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_MONSTER_ENAME_SHADOWWING].GetString(),
		};
		
		std::string filename = MonsterFilename[num];
		filename += ".txt";
		LoadDesc(filename.c_str(), 43, 50, false);
		
		SetDesc(400, 150, color, pi);
		SetDescTitle(400, 120, title_color, title_pi);
		SetDescTitle(Monster[num]);

		Set(0, 0, (*m_pC_progress)[BACK].GetWidth(), (*m_pC_progress)[BACK].GetHeight());
	}
	

	m_world_name = "";
	m_server_name = "";
	m_server_status = C_VS_UI_SERVER_SELECT::STATUS_DOWN;

//	m_bar_width = m_pC_progress->GetWidth(BAR);
//	m_bar_height = m_pC_progress->GetHeight(BAR);

//	assert(m_bar_width > 0 && m_bar_height > 0);

//	m_p_bar_buf = new WORD[m_bar_width*m_bar_height];

	// bar image를 offscreen에 blt한다.
//	gpC_base->m_DDSurface_offscreen.FillSurface(BAR_COLORKEY);
//	m_pC_progress->BltOffscreen(0, 0, BAR);

	// blt from offscreen
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO	src_surface, dest_surface;
//		S_RECT			src_rect, dest_rect;
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//		SetSurfaceInfo(dest_surface, m_p_bar_buf, m_bar_width, m_bar_height);
//		SetRect(src_rect, 0, 0, m_bar_width, m_bar_height);
//		SetRect(dest_rect, 0, 0, m_bar_width, m_bar_height);
//
//		Bltz(&dest_surface, &src_surface, &dest_rect, &src_rect);
//
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_PROGRESS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PROGRESS::~C_VS_UI_PROGRESS()
{
	g_UnregisterWindow(this);

	if(m_pC_event_progress != NULL)
		DeleteNew(m_pC_event_progress);

	if(m_pC_progress != NULL)
		DeleteNew(m_pC_progress);
	
	if(m_pC_character != NULL)
		DeleteNew(m_pC_character);
//	DeleteNewArray(m_p_bar_buf);
}

//-----------------------------------------------------------------------------
// C_VS_UI_PROGRESS::SetProgressStyle
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_PROGRESS::SetProgressStyle(PROGRESS_SPK_INDEX style)
//{
//	if (style >= BAR)
//	{
//		style = SLAYER_PROGRESS_1;
//	}

//	m_progress_back_image = style; 

	/*
	if (m_progress_back_image == VAMPIRE_PROGRESS)
	{
		int index = rand()%VAMPIRE_RAND_MAX;
		if (index > 0)
		{
			DeleteNew(m_pC_progress);
			m_pC_progress = new C_SPRITE_PACK(SPK_PROGRESS_VAMPIRE_RAND);

			m_progress_back_image = index-1;
		}
	}
	*/
//}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_PROGRESS::IsPixel(int _x, int _y)
{
//	return m_pC_progress->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), SLAYER_PROGRESS_1);
	return true;//(*m_pC_progress)[BACK].IsColorPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PROGRESS::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PROGRESS::Start()
{
	PI_Processor::Start();

	gpC_window_manager->AppearWindow(this);

	m_world_name = "";
	m_server_name = "";
	m_server_status = C_VS_UI_SERVER_SELECT::STATUS_DOWN;


//	m_cur_line = 0;
	m_percent = 0;
//	m_progress_back_image = SLAYER_PROGRESS_1;

	g_descriptor_manager.Unset();
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PROGRESS::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// SetProgress
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PROGRESS::SetProgress(WORD cur_val, WORD max_val)
{
	if (max_val == 0 || cur_val > max_val)
		return;

	if (max_val > 0)
	{
		float f = cur_val / (float)max_val;

		m_percent = (WORD)(f*100);

//		f = (m_percent / 100.) * TOTAL_BAR_LINE;

//		WORD n = f;
//		float t = n;
//		float r = f - t;
//		if (r > 0.)
//			n += 1; // 반올림.
//
//		if (m_percent < 100 && n >= TOTAL_BAR_LINE)
//			n = TOTAL_BAR_LINE - 1;
//
//		m_cur_line = n;
	}
}

//-----------------------------------------------------------------------------
// DrawBar (Based on Vampire Energy interface)
//
//
//-----------------------------------------------------------------------------
/*
void C_VS_UI_PROGRESS::DrawBar()
{
	if (m_cur_line < 1 || m_cur_line > TOTAL_BAR_LINE)
		return;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	src_surface, dest_surface;
		S_RECT			src_rect, dest_rect;
		SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
		SetSurfaceInfo(src_surface, m_p_bar_buf, m_bar_width, m_bar_height);
		SetRect(src_rect, 0, 0, m_cur_line, m_bar_height);
		SetRect(dest_rect, x+BAR_GAP_X, y+BAR_GAP_Y, m_cur_line, m_bar_height);

		CkBltz(&dest_surface, &src_surface, &dest_rect, &src_rect, BAR_COLORKEY);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}
*/

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PROGRESS::Show()
{

	gpC_base->m_p_DDSurface_back->FillSurface(0);

	POINT p = {0, 0};

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if(m_pC_event_progress != NULL)	// EVENT_PROGRESS
		{
			// 하드코딩
			Rect rect;
			m_pC_event_progress->BltLocked(0, 0, (m_Event_Num*2));
			rect.Set(0, m_pC_event_progress->GetHeight(m_Event_Num)*(100-m_percent)/100, m_pC_event_progress->GetWidth(m_Event_Num), m_pC_event_progress->GetHeight(m_Event_Num)*(m_percent)/100);
			m_pC_event_progress->BltLockedClip(0, 0, rect, (m_Event_Num*2)+1);
		}
		else
		{
			//BACK찍기
			gpC_base->m_p_DDSurface_back->BltSprite(&p, &(*m_pC_progress)[BACK]);
			
			//캐릭터 찍기
			p.x = 200 - (*m_pC_character)[0].GetWidth()/2;
			p.y = 300 - (*m_pC_character)[0].GetHeight()/2;
			gpC_base->m_p_DDSurface_back->s_Value1 = m_percent*32/100; 	// 0이면 완전 gray, 32이면 원래색깔
			
			gpC_base->m_p_DDSurface_back->SetEffect( CSpriteSurface::EFFECT_GRAY_SCALE );//EFFECT_NET);//
			
			
			Rect rect;
			RECT rt;
			S_SURFACEINFO surface_info;
			SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
			
			rect.Set(0, 0, (*m_pC_character)[0].GetWidth(), (*m_pC_character)[0].GetHeight()*(100-m_percent)/100);
			rt.left = max(-p.x, rect.x);
			rt.top = max(-p.y, rect.y);
			// add by Sonic 2006.9.26
			if(g_MyFull)
			{
				rt.right = min(rect.x+rect.w, 1024-p.x);
				rt.bottom = min(rect.y+rect.h, 768-p.y);
			}
			else
			{
				rt.right = min(rect.x+rect.w, 800-p.x);
				rt.bottom = min(rect.y+rect.h, 600-p.y);
			}
			// end
			if(rt.left < rt.right && rt.top < rt.bottom)
			{
				WORD * p_dest = (WORD *)surface_info.p_surface+p.x+rt.left;
				p_dest = (WORD *)((BYTE *)p_dest+(p.y+rt.top)*surface_info.pitch);
				
				(*m_pC_character)[0].BltEffectClipHeight(p_dest, surface_info.pitch, &rt);
			}
			
			SetRect((RECT *)&rect, 0, (*m_pC_character)[0].GetHeight()*(100-m_percent)/100, (*m_pC_character)[0].GetWidth(), (*m_pC_character)[0].GetHeight() - (*m_pC_character)[0].GetHeight()*(100-m_percent)/100);
			rt.left = max(-p.x, rect.x);
			rt.top = max(-p.y, rect.y);
			// add by Sonic 2006.9.26
			if(g_MyFull)
			{
				rt.right = min(rect.x+rect.w, 1024-p.x);
				rt.bottom = min(rect.y+rect.h, 768-p.y);
			}
			else
			{
				rt.right = min(rect.x+rect.w, 800-p.x);
				rt.bottom = min(rect.y+rect.h, 600-p.y);
			}
			// end
			if(rt.left < rt.right && rt.top < rt.bottom)
			{
				
				WORD * p_dest = (WORD *)surface_info.p_surface+p.x+rt.left;
				p_dest = (WORD *)((BYTE *)p_dest+(p.y+rt.top)*surface_info.pitch);
				
				(*m_pC_character)[0].BltClipHeight(p_dest, surface_info.pitch, &rt);
			}
			
			
			p.x = 200 - (*m_pC_progress)[PROGRESS_BAR].GetWidth()/2;
			p.y += (*m_pC_character)[0].GetHeight()*(100-m_percent)/100-(*m_pC_progress)[PROGRESS_BAR].GetHeight()/2;
			gpC_base->m_p_DDSurface_back->SetEffect( CSpriteSurface::EFFECT_SCREEN );
			gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &(*m_pC_progress)[PROGRESS_BAR]);
			
			
			//캐릭터이름찍기(하단큰거)
			// add by Sonic 2006.9.26
			if(g_MyFull)
			{
				p.x = 1024 - (*m_pC_progress)[CHAR_NAME_BACK].GetWidth()-10;
				p.y = 768 - (*m_pC_progress)[CHAR_NAME_BACK].GetHeight()-45;
				gpC_base->m_p_DDSurface_back->SetEffect( CSpriteSurface::EFFECT_GRAY_SCALE_VARIOUS );
				gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &(*m_pC_progress)[CHAR_NAME_BACK]);
				
				p.x = 1024 - (*m_pC_progress)[CHAR_NAME_BACK].GetWidth()/2 -(*m_pC_character)[1].GetWidth()/2-10;
				p.y = 768 - (*m_pC_progress)[CHAR_NAME_BACK].GetHeight()/2 -(*m_pC_character)[1].GetHeight()/2-25;
				gpC_base->m_p_DDSurface_back->BltSprite(&p, &(*m_pC_character)[1]);
			}
			else
			{
				p.x = 800 - (*m_pC_progress)[CHAR_NAME_BACK].GetWidth()-10;
				p.y = 600 - (*m_pC_progress)[CHAR_NAME_BACK].GetHeight()-45;
				gpC_base->m_p_DDSurface_back->SetEffect( CSpriteSurface::EFFECT_GRAY_SCALE_VARIOUS );
				gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &(*m_pC_progress)[CHAR_NAME_BACK]);
				
				p.x = 800 - (*m_pC_progress)[CHAR_NAME_BACK].GetWidth()/2 -(*m_pC_character)[1].GetWidth()/2-10;
				p.y = 600 - (*m_pC_progress)[CHAR_NAME_BACK].GetHeight()/2 -(*m_pC_character)[1].GetHeight()/2-25;
				gpC_base->m_p_DDSurface_back->BltSprite(&p, &(*m_pC_character)[1]);

			}
			// end
		}
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	//현재 서버이름 표시
	COLORREF focus_color;
	focus_color = RGB_WHITE;
	if(m_pC_event_progress != NULL)
		focus_color = RGB_BLACK;
	
	std::string name = GetWorldName();
	name += " - ";
	name += GetServerName();
	name += " (";

	
	switch(GetServerStatus())
	{
	case C_VS_UI_SERVER_SELECT::STATUS_VERY_GOOD:
		name +=(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_VERY_GOOD].GetString();
		break;
		
	case C_VS_UI_SERVER_SELECT::STATUS_GOOD:
		name +=(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_GOOD].GetString();
		break;
		
	case C_VS_UI_SERVER_SELECT::STATUS_NORMAL:
		name +=(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_NORMAL].GetString();
		break;
		
	case C_VS_UI_SERVER_SELECT::STATUS_BAD:
		name +=(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_BAD].GetString();
		break;
		
	case C_VS_UI_SERVER_SELECT::STATUS_VERY_BAD:
		name += (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_VERY_BAD].GetString();
		break;
		
	case C_VS_UI_SERVER_SELECT::STATUS_DOWN:
		name += (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_DOWN].GetString();
		break;
	}
	name += ")";

	g_PrintColorStr(400 - g_GetStringWidth(name.c_str(), gpC_base->m_dialog_menu_pi.hfont)/2, 520, name.c_str(), gpC_base->m_desc_menu_pi, focus_color);//by larosel

	if(m_pC_event_progress == NULL)
	{
		ShowDesc(x, y);
		
		p.x = m_desc_title_x + g_GetStringWidth(m_desc_title.c_str())/2 - (*m_pC_progress)[CHAR_NAME_ALPHA].GetWidth()/2;
		//캐릭터 이름위에 알파
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			p.y = m_desc_title_y-4;
			gpC_base->m_p_DDSurface_back->SetEffect( CSpriteSurface::EFFECT_SCREEN );
			gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &(*m_pC_progress)[CHAR_NAME_ALPHA]);
			
			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}

//	DrawBar();

	// percent 출력
#ifndef _LIB
//	char str[5];
//	sprintf(str, "%d%%", m_percent);
//	g_Print(380, 500, str);
#endif

}
