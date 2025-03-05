//----------------------------------------------------------------------
// TempInformation.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "TempInformation.h"
//#include <fstream>

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
TempInformation*		g_pTempInformation = NULL;

//----------------------------------------------------------------------
// 
// constructor
//
//----------------------------------------------------------------------
TempInformation::TempInformation()
{
	Mode = MODE_NULL;
}

TempInformation::~TempInformation()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
void TempInformation::SetMode(TempInformation::TEMP_MODE mode)
{
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD_FORMAT("[TempInformation] Setmode : %d", mode);
#endif

	Mode = mode;
}

const TempInformation::TEMP_MODE TempInformation::GetMode() const
{
	return Mode;
}
