#include"stdafx.h"
#include <assert.h>
#include "smartcutting.h"

/*-----------------------------------------------------------------------------
  line direction
-----------------------------------------------------------------------------*/
typedef enum
{
	LINE_WIDTH,
	LINE_HEIGHT

} E_LINE_DIRECTION;

/*-----------------------------------------------------------------------------
  Prototype
-----------------------------------------------------------------------------*/
static bool TestAccordLine(S_SURFACEINFO *pS_surfaceinfo, int x, int y, int len, 
						  E_LINE_DIRECTION e_direction, int colorkey);


// get pixel from image data array
DWORD getPixel24( S_SURFACEINFO* pSI, int x, int y)
{
	int bytePerPixel = pSI->pitch / pSI->width;
	DWORD c = *(DWORD*)(((BYTE*)pSI->p_surface) + y*pSI->pitch + x*bytePerPixel) & 0x00ffffff;
	return c;
}

/*-----------------------------------------------------------------------------
- TestAccordLine
- line 중 모든 pixel의 색이 colorkey와 일치하면 true, 그렇지 않으면 false를 반환한다.
-----------------------------------------------------------------------------*/
static bool TestAccordLine(S_SURFACEINFO *pS_surfaceinfo, int x, int y, int len, 
						  E_LINE_DIRECTION e_direction, int colorkey)
{
	int color;

	if (e_direction == LINE_WIDTH) // 가로
	{
		for (int i = 0; i < len; i++)
		{
			color = getPixel24(pS_surfaceinfo, x+i, y);
			if (colorkey != color) 
				return false;
		}
	}
	else // 세로
	{
		for (int i = 0; i < len; i++)
		{
			color = getPixel24(pS_surfaceinfo, x, y+i);
			if (colorkey != color) 
				return false;
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------
- DoSmartCutting
- '안 -> 밖', '밖 -> 안' smart cutting을 한다.

  `결과물은 source rect인 p_cur_rect에 반영된다.
-----------------------------------------------------------------------------*/
void DoSmartCutting(S_SURFACEINFO *pS_surfaceinfo, S_RECT *p_cur_rect, int colorkey)
{
	assert(pS_surfaceinfo);
	assert(p_cur_rect);

	// 4개의 선분의 색을 검사한다.
	int x, y, w, h;
	int line_length;
	int color, prev_color;
	bool seek_out_in; // out -> in을 검색했는가 flag
	bool seek_in_out; // in -> out을 검색했는가 flag
 
	//
	//         (1)
	//     +----------+
	//     |          |
	// (3) |          | (4)
	//     |          |
	//     +----------+
	//         (2)
	//
	color = prev_color = 0;
	x				= p_cur_rect->x;
	y				= p_cur_rect->y;
	w				= p_cur_rect->w;
	h				= p_cur_rect->h;
	
	//
	// 예외적인 상황을 배려하여, out -> in일 경우는 1회만 하고, in -> out일 경우에는
	// goto _again_으로 out -> in일 경우가 될 때까지 반복한다.
	//

_again_:
	// (1)
	line_length = w;
	seek_out_in = false;
	seek_in_out = false;
	while (1)
	{
		if (TestAccordLine(pS_surfaceinfo, x, y, line_length, LINE_WIDTH, colorkey))
		{ // out -> in
			if (seek_in_out) break;
			seek_out_in = true;
			y++;
			h--;
			if (y >= pS_surfaceinfo->height || h < 0) // exception
			{
				w = 0;
				h = 0;
				break;
			}
		}
		else
		{ // in -> out
			if (seek_out_in) break;
			y--;
			h++;
			if (y < 0) // exception: y만 검사
			{
				h += y;// MBCD At 00-07-19 오후 2:05:20
				y = 0;
				break;
			}
			seek_in_out = true;
		}
	}
	// (2)
	seek_out_in = false;
	while (1)
	{
		if (TestAccordLine(pS_surfaceinfo, x, y+h-1, line_length, LINE_WIDTH, colorkey))
		{ // out -> in
			if (seek_in_out) break;
			seek_out_in = true;
			h--;
			if (h < 0) // exception
			{
				w = 0;
				h = 0;
				break;
			}
		}
		else
		{ // in -> out
			if (seek_out_in) break;
			if (y+h < pS_surfaceinfo->height)
				h++;
			else
				break;
			seek_in_out = true;
		}
	}
	// (3)
	line_length = h;
	seek_out_in = false;
	while (1)
	{
		if (TestAccordLine(pS_surfaceinfo, x, y, line_length, LINE_HEIGHT, colorkey))
		{ // out -> in
			if (seek_in_out) break;
			seek_out_in = true;
			x++;
			w--;
			if (x > pS_surfaceinfo->width || w < 0) // exception
			{
				w = 0;
				h = 0;
				break;
			}
		}
		else
		{ // in -> out
			if (seek_out_in) break;
			x--;
			w++;
			if (x < 0) // exception: x만 검사
			{
				w += x;// MBCD At 00-07-19 오후 2:05:20
				x = 0;
				break;
			}
			seek_in_out = true;
		}
	}
	// (4)
	seek_out_in = false;
	while (1)
	{
		if (TestAccordLine(pS_surfaceinfo, x+w-1, y, line_length, LINE_HEIGHT, colorkey))
		{ // out -> in
			if (seek_in_out) break;
			seek_out_in = true;
			w--;
			if (w < 0) // exception
			{
				w = 0;
				h = 0;
				break;
			}
		}
		else
		{ // in -> out
			if (seek_out_in) break;
			if (x+w < pS_surfaceinfo->width)
				w++;
			else
				break;
			seek_in_out = true;
		}
	}

	if (seek_in_out) goto _again_;

	p_cur_rect->x = x;
	p_cur_rect->y = y;
	p_cur_rect->w = w;
	p_cur_rect->h = h;
}