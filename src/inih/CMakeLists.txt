find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(inih
    PUBLIC
        ini.c
        INIReader.cpp
)

target_include_directories(inih
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

target_compile_options(inih
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(inih
    PRIVATE
        ${SDL2_LIBRARY}
)