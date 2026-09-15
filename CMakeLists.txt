cmake_minimum_required(VERSION 3.28)
project(civilarium CXX)

# Settings for SDL2
set(PATH_TO_SDL2
    CACHE STRING
        "Path to a directory that contains SDL2 libraries."
)

set(SDL2_LIBS)
if(WIN32)
    list(APPEND SDL2_LIBS

        # SDL
        mingw32
        SDL2main
        SDL2

        # SDL_image
        SDL2_image
    )
endif()

# Sub-directories
add_subdirectory(src)
add_subdirectory(tests)
