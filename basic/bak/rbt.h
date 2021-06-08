/*-----------------------------------------------------------------------------

	rbt.h

	RBT (RaBbiT).
	Data Base file format.

	2000.6.8. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __RBT_H__
#define __RBT_H__

#include "typedef.h"
#include <fstream.h>
#include <windows.h> // for MAX_PATH & file search

typedef DWORD data_id_t, data_count_t;
typedef int rbt_result_t;

//----------------------------------------------------------------------------
// Class RBT (RaBbiT)
// 하나의 RBT file이다.
//
// `아주 큰 용량의 RBT file이라면, 예를 들어 1GB짜리일 경우 open해서 전부를
//  read한다면, 1GB의 Memory를 필요로 할 것이다. 따라서 아무리 큰 RBT file을
//  읽기 위해서 header를 두고, header에서 data id를 검색하여 필요로 하는 data만
//	 read한다. 따라서 RBT file의 구조는 다음과 같다.
//
//	 RBT:
//			| header(DATA_UNITs) | data 1 | data 2 | data 3| ... |
//
//
//  이 구조는 header로써 data의 기본적인 정보를 빠르게 얻을 수 있는 장점이 있다.
//	 또한 header가 node마다 있을 경우보다 간단하며, 더 빠르게 data를 얻을 수 있다.
//
// `file만 RBT에 추가할 수 있다.
//----------------------------------------------------------------------------
#define RBT_OK									-1
#define RBT_FILE_EMPTY						0
#define RBT_OPEN_FAILED						1
#define RBT_MEMALLOC_FAILED				2
#define RBT_FILE_NOT_EXIST					3
#define RBT_NO_DATA							4
#define RBT_SIZE_ZERO						5
#define RBT_ARG_SIZE_ZERO					6
#define RBT_ARG_PTR_NULL					7
#define RBT_DATA_ID_NOT_FOUNT				8
#define RBT_SIZE_NOT_MATCH					9
#define RBT_ARG_TOO_BIG						10
#define RBT_READ_SIZE_BAD					11
#define RBT_ALREADY_EXIST					12

class RBT
{
private:
	struct FILE_LIST
	{
		char			file[MAX_PATH];
	};

	FILE_LIST *		m_p_file_list;
	int				m_file_list_size;

public:
	// 하나의 data에 대한 정보.
	struct DATA_HEAD
	{
		//
		// id
		//
		// id는 unique해야 하며, (0 ~ 0xFFFFFFFF)의 범위의 값이다. 따라서 최대 0xFFFFFFFF개의
		// data를 저장할 수 있지만, 크기가 0인 것은 포함하지 않으므로 OS에서 지원하는 file size를
		// 넘지못한다.
		//
		// 어떤 id의 data를 삭제했다면, 그 data의 header인 DATA_HEAD도 삭제되며, 삭제된 id는
		// 최대값을 넘은 이후에 나올 수 있다. 그러나 replace는 data만 바꾼다.
		//
		data_id_t	id;
		long	start_pos;
		size_t		size;

		//
		// pathfile
		//
		// file로써 add될 경우 head는 file name(+path)을 갖는다. file name을 갖으므로써
		// 생기는 이점은 다음과 같다.
		//   
		// (1) 같은 file을 갖지 않게 할 수 있다.
		//	(2) 그 file로 복원할 수 있다.
		//	(3) data id가 아닌 file로써 data를 찾을 수 있다.
		//
		size_t		file_len;
		char *		pathfile;
	};

private:
	data_count_t	m_count; // data의 총 개수
	DATA_HEAD *		m_p_header;

	char *			m_pathrbt; // this rbt file when open or save.

private:
	void	FreeFileList();
	void	FreeHeader();
	data_id_t GetDataID() const;

public:
	RBT(const char * sz_pathfile = NULL);
	~RBT();

//----------------------------------------------------------------------------
// Open/Save
//----------------------------------------------------------------------------
	rbt_result_t	Open(const char * sz_pathfile);
	rbt_result_t	Save(const char * sz_pathfile);

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------
	rbt_result_t	AddFile(const char * sz_pathfile, data_id_t data_id);
	rbt_result_t	AddFile(const char * sz_pathfile);

//----------------------------------------------------------------------------
// Delete
//----------------------------------------------------------------------------
	rbt_result_t	Delete(data_id_t data_id);
	rbt_result_t	Delete(const char * sz_pathfile);

//----------------------------------------------------------------------------
// Replace/Update
//----------------------------------------------------------------------------
	rbt_result_t	Replace(data_id_t data_id, const char * sz_pathfile);
	rbt_result_t	Update(const char * sz_pathfile);

//----------------------------------------------------------------------------
// Read
//----------------------------------------------------------------------------
	rbt_result_t	Read(data_id_t data_id, char * &p_data, size_t size) const;
	rbt_result_t	ReadSequence(data_id_t sequence, char * &p_data, size_t size) const;

//----------------------------------------------------------------------------
// Get
//----------------------------------------------------------------------------
	size_t			GetDataSize(data_id_t data_id) const;
	size_t			GetDataSizeBySequence(data_id_t sequence) const;
	rbt_result_t	GetDataHead(data_id_t sequence, DATA_HEAD &head) const;
	data_count_t	Size() const;

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------
	rbt_result_t	Restore(data_id_t sequence) const;
	rbt_result_t	Restore() const;

//----------------------------------------------------------------------------
// Misc
//----------------------------------------------------------------------------
	rbt_result_t	MakeReport(const char * sz_pathfile) const;
	rbt_result_t	MakeFileList(const char * sz_pathfile) const;
	rbt_result_t	ExistFile(const char * sz_pathfile, data_id_t &data_id) const;
};

#endif