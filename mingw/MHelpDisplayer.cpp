//---------------------------------------------------------------------------
// MHelpDisplayer.cpp
//---------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MHelpDisplayer.h"
#include "MHelpStringTable.h"
#include "CMessageArray.h"

//---------------------------------------------------------------------------
// Global
//---------------------------------------------------------------------------
MHelpDisplayer*		g_pHelpDisplayer = NULL;

extern DWORD			g_CurrentTime;
extern CMessageArray*	g_pHelpMessage;

//---------------------------------------------------------------------------
// Define
//---------------------------------------------------------------------------
// 메세지 유지 시간
#define	DELAY_HELP_OUTPUT_LAST		10000		// 10초 = 20*1000 

//---------------------------------------------------------------------------
//
// constructor / destructor
//
//---------------------------------------------------------------------------
MHelpDisplayer::MHelpDisplayer()
{
	m_DelayTime = 0;
}

MHelpDisplayer::~MHelpDisplayer()
{
}

//---------------------------------------------------------------------------
//
// member functions
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// OutputHelp (HELP_OUTPUT)
//---------------------------------------------------------------------------
// HELP_OUTPUT 하나를 출력하는데..
// 출력이 안될 수도 있다. - -;
// 동시에 너무 많은 message들이 출력되면 정신없기 때문에..
// 시간대가 겹치면 message들을 무시해버린다.
//---------------------------------------------------------------------------
void	
MHelpDisplayer::OutputHelp(HELP_OUTPUT ho)
{
	if (ho < g_pHelpStringTable->GetSize())
	{
		if (g_CurrentTime > m_DelayTime)
		{
			if (g_pHelpStringTable->IsDisplayed(ho))
			{			
				// 이미 출력했던 거면.. 무시한당. - -;
			}
			else
			{
				MString& helpString = (*g_pHelpStringTable)[ho];

				g_pHelpMessage->Add( helpString.GetString() );

				m_DelayTime = g_CurrentTime + DELAY_HELP_OUTPUT_LAST;
			}					
		}	
		else
		{
			// 이전에 출력한거랑 시간이 겹쳐서 무시하는 경우 -_-;
		}
	}
}

