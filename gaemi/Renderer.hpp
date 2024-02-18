//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_RENDERER_HPP
#define GAEMI_RENDERER_HPP

#include "Defines.hpp"
#include "Rect.hpp"
#include "Vec4.hpp"

struct Texture;
struct Color;

using gmath::Vec2;
using gmath::Rect;
using gmath::Vec4;

namespace render {

    enum class TextureWrapMode {
        Repeat = 0,
        Clamp = 1,
        MirrorRepeat = 2,
        MirrorClamp = 3
    };

    enum class TextureFilterMode {
        Point = 0,
        Bilinear = 1,
        Trilinear = 2,
        Anisotropic4x = 3,
        Anisotropic8x = 4,
        Anisotropic16x = 5
    };

    void BeginDraw();

    void ClearScreen();

    void EndDraw();

    GAPI void DrawTexture(const Texture& texture2D, i32 x, i32 y, Color tint);
    GAPI void DrawSprite(const Texture& texture2D, Rect srcRect, Rect dstRect, Color tint);
    GAPI void DrawLine(const Vec2& origin, const Vec2& direction, float length, Color color);
    GAPI void DrawRectangleLines(const Rect& rect, Color color);
    GAPI void DrawDefaultText(const str& text, const Vec2& pos, i32 fontSize, Color color);

    GAPI void SetTextureWrap(const Texture& texture2D, TextureWrapMode mode);
    GAPI void SetTextureFilter(const Texture& texture2D, TextureFilterMode mode);
    GAPI void GenerateTextureMipmaps(Texture& texture2D);

    GAPI void BeginShaderMode(const str& shaderName);
    GAPI void EndShaderMode();

    GAPI void SetShaderVec2(const str& shaderName, const str& location, Vec2 vec2);
    GAPI void SetShaderVec4(const str& shaderName, const str& location, Vec4 vec4);
    GAPI void SetShaderSampler2D(const str& shaderName, const str& location, const Texture2D& tex);
}

#endif //GAEMI_RENDERER_HPP
