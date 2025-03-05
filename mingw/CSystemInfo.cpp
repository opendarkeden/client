/********************************************************************
	created:	2003/10/17
	created:	17:10:2003   13:48
	filename: 	E:\designed\project\client\CSystemInfo.cpp
	file path:	E:\designed\project\client
	file base:	CSystemInfo
	file ext:	cpp
	author:		sonee
	
	purpose:	시스템 정보를 알아낸다.
				2003-10-17		CPU Clock 얻어오기
								MMX,SSE2 테크놀러지 Enable 여부
								Hyper Thread Enable 여부
*********************************************************************/
#include "Client_PCH.h"
#include <windows.h>
#include "CSystemInfo.h"

#pragma warning( disable : 4035 )		// disable 시켜버리자-_-;
#pragma warning( disable: 4800 ) //'int' : forcing value to bool 'true' or 'false' (performance warning)

inline unsigned __int64 theCycleCount(void)
{
    
    _asm    _emit 0x0F
    _asm    _emit 0x31
    
    // -_- return 은 없지만 _emits 는 __int64를 리턴합니다-_-;
}

static bool cpuid(unsigned long function, unsigned long& out_eax, unsigned long& out_ebx, unsigned long& out_ecx, unsigned long& out_edx)
{
#ifdef _LINUX
	asm("cpuid": "=a" (out_eax), "=b" (out_ebx), "=c" (out_ecx), "=d" (out_edx) : "a" (function));
	return true;
#else
	bool retval = true;
	unsigned long local_eax, local_ebx, local_ecx, local_edx;
	_asm pushad;

	__try
	{
        _asm
		{
			xor edx, edx		// Clue the compiler that EDX is about to be used.
            mov eax, function   // set up CPUID to return processor version and features
								//      0 = vendor string, 1 = version info, 2 = cache info
            cpuid				// code bytes = 0fh,  0a2h
            mov local_eax, eax	// features returned in eax
            mov local_ebx, ebx	// features returned in ebx
            mov local_ecx, ecx	// features returned in ecx
            mov local_edx, edx	// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = false; 
	}

	out_eax = local_eax;
	out_ebx = local_ebx;
	out_ecx = local_ecx;
	out_edx = local_edx;

	_asm popad

	return retval;
#endif


}

long CSystemInfo::GetCpuClock()
{
	unsigned __int64			start;
	unsigned __int64			overhead;

	start = theCycleCount();
	overhead = theCycleCount()-start;
	start = theCycleCount();
	Sleep(100);

	unsigned cpuspeed100 = (unsigned)( (theCycleCount()-start-overhead) / 1000 );
	return cpuspeed100 /100;
}

// --------------------------------------------------------------------------
bool CSystemInfo::CheckMMXTechnology()
{
    BOOL retval = TRUE;
    DWORD RegEDX;

#ifdef CPUID
	_asm pushad;
#endif

    __try
	{
        _asm
		{
#ifdef CPUID
			xor edx, edx	// Clue the compiler that EDX is about to be used.
#endif
            mov eax, 1      // set up CPUID to return processor version and features
                            //      0 = vendor string, 1 = version info, 2 = cache info
            CPUID           // code bytes = 0fh,  0a2h
            mov RegEDX, edx // features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = FALSE; 
	}

	// If CPUID not supported, then certainly no MMX extensions.
    if (retval)
	{
		if (RegEDX & 0x800000)          // bit 23 is set for MMX technology
		{
		   __try 
		   { 
				// try executing the MMX instruction "emms"
			   _asm EMMS
		   } 
		   __except(EXCEPTION_EXECUTE_HANDLER) 
		   { 
			   retval = FALSE; 
		   }
		}

		else
			retval = FALSE;           // processor supports CPUID but does not support MMX technology

		// if retval == 0 here, it means the processor has MMX technology but
		// floating-point emulation is on; so MMX technology is unavailable
	}

#ifdef CPUID
	_asm popad;
#endif

    return retval;
}


/*
// --------------------------------------------------------------------------
bool CSystemInfo::CheckSSETechnology(void)
{
    BOOL retval = TRUE;
    DWORD RegEDX;

#ifdef CPUID
	_asm pushad;
#endif

	// Do we have support for the CPUID function?
    __try
	{
        _asm
		{
#ifdef CPUID
			xor edx, edx			// Clue the compiler that EDX is about to be used.
#endif
            mov eax, 1				// set up CPUID to return processor version and features
									//      0 = vendor string, 1 = version info, 2 = cache info
            CPUID					// code bytes = 0fh,  0a2h
            mov RegEDX, edx			// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = FALSE; 
	}

	// If CPUID not supported, then certainly no SSE extensions.
    if (retval)
	{
		// Do we have support for SSE in this processor?
		if ( RegEDX & 0x2000000L )		// bit 25 is set for SSE technology
		{
			// Make sure that SSE is supported by executing an inline SSE instruction

// BUGBUG, FIXME - Visual C Version 6.0 does not support SSE inline code YET (No macros from Intel either)
// Fix this if VC7 supports inline SSE instructinons like "xorps" as shown below.
#if 1
			__try
			{
				_asm
				{
					// Attempt execution of a SSE instruction to make sure OS supports SSE FPU context switches
					xorps xmm0, xmm0
					// This will work on Win2k+ (Including masking SSE FPU exception to "normalized" values)
					// This will work on Win98+ (But no "masking" of FPU exceptions provided)
				}
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 
#endif

			{ 
				retval = FALSE; 
			}
		}
		else
			retval = FALSE;
	}
#ifdef CPUID
	_asm popad;
#endif

    return retval;
}
bool CSystemInfo::CheckSSE2Technology()
{
    BOOL retval = TRUE;
    DWORD RegEDX;

#ifdef CPUID
	_asm pushad;
#endif

	// Do we have support for the CPUID function?
    __try
	{
        _asm
		{
#ifdef CPUID
			xor edx, edx			// Clue the compiler that EDX is about to be used.
#endif
            mov eax, 1				// set up CPUID to return processor version and features
									//      0 = vendor string, 1 = version info, 2 = cache info
            CPUID					// code bytes = 0fh,  0a2h
            mov RegEDX, edx			// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = FALSE; 
	}

	// If CPUID not supported, then certainly no SSE extensions.
    if (retval)
	{
		// Do we have support for SSE in this processor?
		if ( RegEDX & 0x04000000 )		// bit 26 is set for SSE2 technology
		{
			// Make sure that SSE is supported by executing an inline SSE instruction

			__try
			{
				_asm
				{
					// Attempt execution of a SSE2 instruction to make sure OS supports SSE FPU context switches
					xorpd xmm0, xmm0
				}
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 

			{ 
				retval = FALSE; 
			}
		}
		else
			retval = FALSE;
	}
#ifdef CPUID
	_asm popad;
#endif

    return retval;
}
*/

// --------------------------------------------------------------------------
bool CSystemInfo::Check3DNowTechnology()
{
    BOOL retval = TRUE;
    DWORD RegEAX;

#ifdef CPUID
	_asm pushad;
#endif

    // First see if we can execute CPUID at all
	__try
	{
        _asm
		{
#ifdef CPUID
//			xor edx, edx			// Clue the compiler that EDX is about to be used.
#endif
            mov eax, 0x80000000     // setup CPUID to return whether AMD >0x80000000 function are supported.
									// 0x80000000 = Highest 0x80000000+ function, 0x80000001 = 3DNow support
            CPUID					// code bytes = 0fh,  0a2h
            mov RegEAX, eax			// result returned in eax
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = FALSE; 
	}

	// If CPUID not supported, then there is definitely no 3DNow support
    if (retval)
	{
		// Are there any "higher" AMD CPUID functions?
		if (RegEAX > 0x80000000L )				
		{
		   __try 
			{
			_asm
				{
					mov			eax, 0x80000001		// setup to test for CPU features
					CPUID							// code bytes = 0fh,  0a2h
					shr			edx, 31				// If bit 31 is set, we have 3DNow support!
					mov			retval, edx			// Save the return value for end of function
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{ 
				retval = FALSE; 
			}
		}
		else
		{
			// processor supports CPUID but does not support AMD CPUID functions
			retval = FALSE;					
		}
	}

#ifdef CPUID
	_asm popad;
#endif

    return retval;
}

// Returns non-zero if Hyper-Threading Technology is supported on the processors and zero if not.  This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
bool CSystemInfo::CheckHyperThreadTechnology()
{
	const unsigned int HT_BIT		 = 0x10000000;  // EDX[28] - Bit 28 set indicates Hyper-Threading Technology is supported in hardware.
	const unsigned int FAMILY_ID     = 0x0f00;      // EAX[11:8] - Bit 11 thru 8 contains family processor id
	const unsigned int EXT_FAMILY_ID = 0x0f00000;	// EAX[23:20] - Bit 23 thru 20 contains extended family  processor id
	const unsigned int PENTIUM4_ID   = 0x0f00;		// Pentium 4 family processor id

	unsigned long unused,
				  reg_eax = 0, 
				  reg_edx = 0,
				  vendor_id[3] = {0, 0, 0};

	// verify cpuid instruction is supported
	if( !cpuid(0,unused, vendor_id[0],vendor_id[2],vendor_id[1]) 
	 || !cpuid(1,reg_eax,unused,unused,reg_edx) )
	 return false;

	//  Check to see if this is a Pentium 4 or later processor
	if (((reg_eax & FAMILY_ID) ==  PENTIUM4_ID) || (reg_eax & EXT_FAMILY_ID))
		if (vendor_id[0] == 'uneG' && vendor_id[1] == 'Ieni' && vendor_id[2] == 'letn')
			return (reg_edx & HT_BIT) != 0;	// Genuine Intel Processor with Hyper-Threading Technology

	return false;  // This is not a genuine Intel processor.
}
