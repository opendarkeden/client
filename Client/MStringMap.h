//----------------------------------------------------------------------
// MStringMap.h
//----------------------------------------------------------------------
// string의 (key, value)가 하나의 노드이다.
//
// 그런데~ value 자체가 key가 되는 경우도 있다.
// 이런 경우, 메모리를 아끼기 위해서.. 
// key와 value는 같은 pointer값이 될 것이다.
// delete할때 이거를 염두해야한다.
//----------------------------------------------------------------------

#ifndef __MSTRINGMAP_H__
#define __MSTRINGMAP_H__

#pragma warning(disable:4786)

#include "MString.h"
ifstream;
std::ofstream;

//----------------------------------------------------------------------
// MString*의 값을 비교한다. 적은게 먼저..
// 이거 필요없는거 같기도 한데... 확인하기 귀찮아서 그냥.. ㅡ.ㅡ;
//----------------------------------------------------------------------
class MStringPointerCompare {
	public :
		bool operator () ( MString* pLeft, MString* pRight ) const
		{
			return *pLeft < *pRight;
		}
};

//----------------------------------------------------------------------
// MString*의 map
//----------------------------------------------------------------------
class MStringMap : public std::map<MString*, MString*, MStringPointerCompare> {
	public :
		typedef std::map<MString*, MString*, MStringPointerCompare>	STRING_MAP;
		typedef STRING_MAP::iterator			iterator;
		typedef STRING_MAP::const_iterator		const_iterator;

	public :
		MStringMap();
		~MStringMap();

		//-------------------------------------------------------
		// Release
		//-------------------------------------------------------
		void				Release();

		//-------------------------------------------------------
		// Add / Remove / Get
		//-------------------------------------------------------
		bool				Add(const char* pKey, const char* pValue=NULL);
		bool				Remove(const char* pKey);
		const MString*		Get(const char* pKey) const;

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void				SaveToFile(std::ofstream& file);
		void				LoadFromFile(ifstream& file);

	protected :
		
};

#endif

