/*-----------------------------------------------------------------------------

	directory.h

	`Program을 Release할 때, 참조되는 file들의 경로가 어떻게 될지 모르므로
    Program이 실행되는 위치에서 path를 얻어 저장해서, 그 path를 바탕으로
	 sub directory를 참조 한다. 물론 이때, sub directory의 이름은 미리 정해진다.

	19991202. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include <windows.h>

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