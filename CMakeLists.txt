cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME rtos-loopstation)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Add application sources
add_subdirectory(App)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/Src/debug_fault.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    loopstation_app

    # Add user defined libraries
)

set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES LINKER_LANGUAGE CXX
)

option(LOOPSTATION_SYSTEMVIEW "Enable SystemView RTT tracing" ON)
if(LOOPSTATION_SYSTEMVIEW)
    set(SYSVIEW_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/SEGGER/SystemView)
    set(RTT_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/SEGGER/RTT)
    target_include_directories(stm32cubemx INTERFACE
        ${SYSVIEW_DIR}/SYSVIEW ${SYSVIEW_DIR}/SEGGER
        ${SYSVIEW_DIR}/Sample/FreeRTOSV10.4 ${RTT_DIR}/RTT)
    target_compile_definitions(stm32cubemx INTERFACE LOOPSTATION_SYSTEMVIEW=1)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ${SYSVIEW_DIR}/SYSVIEW/SEGGER_SYSVIEW.c
        ${SYSVIEW_DIR}/Sample/FreeRTOSV10.4/SEGGER_SYSVIEW_FreeRTOS.c
        ${RTT_DIR}/RTT/SEGGER_RTT.c
        Core/Src/systemview_config.c)
endif()
