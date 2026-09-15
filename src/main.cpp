#include <iostream>

#include <SDL2/SDL.h>

#include "system/launch.h"
#include "time/framerate.h"

#if (defined(__WIN32) || defined(__WIN64))
    #include "system/winfunc.h"
#endif

namespace impl {

namespace launch = civilarium::system::launch;
namespace framerate = civilarium::time::framerate;

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
    impl::framerate::FrameRateAdjuster fra(60);
    impl::framerate::FrameRateMeasurer frm;
    double measured_frame_rate = 0.0;

    if (initializes_gui) {
        // DEBUG
        int count = 0;
        while (true) {
            if (count > 5) {
                break;
            }

            // Frame rate
            if (frm.MeasureFrameRate(measured_frame_rate)) {
                std::cout << "FPS: " << measured_frame_rate << std::endl;
                ++count;
            }
            fra.Adjust();
        }
    }

    // Close the GUI
    impl::launch::CloseGui(window, renderer);

    return 0;
}
