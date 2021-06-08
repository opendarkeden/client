// VS_UI_Util.cpp

#include "client_PCH.h"
#include <assert.h>
#include <fstream.h>
#include <stdlib.h>
#include "VS_UI_base.h"
#include "VS_UI_util.h"

extern	BOOL g_MyFull;
//#define TEMP_FILE			"_000_TEMP"

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Read_rbt (FrameArray)
//
// 
//-----------------------------------------------------------------------------
//void Read_rbt(const char * rbt_file, C_SPRITE_FRAME_ARRAY * p_sf_array, C_CONVERT_TABLE * p_ct)
//{
//	assert(rbt_file);
//	assert(p_sf_array);
//	assert(p_ct);
//
//	RBT rbt;
//
//	if (rbt.Open(rbt_file) != RBT_OK)
//		return;
//
//	for (data_count_t i = 0; i < rbt.Size(); i++)
//	{
//		RBT::DATA_HEAD head;
//		rbt.GetDataHead(i, head);
//
//		int size = head.size;
//
//		assert(size > 0);
//
//		char * buf = new char[size];
//
//		rbt_result_t ret = rbt.ReadSequence(i, buf, size);
//		if (ret == RBT_DATA_ID_NOT_FOUNT)
//		{
//			DeleteNewArray(buf);
//			continue;
//		}
//		else if (ret != RBT_OK)
//		{
//			DeleteNewArray(buf);
//			_Error(FAILED_JOB);
//		}
//
//		ofstream spr_file(TEMP_FILE, ios::binary);
//		if (!spr_file)
//		{
//			DeleteNewArray(buf);
//			return;
//		}
//		spr_file.write(buf, size);
//		spr_file.close();
//
//		C_SPRITE_FRAME sprite_frame;
//		sprite_frame.Open(TEMP_FILE);
//
//		p_sf_array->AddSpriteFrame(&sprite_frame);
//
//		// set table
//		p_ct->Add(head.id, i);
//
//		remove(TEMP_FILE);
//		DeleteNewArray(buf);
//	}
//}

//-----------------------------------------------------------------------------
// Read_rbt (SpritePackList)
//
// 
//-----------------------------------------------------------------------------
//void Read_rbt(const char * rbt_file, CSpritePackList * p_spk_list, C_CONVERT_TABLE * p_ct)
//{
//	assert(rbt_file);
//	assert(p_spk_list);
//	assert(p_ct);
//
//	RBT rbt;
//
//	if (rbt.Open(rbt_file) != RBT_OK)
//		return;
//
//	for (data_count_t i = 0; i < rbt.Size(); i++)
//	{
//		RBT::DATA_HEAD head;
//		rbt.GetDataHead(i, head);
//
//		int size = head.size;
//
//		assert(size > 0);
//
//		char * buf = new char[size];
//
//		rbt_result_t ret = rbt.ReadSequence(i, buf, size);
//		if (ret == RBT_DATA_ID_NOT_FOUNT)
//		{
//			DeleteNewArray(buf);
//			continue;
//		}
//		else if (ret != RBT_OK)
//		{
//			DeleteNewArray(buf);
//			_Error(FAILED_JOB);
//		}
//
//		ofstream spr_file(TEMP_FILE, ios::binary);
//		if (!spr_file)
//		{
//			DeleteNewArray(buf);
//			return;
//		}
//		spr_file.write(buf, size);
//		spr_file.close();
//
//		ifstream spr_file2(TEMP_FILE, ios::binary);
//		if (!spr_file2)
//		{
//			DeleteNewArray(buf);
//			return;
//		}
//
//		CSprite * p_sprite;
//		if (CDirectDraw::Is565())
//			p_sprite = new CSprite565;
//		else
//			p_sprite = new CSprite555;
//
//		if (p_sprite->LoadFromFile(spr_file2) == false)
//			_Error(FAILED_JOB);
//
//		p_spk_list->AddSprite(p_sprite);
//
//		// set table
//		p_ct->Add(head.id, i);
//
//		spr_file2.close();
//		remove(TEMP_FILE);
//		DeleteNewArray(buf);
//	}
//}

//SPRITE_ID C_CONVERT_TABLE::Index(data_id_t id) const
//{
//	assert(m_p_unit_list);
//
//	for (int i = 0; i < m_size; i++)
//	{
//		if (m_p_unit_list[i].id == id)
//			return m_p_unit_list[i].index;
//	}
//
//	return NO_SPRITE_ID;
//}
//
//C_CONVERT_TABLE::C_CONVERT_TABLE()
//{
//	m_p_unit_list = NULL;
//	m_size = 0;
//}
//
//C_CONVERT_TABLE::~C_CONVERT_TABLE()
//{
//	if (m_p_unit_list)
//	{
//		assert(m_size > 0);
//
//		free(m_p_unit_list);
//		m_p_unit_list = NULL;
//		m_size = 0;
//	}
//}
//
//void C_CONVERT_TABLE::Add(data_id_t id, SPRITE_ID index)
//{
//	m_p_unit_list = (UNIT *)realloc(m_p_unit_list, sizeof(UNIT)*(m_size+1));
//
//	m_p_unit_list[m_size].id = id;
//	m_p_unit_list[m_size].index = index;
//
//	m_size++;
//}


//-----------------------------------------------------------------------------
// GetHeight
//
// 
//-----------------------------------------------------------------------------
int C_ANI_OBJECT::GetHeight(SPRITE_ID sprite_id) const
{
	return m_pC_spk->GetHeight(sprite_id);
}

//-----------------------------------------------------------------------------
// GetWidth
//
// 
//-----------------------------------------------------------------------------
int C_ANI_OBJECT::GetWidth(SPRITE_ID sprite_id) const
{
	return m_pC_spk->GetWidth(sprite_id);
}

/*-----------------------------------------------------------------------------
- Blt
-
-----------------------------------------------------------------------------*/
void C_ANI_OBJECT::Blt(POINT &point, SPRITE_ID sprite_id)
{
	m_pC_spk->Blt(point, sprite_id);
}

/*-----------------------------------------------------------------------------
- BltColor
-
-----------------------------------------------------------------------------*/
void C_ANI_OBJECT::BltColor(POINT &point, SPRITE_ID sprite_id, int rgb)
{
	m_pC_spk->BltColor(point, sprite_id, rgb);
}

/*-----------------------------------------------------------------------------
- BltDarkness
-
-----------------------------------------------------------------------------*/
void C_ANI_OBJECT::BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark_bit)
{
	m_pC_spk->BltDarkness(point, sprite_id, dark_bit);
}

/*-----------------------------------------------------------------------------
- FrrRX
-
-----------------------------------------------------------------------------*/
int C_ANI_OBJECT::FrrRX(int index) const
{
	return m_pC_frr->GetRX(index);
}

/*-----------------------------------------------------------------------------
- FrrRY
-
-----------------------------------------------------------------------------*/
int C_ANI_OBJECT::FrrRY(int index) const
{
	return m_pC_frr->GetRY(index);
}

/*-----------------------------------------------------------------------------
- FrrSize
-
-----------------------------------------------------------------------------*/
int C_ANI_OBJECT::FrrSize() const
{
	return m_pC_frr->Size();
}

/*-----------------------------------------------------------------------------
- C_ANI_OBJECT
-
-----------------------------------------------------------------------------*/
C_ANI_OBJECT::C_ANI_OBJECT(const char *sz_spkfile, const char *sz_frrfile)
{
	if (!sz_spkfile || !sz_frrfile)
		_Error(NULL_REF);

	m_pC_spk = new C_SPRITE_PACK(sz_spkfile);

	m_pC_frr = new C_FRR(sz_frrfile);
}

/*-----------------------------------------------------------------------------
- ~C_ANI_OBJECT
-
-----------------------------------------------------------------------------*/
C_ANI_OBJECT::~C_ANI_OBJECT()
{
	DeleteNew(m_pC_spk);
	DeleteNew(m_pC_frr);
}

/*-----------------------------------------------------------------------------
- Timer
-
-----------------------------------------------------------------------------*/
void C_ANIMATION::Timer()
{
	if (m_play_order == STOP)
		return;

	if (m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		// next frame!

		m_dw_prev_tickcount = GetTickCount();

		switch (m_play_order)
		{
			case PLAY:
				if (m_pC_ani_object->FrrSize() > m_current_frame+1)
					m_current_frame++;
				else
				{
					m_play_order = STOP;

					// m_play_order가 next order로 바뀔 때, 시간간격이 있기 때문에(외부에서 알 때
					// 이것은 중요한 문제다) 곧바로 next order를 수행한다.
					RunNextPlayOrder();
				}
				break;

			case PLAY_BACK:
				if (0 < m_current_frame)
					m_current_frame--;
				else
				{
					m_play_order = STOP;
					RunNextPlayOrder();
				}
				break;

			case PLAY_LOOP:
				if (m_pC_ani_object->FrrSize() > m_current_frame+1)
					m_current_frame++;
				else
					m_current_frame = 0;
				break;

			case PLAY_LOOPBACK:
				if (m_bl_reverse)
					m_current_frame--;
				else
					m_current_frame++;

				if (m_current_frame == 0)
					m_bl_reverse = false;
				if (m_current_frame == m_pC_ani_object->FrrSize()-1)
					m_bl_reverse = true;
				break;
		}
	}
}

/*-----------------------------------------------------------------------------
- Doing
- Animation 중이라면 true를 반환한다.
-----------------------------------------------------------------------------*/
C_ANIMATION::PLAY_ORDER C_ANIMATION::GetAnimationState() const
{
	return m_play_order;
}

/*-----------------------------------------------------------------------------
- C_ANIMATION
-
-----------------------------------------------------------------------------*/
C_ANIMATION::C_ANIMATION(C_ANI_OBJECT *p_object)
{
	m_pC_ani_object = p_object;
	if (!m_pC_ani_object)
		_Error(NULL_REF);

	m_play_order_next = STOP;
	m_play_order = STOP;
	m_current_frame = 0;
	m_dw_millisec = 100;
	m_x = 0;
	m_y = 0;
}

/*-----------------------------------------------------------------------------
- ~C_ANIMATION
-
-----------------------------------------------------------------------------*/
C_ANIMATION::~C_ANIMATION()
{

}

//-----------------------------------------------------------------------------
// RunNextPlayOrder
//
// next order가 있으면 m_play_order으로 대체하고, next order를 다시 대기상태(STOP)로
// 바꾼다.
//-----------------------------------------------------------------------------
void C_ANIMATION::RunNextPlayOrder()
{
	switch (m_play_order_next)
	{
		case PLAY:
			Play();
			break;

		case PLAY_BACK:
			PlayBack();
			break;

		case PLAY_LOOP:
			PlayLoop();
			break;

		case PLAY_LOOPBACK:
			PlayLoopBack();
			break;
	}

	m_play_order_next = STOP;
}

/*-----------------------------------------------------------------------------
- SetSpeed
- timer 속도를 설정한다.
-----------------------------------------------------------------------------*/
void C_ANIMATION::SetSpeed(DWORD millisec)
{
	m_dw_millisec = millisec;
}

/*-----------------------------------------------------------------------------
- SetPlayPosition
- animation을 하는 절대위치를 설정한다.
-----------------------------------------------------------------------------*/
void C_ANIMATION::SetPlayPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

/*-----------------------------------------------------------------------------
- PlayLoop
-
-----------------------------------------------------------------------------*/
void C_ANIMATION::PlayLoop()
{
	// refresh timer
	m_dw_prev_tickcount = GetTickCount();

	m_play_order = PLAY_LOOP;
	m_current_frame = 0;
}

//-----------------------------------------------------------------------------
// PlayLoopBack
//
// play -> back -> play (반복)
//-----------------------------------------------------------------------------
void C_ANIMATION::PlayLoopBack()
{
	// refresh timer
	m_dw_prev_tickcount = GetTickCount();

	m_play_order = PLAY_LOOPBACK;
	m_current_frame = 0;
	m_bl_reverse = false;
}

/*-----------------------------------------------------------------------------
- Play
- 앞에서 뒤로 한 번 animation한다.
-----------------------------------------------------------------------------*/
void C_ANIMATION::Play()
{
	// refresh timer
	m_dw_prev_tickcount = GetTickCount();
	
	m_current_frame = 0;
	m_play_order = PLAY;
}

//-----------------------------------------------------------------------------
// Stop
//
// 현재 위치에서 멈춘다.
//-----------------------------------------------------------------------------
void C_ANIMATION::Stop()
{
	// refresh timer
	m_dw_prev_tickcount = GetTickCount();

	m_play_order = STOP;
}

//-----------------------------------------------------------------------------
// SetNextPlayOrder
//
//
//-----------------------------------------------------------------------------
void C_ANIMATION::SetNextPlayOrder(C_ANIMATION::PLAY_ORDER order)
{
	m_play_order_next = order;
}

//-----------------------------------------------------------------------------
// GetNextPlayOrder
//
// 
//-----------------------------------------------------------------------------
C_ANIMATION::PLAY_ORDER C_ANIMATION::GetNextPlayOrder() const
{
	return m_play_order_next;
}

//-----------------------------------------------------------------------------
// Refresh
//
// 
//-----------------------------------------------------------------------------
void C_ANIMATION::Refresh()
{
	m_play_order = STOP;
	m_play_order_next = STOP;
	m_current_frame = 0;
}

/*-----------------------------------------------------------------------------
- PlayBack
- 뒤에서 앞으로 한 번 animation한다.
-----------------------------------------------------------------------------*/
void C_ANIMATION::PlayBack()
{
	// refresh timer
	m_dw_prev_tickcount = GetTickCount();

	m_play_order = PLAY_BACK;
	m_current_frame = m_pC_ani_object->FrrSize()-1;
}

/*-----------------------------------------------------------------------------
- Show
- animation이 끝나면 마지막 frame을 보인다.
-----------------------------------------------------------------------------*/
void C_ANIMATION::Show(bool enable)
{
	POINT point = {m_x+m_pC_ani_object->FrrRX(m_current_frame), m_y+m_pC_ani_object->FrrRY(m_current_frame)};

	if(enable)
		m_pC_ani_object->Blt(point, m_current_frame);
	else
//		gpC_base->m_p_DDSurface_back->BltIndexSpriteDarkness(&point, &m_pC_slayer_woman_ispk[m_pC_slayer_woman_cfpk[p_slot->woman_info.right][0][0][index].GetSpriteID()], DARK_BIT);
		m_pC_ani_object->BltDarkness(point, m_current_frame, 2);

	// 속도를 위해 Show()에 포함...
	Timer();
}

/*-----------------------------------------------------------------------------
- Size
- frame의 충 수를 반환한다.
-----------------------------------------------------------------------------*/
int C_FRR::Size() const
{
	return m_C_frame_array.GetSize();
}

/*-----------------------------------------------------------------------------
- GetRX
-
-----------------------------------------------------------------------------*/
int C_FRR::GetRX(int frame) const
{
	assert(frame > -1 && frame < m_C_frame_array.GetSize());

	return m_C_frame_array[frame].GetCX();
}

/*-----------------------------------------------------------------------------
- GetRY
-
-----------------------------------------------------------------------------*/
int C_FRR::GetRY(int frame) const
{
	assert(frame > -1 && frame < m_C_frame_array.GetSize());

	return m_C_frame_array[frame].GetCY();
}

/*-----------------------------------------------------------------------------
- C_FRR
-
-----------------------------------------------------------------------------*/
C_FRR::C_FRR(const char *sz_filename)
{
	if (sz_filename)
	{
		if (!Open(sz_filename))
			_Error(FAILED_JOB);
	}
}

/*-----------------------------------------------------------------------------
- ~C_FRR
-
-----------------------------------------------------------------------------*/
C_FRR::~C_FRR()
{
	m_C_frame_array.Release();
}

/*-----------------------------------------------------------------------------
- Open
-
  `이미 load되어 있다면, release하고 sz_filename을 open한다.
-----------------------------------------------------------------------------*/
bool C_FRR::Open(const char *sz_filename)
{
	if (!sz_filename)
		return false;

	// 이전에 load했다면 release한다.
	if (m_C_frame_array.GetSize() > 0)
		m_C_frame_array.Release();

	ifstream file(sz_filename, ios::binary | ios::nocreate);
	if (!file)
		_Error(FILE_OPEN);

	m_C_frame_array.LoadFromFile(file);
	file.close();

	return true;
}

/*-----------------------------------------------------------------------------
- Open
- Sprite Pack file을 open한다.

  `Sprite pack file이 아닐경우의 예외처리는 없다.
  `이미 load되어 있다면, release하고 sz_filename을 open한다.
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::Open(const char *sz_filename)
{
	//assert(m_pC_spk_list);	// by sigi

//	if (!sz_filename)// || !m_pC_spk_list)		// by sigi
//		_ErrorStr((char *)sz_filename);//(FAILED_JOB);

	// by sigi
	// 이전에 load했다면 release한다.
	//if (m_pC_spk_list->GetSize() > 0)
	//	m_pC_spk_list->Release();

//	ifstream file(sz_filename, ios::binary | ios::nocreate);
//	if (!file)
//	{
//		_ErrorStr((char *)sz_filename);
//		//_Error(FILE_OPEN);
//	}

	//m_pC_spk_list->LoadFromFile(file);
	m_SPK.LoadFromFileRunning( sz_filename );
}

/*-----------------------------------------------------------------------------
- C_SPRITE_PACK
-
-----------------------------------------------------------------------------*/
C_SPRITE_PACK::C_SPRITE_PACK(const char *sz_filename)
{
	// Sprite를 저장할 때 565인지 555인지 알아야되니까...
	// 그런데 이 검사를 외부에서 하게 했다는군...
	
	// by sigi
	//if (CDirectDraw::Is565())
	//	m_pC_spk_list = new CSpritePackList565;
	//else
	//	m_pC_spk_list = new CSpritePackList555;

	if (sz_filename) // file명을 지정하였다면...
		Open(sz_filename);
}

/*-----------------------------------------------------------------------------
- ~C_SPRITE_PACK
-
-----------------------------------------------------------------------------*/
C_SPRITE_PACK::~C_SPRITE_PACK()
{
	// by sigi
	//DeleteNew(m_pC_spk_list);
}

//-----------------------------------------------------------------------------
// BltColor
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltColor(int x, int y, SPRITE_ID sprite_id, int rgb)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	POINT point;
	point.x = x;
	point.y = y;

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, &m_SPK[sprite_id], rgb);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltColor
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltColor(POINT &point, SPRITE_ID sprite_id, int rgb)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, &m_SPK[sprite_id], rgb);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltDarkness
//
// dark bit shift blt
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltDarkness(int x, int y, SPRITE_ID sprite_id, int dark)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	POINT point;
	point.x = x;
	point.y = y;

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&point, &m_SPK[sprite_id], dark);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltDarkness
//
// dark bit shift blt
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&point, &m_SPK[sprite_id], dark);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// BltClip
//
// sprite를 clipping한다.
// sprite의 원하는 부분만 출력할 때 용이하다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltClip(int x, int y, Rect &rect, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO surface_info;
		SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
		
		RECT rt;
		rt.left = max(-x, rect.x);
		rt.top = max(-y, rect.y);
		// add by Sonic 2006.9.26
		if(g_MyFull)
		{
			rt.right = min(rect.x+rect.w, 1024-x);
			rt.bottom = min(rect.y+rect.h, 768-y);
		}
		else
		{
			rt.right = min(rect.x+rect.w, 800-x);
			rt.bottom = min(rect.y+rect.h, 600-y);
		}
		// end
		if(rt.left < rt.right && rt.top < rt.bottom)
		{
			
			WORD * p_dest = (WORD *)surface_info.p_surface+x+rt.left;
			p_dest = (WORD *)((BYTE *)p_dest+(y+rt.top)*surface_info.pitch);
			
			//void BltClip(WORD *pDest, WORD pitch, RECT* pRect); // in CSprite.h
			m_SPK[sprite_id].BltClipWidth(p_dest, surface_info.pitch, &rt);
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

/*-----------------------------------------------------------------------------
- BltAlpha
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltAlpha(int x, int y, SPRITE_ID sprite_id, int alpha)
{
	POINT point = {x, y};
	BltAlpha(point, sprite_id, alpha);
}

/*-----------------------------------------------------------------------------
- BltAlpha
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltAlpha(POINT &point, SPRITE_ID sprite_id, int alpha)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteAlpha(&point, &m_SPK[sprite_id], alpha);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

/*-----------------------------------------------------------------------------
- GetWidth
-
-----------------------------------------------------------------------------*/
int C_SPRITE_PACK::GetWidth(SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);
	//assert(p_sprite);

	//if (!p_sprite)
	//	return 0;

	return m_SPK[sprite_id].GetWidth();
}

/*-----------------------------------------------------------------------------
- GetHeight
-
-----------------------------------------------------------------------------*/
int C_SPRITE_PACK::GetHeight(SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);
	//assert(p_sprite);

	//if (!p_sprite)
	//	return 0;

	return m_SPK[sprite_id].GetHeight();
}

//-----------------------------------------------------------------------------
// IsPixel
//
// (x, y)가 투명색이면 false를 반환한다.
//-----------------------------------------------------------------------------
bool C_SPRITE_PACK::IsPixel(int x, int y, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);
	
	return m_SPK[sprite_id].IsColorPixel(x, y);
}

/*-----------------------------------------------------------------------------
- Blt
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::Blt(int x, int y, SPRITE_ID sprite_id)
{
	POINT point = {x, y};
	Blt(point, sprite_id);
}

/*-----------------------------------------------------------------------------
- Blt
- Sprite Surface인 BackSurface에 blt한다.
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::Blt(POINT &point, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSprite(&point, &m_SPK[sprite_id]);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

/*-----------------------------------------------------------------------------
- BltOffscreen
- Sprite Surface인 BackSurface에 blt한다.
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltOffscreen(POINT &point, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_DDSurface_offscreen.Lock())
	{
		gpC_base->m_DDSurface_offscreen.BltSprite(&point, &m_SPK[sprite_id]);

		gpC_base->m_DDSurface_offscreen.Unlock();
	}
}

void C_SPRITE_PACK::BltOffscreen(int x, int y, SPRITE_ID sprite_id)
{
	POINT point = {x, y};
	BltOffscreen(point, sprite_id);
}

//-----------------------------------------------------------------------------
// BltOutline
//
// Sprite에 외곽선을 그려서 함께 출력한다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltOutline(int x, int y, int color, SPRITE_ID sprite_id)
{
	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_SPK[sprite_id]);
	outline_o.Generate();

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_base->m_p_DDSurface_back->BltSpriteOutline(&outline_o,  color);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	outline_o.Clear();
}


/*-----------------------------------------------------------------------------
- BltLocked
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltLocked(int x, int y, SPRITE_ID sprite_id)
{
	POINT point = {x, y};
	BltLocked(point, sprite_id);
}

/*-----------------------------------------------------------------------------
- BltLocked
- Sprite Surface인 BackSurface에 blt한다.
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltLocked(POINT &point, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSprite(&point, &m_SPK[sprite_id]);
}

/*-----------------------------------------------------------------------------
- BltLockedOffscreen
- Sprite Surface인 BackSurface에 blt한다.
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltLockedOffscreen(POINT &point, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_DDSurface_offscreen.BltSprite(&point, &m_SPK[sprite_id]);
}

void C_SPRITE_PACK::BltLockedOffscreen(int x, int y, SPRITE_ID sprite_id)
{
	POINT point = {x, y};
	BltLockedOffscreen(point, sprite_id);
}

//-----------------------------------------------------------------------------
// BltOutline
//
// Sprite에 외곽선을 그려서 함께 출력한다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltLockedOutline(int x, int y, int color, SPRITE_ID sprite_id)
{
	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	// focus된 것은 외곽선을 그린다.
	CSpriteOutlineManager	outline_o; // 외곽선출력 객체.

	// 외곽선출력 객체 추가.
	outline_o.Add(x, y, &m_SPK[sprite_id]);
	outline_o.Generate();

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSpriteOutline(&outline_o,  color);

	outline_o.Clear();
}

//-----------------------------------------------------------------------------
// BltLockedColor
//
// rgb 중 하나만으로 blt한다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltLockedColor(int x, int y, SPRITE_ID sprite_id, int rgb)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	POINT point;
	point.x = x;
	point.y = y;

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, &m_SPK[sprite_id], rgb);
}

//-----------------------------------------------------------------------------
// BltLockedDarkness
//
// dark bit shift blt
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltLockedDarkness(int x, int y, SPRITE_ID sprite_id, int dark)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	POINT point;
	point.x = x;
	point.y = y;

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&point, &m_SPK[sprite_id], dark);
}

//-----------------------------------------------------------------------------
// BltLockedClip
//
// sprite를 clipping한다.
// sprite의 원하는 부분만 출력할 때 용이하다.
//-----------------------------------------------------------------------------
void C_SPRITE_PACK::BltLockedClip(int x, int y, Rect &rect, SPRITE_ID sprite_id)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(gpC_base->m_p_DDSurface_back->IsLock());

	S_SURFACEINFO surface_info;
	SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
	
	RECT rt;
	rt.left = max(-x, rect.x);
	rt.top = max(-y, rect.y);
	// add by Sonic 2006.9.26
	if(g_MyFull)
	{
		rt.right = min(rect.x+rect.w, 1024-x);
		rt.bottom = min(rect.y+rect.h, 768-y);
	}
	else
	{
		rt.right = min(rect.x+rect.w, 800-x);
		rt.bottom = min(rect.y+rect.h, 600-y);
	}
	// end
	
	if(rt.left >= rt.right || rt.top >= rt.bottom)return;

	WORD * p_dest = (WORD *)surface_info.p_surface+x+rt.left;
	p_dest = (WORD *)((BYTE *)p_dest+(y+rt.top)*surface_info.pitch);

	//void BltClip(WORD *pDest, WORD pitch, RECT* pRect); // in CSprite.h
	m_SPK[sprite_id].BltClipWidth(p_dest, surface_info.pitch, &rt);

}

/*-----------------------------------------------------------------------------
- BltLockedAlpha
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltLockedAlpha(int x, int y, SPRITE_ID sprite_id, int alpha)
{
	POINT point = {x, y};
	BltLockedAlpha(point, sprite_id, alpha);
}

/*-----------------------------------------------------------------------------
- BltLockedAlpha
-
-----------------------------------------------------------------------------*/
void C_SPRITE_PACK::BltLockedAlpha(POINT &point, SPRITE_ID sprite_id, int alpha)
{
	//assert(sprite_id >= 0);

	//CSprite *p_sprite = m_pC_spk_list->GetSprite(sprite_id);

	//assert(p_sprite);

	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSpriteAlpha(&point, &m_SPK[sprite_id], alpha);

}

void C_SPRITE_PACK::BltLockedColorSet(POINT &point, SPRITE_ID sprite_id, WORD colorset)
{
	assert(gpC_base->m_p_DDSurface_back->IsLock());
	gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&point, &m_SPK[sprite_id], colorset);
}


void C_SPRITE_PACK::BltLockedColorSet(int x, int y, SPRITE_ID sprite_id, WORD colorset)
{
	POINT point = {x, y};
	BltLockedColorSet(point, sprite_id, colorset);
}

//-----------------------------------------------------------------------------
// C_SPRITE_FRAME_ARRAY
//
// 
//-----------------------------------------------------------------------------
C_SPRITE_FRAME_ARRAY::C_SPRITE_FRAME_ARRAY()
{
	m_size = 0;
	m_pC_sprite_frame_buf = NULL;
}

//-----------------------------------------------------------------------------
// ~C_SPRITE_FRAME_ARRAY
//
// 
//-----------------------------------------------------------------------------
C_SPRITE_FRAME_ARRAY::~C_SPRITE_FRAME_ARRAY()
{
	if (m_pC_sprite_frame_buf)
	{
		free(m_pC_sprite_frame_buf);
		m_pC_sprite_frame_buf = NULL;
	}
}

//-----------------------------------------------------------------------------
// AddSpriteFrame
//
// 
//-----------------------------------------------------------------------------
void C_SPRITE_FRAME_ARRAY::AddSpriteFrame(C_SPRITE_FRAME * pC_sf)
{
	m_pC_sprite_frame_buf = (C_SPRITE_FRAME *)realloc(m_pC_sprite_frame_buf, sizeof(C_SPRITE_FRAME)*(m_size+1));

	m_pC_sprite_frame_buf[m_size] = *pC_sf;
	m_size++;
}

//-----------------------------------------------------------------------------
// GetRX
//
// 
//-----------------------------------------------------------------------------
int C_SPRITE_FRAME_ARRAY::GetRX(int index)
{
	if (index == NO_SPRITE_ID)
		return 0;

	if (index < 0 || index >= m_size)
		_Error(FAILED_JOB);

	return m_pC_sprite_frame_buf[index].GetRX();
}

//-----------------------------------------------------------------------------
// GetRY
//
// 
//-----------------------------------------------------------------------------
int C_SPRITE_FRAME_ARRAY::GetRY(int index)
{
	if (index == NO_SPRITE_ID)
		return 0;

	if (index < 0 || index >= m_size)
		_Error(FAILED_JOB);

	return m_pC_sprite_frame_buf[index].GetRY();
}

//-----------------------------------------------------------------------------
// Constructor.
//
// 
//-----------------------------------------------------------------------------
C_SPRITE_FRAME::C_SPRITE_FRAME()
{
	m_rx = 0;
	m_ry = 0;
}

//-----------------------------------------------------------------------------
// Destructor.
//
// 
//-----------------------------------------------------------------------------
C_SPRITE_FRAME::~C_SPRITE_FRAME()
{

}

//-----------------------------------------------------------------------------
// Open
//
// 
//-----------------------------------------------------------------------------
bool C_SPRITE_FRAME::Open(const char * pathfile)
{
	ifstream file(pathfile, ios::binary | ios::nocreate);
	if (!file)
		return false;

	file.read((char *)&m_rx, sizeof(m_rx));
	file.read((char *)&m_ry, sizeof(m_ry));

	file.close();

	return true;
}

//-----------------------------------------------------------------------------
// Save
//
// 
//-----------------------------------------------------------------------------
bool C_SPRITE_FRAME::Save(const char * pathfile)
{
	ofstream file(pathfile, ios::binary);
	if (!file)
		return false;

	file.write((char *)&m_rx, sizeof(m_rx));
	file.write((char *)&m_ry, sizeof(m_ry));

	file.close();

	return true;
}