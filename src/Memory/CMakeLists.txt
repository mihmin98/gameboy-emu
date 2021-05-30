find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(Memory
    PUBLIC
        Memory.cpp
        ROM.cpp
)

target_include_directories(Memory
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/PPU"
        "${PROJECT_SOURCE_DIR}/include/Timer"
        "${PROJECT_SOURCE_DIR}/include/Joypad"
        "${PROJECT_SOURCE_DIR}/include/Audio"
        "${PROJECT_SOURCE_DIR}/include/inih"
)

target_compile_options(Memory
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(Memory
    PRIVATE
        ${SDL2_LIBRARY}
)