#ifndef __DLL_INFO__
#define __DLL_INFO__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#define __WORK__
//#define __MK_DLL__			// DLL¸¸µé¶§
#ifdef __WORK__

        #define         __EX

#else

        #ifdef __MK_DLL__
                #define __EX __declspec(dllexport)
        #else
                #define __EX __declspec(dllimport)
        #endif

#endif

#endif
