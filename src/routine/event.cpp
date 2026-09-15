#include "routine/event.h"

#include "SDL2/SDL.h"

namespace civilarium::routine::event {

bool HandleEvents()
{
    bool quits = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quits = true;
            break;
        default:
            break;
        }
    }
    return quits;
}

}  // namespace civilarium::routine::event
