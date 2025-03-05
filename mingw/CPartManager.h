//----------------------------------------------------------------------
// CPartManager.h
//----------------------------------------------------------------------
// index(IndexType)개수 중에서 
// 일부(PartIndexType)만 memory를 잡아서 DataType으로 사용하도록 한다.
//
//----------------------------------------------------------------------
//  Index : PartIndex    PartIndex : Data        : 실제Index
//----------------------------------------------------------------------
//	    0 : .					 0 : DataType[4] : 4
//	    1 : .					 1 : DataType[2] : 2
//	    2 : 1					 2 : DataType[3] : 3
//	    3 : 2					 
//	    4 : 0					
//	    5 : .					
//----------------------------------------------------------------------
//
// [필요성]
//
// 하나의 Data가 1K이고 사용할려는 Data의 Array가 1만개라면..
// 약 10M의 메모리가 필요하다. 
//
// 그러나, 실제로 10M의 메모리를 그 Data들을 위해서 할당해주는게 힘들경우
// 1M정도의 여유만 있어서 1M만 써야한다면..??
//
// 1M에 해당하는 개수(천개=PartIndexType)만큼만 
// DataType으로 메모리를 잡아두고
// 실제로 전체 Data개수에 해당하는 만큼의 Array를 잡아두고
// 이것은 실제 Index(만개)가 PartIndex(천개)를 가리키도록 한다.
// 즉, 실제Index의 값인 PartIndex를 통해서 PartIndex에 해당하는 
// Data를 얻을 수 있다. 
//
//
// [구현 방법] 
//
// 그렇다면, 어떻게 천개(1M)를 이용해서 만개(10M)를 적절히 사용할 수 있을까?
//
// 일단.. 사용에 필요하다는 요청에 의해서 
// PartIndex의 Data에 천개의 Data를 생성한다.
// (예) Index 1의 Data가 필요하다면, 
//      Index 1을 보고 .. PartIndex가 있으면 그 PartIndex의 Data를 사용한다.
//      PartIndex가 없다면, 그 Data는 없는 경우이므로 Load해야 한다.
//      PartIndex가 꽉 찼다면.. 가장 쓸모 없는 것을 하나 없애야 하는데,
//      이때 LRU(Least Recently Used) 방법을 이용한다.
//      제거대상이 된 PartIndex의 실제Index의 PartIndex에는 INDEX_NULL 표시
//		...
//
// LRU는 어떻게 구현할까?
//      //가장 오래전에 사용한 것을 알아내야 한다.
//      //Data사용할 때 마다.. 그 index에 '최근시간' 표시를 해 둔다.
//      //LRU를 찾아야 할 경우는.. '최근 시간'이 가장 오래전인걸 제거한다.
//      //단, 이렇게만 구현하면 LRU를 찾을 때, 모두 비교 할 수 밖에 없다 T_T
//
//      //시간에 따른 Priority Queue를 사용하면 좀 빠를까?
//      //그러면, 일반적인 추가시간에 속도를 손해보는데...
//
//      LRU list를 사용한다.
//
//      PartIndex data들 이외에 LRU list를 가지고 .. 다음과 같은 순서로 유지한다.
//      LRU list :  MRU ----> LRU 
//
//      이 경우, PartIndexIterator를 둬서 각각 list의 자기 node를 가리키도록 해야한다.
//
//      [사용할때] partIndex의 iterator로 LRU list에 접근해서 그 node를 front로 옮긴다.
//      [LRU 찾기] LRU list의 back이 LRU index이다.
//      [처음 생성시] list의 front에 추가하고 partIndexIterator에 iterator를 설정한다.
//
//----------------------------------------------------------------------

#ifndef	__CPARTMANAGER_H__
#define	__CPARTMANAGER_H__

#include <Windows.h>
#include <list>
#include "DebugInfo.h"

template <class IndexType, class PartIndexType, class DataType>
class CPartManager {
	public :
		typedef	std::list<PartIndexType>	PARTINDEX_LIST;				// PartIndex의 ID
		typedef	PARTINDEX_LIST::iterator	PARTINDEX_LIST_ITERATOR;
		
	public :
		CPartManager();
		~CPartManager();

		//-------------------------------------------------------
		// Init/Release
		//-------------------------------------------------------
		void			Init(IndexType maxIndex, PartIndexType maxPart);
		void			Release();

		//-------------------------------------------------------
		// 그 index에 관련된 data가 생성되어 있는가?
		//-------------------------------------------------------
		bool			IsDataNULL(IndexType index)		{ return m_pPartIndex[index]==m_PartIndexNULL; }
		bool			IsDataNotNULL(IndexType index)	{ return m_pPartIndex[index]!=m_PartIndexNULL; }

		//-------------------------------------------------------
		// Get / Set		
		//-------------------------------------------------------
		void			SetData(IndexType index, const DataType& data);
		// return값은 bReplace이다. 즉, 원래 뭔가가 있었으면.. true
		IndexType		SetData(IndexType index, const DataType& data, DataType& oldData);
		bool			GetData(IndexType index, DataType& data);

		// 사용한 PartIndex 개수
		PartIndexType	GetUsed() const					{ return m_nPartUsed; }
		IndexType		GetMaxIndex() const				{ return m_nIndex; }
		PartIndexType	GetMaxPartIndex() const			{ return m_nPart; }

	protected :
		//-------------------------------------------------------
		// Least_Recently_Used PartIndex를 찾아낸다.
		//-------------------------------------------------------
		PartIndexType	GetLRU() const;
		DataType&		GetPartData(PartIndexType partIndex)	{ return m_pData[partIndex]; }


	protected :
		//-------------------------------------------------------
		// [Index] ( PartIndex )
		//-------------------------------------------------------
		IndexType			m_nIndex;			// 전체 index 개수		
		PartIndexType*		m_pPartIndex;		// Part 하나를 가리키는 index

		//-------------------------------------------------------
		// [PartIndex] ( Data : ParentIndex : LastTime )
		//-------------------------------------------------------
		PartIndexType		m_nPart;			// Part의 개수
		PartIndexType		m_nPartUsed;		// 이미 사용한 Part의 개수
		DataType*			m_pData;			// 실제 Data
		IndexType*			m_pParentIndex;		// 원래의 index
		DWORD*				m_pLastTime;		// 최종 사용 시간

		//-------------------------------------------------------
		// LRU list
		//-------------------------------------------------------
		PARTINDEX_LIST				m_listLRU;			// LRU list
		PARTINDEX_LIST_ITERATOR*	m_pPartIterator;	// iterator for LRU list
		
		//-------------------------------------------------------
		// 기타 필요한 값
		//-------------------------------------------------------
		IndexType			m_IndexNULL;		// index NULL 값
		PartIndexType		m_PartIndexNULL;	// part index NULL 값

		//-------------------------------------------------------
		// count
		//-------------------------------------------------------
		DWORD				m_Counter;
};


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
CPartManager<IndexType, PartIndexType, DataType>::CPartManager()
{
	m_nIndex = 0;
	m_pPartIndex = NULL;

	m_nPart = 0;
	m_nPartUsed = 0;
	m_pData = NULL;
	m_pParentIndex = NULL;
	m_pLastTime = NULL;

	// LRU
	m_pPartIterator = NULL;

	// Null값
	m_IndexNULL	= (IndexType)0xFFFFFFFF;
	m_PartIndexNULL = (PartIndexType)0xFFFFFFFF;

	// timer
	m_Counter = 0;
}

template <class IndexType, class PartIndexType, class DataType>
CPartManager<IndexType, PartIndexType, DataType>::~CPartManager()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init(maxIndex, maxPart)
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
void		
CPartManager<IndexType, PartIndexType, DataType>::Init(IndexType maxIndex, PartIndexType maxPart)
{
	Release();

	//------------------------------------------------------
	// 부분이 전체보다 클수는 없으므로...
	//------------------------------------------------------
	if (maxPart > maxIndex)
	{
		maxPart = maxIndex;
	}

	//------------------------------------------------------
	// PartIndexType을 m_nIndex개 생성한다.
	//------------------------------------------------------
	m_nIndex = maxIndex;
	m_pPartIndex = new PartIndexType [m_nIndex];

	for (int i=0; i<m_nIndex; i++)
	{
		m_pPartIndex[i] = m_PartIndexNULL;
	}

	//------------------------------------------------------
	// PartIndex관련 부분 생성
	//------------------------------------------------------
	m_nPart			= maxPart;
	m_nPartUsed		= 0;
	m_pData			= new DataType [m_nPart];
	m_pParentIndex	= new IndexType [m_nPart];	// 원래의 index
	m_pLastTime		= new DWORD [m_nPart];		// 최종 사용 시간
	
	//------------------------------------------------------
	// LRU list
	//------------------------------------------------------
	m_listLRU.clear();
	m_pPartIterator		= new PARTINDEX_LIST_ITERATOR [m_nPart];	// iterator

	for (i=0; i<m_nPart; i++)
	{
		m_pData[i] = NULL;
		m_pParentIndex[i] = m_IndexNULL;
		m_pLastTime[i] = 0;

		m_pPartIterator[i] = m_listLRU.end();
	}

	m_Counter = 0;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
void		
CPartManager<IndexType, PartIndexType, DataType>::Release()
{
	if (m_pPartIndex!=NULL)
	{
		delete [] m_pPartIndex;
		m_pPartIndex = NULL;
		m_nIndex = 0;
	}

	if (m_pData!=NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
		m_nPart = 0;
		m_nPartUsed = 0;
	}

	if (m_pParentIndex!=NULL)
	{
		delete [] m_pParentIndex;
		m_pParentIndex = NULL;
	}

	if (m_pLastTime!=NULL)
	{
		delete [] m_pLastTime;
		m_pLastTime = NULL;
	}
	
	if (m_pPartIterator!=NULL)
	{
		delete [] m_pPartIterator;
		m_pPartIterator = NULL;
	}

	m_listLRU.clear();
}

//----------------------------------------------------------------------
// Set Data
//----------------------------------------------------------------------
// index번째 data를 추가시킨다.
// 이미 사용중인게 있더라도 덮어버린다.
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
void
CPartManager<IndexType, PartIndexType, DataType>::SetData(IndexType index, const DataType& data)
{
	// 추가할 위치
	PartIndexType	newPartIndex = GetLRU();

	//------------------------------------------------------------
	// 원래 있던거 제거
	//------------------------------------------------------------		
	IndexType oldIndex = m_pParentIndex[newPartIndex];
	if (oldIndex != m_IndexNULL)
	{
		// 원래의 PartIndex와 관련있는 index를 null로 바꾼다.		
		m_pPartIndex[oldIndex] = m_PartIndexNULL;
		m_pParentIndex[newPartIndex] = m_IndexNULL;	
	}

	//------------------------------------------------------------
	// 새것 생성
	//------------------------------------------------------------
	// link 생성
	m_pPartIndex[index] = newPartIndex;
	m_pParentIndex[newPartIndex] = index;

	// data 설정
	m_pData[newPartIndex] = data;

	// 최종 사용 시간 : counter를 증가시킬 필요는 없다.
	m_pLastTime[newPartIndex] = m_Counter;

	//------------------------------------------------------------
	// LRU list
	//------------------------------------------------------------
	// front가 가장 최근에 사용된 것이다.
	// m_pPartIterator[]에는 그 partIndex의 iterator를 저장한다.
	m_listLRU.push_front( newPartIndex );
	m_pPartIterator[newPartIndex] = m_listLRU.begin();
	
	// 사용한 개수 증가
	if (m_nPartUsed < m_nPart) 
		m_nPartUsed++;
}

//----------------------------------------------------------------------
// Set Data
//----------------------------------------------------------------------
// index번째 data를 추가시킨다.
// 이미 사용중인게 있으면 넘겨준다.
//
// return값은 제거되는 data의 index이다.
// m_IndexNULL이면 없는 것..
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
IndexType
CPartManager<IndexType, PartIndexType, DataType>::SetData(IndexType index, const DataType& data, DataType& oldData)
{
	// 추가할 위치
	PartIndexType	newPartIndex = GetLRU();

	//bool bReplace = false;
	//------------------------------------------------------------
	// 원래 있던거 제거
	//------------------------------------------------------------	
	IndexType oldIndex = m_pParentIndex[newPartIndex];
	if (oldIndex != m_IndexNULL)
	{
		// 원래의 PartIndex와 관련있는 index를 null로 바꾼다.		
		m_pPartIndex[oldIndex] = m_PartIndexNULL;
		m_pParentIndex[newPartIndex] = m_IndexNULL;

		// 원래 있던걸 넘겨준다.
		oldData = m_pData[newPartIndex];

		//bReplace = true;
	}

	//------------------------------------------------------------
	// 새것 생성
	//------------------------------------------------------------
	// link 생성
	m_pPartIndex[index] = newPartIndex;
	m_pParentIndex[newPartIndex] = index;

	// data 설정
	m_pData[newPartIndex] = data;

	// 최종 사용 시간 
	m_pLastTime[newPartIndex] = m_Counter++;

	//------------------------------------------------------------
	// LRU list
	//------------------------------------------------------------
	// front가 가장 최근에 사용된 것이다.
	// m_pPartIterator[]에는 그 partIndex의 iterator를 저장한다.
	if (m_pPartIterator[newPartIndex]!=m_listLRU.end())
	{
		m_listLRU.erase( m_pPartIterator[newPartIndex] );		// 제거
	}
	m_listLRU.push_front( newPartIndex );
	m_pPartIterator[newPartIndex] = m_listLRU.begin();
	
	//---------------------------------------------------
	// [ TEST CODE ]
	/*
	PARTINDEX_LIST_ITERATOR	iIndex = m_listLRU.begin();

	char str[1024] = "[SetData] ";
	char str2[256];
	while (iIndex != m_listLRU.end())
	{
		sprintf(str2, "%d ", *iIndex);
		strcat(str, str2);

		iIndex++;
	}
	DEBUG_ADD(str);
	*/
	//---------------------------------------------------
	
	// 사용한 개수 증가
	if (m_nPartUsed < m_nPart) 
		m_nPartUsed++;

	//return bReplace;

	return oldIndex;
}


//----------------------------------------------------------------------
// Get Data
//----------------------------------------------------------------------
// 외부에서 index번째 data를 얻는다.
// 만약 없으면? LRU를 제거한다.
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
bool
CPartManager<IndexType, PartIndexType, DataType>::GetData(IndexType index, DataType& data)
{
	PartIndexType partIndex = m_pPartIndex[index];

	//------------------------------------------------------
	// 사용중인 Data가 없을 경우 return해준다.
	//------------------------------------------------------
	if (partIndex==m_PartIndexNULL)
	{
		return false;
	}

	// 사용 시간 check
	m_pLastTime[partIndex] = m_Counter++;

	//------------------------------------------------------
	// LRU list에서 현재 partIndex를 
	// 가장 최근에 사용한 걸로 바꾼다.
	//------------------------------------------------------
	// iterator를 이용해서 순서를 바꾼다.
	if (m_pPartIterator[partIndex]!=m_listLRU.end())
	{
		m_listLRU.erase( m_pPartIterator[partIndex] );		// 제거
	}
	m_listLRU.push_front( partIndex );				// 추가
	m_pPartIterator[partIndex] = m_listLRU.begin();		// iterator 다시 설정

	//---------------------------------------------------
	// [ TEST CODE ]
	/*
	PARTINDEX_LIST_ITERATOR	iIndex = m_listLRU.begin();

	char str[1024] = "[GetData] ";
	char str2[256];
	while (iIndex != m_listLRU.end())
	{
		sprintf(str2, "%d ", *iIndex);
		strcat(str, str2);

		iIndex++;
	}
	DEBUG_ADD(str);
	*/
	//---------------------------------------------------


	//------------------------------------------------------
	// counter가 끝이면.. 정리~~
	//------------------------------------------------------
	if (m_Counter==0xFFFFFFFF)
	{
		// 제일 오래된 시간을 찾아서...
		// 그 시간을 0으로 만든다. --> 모두 그 시간만큼 뺀다.
		int leastTimeIndex = 0;
		int maxTimeIndex = 0;
		for (int i=1; i<m_nPart; i++)
		{
			// 가장 적은 시간
			if (m_pLastTime[i] < m_pLastTime[leastTimeIndex])
			{
				leastTimeIndex = i;
			}

			// 가장 큰 시간
			if (m_pLastTime[i] > m_pLastTime[maxTimeIndex])
			{
				maxTimeIndex = i;
			}
		}

		// 모두 가장 작은 시간만큼 뺀다.
		int leastTime = m_pLastTime[leastTimeIndex];
		for (i=0; i<m_nPart; i++)
		{
			m_pLastTime[i] -= m_pLastTime[leastTimeIndex];
		}

		// Counter는 현재꺼 중에서 가장 큰 시간의 것이다.
		m_Counter = m_pLastTime[maxTimeIndex];
	}

	// 넘겨줄 data설정
	data = m_pData[partIndex];

	return true;
}

//----------------------------------------------------------------------
// Get LRU
//----------------------------------------------------------------------
// 가장 오래전에 사용된 것의 PartIndex를 return한다.
//----------------------------------------------------------------------
template <class IndexType, class PartIndexType, class DataType>
PartIndexType
CPartManager<IndexType, PartIndexType, DataType>::GetLRU() const
{
	//---------------------------------------------------
	// m_nPart개 중에서 아직 꽉 찬 상태가 아니면...
	// 사용중인 마지막 PartIndex를 넘겨준다.
	//---------------------------------------------------
	if (m_nPartUsed < m_nPart)
	{	
		//DEBUG_ADD_FORMAT("[LRU] %d", m_nPartUsed);
		return m_nPartUsed;		
	}

	//---------------------------------------------------
	// LRU list의 tail이 LRU index이다.
	//---------------------------------------------------
	PartIndexType leastTimeIndex = *m_listLRU.rbegin();


	//---------------------------------------------------
	// [ TEST CODE ]
	/*
	PARTINDEX_LIST::const_iterator	iIndex = m_listLRU.begin();

	char str[1024] = "[GetLRU] ";
	char str2[256];
	while (iIndex != m_listLRU.end())
	{
		sprintf(str2, "%d ", *iIndex);
		strcat(str, str2);

		iIndex++;
	}
	DEBUG_ADD(str);
	*/
	//---------------------------------------------------

	//---------------------------------------------------
	// 전체를 검색해서 가장 오래전에 사용된 걸 찾는다.
	//---------------------------------------------------
	/*
	PartIndexType leastTimeIndex = 0;

	for (int i=1; i<m_nPart; i++)
	{
		// 가장 적은 시간
		if (m_pLastTime[i] < m_pLastTime[leastTimeIndex])
		{
			leastTimeIndex = i;
		}
	}
	*/

	//---------------------------------------------------
	// 가장 오래전에 사용된 것의 PartIndex
	//---------------------------------------------------
	//DEBUG_ADD_FORMAT("[LRU] %d", leastTimeIndex);
	return leastTimeIndex;
}


#endif
