// RarFile.h: interface for the CRarFile class.
// Made by 쑥갓 2001. 7. 24
//////////////////////////////////////////////////////////////////////

#ifndef _RAR_FILE_HEADER_
#define _RAR_FILE_HEADER_

#pragma warning(disable:4786)

#include <windows.h>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// urarlib.h
///////////////////////////////////////////////////////////////////////////////
#define _DO_CRC32_CHECK                     /* perform cyclical redundancy  */
                                            /* check (CRC32) - disable this */
                                            /* for a little speed-up        */
#define _USE_ASM                            /*
                                             * enable assembly extensions
                                             * x86 cpus.
                                             */

#define _WIN_32                             /* Win32 with VisualC           */

typedef unsigned char    UBYTE;             /* WIN32 definitions            */
typedef unsigned short   UWORD;
typedef unsigned long    UDWORD;

/* This structure is used for listing archive content                       */
struct RAR20_archive_entry                  /* These infos about files are  */
{                                           /* stored in RAR v2.0 archives  */
  char *Name;
  UWORD NameSize;
  UDWORD PackSize;
  UDWORD UnpSize;
  UBYTE HostOS;                             /* MSDOS=0,OS2=1,WIN32=2,UNIX=3 */
  UDWORD FileCRC;
  UDWORD FileTime;
  UBYTE UnpVer;
  UBYTE Method;
  UDWORD FileAttr;
};

struct ArchiveList_struct
{
  struct RAR20_archive_entry item;
  struct ArchiveList_struct *next;
};
/*typedef struct
{
  struct RAR20_archive_entry item;
  struct ArchiveList_struct *next;
} ArchiveList_struct;*/

////////////////////////////////////////////////////////////////////////////////////

class CRarFile  
{
private:
	std::string m_rar_filename;
	std::string m_password;

	char *m_data;
	char *m_file_pointer;
	int m_size;

public:
	// 생성자
	CRarFile();
	CRarFile(const char *rar_filename, const char *pass);

	// 소멸자
	~CRarFile();

	// 읽어온 rar을 해제한다.
	void Release();

	// rar파일 이름 & 패스 세팅
	void SetRAR(const char *rar_filename, const char *pass);

	// 파일 열기
	bool Open(const char *in_filename);

	// 파일에서 읽기
	char*	Read(char *buf, int size);
	char*	Read(int size);
	bool	GetString(char* buf, int size);

	// 파일이 열렸나?
	bool	IsSet()	{ return (m_rar_filename !=""); }

	//	파일의 끝이면 true else false
	bool	IsEOF(int plus = 0);

	std::vector<std::string> *GetList(char *filter = NULL);



};

#endif
