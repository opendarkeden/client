cmake_minimum_required(VERSION 3.10)

# FrameLib - Frame/Sprite animation library

# Collect source files
file(GLOB FRAMELIB_SOURCES *.cpp)

# Remove precompiled header source
list(FILTER FRAMELIB_SOURCES EXCLUDE REGEX ".*_PCH\\.cpp")

# Create library
add_library(framelib STATIC ${FRAMELIB_SOURCES})

# Include directories
target_include_directories(framelib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../Packet
	${CMAKE_CURRENT_SOURCE_DIR}/../SpriteLib
)

# Link libraries
target_link_libraries(framelib PUBLIC basic SpriteLib)
