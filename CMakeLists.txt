cmake_minimum_required(VERSION 3.10)

project(DarkEdenClient VERSION 1.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(BUILD_TESTS "Build test programs" ON)
option(USE_SDL_BACKEND "Use SDL backend instead of native Windows APIs" OFF)
option(BUILD_ENGINE "Build new SDL-based engine" ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SDL2 (needed for engine and DXLib)
if(BUILD_ENGINE OR USE_SDL_BACKEND OR NOT WIN32)
	find_package(SDL2 REQUIRED)
endif()

# Subdirectories
add_subdirectory(basic)
add_subdirectory(Client/DXLib)
add_subdirectory(Client/SpriteLib)

if(BUILD_ENGINE)
	add_subdirectory(engine/sprite)
	add_subdirectory(engine/ui)
endif()

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

# Examples (always built if backend is enabled)
if(USE_SDL_BACKEND OR NOT WIN32)
	add_subdirectory(examples)
endif()

# Dark Eden Client Executable
if(USE_SDL_BACKEND OR NOT WIN32)
	# Collect all Client source files
	file(GLOB_RECURSE CLIENT_SOURCES
		Client/*.cpp
		Client/*.h
	)

	# Collect VS_UI source files separately
	file(GLOB VS_UI_SOURCES VS_UI/src/*.cpp)

	# Combine source lists
	list(APPEND CLIENT_SOURCES ${VS_UI_SOURCES})

	# Exclude WinMain.cpp (standalone VS_UI program entry point)
	# Build the full path that GLOB would have created
	list(FILTER CLIENT_SOURCES EXCLUDE REGEX ".*WinMain\\.cpp")

	# Exclude Windows-specific Immersion tactile feedback library on non-Windows platforms
	if(NOT WIN32)
		list(FILTER CLIENT_SOURCES EXCLUDE REGEX ".*Imm.*")
	endif()

	# Add DarkEden executable
	add_executable(DarkEden ${CLIENT_SOURCES})

	# Set output directory
	set_target_properties(DarkEden PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	)

	# Link libraries
	target_link_libraries(DarkEden PRIVATE
		SpriteLib
		dxlib
		basic
		${SDL2_LIBRARIES}
	)

	# Include directories
	target_include_directories(DarkEden PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/Client
		${CMAKE_CURRENT_SOURCE_DIR}/Client/Packet
		${CMAKE_CURRENT_SOURCE_DIR}/Client/framelib
		${CMAKE_CURRENT_SOURCE_DIR}/engine/sprite/include
		${CMAKE_CURRENT_SOURCE_DIR}/engine/ui/include
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/header
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/widget
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/hangul
		${CMAKE_CURRENT_SOURCE_DIR}/game/vs_ui
		${SDL2_INCLUDE_DIRS}
	)

	# Add Immersion library include path only on Windows
	if(WIN32)
		target_include_directories(DarkEden PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/Imm
		)
	endif()

	# Compile definitions
	target_compile_definitions(DarkEden PRIVATE
		USE_SDL_BACKEND
		SPRITELIB_BACKEND_SDL
	)

	message(STATUS "DarkEden executable will be built from ${CLIENT_SOURCES_COUNT} source files")
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "=== Dark Eden Client Configuration ===")
message(STATUS "  Build Tests:      ${BUILD_TESTS}")
message(STATUS "  Use SDL Backend:  ${USE_SDL_BACKEND}")
message(STATUS "  Build Engine:     ${BUILD_ENGINE}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "=======================================")
message(STATUS "")
