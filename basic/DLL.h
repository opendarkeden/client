/*-----------------------------------------------------------------------------

	DLL.h

	Dynamic Link Library general header file.

	2000.8.8. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __DLL_H__
#define __DLL_H__

#define _CPP

#ifdef PLATFORM_WINDOWS
#ifndef _CPP
#define export extern "C" __declspec (dllexport)
#define import extern "C" __declspec (dllimport)
#else
#define export __declspec (dllexport)
#define import __declspec (dllimport)
#endif

#define DllExport		export
#define DllImport		import

#ifdef _DLL_EXPORT
#define DllClass		DllExport
#else
#define DllClass		DllImport
#endif
#else
// Non-Windows platforms: empty macros
#define export
#define import
#define DllExport
#define DllImport
#define DllClass
#endif

#endif