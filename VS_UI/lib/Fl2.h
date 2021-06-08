/*-----------------------------------------------------------------------------

	FL2.h

	FL2 master header file.

	2000.10.4. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __FL2_H__
#define __FL2_H__

#include "CI.h"
#include <string>
#include <ddraw.h>

void	g_SetFL2Surface(LPDIRECTDRAWSURFACE7 surface);

extern LPDIRECTDRAWSURFACE7	gpC_fl2_surface;
extern HDC gh_FL2_DC;

struct PrintInfo
{
	HFONT		hfont;
	COLORREF	text_color;
	COLORREF	back_color;
	int		bk_mode;
	UINT		text_align;
};

//----------------------------------------------------------------------------
// Public
//----------------------------------------------------------------------------
int	g_DBCSLen(const char_t * p_dbcs);
int	g_GetByteLenth(const char_t * p_dbcs, int dbcs_len);
int	g_Convert_DBCS_Ascii2SingleByte(const char_t * p_dbcs, int dbcs_len, char * &p_new_buf);
int	g_ConvertAscii2DBCS(const char * p_ascii, int ascii_len, char_t * &p_new_buf);
void	g_Print(int x, int y, const char * sz_str, PrintInfo * p_print_info=NULL);
int		g_GetStringWidth(const char * sz_str, HFONT hfont=NULL);
int		g_GetStringHeight(const char * sz_str, HFONT hfont=NULL);
bool	g_PossibleStringCut(const char * sz_str, int position);
bool	g_FL2_GetDC();
bool	g_FL2_ReleaseDC();
void	ReduceString(char *str,int len);
void	ReduceString2(char *str,int len);
void	ReduceString3(char *str,int len);
int g_PrintColorStr(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb);
int g_PrintColorStrOut(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb, COLORREF out_rgb);
int g_PrintColorStrShadow(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb, COLORREF shadow_rgb = 0xFFFFFFFF);
std::string g_GetNumberString(int number);
std::string g_GetStringByMoney(DWORD dwMoney);


#endif