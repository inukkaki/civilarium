#ifndef CIVILARIUM_GRAPHICS_TEXTURE_H_
#define CIVILARIUM_GRAPHICS_TEXTURE_H_

#include <cstdint>

#include "SDL2/SDL.h"

#include "math/vector.h"

namespace civilarium::graphics::texture {

namespace impl {

namespace vector = civilarium::math::vector;

}  // namespace impl

struct RenderRect {
    int x;
    int y;
    int w;  // Width
    int h;  // Height

    int offset_x;
    int offset_y;

    RenderRect(int x, int y, int w, int h)
        : x(x), y(y), w(w), h(h), offset_x(0), offset_y(0) {}
    RenderRect(int x, int y, int w, int h, int offset_x, int offset_y)
        : x(x), y(y), w(w), h(h), offset_x(offset_x), offset_y(offset_y) {}
};

class Texture {
public:
    explicit Texture(SDL_Renderer* renderer)
        : renderer_(renderer), texture_(nullptr), w_(0), h_(0) {}
    ~Texture() { FreeTexture(); }

    bool CreateTexture(int width, int height);
    void FreeTexture();

    void Render(
        SDL_Texture* target, const RenderRect& src,
        const RenderRect& dst) const;
    void Render(
        SDL_Texture* target, const RenderRect& src,
        float dst_x, float dst_y) const;

    bool SetRenderTarget() const;

    void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;

    void Clear() const;
    void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;

    void DrawPoint(float x, float y) const;
    void DrawLine(float x1, float y1, float x2, float y2) const;
    void DrawRect(float x, float y, float w, float h) const;
    void FillRect(float x, float y, float w, float h) const;

    void DrawVector(
        const impl::vector::Vector2D& v,
        const impl::vector::Vector2D& offset) const;

private:
    SDL_Renderer* renderer_;

    SDL_Texture* texture_;

    int w_;
    int h_;
};

}  // namespace civilarium::graphics::texture

#endif  // CIVILARIUM_GRAPHICS_TEXTURE_H_
