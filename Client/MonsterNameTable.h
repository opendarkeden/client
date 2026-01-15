//----------------------------------------------------------------------
// MonsterNameTable.h
//----------------------------------------------------------------------

#ifndef __MONSTER_NAME_TABLE_H__
#define	__MONSTER_NAME_TABLE_H__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include "MString.h"
#include "CTypeTable.h"

class MonsterNameTable {
	public :
		typedef CTypeTable<MString>		STRING_TABLE;

	public :
		MonsterNameTable();
		~MonsterNameTable();

		//------------------------------------------------------------
		// Get Name
		//------------------------------------------------------------
		int			GetFirstNameSize() const	{ return m_FirstNames.GetSize(); }
		int			GetMiddleNameSize() const	{ return m_MiddleNames.GetSize(); }
		int			GetLastNameSize() const		{ return m_LastNames.GetSize(); }

		const char*	GetFirstName(int n) const	{ return ( n<m_FirstNames.GetSize()? m_FirstNames[n].GetString() : NULL ); }	// 음수는 없다고 보고. - -;
		const char*	GetMiddleName(int n) const	{ return ( n<m_MiddleNames.GetSize()? m_MiddleNames[n].GetString() : NULL ); }
		const char*	GetLastName(int n) const	{ return ( n<m_LastNames.GetSize()? m_LastNames[n].GetString() : NULL ); }

		//------------------------------------------------------------
		// File I/O
		//------------------------------------------------------------
		//void		SaveToFile(class ofstream& file);	// 서버에서 읽어온걸 쓴다는 가정하에.. 
		void		LoadFromFile(class ifstream& file);

	public :
		STRING_TABLE	m_FirstNames;
		STRING_TABLE	m_MiddleNames;
		STRING_TABLE	m_LastNames;
};

extern MonsterNameTable*			g_pMonsterNameTable;

#endif

