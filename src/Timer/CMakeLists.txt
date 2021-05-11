find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(Timer
    PUBLIC
        Timer.cpp
)

target_include_directories(Timer
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/PPU"
        "${PROJECT_SOURCE_DIR}/include/Timer"
)

target_compile_options(Timer
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(Timer
    PRIVATE
        ${SDL2_LIBRARY}
)