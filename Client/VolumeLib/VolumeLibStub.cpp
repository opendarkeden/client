/*-----------------------------------------------------------------------------

	VolumeLibStub.cpp

	Stub implementation of VolumeLib for macOS/SDL builds.
	Original VolumeLib uses DirectSound for volume control, which is Windows-only.

	2025.01.16

-----------------------------------------------------------------------------*/

#ifndef PLATFORM_WINDOWS

#include "../basic/Platform.h"

// Stub implementations for volume control functions
// These would need to be implemented using SDL audio mixer if needed

// Initialize volume system
extern "C" void InitVolume() {
	// Stub: SDL mixer initialization would go here
}

// Set master volume
extern "C" void SetMasterVolume(uint32_t volume) {
	// Stub: Volume = 0-100
	(void)volume;
}

// Get master volume
extern "C" uint32_t GetMasterVolume() {
	return 50; // Default to 50%
}

// Release volume system
extern "C" void ReleaseVolume() {
	// Stub: Cleanup
}

#endif /* PLATFORM_WINDOWS */
