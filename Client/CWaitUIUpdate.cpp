//---------------------------------------------------------------------------
// CWaitUIUpdate.cpp
//---------------------------------------------------------------------------
// UI에서 메시지가 오기를 기다리는 loop
//---------------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <MMSystem.h>
#else
#include "../../basic/Platform.h"
#endif
#include <string>
#include "Client.h"
#include "GameObject.h"
#include "ServerInfo.h"
#include "PacketDef.h"
#include "VS_UI.h"
#include "ServerInformation.h"
#include "packet/cpackets/CLGetServerList.h"
#include "packet/cpackets/CLChangeServer.h"
#include "CWaitUIUpdate.h"
#include "MTestDef.h"

extern bool	LoadingAddonSPK(bool bLoadingAll);
extern bool	g_AddonSPKAllLoaded;

extern void	DrawTitleLoading();
extern void	EndTitleLoading(bool SendLogin = false);
extern bool 	CheckInvalidProcess();
extern BOOL g_MyFull;
extern RECT g_GameRect;
extern int	g_TitleSpriteAlpha;

// Global
CWaitUIUpdate*	g_pCWaitUIUpdate = NULL;


extern DWORD g_double_click_time;

#if defined(OUTPUT_DEBUG) //&& defined(_DEBUG)
	#ifdef __METROTECH_TEST__
//		#define	OUTPUT_DEBUG_UPDATE_LOOP
	#endif
	//#define OUTPUT_DEBUG_PROCESS_INPUT
#endif

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
CWaitUIUpdate::Init()
{
	// mouse event 처리
	g_pDXInput->SetMouseEventReceiver( DXMouseEvent );

	// keyboard event 처리
	g_pDXInput->SetKeyboardEventReceiver( DXKeyboardEvent );
}

//-----------------------------------------------------------------------------
// DXKeyboardEvent
//-----------------------------------------------------------------------------
void	
CWaitUIUpdate::DXKeyboardEvent(CDirectInput::E_KEYBOARD_EVENT event, DWORD key)
{
		if (event==CDirectInput::KEYDOWN)
		{			
			gC_vs_ui.DIKeyboardControl(event, key);
	
			#ifdef OUTPUT_DEBUG

			switch (key)
			{
				//------------------------------------
				// FPS Toggle
				//------------------------------------
				case DIK_F : 
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						(*g_pUserOption).DrawFPS = !(*g_pUserOption).DrawFPS;
					}
				break;

				//-----------------------------------------------------
				// Sound Test
				//-----------------------------------------------------
				case DIK_F4 :
				{
					PlaySound( SOUND_VAMPIRE_POISON_DB );
				}
				break;

				//-----------------------------------------------------
				// Server List 받기
				//-----------------------------------------------------
				case DIK_SPACE :
				{
				}
				break;

				//-----------------------------------------------------
				// Server 선택하기
				//-----------------------------------------------------
				case DIK_C :
				{
				}
				break;			

			}		
	
			#endif
		}
}

//-----------------------------------------------------------------------------
// DXMouseEvent
//-----------------------------------------------------------------------------
void		
CWaitUIUpdate::DXMouseEvent(CDirectInput::E_MOUSE_EVENT event, int x, int y, int z)
{
	static DWORD	last_click_time;
	static int		double_click_x, double_click_y;

	switch (event)
	{
		case CDirectInput::LEFTDOWN:
			//  double-click interval?
			if ((DWORD)labs((long)(GetTickCount() - last_click_time)) <= g_double_click_time)
			{
				if (g_x>= double_click_x-1 && g_x <= double_click_x+1 &&
					 g_y>= double_click_y-1 && g_y <= double_click_y+1)
				{
					#ifdef OUTPUT_DEBUG_UPDATE_LOOP
						DEBUG_ADD("MLD");
					#endif
					
						gC_vs_ui.MouseControl(M_LB_DOUBLECLICK, g_x, g_y);

					#ifdef OUTPUT_DEBUG_UPDATE_LOOP
						DEBUG_ADD("MLD2");
					#endif

					last_click_time = 0;
					return;
				}
			}				

			//gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, g_x, g_y);
			last_click_time = GetTickCount();
			double_click_x = g_x;
			double_click_y = g_y;
		break;

		case CDirectInput::WHEELDOWN:
			gC_vs_ui.MouseControl(M_WHEEL_DOWN, x, y);
			break;

		case CDirectInput::WHEELUP:
			gC_vs_ui.MouseControl(M_WHEEL_UP, x, y);
			break;
	}
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
// 접속 전..
//-----------------------------------------------------------------------------
void		
CWaitUIUpdate::Update()
{
	static DWORD lastTime = g_CurrentTime;
	bool bChanged = false;

	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		DEBUG_ADD("UM");
	#endif

	UpdateMouse();

	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		DEBUG_ADD("UM2");
	#endif

	CheckInvalidProcess();
	//------------------------------------------
	// 일정시간마다 한번씩 update
	//------------------------------------------
	if (g_CurrentTime - lastTime >= g_UpdateDelay)
	{
		//------------------------------------------
		// Sound Stream
		//------------------------------------------
//		if (g_pDXSoundStream!=NULL)
//		{
//			g_pDXSoundStream->Update();
//		}

		//------------------------------------------
		// Socket 입력 처리
		//------------------------------------------
		if (!UpdateSocketInput())
		{
			return;
		}
		
		//------------------------------------------
		// Input
		//------------------------------------------	
		// edit by coffee 錦攣槨눗왯뺏冷攣끽鞫刻
		extern bool	g_bTestMode;
		if (g_bActiveGame
#ifdef OUTPUT_DEBUG
			|| g_bTestMode
#endif
			)
		{
			UpdateInput();
			ProcessInput();			
		}
		
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("UP");
		#endif

		gC_vs_ui.Process();
	
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("UP2");
		#endif

		if (!g_bActiveApp)
			return;

		//------------------------------------------
		// Socket Output부분 처리
		//------------------------------------------	
		if (!UpdateSocketOutput())
		{
			return;	
		}		

		//------------------------------------------	
		// [ TEST CODE]
		//------------------------------------------	
		// UI 기다리는 모드가 아닐 경우...
		//------------------------------------------	
		if (g_Mode!=MODE_MAINMENU &&
			g_Mode!=MODE_NEWUSER &&
			g_Mode!=MODE_WAIT_SELECTPC &&
			g_Mode!=MODE_WAIT_SELECT_WORLD &&
			g_Mode!=MODE_WAIT_SELECT_SERVER)
		{
			if (g_Mode==MODE_CHANGE_OPTION)
			{
				SetMode( MODE_CHANGE_OPTION );
			}

			return;
		}

		//------------------------------------------
		// Draw
		//------------------------------------------
// 		if (g_bActiveGame
// #ifdef OUTPUT_DEBUG
// 			|| g_bTestMode
// #endif
// 			)
// 		{
		
		{
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				DEBUG_ADD("D");
			#endif

			UpdateDraw();	

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				DEBUG_ADD("D2");
			#endif
		}

		lastTime = g_CurrentTime;

		// Frame증가
		g_FrameCount++;
	}

	//------------------------------------------------------------
	// Test 2001.8.20
	//------------------------------------------------------------
	// 실행 중에 loading하는 걸로 바꿔서 뺀다..
	/*
	bool bLoad = false;
	if (g_Mode==MODE_MAINMENU)
	{		
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo1");
		#endif

		// main menu에서
		// 남자 뱀파이어 로딩
		if (!(*g_pCreatureSpriteTable)[2].bLoad)
		{
			LoadCreature( 2 );
			bLoad = true;
		}
		
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo2");
		#endif
	}
	else if (g_Mode==MODE_WAIT_SELECTPC)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo3");
		#endif

		// main menu에서
		// 여자 뱀파이어 로딩
		if (!(*g_pCreatureSpriteTable)[3].bLoad)
		{
			LoadCreature( 3 );	
			bLoad = true;
		}

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo4");
		#endif
	}

	//-------------------------------------------------------------
	// 틈틈히(-_-;) 슬레이어 그림 loading하기
	//-------------------------------------------------------------
	static DWORD lastLoadingTime = 0;
		
	if (!bLoad && !g_AddonSPKAllLoaded
		// 5초에 한번씩 로딩한다.
		&& g_CurrentTime - lastLoadingTime > 5000)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo5");
		#endif

		// 슬레이어 그림 일부 로딩 - 2001.8.20
		LoadingAddonSPK( false );

		lastLoadingTime = timeGetTime();

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("Lo6");
		#endif
	}
	*/

	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		DEBUG_ADD("UOK");
	#endif
}

//---------------------------------------------------------------------------
// Process Input
//---------------------------------------------------------------------------
void
CWaitUIUpdate::ProcessInput()
{
	//-----------------------------------------------
	// Quit
	//-----------------------------------------------
	/*
	if (g_pDXInput->KeyDown(DIK_ESCAPE))
	{	
		SetMode( MODE_QUIT );

		return;
	}
	*/
  
	//---------------------------------------------------	
	// UI Input
	//---------------------------------------------------	
	gC_vs_ui.MouseControl(M_MOVING, g_x, g_y);
	if (g_pDXInput->m_lb_down)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("LD");
		#endif

		gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, g_x, g_y);

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("LD1");
		#endif
	}

	if (g_pDXInput->m_lb_up)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("LU");
		#endif

		gC_vs_ui.MouseControl(M_LEFTBUTTON_UP, g_x, g_y);

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("LU1");
		#endif
	}
	if (g_pDXInput->m_rb_down)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("RD");
		#endif

		if (gC_vs_ui.MouseControl(M_RIGHTBUTTON_DOWN, g_x, g_y))
		{
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				DEBUG_ADD("RD1");
			#endif

			return;
		}

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("RD2");
		#endif
	}
	if (g_pDXInput->m_rb_up)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("RU");
		#endif

		if (gC_vs_ui.MouseControl(M_RIGHTBUTTON_UP, g_x, g_y))
		{
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				DEBUG_ADD("RU1");
			#endif

			return;
		}

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			DEBUG_ADD("RU2");
		#endif
	}
	if (g_pDXInput->m_cb_down)
	{
		if (gC_vs_ui.MouseControl(M_CENTERBUTTON_DOWN, g_x, g_y))
			return;
	}
	if (g_pDXInput->m_rb_up)
	{
		if (gC_vs_ui.MouseControl(M_CENTERBUTTON_UP, g_x, g_y))
			return;
	}
}



//---------------------------------------------------------------------------
// Update Draw
//---------------------------------------------------------------------------
void 
CWaitUIUpdate::UpdateDraw()
{
	//char	str[128];
	UpdateMouse();

	gC_vs_ui.MouseControl(M_MOVING, g_x, g_y);

//	if (CDirect3D::IsHAL())
//	{
//		//HRESULT hr;
//
//		//hr = CDirect3D::GetDevice()->BeginScene();
//
//		//if (hr!=D3D_OK)
//		{
//			/*
//			int a = 1000;
//			switch (hr)
//			{
//				case D3DERR_BADMAJORVERSION : a =0; break;
//				case D3DERR_BADMINORVERSION  : a =1; break;
//				case D3DERR_COLORKEYATTACHED  : a =2; break;
//				case D3DERR_CONFLICTINGTEXTUREFILTER  : a =3; break;
//				case D3DERR_CONFLICTINGTEXTUREPALETTE  : a =4; break;
//				case D3DERR_CONFLICTINGRENDERSTATE  : a =5; break;
//				case D3DERR_DEVICEAGGREGATED  : a =6; break;
//				case D3DERR_EXECUTE_CLIPPED_FAILED  : a =7; break;
//				case D3DERR_EXECUTE_CREATE_FAILED  : a =8; break;
//				case D3DERR_EXECUTE_DESTROY_FAILED  : a =9; break;
//				case D3DERR_EXECUTE_FAILED  : a =10; break;
//				case D3DERR_EXECUTE_LOCK_FAILED  : a =11; break;
//				case D3DERR_EXECUTE_LOCKED  : a =12; break;
//				case D3DERR_EXECUTE_NOT_LOCKED  : a =13; break;
//				case D3DERR_EXECUTE_UNLOCK_FAILED  : a =14; break;
//				case D3DERR_INBEGIN  : a =15; break;
//				case D3DERR_INBEGINSTATEBLOCK  : a =16; break;
//				case D3DERR_INITFAILED  : a =17; break;
//				case D3DERR_INVALID_DEVICE  : a =18; break;
//				case D3DERR_INVALIDCURRENTVIEWPORT  : a =19; break;
//				case D3DERR_INVALIDMATRIX  : a =20; break;
//				case D3DERR_INVALIDPALETTE  : a =21; break;
//				case D3DERR_INVALIDPRIMITIVETYPE  : a =22; break;
//				case D3DERR_INVALIDRAMPTEXTURE  : a =23; break;
//				case D3DERR_INVALIDSTATEBLOCK  : a =30; break;
//				case D3DERR_INVALIDVERTEXFORMAT  : a =40; break;
//				case D3DERR_INVALIDVERTEXTYPE  : a =50; break;
//				case D3DERR_LIGHT_SET_FAILED  : a =60; break;
//				case D3DERR_LIGHTHASVIEWPORT  : a =70; break;
//				case D3DERR_LIGHTNOTINTHISVIEWPORT  : a =80; break;
//				case D3DERR_MATERIAL_CREATE_FAILED  : a =90; break;
//				case D3DERR_MATERIAL_DESTROY_FAILED  : a =100; break;
//				case D3DERR_MATERIAL_GETDATA_FAILED  : a =110; break;
//				case D3DERR_MATERIAL_SETDATA_FAILED  : a =120; break;
//				case D3DERR_MATRIX_CREATE_FAILED  : a =130; break;
//				case D3DERR_MATRIX_DESTROY_FAILED  : a =140; break;
//				case D3DERR_MATRIX_GETDATA_FAILED  : a =150; break;
//				case D3DERR_MATRIX_SETDATA_FAILED  : a =160; break;
//				case D3DERR_NOCURRENTVIEWPORT  : a =170; break;
//				case D3DERR_NOTINBEGIN  : a =180; break;
//				case D3DERR_NOTINBEGINSTATEBLOCK  : a =190; break;
//				case D3DERR_NOVIEWPORTS  : a =200; break;
//				case D3DERR_SCENE_BEGIN_FAILED  : a =220; break;
//				case D3DERR_SCENE_END_FAILED  : a =230; break;
//				case D3DERR_SCENE_IN_SCENE  : a =240; break;
//				case D3DERR_SCENE_NOT_IN_SCENE  : a =250; break;
//				case D3DERR_SETVIEWPORTDATA_FAILED  : a =0; break;
//				case D3DERR_STENCILBUFFER_NOTPRESENT  : a =0; break;
//				case D3DERR_SURFACENOTINVIDMEM  : a =0; break;
//				case D3DERR_TEXTURE_BADSIZE  : a =0; break;
//				case D3DERR_TEXTURE_CREATE_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_DESTROY_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_GETSURF_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_LOAD_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_LOCK_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_LOCKED  : a =0; break;
//				case D3DERR_TEXTURE_NO_SUPPORT  : a =0; break;
//				case D3DERR_TEXTURE_NOT_LOCKED  : a =0; break;
//				case D3DERR_TEXTURE_SWAP_FAILED  : a =0; break;
//				case D3DERR_TEXTURE_UNLOCK_FAILED  : a =0; break;
//				case D3DERR_TOOMANYOPERATIONS  : a =0; break;
//				case D3DERR_TOOMANYPRIMITIVES  : a =0; break;
//				case D3DERR_UNSUPPORTEDALPHAARG  : a =0; break;
//				case D3DERR_UNSUPPORTEDALPHAOPERATION  : a =0; break;
//				case D3DERR_UNSUPPORTEDCOLORARG  : a =0; break;
//				case D3DERR_UNSUPPORTEDCOLOROPERATION  : a =0; break;
//				case D3DERR_UNSUPPORTEDFACTORVALUE  : a =0; break;
//				case D3DERR_UNSUPPORTEDTEXTUREFILTER  : a =0; break;
//				case D3DERR_VBUF_CREATE_FAILED  : a =0; break;
//				case D3DERR_VERTEXBUFFERLOCKED  : a =0; break;
//				case D3DERR_VERTEXBUFFEROPTIMIZED  : a =0; break;
//				case D3DERR_VERTEXBUFFERUNLOCKFAILED  : a =0; break;
//				case D3DERR_VIEWPORTDATANOTSET  : a =0; break;
//				case D3DERR_VIEWPORTHASNODEVICE  : a =0; break;
//				case D3DERR_WRONGTEXTUREFORMAT  : a =0; break;
//				case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY  : a =0; break;
//				case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY  : a =0; break;
//				case D3DERR_ZBUFFER_NOTPRESENT  : a =0; break;
//				case DDERR_INVALIDOBJECT : a = 990; break;
//				case DDERR_INVALIDPARAMS  : a = 999; break;
//				case DDERR_NOTFOUND  : a = 997; break;
//				case DDERR_NOTINITIALIZED  : a = 9645; break;
//				case DDERR_OUTOFMEMORY  : a = 345; break;
//			}
//			*/
//			//CDirectDraw::RestoreAllSurfaces();
//			//CDirect3D::Restore();
//			//return;
//		}
//
//		gC_vs_ui.Show();
//
//		//-----------------------------------------------------------------
//		//-----------------------------------------------------------------
//		/*
//		if (g_Mode==MODE_MAINMENU)
//		{
//			static CAlphaSpritePack ASPK;
//			static int first = 1;
//
//			if (first)
//			{
//				class ifstream file("fog.aspk", ios::binary);
//				ASPK.LoadFromFile(file);
//				file.close();
//
//				first = 0;
//			}
//
//			POINT point = { 100, 0 };
//			POINT point2 = { 500, 0 };
//
//			static int f = 0;
//
//			g_pBack->BltAlphaSprite( &point, &ASPK[f] );
//			g_pBack->BltAlphaSprite( &point2, &ASPK[f] );
//
//			static DWORD lastTime = g_CurrentTime;
//
//			if (g_CurrentTime - lastTime > 400)
//			{
//				if (++f == 3)
//					f = 0;
//
//				lastTime = g_CurrentTime;
//			}
//		}
//		*/
//		//-----------------------------------------------------------------
//		// 서버 이름 출력
//		//-----------------------------------------------------------------
//		/*
//		#ifdef OUTPUT_DEBUG
//
//			if (g_Mode==MODE_WAIT_SELECTPC)
//			{
//				char str[80];
//
//				sprintf(str, "ServerGroup: %s", g_pServerInformation->GetServerGroupName());						
//
//				g_pBack->GDI_Text(101,11, str, RGB(0,0,0));
//				g_pBack->GDI_Text(100,10, str, RGB(240,240,240));
//
//				//sprintf(str, "Server: %s", g_pServerInformation->GetServerName());
//
//				//g_pBack->GDI_Text(401,11, str, RGB(0,0,0));
//				//g_pBack->GDI_Text(400,10, str, RGB(240,240,240));
//
//				ServerInformation::const_iterator iGroup = g_pServerInformation->begin();
//
//				int y = 30;
//				
//				while (iGroup!=g_pServerInformation->end())
//				{
//					ServerGroup* pGroup = iGroup->second;
//
//					sprintf(str, "[%d] %s", iGroup->first, pGroup->GetGroupName());
//
//					g_pBack->GDI_Text(101,y+1, str, RGB(0,0,0));
//					g_pBack->GDI_Text(100,y, str, RGB(240,240,240));
//
//					y += 20;
//
//					//ServerGroup::const_iterator iServer = pGroup->begin();
//
//					//while (iServer!=pGroup->end())
//					{
//					//	SERVER_INFO* pServer = iServer->second;
//
//					//	sprintf(str, "[%d] %s", iServer->first, pServer->ServerName.GetString());						
//
//					//	g_pBack->GDI_Text(401,y+1, str, RGB(0,0,0));
//					//	g_pBack->GDI_Text(400,y, str, RGB(240,240,240));
//
//					//	y += 20;
//
//					//	iServer++;
//					}
//					
//
//					iGroup++;
//				}
//			}
//		#endif
//		*/
//
//		//-----------------------------------------------------------------
//		// Mouse 그리기
//		//-----------------------------------------------------------------
//		gC_vs_ui.DrawMousePointer();
//
//		#ifdef OUTPUT_DEBUG
//			if (g_pUserOption->DrawFPS)
//			{
//				char str[256];
//				
//				sprintf(str, "%d FPS(HAL)", g_FrameRate);
//				
//				g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
//				g_pBack->GDI_Text(10,10, str, 0xFFFFFF);
//			}
//		#endif
//	
//		//CDirect3D::GetDevice()->EndScene();
//	}
//	else
	{
		POINT point;
		
		static DWORD oldTime = timeGetTime();
		gC_vs_ui.Show();

		if(g_TitleSpriteAlpha > 0)
		{
			g_TitleSpriteAlpha = std::max(0, (int)(31-(timeGetTime()-oldTime)*16/1000));
			DrawTitleLoading();

			if(g_TitleSpriteAlpha <= 0)
			{
				EndTitleLoading(true);
			}
		}
				
		//-----------------------------------------------------------------
		// 서버 이름 출력
		//-----------------------------------------------------------------
		/*
		#ifdef OUTPUT_DEBUG

			if (g_Mode==MODE_WAIT_SELECTPC)
			{
				char str[80];

				sprintf(str, "ServerGroup: %s", g_pServerInformation->GetServerGroupName());						

				g_pLast->GDI_Text(101,11, str, RGB(0,0,0));
				g_pLast->GDI_Text(100,10, str, RGB(240,240,240));

				//sprintf(str, "Server: %s", g_pServerInformation->GetServerName());

				//g_pLast->GDI_Text(401,11, str, RGB(0,0,0));
				//g_pLast->GDI_Text(400,10, str, RGB(240,240,240));

				ServerInformation::const_iterator iGroup = g_pServerInformation->begin();

				int y = 30;
				
				while (iGroup!=g_pServerInformation->end())
				{
					ServerGroup* pGroup = iGroup->second;

					sprintf(str, "[%d] %s", iGroup->first, pGroup->GetGroupName());

					g_pLast->GDI_Text(101,y+1, str, RGB(0,0,0));
					g_pLast->GDI_Text(100,y, str, RGB(240,240,240));

					y += 20;

					//ServerGroup::const_iterator iServer = pGroup->begin();

					//while (iServer!=pGroup->end())
					{
					//	SERVER_INFO* pServer = iServer->second;

					//	sprintf(str, "[%d] %s", iServer->first, pServer->ServerName.GetString());

					//	g_pLast->GDI_Text(401,y+1, str, RGB(0,0,0));
					//	g_pLast->GDI_Text(400,y, str, RGB(240,240,240));

					//	y += 20;

					//	iServer++;
					}
					

					iGroup++;
				}
			}
		#endif
		*/

		/*
		static float anim = 0.0f;

		static CSpriteSurface* pWaveSurface;
		static bool first = true;

		if (first)
		{
			pWaveSurface = new CSpriteSurface;
			pWaveSurface ->InitOffsurface(SURFACE_WIDTH, SURFACE_HEIGHT, DDSCAPS_SYSTEMMEMORY);
			pWaveSurface->SetTransparency( 0 );
			pWaveSurface->FillSurface( CDirectDraw::Color(0,0,0) );
			first = false;
		}

		g_pLast->Lock();

		WORD* lpSurface = (WORD*)g_pLast->GetSurfacePointer();
		long	pitch	= (long)g_pLast->GetSurfacePitch();

		pWaveSurface->Lock();		
		
		WORD* lpWaveSurface = (WORD*)pWaveSurface->GetSurfacePointer();
		long	wavePitch	= (long)pWaveSurface->GetSurfacePitch();

		WORD* lpSrcSurfaceTemp, * lpSrcSurfaceTemp2;
		WORD* lpDestSurfaceTemp, * lpDestSurfaceTemp2;

		const int xStep = 20;
		const int yStep = 20;

		anim += 0.05f;
		RECT rectWave = { 100, 100, 700, 500 };

        for (int x=rectWave.left; x<rectWave.right; x+=xStep)
		{
            for (int y=rectWave.top; y<rectWave.bottom; y+=yStep)
			{
                int xpos = x+(sin(anim+x*0.01f)*15);
                int ypos = y+(sin(anim+y*0.01f)*15);

				// (x,y)
				lpSrcSurfaceTemp = (WORD*)((BYTE*)lpSurface + y*pitch + (x<<1));

				// (xpos, ypos) 
				lpDestSurfaceTemp = (WORD*)((BYTE*)lpWaveSurface + ypos*wavePitch + (xpos<<1));
				
				for (int dy=0; dy<yStep; dy++)
				{
					lpSrcSurfaceTemp2 = lpSrcSurfaceTemp;
					lpDestSurfaceTemp2 = lpDestSurfaceTemp;

					for (int dx=0; dx<xStep; dx++)
					{
						*lpDestSurfaceTemp2 = *lpSrcSurfaceTemp2;

						lpSrcSurfaceTemp2++;
						lpDestSurfaceTemp2++;
					}

					lpSrcSurfaceTemp += pitch;		// 다음줄
					lpDestSurfaceTemp += wavePitch;		// 다음줄
				}
                
			}
        }

		g_pLast->Unlock();
		pWaveSurface->Unlock();

		point.x = rectWave.left;
		point.y = rectWave.top;
		
		//-----------------------------------------------------------------
		// WaveSurface --> Back
		//-----------------------------------------------------------------
		g_pLast->BltNoColorkey( &point, pWaveSurface, &rectWave );	
		*/
		//-----------------------------------------------------------------
		// Mouse 그리기
		//-----------------------------------------------------------------
		gC_vs_ui.DrawMousePointer();

		#ifdef OUTPUT_DEBUG
			if (g_pUserOption->DrawFPS)
			{
				char str[256];
				
				sprintf(str, "%d FPS", g_FrameRate);	
				
				g_pLast->GDI_Text(11,11, str, RGB(20,20,20));
				g_pLast->GDI_Text(10,10, str, 0xFFFFFF);
			}
		#endif

		//-----------------------------------------------------------------
		// Last를 Back으로 copy - 3D HAL이 아닌 경우만..
		//-----------------------------------------------------------------		
		point.x = 0;
		point.y = 0;
		RECT rect = { 0, 0, g_GameRect.right, g_GameRect.bottom };
		g_pBack->BltNoColorkey( &point, g_pLast, &rect );	

		// 창모드에서 3D가속 안한 경우에..
		// 왜 이거 하니까 빨라지지? - -;
		//HDC hdc;
		//g_pBack->GetSurface()->GetDC(&hdc);
		//g_pBack->GetSurface()->ReleaseDC(hdc);
	}	

	/*
#ifdef OUTPUT_DEBUG
	if (g_pUserOption->DrawFPS)
	{
		char str[256];
		//-----------------------------------------------------------------
		// FPS 찍기	
		//-----------------------------------------------------------------
		if (CDirect3D::IsHAL())
		{
			sprintf(str, "%d FPS(HAL)", g_FrameRate);	
		}
		else
		{
			sprintf(str, "%d FPS", g_FrameRate);	
		}
		g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
		g_pBack->GDI_Text(10,10, str, 0xFFFFFF);

		//RECT rect = { 0, 0, 50, 50 };
		//g_pBack->DrawRect(&rect, 0xFFFF);

		//DDSURFACEDESC2    ddsd;
		//ZeroMemory(&ddsd, sizeof(ddsd));
		//ddsd.dwSize = sizeof(ddsd);
		//g_pBack->GetSurface()->GetSurfaceDesc(&ddsd);		
	}
#endif
	*/
	// FPS 찍기	
//	sprintf(str, "%d Updates", g_FrameRate);	
	//g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
//	g_pBack->GDI_Text(1,1, str, 0xFFFFFF);

	// Mouse Cursor
	/*
	WORD color = 0xFFFF;//(rand()%2)?CDirectDraw::Color(20,20,20):CDirectDraw::Color(230,230,230);
	g_pBack->HLine(g_x-7, g_y, 7, color);
	g_pBack->HLine(g_x+1, g_y, 7, color);
	g_pBack->VLine(g_x, g_y-7, 7, color);
	g_pBack->VLine(g_x, g_y+1, 7, color);
	*/

	// flip
	CDirectDraw::Flip();
}

