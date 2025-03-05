//#define NETMARBLE_DEBUG
#include "Client_PCH.h"
#include <windows.h>
#include "Assert.h"
#include "NMCrypt.h"
#include "MString.h"

namespace
{
	MString g_NetmarbleCommandLine = "";
};

struct NETMARBLE_INFO
{
	MString ID;
	MString Password;
	int WorldID;
	int ServerID;
	bool bGore;
};

// 2004, 7, 14, sobeit add start
struct REALSERVER_INFO
{
	bool bMode;		// 0: 수동접속, 1:정상접속
	int WorldID;	// 0: 브란, 1: 펠레스
	MString ID;		// id
	MString Key;	// Key
};
// 2004, 7, 14, sobeit add end
std::string g_ServIP;//     = arg[0];					// 서버 IP
DWORD g_ServPort;//   = (UINT)::atoi(arg[1]);	// 서버 Port
std::string g_AuthCookie;// = arg[2];					// 인증 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
std::string g_DataCookie;// = arg[3];					// 데이터 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
std::string g_CpCookie;//   = arg[4];					// CP쿠키		- 버퍼 크기를1024*2 정도로 잡아야 한다
std::string g_SpareParam;///*= arg[5];					// 여분의 인자(CP게임등에서 사용)*/

char* _StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받고
	// 인자가 비어있는 경우도 인자로 인정함

	static const int TOK_BUFSIZE = 1024*40;

	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];

	if(str) {
		int slen = strlen(str);
//		assert(slen < TOK_BUFSIZE);		// ASSERT
		strcpy(buf, str);
		pnow = buf;
	}

	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep) {
			++pnow;
			break;
		}

		++tlen;
		++pnow;
	}

	if(tlen > 0) {
		strncpy(token, porg, tlen);
		token[tlen] = 0;
	}
	else {
		token[0] = 0;
	}

	return token;
}

BOOL AnalyzeArgument(char *key)
{
	//
	// 클립보드 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
	// 전달 인자의 순서 : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (인자는 암호화되어 있다)
	// - 넷마블 게임에서는 CpCookie를 전달하지 않는다
	// - CP 게임에서는 AuthCookie, DataCookie를 전달하지 않는다(몇몇 CP게임은 예외)
	// - Extra 인자는 생략되기도 한다
	//

	char buf[1024*20] = {0,};	// 버퍼는 충분히 크게 잡는다

	// 클립보드에서 인자 데이터를 읽어온다(한번 얻어오면 삭제됨)
	if(!GetNMClipData(buf, sizeof(buf)-1, key, true))
		return FALSE;

	if(strlen(buf)==0)
		return FALSE;

	char arg[6][1024*2] = {0,};

	int argcnt = 0;
	char* token = _StrTok(buf, ',');
	while(token && argcnt < 6)
	{
		strcpy(arg[argcnt], token);
		argcnt++;
		token = _StrTok(NULL, ',');
	}

	// 인자가 최소한 5개는 되어야 한다
	if(argcnt < 5)
		return FALSE;

	// IP주소가 올바른지 검사
	if(strlen(arg[0]) > 16)
		return FALSE;

	// CP 게임은 아래부분을 고쳐 쓸것
	g_ServIP     = arg[0];					// 서버 IP
	g_ServPort   = (UINT)::atoi(arg[1]);	// 서버 Port
	g_AuthCookie = arg[2];					// 인증 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_DataCookie = arg[3];					// 데이터 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_CpCookie   = arg[4];					// CP쿠키		- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_SpareParam = arg[5];					// 여분의 인자(CP게임등에서 사용)
	
	return TRUE;
}

int HexStringToDec(char *str)
{
	int dec = 0;
	const int strLen = strlen(str);

	for(int i = strLen-1, multi = 1; i >= 0; i--, multi *= 16)
	{
		int num;
		if(str[i] >= '0' && str[i] <= '9')
			num = str[i] - '0';
		else
		if(str[i] >= 'A' && str[i] <= 'F')
			num = str[i] - 'A' + 10;
		else
			return -1;

		dec += num*multi;
	}
	return dec;
}

// 넷마블용
//-----------------------------------------------------------------------
// ParsingNetmarble
//-----------------------------------------------------------------------
// DarkEden.exe ID&PASS&WORLDID&SERVERID
//-----------------------------------------------------------------------
bool
ParsingNetmarble(const char* pCommandLine, NETMARBLE_INFO &info)
{
#ifdef NETMARBLE_DEBUG
	MessageBox(NULL,pCommandLine,"CommandLine",MB_OK);
#endif
	char szTemp[4096];

	const char *pString = strchr(pCommandLine, ' ');
	if(pString == NULL)
	{
		return false;
	}

	strcpy(szTemp, pString+1);
		
	BOOL bResult = AnalyzeArgument( szTemp );

	if( bResult == FALSE )
	{
		MessageBox( NULL, "넷마블 홈페이지를 통해서 다시 실행해주세요.","NetmarbleDarkEden",MB_OK);
		exit(0);
		Assert( bResult );
	}

	info.WorldID = g_ServPort/10;
	info.ServerID = g_ServPort%10;

//	sprintf( szTemp, "%d %d", info.WorldID, info.ServerID );
//	MessageBox( NULL, szTemp, szTemp, MB_OK );
	
	info.ID = g_CpCookie.c_str();
	info.Password = "test";

//	strcpy( szTemp, g_NetmarbleCommandLine.GetString() );
////	strcpy( szTemp, "2,20,netmetro3,netmetro3NM00011EB7BA45");
//
//	char *pszTemp, *pszTemp2;
////	char* pStr;
//	
//	pszTemp = szTemp;
//
//	//-------------------------------------------------
//	// Teen 축출
//	//-------------------------------------------------
////	pszTemp = pszTemp2+1;
//	pszTemp2 = strchr(pszTemp, ',');
//
//	// 파싱용 &를 찾지 못하면 에러!
//	if(pszTemp2 == NULL)
//		return false;
//
//	*pszTemp2 = '\0';
//
//	if(strlen(pszTemp) != 1)
//		return false;
//
//	switch(atoi(pszTemp))
//	{
//	case 0:
//	case 1:
////		g_pUserInformation->GoreLevel = false;
//		info.bGore = false;
//		break;
//	
//	case 2:
////		g_pUserInformation->GoreLevel = true;
//		info.bGore = true;
//		break;
//	}
//
//	//-------------------------------------------------
//	// WorldID 축출
//	//-------------------------------------------------
//	pszTemp = pszTemp2+1;
//	pszTemp2 = strchr(pszTemp, ',');
//
//	// 파싱용 &를 찾지 못하면 에러!
//	if(pszTemp2 == NULL)
//	{
//		return false;
//	}
//
//	*pszTemp2 = '\0';
//
//	if(strlen(pszTemp) != 2)
//	{
//		return false;
//	}
//	
//	int num = atoi(pszTemp);
////	g_pUserInformation->NetmarbleWorldID = num/10;
////	g_pUserInformation->NetmarbleServerID = num%10;
//	info.WorldID = num/10;
//	info.ServerID = num%10;
//
//	//-------------------------------------------------
//	// ID 축출
//	//-------------------------------------------------
//	// 2,10,larosellarosel,larosellaroselNM000076B17852
//	pszTemp = pszTemp2+1;
//	pszTemp2 = strchr(pszTemp, ',');
//
//	// 파싱용 &를 찾지 못하면 에러!
//	if(pszTemp2 == NULL)
//	{
//		return false;
//	}
//
//	*pszTemp2 = '\0';
//	// ID가 12byte 이상이면 에러!
//	if(strlen(pszTemp) > 24)
//	{
//		return false;
//	}
//
////	g_pUserInformation->NetmarbleID = szTemp;

//	g_pUserInformation->NetmarbleID = g_CpCookie.c_str();
//	
//	//-------------------------------------------------
//	// Password 축출
//	//-------------------------------------------------
//	pszTemp = pszTemp2+1;
//	pszTemp2 = pszTemp+strlen(pszTemp);
//
//	// ID만큼 제낀다.
//	pszTemp += info.ID.GetLength();
//
////	pszTemp2 = strchr(pszTemp, ',');
//
//	// 파싱용 &를 찾지 못하면 에러!
////	if(pszTemp2 == NULL)
////		return false;
//
//	*pszTemp2 = '\0';
//
//	// ID가 5byte 이하이면 에러!
//	if(strlen(pszTemp) != 14)
//	{
//		return false;
//	}
//
//	switch(HexStringToDec(pszTemp+12))
//	{
//	case 'M':
//	case 'E':
//	case 'T':
//	case 'R':
//	case 'O':
//	case 'C':
//	case 'H':
//		break;
//
//	default:
//		return false;
//	}
//
//	*(pszTemp2-2) = '\0';
//
//	if(pszTemp[0] != 'N' || pszTemp[1] != 'M')
//	{
//		return false;
//	}
//
//	int uniID = HexStringToDec(pszTemp+2);
//	if(uniID == -1)
//	{
//		return false;
//	}
//
//	sprintf(szTemp, "%d", uniID);
//	int skip = 12 - strlen(szTemp);
//	
//	sprintf(szTemp, "NM0000000000");
//	sprintf(szTemp+skip, "%d", uniID);
//	info.Password = szTemp;
//
////	g_pUserInformation->NetmarblePassword = pszTemp;
//	g_pUserInformation->NetmarblePassword = "test";

	return true;
}



/* old version
char* _StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받고
	// 인자가 비어있는 경우도 인자로 인정함

	static const int TOK_BUFSIZE = 4096;

	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];

	if(str) {
		int slen = strlen(str);
		Assert(slen < TOK_BUFSIZE);		// ASSERT

		strcpy(buf, str);
		pnow = buf;
	}

	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep) {
			++pnow;
			break;
		}

		++tlen;
		++pnow;
	}

	if(tlen > 0) {
		strncpy(token, porg, tlen);
		token[tlen] = 0;
	}
	else {
		token[0] = 0;
	}

	return token;
}
 


BOOL AnalizeArgument(const char *strarg)
{
#ifdef NETMARBLE_DEBUG
	MessageBox(NULL,strarg,strarg,MB_OK);
#endif
	//
	// 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
	// 전달 인자의 순서 : ServerIP,Port,UserID,Pass,Extra (인자는 암호화되어 있다)
	// - Extra 인자는 생략되기도 한다
	//

	// arg[0],arg[1],arg[2],arg[2]arg[3]arg[4]
	// 2,10,larosellarosel,larosellaroselNM000076B17852

	// 인자가 너무 길면 안된다
	if(strlen(strarg) >= 4096)
		return FALSE;

	char key[1024] = {0,};
	char buf[4096] = {0,};

	// 암호 해독 키를 얻어온다(한번 얻어오면 삭제됨)
	if(!GetCryptKey(key, 1024-1, false)) 
	{
		return FALSE;
	}

	// 복호화 성공여부 검사
	if(CRYPT_SUCCESS != DecryptString(strarg, key, buf))
	{
		return FALSE;
	}

	// -_- netmarble 에서 보내주는 extra 코드는 안쓴다.
	char *endchar = strstr(buf,",NM");

	if( endchar != NULL )
		*endchar = NULL;

	buf[ strlen(buf) - 1 ] = '\0';
	
	g_NetmarbleCommandLine = buf;

//	char arg[5][1024] = {0,};
//
//	int argcnt = 0;
//	char* token = _StrTok(buf, ',');
//	while(token && argcnt<5)
//	{
//		strcpy(arg[argcnt], token);
//		argcnt++;
//		token = _StrTok(NULL, ',');
//	}

	// 인자가 최소한 4개는 되어야 한다
//	if(argcnt < 4)
//		return FALSE;

	// IP주소가 올바른지 검사
//	if(strlen(arg[0]) > 16)
//		return FALSE;

	// CP 게임은 아래부분을 고쳐 쓸것
//	g_ServIP    = arg[0];		// 서버 IP
//	g_ServPort  = (UINT)::atoi(arg[1]);	// 서버 Port
//	g_LoginID   = arg[2];		// 로그인 ID
//	g_LoginPass = arg[3];		// 로그인 패스워드(CP게임이라면 유니크 넘버)
//	g_Extra     = arg[4];		// 여분의 인자(CP게임등에서 사용)
	
	return TRUE;
}
*/
