#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include <SDL2/SDL.h>

#include "graphics/texture.h"
#include "math/epsilon.h"
#include "math/vector.h"
#include "routine/event.h"
#include "system/assert.h"
#include "system/launch.h"
#include "time/framerate.h"

#if (defined(__WIN32) || defined(__WIN64))
    #include "system/winfunc.h"
#endif

namespace impl {

namespace texture = civilarium::graphics::texture;
namespace epsilon = civilarium::math::epsilon;
namespace vector = civilarium::math::vector;
namespace event = civilarium::routine::event;
namespace launch = civilarium::system::launch;
namespace framerate = civilarium::time::framerate;

}  // namespace impl

constexpr int kFrameRate = 60;  // s -1

constexpr float kWindowScale = 8.0f;

constexpr int kWindowWidth = 64*16;
constexpr int kWindowHeight = 32*16;

constexpr int kFieldWidth = 128;
constexpr int kFieldHeight = 64;

class Fluid {
public:
    Fluid(int width, int height, double density, double l, int n_iters,
          double sor_value)
        : w_(width), h_(height), density_(density), l_(l), n_iters_(n_iters),
          sor_value_(sor_value)
    {
        s_.assign(h_, std::vector<double>(w_, 1.0));
        p_.assign(h_, std::vector<double>(w_, 0.0));
        vv_.assign(h_, std::vector<double>(w_ + 1, 0.0));
        vh_.assign(h_ + 1, std::vector<double>(w_, 0.0));
        vv_next_.assign(h_, std::vector<double>(w_ + 1, 0.0));
        vh_next_.assign(h_ + 1, std::vector<double>(w_, 0.0));
        m_.assign(h_, std::vector<double>(w_, 0.0));
        m_next_.assign(h_, std::vector<double>(w_, 0.0));
    }

    void Init()
    {
        // DEBUG
        /*for (int i = 0; i < 51; ++i) {
            p_[1][i + 1] = 2.0f*i;
            p_[2][i + 1] = -2.0f*i;
        }*/
        //vv_[h_/2][10] = 10.0;
        //vv_[h_/2][54] = -10.0;
        for (int i = 1; i < h_ - 1; ++i) {
            for (int j = 1; j < w_ - 1; ++j) {
                double rx = j - 40.0;
                double ry = i - h_/2.0;
                double r = std::sqrt(rx*rx + ry*ry);
                if (r < 12.0f) {
                    s_[i][j] = 0.0;
                }
            }
        }
        for (int j = 0; j < w_ - 1; ++j) {
            s_[0][j] = 0.0f;
            s_[h_ - 1][j] = 0.0f;
        }
    }

    void SolveIncompressibility(double dt)
    {
        double coeff_p = density_*l_/dt;

        for (int iter = 0; iter < n_iters_; ++iter) {
            for (int i = 1; i < h_ - 1; ++i) {
                int offset = i % 2;
                for (int j = 1 + offset; j < w_ - 1; j += 2) {
                    if (impl::epsilon::EqualsZero(s_[i][j])) {
                        continue;
                    }

                    double sy0 = s_[i - 1][j];
                    double sx0 = s_[i][j - 1];
                    double sx1 = s_[i][j + 1];
                    double sy1 = s_[i + 1][j];
                    double s = sy0 + sx0 + sx1 + sy1;
                    if (impl::epsilon::EqualsZero(s)) {
                        continue;
                    }

                    double div = (
                        -vh_[i][j] - vv_[i][j] + vv_[i][j + 1]
                        + vh_[i + 1][j]);

                    double p = -div/s;
                    p *= sor_value_;
                    p_[i][j] += coeff_p*p;

                    vh_[i][j] -= p*sy0;
                    vv_[i][j] -= p*sx0;
                    vv_[i][j + 1] += p*sx1;
                    vh_[i + 1][j] += p*sy1;
                }
            }
            for (int i = 1; i < h_ - 1; ++i) {
                int offset = (i + 1) % 2;
                for (int j = 1 + offset; j < w_ - 1; j += 2) {
                    if (impl::epsilon::EqualsZero(s_[i][j])) {
                        continue;
                    }

                    double sy0 = s_[i - 1][j];
                    double sx0 = s_[i][j - 1];
                    double sx1 = s_[i][j + 1];
                    double sy1 = s_[i + 1][j];
                    double s = sy0 + sx0 + sx1 + sy1;
                    if (impl::epsilon::EqualsZero(s)) {
                        continue;
                    }

                    double div = (
                        -vh_[i][j] - vv_[i][j] + vv_[i][j + 1]
                        + vh_[i + 1][j]);

                    double p = -div/s;
                    p *= sor_value_;
                    p_[i][j] += coeff_p*p;

                    vh_[i][j] -= p*sy0;
                    vv_[i][j] -= p*sx0;
                    vv_[i][j + 1] += p*sx1;
                    vh_[i + 1][j] += p*sy1;
                }
            }
        }
    }

    void Extrapolate()
    {
        for (int i = 0; i < h_; ++i) {
            vv_[i][0] = vv_[i][1];
            vv_[i][w_] = vv_[i][w_ - 1];
        }
        for (int j = 0; j < w_; ++j) {
            vh_[0][j] = vh_[1][j];
            vh_[h_][j] = vh_[h_ - 1][j];
        }
    }

    double AverageVv(int i, int j)
    {
        double v = (
            vv_[i - 1][j] + vv_[i - 1][j + 1] + vv_[i][j] + vv_[i][j + 1])/4;
        return v;
    }

    double AverageVh(int i, int j)
    {
        double v = (
            vh_[i][j - 1] + vh_[i][j] + vh_[i + 1][j - 1] + vh_[i + 1][j])/4;
        return v;
    }

    void AdvectVelocity(double dt)
    {
        vv_next_ = vv_;
        vh_next_ = vh_;

        double l2 = l_/2;

        for (int i = 1; i < h_ - 1; ++i) {
            for (int j = 1; j < w_ - 1; ++j) {
                // vv
                if (!impl::epsilon::EqualsZero(s_[i][j])
                        && !impl::epsilon::EqualsZero(s_[i][j - 1])) {
                    double x = j*l_;
                    double y = i*l_ + l2;
                    double vv = vv_[i][j];
                    double vh = AverageVh(i, j);
                    x -= vv*dt;
                    y -= vh*dt;
                    int i0 = std::clamp(
                        static_cast<int>((y - l2)/l_), 0, h_ - 1);
                    int i1 = std::clamp(i0 + 1, 0, h_ - 1);
                    int j0 = std::clamp(static_cast<int>(x/l_), 0, w_ - 1);
                    int j1 = std::clamp(j0 + 1, 0, w_ - 1);
                    double u0 = (x - j0*l_)/l_;
                    double u1 = 1.0 - u0;
                    double v0 = ((y - l2) - i0*l_)/l_;
                    double v1 = 1.0 - v0;
                    vv_next_[i][j] = (
                        u1*v1*vv_[i0][j0] + u0*v1*vv_[i0][j1]
                        + u1*v0*vv_[i1][j0] + u0*v0*vv_[i1][j1]);
                }

                // vh
                if (!impl::epsilon::EqualsZero(s_[i][j])
                        && !impl::epsilon::EqualsZero(s_[i - 1][j])) {
                    double x = j*l_ + l2;
                    double y = i*l_;
                    double vv = AverageVv(i, j);
                    double vh = vh_[i][j];
                    x -= vv*dt;
                    y -= vh*dt;
                    int i0 = std::clamp(static_cast<int>(y/l_), 0, h_ - 1);
                    int i1 = std::clamp(i0 + 1, 0, h_ - 1);
                    int j0 = std::clamp(
                        static_cast<int>((x - l2)/l_), 0, w_ - 1);
                    int j1 = std::clamp(j0 + 1, 0, w_ - 1);
                    double u0 = ((x - l2) - j0*l_)/l_;
                    double u1 = 1.0 - u0;
                    double v0 = (y - i0*l_)/l_;
                    double v1 = 1.0 - v0;
                    vh_next_[i][j] = (
                        u1*v1*vh_[i0][j0] + u0*v1*vh_[i0][j1]
                        + u1*v0*vh_[i1][j0] + u0*v0*vh_[i1][j1]);
                }
            }
        }

        vv_ = vv_next_;
        vh_ = vh_next_;
    }

    void AdvectSmoke(double dt)
    {
        m_next_ = m_;

        double l2 = l_/2;

        for (int i = 1; i < h_ - 1; ++i) {
            for (int j = 1; j < w_ - 1; ++j) {
                if (!impl::epsilon::EqualsZero(s_[i][j])
                        && !impl::epsilon::EqualsZero(s_[i][j - 1])) {
                    double x = j*l_ + l2;
                    double y = i*l_ + l2;
                    double vv = (vv_[i][j] + vv_[i][j + 1])/2;
                    double vh = (vh_[i][j] + vh_[i + 1][j])/2;
                    x -= vv*dt;
                    y -= vh*dt;
                    int i0 = std::clamp(
                        static_cast<int>((y - l2)/l_), 0, h_ - 1);
                    int i1 = std::clamp(i0 + 1, 0, h_ - 1);
                    int j0 = std::clamp(
                        static_cast<int>((x - l2)/l_), 0, w_ - 1);
                    int j1 = std::clamp(j0 + 1, 0, w_ - 1);
                    double u0 = ((x - l2) - j0*l_)/l_;
                    double u1 = 1.0 - u0;
                    double v0 = ((y - l2) - i0*l_)/l_;
                    double v1 = 1.0 - v0;
                    m_next_[i][j] = (
                        u1*v1*m_[i0][j0] + u0*v1*m_[i0][j1]
                        + u1*v0*m_[i1][j0] + u0*v0*m_[i1][j1]);
                }
            }
        }

        m_ = m_next_;
    }

    void Simulate(double dt)
    {
        for (int i = 1; i < h_ - 1; ++i) {
            vv_[i][1] = 1.0;
            if ((h_/2 - 5 < i) && (i < h_/2 + 5)) {
                m_[i][1] = 100.0;
            }
        }
        p_.assign(h_, std::vector<double>(w_, 0.0));
        SolveIncompressibility(dt);
        Extrapolate();
        AdvectVelocity(dt);
        AdvectSmoke(dt);
    }

    void RenderPressure(const impl::texture::Texture& texture) const
    {
        double min_p = std::numeric_limits<double>::max();
        double max_p = std::numeric_limits<double>::min();
        for (const auto& row : p_) {
            double tmp_min = *std::min_element(row.begin(), row.end());
            double tmp_max = *std::max_element(row.begin(), row.end());
            if (tmp_min < min_p) {
                min_p = tmp_min;
            }
            if (max_p < tmp_max) {
                max_p = tmp_max;
            }
        }
        //std::cout << min_p << " ~ " << max_p << std::endl;

        for (int i = 0; i < h_; ++i) {
            for (int j = 0; j < w_; ++j) {
                double val = (std::clamp(p_[i][j], min_p, max_p) - min_p)/(
                    max_p - min_p);
                int lvl = static_cast<int>(val/0.25);
                double c = (val - lvl*0.25)/0.25;
                uint8_t r, g, b;
                switch (lvl) {
                    case 0:
                        r = 0x00;
                        g = static_cast<uint8_t>(255*c);
                        b = 0xff;
                        break;
                    case 1:
                        r = 0x00;
                        g = 0xff;
                        b = static_cast<uint8_t>(255*(1.0 - c));
                        break;
                    case 2:
                        r = static_cast<uint8_t>(255*c);
                        g = 0xff;
                        b = 0x00;
                        break;
                    case 3:
                        r = 0xff;
                        g = static_cast<uint8_t>(255*(1.0 - c));
                        b = 0x00;
                        break;
                    default:
                        r = 0xff;
                        g = 0x00;
                        b = 0x00;
                        break;
                }
                texture.SetDrawColor(r, g, b, 0xff);
                texture.FillRect(
                    kWindowScale*j, kWindowScale*i, kWindowScale,
                    kWindowScale);
            }
        }
    }

    void RenderVelocity(const impl::texture::Texture& texture) const
    {
        texture.Clear(0x00, 0x00, 0x00, 0x00);
        texture.SetDrawColor(0x00, 0x00, 0x00, 0xff);
        for (int i = 0; i < h_; ++i) {
            for (int j = 0; j < w_; ++j) {
                impl::vector::Vector2D v;
                v.x = (vv_[i][j] + vv_[i][j + 1])/2;
                v.y = (vh_[i][j] + vh_[i + 1][j])/2;
                v *= 100.0;
                impl::vector::Vector2D offset(
                    kWindowScale*j + kWindowScale/2,
                    kWindowScale*i + kWindowScale/2);
                texture.DrawVector(v, offset);
            }
        }
    }

    void RenderDivergence(const impl::texture::Texture& texture) const
    {
        for (int i = 0; i < h_; ++i) {
            for (int j = 0; j < w_; ++j) {
                double div = (
                    -vh_[i][j] - vv_[i][j] + vv_[i][j + 1] + vh_[i + 1][j]);
                if (div < 0.0) {
                    uint8_t c = static_cast<uint8_t>(
                        255*std::clamp(-div/100, 0.0, 1.0));
                    texture.SetDrawColor(c, 0x00, 0x00, 0xff);
                } else {
                    uint8_t c = static_cast<uint8_t>(
                        255*std::clamp(div/100, 0.0, 1.0));
                    texture.SetDrawColor(0x00, 0x00, c, 0xff);
                }
                texture.FillRect(
                    kWindowScale*j, kWindowScale*i, kWindowScale,
                    kWindowScale);
            }
        }
    }

    void RenderSmoke(const impl::texture::Texture& texture) const
    {
        for (int i = 0; i < h_; ++i) {
            for (int j = 0; j < w_; ++j) {
                if (m_[i][j] > 0.0) {
                    uint8_t c = static_cast<uint8_t>(
                        255*std::clamp(m_[i][j]/100, 0.0, 1.0));
                    texture.SetDrawColor(0xff, 0xff, 0xff, c);
                } else {
                    texture.SetDrawColor(0x00, 0x00, 0x00, 0x00);
                }
                texture.FillRect(
                    kWindowScale*j, kWindowScale*i, kWindowScale,
                    kWindowScale);
            }
        }
    }

private:
    int w_;
    int h_;

    double density_;
    double l_;        // Length of an edge of a cell

    int n_iters_;       // Number of iterations
    double sor_value_;  // Used for Successive Over-Relaxation

    std::vector<std::vector<double>> s_;

    std::vector<std::vector<double>> p_;  // Pressure

    std::vector<std::vector<double>> vv_;  // Velocity (vertical)
    std::vector<std::vector<double>> vh_;  // Velocity (horizontal)

    std::vector<std::vector<double>> vv_next_;  // Velocity (vertical)
    std::vector<std::vector<double>> vh_next_;  // Velocity (horizontal)

    std::vector<std::vector<double>> m_;       // Smoke
    std::vector<std::vector<double>> m_next_;
};

void main_routine(SDL_Window* window, SDL_Renderer* renderer)
{
    // Fluid
    Fluid fluid(kFieldWidth, kFieldHeight, 100.0, 0.01, 50, 1.9);
    fluid.Init();

    // Graphics
    impl::texture::Texture main_texture(renderer);
    main_texture.CreateTexture(kWindowWidth, kWindowHeight);
    impl::texture::RenderRect src_rect(0, 0, kWindowWidth, kWindowHeight);

    impl::texture::Texture p_texture(renderer);
    impl::texture::Texture v_texture(renderer);
    impl::texture::Texture m_texture(renderer);
    p_texture.CreateTexture(kWindowWidth, kWindowHeight);
    v_texture.CreateTexture(kWindowWidth, kWindowHeight);
    m_texture.CreateTexture(kWindowWidth, kWindowHeight);

    impl::texture::RenderRect fluid_rect(0, 0, kWindowWidth, kWindowHeight);

    // Frame rate
    int frame_rate = kFrameRate;  // t -1
    double dt = 1.0/frame_rate;   // Frame duration / t

    impl::framerate::FrameRateAdjuster fra(frame_rate);
    impl::framerate::FrameRateMeasurer frm;
    double measured_frame_rate = 0.0;

    // Main routine
    bool quits = false;
    while (!quits) {
        // Handle events
        quits = impl::event::HandleEvents();

        // Clear the main texture
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);

        main_texture.Clear(0x00, 0x00, 0x00, 0x00);

        // Simulation
        fluid.Simulate(dt);

        // Integrate the texture (?)
        fluid.RenderPressure(p_texture);
        fluid.RenderSmoke(m_texture);
        fluid.RenderVelocity(v_texture);
        //p_texture.Render(main_texture, fluid_rect, 0.0, 0.0);
        m_texture.Render(main_texture, fluid_rect, 0.0, 0.0);
        //v_texture.Render(main_texture, fluid_rect, 0.0, 0.0);

        // Update the window
        main_texture.Render(nullptr, src_rect, 0.0, 0.0);
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
        window, renderer, kWindowWidth, kWindowHeight, "fluid", 1.0f);

    // Main routine
    if (initializes_gui) {
        main_routine(window, renderer);
    }

    // Close the GUI
    impl::launch::CloseGui(window, renderer);

    return 0;
}
