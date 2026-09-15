#ifndef CIVILARIUM_SYSTEM_LAUNCH_H_
#define CIVILARIUM_SYSTEM_LAUNCH_H_

#include "SDL2/SDL.h"

namespace civilarium::system::launch {

bool InitGui(
    SDL_Window*& window, SDL_Renderer*& renderer, int window_width,
    int window_height, const char title[], float window_scale);
void CloseGui(SDL_Window*& window, SDL_Renderer*& renderer);

}  // namespace civilarium::system::launch

#endif  // CIVILARIUM_SYSTEM_LAUNCH_H_
