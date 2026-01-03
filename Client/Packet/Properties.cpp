//--------------------------------------------------------------------------------
//
// Filename    : Properties.cpp
// Written By  : Reiot
// Description : 
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include <wtypes.h>
#include "Properties.h"
#include <stdlib.h>			// atoi()
#include "fstream"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
const char Properties::Comment = '#';
const char Properties::Separator = ':';
const char * Properties::WhiteSpaces = " \t";


//--------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------
Properties::Properties () 
	throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

Properties::Properties ( const std::string & filename ) 
	throw ()
: m_Filename(filename)
{
	__BEGIN_TRY
	__END_CATCH
}
	

//--------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------
Properties::~Properties () 
	throw ()
{	
	__BEGIN_TRY
		
	// 모든 pair 를 삭제한다.
	m_Properties.clear();

	__END_CATCH
}


//--------------------------------------------------------------------------------
// load from file
//--------------------------------------------------------------------------------
void Properties::load () 
	throw ( IOException , Error )
{
	__BEGIN_TRY
		
	if ( m_Filename.empty() )
		throw Error("filename not specified");
		
	std::ifstream ifile( m_Filename.c_str() , ios::in );
	
	if ( ! ifile )
		throw FileNotExistException( m_Filename.c_str() );

	while ( true ) {

		std::string line;
		std::getline( ifile , line );
		
		if ( ifile.eof() )
			break;

		// 코멘트 라인이거나 빈 라인이므로 skip 한다.
		if ( line[0] == Comment || line.size() == 0 )	
			continue;

		// key 의 시작문자(white space가 아닌 문자)를 찾는다. 
		uint key_begin = line.find_first_not_of( WhiteSpaces );
		
		// key_begin이 npos 라는 뜻은 그런 문자를 찾지 못했다는 뜻이다.
		// 즉, 온통 white space 로만 되어 있는 라인이므로 skip 한다.
		if ( key_begin == std::string::npos )
			continue;

		// key 와 value 를 구분짓는 separator 를 찾는다.
		// key_end 보다 sep 를 먼저 찾는 이유는 find_last_not_of()를 써서
		// sep 에서부터 역으로 key_end 를 찾기 위해서이다. ^^;
		uint sep = line.find( Separator , key_begin );

		// Separator 를 발견하지 못했을 경우는 파싱 에러로 간주한다.
		if ( sep == std::string::npos )
			throw IOException("missing separator");
		
		// sep 에서부터 역으로 key_end 를 찾아나간다.
		uint key_end = line.find_last_not_of( WhiteSpaces , sep - 1 );
		
		// sep 에서부터 value_begin 을 찾는다.
		uint value_begin = line.find_first_not_of( WhiteSpaces , sep + 1 );
		
		// key 는 있지만 value 가 없는 상태이다. 
		if ( value_begin == std::string::npos )
			throw IOException("missing value");
		
		// 맨 끝에서부터 역으로 value_end 를 찾는다. 
		// ( value_begin 이 있으면 value_end 는 무조건 존재한다.)
		uint value_end = line.find_last_not_of( WhiteSpaces ); 

		// key_begin,key_end 와 value_begin,value_end 를 사용해서 
		// line 의 substring 인 key 와 value 를 생성한다.
		std::string key = line.substr( key_begin , key_end - key_begin + 1 );
		std::string value = line.substr( value_begin , value_end - value_begin + 1 );

		// property 로 등록한다.
		setProperty( key , value );
	}
	
	ifile.close();

	__END_CATCH
}


//--------------------------------------------------------------------------------
// save to file
//--------------------------------------------------------------------------------
void Properties::save () 
	throw ( IOException )
{
	__BEGIN_TRY
		
	if ( m_Filename.empty() )
		throw Error("filename not specified");

	std::ofstream ofile( m_Filename.c_str() , ios::out | ios::trunc );
	
	for ( std::map< std::string , std::string , StringCompare >::iterator itr = m_Properties.begin() ;
		  itr != m_Properties.end() ;
		  itr ++ )
		ofile << itr->first.c_str() << ' ' << Separator << ' ' << itr->second.c_str() << endl;
	
	ofile.close();

	__END_CATCH
}


//--------------------------------------------------------------------------------
// get property
//--------------------------------------------------------------------------------
std::string Properties::getProperty ( std::string key ) const 
	throw ( NoSuchElementException )
{
	__BEGIN_TRY

		return getProperty(key.c_str());

	__END_CATCH
}

//--------------------------------------------------------------------------------
// get property
//--------------------------------------------------------------------------------
std::string Properties::getProperty ( const char* key ) const 
throw ( NoSuchElementException )
{
	__BEGIN_TRY
		
		std::string value;
	
	std::map< std::string , std::string , StringCompare >::const_iterator itr = m_Properties.find( key );
	
	if ( itr != m_Properties.end() )
		value = itr->second;
	else
		throw NoSuchElementException(key);
	
	return value;
	
	__END_CATCH
}


//--------------------------------------------------------------------------------
// get property as int
//--------------------------------------------------------------------------------
int Properties::getPropertyInt ( std::string key ) const 
	throw ( NoSuchElementException )
{
	__BEGIN_TRY
		
	return atoi( getProperty(key).c_str() );
	
	__END_CATCH
}

//--------------------------------------------------------------------------------
// get property as int
//--------------------------------------------------------------------------------
int Properties::getPropertyInt ( const char* key ) const 
throw ( NoSuchElementException )
{
	__BEGIN_TRY
		
		return atoi( getProperty(key).c_str() );
	
	__END_CATCH
}


//--------------------------------------------------------------------------------
// set property
//--------------------------------------------------------------------------------
void Properties::setProperty ( std::string key , std::string value )
	throw ()
{
	__BEGIN_TRY
		
	// 이미 키가 존재할 경우, value 를 덮어쓴다.
	m_Properties[ key ] = value;

	__END_CATCH
}


//--------------------------------------------------------------------------------
// get debug string
//--------------------------------------------------------------------------------
std::string Properties::toString () const
    throw ()
{
	__BEGIN_TRY
		
	StringStream msg;

	for ( std::map< std::string , std::string , StringCompare >::const_iterator itr = m_Properties.begin() ;
		  itr != m_Properties.end() ;
		  itr ++ ) {
		
		msg << itr->first << " : " << itr->second << "\n";
	}
	
	if ( msg.isEmpty() )
		msg << "empty properties";
	
	return msg.toString();

	__END_CATCH
}


//--------------------------------------------------------------------------------
// global variable definition
//--------------------------------------------------------------------------------
Properties * g_pConfigKorean = NULL;
Properties * g_pFileDef = NULL;
