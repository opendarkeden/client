#ifndef __2D__
#define __2D__

#include "typedef.h"
#include <DDraw.h>
#include "DLL.h"

/*-----------------------------------------------------------------------------
  S_SURFACEINFO (Ç¥¸éÁ¤º¸)
-----------------------------------------------------------------------------*/
typedef struct
{
   void  *p_surface;
   int   width;
   int   height;
   int   pitch;

} S_SURFACEINFO;

/*-----------------------------------------------------------------------------
  »ç°¢Çü ±¸Á¶Ã¼
-----------------------------------------------------------------------------*/
typedef struct
{
   int   x;    // x ÁÂÇ¥
   int   y;    // y ÁÂÇ¥
   int   w;    // ³ÐÀÌ
   int   h;    // ³ôÀÌ

} S_RECT;

#endif
