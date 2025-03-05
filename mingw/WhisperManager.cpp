//----------------------------------------------------------------------
// WhisperManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "WhisperManager.h"
#include "RequestClientPlayerManager.h"
#include "RequestUserManager.h"
#include "MGameStringTable.h"

#include "Cpackets\CGRequestIP.h"
#include "Cpackets\CGWhisper.h"
#include "ClientPlayer.h"
#include "DebugInfo.h"
#include "ServerInfo.h"
#include "UserInformation.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
WhisperManager*		g_pWhisperManager = NULL;

//----------------------------------------------------------------------
// extern
//----------------------------------------------------------------------
	extern ClientPlayer*		g_pSocket;

extern void		UI_AddChatToHistory(char* str, char* sz_id, int cond, DWORD color);


//----------------------------------------------------------------------
//
//						WhisperInfo
//
//----------------------------------------------------------------------
// Send to GameServer
//----------------------------------------------------------------------
void
WhisperInfo::SendToGameServer() const
{
	std::list<WHISPER_MESSAGE>::const_iterator iMessage = Messages.begin();


	CGWhisper _CGWhisper;
	_CGWhisper.setName( Name.c_str() );

	// 모든 message 보내기
	while (iMessage != Messages.end())
	{		
		_CGWhisper.setMessage( (*iMessage).msg.c_str() );
		_CGWhisper.setColor( (*iMessage).color );

		g_pSocket->sendPacket( &_CGWhisper );

		iMessage ++;
	}
}

//----------------------------------------------------------------------
//
//						WhisperManager
// 
//----------------------------------------------------------------------
WhisperManager::WhisperManager()
{
	InitializeCriticalSection(&m_Lock);
}

WhisperManager::~WhisperManager()
{
	Release();

	DeleteCriticalSection(&m_Lock);
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void		
WhisperManager::Release()
{
	Lock();

	WHISPER_INFO_MAP::iterator iInfo = m_WhisperInfos.begin();

	while (iInfo != m_WhisperInfos.end())
	{
		WhisperInfo* pInfo = iInfo->second;

		delete pInfo;

		iInfo++;
	}

	m_WhisperInfos.clear();

	Unlock();
}

//----------------------------------------------------------------------
// Send / Remove Message
//----------------------------------------------------------------------
bool		
WhisperManager::HasWhisperMessage(const char* pName) const
{
	WHISPER_INFO_MAP::const_iterator iInfo = m_WhisperInfos.find( std::string(pName) );	

	if (iInfo != m_WhisperInfos.end())
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Send Message
//----------------------------------------------------------------------
//
// if (has toUser's IP in RequestUserManager)
// {			
//		RequestClientManager --(CRWhisper)--> RequestServer(toUser) [2]			
// }
// else
// {
//		CGWhisper --> GameServer [3]
// }
//
//----------------------------------------------------------------------
void		
WhisperManager::SendWhisperMessage(const char* pName, const char* pMessage, DWORD color)
{
	//CRWhisper *pPacket;
	//_CRWhisper.setName( pName );
	//_CRWhisper.setMessage( pMessage );
	//Packet* pPacket = NULL;

	if( 0 && g_pUserInformation->bKorean == true )	// 한국버전이면 p2p
	{
		//-------------------------------------------------------
		// 접속중이거나 접속 시도 중인 경우..
		//-------------------------------------------------------
		if (g_pRequestClientPlayerManager->HasConnection(pName)
			|| g_pRequestClientPlayerManager->HasTryingConnection(pName))//!g_pRequestClientPlayerManager->SendPacket(pName, pPacket))
		{
			AddWhisperMessage( pName, pMessage, color );
		}
		//-------------------------------------------------------
		// 접속중이 아닌 경우
		//-------------------------------------------------------
		else
		{
			RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo(pName);

			//-------------------------------------------------------
			// 사용자 정보가 있다면 접속 시도를 한다.
			//-------------------------------------------------------
			if (pUserInfo!=NULL)
			{
				//-------------------------------------------------------
				// 접속 불가 상태인 경우
				//-------------------------------------------------------
				if (pUserInfo->IsStatusUnable())
				{
					// GameServer로 그냥 보낸다.
						CGWhisper _CGWhisper;
						_CGWhisper.setName( pName );

						_CGWhisper.setMessage( pMessage );
						_CGWhisper.setColor( color );

						g_pSocket->sendPacket( &_CGWhisper );
				}
				//-------------------------------------------------------
				// 정상적인 접속이 되는 경우..
				//-------------------------------------------------------
				else
				{
					AddWhisperMessage( pName, pMessage, color );
					g_pRequestClientPlayerManager->Connect(pUserInfo->IP.c_str(), pName, REQUEST_CLIENT_MODE_WHISPER);
				}
			}
			//-------------------------------------------------------
			// 사용자 정보가 없다면 ... 서버에 IP를 요청한다.
			//-------------------------------------------------------
			else
			{
				if (!g_pRequestUserManager->HasRequestingUser( pName ))
				{
					if( g_pUserInformation->bKorean == true )
					{
						// 서버에 IP를 요청한다.
						CGRequestIP _CGRequestIP;
						_CGRequestIP.setName( pName );

						g_pSocket->sendPacket( &_CGRequestIP );			
		
					g_pRequestUserManager->AddRequestingUser( pName, RequestUserManager::REQUESTING_FOR_WHISPER );
					}
				}

				// IP를 받고 나서 message를 보내주기 위해서 저장해둔다.
				AddWhisperMessage( pName, pMessage, color );
			}
		}	
	}
	else	// 한국 버전이 아니면
	{
		// GameServer로 그냥 보낸다.
		CGWhisper _CGWhisper;
		_CGWhisper.setName( pName );
		
		_CGWhisper.setMessage( pMessage );
		_CGWhisper.setColor( color );
		
		g_pSocket->sendPacket( &_CGWhisper );
	}
}

//----------------------------------------------------------------------
// Add Message
//----------------------------------------------------------------------
void		
WhisperManager::AddWhisperMessage(const char* pName, const char* pMessage, DWORD color)
{
	Lock();

	WHISPER_INFO_MAP::iterator iInfo = m_WhisperInfos.find( std::string(pName) );

	//------------------------------------------------
	// 없다면 생성한다.
	//------------------------------------------------
	if (iInfo == m_WhisperInfos.end())
	{
		WhisperInfo* pInfo = new WhisperInfo;
		pInfo->Name		= std::string(pName);
		WHISPER_MESSAGE temp_message;
		temp_message.msg = std::string(pMessage);
		temp_message.color = color;
		pInfo->Messages.push_back( temp_message );

		m_WhisperInfos[pInfo->Name] = pInfo;
	}
	//------------------------------------------------
	// 이미 있다면 추가해둔다.
	//------------------------------------------------
	else
	{
		WhisperInfo* pInfo = iInfo->second;
		
		WHISPER_MESSAGE temp_message;
		temp_message.msg = std::string(pMessage);
		temp_message.color = color;
		pInfo->Messages.push_back( temp_message );
	}

	Unlock();
}

//----------------------------------------------------------------------
// Get Message
//----------------------------------------------------------------------
const std::list<WHISPER_MESSAGE>*
WhisperManager::GetWhisperMessages(const char* pName) const
{
	WHISPER_INFO_MAP::const_iterator iInfo = m_WhisperInfos.find( std::string(pName) );

	if (iInfo != m_WhisperInfos.end())
	{
		return &iInfo->second->Messages;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Remove Message
//----------------------------------------------------------------------
void		
WhisperManager::TryToSendWhisperMessage(const char* pName)
{
	Lock();

	WHISPER_INFO_MAP::const_iterator iInfo = m_WhisperInfos.find( std::string(pName) );

	if (iInfo != m_WhisperInfos.end())
	{
		// 시도 회수 증가
		iInfo->second->TryingCount++;		
	}
	else
	{
		// 귓속말을 보낼려고 시도하는 중이 아니라면 정보를 없앤다.
//		g_pRequestUserManager->RemoveRequestUserLater( pName );
	}

	Unlock();
}

//----------------------------------------------------------------------
// Remove Message
//----------------------------------------------------------------------
bool
WhisperManager::RemoveWhisperMessage(const char* pName)
{
	Lock();

	WHISPER_INFO_MAP::iterator iInfo = m_WhisperInfos.find( std::string(pName) );

	if (iInfo != m_WhisperInfos.end())
	{
		delete iInfo->second;

		m_WhisperInfos.erase( iInfo );

		Unlock();
		return true;
	}

	Unlock();
	return false;
}


//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
// 쌓여 있는 message들을 처리한다.
//----------------------------------------------------------------------
void
WhisperManager::Update()
{
	if (m_WhisperInfos.empty())
	{
		return;
	}	

	Lock();
	
	WHISPER_INFO_MAP::iterator iInfo = m_WhisperInfos.begin();

	while (iInfo != m_WhisperInfos.end())
	{
		WhisperInfo* pInfo = iInfo->second;

		//-------------------------------------------------------
		// 시도를 많이 한 경우는... 포기~해야 한다.
		// 그럴 확률은 거의 없지만..
		// 서버에서 잘못된 IP가 넘어오는 경우가 있다.
		//-------------------------------------------------------
		if (pInfo->TryingCount > 2)		// 2 _-_;
		{
			WHISPER_INFO_MAP::iterator iTemp = iInfo ++;

			// 귓속말 전해지지 않았다고 출력해주고
			//UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_WHISPER_FAILED].GetString(), NULL, 5 );

			// 서버로 보낸다.
			pInfo->SendToGameServer();

			// 사용자 정보를 접속 불가 상태로 설정한다.
			RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo(pInfo->Name.c_str());

			if (pUserInfo!=NULL)
			{
				pUserInfo->SetStatusUnable();
			}

			// 정보를 제거한다.
			delete pInfo;
			m_WhisperInfos.erase( iTemp );

			continue;
		}
		//-------------------------------------------------------
		// 귓속말 보내기 위한 접속 요청..
		//-------------------------------------------------------
		else
		{			
			const char* pName = pInfo->Name.c_str();

			//-------------------------------------------------------
			// 접속중이거나 접속 시도 중인 경우..
			//-------------------------------------------------------
			if (g_pRequestClientPlayerManager->HasTryingConnection(pName)
				|| g_pRequestClientPlayerManager->HasConnection(pName))			
			{			
			}
			//-------------------------------------------------------
			// 접속중이 아닌 경우
			//-------------------------------------------------------
			else
			{
				RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo(pName);
				
				//-------------------------------------------------------
				// 사용자 정보가 있다면 접속 시도를 한다.
				//-------------------------------------------------------
				if (pUserInfo!=NULL)
				{
					if (pUserInfo->IsStatusUnable())
					{
						pInfo->SendToGameServer();
						
						// 정보를 제거한다.
						WHISPER_INFO_MAP::iterator iTemp = iInfo ++;
						delete pInfo;
						m_WhisperInfos.erase( iTemp );
						continue;
					}
					else
					{
						g_pRequestClientPlayerManager->Connect(pUserInfo->IP.c_str(), pName, REQUEST_CLIENT_MODE_WHISPER);
					}
				}
				
				//-------------------------------------------------------
				// 사용자 정보가 없다면 ... 서버에 IP를 요청한다.
				//-------------------------------------------------------
				else
				{
					if (!g_pRequestUserManager->HasRequestingUser( pName ))
					{
						if( 0 && g_pUserInformation->bKorean == true )
						{
							// 서버에 IP를 요청한다.
							CGRequestIP _CGRequestIP;
							_CGRequestIP.setName( pName );

							g_pSocket->sendPacket( &_CGRequestIP );

						g_pRequestUserManager->AddRequestingUser( pName, RequestUserManager::REQUESTING_FOR_WHISPER );
						}
					}
				}
			}	
		}
		

		iInfo++;
	}

	Unlock();
}
