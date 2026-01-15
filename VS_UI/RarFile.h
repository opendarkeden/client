// RarFile.h: interface for the CRarFile class.
// Made by ���� 2001. 7. 24
//////////////////////////////////////////////////////////////////////

#ifndef _RAR_FILE_HEADER_
#define _RAR_FILE_HEADER_

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include "../../basic/Platform.h"
#endif
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
	// ������
	CRarFile();
	CRarFile(const char *rar_filename, const char *pass);

	// �Ҹ���
	~CRarFile();

	// �о�� rar�� �����Ѵ�.
	void Release();

	// rar���� �̸� & �н� ����
	void SetRAR(const char *rar_filename, const char *pass);

	// ���� ����
	bool Open(const char *in_filename);

	// ���Ͽ��� �б�
	char*	Read(char *buf, int size);
	char*	Read(int size);
	bool	GetString(char* buf, int size);

	// ������ ���ȳ�?
	bool	IsSet()	{ return (m_rar_filename !=""); }

	//	������ ���̸� true else false
	bool	IsEOF(int plus = 0);

	std::vector<std::string> *GetList(char *filter = NULL);

	char* GetFilePointer(){return m_file_pointer;};



};

#endif
