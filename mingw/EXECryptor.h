#ifndef EXECRYPTOR_H
#define EXECRYPTOR_H

#include <windows.h>

// EXECryptor API v. 2.1.16

#define USE_API
//#define USE_CRYPT_REG
//#define USE_STD_SERIALS

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_API
void SafeGetDate(int *Day, int *Month, int *Year);
DWORD __declspec(dllexport) __stdcall EXECryptor_GetHardwareID();

void __declspec(dllexport) _stdcall EXECryptor_EncryptStr(const char *Src, char *Dst);
void __declspec(dllexport) _stdcall EXECryptor_DecryptStr(const char *Src, char *Dst);
bool __declspec(dllexport) _stdcall EXECryptor_SecureWrite(const char *Name, const char *Value);
bool __declspec(dllexport) _stdcall EXECryptor_SecureRead(const char *Name, char *Value);

int __declspec(dllexport) _stdcall EXECryptor_GetTrialDaysLeft(int TrialPeriod);
int __declspec(dllexport) _stdcall EXECryptor_GetTrialRunsLeft(int TrialRuns);

int __declspec(dllexport) _stdcall EXECryptor_MessageBoxA(HWND hWnd, LPCSTR lpText,
  LPCSTR lpCaption, UINT uType);
FARPROC __declspec(dllexport) _stdcall EXECryptor_GetProcAddr(HMODULE hModule, LPCSTR lpProcName);

void __declspec(dllexport) _stdcall EXECryptor_AntiDebug();
void __declspec(dllexport) _stdcall EXECryptor_ProtectImport();
#endif

#ifdef USE_CRYPT_REG

enum TVerifyResult { vrInvalid, vrExpired, vrOK };

#ifdef USE_STD_SERIALS

#pragma pack(push,1)
typedef struct {
  int LicType;    //0..15
  // if LicType = 1 then we get ExpiryMonth/ExpiryYear
  // otherwise we get UserParam
  int UserParam;  //0..1023
  int ExpiryMonth,//1..12
      ExpiryYear; //2004..2024
  bool F1,F2,F3,F4,F5,F6;
} TSerialNumberInfo;
#pragma pack(pop)

TVerifyResult __declspec(dllexport) _stdcall EXECryptor_VerifySerialNumber(const char *RegName,
  const char *SerialNumber, int CurrentYear, int CurrentMonth,
  TSerialNumberInfo *SNInfo = NULL, const char *HardwareID = NULL);

#else

void __declspec(dllexport) _stdcall EXECryptor_SetCodeKey(const void *Key, int Size);

#endif

TVerifyResult __declspec(dllexport) _stdcall EXECryptor_IsRegistered();

DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_0();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_1();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_2();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_3();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_4();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_5();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_6();
DWORD __declspec(dllexport) _stdcall EXECryptor_RegConst_7();
#endif 

#ifdef __cplusplus
};
#endif

#define CRYPT_START  { \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0F8h; \
        {

#define CRYPT_START_UNSAFE   \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0F8h

#define CRYPT_END  } \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0FFh; \
	}

#define CRYPT_END_UNSAFE    \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0FFh 

#ifdef USE_CRYPT_REG

#define CRYPT_REG  { \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0F9h; \
        {

#define CRYPT_REG_UNSAFE   \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0F9h

#define CRYPT_UNREG  { \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0FAh; \
        {

#define CRYPT_UNREG_UNSAFE   \
	__asm _emit 0EBh \
	__asm _emit 006h \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0EBh \
	__asm _emit 0FCh \
	__asm _emit 0FFh \
	__asm _emit 0FAh

#endif

#endif
