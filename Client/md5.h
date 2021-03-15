////////////////////////////////////////////////////////////////////
///md5.h this is the head of the class md5.
#include <windows.h>
#include <fstream>

#ifndef __MD5CLASS__
#define __MD5CLASS__

#define ENGLISH

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

typedef LONGLONG LONG64;

class CMd5
{
public:
	CMd5();
	CMd5(char *filename);
	~CMd5();
	LPCSTR TargetFile(char *filename=NULL);
	void TargetStr(LPCSTR str);
	LPCSTR GetDigestKey();
	LPCSTR GetErr();


private:
	inline ULONG LRotate(ULONG Sdata,int nBit);
	inline ULONG F(ULONG x,ULONG y,ULONG z);
	inline ULONG G(ULONG x,ULONG y,ULONG z);
	inline ULONG H(ULONG x,ULONG y,ULONG z);
	inline ULONG I(ULONG x,ULONG y,ULONG z);
	inline void FF(ULONG &a,ULONG b,ULONG c,ULONG d,ULONG Msg,int nBit,ULONG Cnt);
    inline void GG(ULONG &a,ULONG b,ULONG c,ULONG d,ULONG Msg,int nBit,ULONG Cnt);
    inline void HH(ULONG &a,ULONG b,ULONG c,ULONG d,ULONG Msg,int nBit,ULONG Cnt);
    inline void II(ULONG &a,ULONG b,ULONG c,ULONG d,ULONG Msg,int nBit,ULONG Cnt);
private:
	bool Init();
	bool OpenFile();
	bool Read64Byte();
	void TransForm();
	bool CaculateFile();
	bool CaculateStr(LPCSTR str);
	void SetErr(int nErr);
	void JustifytoStr();
protected:
	fstream Sfile;
	ULONG *pM;
	LPVOID Pointer;
	ULONG T[4];
	bool IsSuc;
	char fname[MAX_PATH];
	LONG64 FileLen;
	static const ULONG Context[4];
	static const LPSTR ErrInfo[5][2];
	char strErr[50];
	char Digest[33];
};

#endif
/*　　MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
　　　MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
　　　MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
　　　MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
　　　MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
　　　MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
d174ab98d277d9f5a5611c2c9f419d9f
　　　MD5 ("123456789012345678901234567890123456789012345678901234567890123456789
01234567890") = 57edf4a22be3c955ac49da2e2107b67a
*/