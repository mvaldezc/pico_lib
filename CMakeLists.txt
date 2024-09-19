# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion RISCV_COREV_MAY_24)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_lib C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico_lib pico_lib.cpp
  src/StateMachine/RobotArm/state_transition_matrix.cpp
  src/StateMachine/RobotArm/fsm_state_factory.cpp
  src/StateMachine/RobotArm/states.cpp
)

pico_set_program_name(pico_lib "pico_lib")
pico_set_program_version(pico_lib "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_lib 1)
pico_enable_stdio_usb(pico_lib 0)

# Add include files to the build
target_include_directories(pico_lib PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Mutex
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Motor
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Communication
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/StateMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/StateMachine/RobotArm
)

# Add any user requested libraries
target_link_libraries(pico_lib 
        pico_stdlib
        pico_sync
        pico_multicore
        hardware_timer
        hardware_i2c
        hardware_dma
        pico_atomic
        )

pico_add_extra_outputs(pico_lib)

