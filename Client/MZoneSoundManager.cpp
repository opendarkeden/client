//-----------------------------------------------------------------------------
// MZoneSoundManager.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MZoneSoundManager.h"
#include "UserOption.h"
#include "CDirectSound.h"
#include "MSoundTable.h"
#include "DebugInfo.h"
#include "MTestDef.h"


#ifdef __GAME_CLIENT__
	#include "CSoundPartManager.h"
	#include "MZone.h"
	#include "MPlayer.h"

	#ifdef __METROTECH_TEST__
		#define OUTPUT_DEBUG_ZONESOUND_PROCESS
	#endif
#endif

//-----------------------------------------------------------------------------
// global
//-----------------------------------------------------------------------------
MZoneSoundTable*	g_pZoneSoundTable = NULL;

MZoneSoundManager*	g_pZoneSoundManager = NULL;

#ifdef __GAME_CLIENT__
	extern DWORD	g_CurrentTime;

	extern CSoundPartManager*	g_pSoundManager;
#endif

//-----------------------------------------------------------------------------
//
//						ZONESOUND_NODE
//
//-----------------------------------------------------------------------------
ZONESOUND_NODE::ZONESOUND_NODE(TYPE_SOUNDID id)
{
	m_SoundID	= id;
	m_pBuffer	= NULL;
	m_X			= -1; 
	m_Y			= -1;
	m_bLoop		= false;
	m_bContinueLoop = false;
}

ZONESOUND_NODE::~ZONESOUND_NODE()
{
	if (m_pBuffer!=NULL)
	{
		m_pBuffer->Stop();
		m_pBuffer->Release();
		m_pBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
// Save To File
//-----------------------------------------------------------------------------
void					
ZONESOUND_NODE::SaveToFile(ofstream& file)
{
	// 사실 이거는 file 입출력할 필요가 없당.
}

//-----------------------------------------------------------------------------
// Load From File
//-----------------------------------------------------------------------------
void					
ZONESOUND_NODE::LoadFromFile(ifstream& file)
{
	// 사실 이거는 file 입출력할 필요가 없당.
}

//-----------------------------------------------------------------------------
// Play
//-----------------------------------------------------------------------------
void
ZONESOUND_NODE::Play(int x, int y, bool bLoop)
{	
#ifdef __GAME_CLIENT__
	//-----------------------------------------------------------
	// 소리 출력해도 되는지 체크..
	//-----------------------------------------------------------
	if (!g_DXSound.IsInit() 
		|| m_SoundID >= g_pSoundTable->GetSize()
		|| !g_pUserOption->PlaySound)
	{
		return;
	}

	int playerX = g_pPlayer->GetX();
	int playerY = g_pPlayer->GetY();

	//-----------------------------------------------------------
	// Player와 소리 근원지와의 거리 계산..
	//-----------------------------------------------------------
	int gapX = x - playerX;
	int gapY = y - playerY;

	int dist = max(abs(gapX), abs(gapY));
	
	//-----------------------------------------------------------
	// 이미 같은 위치에서 play되고 있는 sound와 비교한다.
	//-----------------------------------------------------------
	if (m_pBuffer!=NULL
		&& m_playerX==playerX && m_playerY==playerY)
	{
		// 소리 큰 쪽을 남겨둬야 한다.
		int oldGapX = m_X - m_playerX;
		int oldGapY = m_Y - m_playerY;

		int oldDist = max(abs(oldGapX), abs(oldGapY));
	
		// 기존의 소리가 더 가까우면 새로운 소리를 출력할 필요가 없다.
		if (oldDist < dist)
		{
			return;
		}
	}

	//-----------------------------------------------------------
	// 값 설정
	//-----------------------------------------------------------
	m_X				= x;
	m_Y				= y;
	m_bLoop			= bLoop;
	m_bContinueLoop = bLoop;
	m_playerX		= playerX;
	m_playerY		= playerY;	

	//-----------------------------------------------------------
	// 거리가 어느 정도 이상이면 소리를 출력하지 않아야 한다.
	//-----------------------------------------------------------
	if (dist < 40)
	{			
		//-----------------------------------------------------------
		// 사운드 화일이 Load되었는지 확인.. 
		// 안됐으면.. Load한다.
		//-----------------------------------------------------------
		if (m_pBuffer==NULL)
		{
			DEBUG_ADD("[ZONESOUND_NODE] Play:New Buffer");

			//-----------------------------------------------------------
			// 없으면 --> Load
			//-----------------------------------------------------------
			if (g_pSoundManager->IsDataNULL(m_SoundID))
			{
				DEBUG_ADD("[ZONESOUND_NODE] Load Wave");

				// 다시 load						
 				LPDIRECTSOUNDBUFFER pBuffer = g_DXSound.LoadWav( (*g_pSoundTable)[m_SoundID].Filename );

				//-----------------------------------------------------------
				// Loading 실패
				//-----------------------------------------------------------
				if (pBuffer==NULL)
				{
					DEBUG_ADD_FORMAT("[Error] Failed to Load WAV. id=%d, fn=%s", m_SoundID, (*g_pSoundTable)[m_SoundID].Filename );

					return;
				}
				else
				//-----------------------------------------------------------
				// Load에 성공 했으면...			
				//-----------------------------------------------------------
				{
					// Replace됐으면 원래것을 메모리에서 지운다.
					LPDIRECTSOUNDBUFFER pOld;
					if ((*g_pSoundManager).SetData( m_SoundID, pBuffer, pOld )!=0xFFFF)
					{
						pOld->Release();
					}
					
					//-----------------------------------------------------------
					// Duplicate해서 가지고 있는다.
					//-----------------------------------------------------------
					// autoRelease 하면 안된다.
					m_pBuffer = g_DXSound.DuplicateSoundBuffer(pBuffer, false);
				}
			}
			//-----------------------------------------------------------
			// 있는 경우 --> Play
			//-----------------------------------------------------------
			else
			{
				DEBUG_ADD("[ZONESOUND_NODE] Already Exist Buffer");

				LPDIRECTSOUNDBUFFER pBuffer;
				if ((*g_pSoundManager).GetData(m_SoundID, pBuffer))
				{			
					//-----------------------------------------------------------
					// Duplicate해서 가지고 있는다.
					//-----------------------------------------------------------
					// autoRelease 하면 안된다.
					m_pBuffer = g_DXSound.DuplicateSoundBuffer(pBuffer, false);
				}
			}
		}

		DEBUG_ADD("[ZONESOUND_NODE] Arrange Volume & Dist");
			
		//-----------------------------------------------------------
		// 냠..
		//-----------------------------------------------------------
		if (m_pBuffer!=NULL)
		{
			//-----------------------------------------------------------
			// 좌우  (적당히. - -;;) 조절..
			//-----------------------------------------------------------
			if (gapX > 3)
			{
				g_DXSound.CenterToRightPan( m_pBuffer, (gapX-3) << 7 );
			}
			else if (gapX < -3)
			{
				g_DXSound.CenterToLeftPan( m_pBuffer, (abs(gapX+3)) << 7 );
			}
			else
			{
				g_DXSound.CenterPan( m_pBuffer );
			}

			//-----------------------------------------------------------
			// 소리 크기 조절
			//-----------------------------------------------------------
			int sub = (dist << 2) * g_pUserOption->VolumeSound;
			
			g_DXSound.SubVolumeFromMax(m_pBuffer, sub);

			//-----------------------------------------------------------
			// 이미 연주중이면 다시 Play하지 않는다.
			// 위에서 방향과 크기는 수정됐으니...
			//-----------------------------------------------------------
			DEBUG_ADD("[ZONESOUND_NODE] Play Buffer");
	
			if (!g_DXSound.IsPlay( m_pBuffer ))
			{
				g_DXSound.Play( m_pBuffer, m_bLoop );
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Stop
//-----------------------------------------------------------------------------
void
ZONESOUND_NODE::Stop()
{
	//-----------------------------------------------------------
	//
	//-----------------------------------------------------------
	if (m_pBuffer!=NULL)
	{
		if (g_DXSound.IsPlay( m_pBuffer ))
		{
			g_DXSound.Stop( m_pBuffer );
		}
	}

	m_bContinueLoop = false;
}

//-----------------------------------------------------------------------------
// Stop
//-----------------------------------------------------------------------------
void
ZONESOUND_NODE::StopLoop()
{
	//-----------------------------------------------------------
	//
	//-----------------------------------------------------------
	if (m_pBuffer!=NULL)
	{
		if (g_DXSound.IsPlay( m_pBuffer ))
		{
			m_pBuffer->Play(0, 0, 0);	// loop를 멈춘다.
		}
	}

	m_bContinueLoop = false;
}

//-----------------------------------------------------------------------------
//
//								MZoneSoundTable
//
//-----------------------------------------------------------------------------
MZoneSoundTable::MZoneSoundTable()
{
}

MZoneSoundTable::~MZoneSoundTable()
{
}

//-----------------------------------------------------------------------------
//
//								MZoneSoundManager
//
//-----------------------------------------------------------------------------
MZoneSoundManager::MZoneSoundManager()
{
	m_LastX = -1;
	m_LastY = -1;
	m_LastUpdateTime = 0;
}

MZoneSoundManager::~MZoneSoundManager()
{
}

//-----------------------------------------------------------------------------
// Update Sound
//-----------------------------------------------------------------------------
// Zone의 sound를 출력.. 기존에 출력되던거 있으면 수정.
//-----------------------------------------------------------------------------
void			
MZoneSoundManager::UpdateSound()
{
#ifdef __GAME_CLIENT__
	if (g_pZone==NULL 
		|| g_pPlayer==NULL)
	{
		return;
	}
		
	//--------------------------------------------------------------------
	// 음악 update해야할 시간이 됐거나..
	// Player의 좌표가 달라진 경우..
	//--------------------------------------------------------------------
	if (g_CurrentTime - m_LastUpdateTime > 5000
		|| g_pPlayer->GetX()!=m_LastX
		|| g_pPlayer->GetY()!=m_LastY)
	{
		#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
			DEBUG_ADD_FORMAT("Get Sector(%d,%d)", g_pPlayer->GetX(), g_pPlayer->GetY());
		#endif

		//--------------------------------------------------------------
		// 현재 play하고 있는 소리들에 대해서..
		//--------------------------------------------------------------
		CTypeMap<ZONESOUND_NODE>::iterator iPlaySound = begin();

		while (iPlaySound != end())
		{
			ZONESOUND_NODE* pNode = iPlaySound->second;

			//----------------------------------------------------------
			// Loop인 경우는 체크해봐야 한다.
			//----------------------------------------------------------
			pNode->UnSetContinueLoop();
			
			iPlaySound++;
		}

		//--------------------------------------------------------------
		// sector의 sound정보를 읽어온다.
		//--------------------------------------------------------------
		const MSector& sector = g_pZone->GetSector( g_pPlayer->GetX(), g_pPlayer->GetY() );

		const SECTORSOUND_LIST& listSectorSound = sector.GetSectorSoundList();

		SECTORSOUND_LIST::const_iterator iSound = listSectorSound.begin();

		#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
			DEBUG_ADD_FORMAT("listSectorSound. size=%d", listSectorSound.size());	
		#endif

		//--------------------------------------------------------------
		// 각각의 ZoneSound에 대해서 play한다.
		//--------------------------------------------------------------
		while (iSound != listSectorSound.end())
		{
			const SECTORSOUND_INFO& soundInfo = *iSound;

			int zoneSoundID = soundInfo.ZoneSoundID;
			int x = soundInfo.X;
			int y = soundInfo.Y;

			#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
				DEBUG_ADD_FORMAT("SectorSoundInfo. id=%d, (%d, %d)", zoneSoundID, x, y);
			#endif

			ZONESOUND_INFO* pInfo = g_pZoneSoundTable->GetData( zoneSoundID );

			//---------------------------------------------------------
			// 연주 해야할 시간이면..
			//---------------------------------------------------------
			if (pInfo!=NULL)
			{
				if (pInfo->IsShowTime())
				{
					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("Get ZONESOUND_NODE");
					#endif
	
					ZONESOUND_NODE* pSound = GetData( zoneSoundID );

					//------------------------------------------------------
					// Play
					//------------------------------------------------------
					if (pSound==NULL)
					{
						// 없으면 생성한다.
						pSound = new ZONESOUND_NODE( pInfo->SoundID );

						// 추가하고 play
						AddData( zoneSoundID, pSound );						
					}					
					
					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("Before Play");
					#endif

					pSound->Play( x, y, pInfo->Loop );					

					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("After Play");
					#endif

					//------------------------------------------------------
					// 다음에 연주할 시간을 결정한다.
					//------------------------------------------------------
					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("Set NextPlayTime");
					#endif

					pInfo->SetNextShowTime();
				}
				//---------------------------------------------------------
				// 연주할 시간대가 아니면 소리를 멈춘다.
				//---------------------------------------------------------
				else if (!pInfo->IsShowHour())
				{
					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("[for stop]Get ZONESOUND_NODE");
					#endif

					ZONESOUND_NODE* pSound = GetData( zoneSoundID );

					//------------------------------------------------------
					// Play
					//------------------------------------------------------
					if (pSound!=NULL)
					{
						pSound->Stop();
					}	
				}
				else
				{
					ZONESOUND_NODE* pSound = GetData( zoneSoundID );

					if (pSound->IsLoop())
					{
						pSound->SetContinueLoop();
					}

					#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
						DEBUG_ADD("Skip");
					#endif
				}

			}

			iSound++;
		}

		#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
			DEBUG_ADD("UpdateSectorSound OK");
		#endif

		//--------------------------------------------------------------
		// 현재 play하고 있는 소리들에 대해서..
		//--------------------------------------------------------------
		iPlaySound = begin();

		while (iPlaySound != end())
		{
			ZONESOUND_NODE* pNode = iPlaySound->second;

			//----------------------------------------------------------
			// Loop인 경우는 현재 sector에 없으면 소리를 멈춰야 한다.
			//----------------------------------------------------------
			if (pNode->IsLoop() && !pNode->IsContinueLoop())
			{
				pNode->StopLoop();
			}

			iPlaySound++;
		}

		#ifdef OUTPUT_DEBUG_ZONESOUND_PROCESS
			DEBUG_ADD("zoneSoundNode OK");
		#endif

		//---------------------------------------------------------
		// update 정보 기억
		//---------------------------------------------------------
		m_LastX = g_pPlayer->GetX();
		m_LastY = g_pPlayer->GetY();
		m_LastUpdateTime = g_CurrentTime;
	}
#endif
}