#ifndef CIVILARIUM_SYSTEM_ASSERT_H_
#define CIVILARIUM_SYSTEM_ASSERT_H_

#include <cassert>

#include "SDL2/SDL.h"

#ifdef USE_ASSERT_IN_GUI
    #define INUK_ASSERT(expression) SDL_assert(expression)
#else
    #define INUK_ASSERT(expression) assert(expression)
#endif  // USE_ASSERT_IN_GUI

#endif  // CIVILARIUM_SYSTEM_ASSERT_H_
