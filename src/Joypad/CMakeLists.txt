find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

target_sources(Joypad
    PUBLIC
        Joypad.cpp
)

target_include_directories(Joypad
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/CPU"
        "${PROJECT_SOURCE_DIR}/include/Memory"
        "${PROJECT_SOURCE_DIR}/include/Timer"
        "${PROJECT_SOURCE_DIR}/include/Joypad"
)

target_compile_options(Joypad
    PRIVATE
        -Wall -Wextra
)

target_link_libraries(Joypad
    PRIVATE
        ${SDL2_LIBRARY}
)