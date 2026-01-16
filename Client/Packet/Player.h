//////////////////////////////////////////////////////////////////////
// 
// Filename    : Player.h 
// Written by  : reiot@ewestsoft.com
// Description : °ÔÀÓ¼­¹ö/·Î±×ÀÎ¼­¹ö/Å×½ºÆ® Å¬¶óÀÌ¾ðÆ®¿ë ÇÃ·¹ÀÌ¾î Å¬·¡½º
// 
//////////////////////////////////////////////////////////////////////

#ifndef __PLAYER_H__
#define __PLAYER_H__

// include files
#include "Types.h"
#include "Exception.h"

// forward declaration
class Socket;
class SocketInputStream;
class SocketOutputStream;
class Packet;

//////////////////////////////////////////////////////////////////////
//
// class Player
//
// ÇÃ·¹ÀÌ¾î´Â ½Ã½ºÅÛ °´Ã¼·Î, ¿¬°áµÈ ÇÏ³ªÀÇ Å¬¶óÀÌ¾ðÆ®¿Í ´ëÀÀµÈ´Ù.
// TCP ¼ÒÄÏ°ú ¼ÒÄÏÀÔÃâ·Â½ºÆ®¸²À» ³»ºÎ¿¡ °¡Áö¸ç, ÆÐÅ¶ ÀÔÃâ·Â/Ã³¸® ¸Þ½îµå¸¦
// °®°í ÀÖ´Ù. °ÔÀÓ¼­¹ö/·Î±×ÀÎ¼­¹ö/Å×½ºÆ®Å¬¶óÀÌ¾ðÆ®¿¡¼­´Â ÀÌ Å¬·¡½º¸¦
// »ó¼Ó¹Þ¾Æ¼­ »ç¿ëÇÏ°Ô µÈ´Ù.À

//
// *CAUTION*
//
// Æ¯È÷ °ÔÀÓ¼­¹ö³ª ·Î±×ÀÎ¼­¹öÀÇ °æ¿ì, ÀÌ Å¬·¡½º¸¦ »ó¼Ó¹ÞÀº Å¬·¡½º¿¡¼­
// µ¿±âÈ­(Mutex Lock/Unlock)¸¦ ÇØÁà¾ß ÇÑ´Ù.
//
//////////////////////////////////////////////////////////////////////

const bool UNDISCONNECTED = true;
const bool DISCONNECTED   = false;

class Player {

public :

	// constructor
	Player() throw (Error);
	Player ( Socket * pSocket ) throw ( ProtocolException , Error );
	
	// destructor
	virtual ~Player () throw ( ProtocolException , Error );

	// read socket's receive buffer and fill input buffer
	virtual void processInput () throw ( IOException , Error );
	
	// parse packet and execute handler for the packet
	virtual void processCommand () throw ( IOException , Error );
	
	// flush output buffer to socket's send buffer
	virtual void processOutput () throw ( IOException , Error );
	
	// send packet to player's output buffer
	virtual void sendPacket ( Packet * pPacket ) throw ( ProtocolException , Error );

	// disconnect
	// ÇÃ·¹ÀÌ¾îÀÇ ¿¬°áÀ» Á¾·áÇÒ ¶§, »ó´ëÆíÀÌ ÀûÀýÇÏ°Ô ·Î±×¾Æ¿ôÇÏÁö ¾ÊÀ» °æ¿ì ¼ÒÄÏÀÇ ¿¬°áÀÌ 
	// ÀÌ¹Ì ²÷°Ü ÀÖÀ¸¹Ç·Î disconnect(DISCONNECTED) ¸¦ »ç¿ëÇØ¼­ ¿¬°áÀ» Á¾·áÇØ¾ß ÇÑ´Ù. ¹Ý¸é, 
	// Á¤´çÇÏ°Ô ·Î±×¾Æ¿ôÀ» ÇÑ °æ¿ì¿¡´Â disconnect(UNDISCONNECTED) ¸¦ »ç¿ëÇØ¾ß ÇÑ´Ù.
	virtual void disconnect ( bool bDisconnected = DISCONNECTED ) throw ( ProtocolException , Error );
	
	// get/set socket
	Socket * getSocket () throw () { return m_pSocket; }
	void setSocket ( Socket * pSocket ) throw ();

	// get/set player ID
	std::string getID () const throw () { return m_ID; }
	void setID ( const std::string & id ) throw () { m_ID = id; }

	// get debug std::string
	virtual std::string toString () const throw ( ProtocolException , Error );

	//add by viva
	void setKey(WORD EncryptKey, WORD HashKey) throw();
	void delKey() throw();

	BYTE* pHashTable;
	//end
	
protected :

	// player id
	std::string m_ID;
	
	// TCP client socket
	Socket * m_pSocket;
	
	// buffered socket input stream
	SocketInputStream * m_pInputStream;

	// buffered socket output stream
	SocketOutputStream * m_pOutputStream;
	
};

#endif
