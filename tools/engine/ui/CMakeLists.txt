cmake_minimum_required(VERSION 3.10)

project(ui)

add_library(ui STATIC
    src/ui_event_router.c
    src/ui_window.c
    src/ui_button.c
    src/ui_button_group.c
    src/ui_edit.c
#    src/ui_global_resource.c
    src/ui_window_manager.c
)

target_include_directories(ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../sprite/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../game/vs_ui/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../basic
)

target_link_libraries(ui
    PUBLIC
        basic
        sprite
        SDL2::SDL2
)

# Tests
enable_testing()

function(add_ui_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../sprite/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../basic
    )
    target_link_libraries(${test_name} PRIVATE ui basic sprite SDL2::SDL2)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_ui_test(test_ui_edit test/test_ui_edit.c)
