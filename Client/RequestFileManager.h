//---------------------------------------------------------------------------
// RequestFileManager.h 
//---------------------------------------------------------------------------
// client 사이의 file주고받기를 담당하는 부분이다.
//
// 내가 요청하는거는 받고..
// 남이 요청하는거는 주고..
//
// 게임이 계속 돌아가도록 하면서 file을 주고받기 위해서
// 다른 thread로 돌아가야 한다(적절한 priority는 뭘까?)...고 생각했는데..
// 매 loop마다 조금조금씩(얼마나?) 보내주는게 나을지도 모르겠다.
//
// 동시에 너무 많은 file들을 주고받으면 부하가 많이 걸리므로
// 한번에 하나만 주고받게(주고/받는거 따로?) 하는게 어떨까..싶었는데 -_-;
// 기다리는 시간이 넘 길지 않을까...싶어서.. 걍 다 보내기로 하겠다.
//
//
// REQUEST_RECEIVE_MAP : 내가 요청한 file들의 정보
//
//    - 남한테 요청할려는 file의 정보를 여기 넣어두고 요청하면
//      그 file을 남이 보내줄때, 여기서 정보를 참고해서 받을 수 있다.
//
//	  - RequestClientPlayer::processCommand()에서
//      file을 받는 중이라면 ReceiveMyRequest(RequestClientPlayer*)를 해주면 된다.
//
//
// REQUESTED_SEND_MAP : 남이 요청하는 file들의 정보
//    - 누가 어떤 file을 요청하면.. 일단 여기 정보를 넣어두고
//		SendOtherRequest(RequestServerPlayer*)
//
// 두 개의 class로 분리하는게 좋았지 싶은데... 할... - -;
//---------------------------------------------------------------------------

#ifndef __REQUEST_FILE_MANAGER_H__
#define __REQUEST_FILE_MANAGER_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include "types\RequestTypes.h"
class RequestClientPlayer;
class RequestServerPlayer;
class ConnectException;
class RCRequestedFile;

//---------------------------------------------------------------------------
// REQUEST_FILE_MODE
//---------------------------------------------------------------------------
enum REQUEST_FILE_MODE
{
	REQUEST_FILE_MODE_BEFORE,
	REQUEST_FILE_MODE_SEND,
	REQUEST_FILE_MODE_RECEIVE,
	REQUEST_FILE_MODE_AFTER,
};

//---------------------------------------------------------------------------
// SendFileInfo
//---------------------------------------------------------------------------
// 남이 나에게 요청한 것
//---------------------------------------------------------------------------
class SendFileInfo
{
	private :
		REQUEST_FILE_MODE		m_Mode;

		// 보내기 전에
		std::string				m_Filename;		// 보내주는 file이름		
		REQUEST_FILE_TYPE		m_FileType;		// 어떤 file인가?
		
		// 보내는 동안		
		ifstream			m_FileStream;	// 보내주는 Filename을 open한 것		
		DWORD					m_FileSizeLeft;

	public :
		SendFileInfo(const char* pFilename, REQUEST_FILE_TYPE fileType);
		~SendFileInfo();

		void		StartSend();
		bool		IsSendMode() const	{ return m_Mode==REQUEST_FILE_MODE_SEND; }
		DWORD		Send(char* pBuffer);		// Get()이 더 어울리는데.. - -;
		void		SendBack(DWORD nBack);
		void		EndSend();

		// Get
		REQUEST_FILE_MODE	GetMode() const			{ return m_Mode; }
		REQUEST_FILE_TYPE	GetFileType() const		{ return m_FileType; }
		const std::string&	GetFilename() const		{ return m_Filename; }
		DWORD			GetFileSizeLeft() const	{ return m_FileSizeLeft; }
};

//---------------------------------------------------------------------------
// ReceiveFileInfo
//---------------------------------------------------------------------------
class ReceiveFileInfo
{
	public :
		REQUEST_FILE_MODE		m_Mode;

		std::string				m_Filename;		// 요청한 filename
		REQUEST_FILE_TYPE		m_FileType;		// 어디에 쓸 file인가?
		
		// 받는 동안
		std::string				m_FilenameTemp;	// 요청한 file을 받아서 잠시 저장해둘 filename
		ofstream				m_FileStream;		// FilenameTemp를 open한 것..		
		DWORD					m_FileSizeLeft;

	public :
		ReceiveFileInfo(const char* pFilename, REQUEST_FILE_TYPE fileType);
		~ReceiveFileInfo();
		
		void		StartReceive(DWORD filesize);
		bool		IsReceiveMode() const	{ return m_Mode==REQUEST_FILE_MODE_RECEIVE; }
		void		Receive(const char* pBuffer, DWORD size);
		void		EndReceive(const std::string& requestUser);

		// Get
		REQUEST_FILE_MODE	GetMode() const			{ return m_Mode; }
		REQUEST_FILE_TYPE	GetFileType() const		{ return m_FileType; }
		const std::string&	GetFilename() const		{ return m_Filename; }
		DWORD			GetFileSizeLeft() const	{ return m_FileSizeLeft; }
};

//---------------------------------------------------------------------------
// RequestReceiveInfo
//---------------------------------------------------------------------------
// 내가 남에게 요청한 것
//---------------------------------------------------------------------------
class RequestReceiveInfo
{
	private :
		// 받기 전에
		std::string				m_RequestUser;	// file을 나에게 보내줄 사람

		std::list<ReceiveFileInfo*>	m_FileInfos;

	public :
		RequestReceiveInfo(const char* pRequestUser);
		~RequestReceiveInfo();

		void				AddReceiveFileInfo(ReceiveFileInfo* pInfo)	{ m_FileInfos.push_back( pInfo ); }

		const std::string&	GetRequestUser() const	{ return m_RequestUser; }

		DWORD				GetSize() const		{ return m_FileInfos.size(); }
		ReceiveFileInfo*	GetFront()			{ return (m_FileInfos.empty()? NULL : m_FileInfos.front()); }
		void				DeleteFront()		{ if (!m_FileInfos.empty()) { delete m_FileInfos.front(); m_FileInfos.pop_front(); } }		
		bool				IsEnd()	const		{ return m_FileInfos.empty(); }

};

//---------------------------------------------------------------------------
// RequestSendInfo
//---------------------------------------------------------------------------
// 남이 나에게 요청한 것
//---------------------------------------------------------------------------
class RequestSendInfo
{
	private :
		std::string				m_RequestUser;	// 내가 file을 보내줄 사람

		std::list<SendFileInfo*>	m_FileInfos;

	public :
		RequestSendInfo(const char* pRequestUser);
		~RequestSendInfo();
		
		void				AddSendFileInfo(SendFileInfo* pInfo)	{ m_FileInfos.push_back( pInfo ); }

		const std::string&	GetRequestUser() const	{ return m_RequestUser; }

		DWORD				GetSize() const		{ return m_FileInfos.size(); }
		SendFileInfo*		GetFront()			{ return (m_FileInfos.empty()? NULL : m_FileInfos.front()); }
		void				DeleteFront()		{ if (!m_FileInfos.empty()) { delete m_FileInfos.front(); m_FileInfos.pop_front(); } }		
		bool				IsEnd()	const		{ return m_FileInfos.empty(); }

		void				MakeRCRequestedFilePacket(RCRequestedFile& packet) const;
};


//---------------------------------------------------------------------------
// RequestFileManager
//---------------------------------------------------------------------------
class RequestFileManager {
	public :
		//-----------------------------------------------------------------
		// 내가 요청한 것 : < 요청한사람, 내가 요청한file정보 >
		//-----------------------------------------------------------------
		typedef std::map<std::string, RequestReceiveInfo*>		REQUEST_RECEIVE_MAP;

		//-----------------------------------------------------------------
		// 다른 사람이 요청한 것 : < 요청한사람, 다른 사람이 요청한file정보 >
		//-----------------------------------------------------------------
		typedef std::map<std::string, RequestSendInfo*>			REQUEST_SEND_MAP;

	public :
		RequestFileManager();
		~RequestFileManager();

		//--------------------------------------------------------------
		// Release
		//--------------------------------------------------------------
		void			Release();

		//--------------------------------------------------------------
		// MyRequest - 내가 요청한 file처리
		//--------------------------------------------------------------
		bool			AddMyRequest(RequestReceiveInfo* pInfo);
		bool			RemoveMyRequest(const std::string& name);
		bool			HasMyRequest(const std::string& name) const;
		bool			ReceiveMyRequest(const std::string& name, RequestClientPlayer* pRequestClientPlayer) throw (ConnectException);

		//--------------------------------------------------------------
		// OtherRequest - 다른 사람이 요청한 file처리
		//--------------------------------------------------------------
		bool			AddOtherRequest(RequestSendInfo* pInfo);
		bool			RemoveOtherRequest(const std::string& name);
		bool			HasOtherRequest(const std::string& name) const;
		bool			SendOtherRequest(const std::string& name, RequestServerPlayer* pRequestServerPlayer);

		//--------------------------------------------------------------
		// Update
		//--------------------------------------------------------------
		void			Update();

	protected :
		REQUEST_RECEIVE_MAP			m_MyRequests;		// 내가 요청한 file들
		REQUEST_SEND_MAP		m_OtherRequests;	// 다른 사람이 요청한 file들
};

extern RequestFileManager* g_pRequestFileManager;

#endif


