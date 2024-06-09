cmake_minimum_required(VERSION 3.10)
project(ByteCoreFast)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunused -O3")
include_directories(src tests)

# Make clang recognize <Python.h>
find_package(Python3 REQUIRED COMPONENTS Development)
include_directories(${Python3_INCLUDE_DIRS})

set(SRC_FILES
    src/memory.c
    src/control_unit.c
    src/emulator.c
)

set(TEST_FILES
    tests/test_memory.c
    tests/test_control_unit.c
    tests/test_emulator.c
)

set(UNITY_FILE
    tests/unity.c
)

add_executable(test_memory ${SRC_FILES} tests/test_memory.c ${UNITY_FILE})
target_link_libraries(test_memory m)

add_executable(test_control_unit ${SRC_FILES} tests/test_control_unit.c ${UNITY_FILE})
target_link_libraries(test_control_unit m)

add_executable(test_emulator ${SRC_FILES} tests/test_emulator.c ${UNITY_FILE})
target_link_libraries(test_emulator m)

add_custom_target(test ALL
    COMMAND test_memory
    COMMAND test_control_unit
    COMMAND test_emulator
    DEPENDS test_memory test_control_unit test_emulator
    COMMENT "Running all tests"
)
