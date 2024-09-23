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
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_PLATFORM host)

include(../pico_sdk_import.cmake)

project(test C CXX ASM)

pico_sdk_init()

# Add Google Test and Google Mock
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    test_fsm_state_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/state_transition_matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/fsm_state_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/states.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/StateMachine
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/StateMachine/RobotArm
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

target_link_libraries(tests 
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(tests)

# add test variable

target_compile_definitions(tests PRIVATE
    TEST
)
