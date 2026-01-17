# SpriteLib Backend Usage Examples

# Sprite Backend Example
add_executable(sprite_backend_example
	sprite_backend_example.cpp
)

target_include_directories(sprite_backend_example
	PRIVATE
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Client
)

target_link_libraries(sprite_backend_example
	SpriteLib
	basic
)

if(TARGET SDL2::SDL2)
	target_link_libraries(sprite_backend_example SDL2::SDL2)
else()
	target_link_libraries(sprite_backend_example ${SDL2_LIBRARIES})
	target_include_directories(sprite_backend_example PRIVATE ${SDL2_INCLUDE_DIRS})
	endif()

message(STATUS "SpriteLib backend example: sprite_backend_example")
