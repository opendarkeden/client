//-----------------------------------------------------------------------------
//
//	ClientTimeFunctions.cpp
//	Time conversion functions for the Dark Eden client
//	NOTE: ConvertDurationToFrame and ConvertDurationToMillisecond are already
//	defined in GamePacketFunctions.cpp, so only ConvertMillisecondToFrame is here
//
//-----------------------------------------------------------------------------

#include "ClientDef.h"
#include "ClientConfig.h"

//-----------------------------------------------------------------------------
// Convert Millisecond To Frame
// NOTE: This function is NOT defined in GamePacketFunctions.cpp
//-----------------------------------------------------------------------------
DWORD
ConvertMillisecondToFrame(DWORD ms)
{
	// 1 ms = 1/1000
	// 1000 ms = 1
	// 1 초 = 16 frame
	return ms * g_pClientConfig->FPS / 1000;
}
