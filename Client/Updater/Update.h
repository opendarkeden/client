//--------------------------------------------------------------------------------
//
// Filename   : Update.h
// Written By : Reiot
//
//--------------------------------------------------------------------------------

#ifndef __UPDATE_H__
#define __UPDATE_H__

// include files
#include "UpdateDef.h"


//--------------------------------------------------------------------------------
// update types
//--------------------------------------------------------------------------------
enum UPDATETYPE {
	UPDATETYPE_CREATE_DIRECTORY ,
	UPDATETYPE_DELETE_DIRECTORY ,
	UPDATETYPE_RENAME_DIRECTORY ,
	UPDATETYPE_CREATE_FILE,
	UPDATETYPE_DELETE_FILE,
	UPDATETYPE_RENAME_FILE,
	UPDATETYPE_APPEND_SPRITE_PACK ,
	UPDATETYPE_DELETE_SPRITE_PACK ,
	UPDATETYPE_UPDATE_SPRITE_PACK ,
	UPDATETYPE_APPEND_FRAME_PACK ,
	UPDATETYPE_APPEND_INFO,
	UPDATETYPE_VERIFY,
	UPDATETYPE_MAX	
};


const std::string UPDATETYPE2String[] = {
	"UPDATETYPE_CREATE_DIRECTORY" ,
	"UPDATETYPE_DELETE_DIRECTORY" ,
	"UPDATETYPE_RENAME_DIRECTORY" ,
	"UPDATETYPE_CREATE_FILE",
	"UPDATETYPE_DELETE_FILE",
	"UPDATETYPE_RENAME_FILE",
	"UPDATETYPE_APPEND_SPRITE_PACK" ,
	"UPDATETYPE_DELETE_SPRITE_PACK" ,
	"UPDATETYPE_UPDATE_SPRITE_PACK" ,
	"UPDATETYPE_APPEND_FRAME_PACK" ,
	"UPDATETYPE_APPEND_INFO" ,
	"UPDATETYPE_VERIFY"
};


//--------------------------------------------------------------------------------
//
// class Update;
//
// 특정 버전의 Smart Update 명령 하나와 대응되는 클래스이다.
// 
//--------------------------------------------------------------------------------
class Update {

public :

	std::string		getFilename(uint i);
	int			getFileSize(uint i);

	// load from file
	void load ( class ifstream & ifile );

	// save to file
	void save ( class ofstream & ofile ) const;

	// get size
	uint getSize () const;

	// get max size
	static uint getMaxSize ();

	// execute update
	bool execute ();


public :

	// get/set update type
	Version_t getVersion () const  { return m_Version; }
	void setVersion ( Version_t version )  { m_Version = version; }

	// get update type
	UPDATETYPE getUpdateType () const 
	{ 
		return m_UpdateType; 
	}

	// set update type
	void setUpdateType ( UPDATETYPE updateType ) 
	{ 
		m_UpdateType = updateType; 
	}

	// get parameter
	std::string getParam ( uint i ) const 
	{ 
		return m_Params[i]; 
	}

	// set parameter
	void setParam ( uint i , const std::string & param ) 
	{ 
		m_Params[i] = param; 
	}

	// get debug std::string
	std::string toString () const;

	void		operator = (const Update& update);


private :

	// Client Version
	Version_t m_Version;

	// update type
	UPDATETYPE m_UpdateType;

	// parameters
	std::string m_Params[maxParams];

};

#endif
