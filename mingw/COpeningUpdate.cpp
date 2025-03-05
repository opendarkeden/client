//---------------------------------------------------------------------------
// COpeningUpdate.cpp
//---------------------------------------------------------------------------
// Opening 동영상을 보여줄때의 loop
//---------------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)
#include "Client.h"
#include "ServerInfo.h"
#include "COpeningUpdate.h"

#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

// Global
COpeningUpdate*		g_pCOpeningUpdate;


//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
COpeningUpdate::Init()
{
	// mouse event 처리
	g_pDXInput->SetMouseEventReceiver( NULL );

	// keyboard event 처리
	g_pDXInput->SetKeyboardEventReceiver( NULL );
}

//---------------------------------------------------------------------------
// Opening AVI
//---------------------------------------------------------------------------
void
COpeningUpdate::PlayMPG(char* filename)
{
	// AVI
	if (g_pAvi != NULL)
	{
		//#ifdef DEBUG_NEW
		//	operator delete ( g_pAvi, __FILE__, __LINE__ );
		//#else
			delete g_pAvi;
		//#endif
	}

	g_pAvi = new CAVI;
	if(!g_pAvi->OpenMPG(g_hWnd, filename, 640, 480)) 
	{
		char str[256];
		sprintf(str, "Not Found %s", filename);
		MessageBox(g_hWnd, str, "Error!", MB_OK);
		return;
	}
	g_pAvi->Play();
}


//-----------------------------------------------------------------------------
// UpdateOpening
//-----------------------------------------------------------------------------
// 접속 전..
//-----------------------------------------------------------------------------
void		
COpeningUpdate::Update()
{
	//------------------------------------------
	// Input
	//------------------------------------------	
	extern bool	g_bTestMode;
	if (g_bActiveGame
#ifdef OUTPUT_DEBUG
		|| g_bTestMode
#endif
		)
	{
		UpdateInput();
		UpdateMouse();

		ProcessInput();
	}

	if (!g_bActiveApp)
		return;

	//------------------------------------------
	// Draw
	//------------------------------------------
	//if (g_bActiveGame)
	//	UpdateLoginDraw();	
}


//---------------------------------------------------------------------------
// Update Opening Input
//---------------------------------------------------------------------------
void
COpeningUpdate::ProcessInput()
{
}
