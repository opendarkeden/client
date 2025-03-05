//------------------------------------------------------------------------
// CToken.h
//------------------------------------------------------------------------
//													Made by 윤영식(9684050)
//													Coding : 1998.11.
//													Modify : 1998.11.20
//------------------------------------------------------------------------
#ifndef	__CTOKEN_H__
#define	__CTOKEN_H__

#include <string.h>


class CToken {
	public :
		// constructor/destructor
		CToken(const char* str=NULL);
		~CToken();

		// set string
		void SetString(const char* str);

		// 공백이 여러개 있을때 무시한다.
		void SkipSpace();


		// delimiter에 의해서 현재의 token string을 구한다.
		const char* GetToken(const char* delimiter=" ");

		// 현재 위치부터 끝까지의 string을 넘겨준다.
		const char* GetEnd();



	//------------------------------------
	// private member functions
	private :

		// 메모리 해제
		void Release();



	//------------------------------------
	// data members
	protected :

		// token으로 나눌려는 string
		char* m_pString;

		// m_pString에서의 현재의 위치
		char* m_pCurrent;
};

#endif


