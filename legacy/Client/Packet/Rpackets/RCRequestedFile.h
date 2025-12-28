//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCRequestedFile.h 
// Written By  : elca
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __RC_REQUESTED_FILE_H__
#define __RC_REQUESTED_FILE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include "../types/RequestTypes.h"

class RCRequestedFileInfo
{
public :
	RCRequestedFileInfo();

   // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// get packet's body size
	PacketSize_t getPacketSize () const throw () { return szBYTE + 4 + szBYTE + m_Filename.size() + 4; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCRequestedFile"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

	// get/set RequestFileType
	REQUEST_FILE_TYPE getRequestFileType () const throw () { return m_RequestFileType; }
	void setRequestFileType ( REQUEST_FILE_TYPE RequestFileType ) throw () { m_RequestFileType = RequestFileType; }

	// get/set version
	DWORD getVersion () const throw () { return m_Version; }
	void setVersion ( DWORD version ) throw () { m_Version = version; }
	
	// get/set filename
	std::string getFilename () const throw () { return m_Filename; }
	void setFilename ( const std::string & filename ) throw () { m_Filename = filename; }

	// get/set filesize
	DWORD getFileSize () const throw () { return m_FileSize; }
	void setFileSize ( DWORD filesize ) throw () { m_FileSize = filesize; }	
	
private :

	REQUEST_FILE_TYPE		m_RequestFileType;
	DWORD					m_Version;
	std::string					m_Filename;		// 다운받아야 할 파일명
	DWORD					m_FileSize;		// 파일 크기
};

//////////////////////////////////////////////////////////////////////
//
// class RCRequestedFile;
//
// 게임 서버가 특정 플레이어의 Whisper 를 다른 플레이어들에게 브로드캐스트
// 할 때 전송하는 패킷이다. 내부에 캐릭터명과 Whisper 스트링을 데이타
// 필드로 가지고 있다.
//
//////////////////////////////////////////////////////////////////////

class RCRequestedFile : public Packet {

public :
	
	RCRequestedFile();
	~RCRequestedFile();

	void	Release() throw();

    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_RC_REQUESTED_FILE; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw ();

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCRequestedFile"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

	DWORD getListNum () const throw () { return m_FileInfos.size(); }
	
	void	addInfo(RCRequestedFileInfo* pInfo) throw()	{ m_FileInfos.push_back( pInfo ); }
	RCRequestedFileInfo* popInfo() throw()	{ if (m_FileInfos.empty()) return NULL; RCRequestedFileInfo* pInfo = m_FileInfos.front(); m_FileInfos.pop_front(); return pInfo; }
	
private :

	std::list<RCRequestedFileInfo*>	m_FileInfos;
};


//////////////////////////////////////////////////////////////////////
//
// class RCRequestedFileFactory;
//
// Factory for RCRequestedFile
//
//////////////////////////////////////////////////////////////////////

class RCRequestedFileFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new RCRequestedFile(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCRequestedFile"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_RC_REQUESTED_FILE; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static RCRequestedFilePacketMaxSize 를 정의, 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () { return 1 + 4 + ( 1 + 256 ) + 4; }

};


//////////////////////////////////////////////////////////////////////
//
// class RCRequestedFileHandler;
//
//////////////////////////////////////////////////////////////////////

class RCRequestedFileHandler {
	
public :
	
	// execute packet's handler
	static void execute ( RCRequestedFile * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );

};

#endif
