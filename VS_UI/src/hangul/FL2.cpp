// FL2.cpp

#include "client_PCH.h"

#include "FL2.h"
#include "VS_UI_Base.h"

//-----------------------------------------------------------------------------
// Utilities for IO.
//-----------------------------------------------------------------------------

//


//


//
LPDIRECTDRAWSURFACE7	gpC_fl2_surface = NULL;
HDC gh_FL2_DC = NULL;

//-----------------------------------------------------------------------------
// g_SetFL2Surface
//
// 
//-----------------------------------------------------------------------------
void g_SetFL2Surface(LPDIRECTDRAWSURFACE7 surface)
{
	gpC_fl2_surface = surface;
}

//-----------------------------------------------------------------------------
// g_PossibleStringCut
//


//-----------------------------------------------------------------------------
bool g_PossibleStringCut(const char * sz_str, int position)
{
	if (sz_str != NULL)
	{
		if ((int)strlen(sz_str) <= position)
			return true;

		//

		//
		// (1) ASCII


		//


		//
		enum _CODE
		{
			_ENG,
			_HAN,
		};

		_CODE code = _ENG;
		int i;
		for (i=0; i <= position; i++)
		{
			if ((BYTE)sz_str[i] > 128) // no ASCII?
			{
				i++;
				code = _HAN;
			}
			else
				code = _ENG;
		}

		if (code == _HAN)
			if (i <= position+1)
				return false;

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// g_GetStringWidth
//


//

//-----------------------------------------------------------------------------
int g_GetStringWidth(const char * sz_str, HFONT hfont)
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_fl2_surface == NULL)
		_Error(NULL_REF);

	if (sz_str == NULL)
		return 0;

	HDC hdc;

	bool bGetDC = g_FL2_GetDC();

	hdc = gh_FL2_DC;
	
	if (hfont != NULL)
		SelectObject(hdc, hfont);

	SIZE size;
	GetTextExtentPoint32(hdc, sz_str, strlen(sz_str), &size);

	if(bGetDC)
		g_FL2_ReleaseDC();

	return size.cx;
}

//-----------------------------------------------------------------------------
// g_GetStringHeight
//


//

//-----------------------------------------------------------------------------
int g_GetStringHeight(const char * sz_str, HFONT hfont)
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_fl2_surface == NULL)
		_Error(NULL_REF);

	if (sz_str == NULL)
		return 0;
	
	HDC hdc;

	bool bGetDC = g_FL2_GetDC();

	hdc = gh_FL2_DC;
	
	
	if (hfont != NULL)
		SelectObject(hdc, hfont);

	SIZE size;
	GetTextExtentPoint32(hdc, sz_str, strlen(sz_str), &size);

	if(bGetDC)
		g_FL2_ReleaseDC();

	return size.cy;
}

//-----------------------------------------------------------------------------
// g_Print
//


//

//-----------------------------------------------------------------------------
void g_Print(int x, int y, const char * sz_str, PrintInfo * p_print_info)
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (sz_str!=NULL)
	{
		//assert(sz_str != NULL);
		assert(gpC_fl2_surface != NULL);

		// no Release exception handling code for speed.

		HDC hdc;

		bool bGetDC = g_FL2_GetDC();

		hdc = gh_FL2_DC;

		if (p_print_info != NULL)
		{
			SetTextAlign(hdc, p_print_info->text_align);
			SetBkMode(hdc, p_print_info->bk_mode);
			SetBkColor(hdc, p_print_info->back_color);
			SetTextColor(hdc, p_print_info->text_color);
			SelectObject(hdc, p_print_info->hfont);
		}

		TextOut(hdc, x, y, sz_str, strlen(sz_str));

		if(bGetDC)
			g_FL2_ReleaseDC();

	}
}

//-----------------------------------------------------------------------------
// g_DBCSLen
//

//-----------------------------------------------------------------------------
int g_DBCSLen(const char_t * p_dbcs)
{
	if (p_dbcs == NULL)
		return 0;

	int len = 0;
	while (*p_dbcs++ != 0)
		len++;

	return len;
}
int	g_GetByteLenth(const char_t * p_dbcs, int dbcs_len)
{

	if (p_dbcs == NULL || dbcs_len <= 0)
		return 0;
	// convert
	for (int i=0, c=0; i < dbcs_len; i++)
	{
		// check high byte
		if ((p_dbcs[i]&0xFF00) != 0)
		{
			c+=2;
		}
		else
		{
			c++;
		}
	}
	return c;

}
//-----------------------------------------------------------------------------
// g_Convert_DBCS_Ascii2SingleByte
//


//

//-----------------------------------------------------------------------------
int g_Convert_DBCS_Ascii2SingleByte(const char_t * p_dbcs, int dbcs_len, char * &p_new_buf)
{
	//
	// (1) ignore 'p_new_buf'.
	//

	if (p_dbcs == NULL || dbcs_len <= 0)
		return 0;

	int max_size = dbcs_len*2+1;

	char * p_temp = new char[max_size];
	CheckMemAlloc(p_temp);
	memset(p_temp, 0, max_size);

	// convert
	for (int i=0, c=0; i < dbcs_len; i++)
	{
		// check high byte
		if ((p_dbcs[i]&0xFF00) != 0)
		{
			*(char_t *)(p_temp+i+c) = p_dbcs[i];
			c++;
		}
		else
		{
			p_temp[i+c] = (char)(p_dbcs[i]);
		}
	}

	int len = strlen(p_temp);
	p_new_buf = new char[len+1];
	CheckMemAlloc(p_new_buf);

	strcpy(p_new_buf, p_temp);

	DeleteNewArray(p_temp);

	return len;
}

//-----------------------------------------------------------------------------
// g_ConvertAscii2DBCS
//


//
//-----------------------------------------------------------------------------
// p_ascii:		single byte string
// ascii_len:	p_ascii length
// p_new_buf:	converted DBCS buf
//
// return:		p_new_buf length
//					zero is failed.
//-----------------------------------------------------------------------------
int g_ConvertAscii2DBCS(const char * p_ascii, int ascii_len, char_t * &p_new_buf)
{
	if (p_ascii == NULL || ascii_len <= 0)
		return 0;

	int size = ascii_len+1;
	char_t * p_temp = new char_t[size];

	int dbcs = 0;
	for (int i=0; i<ascii_len; i++, dbcs++)
		if (isascii(p_ascii[i]) != 0) // ascii? or not hangul code?
		{
			// ascii

			p_temp[dbcs] = (char_t)p_ascii[i];
		}
		else
		{
			// hangul

			p_temp[dbcs] = *((char_t *)(p_ascii+i));
			i++;
		}



	p_new_buf = new char_t[dbcs+1];

	for (int m=0; m < dbcs; m++)
		p_new_buf[m] = p_temp[m];

	DeleteNewArray(p_temp);
	p_new_buf[dbcs] = 0;

	return dbcs;
}


bool	g_FL2_GetDC()
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_fl2_surface == NULL)
		_Error(NULL_REF);

	if(gh_FL2_DC == NULL)
	{
		gpC_fl2_surface->GetDC(&gh_FL2_DC);

		return true;
	}

	return false;
}


bool	g_FL2_ReleaseDC()
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (gpC_fl2_surface == NULL)
		_Error(NULL_REF);

	if(gh_FL2_DC != NULL)
	{
		gpC_fl2_surface->ReleaseDC(gh_FL2_DC);
		gh_FL2_DC = NULL;

		return true;
	}

	return false;
}


////////////////////////////////////////////////

//



////////////////////////////////////////////////

void ReduceString(char *str,int len)
{
	if(len<15) return;
	char name[100]="",*p_name;
	int lt=strlen(str),passlen,position,size;
	p_name=&name[0];
	//bool g_PossibleStringCut(const char * sz_str, int position)
	if(lt>len)
	{
		if(g_PossibleStringCut(str,5))
			passlen=5;
		else	
			passlen=6;
		memcpy(p_name,str,passlen);
		p_name+=passlen;
		*(p_name++)='.';
		*(p_name++)='.';
		*(p_name++)='.';
		passlen+=3;
		size=passlen;
		position=lt-(len-passlen);
		if(g_PossibleStringCut(str,position))
			{
			position=lt-len+passlen;
			passlen=len-passlen;
		}
		else
			{
			position=lt-len+passlen-1;
			passlen=len-passlen+1;
		}
		size+=passlen;
		memcpy(p_name,str+position,passlen);		
		memcpy(str,name,size);
		str[size]='\0';		
	}		
}


void ReduceString2(char *str,int len)
{
	if(len<15) return;
	char name[100]="",*p_name;
	int lt=strlen(str),passlen,position,size;
	p_name=&name[0];
	//bool g_PossibleStringCut(const char * sz_str, int position)
	
	int skip_len=len-14;
	if(lt>len)
	{
		if(g_PossibleStringCut(str,skip_len))
			passlen=skip_len;
		else	
			passlen=skip_len+1;
		memcpy(p_name,str,passlen);
		p_name+=passlen;
		*(p_name++)='.';
		*(p_name++)='.';
		*(p_name++)='.';
		passlen+=3;
		size=passlen;
		position=lt-(len-passlen);
		if(g_PossibleStringCut(str,position))
			{
			position=lt-len+passlen;
			passlen=len-passlen;
		}
		else
			{
			position=lt-len+passlen-1;
			passlen=len-passlen+1;
		}
		size+=passlen;
		memcpy(p_name,str+position,passlen);		
		memcpy(str,name,size);
		str[size]='\0';		
	}		
}
// 2004, 10, 26, sobeit add start
void ReduceString3(char *str,int len)
{
	if(str == NULL) return;
	if(len<15) return;
	int CurLen = strlen(str);
	if(CurLen<=len) return;
	
	if(!g_PossibleStringCut(str,len))
		str[len-3] = '.';
	str[len-2] = '.';
	str[len-1] = '.';
	str[len] = '\0';
}
// 2004, 10, 26, sobeit add end

//-----------------------------------------------------------------------------
// g_PrintColorStr
//

//-----------------------------------------------------------------------------
int g_PrintColorStr(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb)
{
	PrintInfo use_pi = pi;

	use_pi.text_color = str_rgb;

	bool bGetDC = g_FL2_GetDC();

	g_Print(x, y, sz_str, &use_pi);

	int re = (x+g_GetStringWidth(sz_str, use_pi.hfont));

	if(bGetDC)
		g_FL2_ReleaseDC();

	return re;
}

//-----------------------------------------------------------------------------
// g_PrintColorStrOut
//

//-----------------------------------------------------------------------------
int g_PrintColorStrOut(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb, COLORREF out_rgb)
{
	PrintInfo use_pi = pi;

	use_pi.text_color = out_rgb;
	bool bGetDC = g_FL2_GetDC();
	g_Print(x-1, y, sz_str, &use_pi);
	g_Print(x+1, y, sz_str, &use_pi);
	g_Print(x, y-1, sz_str, &use_pi);
	g_Print(x, y+1, sz_str, &use_pi);

	use_pi.text_color = str_rgb;
	g_Print(x, y, sz_str, &use_pi);
	
	int re = (x+g_GetStringWidth(sz_str, use_pi.hfont));

	if(bGetDC)
		g_FL2_ReleaseDC();

	return re;
}

//-----------------------------------------------------------------------------
// g_PrintColorStrShadow
//

//-----------------------------------------------------------------------------
int g_PrintColorStrShadow(int x, int y, const char * sz_str, PrintInfo &pi, COLORREF str_rgb, COLORREF shadow_rgb)
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	PrintInfo use_pi = pi;

	if(shadow_rgb == 0xFFFFFFFF)
	{
		int r = (str_rgb&0xFF0000) >> 16;
		int g = (str_rgb&0xFF00) >> 8;
		int b = str_rgb&0xFF;
//		shadow_rgb = RGB(max(0, r-150), max(0, g-150), max(0, b-150));
		shadow_rgb = RGB(r>>2, g>>2, b>>2);
	}

	bool bGetDC = g_FL2_GetDC();

	use_pi.text_color = shadow_rgb;
	g_Print(x+1, y+1, sz_str, &use_pi);

	use_pi.text_color = str_rgb;
	g_Print(x, y, sz_str, &use_pi);
	
	int re = (x+g_GetStringWidth(sz_str, use_pi.hfont));

	if(bGetDC)
		g_FL2_ReleaseDC();

	return re;
}

std::string g_GetNumberString(int number)
{
	char sz_temp[20];
	wsprintf(sz_temp, "%d", number);
	std::string sstr = sz_temp;
	for(int i = 3; i <= 13; i += 4)
		if((int)sstr.size() > i)sstr.insert(sstr.size()-i, ",");

	return sstr;
}


std::string g_GetStringByMoney(DWORD dwMoney)
{
	char TempBuffer[32] = {0,};
	std::string sstr;
	DWORD TempMoney = 0;
	if(dwMoney>=100000000)
	{
		TempMoney = dwMoney / 100000000;
		if(TempMoney)
		{
			wsprintf(TempBuffer, "%d억", TempMoney);
			sstr+= TempBuffer;
		}
	}
	if(dwMoney>=10000)
	{
		TempMoney = (dwMoney%100000000) / 10000;
		if(TempMoney)
		{
			wsprintf(TempBuffer, "%d만", TempMoney);
			sstr+= TempBuffer;
		}
	}
	
	TempMoney = (dwMoney%10000);
	if(TempMoney || 0 == dwMoney)
	{
		wsprintf(TempBuffer, "%d", TempMoney);
		sstr+= TempBuffer;
	}

	return sstr;
}















