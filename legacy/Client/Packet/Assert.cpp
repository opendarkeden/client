//--------------------------------------------------------------------------------
// 
// Filename   : Assert.cpp 
// Written By : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "Types.h"
#include "Assert.h"
#include "Exception.h"
#include <time.h>

//--------------------------------------------------------------------------------
//
// __assert__
//
// 이 함수에서는 __BEGIN_TRY , __END_CATCH로 wrapping할 필요가 없다.
//
//--------------------------------------------------------------------------------
void __assert__ ( const char * file , uint line , const char * func , const char * expr )
	throw ( AssertionError )
{
	StringStream msg;
	
	msg << eos
		<< "Assertion Failed : " << file << " : " << line;

	if ( func )
		msg << " : " << func;

	time_t currentTime = time(0);
	
	msg << expr << " at " << ctime(&currentTime);
	
	ofstream ofile("assertion_failed.log",ios::app);
	ofile << msg.toString().c_str() << endl;
	ofile.close();

	throw AssertionError( msg.toString() );
}
