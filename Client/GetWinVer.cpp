#include "Client_PCH.h"

BOOL GetWinVersion(char *szVersion)
{
#ifdef PLATFORM_WINDOWS
   // Windows implementation - simplified version
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   char szTemp[512];

   switch (osvi.dwPlatformId)
   {
      // Test for the Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
         if ( osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0 )
            strcat (szVersion, "Windows 10/11");
         else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3 )
            strcat (szVersion, "Windows 8.1");
         else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 )
            strcat (szVersion, "Windows 8");
         else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 )
            strcat (szVersion, "Windows 7");
         else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
            strcat (szVersion, "Windows Vista");
         else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            strcat (szVersion, "Windows XP");
         else
            sprintf(szVersion, "Windows NT %d.%d", osvi.dwMajorVersion, osvi.dwMinorVersion);
         break;

      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
             strcpy(szVersion, "Windows 95");
         else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
             strcpy(szVersion, "Windows 98");
         else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
             strcpy(szVersion, "Windows ME");
         else
             strcpy(szVersion, "Windows 9x");
         break;

      default:
         strcpy(szVersion, "Unknown Windows");
         break;
   }

   // Add build number if available
   if (osvi.dwBuildNumber > 0)
   {
      sprintf(szTemp, " (Build %d)", osvi.dwBuildNumber & 0xFFFF);
      strcat(szVersion, szTemp);
   }

   return TRUE;

#else
   // Non-Windows platforms
   strcpy(szVersion, "Non-Windows Platform");
   return TRUE;
#endif
}