/*-----------------------------------------------------------------------------

	VS_UI_progress.h

	Progress class.

	2000.6.8. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_PROGRESS_H__
#define __VS_UI_PROGRESS_H__

#include "VS_UI_Base.h"
#include "VS_UI_util.h"
#include "VS_UI_extradialog.h"	//VS_UI_DESC를 불러오면 extradialog가 에러난다 왜그럴까-_-a
/*
enum PROGRESS_SPK_INDEX
{
	TUTORIAL_HELIPORT,
	DUNGEON_EXIT,
	SLAYER_PROGRESS_1,
	SLAYER_PROGRESS_2,	
	VAMPIRE_PROGRESS_1,
	VAMPIRE_PROGRESS_2,
	VAMPIRE_PROGRESS_3,
	BAR,
};
*/

//----------------------------------------------------------------------------
// Class Progress
//----------------------------------------------------------------------------
class C_VS_UI_PROGRESS : public Window, public C_VS_UI_DESC
{
private:
//	enum 
//	{ 
//		TOTAL_BAR_LINE = 413,//248, 
//		BAR_GAP_X = 192, //49,//44,
//		BAR_GAP_Y = 557, //558,//144,
//		BAR_COLORKEY = 0xFF,

		//VAMPIRE_RAND_MAX = 6, // progress_vampire_rand.spk + original vampire back image
//	};
	enum
	{
		BACK,
		CHAR_NAME_ALPHA,
		CHAR_NAME_BACK,
		PROGRESS_BAR,
		PROGRESS_ICON,
	};

	C_SPRITE_PACK *		m_pC_event_progress;
	CSpritePack *		m_pC_progress;
	CSpritePack	*		m_pC_character;
//	WORD *					m_p_bar_buf;
//	int						m_bar_width, m_bar_height;

	WORD						m_percent;
	int					m_Event_Num;
//	WORD						m_cur_line;

//	int						m_progress_back_image;

//	void	DrawBar();
//	CSpriteOutlineManager m_SOM;

	std::string	m_world_name;
	std::string	m_server_name;
	int m_server_status;

public:
	C_VS_UI_PROGRESS();
	~C_VS_UI_PROGRESS();

	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	Start();
	void	Finish();
	void	Show();
	void	SetProgress(WORD cur_val, WORD max_val);
	void	AcquireDisappear() {}

	void	SetServerName(const char *world_name, const char *server_name, int status)
	{
		if(world_name)
			m_world_name = world_name;
		else
			m_world_name = "";
		
		if(server_name)
			m_server_name = server_name;
		else 
			m_server_name = "";
		
		m_server_status = status; 
	}

	const char*	GetServerName() { return m_server_name.c_str(); }
	const char*	GetWorldName() { return m_world_name.c_str(); }
	int GetServerStatus() { return m_server_status; }

	//
	// !gC_vs_ui.StartProgress() 후에 설정한다.
	//
//	void	SetProgressStyle(PROGRESS_SPK_INDEX style);
};

#endif