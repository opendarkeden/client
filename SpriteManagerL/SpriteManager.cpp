// SpriteManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "SpriteManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerApp

BEGIN_MESSAGE_MAP(CSpriteManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CSpriteManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerApp construction

CSpriteManagerApp::CSpriteManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpriteManagerApp object

CSpriteManagerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerApp initialization

BOOL CSpriteManagerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSpriteManagerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Load Accelerator
	HINSTANCE hInstance = AfxGetInstanceHandle();
	HACCEL hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDR_ACCELERATOR1));
	

/*
	// Set priority high
	SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



/*
	MSG msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE) == FALSE)
	{
		///////////////////////////////////////////////
		// Calculate real fps
		///////////////////////////////////////////////
		static DWORD dwUpdates = 0;
		static float m_fRealFPS = 0;

		dwUpdates++;
		static DWORD oldTickCount = 0;
		DWORD newTickCount = GetTickCount();
		if( newTickCount - oldTickCount > 1000)
		{
			m_fRealFPS = ((float)dwUpdates / (float) (newTickCount - oldTickCount))*1000.0f;

			dwUpdates = 0;
			oldTickCount = newTickCount;
		}

		char msg[256];
		sprintf( msg, "%5.2f\n", m_fRealFPS);
		OutputDebugString( msg);


		((CSpriteManagerDlg*)AfxGetMainWnd())->m_aniDlg.CheckTime();
	}

*/
