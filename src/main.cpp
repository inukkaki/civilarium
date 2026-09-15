#include <SDL2/SDL.h>

#include "system/launch.h"
#include "system/modal.h"

namespace impl {

namespace launch = civilarium::system::launch;
namespace modal = civilarium::system::modal;

}  // namespace impl

int main(int argc, char* argv[])
{
    // Create the main window
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool initializes_gui = impl::launch::InitGui(
        window, renderer, 640, 480, "Civilarium", 2.0f);

    // Main routine
    if (initializes_gui) {
        // DEBUG
        impl::modal::ShowErrorMessage(
            "debug msg", "succeeded in initialization");
    }

    // Close the GUI
    impl::launch::CloseGui(window, renderer);

    return 0;
}
