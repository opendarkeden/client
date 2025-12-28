/*-----------------------------------------------------------------------------

	BasicMemory.h	

	Memory 관련 functions & utility.

	2000.8.25. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __BASICMEMORY_H__
#define __BASICMEMORY_H__

//----------------------------------------------------------------------------
// Memory release define.
// 
// 함수로 하면 x가 class인 경우 destructor가 불리지 않는다.
// template으로 하면 될까?
//----------------------------------------------------------------------------
#define DeleteNew(x)						if (x) { delete x; x = NULL; }
#define DeleteNewArray(x)				if (x) { delete []x; x = NULL; }

#endif