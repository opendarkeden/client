cmake_minimum_required(VERSION 3.10)

# DEUtil - Dark Eden utility library

# Collect source files
file(GLOB DEUTIL_SOURCES *.cpp)

# Remove precompiled header source
list(FILTER DEUTIL_SOURCES EXCLUDE REGEX ".*StdAfx\\.cpp")

# Create library
add_library(deutil STATIC ${DEUTIL_SOURCES})

# Include directories
target_include_directories(deutil PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../Packet
)

# Link basic library
target_link_libraries(deutil PUBLIC basic)
