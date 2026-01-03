//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCRequestedFile.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "RCRequestedFile.h"


RCRequestedFileInfo::RCRequestedFileInfo()
{
	m_RequestFileType = REQUEST_FILE_NULL;

	m_Version = 0;	
	m_FileSize = 0;
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void RCRequestedFileInfo::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// m_RequestFileType
	BYTE fileType;
	iStream.read(fileType);
	m_RequestFileType = (REQUEST_FILE_TYPE)fileType;

	// m_Version
	iStream.read( m_Version );

	// m_Filename
	BYTE szFilename;

	iStream.read( szFilename );

	if ( szFilename == 0 )
		throw InvalidProtocolException("szFilename == 0");

	if ( szFilename > 255 )
		throw InvalidProtocolException("too large Filename length");

	iStream.read( m_Filename, szFilename );

	// m_FileSize
	iStream.read( m_FileSize );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void RCRequestedFileInfo::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// m_RequestFileType
	BYTE fileType = (BYTE)m_RequestFileType;
	oStream.write( fileType );
	
	// m_Version
	oStream.write( m_Version );

	// m_Filename
	BYTE szFilename = m_Filename.size();

	if ( szFilename == 0 )
		throw InvalidProtocolException("szFilename == 0");

	if ( szFilename > 255 )
		throw InvalidProtocolException("too large Filename length");

	oStream.write( szFilename );

	oStream.write( m_Filename );
		
	// m_FileSize
	oStream.write( m_FileSize );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get packet's debug string
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string RCRequestedFileInfo::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	
	msg << "RCRequestedFile("
		<< "FileType: " << (int)m_RequestFileType
		<< ", Version: " << m_Version
		<< ", Filename: " << m_Filename
		<< ", Filesize: " << m_FileSize
		<< ") ";
	
	return msg.toString();
		
	__END_CATCH
}
#endif

RCRequestedFile::RCRequestedFile()
{
}

RCRequestedFile::~RCRequestedFile()
{
	Release();
}

void
RCRequestedFile::Release()
	throw()
{
	while (!m_FileInfos.empty())
	{
		RCRequestedFileInfo* pInfo = m_FileInfos.front();

		delete pInfo;

		m_FileInfos.pop_front();
	}
}

//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void RCRequestedFile::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	RCRequestedFileHandler::execute( this , pPlayer );

	__END_CATCH
}

PacketSize_t 
RCRequestedFile::getPacketSize () const throw ()
{
	PacketSize_t ps = szBYTE;	// for listNum

	std::list<RCRequestedFileInfo*>::const_iterator iInfo = m_FileInfos.begin();

	while (iInfo != m_FileInfos.end())
	{
		RCRequestedFileInfo* pInfo = *iInfo;

		ps += pInfo->getPacketSize();

		iInfo ++;
	}

	return ps;
}

// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
void 
RCRequestedFile::read ( SocketInputStream & iStream ) throw ( ProtocolException , Error )
{
	Release();

	BYTE listNum;

	iStream.read( listNum );

	int intListNum = (int)listNum;
	for (int i=0; i<intListNum; i++)
	{	
		RCRequestedFileInfo* pInfo = new RCRequestedFileInfo;
		
		pInfo->read( iStream );

		m_FileInfos.push_back( pInfo );
	}
}
		
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
void 
RCRequestedFile::write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error )
{
	BYTE listNum = m_FileInfos.size();

	oStream.write( listNum );

	std::list<RCRequestedFileInfo*>::const_iterator iInfo = m_FileInfos.begin();

	while (iInfo != m_FileInfos.end())
	{
		RCRequestedFileInfo* pInfo = *iInfo;

		pInfo->write(oStream);

		iInfo ++;
	}
}

#ifdef __DEBUG_OUTPUT__
std::string 
RCRequestedFile::toString () const throw ()
{
	StringStream msg;

	std::list<RCRequestedFileInfo*>::const_iterator iInfo = m_FileInfos.begin();

	while (iInfo != m_FileInfos.end())
	{
		RCRequestedFileInfo* pInfo = *iInfo;

		msg << pInfo->toString();

		iInfo ++;
	}

	return msg.toString();
}


#endif