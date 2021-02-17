cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0110 NEW)
project(gameboy-emu)

# CPP related
set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(Python3 REQUIRED)
find_package(Catch2 REQUIRED)

# Add cmake modules path
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

include(CTest)
include(Catch)
enable_testing()

# Create main library
add_library(emulator "")
add_subdirectory(src)

# Create executable
add_executable(gameboy-emu
    src/main.cpp
)

target_link_libraries(gameboy-emu
    PRIVATE
        emulator
)

target_compile_options(gameboy-emu
    PRIVATE
        -Wall -Wextra
)

# Create lib for executic Catch tests
add_library(Catch INTERFACE)

target_include_directories(Catch
    INTERFACE
        include
)

# Create target for executing Catch tests
add_executable(unit_tests "")
add_subdirectory(test/src)

# Run the python scripts to generate the rom files
add_custom_target(
    generate_test_roms ALL
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/src/generate-test-roms.py -d ${CMAKE_CURRENT_BINARY_DIR}/test-roms
    COMMENT "Generating test ROMs"
)

add_dependencies(unit_tests generate_test_roms)

# Load the .cmake file to make tests available to CTest
catch_discover_tests(unit_tests)

install(
    TARGETS unit_tests
    RUNTIME DESTINATION bin
)

# This must be last
#include(CPack)