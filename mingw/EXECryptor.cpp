#include "stdafx.h"

#include "EXECryptor.h"
#include <string.h>

//#define ANTIDEBUG_ON_INIT

#pragma optimize( "", off )

// reserve space & suppres compiler optimization
#define EMIT5 \
  __asm _emit 090h; \
  __asm _emit 090h; \
  __asm _emit 090h; \
  __asm _emit 090h; \
  __asm _emit 090h; \

#ifdef USE_API
extern "C" 
DWORD __declspec(dllexport) _stdcall EXECryptor_GetDate()
{
  FILETIME t1,t2;
  WORD dt,tt;
  GetSystemTimeAsFileTime(&t1);
  FileTimeToLocalFileTime(&t1,&t2);
  FileTimeToDosDateTime(&t2,&dt,&tt);
  return dt;
}

void SafeGetDate(int *Day, int *Month, int *Year)
{
  DWORD dt = EXECryptor_GetDate();
  *Day = dt & 0x1F;
  *Month = (dt >> 5) & 0xF;
  *Year = (dt >> 9)+1980;
}

DWORD __declspec(dllexport) __stdcall EXECryptor_GetHardwareID()
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 000h;
  return 0;
}

void __declspec(dllexport) _stdcall EXECryptor_EncryptStr(const char *Src, char *Dst)
{
  EMIT5
  strcpy(Dst,Src);
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 001h;
}

void __declspec(dllexport) _stdcall EXECryptor_DecryptStr(const char *Src, char *Dst)
{
  EMIT5
  strcpy(Dst,Src);
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 002h;
}

int __declspec(dllexport) _stdcall EXECryptor_GetTrialDaysLeft(int TrialPeriod)
{
	return TrialPeriod;
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 016h;
}

int __declspec(dllexport) _stdcall EXECryptor_GetTrialRunsLeft(int TrialRuns)
{
	return TrialRuns;
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 017h;
}

const
  char *TestKeyPath = "Software\\EXECryptorTestKeys";

bool __declspec(dllexport) _stdcall EXECryptor_SecureWrite(const char *Name, const char *Value)
{
  DWORD Disposition;
  HKEY KeyHandle;
  if (RegCreateKeyExA(HKEY_CURRENT_USER, TestKeyPath, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &KeyHandle, &Disposition) == ERROR_SUCCESS) {
    RegSetValueExA(KeyHandle, Name, 0, REG_BINARY, (CONST BYTE*)Value, (DWORD)strlen(Value)+1);
    RegCloseKey(KeyHandle);
    return true;
  } else
    return false;
}

bool __declspec(dllexport) _stdcall EXECryptor_SecureRead(const char *Name, char *Value)
{
  HKEY KeyHandle;
  DWORD sz,tp;
  bool Result = false;
  *Value=0;
  if (RegOpenKeyExA(HKEY_CURRENT_USER, TestKeyPath, 0, KEY_QUERY_VALUE, &KeyHandle) == ERROR_SUCCESS) {
    tp=REG_BINARY;
    if (RegQueryValueExA(KeyHandle, Name, NULL, &tp, NULL, &sz) == ERROR_SUCCESS) {
      RegQueryValueExA(KeyHandle, Name, NULL, &tp, (BYTE*)Value, &sz);
      Result=true;
    }
    RegCloseKey(KeyHandle);
  };
  return Result;
}

int __declspec(dllexport) _stdcall EXECryptor_MessageBoxA(HWND hWnd, LPCSTR lpText,
  LPCSTR lpCaption, UINT uType)
{
  EMIT5
  return MessageBoxA(hWnd, lpText, lpCaption, uType);
}

FARPROC __declspec(dllexport) _stdcall EXECryptor_GetProcAddr(HMODULE hModule, LPCSTR lpProcName)
{
  EMIT5
  return GetProcAddress(hModule, lpProcName);
}

void __declspec(dllexport) _stdcall EXECryptor_AntiDebug()
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 014h;
}

void __declspec(dllexport) _stdcall EXECryptor_ProtectImport()
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 015h;
}

#ifdef ANTIDEBUG_ON_INIT

class TInitialize {
  public:
  TInitialize() {
  EXECryptor_ProtectImport();
  EXECryptor_AntiDebug();
  }
};

static TInitialize _TInitialize;

#endif

#endif

#ifdef USE_CRYPT_REG
#ifdef USE_STD_SERIALS

TVerifyResult __declspec(dllexport) _stdcall EXECryptor_VerifySerialNumber(const char *RegName,
  const char *SerialNumber, int CurrentYear, int CurrentMonth,
  TSerialNumberInfo *SNInfo, const char *HardwareID)
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 003h;
  return vrOK;
}

#else

void __declspec(dllexport) _stdcall EXECryptor_SetCodeKey(const void *Key, int Size)
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 005h;
}

#endif

TVerifyResult __declspec(dllexport) _stdcall EXECryptor_IsRegistered()
{
  EMIT5
  __asm _emit 0EBh;
  __asm _emit 001h;
  __asm _emit 004h;
  return vrOK;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_0()
{
  EMIT5
  return 0;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_1()
{
  EMIT5
  return 1;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_2()
{
  EMIT5
  return 2;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_3()
{
  EMIT5
  return 3;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_4()
{
  EMIT5
  return 4;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_5()
{
  EMIT5
  return 5;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_6()
{
  EMIT5
  return 6;
}

__declspec(dllexport) DWORD _stdcall EXECryptor_RegConst_7()
{
  EMIT5
  return 7;
}

#endif

#pragma optimize( "", off )
