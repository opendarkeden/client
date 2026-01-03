#ifndef __2D__
#define __2D__

#include "typedef.h"
#include <DDraw.h>
#include "DLL.h"

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

#endif
