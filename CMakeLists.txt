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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(EmbarcaTech_ADC C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(EmbarcaTech_ADC EmbarcaTech_ADC.c lib/ssd1306.c)

pico_set_program_name(EmbarcaTech_ADC "EmbarcaTech_ADC")
pico_set_program_version(EmbarcaTech_ADC "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(EmbarcaTech_ADC 1)
pico_enable_stdio_usb(EmbarcaTech_ADC 1)

# Add the standard library to the build
target_link_libraries(EmbarcaTech_ADC
        pico_stdlib
        hardware_adc    
        hardware_gpio
        hardware_i2c
        hardware_pwm
        hardware_clocks)

# Add the standard include files to the build
target_include_directories(EmbarcaTech_ADC PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib
)

# Add any user requested libraries
target_link_libraries(EmbarcaTech_ADC 
        
        )

include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include)
include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include)
include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_i2c/include)
include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_clocks/include)
include_directories(${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include)

pico_add_extra_outputs(EmbarcaTech_ADC)

