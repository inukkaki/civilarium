#include "graphics/texture.h"

#include <cstdint>

#include "SDL2/SDL.h"

#include "math/vector.h"
#include "system/modal.h"

namespace civilarium::graphics::texture {

namespace impl {

namespace vector = civilarium::math::vector;
namespace modal = civilarium::system::modal;

}  // namespace impl

bool Texture::CreateTexture(int width, int height)
{
    bool succeeds = true;
    FreeTexture();
    texture_ = SDL_CreateTexture(
        renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        width, height);
    if (texture_ == nullptr) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Texture Error",
            "Could not create a texture.",
            SDL_GetError());
    } else {
        w_ = width;
        h_ = height;
        SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
    }
    return succeeds;
}

void Texture::FreeTexture()
{
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Texture::Render(
    SDL_Texture* target, const RenderRect& src, const RenderRect& dst) const
{
    if (SDL_SetRenderTarget(renderer_, target) < 0) {
        impl::modal::ShowErrorMessage(
            "Texture Error",
            "Could not set a render target.",
            SDL_GetError());
    } else {
        SDL_Rect src_rect = {src.x, src.y, src.w, src.h};
        SDL_Rect dst_rect = {
            dst.x - src.offset_x, dst.y - src.offset_y, dst.w, dst.h};
        SDL_RenderCopy(renderer_, texture_, &src_rect, &dst_rect);
    }
}

void Texture::Render(
    SDL_Texture* target, const RenderRect& src, float dst_x, float dst_y) const
{
    RenderRect dst(
        static_cast<int>(dst_x + 0.5f), static_cast<int>(dst_y + 0.5f),
        src.w, src.h);
    Render(target, src, dst);
}

bool Texture::SetRenderTarget() const
{
    bool succeeds = true;
    if (SDL_SetRenderTarget(renderer_, texture_) < 0) {
        succeeds = false;
        impl::modal::ShowErrorMessage(
            "Texture Error",
            "Could not set the texture as a render target.",
            SDL_GetError());
    }
    return succeeds;
}

void Texture::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Texture::Clear() const
{
    if (SetRenderTarget()) {
        SDL_RenderClear(renderer_);
    }
}

void Texture::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
    SetDrawColor(r, g, b, a);
    Clear();
}

void Texture::DrawPoint(float x, float y) const
{
    if (SetRenderTarget()) {
        SDL_RenderDrawPointF(renderer_, x, y);
    }
}

void Texture::DrawLine(float x1, float y1, float x2, float y2) const
{
    if (SetRenderTarget()) {
        SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    }
}

void Texture::DrawRect(float x, float y, float w, float h) const
{
    if (SetRenderTarget()) {
        SDL_FRect rect = {x, y, w, h};
        SDL_RenderDrawRectF(renderer_, &rect);
    }
}

void Texture::FillRect(float x, float y, float w, float h) const
{
    if (SetRenderTarget()) {
        SDL_FRect rect = {x, y, w, h};
        SDL_RenderFillRectF(renderer_, &rect);
    }
}

void Texture::DrawVector(
    const impl::vector::Vector2D& v,
    const impl::vector::Vector2D& offset) const
{
    impl::vector::Vector2D end = v + offset;
    DrawLine(offset.x, offset.y, end.x, end.y);
}

}  // namespace civilarium::graphics::texture
