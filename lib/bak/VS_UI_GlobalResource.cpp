// VS_UI_GlobalResource.cpp

#include "VS_UI_GlobalResource.h"
#include "VS_UI_filepath.h"
#include "ClientFunction.h"
#include "UserOption.h"
extern BOOL g_MyFull;
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
C_GLOBAL_RESOURCE	*	gpC_global_resource;
bool	gbl_wood_skin;

//-----------------------------------------------------------------------------
// Operations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// C_GLOBAL_RESOURCE
//
// Load all resources.
//-----------------------------------------------------------------------------
C_GLOBAL_RESOURCE::C_GLOBAL_RESOURCE()
{
	//LoadAniButton();
//	LoadAssembleBox();
	//LoadCommonImage();
//	LoadCommonButton();
	m_pC_assemble_box_spk = NULL;
	m_pC_assemble_box_button_spk = NULL;
//	m_pC_assemble_box_etc_spk = NULL;
	m_pC_scroll_bar_spk = NULL;
	m_pC_box_spk = NULL;
	m_pC_info_spk = NULL;
	LoadAssemble();
}

//-----------------------------------------------------------------------------
// ~C_GLOBAL_RESOURCE
//
// Free all resources.
//-----------------------------------------------------------------------------
C_GLOBAL_RESOURCE::~C_GLOBAL_RESOURCE()
{
	//FreeAniButton();
//	FreeAssembleBox();
	//FreeCommonImage();
//	FreeCommonButton();
	//FreeCommonInTitle();
	FreeAssemble();
}

//-----------------------------------------------------------------------------
// LoadAssemble
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::LoadAssemble()
{
	FreeAssemble();
//	m_pC_assemble_box_button_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_BUTTON);
	if(g_MyFull)
	{
		m_pC_scroll_bar_spk = new C_SPRITE_PACK(SPK_SCROLL_BAR_1024);
	}
	else
	{
		m_pC_scroll_bar_spk = new C_SPRITE_PACK(SPK_SCROLL_BAR);
	}
	m_pC_box_spk = new C_SPRITE_PACK(SPK_BOX);
//	if (gbl_wood_skin)
//	{
//		m_pC_assemble_box_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_COMMON);
//		m_pC_assemble_box_button_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_BUTTON_COMMON);
//		m_pC_assemble_box_etc_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_ETC);
//	}
//	else
//	{
		if (gbl_vampire_interface)
		{
			m_pC_assemble_box_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_VAMPIRE);
			m_pC_assemble_box_button_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_BUTTON_VAMPIRE);
			m_pC_info_spk = new C_SPRITE_PACK(SPK_INFO_VAMPIRE);
//			m_pC_assemble_box_etc_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_ETC_VAMPIRE);
		}
		else
		{
			m_pC_info_spk = new C_SPRITE_PACK(SPK_INFO_SLAYER);
			m_pC_assemble_box_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_SLAYER);
			m_pC_assemble_box_button_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_BUTTON_SLAYER);
//			m_pC_assemble_box_etc_spk = new C_SPRITE_PACK(SPK_ASSEMBLE_BOX_ETC);
		}
//	}
}

//-----------------------------------------------------------------------------
// FreeAssemble
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::FreeAssemble()
{
	if(m_pC_box_spk)
	{
		DeleteNew(m_pC_box_spk);
		m_pC_box_spk = NULL;
	}
	if(m_pC_scroll_bar_spk)
	{
		DeleteNew(m_pC_scroll_bar_spk);
		m_pC_scroll_bar_spk = NULL;
	}
	if(m_pC_assemble_box_spk)
	{
		DeleteNew(m_pC_assemble_box_spk);
		m_pC_assemble_box_spk = NULL;
	}
	if(m_pC_assemble_box_button_spk)
	{
		DeleteNew(m_pC_assemble_box_button_spk);
		m_pC_assemble_box_button_spk = NULL;
	}
	if(m_pC_info_spk)
	{
		DeleteNew(m_pC_info_spk);
		m_pC_info_spk = NULL;		
	}
//	if(m_pC_assemble_box_etc_spk)
//	{
//		DeleteNew(m_pC_assemble_box_etc_spk);
//		m_pC_assemble_box_etc_spk = NULL;
//	}
}

//-----------------------------------------------------------------------------
// Load/Free CommonButton
//
// 
//-----------------------------------------------------------------------------
//void C_GLOBAL_RESOURCE::LoadCommonButton()
//{
//	m_pC_common_button = new C_SPRITE_PACK(SPK_COMMON_BUTTON);
//}
//
//void C_GLOBAL_RESOURCE::FreeCommonButton()
//{
//	DeleteNew(m_pC_common_button);
//}

//-----------------------------------------------------------------------------
// Load/Free CommonInTitle
//
// 
//-----------------------------------------------------------------------------
/*
void C_GLOBAL_RESOURCE::LoadCommonInTitle()
{
	m_pC_common_in_title = new C_SPRITE_PACK(SPK_COMMON_IN_TITLE);
}

void C_GLOBAL_RESOURCE::FreeCommonInTitle()
{
	DeleteNew(m_pC_common_in_title);
}*/
/*
//-----------------------------------------------------------------------------
// LoadAniButton
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::LoadAniButton()
{
	m_pC_ao_ani_button = new C_ANI_OBJECT(SPK_BUTTON1, FRR_BUTTON1);
}

//-----------------------------------------------------------------------------
// FreeAniButton
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::FreeAniButton()
{
	DeleteNew(m_pC_ao_ani_button);
}
*/
//-----------------------------------------------------------------------------
// LoadAssembleBox
//
// 조립박스 load.
//-----------------------------------------------------------------------------
/*void C_GLOBAL_RESOURCE::LoadAssembleBox()
{
	m_pic_box_back = new Tga;
	if (!m_pic_box_back->Load(IMAGE_BOXBACK))
		_Error(FAILED_JOB);

	m_pC_box_line_spk = new C_SPRITE_PACK(SPK_BOX_LINE);

	m_pC_ok_spk = new C_SPRITE_PACK(SPK_OK);
}*/

//-----------------------------------------------------------------------------
// FreeAssembleBox
//
// 
//-----------------------------------------------------------------------------
/*void C_GLOBAL_RESOURCE::FreeAssembleBox()
{
	DeleteNew(m_pC_ok_spk);
	DeleteNew(m_pC_box_line_spk);
	DeleteNew(m_pic_box_back);
}
*/
/*
//-----------------------------------------------------------------------------
// LoadCommonImage
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::LoadCommonImage()
{
	m_pC_common_spk = new C_SPRITE_PACK(SPK_COMMON);
}

//-----------------------------------------------------------------------------
// FreeCommonImage
//
// 
//-----------------------------------------------------------------------------
void C_GLOBAL_RESOURCE::FreeCommonImage()
{
	DeleteNew(m_pC_common_spk);
}*/

void C_GLOBAL_RESOURCE::DrawOutBox(Rect &rect)
{
	DrawOutBox(rect.x, rect.y, rect.w, rect.h);
}

void C_GLOBAL_RESOURCE::DrawOutBox(int x, int y, int w, int h)
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		DrawOutBoxLocked(x, y, w, h);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_GLOBAL_RESOURCE::DrawOutBoxLocked(Rect &rect)
{
	DrawOutBoxLocked(rect.x, rect.y, rect.w, rect.h);
}


void C_GLOBAL_RESOURCE::DrawOutBoxLocked(int x, int y, int w, int h)
{
	Rect rect;

	rect.Set(0, 0, w-m_pC_box_spk->GetWidth(OB_RIGHT), h-m_pC_box_spk->GetHeight(OB_DOWN));
	m_pC_box_spk->BltLockedClip(x, y, rect, OB_MAIN);

	rect.Set(0, 0, m_pC_box_spk->GetWidth(OB_RIGHT), h);
	m_pC_box_spk->BltLockedClip(x+w-m_pC_box_spk->GetWidth(OB_RIGHT), y, rect, OB_RIGHT);

	rect.Set(0, 0, w, m_pC_box_spk->GetHeight(OB_DOWN));
	m_pC_box_spk->BltLockedClip(x, y+h-m_pC_box_spk->GetHeight(OB_DOWN), rect, OB_DOWN);

	m_pC_box_spk->BltLocked(x+w-m_pC_box_spk->GetWidth(OB_RIGHTDOWN), y+h-m_pC_box_spk->GetHeight(OB_RIGHTDOWN), OB_RIGHTDOWN);

}


void C_GLOBAL_RESOURCE::DrawDialog(Rect &rect, bool alpha)
{
	DrawDialog(rect.x, rect.y, rect.w, rect.h, alpha);
}

void C_GLOBAL_RESOURCE::DrawDialog(int x, int y, int w, int h, bool alpha)
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		DrawDialogLocked(x, y, w, h, alpha);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_GLOBAL_RESOURCE::DrawDialogLocked(Rect &rect, bool alpha)
{
	DrawDialogLocked(rect.x, rect.y, rect.w, rect.h, alpha);
}


void C_GLOBAL_RESOURCE::DrawDialogLocked(int x, int y, int w, int h, bool alpha)
{
	Rect rect;

	rect.Set(0, 0, w-2, h-2);
	if(alpha)
	{
		RECT alpha_rect;
		if(gbl_vampire_interface)
		{
			SetRect(&alpha_rect, x+10, y+9, x+rect.w-7, y+rect.h-8);
			DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
		}
		else
		{
			SetRect(&alpha_rect, x+5, y+12, x+rect.w-4, y+rect.h-10);
			DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
		}
		m_pC_assemble_box_spk->BltLockedClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK_ALPHA);
	}
	else
	{
		m_pC_assemble_box_spk->BltLockedClip(x, y, rect, AB_BACK);
	}

	rect.Set(0, 0, m_pC_assemble_box_spk->GetWidth(AB_RIGHT), h);
	m_pC_assemble_box_spk->BltLockedClip(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHT), y, rect, AB_RIGHT);

	rect.Set(0, 0, w, m_pC_assemble_box_spk->GetHeight(AB_DOWN));
	m_pC_assemble_box_spk->BltLockedClip(x, y+h-m_pC_assemble_box_spk->GetHeight(AB_DOWN), rect, AB_DOWN);

	m_pC_assemble_box_spk->BltLocked(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHTDOWN), y+h-m_pC_assemble_box_spk->GetHeight(AB_RIGHTDOWN), AB_RIGHTDOWN);

}


void C_GLOBAL_RESOURCE::DrawDialogOnly(Rect &rect)
{
	DrawDialogOnly(rect.x, rect.y, rect.w, rect.h);
}

void C_GLOBAL_RESOURCE::DrawDialogOnly(int x, int y, int w, int h)
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		DrawDialogOnlyLocked(x, y, w, h);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_GLOBAL_RESOURCE::DrawDialogOnlyLocked(Rect &rect)
{
	DrawDialogOnlyLocked(rect.x, rect.y, rect.w, rect.h);
}


void C_GLOBAL_RESOURCE::DrawDialogOnlyLocked(int x, int y, int w, int h)
{
	Rect rect;

	rect.Set(0, 0, w-2, h-2);

	m_pC_assemble_box_spk->BltLockedClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK_ALPHA);

	rect.Set(0, 0, m_pC_assemble_box_spk->GetWidth(AB_RIGHT), h);
	m_pC_assemble_box_spk->BltLockedClip(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHT), y, rect, AB_RIGHT);

	rect.Set(0, 0, w, m_pC_assemble_box_spk->GetHeight(AB_DOWN));
	m_pC_assemble_box_spk->BltLockedClip(x, y+h-m_pC_assemble_box_spk->GetHeight(AB_DOWN), rect, AB_DOWN);

	m_pC_assemble_box_spk->BltLocked(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHTDOWN), y+h-m_pC_assemble_box_spk->GetHeight(AB_RIGHTDOWN), AB_RIGHTDOWN);

}


// dialog2

void C_GLOBAL_RESOURCE::DrawDialog2(Rect &rect, bool alpha)
{
	DrawDialog2(rect.x, rect.y, rect.w, rect.h, alpha);
}

void C_GLOBAL_RESOURCE::DrawDialog2(int x, int y, int w, int h, bool alpha)
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		DrawDialogLocked2(x, y, w, h, alpha);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_GLOBAL_RESOURCE::DrawDialogLocked2(Rect &rect, bool alpha)
{
	DrawDialogLocked2(rect.x, rect.y, rect.w, rect.h, alpha);
}


void C_GLOBAL_RESOURCE::DrawDialogLocked2(int x, int y, int w, int h, bool alpha)
{
	Rect rect;

	rect.Set(0, 0, w-2, h-2);
	if(alpha)
	{
		RECT alpha_rect;
		if(gbl_vampire_interface)
		{
			SetRect(&alpha_rect, x+2, y+2, x+rect.w-2, y+rect.h-7);
			DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
		}
		else
		{
			SetRect(&alpha_rect, x+2, y+2, x+rect.w-2, y+rect.h-7);
			DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
		}
		m_pC_assemble_box_spk->BltLockedClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK_ALPHA2);
	}
	else
	{
		m_pC_assemble_box_spk->BltLockedClip(x, y, rect, AB_BACK2);
	}

	rect.Set(0, 0, m_pC_assemble_box_spk->GetWidth(AB_RIGHT2), h);
	m_pC_assemble_box_spk->BltLockedClip(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHT2), y, rect, AB_RIGHT2);

	rect.Set(0, 0, w, m_pC_assemble_box_spk->GetHeight(AB_DOWN2));
	m_pC_assemble_box_spk->BltLockedClip(x, y+h-m_pC_assemble_box_spk->GetHeight(AB_DOWN2), rect, AB_DOWN2);

	m_pC_assemble_box_spk->BltLocked(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHTDOWN2), y+h-m_pC_assemble_box_spk->GetHeight(AB_RIGHTDOWN2), AB_RIGHTDOWN2);

}


void C_GLOBAL_RESOURCE::DrawDialogOnly2(Rect &rect)
{
	DrawDialogOnly2(rect.x, rect.y, rect.w, rect.h);
}

void C_GLOBAL_RESOURCE::DrawDialogOnly2(int x, int y, int w, int h)
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		DrawDialogOnlyLocked2(x, y, w, h);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_GLOBAL_RESOURCE::DrawDialogOnlyLocked2(Rect &rect)
{
	DrawDialogOnlyLocked2(rect.x, rect.y, rect.w, rect.h);
}


void C_GLOBAL_RESOURCE::DrawDialogOnlyLocked2(int x, int y, int w, int h)
{
	Rect rect;

	rect.Set(0, 0, w-2, h-2);

	m_pC_assemble_box_spk->BltLockedClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK_ALPHA2);

	rect.Set(0, 0, m_pC_assemble_box_spk->GetWidth(AB_RIGHT2), h);
	m_pC_assemble_box_spk->BltLockedClip(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHT2), y, rect, AB_RIGHT2);

	rect.Set(0, 0, w, m_pC_assemble_box_spk->GetHeight(AB_DOWN2));
	m_pC_assemble_box_spk->BltLockedClip(x, y+h-m_pC_assemble_box_spk->GetHeight(AB_DOWN2), rect, AB_DOWN2);

	m_pC_assemble_box_spk->BltLocked(x+w-m_pC_assemble_box_spk->GetWidth(AB_RIGHTDOWN2), y+h-m_pC_assemble_box_spk->GetHeight(AB_RIGHTDOWN2), AB_RIGHTDOWN2);

}