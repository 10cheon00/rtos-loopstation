add_library(loopstation_app STATIC)

target_sources(loopstation_app PRIVATE
    tasks/input/input_task.c
    tasks/display/display_task.c
    drivers/gmg12864_lcd.c
    drivers/mcp23017.c
)

target_include_directories(loopstation_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/input
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
)
