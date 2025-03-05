//----------------------------------------------------------------------
// MStringList.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStringList.h"

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MStringList::MStringList()
{
}

MStringList::~MStringList()
{
	Release();
}


//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MStringList::Release()
{
	iterator iString = begin();

	while (iString != end())
	{
		MString* pString = *iString;

		if (pString!=NULL)
		{
			delete pString;
		}

		iString++;
	}

	clear();
}

//----------------------------------------------------------------------
// Add String
//----------------------------------------------------------------------
// list의 끝에 str을 추가한다.
//----------------------------------------------------------------------
void			
MStringList::AddString(const char* str)
{
	MString* pString = new MString(str);

	push_back( pString );
}

//----------------------------------------------------------------------
// Get Iterator
//----------------------------------------------------------------------
// index번째 iterator를 얻는다.
// 0-based
//----------------------------------------------------------------------
MStringList::STRING_LIST::iterator		
MStringList::GetIterator(unsigned int index)
{
	// 한계를 넘어가는 경우
	if (index>=size())
	{
		return end();
	}

	iterator iString = begin();

	for (int i=0; i<index; i++)
	{
		iString++;
	}

	return iString;
}