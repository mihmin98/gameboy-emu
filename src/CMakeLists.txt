find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(emulator
    PUBLIC
        GameBoy.cpp
        Config.cpp
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

add_library(inih "")
add_subdirectory(inih)

target_link_libraries(emulator
    PUBLIC
        CPU
        Memory
        PPU    
        Timer
        Audio
        Joypad
        inih
)

target_include_directories(emulator
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/PPU"
        "${PROJECT_SOURCE_DIR}/include/Timer"
        "${PROJECT_SOURCE_DIR}/include/Joypad"
        "${PROJECT_SOURCE_DIR}/include/inih"
)

target_compile_options(emulator
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(emulator
    PRIVATE
        ${SDL2_LIBRARY}
)