# Output an executable file in the project directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Target: civilarium
add_executable(civilarium
    main.cpp

    graphics/texture.cpp

    math/epsilon.cpp
    math/vector.cpp

    routine/event.cpp

    system/assert.cpp
    system/launch.cpp
    system/modal.cpp
    system/winfunc.cpp

    time/framerate.cpp
    time/timer.cpp
)
target_compile_features(civilarium PUBLIC cxx_std_23)
target_include_directories(civilarium
    PUBLIC
        ${PATH_TO_SDL2}/include
        ${PROJECT_SOURCE_DIR}/src
)
target_link_directories(civilarium PUBLIC ${PATH_TO_SDL2}/lib)
target_link_libraries(civilarium PUBLIC ${SDL2_LIBS})
target_compile_definitions(civilarium PUBLIC USE_ASSERT_IN_GUI)
