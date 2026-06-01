# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(GlideRC_FC_Firmware C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(GlideRC_FC_Firmware 
    GlideRC_FC_Firmware.c
    src/hardware.c
    src/servo.c
)

# Add global macro definition
target_compile_definitions(
    GlideRC_FC_Firmware PRIVATE
    ON_FC=1
)

pico_set_program_name(GlideRC_FC_Firmware "GlideRC_FC_Firmware")
pico_set_program_version(GlideRC_FC_Firmware "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(GlideRC_FC_Firmware 0)
pico_enable_stdio_usb(GlideRC_FC_Firmware 1) # !!!ENABLED USB SERIAL LOGS!!!

# Add the standard library to the build
target_link_libraries(GlideRC_FC_Firmware
    pico_stdlib
    hardware_uart
    hardware_irq
    hardware_pwm
    hardware_clocks
    hardware_spi
)

# Add the standard include files to the build
target_include_directories(GlideRC_FC_Firmware PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(GlideRC_FC_Firmware)

