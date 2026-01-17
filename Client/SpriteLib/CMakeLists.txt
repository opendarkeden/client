# SpriteLib - Cross-platform sprite rendering library
# Uses DirectDraw on Windows, SDL2 on other platforms

cmake_minimum_required(VERSION 3.10)

# Determine backend to use
if(DEFINED USE_SDL_BACKEND)
	set(SPRITELIB_USE_SDL ${USE_SDL_BACKEND})
elseif(WIN32)
	set(SPRITELIB_USE_SDL OFF)
else()
	set(SPRITELIB_USE_SDL ON)
endif()

message(STATUS "SpriteLib Backend: ${SPRITELIB_USE_SDL}")

# Find SDL2 if using SDL backend
if(SPRITELIB_USE_SDL)
	find_package(SDL2 REQUIRED)
	if(TARGET SDL2::SDL2)
		set(SDL2_LIBRARIES SDL2::SDL2)
		set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
	endif()
endif()

# SpriteLib source files
# Phase 1-2: Backend files
# Phase 3: SDL-specific implementations
# Phase 4: CSprite backend integration
# NOTE: SpriteLib is primarily header-only (template-based)
# Only backend-specific files need to be compiled
set(SPRITELIB_SOURCES
	# Backend implementation
	SpriteLibBackendSDL.cpp

	# SDL-specific CSpriteSurface implementation
	CSpriteSurface_SDL.cpp
	CSpriteSurface_Adapter.cpp

	# SDL-specific CSprite implementation
	CSprite_SDL.cpp

	# Base class implementations (constructors/destructors/static members)
	CSprite.cpp
	CIndexSprite.cpp
	CAlphaSprite.cpp

	# Sprite format implementations (needed for vtables)
	CSprite555.cpp
	CSprite565.cpp
	CAlphaSprite555.cpp
	CAlphaSprite565.cpp
	CAlphaSpritePal.cpp
	CIndexSprite555.cpp
	CIndexSprite565.cpp
	CSpritePalBase.cpp

	# Sprite pack implementations (minimal set)
	CSpritePack.cpp
	CSpritePackList.cpp

	# Alpha sprite pack implementations (minimal set)
	CAlphaSpritePack.cpp

	# Shadow sprites (temporarily disabled - has compilation errors)
	CShadowSprite.cpp
	# CShadowSpritePack.cpp

	# Utilities
	CFileIndexTable.cpp
	CFilter.cpp
	CFilterPack.cpp
	CStorageSurface.cpp

	# Other sprite classes (minimal set)
	CSpriteDef.cpp
	CSpriteOutlineManager.cpp
	# CSpriteSet.cpp  # Temporarily disabled
	# CSpriteSetManager.cpp  # Temporarily disabled
	CSpritePal.cpp

	# Palette
	MPalette.cpp
	MPalettePack.cpp
)

# SpriteLib headers
set(SPRITELIB_HEADERS
	# Backend
	SpriteLibBackend.h
	SpriteLibBackendSDL.h

	# Core classes
	CSprite.h
	CSprite555.h
	CSprite565.h
	CSpriteSurface.h
	CSpritePack.h
	CSpritePackList.h
	CSpritePackList555.h
	CSpritePackList565.h
	CSpriteDef.h
	CSpritePal.h
	CSpritePalBase.h
	CSpriteSet.h
	CSpriteSetManager.h
	CSpriteOutlineManager.h

	# Alpha sprites
	CAlphaSprite.h
	CAlphaSprite555.h
	CAlphaSprite565.h
	CAlphaSpritePack.h
	CAlphaSpritePackList.h
	CAlphaSpritePackList555.h
	CAlphaSpritePackList565.h
	CAlphaSpritePal.h

	# Index sprites
	CIndexSprite.h
	CIndexSprite555.h
	CIndexSprite565.h
	CIndexSpritePack.h

	# Shadow sprites
	CShadowSprite.h
	CShadowSpritePack.h

	# Utilities
	CFileIndexTable.h
	CFilter.h
	CFilterPack.h
	CStorageSurface.h
	MPalette.h
	MPalettePack.h
	SP.h
	DrawTypeDef.h
	TArray.h
	CTypePack.h
	CTypePackVector.h
	CSetManager.h
)

# Create SpriteLib library
add_library(SpriteLib STATIC ${SPRITELIB_SOURCES})

# Include directories
target_include_directories(SpriteLib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/..  # For client_PCH.h
		${CMAKE_SOURCE_DIR}/basic
		${CMAKE_SOURCE_DIR}/Client/DXLib
)

# Add SDL2 include if using SDL backend
if(SPRITELIB_USE_SDL)
	target_include_directories(SpriteLib
		PUBLIC
			${SDL2_INCLUDE_DIRS}
	)
endif()

# Define backend macro
if(SPRITELIB_USE_SDL)
	target_compile_definitions(SpriteLib
		PUBLIC
			SPRITELIB_BACKEND_SDL
			SPRITESURFACE_STANDALONE
	)
	target_link_libraries(SpriteLib
		PUBLIC
			${SDL2_LIBRARIES}
	)
endif()

# Windows-specific settings
if(WIN32 AND NOT SPRITELIB_USE_SDL)
	# Link with DirectX libraries
	target_link_libraries(SpriteLib
		PUBLIC
			ddraw
			dxguid
	)
endif()

# Optional: Link with engine/sprite for reference implementation
if(EXISTS ${CMAKE_SOURCE_DIR}/engine/sprite/CMakeLists.txt AND SPRITELIB_USE_SDL)
	# Add engine/sprite as a dependency for reference
	# This is optional and can be enabled by the user
	# add_subdirectory(${CMAKE_SOURCE_DIR}/engine/sprite)
	# target_link_libraries(SpriteLib PRIVATE sprite)
	message(STATUS "SpriteLib: engine/sprite reference implementation available")
endif()
