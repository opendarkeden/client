//--------------------------------------------------------------------------------
// 
// Filename    : UpdateManager.cpp
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "UpdateManager.h"
#include "DebugInfo.h"

#ifdef __DEBUG_OUTPUT__
	#include "StringStream.h"
#endif

//--------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------
UpdateManager::UpdateManager () 
{
}


//--------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------
UpdateManager::~UpdateManager () 
{
	// delete all update
	while ( !m_Updates.empty() ) {
		Update * pUpdate = m_Updates.front();
		delete pUpdate;
		m_Updates.pop_front();
	}
}

	
//--------------------------------------------------------------------------------
// load from update file
//--------------------------------------------------------------------------------
void UpdateManager::load ( const std::string & filename ) 
{
	ifstream ifile( filename.c_str() , ios::binary );
	if ( !ifile ) {
		//StringStream msg;
		//msg << "cannot open " << filename << " with read mode";
	}

	//--------------------------------------------------------------------------------
	// read #Updates
	//--------------------------------------------------------------------------------
	WORD size;
	ifile.read( (char *)&size , szWORD );

	//--------------------------------------------------------------------------------
	// read each recource
	//--------------------------------------------------------------------------------
	for ( int i = 0 ; i < size ; i ++ ) {

		Update * pUpdate = new Update();
		pUpdate->load( ifile );

		push_back( pUpdate );
	
	}

	ifile.close();
}


//--------------------------------------------------------------------------------
// save to update file
//--------------------------------------------------------------------------------
void UpdateManager::save ( const std::string & filename ) const 
{
	ofstream ofile( filename.c_str() , ios::binary );
	if ( !ofile ) {
//		StringStream msg;
//		msg << "cannot open " << filename << " with write mode";
	}

	//--------------------------------------------------------------------------------
	// write #Update
	//--------------------------------------------------------------------------------
	WORD size = m_Updates.size();
	ofile.write( (const char *)&size , szWORD );

	for ( std::list< Update * >::const_iterator itr = m_Updates.begin() ; itr != m_Updates.end() ; itr ++ ) {

		Update * pUpdate = *itr;

		pUpdate->save( ofile );
	}

	ofile.close();

}

//--------------------------------------------------------------------------------
// get file size
//--------------------------------------------------------------------------------
uint UpdateManager::getFileSize () const
{
	uint size = szWORD;	// nUpdates

	for ( std::list< Update * >::const_iterator itr = m_Updates.begin() ; itr != m_Updates.end() ; itr ++ ) {
		size += (*itr)->getFileSize(0);
	}

	return size;
}


//--------------------------------------------------------------------------------
// get size
//--------------------------------------------------------------------------------
uint UpdateManager::getSize () const
{
	uint size = szWORD;	// nUpdates

	for ( std::list< Update * >::const_iterator itr = m_Updates.begin() ; itr != m_Updates.end() ; itr ++ ) {
		size += (*itr)->getSize();
	}

	return size;
}

//--------------------------------------------------------------------------------
// operator = by sigi
//--------------------------------------------------------------------------------
void		
UpdateManager::operator = (const UpdateManager& um)
{
	m_Updates.clear();

	for ( std::list< Update * >::const_iterator itr = um.m_Updates.begin() ; itr != um.m_Updates.end() ; itr ++ ) {
		Update* pUpdate = *itr;

		Update* pNewUpdate = new Update;

		*pNewUpdate = *pUpdate;

		push_back( pNewUpdate );
	}
}

//--------------------------------------------------------------------------------
// operater += by sigi
//--------------------------------------------------------------------------------
void		
UpdateManager::operator += (const UpdateManager& um)
{
	for ( std::list< Update * >::const_iterator itr = um.m_Updates.begin() ; itr != um.m_Updates.end() ; itr ++ ) {
		Update* pUpdate = *itr;

		Update* pNewUpdate = new Update;

		*pNewUpdate = *pUpdate;

		push_back( pNewUpdate );
	}
}


//--------------------------------------------------------------------------------
// get debug string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string UpdateManager::toString () const 
	{
		StringStream msg;

		msg << "UpdateManager(\n";
			
		for ( std::list< Update * >::const_iterator itr = m_Updates.begin() ; itr != m_Updates.end() ; itr ++ ) {
			msg << (*itr)->toString() << eos;
		}

		msg << ")";

		return msg.toString();
	}
#endif