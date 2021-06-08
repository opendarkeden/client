#ifndef __2D__
#define __2D__
#include"stdafx.h"

#include "typedef.h"
#include "TGA.h"
#include "IMG.h"
#include "picinfo.h"

#include <DDraw.h>

/*-----------------------------------------------------------------------------
  S_SURFACEINFO (표면정보)
-----------------------------------------------------------------------------*/
typedef struct
{
   void  *p_surface;
   int   width;
   int   height;
   int   pitch;

} S_SURFACEINFO;

/*-----------------------------------------------------------------------------
  사각형 구조체
-----------------------------------------------------------------------------*/
typedef struct
{
   int   x;    // x 좌표
   int   y;    // y 좌표
   int   w;    // 넓이
   int   h;    // 높이

} S_RECT;

/*-----------------------------------------------------------------------------
  EXTERN
-----------------------------------------------------------------------------*/
extern void (*filledRect)(S_SURFACEINFO *, S_RECT *, int);
extern void (*cls)(S_SURFACEINFO *, int);
extern int (*getPixel)(S_SURFACEINFO *, int, int);
extern void (*putPixel)(S_SURFACEINFO *, int, int, int); // putPixel function pointer
extern void (*vertline)(S_SURFACEINFO *, int, int, int, int, int);
extern void (*horzline)(S_SURFACEINFO *, int, int, int, int, int);
extern void (*Bltz)(const S_SURFACEINFO *, 
                  const S_SURFACEINFO *, 
                  const S_RECT *,
                  const S_RECT *);
extern void (*CkBltz)(const S_SURFACEINFO *, 
                  const S_SURFACEINFO *, 
                  const S_RECT *,
                  const S_RECT *, 
						const int);
/*-----------------------------------------------------------------------------
  PUBLIC FUNCTION
-----------------------------------------------------------------------------*/
void	SetSurfaceInfo(S_SURFACEINFO *surface_info, const DDSURFACEDESC2 *p_ddsd);
void	SetSurfaceInfo(S_SURFACEINFO &surface_info, void *p_surface, int w, int h, int pitch=0);
void	SetRect(S_RECT &rect, int x, int y, int w, int h);
int	Get_ColorkeyColor(S_PICINFO *picinfo);
int	Get_ColorkeyColor(S_SURFACEINFO *surfaceinfo);
DWORD GL_RGB(DWORD r, DWORD g, DWORD b);
void	InitializeGL(int bpp, int r_bit, int g_bit, int b_bit);
int	Convert24RGBto16(int R, int G, int B);
void	rectangle(S_SURFACEINFO *, int lx, int ly, int rx, int ry, int color);
void	line(S_SURFACEINFO *, int x1, int y1, int x2, int y2, int color);
void	TransparentBlt16(S_SURFACEINFO *dest_surface_info, 
								S_SURFACEINFO *src_surface_info, 
								S_RECT *dest_rect,
								S_RECT *src_rect);
int	getPixel16(S_SURFACEINFO *info, int x, int y); // utility에서 사용!

#endif
