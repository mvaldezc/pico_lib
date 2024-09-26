cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate json with compiler calls
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1")

project(tests C CXX ASM)

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
    test_message_format.cpp
    test_communication_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/state_transition_matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/fsm_state_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/StateMachine/RobotArm/states_behavior.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Communication/RobotArm/communication_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Robotics/trajectory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/reset.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/StateMachine
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/StateMachine/RobotArm
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/Communication
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/Communication/RobotArm
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/Robotics
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
)

target_link_libraries(tests 
    GTest::gtest_main
    GTest::gmock_main
)
include(GoogleTest)
gtest_discover_tests(tests)

# add test variable
target_compile_definitions(tests PRIVATE
    TEST
)
