cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

project(RaspberryShifter C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(RaspberryShifter)

add_subdirectory(ps2/atkPico)

# Example source
target_sources(RaspberryShifter PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/usb/hid_app.c
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb/hid_keyboard.c
        ${CMAKE_CURRENT_LIST_DIR}/ps2/ps2ToUsb.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(RaspberryShifter PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/usb
        ${CMAKE_CURRENT_LIST_DIR}/ps2)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_host
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(RaspberryShifter PUBLIC pico_stdlib tinyusb_host tinyusb_board hardware_pio atk_pico_lib)

pico_add_extra_outputs(RaspberryShifter)

 
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
