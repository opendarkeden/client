//=============================================================================
//    File : NPGameLib.h
//     Use : Handle Communication between Game and GameMon
//  Author : Kim Chan Ho (chkim@inca.co.kr)
//    Date : 2003/07/01 - 03
//  Modify : 2003/07/12  Prepare to redistribute
//           2003/07/15  Add Unicode support
//           2003/07/21  Encrypt strings
//           2003/08/01  Add NO_GAMEGUARD compile-time option
//           2003/08/08  Add multi-language support
//           2003/09/03  Change interface. Initialize GG first (VER 2)
//           2003/09/22  Adjust static class initializing
//           2003/10/14  Add CSAuth stuff
//           2003/10/30  Return buffered callback message when Init() is called
//           2003/11/18  Check duplicate execution
//           2004/01/09  Recover cryptoapi error automatically
//           2004/01/12  Support GameGuard Lite Version
//           2004/02/09  Authenticate myself
//           2004/02/11  Add PacketProtect stuff
//           2004/03/16  Self RC4 decrypt code
//           2004/04/02  Fix minor bug in PacketProtect
//           2004/04/14  Check GameMon CRC
//=============================================================================
#include "Client_PCH.h"
#ifndef __NPGAMELIB_H__
#define __NPGAMELIB_H__


/// Definitions

// Callback Message
#define NPGAMEMON_UNDEFINED            1000        // Undefined message
#define NPGAMEMON_COMM_ERROR           1001        // Communication error
#define NPGAMEMON_COMM_CLOSE           1002        // Communication closing

#define NPGAMEMON_SPEEDHACK            1011        // SpeedHack detected
#define NPGAMEMON_GAMEHACK_KILLED      1012        // GameHack killed
#define NPGAMEMON_GAMEHACK_DETECT      1013        // GameHack detected
#define NPGAMEMON_INIT_ERROR           1014        // GameMon Init Error
#define NPGAMEMON_GAMEHACK_DOUBT       1015        // GameHack doubt
#define NPGAMEMON_CHECK_CSAUTH         1016        // CSAuth

// Error Code  110 - 300
#define NPGAMEMON_ERROR_EXIST          110         // GameMon Already Exist
#define NPGAMEMON_ERROR_CREATE         111         // GameGuard Directory Create Error
#define NPGAMEMON_ERROR_NPSCAN         112         // npscan.des Error
#define NPGAMEMON_ERROR_THREAD         113         // CreateThread Error
#define NPGAMEMON_ERROR_INIT           114         // GameMon Initialize Error
#define NPGAMEMON_ERROR_GAME_EXIST     115         // Game Instance Already Exist
#define NPGAMEMON_ERROR_AUTH_INI       120         // .ini Authentication Fail
#define NPGAMEMON_ERROR_AUTH_NPGMUP    121         // npgmup.des Authentication Fail
#define NPGAMEMON_ERROR_AUTH_GAMEMON   122         // GameMon.des Authentication Fail
#define NPGAMEMON_ERROR_AUTH_NEWUP     123         // npgmup.des.new Auth Fail
#define NPGAMEMON_ERROR_AUTH_GAMEGUARD 124         // GameGuard.des Authentication Fail
#define NPGAMEMON_ERROR_DECRYPT        130         // .ini File Decryption Fail
#define NPGAMEMON_ERROR_CORRUPT_INI    141         // Corrupt ini file Error
#define NPGAMEMON_ERROR_CORRUPT_INI2   142         // Not match GameName in ini file Error
#define NPGAMEMON_ERROR_NFOUND_INI     150         // ini File not Found
#define NPGAMEMON_ERROR_NFOUND_NPGMUP  151         // npgmup.des not found
#define NPGAMEMON_ERROR_NFOUND_NEWUP   152         // npgmup.des.new not found
#define NPGAMEMON_ERROR_NFOUND_GG      153         // GameGuard.des not found
#define NPGAMEMON_ERROR_NFOUND_GM      154         // GameMon.des not found
#define NPGAMEMON_ERROR_CRYPTOAPI      155         // rsabase.dll is corrupted
#define NPGAMEMON_ERROR_COMM           160         // Communication Init Error
#define NPGAMEMON_ERROR_EXECUTE        170         // GameMon Execute Error
#define NPGAMEMON_ERROR_EVENT          171         // GameMon Event Create Error
#define NPGAMEMON_ERROR_EVENT2         172         // GameGuard Event Error
#define NPGAMEMON_ERROR_NPGMUP         180         // npgmup.dll Error
#define NPGAMEMON_ERROR_MOVE_INI       191         // Move ini Error
#define NPGAMEMON_ERROR_MOVE_NEWUP     192         // Move npgmup.des.new Error

#define NPGAMEMON_ERROR_ILLEGAL_PRG    200         // Detected a illegal program

#define NPGAMEMON_ERROR_GAMEMON        210         // GameMon Init Error
#define NPGAMEMON_ERROR_SPEEDCHECK     220         // SpeedCheck Init Error
#define NPGAMEMON_ERROR_GAMEGUARD      230         // GameGuard Init Error

// Error Code  310 - 400
// Update Error Code
#define NPGMUP_ERROR_DOWNCFG           340         // Download Error
#define NPGMUP_ERROR_ABORT             350         // Canceled by User
#define NPGMUP_ERROR_AUTH              360         // File Authentication Error
#define NPGMUP_ERROR_AUTH_INI          361         // .ini Authentication Error
#define NPGMUP_ERROR_DECRYPT           370         // .ini Decrypt Error
#define NPGMUP_ERROR_CONNECT           380         // Connect to update server Fail
#define NPGMUP_ERROR_INI               390         // .ini Corrupted


extern const DWORD NPGAMEMON_SUCCESS;              // GameMon Initialize Success


/// Extern Function
DWORD __cdecl CheckNPGameMon();
bool  __cdecl CloseNPGameMon();
DWORD __cdecl InitNPGameMon(HWND hWnd);
DWORD __cdecl PreInitNPGameMonA(LPCSTR szGameName);
DWORD __cdecl PreInitNPGameMonW(LPCWSTR szGameName);
bool  __cdecl SendUserIDToGameMonA(LPCSTR szUserID);
bool  __cdecl SendUserIDToGameMonW(LPCWSTR szUserID);
bool  __cdecl SendCSAuthToGameMon(DWORD dwAuth);
char* __cdecl _NPDect(char *lpszString);

#ifdef _UNICODE
#define PreInitNPGameMon      PreInitNPGameMonW
#define SendUserIDToGameMon   SendUserIDToGameMonW
#else
#define PreInitNPGameMon      PreInitNPGameMonA
#define SendUserIDToGameMon   SendUserIDToGameMonA
#endif


#ifdef __cplusplus
extern "C" {
#endif

DWORD __cdecl InitPacketProtect(LPCTSTR lpszUserKey);
DWORD __cdecl EncryptPacket(LPVOID lpData, DWORD dwLength);
DWORD __cdecl DecryptPacket(LPVOID lpData, DWORD dwLength);
DWORD __cdecl DecryptClientPacket(LPVOID lpData, DWORD dwLength);
DWORD __cdecl PPGetLastError();

#ifdef __cplusplus
}
#endif


bool CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg);

//add by zdj
#define NO_GAMEGUARD


/// CNPGameLib Wrapper Class ///
class CNPGameLib
{
public:
   explicit CNPGameLib(LPCTSTR lpszGameName)
   {
#ifdef NO_GAMEGUARD
      return;
#endif
      PreInitNPGameMon(lpszGameName);
   }
   ~CNPGameLib()
   {
#ifdef NO_GAMEGUARD
      return;
#endif
      CloseNPGameMon();
   }

public:
   DWORD Init(HWND hWnd)
   {
#ifdef NO_GAMEGUARD
      return NPGAMEMON_SUCCESS;
#endif
      return (InitNPGameMon(hWnd));
   }

   DWORD Check()
   {
#ifdef NO_GAMEGUARD
      return NPGAMEMON_SUCCESS;
#endif
      return (CheckNPGameMon());
   }

   DWORD Send(LPCTSTR lpszUserId)
   {
#ifdef NO_GAMEGUARD
      return NPGAMEMON_SUCCESS;
#endif
      return (SendUserIDToGameMon(lpszUserId));
   }

   DWORD Auth(DWORD dwAuth)
   {
#ifdef NO_GAMEGUARD
      return NPGAMEMON_SUCCESS;
#endif
      return (SendCSAuthToGameMon(dwAuth));
   }

private:
   // No copies
   CNPGameLib(const CNPGameLib &rhs);
   CNPGameLib &operator=(const CNPGameLib &rhs);
};


#endif // __NPGAMELIB_H__
