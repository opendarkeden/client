cmake_minimum_required(VERSION 3.10)

# VolumeLib - Volume/collision detection library

if(WIN32)
	# Windows: Use original DirectSound-based volume control
	file(GLOB VOLUMELIB_SOURCES *.cpp)
	list(FILTER VOLUMELIB_SOURCES EXCLUDE REGEX ".*_PCH\\.cpp")
	list(FILTER VOLUMELIB_SOURCES EXCLUDE REGEX ".*Stub\\.cpp")
else()
	# macOS/Linux: Use stub implementation
	set(VOLUMELIB_SOURCES VolumeLibStub.cpp)
endif()

# Create library
add_library(volumelib STATIC ${VOLUMELIB_SOURCES})

# Include directories
target_include_directories(volumelib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../Packet
	${CMAKE_CURRENT_SOURCE_DIR}/../../basic
)

# Add SDL2 include for non-Windows builds
if(NOT WIN32)
	if(EXISTS "/opt/homebrew/include/SDL2")
		target_include_directories(volumelib PUBLIC /opt/homebrew/include/SDL2)
	elseif(EXISTS "/usr/local/include/SDL2")
		target_include_directories(volumelib PUBLIC /usr/local/include/SDL2)
	endif()
	# Also add SDL2::SDL2 target include if available
	if(TARGET SDL2::SDL2)
		target_include_directories(volumelib PUBLIC $<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>)
	endif()
endif()

# Link basic library
target_link_libraries(volumelib PUBLIC basic)
