add_library(unit_tests_dependencies "")

target_sources(unit_tests_dependencies
    PUBLIC
        TestConstants.cpp
)

target_include_directories(unit_tests_dependencies
    PUBLIC
        "${PROJECT_SOURCE_DIR}/test/include"
)

target_sources(unit_tests
    PRIVATE
        test-main.cpp
        test-memory.cpp
        test-opcodes.cpp
        test-rom.cpp
        test-sm83.cpp
        test-ppu.cpp
)

target_include_directories(unit_tests
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/test/include"
)

target_link_libraries(unit_tests
    Catch2::Catch2
    emulator
    unit_tests_dependencies
    stdc++fs
)

target_compile_options(unit_tests
    PRIVATE    
        -Wall -Wextra
)