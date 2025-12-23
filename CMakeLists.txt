cmake_minimum_required(VERSION 3.10)
project(SDLSpriteViewer VERSION 1.0.0 LANGUAGES C CXX)

# Set C++11 standard (Requirements 8.3)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2 library (Requirements 8.2)
find_package(SDL2 REQUIRED)

# Platform-specific settings (Requirements 8.5)
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    # macOS-specific settings
    set(CMAKE_MACOSX_RPATH ON)
elseif(UNIX)
    # Linux-specific settings
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Source files
set(SOURCES
    src/main.c
    src/sdl_framework.c
    src/color.c
    src/sprite.c
    src/spritepack.c
    src/viewer.c
)

# Header files
set(HEADERS
    src/sdl_framework.h
    src/color.h
    src/sprite.h
    src/spritepack.h
    src/viewer.h
)

# Create executable (Requirements 8.4)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

# Link SDL2 (Requirements 8.2)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

# Handle SDL2 linking on different platforms
if(TARGET SDL2::SDL2)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
endif()

# Add SDL2main on Windows
if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Enable testing
enable_testing()

# Test executable
set(TEST_SOURCES
    tests/test_main.c
    tests/test_color.c
    tests/test_sdl_framework.c
    tests/test_sprite.c
    tests/test_spritepack.c
    src/color.c
    src/sdl_framework.c
    src/sprite.c
    src/spritepack.c
)

add_executable(test_runner ${TEST_SOURCES})
target_include_directories(test_runner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${SDL2_INCLUDE_DIRS}
)

# Link SDL2 to test runner for SDL framework tests
target_link_libraries(test_runner PRIVATE ${SDL2_LIBRARIES})
if(TARGET SDL2::SDL2)
    target_link_libraries(test_runner PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
    target_link_libraries(test_runner PRIVATE SDL2::SDL2-static)
endif()

add_test(NAME unit_tests COMMAND test_runner)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
