
#ifndef	__MSKIPEFFECT_H__
#define	__MSKIPEFFECT_H__

#include "MEffect.h"

class MSkipEffect : public MEffect {
	public :
		MSkipEffect(BYTE bltType);
		~MSkipEffect();
		
		void SetSkipValue(int n) { m_nSkipValue = n; }
		int  GetSkipValue() { return m_nSkipValue; }
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool	Update();
		
	protected:
		int m_nSkipValue;
		
};

#endif

