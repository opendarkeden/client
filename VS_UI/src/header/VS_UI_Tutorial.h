/*-----------------------------------------------------------------------------

	VS_UI_Tutorial.h

	2001.2.15. Larosel

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_TUTORAL_H__
#define __VS_UI_TUTORAL_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"
#include "VS_UI_Util.h"
//#include "VS_UI_GameCommon.h"
#include "VS_UI_DESC.h"
#include "../../RarFile.h"
#include <vector>
#include <string>
//using namespace std;

#define CTREE std::vector<C_TREE_BASE>
class C_TREE_BASE
{
public:
	int			open;
	bool		select;
	CTREE		Tree;
	MString	filename;
	std::string	str;

	C_TREE_BASE(){ open = 0; select = false; }
};

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK
//
// Ʃ�丮�� ����� å �������̽��̴�. å�忡�� å�� ������ ���´�.
//-----------------------------------------------------------------------------
class C_VS_UI_BOOK : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
private:
	enum IMAGE_SPK_INDEX
	{
		BOOK_WINDOW,
		CLOSE_BUTTON,
		PREV_BUTTON,
		NEXT_BUTTON,
	};

	enum EXEC_ID
	{
		BOOK_CLOSE_ID,			// â�ݱ�
		BOOK_PREV_ID,			// ������ ����
		BOOK_NEXT_ID,			// ������ ����
	};

	C_SPRITE_PACK *			m_pC_book_spk;

	ButtonGroup *				m_pC_button_group;

//	bool Load(const char *szFilename, int row, int col);

	int m_close_x, m_close_y;
	int m_prev_x, m_prev_y, m_next_x, m_next_y;
	int m_page_x[2], m_page_y, m_page_width;
	int m_char_x[2], m_char_y, m_char_y_distance;
	int m_page_max, m_page_cur;
	int m_col;

//	void ShowBook();

public:
	C_VS_UI_BOOK(const char *szFilename, int row, int col);
	~C_VS_UI_BOOK();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE
//
// Ʃ�丮�� ����� å�� �������̽��̴�. ���������� å���� Ŭ���ϸ� ���´�.
// å�� �����ϰ� Ŭ���ϸ�, å�� ������ ���´�.
// 68,55,67,67 �� : 257��
//-----------------------------------------------------------------------------
class C_VS_UI_BOOKCASE : public Window, public Exec, public ButtonVisual
{
private:
	enum IMAGE_SPK_INDEX
	{
		BOOKCASE_WINDOW,
		CLOSE_BUTTON,
	};

	enum EXEC_ID
	{
		BOOKCASE_CLOSE_ID,			// â�ݱ�
	};

	C_SPRITE_PACK *			m_pC_bookcase_spk;

	ButtonGroup *				m_pC_button_group;

	C_VS_UI_BOOK *				m_pC_book;

	std::vector<std::string> *m_p_book_list;


	int m_books_left, m_books_width, m_books_height, m_books_top[4];
	int m_focus_book_x, m_focus_book_y;
	int m_books_image_index[4];
	int m_book_col, m_book_row;

public:
	C_VS_UI_BOOKCASE();
	~C_VS_UI_BOOKCASE();

	bool	IsReadingBook()			{ return m_pC_book!=NULL; }	// by sigi
	void	CloseBook();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};
/*
//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING
//
// Ʃ�丮�� ����� �긮��ȭ�� �������̽��̴�.
// �������� ���� �ɰų�, ����⸦ ���� Ŭ���ϸ� 
// �ݴ��� ��ũ���� ��ƿ�׸��� �������鼭 �ؽ�Ʈ�� ȭ�鿡 ��ũ�ѵȴ�.
// ������ �����̾���� � �������� ���ӿ� ���� ���� ���� ���̴�.
//-----------------------------------------------------------------------------
class C_VS_UI_BRIEFING : public Window, public Exec, public ButtonVisual
{
private:
	enum IMAGE_SPK_INDEX
	{
		SCREEN_WINDOW,
		CLOSE_BUTTON,
	};

	enum EXEC_ID
	{
		SCREEN_CLOSE_ID,			// â�ݱ�
	};

	C_SPRITE_PACK *				m_pC_screen_spk;

	ButtonGroup *				m_pC_button_group;

	int							m_scroll, m_scroll_detail, m_scroll_max;
	int							m_char_x, m_char_y, m_char_y_distance;

	std::vector<std::string>	m_txt_briefing;
	
	CRarFile					m_briefing_file;

	//timer
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	bool	Timer();

	bool	Load(const char *szFilename);

public:
	C_VS_UI_BRIEFING();
	~C_VS_UI_BRIEFING();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};
*/
//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER
// Ʃ�丮�� ����� ��ǻ�� �������̽��̴�.
// ��ǻ�͸� ���� Ŭ���ϸ� ��ǻ�� ����� ȭ���� �߰�
// �ַ� �����̾���� ����� ����� ���� �����̴�
//
//-----------------------------------------------------------------------------
class C_VS_UI_COMPUTER : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
private:
	enum IMAGE_SPK_INDEX
	{
		MONITOR_WINDOW,
		CLOSE_BUTTON,
		NON_SELECTED_GRID,
		SELECTED_GRID,
		SCROLL_UP,
		SCROLL_DOWN,
		SCROLL_TAG,
	};

	enum EXEC_ID
	{
		MONITOR_CLOSE_ID,			// â�ݱ�
		SCROLL_UP_ID,
		SCROLL_DOWN_ID,
		SCROLL_TREE_UP_ID,
		SCROLL_TREE_DOWN_ID,
	};

	C_SPRITE_PACK *				m_pC_monitor_spk;

	ButtonGroup *				m_pC_button_group;

	CTREE m_tree;
	std::string					m_filename;
	CRarFile					m_computer_tree_file;

	int m_row, m_col;
	int							m_index_x, m_index_y, m_index_distance, m_index_max;
	bool						m_isclose; //Ʈ���� �����ų� ������ ����
	int							m_scroll_x, m_scroll_y, m_scroll_h;
	int							m_tree_limit_h;
	int							m_tree_scroll, m_tree_scroll_x;

// TIMER
	bool						m_timer;
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	void	Timer(bool reset = false);
/////////

	bool	LoadTree(const char* szFilename);
	int		PrintTree(int _x, int _y, int y_distance, int depth, int &limit, bool close, CTREE &tree);
	bool	OpenTree(int &index, CTREE &tree);

	std::string m_second_title_string;
public:
	C_VS_UI_COMPUTER();
	~C_VS_UI_COMPUTER();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};
/*
//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT
//
// Ʃ�丮�� ����� ������ȭ�� �������̽��̴�.
// ������ �������� ���� �ɸ� ����ȴ�.
// ������ ���� ���⼱���� �ϰ� �ȴ�.
//-----------------------------------------------------------------------------
class C_VS_UI_TUTORIAL_EXIT : public Window, public Exec, public ButtonVisual
{
private:
	enum IMAGE_SPK_INDEX
	{
		EXIT_WINDOW,
		OK_BUTTON,
		CANCEL_BUTTON,
		BLADE_BUTTON,
		SWORD_BUTTON,
		MACE_BUTTON,
		CROSS_BUTTON,
		AR_BUTTON,
		TR_BUTTON,
		SMG_BUTTON,
		SG_BUTTON,
	};

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
		SELECT_BLADE_ID,
		SELECT_SWORD_ID,
		SELECT_MACE_ID,
		SELECT_CROSS_ID,
		SELECT_AR_ID,
		SELECT_TR_ID,
		SELECT_SMG_ID,
		SELECT_SG_ID,
	};

	C_SPRITE_PACK *				m_pC_exit_spk;
	C_SPRITE_PACK *				m_pC_item_spk;

	ButtonGroup *				m_pC_button_group;

	id_t m_select;

public:
	C_VS_UI_TUTORIAL_EXIT();
	~C_VS_UI_TUTORIAL_EXIT();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

	void	Accept();

};
*/

#endif