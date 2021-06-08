//--------------------------------------------------------------------------------
//
// Filename   : Update.cpp
// Written By : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "Update.h"
#include "UpdateUtility.h"
#include "DebugInfo.h"

#ifdef __DEBUG_OUTPUT__
	#include "StringStream.h"
#endif

#define TEMP_DIR	"Update"

#ifndef __GAME_CLIENT__
	void __assert__(char const *,unsigned int,char const *,char const *)
	{
	}
#endif

//--------------------------------------------------------------------------------
// load from file
//--------------------------------------------------------------------------------
void Update::load ( ifstream & ifile ) 
{

	//--------------------------------------------------------------------------------
	// load version
	//--------------------------------------------------------------------------------
	ifile.read( (char *)&m_Version , szVersion );
	
	//--------------------------------------------------------------------------------
	// load update type
	//--------------------------------------------------------------------------------
	BYTE updatetype;
	ifile.read( (char *)&updatetype, szBYTE );
	m_UpdateType = (UPDATETYPE)updatetype;

	//--------------------------------------------------------------------------------
	// load parameters
	//--------------------------------------------------------------------------------
	ParameterLen_t szParam;
	char buf[256];
	for ( uint i = 0 ; i < maxParams ; i ++ ) {

		//--------------------------------------------------------------------------------
		// load param size
		//--------------------------------------------------------------------------------
		ifile.read( (char *)&szParam , szParameterLen );

		if ( szParam == 0 )
			continue;

		//--------------------------------------------------------------------------------
		// load param value
		//--------------------------------------------------------------------------------
		ifile.read( buf , szParam );
		buf[szParam] = 0;

		m_Params[i] = buf;
		
	}
}


//--------------------------------------------------------------------------------
// save to file
//--------------------------------------------------------------------------------
void Update::save ( ofstream & ofile ) const 
{
	//--------------------------------------------------------------------------------
	// save update type
	//--------------------------------------------------------------------------------
	ofile.write( (const char *)&m_Version , szVersion );

	//--------------------------------------------------------------------------------
	// save update type
	//--------------------------------------------------------------------------------
	BYTE updatetype = m_UpdateType;
	ofile.write( (const char *)&updatetype , szBYTE );

	//--------------------------------------------------------------------------------
	// save parameters
	//--------------------------------------------------------------------------------
	ParameterLen_t szParam;
	for ( uint i = 0 ; i < maxParams ; i ++ ) {

		//--------------------------------------------------------------------------------
		// save param size
		//--------------------------------------------------------------------------------
		szParam = m_Params[i].size();
		ofile.write( (const char *)&szParam , szParameterLen );

		if ( szParam == 0 )
			continue;

		//--------------------------------------------------------------------------------
		// save param value
		//--------------------------------------------------------------------------------
		ofile.write( m_Params[i].c_str() , szParam );
	}

}

//--------------------------------------------------------------------------------
// getFilename
//--------------------------------------------------------------------------------
std::string 
Update::getFilename(uint i)
{
	if ( m_Params[i] != "" ) {

		// 첫번째와 마지막 스페이스의 인덱스를 구한다.
		uint firstspace = m_Params[i].find_first_of(' ');

		if ( firstspace == std::string::npos )
		{
			return m_Params[i];
			//throw InvalidProtocolException("invalid resource format");
		}

		return m_Params[i].substr( 0 , firstspace );	
		//m_FileSize = atoi( m_Params[i].substr( firstspace + 1 , m_Params[i].size() - firstspace ).c_str() );	

		//cout << toString() << endl;
	}

	return "";
}

//--------------------------------------------------------------------------------
// getFilename
//--------------------------------------------------------------------------------
int 
Update::getFileSize(uint i)
{
	if ( m_Params[i] != "" ) {

		// 첫번째와 마지막 스페이스의 인덱스를 구한다.
		uint firstspace = m_Params[i].find_first_of(' ');

		if ( firstspace == std::string::npos )
		{
			return atoi( m_Params[i].c_str() );	
			//throw InvalidProtocolException("invalid resource format");
		}

		return atoi( m_Params[i].substr( firstspace + 1 , m_Params[i].size() - firstspace ).c_str() );	

		//cout << toString() << endl;
	}

	return 0;
}

//--------------------------------------------------------------------------------
// execute update
//--------------------------------------------------------------------------------
bool Update::execute () 
{
	
	switch ( m_UpdateType ) 
	{
		//------------------------------------------------------------
		//
		//			UPDATETYPE_CREATE_DIRECTORY
		//
		//------------------------------------------------------------
		case UPDATETYPE_CREATE_DIRECTORY :
		{
			if (UUFCreateDirectory( m_Params[0].c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_DELETE_DIRECTORY
		//
		//------------------------------------------------------------
		case UPDATETYPE_DELETE_DIRECTORY :
		{
			if (UUFDeleteDirectory( m_Params[0].c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_RENAME_DIRECTORY
		//
		//------------------------------------------------------------
		case UPDATETYPE_RENAME_DIRECTORY :
		{
			if (UUFRenameFile( m_Params[0].c_str(), m_Params[1].c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_CREATE_FILE
		//
		//------------------------------------------------------------
		case UPDATETYPE_CREATE_FILE :
		{
			std::string tempFile = std::string(TEMP_DIR) + separator + getFilename(0);
			std::string destFile = m_Params[1];

			if (UUFMoveFile( tempFile.c_str(), destFile.c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_DELETE_FILE
		//
		//------------------------------------------------------------
		case UPDATETYPE_DELETE_FILE :
		{
			if (UUFDeleteFile( m_Params[0].c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_RENAME_FILE
		//
		//------------------------------------------------------------
		case UPDATETYPE_RENAME_FILE :
		{
			if (UUFRenameFile( m_Params[0].c_str(), m_Params[1].c_str() ))
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_APPEND_SPRITE_PACK
		//
		//------------------------------------------------------------
		case UPDATETYPE_APPEND_SPRITE_PACK :
		{
			std::string tempSPK = std::string(TEMP_DIR) + separator + getFilename(0);
			std::string destSPK = m_Params[1];
			std::string tempSPKI = std::string(TEMP_DIR) + separator + getFilename(2);			
			std::string destSPKI = m_Params[3];
			
			if (UUFAppendPack( tempSPK.c_str(),		// src-spk
								destSPK.c_str(),		// dest-spk
								tempSPKI.c_str(),		// src-spki
								destSPKI.c_str() )		// dest-spki
								)
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_DELETE_SPRITE_PACK
		//
		//------------------------------------------------------------
		case UPDATETYPE_DELETE_SPRITE_PACK :
		{
			/*
			string tempID = string(TEMP_DIR) + separator + getFilename(0);
			string destSPK = m_Params[1];
			string tempFPK = string(TEMP_DIR) + separator + getFilename(2);
			string destFPK = m_Params[3];
			
			if (UUFDeleteSpritePack(tempID.c_str(),		// id-set
									destSPK.c_str()))		// dest
			{
			}
			else
			{
				return false;
			}
			
			if (UUFMoveFile( tempFPK.c_str(),			// src-fpk
							destFPK.c_str() ))				// dest-fpk
			{
				return true;
			}
			*/
			return true;
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_UPDATE_SPRITE_PACK
		//
		//------------------------------------------------------------
		case UPDATETYPE_UPDATE_SPRITE_PACK :
		{
			/*
			string tempSPK = string(TEMP_DIR) + separator + getFilename(0);
			string tempID = string(TEMP_DIR) + separator + getFilename(1);
			string destSPK = m_Params[2];
			
			if (UUFUpdateSpritePack(tempSPK.c_str(),		// src-spk
									tempID.c_str(),		// id-set
									destSPK.c_str()))		// dest
			{
				return true;
			}
			*/
			return true;
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_APPEND_FRAME_PACK
		//
		//------------------------------------------------------------
		case UPDATETYPE_APPEND_FRAME_PACK :
		{
			std::string tempFPK = std::string(TEMP_DIR) + separator + getFilename(0);
			std::string destFPK = m_Params[1];
			
			if (UUFAppendPack( tempFPK.c_str(),		// src-fpk
								destFPK.c_str() ))		// dest-fpk
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_APPEND_INFO
		//
		//------------------------------------------------------------
		case UPDATETYPE_APPEND_INFO :
		{
			std::string tempINF = std::string(TEMP_DIR) + separator + getFilename(0);
			std::string destINF = m_Params[1];
			
			if (UUFAppendInfo( tempINF.c_str(),		// src-info
								destINF.c_str() ))	// dest-info
			{
				return true;
			}
		}
		break;

		//------------------------------------------------------------
		//
		//			UPDATETYPE_VERIFY
		//
		//------------------------------------------------------------
		case UPDATETYPE_VERIFY :
		{
			return true;
		}
		break;

		default :
			break;
	}

	return false;
}


//--------------------------------------------------------------------------------
// get size
//--------------------------------------------------------------------------------
uint Update::getSize () const
{
	// [Version] [UpdateType] 
	uint size = szVersion + szBYTE;

	for ( uint i = 0 ; i < maxParams ; i ++ ) {
		size += ( szParameterLen + m_Params[i].size() );
	}

	return size;
}


//--------------------------------------------------------------------------------
// get max size
//--------------------------------------------------------------------------------
uint Update::getMaxSize ()
{
	// [Version] [UpdateType] [ParameterLen] [Parameter0] ... [ParameterLen] [Parameter5]
	return szVersion + szBYTE + ( szParameterLen + maxParameterLen ) * maxParams;
}

void		
Update::operator = (const Update& update)
{
	// Client Version
	m_Version = update.m_Version;

	// update type
	m_UpdateType = update.m_UpdateType;

	for (int i=0; i<maxParams; i++)
	{
		m_Params[i] = update.m_Params[i];
	}
}

//--------------------------------------------------------------------------------
// get debug string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string Update::toString () const
	{
		StringStream msg;

		msg << "Update("
			<< "Version:" << m_Version
			<< ",UpdateType:" << UPDATETYPE2String[ m_UpdateType ];

		for ( uint i = 0 ; i < maxParams ; i ++ ) {
			if ( m_Params[i] == "" )
				break;
			msg << ",Params[" << i << "]:" << m_Params[i];
		}

		msg << ")";

		return msg.toString();
	}
#endif