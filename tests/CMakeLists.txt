# Test programs for platform abstraction layer

# Platform test
add_executable(test_platform
	test_platform.cpp
)

target_link_libraries(test_platform
	basic
)

if(USE_SDL_BACKEND OR NOT WIN32)
	if(TARGET SDL2::SDL2)
		target_link_libraries(test_platform SDL2::SDL2)
	else()
		target_link_libraries(test_platform ${SDL2_LIBRARIES})
		target_include_directories(test_platform PRIVATE ${SDL2_INCLUDE_DIRS})
	endif()
endif()

# Add test to CTest
add_test(NAME PlatformTest COMMAND test_platform)

# Set test properties
set_tests_properties(PlatformTest PROPERTIES
	TIMEOUT 30
	LABELS "platform;unit"
)

# SpriteLib Backend Rendering Test
if(NOT WIN32 OR USE_SDL_BACKEND)
	add_executable(test_spritelib_backend
		test_spritelib_backend.cpp
	)

	target_include_directories(test_spritelib_backend
		PRIVATE
		${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/Client
	)

	target_link_libraries(test_spritelib_backend
		SpriteLib
		basic
	)

	if(TARGET SDL2::SDL2)
		target_link_libraries(test_spritelib_backend SDL2::SDL2)
	else()
		target_link_libraries(test_spritelib_backend ${SDL2_LIBRARIES})
		target_include_directories(test_spritelib_backend PRIVATE ${SDL2_INCLUDE_DIRS})
	endif()

	# Add test to CTest
	add_test(NAME SpriteLibBackendTest COMMAND test_spritelib_backend)

	set_tests_properties(SpriteLibBackendTest PROPERTIES
		TIMEOUT 30
		LABELS "spritelib;integration"
	)

	# Sprite rendering test (requires display)
	add_executable(test_sprite_rendering
		test_sprite_rendering.cpp
	)

	target_include_directories(test_sprite_rendering
		PRIVATE
		${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/Client
	)

	target_link_libraries(test_sprite_rendering
		SpriteLib
		basic
	)

	if(TARGET SDL2::SDL2)
		target_link_libraries(test_sprite_rendering SDL2::SDL2)
	else()
		target_link_libraries(test_sprite_rendering ${SDL2_LIBRARIES})
		target_include_directories(test_sprite_rendering PRIVATE ${SDL2_INCLUDE_DIRS})
	endif()

	# Don't add to CTest - requires display and user interaction
endif()

