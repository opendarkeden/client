cmake_minimum_required(VERSION 3.10)

# MZLib - Compression library for network packets

# Collect source files
set(MZLIB_SOURCES MZLib.cpp)

# Create library
add_library(mzlib STATIC ${MZLIB_SOURCES})

# Include directories
target_include_directories(mzlib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../Packet
	${CMAKE_CURRENT_SOURCE_DIR}/../../basic
)

# Link basic library and zlib
target_link_libraries(mzlib PUBLIC basic z)

# Find zlib
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
	target_link_libraries(mzlib PUBLIC ZLIB::ZLIB)
	target_include_directories(mzlib PUBLIC ${ZLIB_INCLUDE_DIRS})
endif()
