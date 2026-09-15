#include <iostream>

#include <SDL2/SDL.h>

#include "routine/event.h"
#include "system/launch.h"
#include "time/framerate.h"

#if (defined(__WIN32) || defined(__WIN64))
    #include "system/winfunc.h"
#endif

namespace impl {

namespace event = civilarium::routine::event;
namespace launch = civilarium::system::launch;
namespace framerate = civilarium::time::framerate;

}  // namespace impl

inline constexpr float kWindowScale = 4.0f;

inline constexpr int kFieldWidth = 256;
inline constexpr int kFieldHeight = 128;

void main_routine(SDL_Window* window, SDL_Renderer* renderer)
{
    // Frame rate
    int frame_rate = 60;         // t -1
    float dt = 1.0f/frame_rate;  // Frame duration / t

    impl::framerate::FrameRateAdjuster fra(frame_rate);
    impl::framerate::FrameRateMeasurer frm;
    double measured_frame_rate = 0.0;

    // Main routine
    bool quits = false;
    while (!quits) {
        // Handle events
        quits = impl::event::HandleEvents();

        // Frame rate
        if (frm.MeasureFrameRate(measured_frame_rate)) {
            std::cout << "FPS: " << measured_frame_rate << std::endl;
        }
        fra.Adjust();
    }
}

int main(int argc, char* argv[])
{
    // Resolve the blurriness of the screen
    #if (defined(__WIN32) || defined(__WIN64))
        civilarium::system::winfunc::ResolveBlurriness();
    #endif

    // Create the main window
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    int window_width = static_cast<int>(kWindowScale*kFieldWidth);
    int window_height = static_cast<int>(kWindowScale*kFieldHeight);
    bool initializes_gui = impl::launch::InitGui(
        window, renderer, window_width, window_height, "fluid",
        kWindowScale);

    // Main routine
    if (initializes_gui) {
        main_routine(window, renderer);
    }

    // Close the GUI
    impl::launch::CloseGui(window, renderer);

    return 0;
}
