#include "Client_PCH.h"
#include <fstream>
#include "NicknameInfo.h"
#include "Packet/Assert.h"
NicknameInfo::NicknameInfo()
{
	m_NicknameID = 0;
	m_NicknameType = 0;
	m_NicknameIndex = 0;
}
PacketSize_t NicknameInfo::getSize() const
{
	switch ( m_NicknameType )
	{
		case NICK_NONE:
			return szWORD + szBYTE;
		case NICK_BUILT_IN:
		case NICK_QUEST:
		case NICK_FORCED:
			return szWORD + szBYTE + szWORD;
		case NICK_CUSTOM_FORCED:
		case NICK_CUSTOM:
			return szWORD + szBYTE + szBYTE + m_Nickname.size();
		default:
			assert(false);
	}

	return 0;
}

void NicknameInfo::read(SocketInputStream& iStream)
	throw (ProtocolException, Error)
{
	__BEGIN_TRY

	iStream.read( m_NicknameID );
	iStream.read( m_NicknameType );

	switch ( m_NicknameType )
	{
		case NICK_NONE:
			{
				break;
			}
		case NICK_BUILT_IN:
		case NICK_QUEST:
		case NICK_FORCED:
			{
				iStream.read( m_NicknameIndex );
				break;
			}
		case NICK_CUSTOM_FORCED:
		case NICK_CUSTOM:
			{
				BYTE szSTR;
				iStream.read( szSTR );
				if ( szSTR != 0 ) iStream.read( m_Nickname, szSTR );
				break;
			}
		default:
			assert(false);
	}

	__END_CATCH
}

void NicknameInfo::write(SocketOutputStream& oStream) const
	throw (ProtocolException, Error)
{
	__BEGIN_TRY

	oStream.write( m_NicknameID );
	oStream.write( m_NicknameType );

	switch ( m_NicknameType )
	{
		case NICK_NONE:
			{
				break;
			}
		case NICK_BUILT_IN:
		case NICK_QUEST:
		case NICK_FORCED:
			{
				oStream.write( m_NicknameIndex );
				break;
			}
		case NICK_CUSTOM_FORCED:
		case NICK_CUSTOM:
			{
				BYTE szSTR = m_Nickname.size();
				oStream.write( szSTR );
				if ( szSTR != 0 ) oStream.write( m_Nickname );
				break;
			}
		default:
			assert(false);
	}

	__END_CATCH
}
