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
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)
endif()

# Subdirectories
add_subdirectory(basic)
add_subdirectory(Client/DXLib)
add_subdirectory(Client/SpriteLib)

if(BUILD_ENGINE)
	add_subdirectory(engine/sprite)
	add_subdirectory(engine/ui)
endif()

# ============================================================
# VS_UI Library - UI System (按需编译必要的 Client 文件)
# ============================================================
if(USE_SDL_BACKEND OR NOT WIN32)
	# VS_UI source files
	file(GLOB_RECURSE VS_UI_SRC_SOURCES
		VS_UI/src/*.cpp
	)

	# Exclude Windows-specific files
	if(NOT WIN32)
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX ".*Imm.*")
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX ".*WinMain.*")
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX ".*WebBrowser.*")
		# Exclude Windows-specific Korean input method files
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX "VS_UI/src/hangul/.*")
		# Exclude Windows-specific edit widget (uses IMM/GDI)
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX ".*U_edit.*")
		list(FILTER VS_UI_SRC_SOURCES EXCLUDE REGEX ".*u_window.*")
	endif()

	# Client files that VS_UI depends on (按需编译)
	set(VS_UI_CLIENT_SOURCES
		# Core
		Client/Client.cpp
		Client/ClientFunction.cpp

		# Item System
		Client/MItem.cpp
		Client/MItemManager.cpp
		Client/MItemOptionTable.cpp
		Client/MMoneyManager.cpp

		# Skill System
		Client/MSkillManager.cpp
		Client/SkillDef.cpp

		# Gear System
		Client/MSlayerGear.cpp
		Client/MVampireGear.cpp
		Client/MOustersGear.cpp
		Client/MQuickSlot.cpp

		# Inventory System
		Client/MInventory.cpp
		Client/MStorage.cpp
		Client/MShopShelf.cpp

		# Zone System
		Client/MZone.cpp
		Client/MZoneTable.cpp

		# Player System
		Client/MPlayer.cpp
		Client/MCreatureTable.cpp
		Client/MFakeCreature.cpp
		Client/MNpcTable.cpp
		Client/MNPCTable.cpp

		# Social System
		Client/MParty.cpp
		Client/MGuildMarkManager.cpp
		Client/MGuildInfoMapper.cpp

		# Trade System
		Client/MTradeManager.cpp
		Client/MPriceManager.cpp
		Client/MShop.cpp

		# Game Data
		Client/MGameStringTable.cpp
		Client/ExperienceTable.cpp
		Client/UserInformation.cpp
		Client/SystemAvailabilities.cpp
		Client/FameInfo.cpp
		Client/DebugInfo.cpp
		Client/ClientConfig.cpp
		Client/UtilityFunction.cpp

		# Events & Time
		Client/MEventManager.cpp
		Client/MGameTime.cpp
		Client/MTimeItemManager.cpp

		# UI Support
		Client/MHelpMessageManager.cpp
		Client/MSoundTable.cpp
		Client/MMonsterKillQuestInfo.cpp

		# Stubs
		Client/Timer2.cpp
	)

	# Combine all sources
	set(VS_UI_ALL_SOURCES ${VS_UI_SRC_SOURCES} ${VS_UI_CLIENT_SOURCES})

	# Create VS_UI static library
	add_library(VS_UI STATIC ${VS_UI_ALL_SOURCES})

	# Include directories
	target_include_directories(VS_UI PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/header
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/widget
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/hangul
		${CMAKE_CURRENT_SOURCE_DIR}/Client
		${CMAKE_CURRENT_SOURCE_DIR}/Client/SpriteLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/DXLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/D3DLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/framelib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/Packet
		${CMAKE_CURRENT_SOURCE_DIR}
	)

	# Link libraries
	target_link_libraries(VS_UI PUBLIC
		basic
		dxlib
		SpriteLib
		SDL2::SDL2
		SDL2::SDL2main
		SDL2_image::SDL2_image
		SDL2_ttf::SDL2_ttf
	)

	# Platform-specific definitions
	if(NOT WIN32)
		target_compile_definitions(VS_UI PRIVATE PLATFORM_MACOS)
	endif()

	message(STATUS "VS_UI library will be built from ${VS_UI_SRC_SOURCES} VS_UI files + ${VS_UI_CLIENT_SOURCES} Client files")
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
	# Collect remaining Client source files (excluding those already in VS_UI)
	file(GLOB CLIENT_MAIN_SOURCES
		Client/*.cpp
	)

	# Remove files that are already compiled in VS_UI library
	list(REMOVE_ITEM CLIENT_MAIN_SOURCES
		${VS_UI_CLIENT_SOURCES}
	)

	# Exclude Windows-specific files
	if(NOT WIN32)
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*Imm.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*D3D.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*Direct3.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*GameMain.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*EXECryptor.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*GetWinVer.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*MInternetConnection.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*MWorkThread.*")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*MMusic\\.cpp")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*PacketFunction\\.cpp")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*ProfileManager\\.cpp")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*VolumeOut.*\\.cpp")
		list(FILTER CLIENT_MAIN_SOURCES EXCLUDE REGEX ".*WavePackFileManager\\.cpp")
	endif()

	# Add DarkEden executable
	add_executable(DarkEden ${CLIENT_MAIN_SOURCES})

	# Set output directory
	set_target_properties(DarkEden PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	)

	# Link libraries (now includes VS_UI)
	target_link_libraries(DarkEden PRIVATE
		SpriteLib
		dxlib
		basic
		VS_UI
		SDL2::SDL2
		SDL2::SDL2main
	)

	# Include directories
	target_include_directories(DarkEden PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/basic
		${CMAKE_CURRENT_SOURCE_DIR}/Client
		${CMAKE_CURRENT_SOURCE_DIR}/Client/D3DLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/DXLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/SpriteLib
		${CMAKE_CURRENT_SOURCE_DIR}/Client/Packet
		${CMAKE_CURRENT_SOURCE_DIR}/Client/framelib
		${CMAKE_CURRENT_SOURCE_DIR}/engine/sprite/include
		${CMAKE_CURRENT_SOURCE_DIR}/engine/ui/include
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/header
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/widget
		${CMAKE_CURRENT_SOURCE_DIR}/VS_UI/src/hangul
		${CMAKE_CURRENT_SOURCE_DIR}/game/vs_ui
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

	message(STATUS "DarkEden executable will be built from remaining Client files (VS_UI compiled separately)")
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
