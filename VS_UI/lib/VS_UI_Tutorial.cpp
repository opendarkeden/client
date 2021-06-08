// VS_UI_Tutorial.cpp by larosel

#include "client_PCH.h"
#include "VS_UI_Filepath.h"
//#include "VS_UI_GameCommon.h"
#include "VS_UI_Mouse_Pointer.h"
#include "VS_UI.h"
#include "VS_UI_Tutorial.h"
//#include "ex\DebugInfo.h"
#include "UserOption.h"
//#include "debuginfo.h"
#include "MGameStringTable.H"
//#define dSTRING_LEN 256 
#include "SXml.h"
extern RECT g_GameRect;

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::C_VS_UI_BOOKCASE
//
//
//-----------------------------------------------------------------------------

C_VS_UI_BOOKCASE::C_VS_UI_BOOKCASE()
{

	g_RegisterWindow(this);

	AttrTopmost(true);

	int w_x, w_y;

	m_pC_bookcase_spk = new C_SPRITE_PACK(SPK_TUTORIAL_BOOKCASE);
	w_x = g_GameRect.right/2 - m_pC_bookcase_spk->GetWidth(BOOKCASE_WINDOW)/2;
	w_y = 50;

//	std::string path;
//	path = TXT_TUTORIAL_BOOKROOT;
//	path += "\\*.txt";

//	WIN32_FIND_DATA fd;
//	HANDLE h = FindFirstFile( path.c_str(), &fd);
//	if( h == INVALID_HANDLE_VALUE)
//	{
//					printf("Cannot find file(s).");
//	return;
//	}

//	do 
//	{
//		char* name = fd.cFileName;
//		name = strrchr(name, '\\');
//		if(name)m_book_list.push_back(name);
//	} while( FindNextFile( h, &fd));
				
//	FindClose( h);

// BOOKLIST파일 LOAD
/*	assert(TXT_TUTORIAL_BOOKLIST);

	FILE *fp=fopen(TXT_TUTORIAL_BOOKLIST,"rt"); // 파일 열기 

	if(!fp)_Error(FAILED_JOB);

	fscanf(fp, "%d %d", &m_book_row, &m_book_col);

	char szLine[dSTRING_LEN]; 

	fgets(szLine, dSTRING_LEN, fp);
	szLine[0]=NULL; 

	while(fgets(szLine,dSTRING_LEN,fp))
	{
		int ret=strlen(szLine)-1;
		if(szLine[ret]=='\n')szLine[ret]='\0';
		if(szLine[0]=='\0')continue;
		m_book_list.push_back(szLine);
		szLine[0]=NULL;
	}

	fclose(fp);
*/

///////////////////////////////////////////////////////////
	m_book_row = 36;
	m_book_col = 19;

	CRarFile file(RPK_BOOK, RPK_PASSWORD);
	m_p_book_list = file.GetList();	
	
	int close_button_x = 621, close_button_y = 83;
	Set(w_x, w_y, m_pC_bookcase_spk->GetWidth(BOOKCASE_WINDOW), m_pC_bookcase_spk->GetHeight(BOOKCASE_WINDOW));

	m_books_left = w_x + 11; m_books_width = 605; m_books_height = 48;
	m_books_top[0] = w_y + 60; m_books_top[1] = w_y + 136; m_books_top[2] = w_y + 213; m_books_top[3] = w_y + 286;
	m_books_image_index[0] = 2; m_books_image_index[1] = 70; m_books_image_index[2] = 125; m_books_image_index[3] = 192;

	m_pC_book = NULL;
	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+close_button_x, y+close_button_y, m_pC_bookcase_spk->GetWidth(CLOSE_BUTTON), m_pC_bookcase_spk->GetHeight(CLOSE_BUTTON), BOOKCASE_CLOSE_ID, this, CLOSE_BUTTON));
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_BOOKCASE
//
//
//-----------------------------------------------------------------------------
C_VS_UI_BOOKCASE::~C_VS_UI_BOOKCASE()
{
	g_UnregisterWindow(this);

	if(m_p_book_list)
	{
		DeleteNew(m_p_book_list);
	}

	DeleteNew(m_pC_bookcase_spk);
	DeleteNew(m_pC_button_group);

	DeleteNew(m_pC_book);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::Process
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::UnacquireMouseFocus
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_book_x = m_focus_book_y = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	m_focus_book_x = NOT_SELECTED, m_focus_book_y = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::Finish()
{
	//if(m_pC_book){m_pC_book->Finish();m_pC_book = NULL; return;}

	CloseBook();	// by sigi	

	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
//
// Close Book  - by sigi
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::CloseBook()
{
	if (m_pC_book)
	{
		m_pC_book->Finish();
		DeleteNew( m_pC_book );
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_BOOKCASE::Show()
{
	RECT rect;
	unsigned int num;

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_bookcase_spk->BltLocked(x, y, BOOKCASE_WINDOW);
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	if(m_focus_book_x != NOT_SELECTED && m_focus_book_y != NOT_SELECTED)
	{
		int _x = m_books_left;
		int _y = m_books_top[m_focus_book_y];
		num = m_books_image_index[m_focus_book_y] + m_focus_book_x -2;
		
		for(int i = 0; i < m_focus_book_x; i++)_x += m_pC_bookcase_spk->GetWidth(m_books_image_index[m_focus_book_y] + i);
		
		m_pC_bookcase_spk->Blt(_x, _y, num+2);

		if((*m_p_book_list).size() > num)
		{
			int size = (*g_pGameStringTable)[UI_STRING_MESSAGE_BOOK_NAME_0+num].GetLength()+1;

			if (size > 0)
			{
				char *tempstr;
				tempstr = new char[size];
				ZeroMemory(tempstr, size);
//				memcpy(tempstr,(*m_p_book_list)[num].c_str(),size-1);
				memcpy( tempstr, (*g_pGameStringTable)[UI_STRING_MESSAGE_BOOK_NAME_0+num].GetString(), size-1 );
				tempstr[size-1] = 0;
				
				rect.right = g_GetStringWidth(tempstr, gpC_base->m_chatting_pi.hfont); //이함수 호출할때 외부에서 락걸면 안됨

				// 검은색 박스 출력..
	//			if (gpC_base->m_p_DDSurface_back->Lock())
				{		

					rect.left = _x;//+m_pC_bookcase_spk->GetWidth(num);
					rect.top = _y-16;
					rect.right += rect.left;//g_GetStringWidth(tempstr, gpC_base->m_chatting_pi.hfont);
					rect.bottom = rect.top+16;
	//				wsprintf(tempstr, "%d", rect.right);

					DrawAlphaBox(&rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

	//				gpC_base->m_p_DDSurface_back->Unlock();
				}

				g_PrintColorStr(_x, _y-16, tempstr, gpC_base->m_chatting_pi, YELLOW);	

				delete [] tempstr;
			}
		}

	}
	m_pC_button_group->ShowDescription();

//	SHOW_WINDOW_ATTR;
}


//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_help_string[1] =
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_BOOKCASE].GetString(),
	};
	
	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);

	if(p_button->GetFocusState() && p_button->GetPressState())
		m_pC_bookcase_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
					gpC_base->SendMessage(UI_CLOSE_BOOKCASE);		
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BOOKCASE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	
//	const MItem * p_selected_item;

 	switch (message)
	{
		case M_MOVING:
			{
				int i, j, px;

				m_focus_book_x = m_focus_book_y = NOT_SELECTED;

				for(i = 0; i < 4; i++)
					if(_x > m_books_left && _y >= m_books_top[i] && _x < (m_books_left + m_books_width) && _y < (m_books_top[i] + m_books_height))
					{
						m_focus_book_y = i;

						for(j = 0, px = _x - m_books_left; px > 0; j++)
							px -= m_pC_bookcase_spk->GetWidth(m_books_image_index[m_focus_book_y] + j);
						
						m_focus_book_x = j - 1;

//						if(m_focus_book_x != NOT_SELECTED && m_focus_book_y != NOT_SELECTED)
//						{
							//descriptor
//						}

						break;
					}
			}
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if(m_focus_book_x != NOT_SELECTED && m_focus_book_y != NOT_SELECTED && !(*m_p_book_list).empty())
			{
				unsigned int num = m_books_image_index[m_focus_book_y] + m_focus_book_x -2;
				if(num > (*m_p_book_list).size()-1)break;
				
				if(m_pC_book)DeleteNew(m_pC_book);
				
				if (m_pC_book!=NULL)
				{
					delete m_pC_book;
				}

				char szFilename[512];
				sprintf( szFilename, "book%d.txt", num );
				m_pC_book = new C_VS_UI_BOOK(/*(*m_p_book_list)[num].c_str()*/szFilename, m_book_row, m_book_col);
				m_pC_book->Start();
			}
			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BOOKCASE::IsPixel(int _x, int _y)
{
	return m_pC_bookcase_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), BOOKCASE_WINDOW);
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOKCASE::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOKCASE::Run(id_t id)
{
	switch (id)
	{
		case BOOKCASE_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_BOOKCASE);
			break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::C_VS_UI_BOOK
//
//
//-----------------------------------------------------------------------------
C_VS_UI_BOOK::C_VS_UI_BOOK(const char *szFilename, int row, int col):C_VS_UI_DESC()
{
	g_RegisterWindow(this);

	int w_x, w_y;

	m_col = col;
	fontx = 7;

	m_pC_book_spk = new C_SPRITE_PACK(SPK_TUTORIAL_BOOK);
	w_x = g_GameRect.right/2 - m_pC_book_spk->GetWidth(BOOK_WINDOW)/2;
	w_y = 30;
	Set(w_x, w_y, m_pC_book_spk->GetWidth(BOOK_WINDOW), m_pC_book_spk->GetHeight(BOOK_WINDOW));

///////////////////////////////////////////////

	m_close_x = 510; m_close_y = 435;
	m_prev_x = 29; m_next_x = 594; m_prev_y = 409; m_next_y = 408;

	m_char_x[0] = 40; m_char_x[1] = 360; m_char_y = 40; m_char_y_distance = 20;
	m_page_x[0] = m_char_x[0] +100, m_page_x[1] = m_char_x[1] +100, m_page_y = 400;
	m_page_width = 252;

	std::string filename;// = TXT_TUTORIAL_BOOKROOT;
	filename += szFilename;
	
	m_pack_file.SetRAR(RPK_BOOK, RPK_PASSWORD);
	LoadDesc(filename.c_str(), row, col);
	m_page_cur = 0;
	m_page_max = GetDescPage();


	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_close_x, y+m_close_y, m_pC_book_spk->GetWidth(CLOSE_BUTTON), m_pC_book_spk->GetHeight(CLOSE_BUTTON), BOOK_CLOSE_ID, this, CLOSE_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_prev_x, y+m_prev_y, m_pC_book_spk->GetWidth(PREV_BUTTON), m_pC_book_spk->GetHeight(PREV_BUTTON), BOOK_PREV_ID, this, PREV_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_next_x, y+m_next_y, m_pC_book_spk->GetWidth(NEXT_BUTTON), m_pC_book_spk->GetHeight(NEXT_BUTTON), BOOK_NEXT_ID, this, NEXT_BUTTON));

}

//-----------------------------------------------------------------------------
// ~C_VS_UI_BOOK
//
//
//-----------------------------------------------------------------------------
C_VS_UI_BOOK::~C_VS_UI_BOOK()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_book_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::Process
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::UnacquireMouseFocus
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();

}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}
/*
//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::ShowBook
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_BOOK::ShowBook()
{
	COLORREF markColor = RGB(0, 0, 0);

	char temp[100];
	char const *ptemp;
	int i, page, max;
	
	for(page = 0; page<2 && (m_page_cur+page)<(m_page_max); page++)
	{
		if(m_page_cur+page == m_page_max -1) max = m_book.size()-(m_page_cur+page)*m_col;
		else max = m_col;
		for(i=0; i<max;i++)
		{
			ptemp = m_book[(m_page_cur+page)*m_col +i].c_str();
			if(ptemp[0] == '^')
			{
				switch(ptemp[2])
				{
				case 'L':
					m_pC_inpicture->Blt(m_char_x[page], m_char_y + m_char_y_distance*i, ptemp[1]-'0');
					break;
					
				case 'R':
					m_pC_inpicture->Blt(m_char_x[page] + m_page_width - m_pC_inpicture->GetWidth(), m_char_y + m_char_y_distance*i, ptemp[1]-'0');
					break;
					
				case 'C':
					m_pC_inpicture->Blt((m_char_x[page]*2 + m_page_width)/2 - m_pC_inpicture->GetWidth()/2, m_char_y + m_char_y_distance*i, ptemp[1]-'0');
					break;
				}
				g_PrintColorStr(m_char_x[page], m_char_y + m_char_y_distance*i, ptemp +3, gpC_base->m_dialog_msg_pi, markColor);
			}
			else g_PrintColorStr(m_char_x[page], m_char_y + m_char_y_distance*i, ptemp, gpC_base->m_dialog_msg_pi, markColor);
		}
		
		wsprintf(temp,"%d/%d", m_page_cur+page+1, m_page_max);
		g_PrintColorStr(m_page_x[page], m_page_y, temp, gpC_base->m_chatting_pi, markColor);
	}

}
*/
//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_BOOK::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_book_spk->BltLocked(x, y, BOOK_WINDOW);
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	for(int page = 0; page<2 && (m_page_cur+page)<(m_page_max); page++)
	{
		char temp[10];

		SetDesc(x+m_char_x[page], y+m_char_y);
		SetDescScrollPos(m_col*(m_page_cur+page));
		ShowDesc();
		wsprintf(temp,"%d/%d", m_page_cur+page+1, m_page_max);
		g_PrintColorStr(x + m_page_x[page], y + m_page_y, temp, gpC_base->m_chatting_pi, RGB_BLACK);
	}
	
	m_pC_button_group->ShowDescription();
	
//	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_help_string[3] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_BOOK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_PREV_PAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NEXT_PAGE].GetString(),
	};
	
	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);

	
	if(p_button->GetFocusState() && p_button->GetPressState())
		m_pC_book_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);

}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BOOK::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BOOK::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_RETURN: // ok로 간주. !cancel은 반드시 ok와 함께 있다.
				Run(BOOK_NEXT_ID);
				break;

			case VK_ESCAPE: // 아무것도 안함.
				Run(BOOK_CLOSE_ID);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BOOK::IsPixel(int _x, int _y)
{
	return m_pC_book_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), BOOK_WINDOW);
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BOOK::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BOOK::Run(id_t id)
{
	switch (id)
	{
		case BOOK_CLOSE_ID:
			Finish();
			break;

		case BOOK_PREV_ID:
			if(m_page_cur > 0)m_page_cur-=2;
			break;
		
		case BOOK_NEXT_ID:
			if(m_page_cur < ((m_page_max-1)&(~1)))m_page_cur+=2;
			break;
	}
}
/*
////////////////////////////////////////////////////////////////////////
// C_VS_UI_BOOK_Load
// txt로 된 book파일을 불러온다.
////////////////////////////////////////////////////////////////////////
bool	C_VS_UI_BOOK::Load(const char *szFilename, int row, int col)
{
	assert(szFilename);

	std::string filename = TXT_TUTORIAL_BOOKROOT;
	std::string spkname = TXT_TUTORIAL_BOOKROOT;
	filename += szFilename;

	FILE *fp=fopen(filename.c_str(),"rt"); // 파일 열기 

	if(!fp)_Error(FAILED_JOB);
	assert(fp);

	char szLine[dSTRING_LEN]; 
	char temp[dSTRING_LEN];
	ZeroMemory(szLine, dSTRING_LEN);

	int fontx = 7;

	int check, check2, w=0, h=0, w2=0;
	char type;

	while(fgets(szLine,dSTRING_LEN,fp))
	{
		int ret=strlen(szLine)-1;
		if(szLine[ret] == '\n')szLine[ret]='\0';

		if(szLine[0] == '&')// 그림삽입 그림 파일명
		{
			spkname += &szLine[1];
			m_pC_inpicture = new C_SPRITE_PACK(spkname.c_str());
			assert(m_pC_inpicture);
			continue;
		}

		if(szLine[0] == '^')//그림삽입 그림 태그
		{
			int num = szLine[1] - '0';
			w = (m_pC_inpicture->GetWidth(num) + fontx -1)/fontx;
			h = (m_pC_inpicture->GetHeight(num) + m_char_y_distance -1)/m_char_y_distance;
			type = szLine[2];

			if(type == 'C')
			{
				strcpy(temp, szLine);
				szLine[3] = '\0';
				m_book.push_back(szLine);
				strcpy(szLine, temp +3);
				h--;
				while(h>0)
				{
					m_book.push_back("");
					h--;
				}
//				continue;
			}

		}

		if(h)w2=w; else w2=0;
		if(szLine[0] == '^')check2=3; else check2=0;

		while(strlen(szLine)-check2 > row -w2)
		{
			if(!g_PossibleStringCut(szLine, row -w2 +check2))check = 1; else check=0;

			strcpy(temp, szLine);
			if(type == 'L' && h)
			{
				memset(szLine+check2, (int)' ', dSTRING_LEN-check2);
				strcpy(szLine+w2+check2, temp+check2);
				szLine[row -check +check2] = '\0';
			}
			else
				szLine[row -w2 -check +check2] = '\0';

			m_book.push_back(szLine);
			if(temp[row -check -w2 +check2] == ' ')strcpy(szLine, temp+row -check -w2 +check2 +1);
			else strcpy(szLine, temp+row -check -w2 +check2);

			if(h)
				if(--h == 0)w2=0;
			check2 = 0;
		}

		if(h)
		{
			if(type == 'L')
			{
				strcpy(temp, szLine);
				memset(szLine+check2, (int)' ', dSTRING_LEN-check2);
				strcpy(szLine+w2+check2, temp+check2);
			}
			if(--h == 0)w2=0;
		}

		m_book.push_back(szLine);
		
		ZeroMemory(szLine, dSTRING_LEN);
	}

	fclose(fp);

	return TRUE;

}
*/
//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::C_VS_UI_BRIEFING
//
//-----------------------------------------------------------------------------
/*
C_VS_UI_BRIEFING::C_VS_UI_BRIEFING()
{
	g_RegisterWindow(this);

	AttrTopmost(true);

	int w_x, w_y;

	m_pC_screen_spk = new C_SPRITE_PACK(SPK_TUTORIAL_BRIEFING);
	w_x = g_GameRect.right/2 - m_pC_screen_spk->GetWidth(SCREEN_WINDOW)/2;
	w_y = 30;

	int close_button_x = 248, close_button_y = 411;
	Set(w_x, w_y, m_pC_screen_spk->GetWidth(SCREEN_WINDOW), m_pC_screen_spk->GetHeight(SCREEN_WINDOW));

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + close_button_x, y+close_button_y, m_pC_screen_spk->GetWidth(CLOSE_BUTTON), m_pC_screen_spk->GetHeight(CLOSE_BUTTON), SCREEN_CLOSE_ID, this, CLOSE_BUTTON));

	// TIMER
	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 50;
	m_scroll_max = 0;

	m_char_x = 30;
	m_char_y = 20;
	m_char_y_distance = 20;

//	BRIEFING파일 LOAD
	m_briefing_file.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);

	assert(TXT_TUTORIAL_BRIEFING);
	Load(TXT_TUTORIAL_BRIEFING);

	m_scroll = -m_scroll_max;
	m_scroll_detail = 0;

}

//-----------------------------------------------------------------------------
// ~C_VS_UI_BRIEFING
//
//-----------------------------------------------------------------------------
C_VS_UI_BRIEFING::~C_VS_UI_BRIEFING()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_screen_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_BRIEFING::Show()
{
	COLORREF markColor = RGB(0, 0, 0);
	Rect rect1(0,m_char_y-m_char_y_distance,m_pC_screen_spk->GetWidth(),m_char_y_distance);
	Rect rect2(0,m_char_y-m_char_y_distance+ m_scroll_max*m_char_y_distance,m_pC_screen_spk->GetWidth(),m_char_y_distance);

	m_pC_screen_spk->Blt(x, y);

	if(m_scroll < (int)m_txt_briefing.size())
	{
	Timer();
	for(int i = 0; i < m_scroll_max; i++)
		if(m_scroll + i >= 0 && m_scroll + i < m_txt_briefing.size())
		{
			
			if(m_txt_briefing[i+m_scroll].size())g_PrintColorStr(x + m_char_x, y + m_char_y + m_char_y_distance*i-m_scroll_detail, m_txt_briefing[i+m_scroll].c_str(), gpC_base->m_desc_msg_pi, markColor);
		}
	m_pC_screen_spk->BltClip(x, y-rect1.y, rect1);
	m_pC_screen_spk->BltClip(x, y-rect2.y, rect2);

	}

	m_pC_button_group->Show();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);

	static char	m_help_string[1][30] = {
		"Briefing창을 닫습니다",
	};

	if (p_button->GetFocusState())
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);

	if(p_button->GetFocusState() && p_button->GetPressState())m_pC_screen_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
					gpC_base->SendMessage(UI_CLOSE_BRIEFING);		
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BRIEFING::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BRIEFING::IsPixel(int _x, int _y)
{
	if(m_pC_screen_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), SCREEN_WINDOW))
		return true;

	if(m_pC_button_group->IsInRect(_x, _y))
		return true;

	return false;
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BRIEFING::Run(id_t id)
{
	switch (id)
	{
		case SCREEN_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_BRIEFING);
			break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_BRIEFING::Timer()
{
	if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_dw_prev_tickcount = GetTickCount();
		m_scroll_detail++;
		if(m_scroll_detail == 20)
		{
			m_scroll++;
			m_scroll_detail = 0;
		}
		
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////
// C_VS_UI_BRIEFING::Load
// txt로 된 브리핑내용을 불러온다.
////////////////////////////////////////////////////////////////////////
bool	C_VS_UI_BRIEFING::Load(const char *szFilename)
{
	int row;
	assert(szFilename);

	m_briefing_file.Open(szFilename);
//	FILE *fp=fopen(szFilename,"rt"); // 파일 열기 

//	if(!fp)_Error(FAILED_JOB);
//	assert(fp);

	char szLine[dSTRING_LEN]; 
	char temp[dSTRING_LEN];
	ZeroMemory(szLine, dSTRING_LEN);
	
	m_briefing_file.GetString(szLine, dSTRING_LEN);

	sscanf(szLine,"%d %d", &row, &m_scroll_max);

	int check;

	while(m_briefing_file.GetString(szLine, dSTRING_LEN))
	{
//		ret = strlen(szLine)-1;
//		if(szLine[ret] == '\n')szLine[ret]='\0';

		while(strlen(szLine) > row)
		{
			if(!g_PossibleStringCut(szLine, row))check = 1; else check=0;

			strcpy(temp, szLine);
			szLine[row -check] = '\0';

			m_txt_briefing.push_back(szLine);
			if(temp[row -check] == ' ')strcpy(szLine, temp+row -check +1);
			else strcpy(szLine, temp+row -check);
		}

		m_txt_briefing.push_back(szLine);
		
		ZeroMemory(szLine, dSTRING_LEN);
		ZeroMemory(temp, dSTRING_LEN);
	}

//	fclose(fp);
	m_briefing_file.Release();

	return TRUE;

}
*/
//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::C_VS_UI_COMPUTER
//
//-----------------------------------------------------------------------------

C_VS_UI_COMPUTER::C_VS_UI_COMPUTER():C_VS_UI_DESC()
{
	if(gpC_base == NULL) return;

	g_RegisterWindow(this);

	AttrTopmost(true);

	int w_x, w_y;

	m_pC_monitor_spk = new C_SPRITE_PACK(SPK_TUTORIAL_COMPUTER);
	w_x = g_GameRect.right/2 - m_pC_monitor_spk->GetWidth(MONITOR_WINDOW)/2;
	w_y = 30;

	int close_button_x = 414, close_button_y = 398;
	int scroll_x = 418, scroll_up_y = 35, scroll_down_y = 309;
	Set(w_x, w_y, m_pC_monitor_spk->GetWidth(MONITOR_WINDOW), m_pC_monitor_spk->GetHeight(MONITOR_WINDOW));

	m_scroll_x = scroll_x;
	m_scroll_y = scroll_up_y +10;
	m_scroll_h = scroll_down_y - scroll_up_y -20;

	m_tree_limit_h = 16;

	m_tree_scroll = 0;
	m_tree_scroll_x = 173;

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+close_button_x, y+close_button_y, m_pC_monitor_spk->GetWidth(CLOSE_BUTTON), m_pC_monitor_spk->GetHeight(CLOSE_BUTTON), MONITOR_CLOSE_ID, this, CLOSE_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+scroll_x, y+scroll_up_y, m_pC_monitor_spk->GetWidth(SCROLL_UP), m_pC_monitor_spk->GetHeight(SCROLL_UP), SCROLL_UP_ID, this, SCROLL_UP));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+scroll_x, y+scroll_down_y, m_pC_monitor_spk->GetWidth(SCROLL_DOWN), m_pC_monitor_spk->GetHeight(SCROLL_DOWN), SCROLL_DOWN_ID, this, SCROLL_DOWN));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_tree_scroll_x, y+scroll_up_y, m_pC_monitor_spk->GetWidth(SCROLL_UP), m_pC_monitor_spk->GetHeight(SCROLL_UP), SCROLL_TREE_UP_ID, this, SCROLL_UP));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_tree_scroll_x, y+scroll_down_y, m_pC_monitor_spk->GetWidth(SCROLL_DOWN), m_pC_monitor_spk->GetHeight(SCROLL_DOWN), SCROLL_TREE_DOWN_ID, this, SCROLL_DOWN));

	m_index_x = 40;
	m_index_y = 36;
	m_index_distance = 18;
	m_index_max = 0;

	m_isclose = false;
	m_filename = "";

	m_timer = false;

	std::string temp;

	SetDesc(x+190, y+70, RGB_GREEN, gpC_base->m_chatting_pi);
	SetDescTitle(x+190, y+40, RGB_YELLOW, gpC_base->m_chatting_pi);

	m_pack_file.SetRAR(RPK_ITEM, RPK_PASSWORD);
	m_computer_tree_file.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);
	if(LoadTree(TXT_TUTORIAL_COMPUTER) == false)return;

	// TIMER

	DWORD currentTime = GetTickCount();

	m_dw_prev_tickcount = currentTime;
	
	m_dw_millisec = 50;


}

//-----------------------------------------------------------------------------
// ~C_VS_UI_COMPUTER
//
//-----------------------------------------------------------------------------
C_VS_UI_COMPUTER::~C_VS_UI_COMPUTER()
{

	g_UnregisterWindow(this);

	DeleteNew(m_pC_monitor_spk);
	DeleteNew(m_pC_button_group);

}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::Start()
{
	PI_Processor::Start();
	if(m_pC_button_group == NULL)
		return;
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::Finish()
{

	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);

}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_COMPUTER::Show()
{
	if(gpC_base == NULL || m_pC_button_group == NULL)return;

	COLORREF color = RGB(0,255,0);
	int scroll;
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_monitor_spk->BltLocked(x, y, MONITOR_WINDOW);
		if(GetDescSize() > GetDescCol())
		{
			if(GetDescSize() == GetDescCol())
				scroll = m_scroll_h*GetDescScrollPos();
			else
				scroll = m_scroll_h*GetDescScrollPos()/(GetDescSize()-GetDescCol());
			
			m_pC_monitor_spk->BltLocked(x+m_scroll_x, y+m_scroll_y + scroll, SCROLL_TAG);
		}
		
		if(m_tree_scroll <= m_index_max - m_tree_limit_h)
		{
			if(m_index_max == m_tree_limit_h)
				scroll = m_scroll_h*m_tree_scroll;
			else
				scroll = m_scroll_h*m_tree_scroll/(m_index_max-m_tree_limit_h);
			m_pC_monitor_spk->BltLocked(x+m_tree_scroll_x, y+m_scroll_y + scroll, SCROLL_TAG);
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	int limit = m_tree.size()+1;
	Timer();
	m_isclose = false;

	m_index_max = PrintTree(0, 0, m_index_distance, 0, limit, false, m_tree);

	ShowDesc();

	g_PrintColorStr(m_desc_title_x, m_desc_title_y + 15, m_second_title_string.c_str(), gpC_base->m_chatting_pi, RGB(200, 200, 200));

	m_pC_button_group->ShowDescription();	

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_help_string[1] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_QUIT_COMPUTER].GetString(),
	};
	
	if (p_button->GetID() == MONITOR_CLOSE_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[0],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{

	assert(p_button);
	if(p_button->GetFocusState() && p_button->GetPressState())
		m_pC_monitor_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
					gpC_base->SendMessage(UI_CLOSE_COMPUTER);		
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_COMPUTER::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	switch (message)
	{
	case M_WHEEL_UP:
		if(_x < 203+x)
			Run(SCROLL_TREE_UP_ID);
		else
			Run(SCROLL_UP_ID);
		break;

	case M_WHEEL_DOWN:
		if(_x < 203+x)
			Run(SCROLL_TREE_DOWN_ID);
		else
			Run(SCROLL_DOWN_ID);
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x > x+m_index_x && _y > y+m_index_y && _x < x+m_index_x+100 && _y < y+m_index_y + min(m_tree_limit_h, m_index_max)*m_index_distance)
		{
			int index = (_y - (y+m_index_y))/m_index_distance+1+m_tree_scroll;
			if(!m_isclose)OpenTree(index, m_tree);
			Timer(0);
		}
		break;
	}

	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_COMPUTER::IsPixel(int _x, int _y)
{
	return m_pC_monitor_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MONITOR_WINDOW);
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::Run(id_t id)
{
	switch (id)
	{
	case SCROLL_UP_ID:
		ScrollDescUp();
		break;
		
	case SCROLL_DOWN_ID:
		ScrollDescDown();
		break;
		
	case SCROLL_TREE_UP_ID:
		if(m_tree_scroll > 0)m_tree_scroll--;
		break;

	case SCROLL_TREE_DOWN_ID:
		if(m_tree_scroll < m_index_max - m_tree_limit_h)m_tree_scroll++;
		break;

	case MONITOR_CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_COMPUTER);
		break;
	}
}

////////////////////////////////////////////////////////////////////////
// C_VS_UI_COMPUTER_LOAD_TREE
// tre(text)로 된 트리파일을 불러온다.
////////////////////////////////////////////////////////////////////////
//char szLine[dSTRING_LEN+1]; 
	
bool	C_VS_UI_COMPUTER::LoadTree(const char* szFilename)
{
	assert(szFilename);

	std::string spkname = _ROOT"spk\\";

//	FILE *fp=fopen(szFilename,"rt"); // 파일 열기 

//	if(!fp)_Error(FAILED_JOB);
//	assert(fp);

//	ZeroMemory(szLine, dSTRING_LEN);

	m_computer_tree_file.Open(szFilename);

	XMLTree computerTree;
	XMLParser parser;
	
	parser.parse( (char *)m_computer_tree_file.GetFilePointer(), &computerTree );

	const XMLTree *pSizeElement = computerTree.GetChild( "Size" );
	if( pSizeElement != NULL )
	{
		const XMLAttribute *pWidthAttr = pSizeElement->GetAttribute( "width" );
		if( pWidthAttr != NULL )
		{
			m_row = pWidthAttr->ToInt();
		}
		const XMLAttribute *pHeightAttr = pSizeElement->GetAttribute( "height" );
		if( pHeightAttr != NULL )
		{
			m_col = pHeightAttr->ToInt();
		}
	}

	const XMLTree *pImageFileNameElement = computerTree.GetChild( "ImageFilename" );
	if( pImageFileNameElement != NULL )
	{
		spkname += pImageFileNameElement->GetText();

		CSpritePack *temp;
		temp = new CSpritePack;
		ifstream file(spkname.c_str(), ios::binary | ios::nocreate);
		if (!file)
			_Error(FILE_OPEN);
		temp->LoadFromFile( file );
		file.close();
		m_pC_inpicture.push_back(temp);
	}

//	int depth = 0;
//	CTREE *cur_tree = &m_tree;
//	CTREE *old_tree[4];
//	C_TREE_BASE temp_tree;

	const XMLTree *pImageListElement = computerTree.GetChild( "ItemList" );
	if( pImageListElement != NULL )
	{
		const size_t itemSize = pImageListElement->GetChildCount();

		for( size_t itemIndex = 0; itemIndex < itemSize; itemIndex++ )
		{
			const XMLTree *pItemElement = pImageListElement->GetChild( itemIndex );
			if( pItemElement != NULL )
			{
				C_TREE_BASE temp_tree;
				const int id = atoi(pItemElement->GetText().c_str());
				temp_tree.str = (*g_pGameStringTable)[UI_STRING_MESSAGE_COMPUTER_STRING_0+id];
				temp_tree.filename.Format( "Computer%d.txt", id);
				const size_t itemSize2 = pItemElement->GetChildCount();
				
				for( size_t itemIndex2 = 0; itemIndex2 < itemSize2; itemIndex2++ )
				{
					const XMLTree *pItemElement2 = pItemElement->GetChild( itemIndex2 );
					if( pItemElement2 != NULL )
					{
						C_TREE_BASE temp_tree2;
						const int id = atoi(pItemElement2->GetText().c_str());
						temp_tree2.str = (*g_pGameStringTable)[UI_STRING_MESSAGE_COMPUTER_STRING_0+id];
						temp_tree2.filename.Format( "Computer%d.txt", id);
						const size_t itemSize3 = pItemElement->GetChildCount();

						for( size_t itemIndex3 = 0; itemIndex3 < itemSize3; itemIndex3++ )
						{
							const XMLTree *pItemClassElement = pItemElement2->GetChild( itemIndex3 );

							if( pItemClassElement != NULL )
							{
								C_TREE_BASE temp_tree3;
								temp_tree3.str = pItemClassElement->GetText();
								
								ITEM_CLASS itemClass = (ITEM_CLASS)atoi(pItemClassElement->GetText().c_str());

								if(	itemClass == ITEM_CLASS_NULL || itemClass >= MAX_ITEM_CLASS) return false;

								size_t item_list_size = (*g_pItemTable)[(ITEM_CLASS)itemClass].GetSize();
								
								for( size_t itemIndex4 = 0; itemIndex4 < item_list_size; itemIndex4++ )
								{
									temp_tree2.Tree.push_back(temp_tree3);
								}
							}
						}
						temp_tree.Tree.push_back(temp_tree2);
					}
				}
				m_tree.push_back(temp_tree);
			}
		}
	}

//	m_computer_tree_file.GetString(szLine, dSTRING_LEN);
//	sscanf(szLine, "%d %d", &m_row, &m_col);
//
//	int depth = 0;
//	CTREE *cur_tree = &m_tree;
//	CTREE *old_tree[4];
//	C_TREE_BASE temp_tree;
//
////	fgets(szLine,dSTRING_LEN,fp);
//
//	while(m_computer_tree_file.GetString(szLine, dSTRING_LEN))
//	{
//		szLine[dSTRING_LEN] = '\0';
//	
//		int ret = strlen(szLine)-1;
//		if(szLine[ret] == '\n')szLine[ret] = '\0';
//
//		for(int i=0; i < dSTRING_LEN && i < ret+1 && (szLine[i] == ' ' || szLine[i] == '\t'); i++);
//		
//		if(szLine[0] == '&')// 그림삽입 그림 파일명
//		{
//			spkname += &szLine[1];
//
//			CSpritePack *temp;
//			temp = new CSpritePack;
//			ifstream file(spkname.c_str(), ios::binary | ios::nocreate);
//			if (!file)
//				_Error(FILE_OPEN);
//			temp->LoadFromFile( file );
//			file.close();
//			m_pC_inpicture.push_back(temp);
//
//			continue;
//		}
//		
//		if(szLine[i] == '{')
//		{		
////			if(depth < 3)
//				old_tree[depth++] = cur_tree;
//			cur_tree = &((*cur_tree)[(*cur_tree).size()-1].Tree);
//		}
//		else if(szLine[i] == '}')
//		{
////			if(depth > 4)
//			cur_tree = old_tree[--depth];
//		}
//		else if(szLine[i] != '\0')
//		{
//			temp_tree.str = &szLine[i];
//			int itemClass = atoi(temp_tree.str.c_str());
//			if(itemClass != ITEM_CLASS_NULL && itemClass != 0)
//			{
//				if(	itemClass >= MAX_ITEM_CLASS) return false;
//
//				int item_list_size = (*g_pItemTable)[(ITEM_CLASS)itemClass].GetSize();
//				for(int i = 0; i < item_list_size; i++)
//				{
//					if(cur_tree)
//						(*cur_tree).push_back(temp_tree);
//				}
//			}
//			else
//			{
//				if(cur_tree)
//				(*cur_tree).push_back(temp_tree);
//			}
//		}
//		
//		ZeroMemory(szLine, dSTRING_LEN);
//	}
////	fclose(fp);
	m_computer_tree_file.Release();
	
	return TRUE;
	
}

////////////////////////////////////////////////////////////////////////
// C_VS_UI_COMPUTER_PrintTree
// 트리를 화면에 출력한다.
////////////////////////////////////////////////////////////////////////
int	C_VS_UI_COMPUTER::PrintTree(int _x, int _y, int y_distance, int depth, int &limit, bool close, std::vector<C_TREE_BASE> &tree)
{
	COLORREF color[4] = 
	{
		RGB(100,225,100),
			RGB(150,157,231),
			RGB(206,130,164),
			RGB(159,120,237),
	};
	
	int max = tree.size();
	//	if(max == 0)return max;
	int i, open = 0, y_plus = 0;
	
	if(limit == -1){limit = 0; return 0;}
	
	int itemType = 0;
	ITEM_CLASS old_itemClass = (ITEM_CLASS)0;
	
	for(i = 0; i < abs(limit)-1 && i < max; i++)
	{
		if(tree[i].open)open++;
	}
	
	for(i = 0; i < abs(limit)-1 && i < max; i++)
	{
		std::string sss = tree[i].str;
		ITEM_CLASS itemClass = (ITEM_CLASS)atoi(tree[i].str.c_str());
		if(old_itemClass != itemClass)itemType = 0;
		old_itemClass = itemClass;

		if(_y + (i+y_plus)*y_distance < (m_tree_limit_h+m_tree_scroll)*y_distance && _y+(i+y_plus-m_tree_scroll)*y_distance >= 0 )
		{
			if(tree[i].select)m_pC_monitor_spk->Blt(x + m_index_x + _x -4, y + m_index_y + _y + (i+y_plus-m_tree_scroll)*y_distance -3, SELECTED_GRID);
			else m_pC_monitor_spk->Blt(x + m_index_x + _x -4, y + m_index_y + _y + (i+y_plus-m_tree_scroll)*y_distance -3, NON_SELECTED_GRID);
			
			if(tree[i].Tree.empty())
			{
				MItem* pItem = MItem::NewItem( itemClass );
				pItem->SetItemType( itemType );
				
				if(strcmp(pItem->GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_INFRA_RED_HELMET].GetString()) == 0)
				{
					char sz_temp[50];
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_INFRA_HELMET].GetString());
					g_PrintColorStr(x + m_index_x + _x, y + m_index_y + _y + (i+y_plus-m_tree_scroll)*y_distance, sz_temp, gpC_base->m_chatting_pi, color[depth]);	
				}
				else
					g_PrintColorStr(x + m_index_x + _x, y + m_index_y + _y + (i+y_plus-m_tree_scroll)*y_distance, pItem->GetName(), gpC_base->m_chatting_pi, color[depth]);	
				delete pItem;
			}
			else
				g_PrintColorStr(x + m_index_x + _x, y + m_index_y + _y + (i+y_plus-m_tree_scroll)*y_distance, tree[i].str.c_str(), gpC_base->m_chatting_pi, color[depth]);	
		}
		
		if(abs(tree[i].open)>0)y_plus += PrintTree(_x+5, _y + (i+1+y_plus)*y_distance, y_distance, depth +1, tree[i].open, ((open > 1)?true:false), tree[i].Tree);
		itemType++;
	}	
	
	if(limit <= max)m_isclose = true;

	if(limit <= max && m_timer && !y_plus)
	{
		if(limit > 0 && !close)
			limit++;
		else if(limit < 0){limit++;y_plus++;}

	}

	return abs(limit)-1+y_plus;
}

////////////////////////////////////////////////////////////////////////
// C_VS_UI_COMPUTER_OpenTree
// 트리를 index 번호에 따라 연다.
////////////////////////////////////////////////////////////////////////
bool C_VS_UI_COMPUTER::OpenTree(int &index, std::vector<C_TREE_BASE> &tree)
{
	int max = tree.size();
	bool re = false;

	int itemType = 0;
	ITEM_CLASS old_itemClass = (ITEM_CLASS)0;

	for(int i = 0; i < max; i++)
	{
		index--;

		ITEM_CLASS itemClass = (ITEM_CLASS)atoi(tree[i].str.c_str());
		if(itemClass != old_itemClass)itemType = 0;
		old_itemClass = itemClass;

		if(index == 0)	// 찾았다
		{
			if(tree[i].open > 0)	// 트리가 열려있는경우
			{
				tree[i].open = -tree[i].open;
				OpenTree(index, tree[i].Tree);
			}
			else // 트리가 닫혀있는경우
			{
				tree[i].open = 1;
				OpenTree(index, tree[i].Tree);
			}

			tree[i].select = true;
			std::string filename;// = TXT_ITEMROOT;
			if(tree[i].Tree.empty())
			{
				MItem* pItem = MItem::NewItem( itemClass );
				pItem->SetItemType( itemType );

				filename += pItem->GetEName();
				filename += ".txt";

				char sz_temp[100];
				wsprintf(sz_temp, "%s", pItem->GetName());
				m_second_title_string = "( ";
				m_second_title_string += +pItem->GetEName();
				m_second_title_string += " )";
				SetDescTitle(sz_temp);

				delete pItem;
			}
			else
			{
				filename = tree[i].filename;
				SetDescTitle(tree[i].str.c_str());
				m_second_title_string = "";
			}

			LoadDesc(filename.c_str(), m_row, m_col, false);

			re = true;
		}
		else if(tree[i].open)
		{
			if(OpenTree(index, tree[i].Tree))
			{
				tree[i].open = tree[i].Tree.size()+1;
				tree[i].select = false;
				re = true;
			}
			else
			{
				tree[i].open = -tree[i].open;
				tree[i].select = false;
			}
		}
		else tree[i].select = false;

		itemType++;
	}
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_COMPUTER::Timer
//
//-----------------------------------------------------------------------------
void	C_VS_UI_COMPUTER::Timer(bool reset)
{
	if(reset)
	{
		m_timer = false;
		m_dw_prev_tickcount = GetTickCount();
	}
	else if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_timer = true;
		m_dw_prev_tickcount = GetTickCount();
	}
	else m_timer = false;

}
/*
//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::C_VS_UI_TUTORIAL_EXIT
//
//-----------------------------------------------------------------------------

C_VS_UI_TUTORIAL_EXIT::C_VS_UI_TUTORIAL_EXIT()
{
	g_RegisterWindow(this);

	AttrTopmost(true);

	int w_x, w_y;

	m_pC_exit_spk = new C_SPRITE_PACK(SPK_TUTORIAL_EXIT);
	m_pC_item_spk =  new C_SPRITE_PACK(SPK_ITEM);
	w_x = g_GameRect.right/2 - m_pC_exit_spk->GetWidth(EXIT_WINDOW)/2;
	w_y = 100;

	int ok_button_x = 120, ok_button_y = 5;
	int cancel_button_x = 120, cancel_button_y = 30;
	int select_button_x = 11, select_button_y = 120, select_button_x_distance = 37, select_button_y_distance = 25;

	Set(w_x, w_y, m_pC_exit_spk->GetWidth(EXIT_WINDOW), m_pC_exit_spk->GetHeight(EXIT_WINDOW));

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + ok_button_x, y + ok_button_y, m_pC_exit_spk->GetWidth(OK_BUTTON), m_pC_exit_spk->GetHeight(OK_BUTTON), OK_ID, this, OK_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + cancel_button_x, y + cancel_button_y, m_pC_exit_spk->GetWidth(CANCEL_BUTTON), m_pC_exit_spk->GetHeight(CANCEL_BUTTON), CANCEL_ID, this, CANCEL_BUTTON));

	for(int i = 0, select = BLADE_BUTTON, exec_id = SELECT_BLADE_ID; i < 8; i++, select++, exec_id++)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + select_button_x + (i%4)*select_button_x_distance, y + select_button_y + (i/4)*select_button_y_distance, m_pC_exit_spk->GetWidth(select), m_pC_exit_spk->GetHeight(select), exec_id, this, select));
	}

	m_select = -1;

}

//-----------------------------------------------------------------------------
// ~C_VS_UI_TUTORIAL_EXIT
//
//-----------------------------------------------------------------------------
C_VS_UI_TUTORIAL_EXIT::~C_VS_UI_TUTORIAL_EXIT()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_exit_spk);
	DeleteNew(m_pC_item_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TUTORIAL_EXIT::Show()
{
	int item_num[8] = {10, 0, 25, 20, 50, 60, 40, 30, };

	m_pC_exit_spk->Blt(x, y);
	if(m_select != -1)m_pC_item_spk->Blt(x +6, y +5, item_num[m_select -2]);

	m_pC_button_group->Show();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);

	static char	m_help_string[2][30] = {
		"무기를 결정합니다",
		"무기선택을 취소합니다",
	};

	if ((p_button->GetID() == OK_ID || p_button->GetID() == CANCEL_ID) && p_button->GetFocusState())
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);

	if((p_button->GetFocusState() && p_button->GetPressState()) || p_button->GetID() == m_select)
		m_pC_exit_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
					gpC_base->SendMessage(UI_CLOSE_TUTORIAL_EXIT);		
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TUTORIAL_EXIT::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TUTORIAL_EXIT::IsPixel(int _x, int _y)
{
	return m_pC_exit_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), EXIT_WINDOW);
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::Run(id_t id)
{
	switch (id)
	{
	case OK_ID:
		{
			if(m_select == -1)return;
			static char select_name[8][20] = {"도", "검", "메이스", "십자가", "AR", "TR", "SMG", "SG"};
			
			// message 필요없음 by sigi
			//gpC_base->SendMessage(UI_CONFIRM_TUTORIAL_EXIT, m_select-2, 0, select_name[m_select-2]);
			gC_vs_ui.RunTutorialExitAsk(m_select-2, select_name[m_select-2]);
		}
		break;
		
	case CANCEL_ID:
		gpC_base->SendMessage(UI_CLOSE_TUTORIAL_EXIT);
		break;

	case SELECT_BLADE_ID:
	case SELECT_SWORD_ID:
	case SELECT_CROSS_ID:
	case SELECT_MACE_ID:
	case SELECT_AR_ID:
	case SELECT_TR_ID:
	case SELECT_SMG_ID:
	case SELECT_SG_ID:
		m_select = id;
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TUTORIAL_EXIT::Accept
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TUTORIAL_EXIT::Accept()
{
	//gpC_base->SendMessage(UI_TUTORIAL_EXIT_SELECT, m_select-2);
	//gpC_base->SendMessage(UI_CLOSE_TUTORIAL_EXIT);

	// message 하나만..  by sigi
	gpC_base->SendMessage(UI_CLOSE_TUTORIAL_EXIT, m_select-2);
}

*/