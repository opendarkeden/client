/*-----------------------------------------------------------------------------

	VS_UI_util.h

	Utility methods.

	2000.2.2. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_UTIL_H__
#define __VS_UI_UTIL_H__

#include <limits.h>
#include "SP.h"
#include "framelib/FR.h"
//#include "rbt.h"
#include "BasicS.h"
#include "SpriteLib/CSpritePack.h"
#include "SpriteLib/CIndexSpritePack.h"

typedef WORD SPRITE_ID;

// sprite id�� �ƴϴ�.
#define NO_SPRITE_ID						ULONG_MAX

// data id <-> spk index
//class C_CONVERT_TABLE
//{
//private:
//	struct UNIT
//	{
//		data_id_t	id;
//		SPRITE_ID	index;
//	};
//
//	UNIT *		m_p_unit_list;
//	int			m_size;
//
//public:
//	C_CONVERT_TABLE();
//	~C_CONVERT_TABLE();
//
//	void			Add(data_id_t id, SPRITE_ID index);
//	SPRITE_ID	Index(data_id_t id) const;
//};

/*-----------------------------------------------------------------------------
  Class Sprite Pack
  `DX�� �ʱ�ȭ�Ǿ 555, 565 ���θ� �Ǻ��ϹǷ� �ٵ�� DX�� �ʱ�ȭ��Ų �Ŀ� 
   ����(����)�ؾ� �Ѵ�.
-----------------------------------------------------------------------------*/
// RGB �� ��� �ϳ�.
#define rgb_RED							0
#define rgb_GREEN							1
#define rgb_BLUE							2

class C_SPRITE_PACK
{
private:
	//CSpritePackList *	m_pC_spk_list; // 565, 555�� �����ؾ��ϱ� ������ pointer��.
	CSpritePack			m_SPK;	// by sigi

	// Disable copy constructor and copy assignment to prevent issues with m_file pointer
	C_SPRITE_PACK(const C_SPRITE_PACK&) = delete;
	C_SPRITE_PACK& operator=(const C_SPRITE_PACK&) = delete;

	// Disable move constructor and move assignment to prevent m_file pointer from being moved
	C_SPRITE_PACK(C_SPRITE_PACK&&) = delete;
	C_SPRITE_PACK& operator=(C_SPRITE_PACK&&) = delete;

public:
	C_SPRITE_PACK(const char *sz_filename = NULL);
	~C_SPRITE_PACK();

	void	BltClip(int x, int y, Rect &rect, SPRITE_ID sprite_id=0);

	int		GetWidth(SPRITE_ID sprite_id = 0);
	int		GetHeight(SPRITE_ID sprite_id = 0);
	void	Open(const char *sz_filename);
	void	BltOutline(int x, int y, int color, SPRITE_ID sprite_id = 0);
	void	Blt(POINT &point, SPRITE_ID sprite_id=0);
	void	Blt(int x=0, int y=0, SPRITE_ID sprite_id=0);
	void	BltColor(int x, int y, SPRITE_ID sprite_id, int rgb);
	void	BltColor(POINT &point, SPRITE_ID sprite_id, int rgb);
	void	BltDarkness(int x, int y, SPRITE_ID sprite_id, int dark);
	void	BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark);
	void	BltAlpha(POINT &point, SPRITE_ID sprite_id, int alpha);
	void	BltAlpha(int x, int y, SPRITE_ID sprite_id, int alpha);
	void	BltOffscreen(POINT &point, SPRITE_ID sprite_id = 0);
	void	BltOffscreen(int x=0, int y=0, SPRITE_ID sprite_id = 0);

	// �̹� Surface�� Lock�� ���¿��� �θ��� �Լ�..
	void	BltLockedClip(int x, int y, Rect &rect, SPRITE_ID sprite_id=0);
	void	BltLockedOutline(int x, int y, int color, SPRITE_ID sprite_id = 0);
	void	BltLocked(POINT &point, SPRITE_ID sprite_id=0);
	void	BltLocked(int x=0, int y=0, SPRITE_ID sprite_id=0);
	void	BltLockedColor(int x, int y, SPRITE_ID sprite_id, int rgb);
	void	BltLockedDarkness(int x, int y, SPRITE_ID sprite_id, int dark);
	void	BltLockedAlpha(POINT &point, SPRITE_ID sprite_id, int alpha);
	void	BltLockedAlpha(int x, int y, SPRITE_ID sprite_id, int alpha);
	void	BltLockedOffscreen(POINT &point, SPRITE_ID sprite_id = 0);
	void	BltLockedOffscreen(int x=0, int y=0, SPRITE_ID sprite_id = 0);
	void	BltLockedColorSet(int x, int y, SPRITE_ID sprite_id, WORD colorset);
	void	BltLockedColorSet(POINT &point, SPRITE_ID sprite_id, WORD colorset);

	bool	IsPixel(int x, int y, SPRITE_ID sprite_id = 0);

	int		GetSize()	{ return m_SPK.GetSize(); }
	CSprite &GetSprite(SPRITE_ID id) { return m_SPK[id]; }
};

/*-----------------------------------------------------------------------------
  Class FRR
  `��ü�� Animation�����ֱ� ���� Frame array object.
-----------------------------------------------------------------------------*/
class C_FRR
{
private:
	FRAME_ARRAY 	m_C_frame_array;

public:
	C_FRR(const char *sz_filename = NULL);
	~C_FRR();

	bool	Open(const char *sz_filename);
	int	Size() const;
	int	GetRX(int frame) const;
	int	GetRY(int frame) const;
};

/*-----------------------------------------------------------------------------
  Class animation object
  `animation�� object. �̰��� �� ���� SPK, FRR �̴�. ���� SPK, FRR�� �����ġ��
   �ٸ��� �� ���, �׸��� ������ Timer�� ����� ��찡 �ֱ� ������ �������. 
	�̰��� C_ANIMATION�� ������ animation�ȴ�.
-----------------------------------------------------------------------------*/
class C_ANI_OBJECT
{
private:
	//
	// m_pC_spk
	//
	// m_pC_frr�� �����ϴ� SPK�̴�. �ϳ��� Animation object�� �ϳ����� SPK, FRR��
	// ���´�. �ϳ��� SPK�� �ټ��� FRR�� ������ �� ����. �̷��� �ϴ� ������, �׷���
	// ���ӻ��� Animation object�� �� ��� ��ɰų� ����, ���� ȥ��(?)�� �����ϸ�,
	// �ܼ��� Interface�� �����ϱ� �����̴�. ���� �׷� �ʿ䰡 ���Ŀ� ����ٸ�,
	// �Ǵٸ� Class�� ����� �׸��̴�.
	//
	C_SPRITE_PACK *		m_pC_spk;
	C_FRR *					m_pC_frr;

public:
	C_ANI_OBJECT(const char *sz_spkfile, const char *sz_frrfile);
	~C_ANI_OBJECT();

	int	FrrSize() const;
	int	FrrRX(int index) const;
	int	FrrRY(int index) const;
	void	Blt(POINT &point, SPRITE_ID sprite_id);
	void	BltColor(POINT &point, SPRITE_ID sprite_id, int rgb);
	void	BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark_bit);
	int	GetWidth(SPRITE_ID sprite_id) const;
	int	GetHeight(SPRITE_ID sprite_id) const;
};

/*-----------------------------------------------------------------------------
  Class animation object
  `Object�� Animation��Ű�� object.

  `SPK file�� Frr file�� animation�� �ϱ� ���� �ʿ������̴�. Animation Object��
   �� �ΰ��� ������ �����Ͽ�, ���� ��ġ�� play, �� animation�����ش�. ����
	Timer�� �����ϰ� Timer�� ������ �� �ִ�. ���� ������ animation�� ���� �ִ�.
-----------------------------------------------------------------------------*/
class C_ANIMATION
{
private:
	//
	// m_pC_ani_object
	//
	// 1. Animation�� object pointer�̴�. �� object�� �ٸ� Animation object������
	//    ����� �� �ֱ� ������ pointer�� �ؾ� ȿ�����̴�.
	//
	// 2. �ϳ��� Animation object������ �ϳ��� ani object�� ���´�. �׷��� �� ����
	//    �ݵ�� �׷����� �ʾƵ� �ȴ�.
	//
	C_ANI_OBJECT *			m_pC_ani_object;

	//
	// Timer
	// 
	// �ϳ��� Animation object�� play�Ǳ� ���� �ϳ��� timer�� ���´�. ���ο� timer��
	// �����ȴ�. Timer library�� ���� �Լ��� ����������ϱ� ������ ����� �� ����.
	//
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	UINT						m_current_frame;
	int						m_x, m_y;

	bool						m_bl_reverse;

public:
	enum PLAY_ORDER
	{
		STOP,					// stop
		PLAY,					// 0�� frame���� ������ �� ����.
		PLAY_LOOP,			// play�� ��� �ݺ�.
		PLAY_BACK,			// �� frame���� 0������ �� ����.
		PLAY_BACKLOOP,		// play back ��� �ݺ�.
		PLAY_LOOPBACK,		// play -> back -> play (�ݺ�)
	};

private:
	PLAY_ORDER				m_play_order;
	PLAY_ORDER				m_play_order_next; // play order�� ������ ���ִ� ��.

public:
	C_ANIMATION(C_ANI_OBJECT *p_object);
	~C_ANIMATION();

	PLAY_ORDER	GetAnimationState() const;
	void	SetSpeed(DWORD millisec);
	void	SetPlayPosition(int x, int y);
	void	Play();
	void	Stop();
	void	PlayLoopBack();
	void	PlayBack();
	void	PlayLoop();
	void	Show(bool enable = true);
	void	SetNextPlayOrder(PLAY_ORDER order);
	void	RunNextPlayOrder();
	PLAY_ORDER GetNextPlayOrder() const;
	void	Refresh();

	void	Timer();
};

//----------------------------------------------------------------------------
// Class SpriteFrame
//----------------------------------------------------------------------------
class C_SPRITE_FRAME
{
private:
	int		m_rx;
	int		m_ry;

public:
	C_SPRITE_FRAME();
	~C_SPRITE_FRAME();

//----------------------------------------------------------------------------
// Open/Save
//----------------------------------------------------------------------------
	bool	Open(const char * pathfile);
	bool	Save(const char * pathfile);

//----------------------------------------------------------------------------
// Get
//----------------------------------------------------------------------------
	int	GetRX() const { return m_rx; }
	int	GetRY() const { return m_ry; }

//----------------------------------------------------------------------------
// Set
//----------------------------------------------------------------------------
	void	SetRX(int rx) { m_rx = rx; }
	void	SetRY(int ry) { m_ry = ry; }
};

//----------------------------------------------------------------------------
// Class SpriteFrame Array
//----------------------------------------------------------------------------
class C_SPRITE_FRAME_ARRAY
{
private:
	int						m_size;
	C_SPRITE_FRAME *		m_pC_sprite_frame_buf;

public:
	C_SPRITE_FRAME_ARRAY();
	~C_SPRITE_FRAME_ARRAY();

	void	AddSpriteFrame(C_SPRITE_FRAME * pC_sf);
	int	GetRX(int index);
	int	GetRY(int index);
};

//----------------------------------------------------------------------------
// Public
//----------------------------------------------------------------------------
//void Read_rbt(const char * rbt_file, CSpritePackList * p_spk_list, C_CONVERT_TABLE * p_ct);
//void Read_rbt(const char * rbt_file, C_SPRITE_FRAME_ARRAY * p_sf_array, C_CONVERT_TABLE * p_ct);

#endif