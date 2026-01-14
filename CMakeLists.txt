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

# Print configuration summary
message(STATUS "")
message(STATUS "=== Dark Eden Client Configuration ===")
message(STATUS "  Build Tests:      ${BUILD_TESTS}")
message(STATUS "  Use SDL Backend:  ${USE_SDL_BACKEND}")
message(STATUS "  Build Engine:     ${BUILD_ENGINE}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "=======================================")
message(STATUS "")
