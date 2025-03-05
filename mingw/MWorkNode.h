//----------------------------------------------------------------------
// MWorkNode.h
//----------------------------------------------------------------------
// 일.. - -;
//
// MWorkThread에서 처리할려는 한 종류의 일이다.
// 종류에 따라서 상속받아서 만들면 된다...
// 
// [Execute]
//   실제로 일을 처리하는 부분인데..
//   Stop()이 호출되었을 경우에 멈출 수 있도록 만들어야 한다.
//   즉, '현재 일'에서 어떤 특정한 단위만큼마다 
//   stop이 되었는지 check해야 한다.
//
//   Stop된 경우.. 
//	 현재 처리 중이던 일에서 남은 일을 생성해서 넘겨준다.
//   MWorkNode를 생성(new)해서 넘겨준다.
//
//   return값이 TRUE이면 일이 모두 끝났다는 것이고,
//              FALSE이면 문제가 있어서 수행이 안됐거나 Stop됐다는 의미이다.
//   Stop됐을때는 pRemainNode를 생성해서 넘겨줘야 한다.
//
//----------------------------------------------------------------------

#ifndef __MWORKNODE_H__
#define	__MWORKNODE_H__

#include <Windows.h>

//----------------------------------------------------------------------
// WorkNode
//----------------------------------------------------------------------
class MWorkNode {
	public :
		MWorkNode();		
		virtual ~MWorkNode();

		//--------------------------------------------------------
		// Type - 특정한 type을 구분해내기 위해서..
		//--------------------------------------------------------
		void				SetType(int type)	{ m_Type = type; }
		BOOL				IsTypeOf(int type)	{ return m_Type==type; }
		int					GetType() const		{ return m_Type; }

		//--------------------------------------------------------
		// Stop
		//--------------------------------------------------------
		BOOL				IsExecute() const	{ return m_bExecute; }
		BOOL				IsStop() const		{ return !m_bExecute; }
		void				Stop()				{ m_bExecute = FALSE; }

		//--------------------------------------------------------
		// Execute
		//--------------------------------------------------------
		virtual BOOL		Execute(MWorkNode*& pNode) = 0;

	protected :
		// 종류
		int					m_Type;

		// 현재 실행 중인가??
		BOOL				m_bExecute;

		// 작업이 중지 됐다는 Event
		HANDLE				m_hStopEvent;
};

#endif


