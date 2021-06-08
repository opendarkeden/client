// FL2.cpp

#include "client_PCH.h"

#include "FL2.h"
#include "VS_UI_Base.h"

//-----------------------------------------------------------------------------
// Utilities for IO.
//-----------------------------------------------------------------------------

//
// FL2에서는 DC를 사용하므로 surface가 필요하다. 이것은 일반적으로 입출력 
// surface이겠지만, offscreen surface로도 할 수 있겠다.
//
// Unicorn edit line widget에서 FL2를 사용하므로 그곳에서 이 surface를 참조할
// 것이다.
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
// sz_str의 position(byte)이 cut될 수 있는가 여부를 반환한다.
// sz_str은 한글 2byte, 영문 1byte이다.
//-----------------------------------------------------------------------------
bool g_PossibleStringCut(const char * sz_str, int position)
{
	if (sz_str != NULL)
	{
		if ((int)strlen(sz_str) <= position)
			return true;

		//
		// position에는 세가지 경우의 data가 있을 수 있다.
		//
		// (1) ASCII
		// (2) 한글 1 byte
		// (3) 한글 2 byte
		//
		// 그러나 (3)의 경우 그 값이 ASCII가 아니라고 장담할 수 없다.
		// 그래서 처음부터 position까지 검사를 해야 한다.
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
// p_str은 null terminated string이다.
// hfont를 올바르게 setting해야 정확한 값이 나온다.
//
// ! 외부에서 lock걸면 안된다.
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
// p_str은 null terminated string이다.
// hfont를 올바르게 setting해야 정확한 값이 나온다.
//
// !외부에서 lock걸면 안된다.
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
// gpC_fl2_surface에 출력한다.
// p_str은 null terminated string이다.
//
// !외부에서 lock걸면 안된다.
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
// p_dbcs의 length를 반환한다.
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
// DBCS Ascii code를 single byte로 바꾼다. 다른 문자들을 그대로 유지시킨다.
// p_new_buf를 new로 할당한다. 따라서 외부에서 p_new_buf를 delete해줘야 한다.
//
// 저장된 buffer의 길이(by byte)를 반환한다.
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
// ascii code(single byte까지 포함)를 DBCS로 변환하여 p_new_buf에 할당한다.
// p_new_buf는 외부에서 delete해야 한다.
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

	// 정확한 size의 buffer를 p_new_buf가 가리키도록 한다.
	// 한글이 포함되면 dbcs와 ascii_len은 다르다.
	p_new_buf = new char_t[dbcs+1];

	for (int m=0; m < dbcs; m++)
		p_new_buf[m] = p_temp[m];

	DeleteNewArray(p_temp);
	p_new_buf[dbcs] = 0;

	return dbcs;
}

// DC를 Get한다.
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

// DC를 Release 한다.
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
// 이름 줄여주는 소스 by sonee
//
// 긴 문자를 40자로 줄이고 싶으면 ReduceString(str,40);
// str 자체를 변경한다.
// 변경하지 않고 리턴값으로 할경우 마지막 복사부분을 빼고 그냥 리턴하면됌
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

// 뒷부분에 ... 을 찍어준다.				 by sonee
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
// str을 출력한 후의 x를 반환한다.
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
// str을 출력한 후의 x를 반환한다.
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
// str을 출력한 후의 x를 반환한다.
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
	if(dwMoney>=100000000) // 억
	{
		TempMoney = dwMoney / 100000000;
		if(TempMoney)
		{
			wsprintf(TempBuffer, "%d억", TempMoney);
			sstr+= TempBuffer;
		}
	}
	if(dwMoney>=10000) // 만
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















