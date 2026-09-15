#include <SDL2/SDL.h>

#include "system/launch.h"
#include "system/modal.h"

#if (defined(__WIN32) || defined(__WIN64))
    #include "system/winfunc.h"
#endif

namespace impl {

namespace launch = civilarium::system::launch;
namespace modal = civilarium::system::modal;

}  // namespace impl

int main(int argc, char* argv[])
{
    // Resolve the blurriness of the screen
    #if (defined(__WIN32) || defined(__WIN64))
        civilarium::system::winfunc::ResolveBlurriness();
    #endif

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
