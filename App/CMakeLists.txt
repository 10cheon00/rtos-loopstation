add_library(loopstation_app STATIC)

target_sources(loopstation_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/mui.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/mui_u8g2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_arc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_bitmap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_box.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_button.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_circle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_cleardisplay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_d_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_d_setup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_font.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_fonts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_hvline.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_input_value.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_intersection.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_kerning.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_line.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_ll_hvline.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_polygon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_selection_list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8g2_setup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8log_u8g2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8log_u8x8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_8x8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_byte.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_cad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_capture.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_d_st7565.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_debounce.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_fonts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_input_value.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_selection_list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_setup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_u16toa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/u8x8_u8toa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/fonts/u8g2_font_ref4x5_prop_v4_tr.c

    ${CMAKE_CURRENT_SOURCE_DIR}/types/parameter.c

    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/input/input_task.c
    
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/adc_input/adc_input_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/adc_input/configs/adc_rank_knob_table.c

    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/display_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/ui_renderer_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/ui_renderer_home_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/ui_renderer_setting_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/ui_renderer_system_setting_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/configs/ui_renderer_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/widgets/knob_widget.c

    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/state_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/configs/knob_parameter_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/configs/panel_parameter_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/configs/button_ui_action_map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/stores/loopstation_parameter_store.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/ui_state_machine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/states/ui_state.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/configs/ui_panel_ui_state_table.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/states/ui_state_home_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/states/ui_state_setting_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/states/ui_state_system_setting_panel.c

    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/gmg12864_lcd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mcp23017.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/configs/mcp23017_gpio_map.c
)

target_include_directories(loopstation_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/u8g2/csrc/

    ${CMAKE_CURRENT_SOURCE_DIR}/types
    ${CMAKE_CURRENT_SOURCE_DIR}/messages
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/input
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/input/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/adc_input
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/adc_input/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/display/ui_renderers/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/stores
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks/state/ui_state_machine/states
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/configs
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(loopstation_app PUBLIC
    stm32cubemx
)
