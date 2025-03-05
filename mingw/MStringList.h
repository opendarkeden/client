//----------------------------------------------------------------------
// MStringList.h
//----------------------------------------------------------------------

#ifndef __MSTRINGLIST_H__
#define __MSTRINGLIST_H__

#pragma warning(disable:4786)

#include <list>
#include "MString.h"

class MStringList : public std::list<MString*> {
	public :
		typedef std::list<MString*>		STRING_LIST;

	public :
		MStringList();
		virtual ~MStringList();

		//----------------------------------------------------------
		// Release
		//----------------------------------------------------------
		void			Release();

		//----------------------------------------------------------
		// Add String
		//----------------------------------------------------------
		void			AddString(const char* str);

		//----------------------------------------------------------
		// Get Iterator - get index-th iterator
		//----------------------------------------------------------
		iterator		GetIterator(unsigned int index);

	protected :

};

#endif


