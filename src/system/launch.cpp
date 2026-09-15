#include "system/launch.h"

#include <cstdint>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "system/modal.h"

namespace civilarium::system::launch {

namespace impl {

namespace modal = civilarium::system::modal;

}  // namespace impl

namespace {

bool InitSdl(uint32_t flags)
{
    bool succeeds = true;
    if (SDL_Init(flags) < 0) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Initialization Error",
            "Could not initialize SDL.",
            SDL_GetError());
    }
    return succeeds;
}

bool CreateWindow(
    SDL_Window*& window, int window_width, int window_height,
    const char title[])
{
    bool succeeds = true;
    window = SDL_CreateWindow(
        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width,
        window_height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Initialization Error",
            "Could not create a main window.",
            SDL_GetError());
    }
    return succeeds;
}

bool CreateRenderer(
    SDL_Window* window, SDL_Renderer*& renderer, float window_scale)
{
    bool succeeds = true;
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Initialization Error",
            "Could not create a renderer.",
            SDL_GetError());
    } else {
        SDL_RenderSetScale(renderer, window_scale, window_scale);
    }
    return succeeds;
}

bool InitSdlImage(int flags)
{
    bool succeeds = true;
    if (!(IMG_Init(flags) | flags)) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Initialization Error",
            "Could not initialize SDL_image.",
            IMG_GetError());
    }
    return succeeds;
}

}  // namespace

bool InitGui(
    SDL_Window*& window, SDL_Renderer*& renderer, int window_width,
    int window_height, const char title[], float window_scale)
{
    bool succeeds = false;
    if (InitSdl(SDL_INIT_VIDEO)) {
        if (CreateWindow(window, window_width, window_height, title)
                && CreateRenderer(window, renderer, window_scale)) {
            if (InitSdlImage(IMG_INIT_PNG)) {
                succeeds = true;
            }
        }
    }
    return succeeds;
}

void CloseGui(SDL_Window*& window, SDL_Renderer*& renderer)
{
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

}  // namespace civilarium::system::launch
