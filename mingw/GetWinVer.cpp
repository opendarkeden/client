#include "Client_PCH.h"

BOOL GetWinVersion(char *szVersion)
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

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

         // Test for the specific product family.
         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
            strcat (szVersion, "Windows Server&nbsp;2003 family, ");

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            strcat (szVersion, "Windows XP ");

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            strcat (szVersion, "Windows 2000 ");

         if ( osvi.dwMajorVersion <= 4 )
            strcat (szVersion, "Windows NT ");

         // Test for specific product on Windows NT 4.0 SP6 and later.
         if( bOsVersionInfoEx )
         {
//            // Test for the workstation type.
//            if ( osvi.wProductType == VER_NT_WORKSTATION )
//            {
//               if( osvi.dwMajorVersion == 4 )
//                  strcat (szVersion, "Workstation 4.0 " );
//               else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
//                  strcat (szVersion, "Home Edition " );
//               else
//                  strcat (szVersion, "Professional " );
//            }
//            
//            // Test for the server type.
//            else if ( osvi.wProductType == VER_NT_SERVER )
//            {
//               if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
//               {
//                  if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
//                     strcat (szVersion, "Datacenter Edition " );
//                  else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
//                     strcat (szVersion, "Enterprise Edition " );
//                  else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
//                     strcat (szVersion, "Web Edition " );
//                  else
//                     strcat (szVersion, "Standard Edition " );
//               }
//
//               else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
//               {
//                  if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
//                     strcat (szVersion, "Datacenter Server " );
//                  else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
//                     strcat (szVersion, "Advanced Server " );
//                  else
//                     strcat (szVersion, "Server " );
//               }
//
//               else  // Windows NT 4.0 
//               {
//                  if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
//                     strcat (szVersion, "Server 4.0, Enterprise Edition " );
//                  else
//                     strcat (szVersion, "Server 4.0 " );
//               }
//            }
         }
//         else  // Test for specific product on Windows NT 4.0 SP5 and earlier
//         {
//            HKEY hKey;
//            char szProductType[BUFSIZE];
//            DWORD dwBufLen=BUFSIZE;
//            LONG lRet;
//
//            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
//               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
//               0, KEY_QUERY_VALUE, &hKey );
//            if( lRet != ERROR_SUCCESS )
//               return FALSE;
//
//            lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
//               (LPBYTE) szProductType, &dwBufLen);
//            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
//               return FALSE;
//
//            RegCloseKey( hKey );
//
//            if ( lstrcmpi( "WINNT", szProductType) == 0 )
//               strcat(szVersion, "Workstation " );
//            if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
//               strcat(szVersion, "Server " );
//            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
//               strcat(szVersion, "Advanced Server " );
//
//            sprintf(szTemp, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
//			strcat(szVersion, szTemp);
//         }

      // Display service pack (if any) and build number.

         if( osvi.dwMajorVersion == 4 && 
             lstrcmpi( osvi.szCSDVersion, "Service Pack 6" ) == 0 )
         {
            HKEY hKey;
            LONG lRet;

            // Test for SP6 versus SP6a.
            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
               0, KEY_QUERY_VALUE, &hKey );
            if( lRet == ERROR_SUCCESS )
			{
				sprintf(szTemp, "Service Pack 6a (Build %d)", osvi.dwBuildNumber & 0xFFFF );
				strcat(szVersion, szTemp);
			}
            else // Windows NT 4.0 prior to SP6a
            {
				sprintf(szTemp, "%s (Build %d)",
                  osvi.szCSDVersion,
                  osvi.dwBuildNumber & 0xFFFF);
				strcat(szVersion, szTemp);
            }

            RegCloseKey( hKey );
         }
         else // Windows NT 3.51 and earlier or Windows 2000 and later
         {
            sprintf(szTemp, "%s (Build %d)",
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
			strcat(szVersion, szTemp);
         }


         break;

      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
             strcat(szVersion, "Windows 95 ");
             if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
                strcat(szVersion, "OSR2 " );
         } 

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
             strcat(szVersion, "Windows 98 ");
             if ( osvi.szCSDVersion[1] == 'A' )
                strcat(szVersion, "SE " );
         } 

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
             strcat(szVersion, "Windows Millennium Edition");
         } 
         break;

      case VER_PLATFORM_WIN32s:

         strcat(szVersion, "Win32s");
         break;
   }

   return TRUE;
}