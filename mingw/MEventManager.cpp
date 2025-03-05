//----------------------------------------------------------------------
// MEventManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEventManager.h"
#include "CJpeg.h"

#if defined(__GAME_CLIENT__)
	#include "MPlayer.h"
	#include "UtilityFunction.h"
	#include "AppendPatchInfo.h"
#endif
#include "DebugInfo.h"
#include "PacketFunction.h"
//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
MEventManager* g_pEventManager = NULL;

MEvent::MEvent()
{
	eventID = EVENTID_NULL;
	eventType = EVENTTYPE_NULL;
	eventDelay = -1;
	showTime = -1;
	totalTime = -1;
	eventFlag = NULL;
	parameter1 = NULL;
	parameter2 = NULL;
	parameter3 = NULL;
	parameter4 = NULL;
}

MEvent::~MEvent()
{
}

const bool
MEvent::IsShowTime() const
{
	if( showTime == -1 )
		return true;

	if( ( GetTickCount() - eventStartTickCount ) % totalTime < showTime )
		return true;

	return false;
}

MEventManager::MEventManager()
{
}

MEventManager::~MEventManager()
{
	RemoveAllEvent();
}

//--------------------------------------------------
// 이벤트를 등록한다
//--------------------------------------------------
void	MEventManager::AddEvent(MEvent &event)
{
	event.eventStartTickCount = GetTickCount();
	m_Events[event.eventID] = event;
	if(event.eventFlag | EVENTFLAG_FADE_SCREEN)
	{
		const MEvent *tempEvent = GetEventByFlag(EVENTFLAG_FADE_SCREEN);
		if(tempEvent != NULL)
			CDirectDraw::SetAddGammaRamp((tempEvent->parameter2 >> 16) & 0xff, (tempEvent->parameter2 >> 8) & 0xff, tempEvent->parameter2 & 0xff);
	}
}

//--------------------------------------------------
// 이벤트를 가져온다
//--------------------------------------------------
const MEvent*	MEventManager::GetEvent(EVENT_ID id)
{
//	return &m_Events[id];
	EVENT_MAP::iterator itr = m_Events.find(id);

	if(itr != m_Events.end())
		return &itr->second;

	return NULL;
}

//--------------------------------------------------
// 이벤트가 있는가 본다
//--------------------------------------------------
bool	MEventManager::IsEvent(EVENT_ID id)
{
	return (GetEvent(id) != NULL);
}

//--------------------------------------------------
// 이벤트를 지운다
//--------------------------------------------------
void	MEventManager::RemoveEvent(EVENT_ID id)
{
	DEBUG_ADD_FORMAT("[MEventManager] RemoveEvent : %d", id);
	const MEvent *event = GetEvent(id);

	if(event == NULL)
	{
		DEBUG_ADD("MEventManager] RemoveEvent event == NULL");
		return;
	}

	bool bFadeScreen = (event->eventFlag | EVENTFLAG_FADE_SCREEN) != false;

	m_Events.erase(id);

	if(bFadeScreen)
	{
		event = GetEventByFlag(EVENTFLAG_FADE_SCREEN);
		if(event == NULL)
			CDirectDraw::SetAddGammaRamp();
		else
			CDirectDraw::SetAddGammaRamp((event->parameter2 >> 16) & 0xff, (event->parameter2 >> 8) & 0xff, event->parameter2 & 0xff);
	}
	DEBUG_ADD("[MEventManager] RemoveEvent OK");
}

//--------------------------------------------------
// 모든 이벤트를 지운다
//--------------------------------------------------
void	MEventManager::RemoveAllEvent()
{ 
	DEBUG_ADD_FORMAT("[MEventManager] RemoveAllEvent Count: %d", m_Events.size());
	EVENT_MAP::iterator itr = m_Events.begin();

	while(itr != m_Events.end())
	{
		EVENT_ID delete_id = itr->second.eventID;
		DEBUG_ADD_FORMAT("[MEventManager] Call RemoveEvent(%d)", delete_id);
		itr++;
		RemoveEvent(delete_id);
	}
	DEBUG_ADD_FORMAT("[MEventManager] RemoveAllEvent OK");

}
//--------------------------------------------------
// 타입별 이벤트를 지운다
//--------------------------------------------------
void	MEventManager::RemoveAllEventByType(EVENT_TYPE type)
{
	EVENT_MAP::iterator itr = m_Events.begin();

	while(itr != m_Events.end())
	{
		if(itr->second.eventType == type)
		{
			EVENT_ID delete_id = itr->second.eventID;
			itr++;
			RemoveEvent(delete_id);
		}
		else
			itr++;
	}
}

//--------------------------------------------------
// Flag별 개수
//--------------------------------------------------
const int	MEventManager::GetEventCountByFlag(DWORD flag)
{
	EVENT_MAP::iterator itr = m_Events.begin();
	int count = 0;

	while(itr != m_Events.end())
	{
		if(itr->second.eventFlag | flag)
		{
			count++;
		}

		itr++;
	}

	return count;
}

//--------------------------------------------------
// Flag로 empty검색
//--------------------------------------------------
const bool		MEventManager::IsEmptyEventByFlag(DWORD flag)
{
	EVENT_MAP::iterator itr = m_Events.begin();

	while(itr != m_Events.end())
	{
		if(itr->second.eventFlag | flag)
		{
			return false;
		}

		itr++;
	}

	return true;
}

//--------------------------------------------------
// Flag로 GetEvent해온다
//--------------------------------------------------
const MEvent*	MEventManager::GetEventByFlag(DWORD flag, int count)
{
	EVENT_MAP::iterator itr = m_Events.begin();
	int i = 0;

	while(itr != m_Events.end())
	{
		if(itr->second.eventFlag & flag)
		{
			if(i == count)
				return &itr->second;
			i++;
		}

		itr++;
	}

	return NULL;
}

extern DWORD g_CurrentFrame;
//--------------------------------------------------
// Process
//--------------------------------------------------
void	MEventManager::ProcessEvent()
{
	EVENT_MAP::iterator itr = m_Events.begin();

	while(itr != m_Events.end())
	{
		if(itr->second.eventDelay != -1)
		{
			if((GetTickCount() - itr->second.eventStartTickCount) > itr->second.eventDelay)
			{
				EVENT_ID delete_id = itr->second.eventID;
				itr++;
				RemoveEvent(delete_id);
#ifdef __GAME_CLIENT__				
				// 2004, 6, 21, sobeit add start - 질드레 연출 - 5초간 흔들렸으면 10초간 어두워짐
				if(delete_id == EVENTID_GDR_PRESENT)
					SetFadeStart(31, -1, 1, 0,0,0, 4);
				// 2004, 6, 21, sobeit add end
#endif
				continue;
			}
		}
		if(itr->second.eventType == EVENTTYPE_EFFECT)
		{
#ifdef __GAME_CLIENT__
			if(!g_pPlayer->HasEffectStatus((EFFECTSTATUS)itr->second.parameter1))
			{
				EVENT_ID delete_id = itr->second.eventID;
				itr++;
				RemoveEvent(delete_id);
				continue;
			}
#endif
				
		}
		itr++;
	}
}

bool MEventManager::AssertEventBackground(EVENTBACKGROUND_ID id)
{
	static std::string strFilename[EVENTBACKGROUNDID_MAX] = 
	{
		"Data\\Image\\EventBackgroundCosmos.jps",
		"Data\\Image\\EventBackgroundOustersSlayer.jps",
		"Data\\Image\\EventBackgroundOustersVampire.jps",
		"Data\\Image\\EventBackgroundQuest2.jps",
		"Data\\Image\\EventBackgroundCloud.jps",
	
	};

	if(id >= EVENTBACKGROUNDID_MAX)
		return false;

	if(m_EventBackGround.GetSize() == 0)
	{
		m_EventBackGround.Init(EVENTBACKGROUNDID_MAX);
	}

	if(m_EventBackGround[id].GetSurface() != NULL)
		return true;

	CJpeg jpg;
	bool bOpen = jpg.Open(strFilename[id].c_str());
	if(bOpen == true && jpg.GetWidth() > 0 && jpg.GetHeight() > 0 && jpg.GetHeight() > 0)
	{
		CDirectDrawSurface &surface = m_EventBackGround[id];
		const int bpp = jpg.GetBpp(), width = jpg.GetWidth(), height = jpg.GetHeight(), pitch = width*bpp;

		if (surface.InitOffsurface(width, height, DDSCAPS_SYSTEMMEMORY))
		{
			if (surface.Lock())
			{
				WORD *pSurface = (WORD *)surface.GetSurfacePointer();
				unsigned char *pData = jpg.GetImage(), *pDataTemp;
				WORD *pSurfaceTemp;
				
				int surfacePitch = surface.GetSurfacePitch();
				
				if (pSurface)
				{
					if (bpp == 1)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;								
							
							for (register int x = 0; x < width; x++)
							{
								BYTE temp_data = *pDataTemp++;	//p_data[y*(pitch)+x];
								BYTE r = temp_data>>3;
								BYTE g = r;
								BYTE b = r;
								
								*pSurfaceTemp++ = CDirectDraw::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					else if (bpp == 3)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;	
							
							for (register int x = 0; x < width; x++)
							{
								//char *temp_data = &p_data[y*pitch+x*bpp];
								BYTE r = *(pDataTemp+2) >> 3;		//temp_data[2]>>3;
								BYTE g = *(pDataTemp+1) >> 3;	//temp_data[1]>>3;
								BYTE b = *pDataTemp >> 3;	//temp_data[0]>>3;
								
								pDataTemp += bpp;
								
								*pSurfaceTemp++ = CDirectDraw::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					
				}
				
				surface.Unlock();
			}
		}
	}

	return true;
}