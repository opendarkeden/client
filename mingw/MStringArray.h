//---------------------------------------------------------------------------
// MStringArray.h
//---------------------------------------------------------------------------

#ifndef __MSTRINGARRAY_H__
#define	__MSTRINGARRAY_H__

#include "MString.h"
#include "CTypeTable.h"

class MStringArray : public CTypeTable<MString> {
	public :
		MStringArray();
		~MStringArray();

	protected :
};

#endif
