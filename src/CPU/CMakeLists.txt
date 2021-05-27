find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(CPU
    PUBLIC
        Opcodes.cpp
        OpcodesMap.cpp
        SM83.cpp
)

target_include_directories(CPU
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/PPU"
        "${PROJECT_SOURCE_DIR}/include/Timer"
        "${PROJECT_SOURCE_DIR}/include/Joypad"
        "${PROJECT_SOURCE_DIR}/include/Audio"
)

target_compile_options(CPU
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(CPU
    PRIVATE
        ${SDL2_LIBRARY}
)