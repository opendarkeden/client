/*-----------------------------------------------------------------------------

	BasicMemory.h	

	Memory  functions & utility.

	2000.8.25. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __BASICMEMORY_H__
#define __BASICMEMORY_H__

//----------------------------------------------------------------------------
// Memory release define.
// 
//----------------------------------------------------------------------------
#define DeleteNew(x)						if (x) { delete x; x = NULL; }
#define DeleteNewArray(x)				if (x) { delete []x; x = NULL; }

#endif