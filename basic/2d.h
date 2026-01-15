#ifndef __2D__
#define __2D__

#include "typedef.h"
#ifdef PLATFORM_WINDOWS
#include <DDraw.h>
#endif
#include "DLL.h"

/*-----------------------------------------------------------------------------
  S_SURFACEINFO (ǥ������)
-----------------------------------------------------------------------------*/
typedef struct
{
   void  *p_surface;
   int   width;
   int   height;
   int   pitch;

} S_SURFACEINFO;

/*-----------------------------------------------------------------------------
  �簢�� ����ü
-----------------------------------------------------------------------------*/
typedef struct
{
   int   x;    // x ��ǥ
   int   y;    // y ��ǥ
   int   w;    // ����
   int   h;    // ����

} S_RECT;

#endif
