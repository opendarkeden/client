//-----------------------------------------------------------------------------
// MTestDef.h
//-----------------------------------------------------------------------------

#ifndef __MTESTDEF_H__
#define __MTESTDEF_H__

#include "DebugInfo.h"

	// #ifdef OUTPUT_DEBUG
	//	#define __METROTECH_TEST__
	// #endif

	#ifdef __METROTECH_TEST__

		struct UDPTest {
			void Init()
			{
				UDPPacketAvailable = 0;
				UDPPacketReceive = 0;
				UDPPacketRead = 0;
				UDPPacketExecute = 0;
				UDPPacketSend = 0;
			}

			int		UDPPacketAvailable;
			int		UDPPacketReceive;
			int		UDPPacketRead;
			int		UDPPacketExecute;
			int		UDPPacketSend;
		};

		extern UDPTest g_UDPTest;

	#endif
	
#endif
