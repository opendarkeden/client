//-----------------------------------------------------------------------------
// AppendPatchInfo.h
//-----------------------------------------------------------------------------
// OriginalFile과 FirstSize를 알고 있으면 
// AppendFile과 AppendSize를 계산해서 패치를 할 수 있다.
//
// 이 정보를 화일로 빼두면 실행화일 바꿀 필요 없다.
//-----------------------------------------------------------------------------

#ifndef __APPENDPATCHINFO_H__
#define __APPENDPATCHINFO_H__

#pragma warning(disable:4786)

#include "MString.h"
#include "CTypeTable.h"


#include <vector> 
using namespace std;

//-----------------------------------------------------------------------------
// APPEND_PATCH_NODE
//-----------------------------------------------------------------------------
class APPEND_PATCH_NODE
{
	public :
		bool		Append(const char* orgFilename) const;

		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);

	public :
		MString		appendFilename;
		long		appendFilesize;
		WORD		appSpkSize;		
		long		writePosition;
		long		afterFilesize;
		WORD		afterSpkSize;
};

//-----------------------------------------------------------------------------
// AppendPatch
//-----------------------------------------------------------------------------
class AppendPatch {
	public :
		typedef std::vector<APPEND_PATCH_NODE*>		APPEND_PATCH_VECTOR;

	public :
		AppendPatch();
		~AppendPatch();

		void		Release();

		int			GetSize() const			{ return m_AppendPatch.size(); }
		
		const char* GetOriginalFilename() const	{ return m_orgFilename.GetString(); }

		//------------------------------------------------------------------
		// Set Info
		//------------------------------------------------------------------
		void		SetOriginalInfo(const char* orgFilename, long orgFilesize, WORD orgSpkSize);
		void		AddAppendInfo(const char* appendFilename, long appFilesize, WORD appSpkSize);
		bool		AddAppendInfo(const char* currentFilename);
		void		SetFinalInfo(long finalFilesize, WORD finalSpkSize);
		bool		SetFinalInfo(const char* currentFilename);


		//------------------------------------------------------------------
		// Execute / Check
		//------------------------------------------------------------------
		bool		ExecutePatch() const;
		bool		CheckFinalInfo() const;			// final file을 읽어봐서
		bool		CalculateFinalInfo() const;		// 계산으로

		//------------------------------------------------------------------
		// File I / O
		//------------------------------------------------------------------
		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);
		const APPEND_PATCH_VECTOR	GetAppendPatchNode() { return m_AppendPatch; }


	protected :
		MString					m_orgFilename;
		long					m_orgFilesize;
		WORD					m_orgSpkSize;

		long					m_finalFilesize;
		WORD					m_finalSpkSize;
	
		APPEND_PATCH_VECTOR		m_AppendPatch;
};

class  AppendPatchTable : public CTypeTable<AppendPatch> {
	public :
		AppendPatchTable() {}
		~AppendPatchTable() {}

		int			GetAllSize() const;
};

#endif
