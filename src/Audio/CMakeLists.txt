find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(Audio
    PUBLIC
        Audio.cpp
        Channel1.cpp
        Channel2.cpp
        Channel3.cpp
        Channel4.cpp
)

target_include_directories(Audio
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/Audio"
        "${PROJECT_SOURCE_DIR}/include/PPU"
)

target_compile_options(Audio
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(Audio
    PRIVATE
        ${SDL2_LIBRARY}
)