find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(emulator
    PUBLIC
        GameBoy.cpp
)

add_library(CPU "")
add_subdirectory(CPU)

add_library(Memory "")
add_subdirectory(Memory)

add_library(PPU "")
add_subdirectory(PPU)

add_library(Timer "")
add_subdirectory(Timer)

add_library(Audio "")
add_subdirectory(Audio)

add_library(Joypad "")
add_subdirectory(Joypad)

target_link_libraries(emulator
    PUBLIC
        CPU
        Memory
        PPU    
        Timer
        Audio
        Joypad
)

target_include_directories(emulator
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_compile_options(emulator
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(emulator
    PRIVATE
        ${SDL2_LIBRARY}
)