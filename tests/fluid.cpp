#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "graphics/texture.h"
#include "routine/event.h"
#include "system/assert.h"
#include "system/launch.h"
#include "time/framerate.h"

#if (defined(__WIN32) || defined(__WIN64))
    #include "system/winfunc.h"
#endif

namespace impl {

namespace texture = civilarium::graphics::texture;
namespace event = civilarium::routine::event;
namespace launch = civilarium::system::launch;
namespace framerate = civilarium::time::framerate;

}  // namespace impl

constexpr int kWindowBaseWidth = 128;
constexpr int kWindowBaseHeight = 64;
constexpr float kWindowScale = 4.0f;

constexpr int kWindowWidth = kWindowScale*kWindowBaseWidth;
constexpr int kWindowHeight = kWindowScale*kWindowBaseHeight;

constexpr int kFieldWidth = 128;
constexpr int kFieldHeight = 64;

class Fluid {
public:
    Fluid(int width, int height) : w_(width + 1), h_(height + 1)
    {
        INUK_ASSERT(2 < w_);
        INUK_ASSERT(2 < h_);
        u_.assign(h_, std::vector<float>(w_, 0.0f));
        v_.assign(h_, std::vector<float>(w_, 0.0f));
        s_.assign(h_, std::vector<float>(w_, 0.0f));
    }

    void Integrate(float dt)
    {
        // NO-OP
    }

    void Simulate(float dt)
    {
        Integrate(dt);
    }

private:
    int w_;
    int h_;

    std::vector<std::vector<float>> u_;  // Velocity
    std::vector<std::vector<float>> v_;  // Velocity
    std::vector<std::vector<float>> s_;
};

void main_routine(SDL_Window* window, SDL_Renderer* renderer)
{
    // Fluid
    Fluid fluid(kFieldWidth, kFieldHeight);

    // Graphics
    impl::texture::Texture main_texture(renderer);
    main_texture.CreateTexture(kWindowBaseWidth, kWindowBaseHeight);
    impl::texture::RenderRect src_rect(
        0, 0, kWindowBaseWidth, kWindowBaseHeight);

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

        // Clear the main texture
        main_texture.Clear(0x00, 0x00, 0x00, 0xff);

        // Integrate the texture
        // ...

        // Update the window
        main_texture.Render(nullptr, src_rect, 0.0f, 0.0f);
        SDL_RenderPresent(renderer);

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

    bool initializes_gui = impl::launch::InitGui(
        window, renderer, kWindowWidth, kWindowHeight, "fluid", kWindowScale);

    // Main routine
    if (initializes_gui) {
        main_routine(window, renderer);
    }

    // Close the GUI
    impl::launch::CloseGui(window, renderer);

    return 0;
}
