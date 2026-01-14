/*-----------------------------------------------------------------------------

	directory.h

	`Program�� Release�� ��, �����Ǵ� file���� ��ΰ� ��� ���� �𸣹Ƿ�
    Program�� ����Ǵ� ��ġ���� path�� ��� �����ؼ�, �� path�� ��������
	 sub directory�� ���� �Ѵ�. ���� �̶�, sub directory�� �̸��� �̸� ��������.

	19991202. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include "Platform.h"

/*-----------------------------------------------------------------------------
  Class Directory
-----------------------------------------------------------------------------*/
class C_DIRECTORY
{
private:
	char * m_sz_program_directory;
	char * m_sz_mixed_directory;

public:
	C_DIRECTORY();
	~C_DIRECTORY();

/*-----------------------------------------------------------------------------
  Get
-----------------------------------------------------------------------------*/
	char *	GetMixedPathWidthProgramDir(const char *name) const;
	char *	GetProgramDirectory() const;
	char *	GetMixedDirectory(const char * path, const char * filename) const;

/*-----------------------------------------------------------------------------
  Set/Test
-----------------------------------------------------------------------------*/
	bool		ExistFile(const char * fullpath) const;
};

/*-----------------------------------------------------------------------------
  EXTERN
-----------------------------------------------------------------------------*/
extern C_DIRECTORY	gC_directory;

#endif