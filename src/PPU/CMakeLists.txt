find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(PPU
    PUBLIC
        PPU.cpp
        OAMSprite.cpp
        Color.cpp
        Tile.cpp
        BgFifo.cpp
        SpriteFifo.cpp
        FifoPixel.cpp
        BgMapAttributes.cpp
)

target_include_directories(PPU
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/PPU"
        "${PROJECT_SOURCE_DIR}/include/Timer"
)

target_compile_options(PPU
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(PPU
    PRIVATE
        ${SDL2_LIBRARY}
)