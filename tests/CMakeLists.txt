# Output an executable file in the project directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Target: fluid
add_executable(test-fluid
    fluid.cpp

    ../src/system/assert.cpp
    ../src/system/launch.cpp
    ../src/system/modal.cpp
    ../src/system/winfunc.cpp

    ../src/time/framerate.cpp
    ../src/time/timer.cpp
)
target_compile_features(test-fluid PUBLIC cxx_std_23)
target_include_directories(test-fluid
    PUBLIC
        ${PATH_TO_SDL2}/include
        ${PROJECT_SOURCE_DIR}/src
)
target_link_directories(test-fluid PUBLIC ${PATH_TO_SDL2}/lib)
target_link_libraries(test-fluid PUBLIC ${SDL2_LIBS})
target_compile_definitions(test-fluid PUBLIC USE_ASSERT_IN_GUI)
