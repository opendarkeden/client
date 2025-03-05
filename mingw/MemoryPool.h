/********************************************************************
created:	2003/12/05
created:	5:12:2003   13:20
filename: 	MemoryPool.h
file ext:	h
author:		sonee

purpose:	memory pool
			고정된 크기를 빈번하게 new/delete 하는 경우 메모리 풀을 사용하면
			메모리 단편화를 줄일 수 있다.

			메모리 leak 현상을 막을 수 있다.

			Debug 모드인 경우에는 메모리가 MEMORY_POOL_GARBAGE 값으로
			채워진다.
*********************************************************************/

#pragma		once

class MemoryPool
{
public :
	MemoryPool( int BlockSize, int BlockCount );
	~MemoryPool();

	void*					Alloc();
	void					Free( void *pMem );

	bool					IsPtrInPool( void *pMem );				// MemoryPool에 해당 Ptr 이 있는가.
	bool					IsAvailablePtr( void *pMem );			// 해당 포인터의 유효성

private :
	class CBlock
	{
	public :
		CBlock				*m_pPrev;
		int					m_leftBlocks;
		unsigned char		*m_pNextBlock;
	};

	class CFreeBlock												// Free 된 값은 단지 포인터만 필요하므로.
	{
	public :
		CFreeBlock			*m_pPrev;
	};

	CBlock					*m_pCurrentBlock;
	CFreeBlock				*m_pFreeBlockList;

	int						m_BlockSize;
	int						m_BlockCount;

};

extern MemoryPool g_CreatureMemoryPool;
extern MemoryPool g_CreatureWearMemoryPool;
extern MemoryPool g_NPCCreatureMemoryPool;
extern MemoryPool g_FakeCreatureMemoryPool;