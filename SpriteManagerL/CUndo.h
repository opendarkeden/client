/********************************************************************
*	 CUndo template class											*
*																	*
*	!! class TYPE은 '=' operator가 정의되어 있어야 한다.			*
*																	*
*																	*
********************************************************************/

///////////////////////////////////////////////////
//	원형 Queue class
template<class TYPE, int arraylength> class CQueue
{
//--------------------------------------------------------
//
// Member Variables
//
private:
	TYPE				m_data[arraylength];
	int					m_iHead, m_iTail;	// Head => m_data의 첫번째 data의 index,
											// Tail => m_data의 마지막 data의 index+1
	bool				m_bEmpty;
//--------------------------------------------------------
//
// Member Functions
//
public:
	CQueue()
	{
		m_iHead = 0;
		m_iTail = 0;
		m_bEmpty = true;
	}

	int					GetSize() { return arraylength;}

	int					IsEmpty() { return GetItemCount() == 0;}

	int					IsFull() { return GetItemCount() == arraylength;}

	// 실제 data가 몇개인가.
	int					GetItemCount()	
	{
		if( m_bEmpty)
			return 0;

		int iDataCount = m_iTail - m_iHead;

		if( iDataCount <= 0)
			iDataCount += arraylength;

		return iDataCount;
	}

	void 				Push( const TYPE& item)
	{
		// 가득이면 밀어내기.
		if( IsFull())
		{
			m_iHead = (m_iHead+1)%arraylength;
		}

		m_iTail = (m_iTail+1)%arraylength;
		m_data[ m_iTail] = item;

		m_bEmpty = false;
	}
	bool				Pop( TYPE& item)
	{
		if( IsEmpty())
			return false;

		item = m_data[ m_iHead];
		m_iHead = (m_iHead+1)%arraylength;

		// pop시에만 empty 상태가 될수 있다.
		if( m_iTail == m_iHead)
			m_bEmpty = true;

		return true;
	}

	//////////////////////////////////////////////////
	// tail에서 i번째 깊이의 data를 얻는다.
	bool				GetByDepth( int i, TYPE& item)
	{
		if( IsEmpty() || i >= arraylength || i < 0) return false;

		int arrayIndex = (m_iTail-i)%arraylength;	// array에서의 index
		if( arrayIndex < 0) 
			arrayIndex += arraylength;

		item = m_data[ arrayIndex];
		return true;
/*
		// 정방향
		if( m_iHead < m_iTail)
		{
			if( arrayIndex < m_iHead) return false;
			item = m_data[ arrayIndex];
			return true;
		}
		// 역방향
		else //if( m_iHead > m_iTail)
		{
			if( arrayIndex > m_iHead) return false;
			item = m_data[ arrayIndex];
			return true;
		}
		*/
	}
};





/////////////////////////////////////////////////////////////
//
//	CUndo class
//
template<class TYPE, int undodepth> class CUndo
{

//------------------------------------------------------------
//
// Member Variables
//
private:
	CQueue<TYPE, undodepth+1>	m_dataQueue;	// +1은 현재상태를 저장하기 위한 공간을 확보하기 위함
	int							m_iCurrentDepth;	// 현재의 undo 깊이상황.
													// -1 인 경우, undo에 들어가지 않은 상태.
typedef void (CALLBACK* CUNDOCALLBACK)(void);
	CUNDOCALLBACK				m_ChangeCallback;					// 상태변화가 생길시 자동 호출될 함수포인터



//------------------------------------------------------------
//
// Member Functions
//
public:
	CUndo()
	{
		m_iCurrentDepth = -1;
		m_ChangeCallback = NULL;
	}

	~CUndo()
	{
	}

	void Clear() { m_iCurrentDepth = -1;}	// Undo불가능하게 만든다.

	void SetCallbackFunction( CUNDOCALLBACK ft) { m_ChangeCallback = ft;}

	// 상태 변화가 생길때마다 호출되야 하는 함수.
	void SaveState( const TYPE& state)
	{
		m_iCurrentDepth = -1;
		m_dataQueue.Push( state);
	}

	//--------------------------------------------------------------------------------
	// !! 처음 undo를 시도할 경우, 현재 상태를 parameter로 넘겨줘야 한다.
	//	  왜냐하면 UNDO 행위 자체가 상태 변화를 일으키기 때문이다.
	//	  이는 외부에서 처리할 수도 있지만( 외부에서 UNDO call이전에 SaveState를 호출)
	//	  코드 중복이 발생할 수 있으므로 ...
	//	return : return false when failed
	//--------------------------------------------------------------------------------
	bool Undo( TYPE& state)
	{
		if( CanUndo() == false)
			return false;

		// 처음 undo를 시도한 경우, 복귀를 위해 현재 상태를 저장.
		if( m_iCurrentDepth == -1)
		{
			SaveState( state);
			m_iCurrentDepth++;
		}

		bool ret = m_dataQueue.GetByDepth( ++m_iCurrentDepth, state);
		if( ret && m_ChangeCallback) 
			(*m_ChangeCallback)();

		return ret;
	}

	bool Redo( TYPE& state)
	{
		if( CanRedo() == false)
			return false;

		bool ret = m_dataQueue.GetByDepth( --m_iCurrentDepth, state);
		if( ret && m_ChangeCallback)
			(*m_ChangeCallback)();

		return ret;
	}

	bool IsFull()
	{
		return m_dataQueue.IsFull();
	}

	bool CanUndo()
	{
		return m_dataQueue.GetItemCount() > (m_iCurrentDepth+1);
	}

	bool CanRedo()
	{
		return m_iCurrentDepth > 0;
	}
};