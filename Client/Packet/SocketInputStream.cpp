//#define __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__

//////////////////////////////////////////////////////////////////////
// 
// SocketInputStream.cpp
// 
// by Reiot
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Client_PCH.h"
#include "SocketInputStream.h"
#include <errno.h>
#include "Assert.h"
#include "Packet.h"
#include "MinTr.h"
#include <cstdio>

#if __LINUX__
	#include <sys/ioctl.h>
#elif __WINDOWS__
	#include <winsock.h>
#endif

#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__

DWORD	g_dwReceivedSizeCheckTime=0;
DWORD	g_dwReceiveSize=0;

#endif

extern uint receiveWithDebug (Socket *pSock, void * buf , uint len);

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
SocketInputStream::SocketInputStream ( Socket * sock , uint BufferLen ) 
	throw ( Error )
: m_pSocket(sock), m_Buffer(NULL), m_BufferLen(BufferLen), m_Head(0), m_Tail(0)
{
	__BEGIN_TRY
		
	Assert( m_pSocket != NULL );
	Assert( m_BufferLen > 0 );
	
	m_Buffer = new char[ m_BufferLen ];

	//add by viva 2008-12-31
	m_EncryptKey = 0;
	m_HashTable = NULL;
	//end
		
	__END_CATCH
}
	

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
SocketInputStream::~SocketInputStream () 
	throw ( Error )
{
	__BEGIN_TRY
		
	if ( m_Buffer != NULL ) {
		delete [] m_Buffer;
		m_Buffer = NULL;
	}
		
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
//
// read data from input buffer
//
//////////////////////////////////////////////////////////////////////
uint SocketInputStream::read ( char * buf , uint len ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	Assert( buf != NULL );	
	
	if ( len == 0 )
		throw InvalidProtocolException("len==0");
	
	// ��û�� ��ŭ�� ����Ÿ�� ���۳��� �������� ���� ��� ���ܸ� ������.
	// ���� ��� read �� peek() �� üũ�� �� ȣ��ȴٸ�, �Ʒ� if-throw �� 
	// �ߺ��� ���� �ִ�. ����, �ڸ�Ʈ�� ó���ص� �����ϴ�.
	// �� �Ʒ� �ڵ带 �ڸ�Ʈó���ϸ�, �ٷ� �Ʒ��� if-else �� 'if'-'else if'-'else'
	// �� ��������� �Ѵ�.
	if ( len > length() )
		throw InsufficientDataException( len - length() );
	
	if ( m_Head < m_Tail ) {	// normal order

		//
        //    H   T
        // 0123456789
        // ...abcd...
        //

		memcpy( buf , &m_Buffer[m_Head] , len );

	} else {					// reversed order ( m_Head > m_Tail )
		
        //
        //     T  H
        // 0123456789
        // abcd...efg
        //
	 
		uint rightLen = m_BufferLen - m_Head;
		if ( len <= rightLen ) {
			memcpy( buf , &m_Buffer[m_Head] , len );
		} else {
			memcpy( buf , &m_Buffer[m_Head] , rightLen );
			memcpy( &buf[rightLen] , m_Buffer , len - rightLen );
		}

	}

	m_Head = ( m_Head + len ) % m_BufferLen;
/*
	#ifdef __DEBUG_OUTPUT__
		if (len > 0) {
			FILE* fp = fopen("read.log", "a");
			for (int i=0; i< len ; i++) {
				fprintf(fp, " %02x", (unsigned char)buf[i]);
			}
			fclose(fp);
		}
	#endif
*/ 	
	return len;
		
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
// read data from input buffer
//////////////////////////////////////////////////////////////////////
uint SocketInputStream::read ( std::string & str , uint len ) 
throw ( ProtocolException , Error ) {
	__BEGIN_TRY
		
	if ( len == 0 )
		throw InvalidProtocolException("len==0");
	
	// ��û�� ��ŭ�� ����Ÿ�� ���۳��� �������� ���� ��� ���ܸ� ������.
	// ���� ��� read �� peek() �� üũ�� �� ȣ��ȴٸ�, �Ʒ� if-throw �� 
	// �ߺ��� ���� �ִ�. ����, �ڸ�Ʈ�� ó���ص� �����ϴ�.
	// �� �Ʒ� �ڵ带 �ڸ�Ʈó���ϸ�, �ٷ� �Ʒ��� if-else �� if-else if-else
	// �� ��������� �Ѵ�.
	if ( len > length() )
		throw InsufficientDataException( len - length() );
	
	// ��Ʈ�����ٰ� len ��ŭ ������ �̸� �Ҵ��Ѵ�.
	str.reserve( len );

	if ( m_Head < m_Tail ) {	// normal order

		//
        //    H   T
        // 0123456789
        // ...abcd...
        //

		str.assign( &m_Buffer[m_Head] , len );

	} else { 					// reversed order ( m_Head > m_Tail )
		
        //
        //     T  H
        // 0123456789
        // abcd...efg
        //
	 
		uint rightLen = m_BufferLen - m_Head;
		if ( len <= rightLen ) {
			str.assign( &m_Buffer[m_Head] , len );
		} else {
			str.assign( &m_Buffer[m_Head] , rightLen );
			str.append( m_Buffer , len - rightLen );
		}
	}
	
	m_Head = ( m_Head + len ) % m_BufferLen;

	/*
	#ifdef __DEBUG_OUTPUT__
		if (len > 0) {
			FILE* fp = fopen("read.log", "a");
			const char *buf = str.c_str();
			for (int i=0; i< len ; i++) {
				fprintf(fp, " %02x", (unsigned char)buf[i]);
			}
			fclose(fp);
		}
	#endif
		*/

	return len;
		
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
// read packet from input buffer
//////////////////////////////////////////////////////////////////////
void SocketInputStream::read ( Packet * pPacket ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// �̹� ���� �������� ID, Size�� �а� ID�� �´� ��Ŷ��ü�� ����
	// �Ķ���ͷ� �Ѱ����Ƿ� ID �� skip �Ѵ�. Size�� ��ü�� ���̳ʸ� 
	// �̹����� ��� �����ߴ����� üũ�һ�, ��ü�� �ʱ�ȭ�ʹ� �����ϱ�
	// ������ skip �Ѵ�.
	skip( szPacketHeader );

	// ���� ���� ��Ŷ Ŭ������ ���ǵ� �޽�带 �����ϸ�, ���ο���
	// �˾Ƽ� �� �ʱ�ȭ�ϰ� �ȴ�.
	// �� � ��Ŷ�� read()���� Ʋ���� �Ǹ� �� ������ ��� ��Ŷ��
	// �Ľ��� �Ұ����ϰ� �ȴ�. ����, ��Ŷ Ŭ������ �������� ��
	// �����ؾ� �Ѵ�.
	pPacket->read( *this );

	//  By tiancaiamao
	#ifdef __DEBUG_OUTPUT__
	ofstream ofile("lalala.log", ios::out | ios::app);
		ofile << "SocketInputStream read packet: ";
		ofile << pPacket->toString().c_str() << endl;
		ofile.close();
	#endif

	
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// peek data from buffer
//////////////////////////////////////////////////////////////////////
bool SocketInputStream::peek ( char * buf , uint len )
{
//	__BEGIN_TRY
			
	Assert( buf != NULL );	

	if ( len == 0 )
		throw InvalidProtocolException("len==0");
	
	// ��û�� ũ�⺸�� ������ ����Ÿ�� ���� ���, ���ܸ� ������.
	if ( len > length() ) {
		// throw InsufficientDataException( len - length() );
		return false;
	}

	// buf �� ����� �ϵ�, m_Head �� ��ȭ��Ű�� �ʴ´�.
	if ( m_Head < m_Tail ) {	// normal order

		//
        //    H   T
        // 0123456789
        // ...abcd...
        //

		memcpy( buf , &m_Buffer[m_Head] , len );

	} else { 					// reversed order ( m_Head > m_Tail )
		
        //
        //     T  H
        // 0123456789
        // abcd...efg
        //
	 
		uint rightLen = m_BufferLen - m_Head;
		if ( len <= rightLen ) {
			memcpy( &buf[0]        , &m_Buffer[m_Head] , len );
		} else {
			memcpy( &buf[0]        , &m_Buffer[m_Head] , rightLen );
			memcpy( &buf[rightLen] , &m_Buffer[0]      , len - rightLen );
		}
	}

/*
#ifdef __DEBUG_OUTPUT__
	if (len == szPacketHeader) {
				FILE* fp = fopen("peek.log", "a");
				fprintf(fp, "(pos=%d) ", m_Head);
				for (int i=0; i<len; i++) {
					fprintf(fp, " %02x", (unsigned char)(buf[i]));
				}
				fprintf(fp, "\r\n");
				fclose(fp);
	}
#endif
*/
	
	return true;
//	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
//
// skip data from buffer
//
// read(N) == peek(N) + skip(N)
//
//////////////////////////////////////////////////////////////////////
void SocketInputStream::skip ( uint len ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	if ( len == 0 )
		throw InvalidProtocolException("len==0");
	
	if ( len > length() )
		throw InsufficientDataException( len - length() );
	
	// m_Head �� ������Ų��.

	uint pos = m_Head;
	m_Head = ( m_Head + len ) % m_BufferLen;

/*
	#ifdef __DEBUG_OUTPUT__
		if (len > 0) {
			FILE* fp = fopen("read.log", "a");
			fprintf(fp, "\r\n pos (%d) ", pos);
			for (uint i=0; i< len; i++) {
				pos = (pos + i) % m_BufferLen;
				fprintf(fp, " %02x", (unsigned char)(m_Buffer[pos]));
			}
			fclose(fp);
		}
	#endif
*/

	__END_CATCH
}
	

//////////////////////////////////////////////////////////////////////
//
// fill buffer from socket
//
// ����Ÿ�� ���� ��� 0 �� �����Ѵ�.
// �׷���, NonBlocking �� ��� NonBlockingIOException �� ������.
//
// *NOTES*
//
// ���� ���� ����� �Լ����ٰ� char buf[N]; �� ������ �Ŀ� ����ٰ�
// receive() ������� ������ ��, ���ۿ� �����ϴ� ���̴�. �׷���, ��
// ����� ���簡 2ȸ �߻��ϱ� ������ ����Ǿ�� �ϰڴ�. 
//
// ����, ��Ʈ���� ���� ���ۿ��ٰ� ���� ��� �ϴµ�, �̶� ���� ���۰�
// circular �̹Ƿ�, normal order �� ��쿡�� 2ȸ �̻� receive()�� ȣ���
// Ȯ���� �ְ� �ȴ�.
//
// �̶� ù��° receive()���� ������ receive ������ ����Ÿ�� �� �о������, 
// ���ÿ� ������ ���� ������ ��� �ι�° receive() ���� NonBlockingIOException 
// �� �߻��ϰ� �ȴ�. (���� �̷� ���� �幰��. ������ ���� �Ǿտ� ���ļ� 
// ����Ÿ�� ���� ���� ���� �߻��Ѵ� ġ����.. ) 
//
// ��. �̷� ��� fill()�� �ܺη� NonBlockingIOException �� ������ �ϴ°�?
// �ƴϸ� ���ݱ��� ���� ����Ÿ ũ�⸦ �����ؾ� �ϴ°�? ������ ���ڴ�. ^^;
// �� �� ����ξ�~~~ ������������~
//
//////////////////////////////////////////////////////////////////////
uint SocketInputStream::fill ()
{
	__BEGIN_TRY
		
	uint nFilled = 0;		// ���ۿ� ä������ ũ��
	uint nReceived;			// Socket::receive()�� �ѹ� �о���� ũ��
	uint nFree;				// ������ �� ������ ũ��

#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
	if( g_dwReceivedSizeCheckTime == 0 )
	{
		g_dwReceivedSizeCheckTime = timeGetTime();
		g_dwReceiveSize = 0;
	}

	if( timeGetTime() - g_dwReceivedSizeCheckTime >= 1000 )
	{
//		_MinTrace("Packet Received Size Per Second : %d %d\n", g_dwReceiveSize,timeGetTime() - g_dwReceivedSizeCheckTime );
		g_dwReceivedSizeCheckTime = timeGetTime();
		g_dwReceiveSize = 0;
	}
#endif
	

	if ( m_Head <= m_Tail ) {	// normal order
								// m_Head == m_Tail �� ���� ���۰� �� ���¸� ��Ÿ����.

		// �ϴ� ������ ������ �� �������� ä���.
		if ( m_Head == 0 ) {

			//
			// H   T
			// 0123456789
			// abcd......
			//
			
			// ������ ����, m_Head == 0 �� ���, m_Tail �� ������ ���� ���Ƽ� �ٽ� 0 ����
			// �ǰ� �Ǹ� buffer empty �� ���� ���°� �ȴٴ� ���̴�. ����, m_Head �� üũ
			// �ؼ� 0 �� ��� m_Tail �� ���ؼ� ������ �� ������ ĭ�� ����־� �ϰڴ�. ^^

			nFree = m_BufferLen - m_Tail - 1;
			nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , nFree );

			// by tiancaiamao
			if (nReceived==0) return 0;
			//add by viva
			if(nReceived>0)
				m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
			//end

			m_Tail += nReceived;
			nFilled += nReceived;

#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
			g_dwReceiveSize += nReceived;
#endif
			
			if ( nReceived == nFree ) {

				// ������ receive ���ۿ� ����Ÿ�� �������� ���ɼ��� �ִ�.
				// �׷���, �Է� ���ۿ� ���� ������ �����Ƿ� ����Ÿ�� ���� 
				// �ִٸ� ���۸� �������Ѿ� �Ѵ�.
				uint available = m_pSocket->available();
				if ( available > 0 ) {
					resize( available + 1 );
					// resize �Ǹ�, ������ ����Ÿ�� ���ĵǹǷ� m_Tail ���� ���� �ȴ�.
					nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , available );
					
					//add by viva
					if(nReceived>0)
						m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
					//end
					m_Tail += nReceived;
					nFilled += nReceived;
#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
					g_dwReceiveSize += nReceived;
#endif
				}
			}

		} else {	// m_Head != 0
			
			//
			//    H   T
			// 0123456789
			// ...abcd...
			//

			// �� ���, m_Tail �� ������ �������� �Ѿ�� �����ϴ�.
			nFree = m_BufferLen - m_Tail;
			nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , nFree );

			// by tiancaiamao Nonblock exception
			if (nReceived==0) return 0;
			//add by viva
			if(nReceived>0)
				m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
			//end
			m_Tail = ( m_Tail + nReceived ) % m_BufferLen;
			nFilled += nReceived;
#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
			g_dwReceiveSize += nReceived;
#endif

			if ( nReceived == nFree ) {
				
				Assert( m_Tail == 0 );

				// ������ receive ���ۿ� ����Ÿ�� �� �������� ���ɼ��� �ִ�.
				// ����, �Է� ������ ���� ���� ������ ����Ÿ�� ������ �Ѵ�.
				// �� �̶����� m_Head == m_Tail �̸� empty �� �ǹǷ�,
				// -1 ���̵��� �Ѵ�.
				nFree = m_Head - 1;
				nReceived = receiveWithDebug(m_pSocket, &m_Buffer[0] , nFree );
				//add by viva
				if(nReceived>0)
					m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
				//end
				m_Tail += nReceived;
				nFilled += nReceived;
				
				if ( nReceived == nFree ) {	// buffer is full

					// ���۰� ���� �� ������ ���, ������ receive ���ۿ� ����Ÿ�� �� 
					// �������� ���ɼ��� �ִ�. ����, �о�� �� ������ ���۸� ������Ų��.
					uint available = m_pSocket->available();
					if ( available > 0 ) {
						resize( available + 1 );
						// resize �Ǹ�, ������ ����Ÿ�� ���ĵǹǷ� m_Tail ���� ���� �ȴ�.
						nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , available );

						// by tiancaiamao
						if (nReceived==0) return 0;
						//add by viva
						if(nReceived>0)
							m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
						//end
						m_Tail += nReceived;
						nFilled += nReceived;
#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
						g_dwReceiveSize += nReceived;
#endif
					}
				}
			}
		}

	} else {	// reversed order ( m_Head > m_Tail )
		
		//
        //     T  H
        // 0123456789
        // abcd...efg
        //
		
		nFree = m_Head - m_Tail - 1;
		nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , nFree );
		// by tiancaiamao
		if(nReceived==0) return 0;
		//add by viva
		if(nReceived>0)
			m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
		//end
		m_Tail += nReceived;
		nFilled += nReceived;
#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
		g_dwReceiveSize += nReceived;
#endif
		
		if ( nReceived == nFree ) {		// buffer is full

			// �� ���, ������ receive ���ۿ� ����Ÿ�� �� �������� 
			// ���ɼ��� �ִ�. ����, �о�� �� ������ ���۸� ������Ų��.
			uint available = m_pSocket->available();
			if ( available > 0 ) {
				resize( available + 1 );
				// resize �Ǹ�, ������ ����Ÿ�� ���ĵǹǷ� m_Tail ���� ���� �ȴ�.
				nReceived = receiveWithDebug(m_pSocket, &m_Buffer[m_Tail] , available );
				if(nReceived==0) return 0;
				//add by viva
				if(nReceived>0)
					m_EncryptKey = EncryptData(m_EncryptKey, &m_Buffer[m_Tail], nReceived);
				//end
				m_Tail += nReceived;
				nFilled += nReceived;
#ifdef __TEST_PACKET_RECEIVED_SIZE_PER_SECOND__
				g_dwReceiveSize += nReceived;
#endif
			}
		}

	}

	return nFilled;

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// fill buffer from socket
//
// ����Ʈ ��� - ���� �� ������. 0 -;
//
//////////////////////////////////////////////////////////////////////
uint SocketInputStream::fill_RAW ()
	 throw ( IOException , Error )
{
	__BEGIN_TRY
		
#if __LINUX__
	uint nfree = m_BufferLen - m_Tail - 1;	

	int nread = recv( m_pSocket->getSOCKET() , &m_Buffer[m_Tail] , nfree , 0 );

	if ( nread < 0 ) {
		if ( errno == EWOULDBLOCK ) {
			// NonBlockingIOException
			nread = 0;
		} else if ( errno == ECONNRESET ) {
			// ConnectException
			throw ConnectException();
		} else {
			// Error
			throw UnknownError(strerror(errno),errno);
		}
	} else if ( nread == 0 ) {
		// EOFException
		throw EOFException();
	}
	
	m_Tail += nread;
	
	if ( nread == (int)nfree ) {
		// ����Ÿ�� �� ���� ���ɼ��� �ִ�.
		uint more = 0;
		int result = ioctl( m_pSocket->getSOCKET() , FIONREAD , &more );
		if ( result < 0 )
			throw UnknownError(strerror(errno),errno);
		
		if ( more > 0 ) {

			// ���۸� ������Ų��.
			resize( more + 1 );

			// ���۸� ä���.
			nread = recv( m_pSocket->getSOCKET() , &m_Buffer[m_Tail] , more , 0 );
			
			// ������ more ��ŭ �о�� �Ѵ�. �׷��� ���� ��� ������.
			Assert( (int)more == nread );
			
			nread += more;
		}		
	}
	
	return nread;

#elif __WINDOWS__
	throw UnsupportedError();
#endif

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// resize buffer
//////////////////////////////////////////////////////////////////////
void SocketInputStream::resize ( int size )
	 throw ( IOException , Error )
{
	__BEGIN_TRY
		
	Assert( size != 0 );
	
	uint newBufferLen = m_BufferLen + size;
	uint len = length();
	
	if ( size < 0 ) {
		
		// ���� ũ�⸦ ���̷��µ� ���ۿ� ����ִ� ����Ÿ�� 
		// �� ����Ƴ� ��� 
		if ( newBufferLen < 0 || newBufferLen < len )
			throw IOException("new buffer is too small!");
		
	} 
	
	// �� ���۸� �Ҵ�޴´�.
	char * newBuffer = new char[ newBufferLen ];
		
	// ���� ������ ������ �����Ѵ�.
	if ( m_Head < m_Tail ) {

		//
		//    H   T
		// 0123456789
		// ...abcd...
		//

		memcpy( newBuffer , &m_Buffer[m_Head] , m_Tail - m_Head );

	} else if ( m_Head > m_Tail ) {

		//
        //     T  H
        // 0123456789
        // abcd...efg
        //
		
		memcpy( newBuffer , &m_Buffer[m_Head] , m_BufferLen - m_Head );
		memcpy( &newBuffer[ m_BufferLen - m_Head ] , m_Buffer , m_Tail );

	}
		
	// ���� ���۸� �����Ѵ�.
	delete [] m_Buffer;
		
	// ���� �� ���� ũ�⸦ �缳���Ѵ�.
	m_Buffer = newBuffer;
	m_BufferLen = newBufferLen;
	m_Head = 0;
	m_Tail = len;	// m_Tail �� ����ִ� ����Ÿ�� ���̿� ����.

	#ifdef __DEBUG_OUTPUT__
		ofstream ofile("buffer_resized.log",ios::app);
		ofile << "SocketInputStream resized " << size << " bytes!" << endl;
		ofile.close();
	#endif

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// get data's size in buffer
//
// NOTES
//
//       H   T           T  H
//    0123456789     0123456789
//    ...abcd...     abcd...efg
//
//    7 - 3 = 4      10 - ( 7 - 4 ) = 7
//
// CAUTION
//
//    m_Tail �� �� ĭ�� ����Ű�� �ִٴ� �Ϳ� �����϶�. 
//    ������ ũ�Ⱑ m_BufferLen ��� ���� �� ť�� �� 
//    �� �ִ� ����Ÿ�� ( m_BufferLen - 1 ) �� �ȴ�.
//
//////////////////////////////////////////////////////////////////////
uint SocketInputStream::length () const
     throw ()
{
	__BEGIN_TRY

	if ( m_Head < m_Tail )
		return m_Tail - m_Head;
	
	else if ( m_Head > m_Tail ) 
		return m_BufferLen - m_Head + m_Tail;
	
	return 0;

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get debug string
//////////////////////////////////////////////////////////////////////
std::string SocketInputStream::toString () const
	throw ()
{
	StringStream msg;

	msg << "SocketInputStream("
		<< "BufferLen:" << m_BufferLen
		<< ",Head:" << m_Head
		<< ",Tail:" << m_Tail
		<< ")";

	return msg.toString();
}
//add by viva 2008-12-31
WORD SocketInputStream::EncryptData(WORD EncryptKey, char* buf, int len)
	throw()
{
	return EncryptKey;

	for(int i = 0; i<len; i++)
		*(buf + i) ^= 0xCC;
	
	if(m_HashTable == NULL) return EncryptKey;

	for(i = 0; i<len; i++)
	{
		*(buf + i) ^= m_HashTable[EncryptKey];
		if(++EncryptKey == 512)	EncryptKey = 0;
	}
	return EncryptKey;
}
//end

// add by tiancaiamao
uint receiveWithDebug (Socket *pSock, void * buf , uint len) {
	uint ret = pSock->receive(buf,len); 

	#ifdef __DEBUG_OUTPUT__
		if (ret > 0) {
			FILE* fp = fopen("fill.log", "a");
			for (int i=0; i<ret; i++) {
				fprintf(fp, " %02x", (unsigned char)(((char*)buf)[i]));
			}
			fprintf(fp, "\r\n");
			fclose(fp);
		}
	#endif

	return ret;
}
