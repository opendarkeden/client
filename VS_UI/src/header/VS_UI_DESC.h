// VS_UI_DESC.h: interface for the C_VS_UI_DESC class.
//
//////////////////////////////////////////////////////////////////////

#include "VS_UI_Util.h"
//#include "VS_UI_GameCommon.h"
#include "RarFile.h"
#include <vector>

#ifndef _VS_UI_DESC_H_
#define _VS_UI_DESC_H_

#define PICTURE_INDENT 20

struct DESC_SPRITE
{
	int pack_num;
	int sprite_num;
	int pos;
};

class C_VS_UI_DESC
{
private:
	std::vector<DESC_SPRITE>	m_Sprite;
	int							m_desc_size;
	PrintInfo					m_pi;
	COLORREF					m_color;

	PrintInfo					m_title_pi;
	COLORREF					m_title_color;

	
protected:
	int							m_delimiter_pack, m_delimiter_sprite;
	int							m_desc_x, m_desc_y, m_desc_title_x, m_desc_title_y;
	int							m_desc_row, m_desc_col, m_desc_scroll;

	std::vector<std::string>	m_desc;
	std::vector<CSpritePack *>	m_pC_inpicture;
	CRarFile					m_pack_file;

	int							fontx;
	int							m_desc_y_distance;

	std::vector<std::string>			m_ori_string;
	std::vector<std::string>			m_rep_string;

	std::string						m_desc_title;

	// description을 출력한다
	void	ShowDesc(int x = 0, int y = 0);

	// description을 불러온다
	bool	LoadDesc(const char *szFilename, int row, int col, bool bl_title = false, int CoreZapID = -1);
	bool	LoadDescFromString(const char *szString, int row, int col, bool bl_title = false, int CoreZapID = -1);
	// description의 출력좌표를 정한다. 
	void	SetDesc(int dx, int dy, COLORREF color = BLACK, PrintInfo &pi = gpC_base->m_desc_msg_pi)
	{
		m_desc_x = dx;
		m_desc_y = dy;
		SetDescColor(color);
		SetDescPi(pi);
	}

	void	SetDescTitle(const char* str)	{ m_desc_title = str; }

	void	SetDescTitle(int dx, int dy, COLORREF color = RGB_WHITE, PrintInfo &pi = gpC_base->m_desc_menu_pi)
	{
		m_desc_title_x = dx;
		m_desc_title_y = dy; 
		m_title_color = color;
		m_title_pi = pi;
	}

	//
	void	SetDescColor(COLORREF color)	{ m_color = color; }
	void	SetDescPi(PrintInfo &pi)		{ m_pi = pi; }

	COLORREF	GetDescColor()				{ return m_color; }
	PrintInfo	SetDescPi()					{ return m_pi; }
	//
	void	SetSprite(int pack, int num, int line);

	// scroll
	void	ScrollDescUp()					{ if(m_desc_scroll > 0)m_desc_scroll--; }
	void	ScrollDescDown()				{ if(m_desc_scroll  + m_desc_col < GetDescSize() )m_desc_scroll++; }
	void	SetDescScrollPos(int pos)		{ m_desc_scroll = pos; }
	int		GetDescScrollPos()				{ return m_desc_scroll; }
	int		GetDescPage()					{ return (m_desc.size()+m_desc_col-1)/m_desc_col; }
	int		GetDescSize()					{ return m_desc.size(); }
	int		GetDescCol()					{ return m_desc_col; }

public:
	C_VS_UI_DESC();
	~C_VS_UI_DESC();


};

#endif
