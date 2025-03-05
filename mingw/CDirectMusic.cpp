//----------------------------------------------------------------------
// CDirectMusic.cpp
//----------------------------------------------------------------------
#include "DirectXlib_PCH.h"
#include "CDirectSound.h"
#include "CDirectMusic.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
 
CDirectMusic		g_DXMusic;

//----------------------------------------------------------------------
// 
// constructor / destructor
//
//----------------------------------------------------------------------
CDirectMusic::CDirectMusic()
{
	m_pDM				= NULL;
	m_pDMPerformance	= NULL;
	m_pDMPort			= NULL;
	m_pDMLoader			= NULL;
	m_pDMSegment		= NULL;
	m_pDMSegmentState	= NULL;

	m_mtStart	= 0;	
	m_mtOffset	= 0;
	m_rtStart	= 0;
	m_rtOffset	= 0;

	m_bInit				= false;
	m_bLoad				= false;
	m_bPlay				= false;

	m_bSoftwareSynth	= true;

	//m_MasterVolume		= 0;	
}

CDirectMusic::~CDirectMusic()
{
	Release();	
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
bool
CDirectMusic::Init(HWND hWnd, DIRECTMUSIC_TYPE type)
{
	Release();

	if (FAILED(CoInitialize(NULL)))
	{
		MessageBox(hWnd, "CDirectMusic: CoInitialize Error!", "Error", MB_OK);
		return false;
	}


	if (g_DXSound.m_pDS) 
	{
		MessageBox(hWnd, "CDirectMusic: DirectSound Error!", "Error", MB_OK);
		return false;
	}

	if (FAILED(CoCreateInstance(CLSID_DirectMusic, NULL,
                                CLSCTX_INPROC_SERVER, IID_IDirectMusic,
                                (VOID**) &m_pDM)))
	{
		MessageBox(hWnd, "CDirectMusic: CoCreate COM Instance Failed!", "Error", MB_OK);
        return false;
	}
	
	
    if (FAILED(m_pDM->SetDirectSound(g_DXSound.m_pDS, hWnd)))
	{
		MessageBox(hWnd, "CDirectMusic: SetDirectSound Failed!", "Error", MB_OK);
        return false;
	}

    if (!CreatePerformance())
	{
		MessageBox(hWnd, "CDirectMusic: Create Performance Failed!", "Error", MB_OK);
		return false;
	}

   	if (!CreateLoader())
	{
		MessageBox(hWnd, "CDirectMusic: Create Loader Failed!", "Error", MB_OK);
		return false;
	}

	 if (!CreatePort(type))
	{
		//MessageBox(hWnd, "CDirectMusic: Create SoftWareSynthPort Failed!", "Error", MB_OK);
		return false;
	}        


	m_bInit = true;

    return true;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void 
CDirectMusic::Release()
{
	m_bInit = false;
	m_bLoad = false;
	m_bPlay = false;

	Stop();

    if (m_pDM)
	{
        m_pDM->Activate(FALSE);
	}

	if (m_pDMSegmentState)
	{
		m_pDMSegmentState->Release();
        m_pDMSegmentState = NULL;
	}

	if (m_pDMSegment)
	{
		m_pDMSegment->SetParam(GUID_Unload, -1, 0, 0, (void*)m_pDMPerformance);
 
		m_pDMSegment->Release();
		m_pDMSegment = NULL;
	}

    if (m_pDMPort)
    {
        m_pDMPort->Release();
        m_pDMPort = NULL;
    }

    if (m_pDMPerformance)
    {
        // If there is any music playing, stop it.
        //m_pDMPerformance->Stop(NULL, NULL, 0, 0);

        // CloseDown and Release the performance object.
        m_pDMPerformance->CloseDown();
        m_pDMPerformance->Release();
        m_pDMPerformance = NULL;
    }

    if (m_pDMLoader)
    {
        m_pDMLoader->Release();
        m_pDMLoader = NULL;
    }

    if (m_pDM)
    {
        m_pDM->Release();
        m_pDM = NULL;
    }
	
	CoUninitialize();   
}

//----------------------------------------------------------------------
// Add Volume
//----------------------------------------------------------------------
// hdec만큼 volume을 증가시킨다.
//----------------------------------------------------------------------
/*
void		
CDirectMusic::AddVolume(long hdec)
{
	if (m_bInit)
	{
		long volume = 0;

		HRESULT hr = m_pDMPerformance->GetGlobalParam(
			GUID_PerfMasterVolume,
			&volume,
			sizeof(volume)
		);

		if (hr==E_FAIL)
		{
			int a = 0;
		}
		else if (hr==E_POINTER)
		{
			int a = 0;
		}
		else if (hr==E_OUTOFMEMORY)
		{
			int a = 0;
		}

		volume -= hdec;

		m_pDMPerformance->SetGlobalParam(
			GUID_PerfMasterVolume,
			&volume,
			sizeof(volume)
		);

	}
}

//----------------------------------------------------------------------
// Sub Volume
//----------------------------------------------------------------------
// hdec만큼 volume을 감소시킨다.
//----------------------------------------------------------------------
void		
CDirectMusic::SubVolume(long hdec)
{
}
*/

//----------------------------------------------------------------------
// Play
//----------------------------------------------------------------------
bool 
CDirectMusic::Play(const char* filename, WORD repeat)
{	
	// file이름이 없는 경우
	if (!m_bInit || filename==NULL) 
	{
		return false;
	}
	
    if (!m_pDMLoader)
	{
		//MessageBox(NULL, "Not Init DirectMusicLoader!", "DMusic Error", MB_OK);
        return false;
	}

    DMUS_OBJECTDESC      ObjDesc;
	/*
	char				 szDir[_MAX_PATH];
    WCHAR                wszDir[_MAX_PATH];    

	if (_getcwd(szDir, _MAX_PATH) == NULL)
		return false;
	else
		mbstowcs(wszDir, szDir, MAX_PATH);

    m_pDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wszDir, FALSE);
	*/

    ObjDesc.dwSize      = sizeof(DMUS_OBJECTDESC);
    ObjDesc.guidClass   = CLSID_DirectMusicSegment;
    ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

    mbstowcs(ObjDesc.wszFileName, filename, DMUS_MAX_FILENAME);

	
	if (m_pDMSegment)
	{
		m_pDMSegment->Release();
		m_pDMSegment = NULL;
	}


    m_pDMLoader->GetObject(&ObjDesc, IID_IDirectMusicSegment2, (VOID **) &m_pDMSegment);

	// 없는 경우...
	if (!m_pDMSegment) 
	{
		return false;
	}

	m_pDMSegment->SetParam(GUID_StandardMIDIFile, -1, 0, 0, (void*)m_pDMPerformance);

	m_pDMSegment->SetParam(GUID_Download, -1, 0, 0, (void*)m_pDMPerformance);

	// 반복 회수 설정
	m_pDMSegment->SetRepeats(repeat);	

	// 연주한다.
	if SUCCEEDED(m_pDMPort->Activate(TRUE))
	{
		if (m_pDMSegmentState)
		{
			m_pDMSegmentState->Release();
			m_pDMSegmentState = NULL;
		}

		if SUCCEEDED(m_pDMPerformance->PlaySegment(
					m_pDMSegment, 
					0,//DMUS_SEGF_BEAT, 
					0, 
					&m_pDMSegmentState))
		{
			// Find out the performance time when segment began playing and
			// convert to reference time. This way if the tempo changes later
			// in the file, we will still have the correct reference start time
			m_pDMSegmentState->GetStartTime( &m_mtStart );
			
			m_pDMPerformance->MusicToReferenceTime( m_mtStart, &m_rtStart );

			m_bLoad = true;
			m_bPlay = true;	
			
			//----------------------------------------------------
			// original tempo를 얻어낸다.
			//----------------------------------------------------
			DMUS_TEMPO_PARAM tempoParams;
			
			HRESULT hr = m_pDMSegment->GetParam( GUID_TempoParam, 
												0xffffffff, 0, 0, NULL,
												(VOID*)&tempoParams );
			if (SUCCEEDED(hr))
			{		
				m_OriginalTempo = (int)tempoParams.dblTempo;
				m_CurrentTempo	= m_OriginalTempo;
			}
			else
			{
				m_OriginalTempo = 100;
				m_CurrentTempo	= 100;
			}

			return true;
		}
	}
	else
	{
		//MessageBox(NULL,"소프트웨어 신디포트 활성화 에러 ","DMusic Error",MB_OK);
		return false;
	}

	return false;
}

//----------------------------------------------------------------------
// Pause
//----------------------------------------------------------------------
// 잠시 중단시킨다.
//----------------------------------------------------------------------
void 
CDirectMusic::Pause()
{
	// 화일이 Load되어서 연주 중인 경우...
	if (m_bInit && m_bLoad && m_bPlay)
	{	
		MUSIC_TIME          mtNow;
		REFERENCE_TIME      rtNow;
                
		m_pDMPerformance->Stop(NULL, NULL, 0, 0);

		//Find out the current performance time so that we can figure out 
		//where we stopped in the segment.
		m_pDMPerformance->GetTime(&rtNow, &mtNow);

		// Caculate the offset into the segment in music time (ticks)
		// and reference time (milliseconds) and add to previous offset in 
		// cause there has been more than one pause in this segment playback
		m_mtOffset = (mtNow - m_mtStart) + m_mtOffset; 
		m_rtOffset = (rtNow - m_rtStart) + m_rtOffset;

		// Set restart point
		m_pDMSegment->SetStartPoint( m_mtOffset ); 
		m_pDMSegmentState->Release();
		m_pDMSegmentState = NULL;

		m_bPlay = false;	
	}
}

//----------------------------------------------------------------------
// Resume
//----------------------------------------------------------------------
// 정지되었던 음악을 다시 연주한다.
//----------------------------------------------------------------------
void 
CDirectMusic::Resume()
{
	if (m_bInit && m_bLoad && !m_bPlay)
	{
		if (SUCCEEDED(m_pDMPerformance->PlaySegment(
				m_pDMSegment,
				DMUS_SEGF_BEAT,
				0,
				&m_pDMSegmentState)))
		{
			// Find out the performance time when segment began playing and
			// convert to reference time. This way if the tempo changes later
			// in the file, we will still have the correct reference start time
			m_pDMSegmentState->GetStartTime( &m_mtStart );

			m_pDMPerformance->MusicToReferenceTime( m_mtStart, &m_rtStart );

			m_bPlay = true;
		}
	}
}

//----------------------------------------------------------------------
// Stop
//----------------------------------------------------------------------
void 
CDirectMusic::Stop()
{
	if (m_bInit && m_bLoad)
	{
		m_bLoad = false;
		m_bPlay = false;

		m_pDMPerformance->Stop(NULL, NULL, 0, 0);		

		// Start segment from beginning in case this has been previously paused.
		m_pDMSegment->SetStartPoint( 0 );

		if (m_pDMSegmentState)
		{
			m_pDMSegmentState->Release();
			m_pDMSegmentState = NULL;
		}

		m_rtOffset = 0;
		m_mtOffset = 0;
	}
}

//----------------------------------------------------------------------
// Set Tempo
//----------------------------------------------------------------------
void
CDirectMusic::SetCurrentTempo(int tempo)
{
	if (m_bInit && m_bLoad)
	{
		//--------------------------------------------------------
		//sends a message to change the tempo
		//--------------------------------------------------------
		// Disable tempo track in segment so that it does not reset the tempo.
		m_pDMSegment->SetParam( GUID_DisableTempo, 0xFFFF,0,0, NULL );
 
		DMUS_TEMPO_PMSG* pTempo;
 
		if( SUCCEEDED(m_pDMPerformance->AllocPMsg(
				sizeof(DMUS_TEMPO_PMSG), (DMUS_PMSG**)&pTempo)))
		{
			// Queue the tempo event.
			ZeroMemory(pTempo, sizeof(DMUS_TEMPO_PMSG));
			pTempo->dwSize = sizeof(DMUS_TEMPO_PMSG);
			pTempo->dblTempo = (double)tempo;
			pTempo->dwFlags = DMUS_PMSGF_REFTIME;
			pTempo->dwType = DMUS_PMSGT_TEMPO;
			m_pDMPerformance->SendPMsg((DMUS_PMSG*)pTempo);

			m_pDMPerformance->FreePMsg((DMUS_PMSG*)pTempo);

			// 현재 템포를 기억해둔다.
			m_CurrentTempo = tempo;
		}

		// Enable tempo track in segment 
		m_pDMSegment->SetParam( GUID_EnableTempo, 0xFFFF,0,0, NULL );
	}

	//--------------------------------------------------------
	// change the tempo parameter
	//--------------------------------------------------------
	//DMUS_TEMPO_PARAM Tempo;
	//Tempo.dblTempo = t;
	//m_pDMSegment->SetParam(GUID_TempoParam, 0xFFFF, 0, 0, &Tempo);
}

//----------------------------------------------------------------------
// Create Performance
//----------------------------------------------------------------------
bool 
CDirectMusic::CreatePerformance()
{
    if (!m_pDM)
        return false;

	/*
    if (SUCCEEDED(CoCreateInstance(CLSID_DirectMusicPerformance,NULL,
                                   CLSCTX_INPROC, IID_IDirectMusicPerformance,
                                   (VOID**) &m_pDMPerformance)))
    {
        if (SUCCEEDED(m_pDMPerformance->Init(&m_pDM, NULL, NULL)))
        {
            // Set autodownloading to be on
            BOOL fAutoDownload = TRUE;
            if (FAILED(m_pDMPerformance->SetGlobalParam(GUID_PerfAutoDownload,
                                             &fAutoDownload,
                                             sizeof(fAutoDownload))))
            {
                m_pDMPerformance->Release();
                m_pDMPerformance = NULL;
				return false;
            }
        }
    }
    else
	{
        m_pDMPerformance = NULL;
		return false;
	}
	*/

    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicPerformance,
            NULL,
            CLSCTX_INPROC, 
            IID_IDirectMusicPerformance2,
            (void**)&m_pDMPerformance
        )))
    {
        m_pDMPerformance = NULL;

		return false;
    }


	if (FAILED(m_pDMPerformance->Init(NULL, NULL, NULL)))
	{
		return false;
	}

    
    return true;
}

//----------------------------------------------------------------------
// Create Port
//----------------------------------------------------------------------
// dmpc.dwType이 WINMM이고 EXTERNAL이 아닌것을 일단 찾는다.
// 없으면.. --> SoftwareSynth(Default)를 사용한다.
//----------------------------------------------------------------------
bool 
CDirectMusic::CreatePort(DIRECTMUSIC_TYPE type)
{
    if (!m_pDM)
        return false;

	m_bSoftwareSynth	= true;

    HRESULT           hr;
    GUID              guidPortGUID;
    DMUS_PORTPARAMS   dmos;
    DMUS_PORTCAPS     dmpc;

	if (m_pDMPort!=NULL)
    {
        m_pDMPort->Release();
        m_pDMPort = NULL;
    }
	
	//----------------------------------------------------
	// Find a output port.
	//----------------------------------------------------
	// Hardware Midi를 사용하는 경우
	//----------------------------------------------------
	if (type==DIRECTMUSIC_TYPE_HW)
	{
		for (DWORD index=0;  ; index++)
		{
			ZeroMemory(&dmpc, sizeof(dmpc));
			dmpc.dwSize = sizeof(DMUS_PORTCAPS);

			hr = m_pDM->EnumPort(index, &dmpc);

			//----------------------------------------------------
			// 존재하는 Port인 경우...
			//----------------------------------------------------
			if (SUCCEEDED(hr) && hr!=S_FALSE)
			{		
				//----------------------------------------------------
				// 적절한 port인지 알아본다.
				//----------------------------------------------------
				// class가 DMUS_PC_OUTPUTCLASS이어야 한다.
				// flag에 DMUS_PC_EXTERNAL이 있으면 안된다.
				// type이 DMUS_PORT_WINMM_DRIVER여야 한다.
				if ((dmpc.dwClass == DMUS_PC_OUTPUTCLASS) &&
					!(dmpc.dwFlags & DMUS_PC_EXTERNAL) &&
					(dmpc.dwType == DMUS_PORT_WINMM_DRIVER))
				{	
					/*
					//----------------------------------------------------
					// Port에 대한 정보를 check하는 test code
					//----------------------------------------------------
					if (portIndex!=3)
					{						
						portIndex ++;
						continue;
					}				

					for (int f=0; f<10; f++)
					{
						bool have = false;
						if (dmpc.dwFlags & flags[f])
						{
							have = true;
						}
					}

					if (dmpc.dwType==DMUS_PORT_WINMM_DRIVER)
					{
						int a =0;
					}
					else if (dmpc.dwType==DMUS_PORT_USER_MODE_SYNTH)
					{
						int a =0;
					}
					else if (dmpc.dwType==DMUS_PORT_KERNEL_MODE)
					{
						int a =0;
					}
					*/

					CopyMemory(&guidPortGUID, &dmpc.guidPort, sizeof(GUID));

					ZeroMemory(&dmos, sizeof(dmos));
					dmos.dwSize          = sizeof(DMUS_PORTPARAMS);
					dmos.dwChannelGroups = 1;
					dmos.dwValidParams   = DMUS_PORTPARAMS_CHANNELGROUPS;						

					//----------------------------------------------------
					// m_pPort에 찾은 Port를 생성
					//----------------------------------------------------
					if (FAILED(m_pDM->CreatePort(guidPortGUID, &dmos, &m_pDMPort, NULL)))
					{
						m_pDMPort = NULL;
						break;
					}

					// WinMM사용 가능한 경우...
					m_bSoftwareSynth	= false;

					break;
				}

				// 다른 경우는 무시한다.
			}
			//----------------------------------------------------
			// 더 이상 체크할 port가 없는 경우
			//----------------------------------------------------
			else
			{
				break;	
			}
		}
	}

	//----------------------------------------------------
	// 적절한 Port를 못찾은 경우... 
	// default port를 사용한다.
	//----------------------------------------------------
	if (m_bSoftwareSynth)
	{	
		// See if the default port suits our needs
		if (SUCCEEDED(m_pDM->GetDefaultPort(&guidPortGUID)))
		{
			ZeroMemory(&dmos, sizeof(dmos));

			dmos.dwSize          = sizeof(DMUS_PORTPARAMS);
			dmos.dwChannelGroups = 1;
			dmos.dwValidParams   = DMUS_PORTPARAMS_CHANNELGROUPS;

			if (SUCCEEDED(m_pDM->CreatePort(guidPortGUID, &dmos, &m_pDMPort, NULL)))
			{
				ZeroMemory(&dmpc, sizeof(dmpc));
				dmpc.dwSize = sizeof(DMUS_PORTCAPS);

				if (SUCCEEDED(m_pDMPort->GetCaps(&dmpc)))
				{
					if ( (dmpc.dwClass != DMUS_PC_OUTPUTCLASS) ||
						!(dmpc.dwFlags & DMUS_PC_DLS) ||
						!(dmpc.dwFlags & DMUS_PC_DIRECTSOUND))
					{
						m_pDMPort->Release();
						m_pDMPort = NULL;
						return false;
					}
				}
				else
				{
					m_pDMPort->Release();
					m_pDMPort = NULL;
					return false;
				}
			}
		}
	}

	/*
	DWORD flags[10] =
	{
		DMUS_PC_DIRECTSOUND ,		// 0
		DMUS_PC_DLS ,				// 1
		DMUS_PC_DLS2 ,				// 2
		DMUS_PC_EXTERNAL ,			// 3
		DMUS_PC_GMINHARDWARE ,		// 4
		DMUS_PC_GSINHARDWARE ,		// 5
		DMUS_PC_MEMORYSIZEFIXED ,	// 6
		DMUS_PC_SHAREABLE ,			// 7
		DMUS_PC_SOFTWARESYNTH ,		// 8
		DMUS_PC_XGINHARDWARE		// 9
	};

	int portIndex = 0;
	*/

	if (!m_pDMPerformance || !m_pDMPort || !m_pDMLoader)
        return false;

	// Add Port
    if (SUCCEEDED(m_pDMPerformance->AddPort(m_pDMPort)))
	{
        if (SUCCEEDED(m_pDMPerformance->AssignPChannelBlock(0, m_pDMPort, 1)))
		{
			//m_bInit = true;

            return true;
		}
	}

    return false;
}

//----------------------------------------------------------------------
// Create Loader
//----------------------------------------------------------------------
bool 
CDirectMusic::CreateLoader()
{
	/*
    if (FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,
                                 IID_IDirectMusicLoader, (VOID**)&m_pDMLoader)))
	{
        m_pDMLoader = NULL;
		return false;
	}
	*/

    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicLoader,
            NULL,
            CLSCTX_INPROC, 
            IID_IDirectMusicLoader,
            (void**)&m_pDMLoader
        )))
    {
        m_pDMLoader = NULL;

		return false;
    }
    

    return true;
}

