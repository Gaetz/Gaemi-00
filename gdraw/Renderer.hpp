//
// Created by gaetz on 05/11/2022.
//

#ifndef GDRAW_RENDERER_HPP
#define GDRAW_RENDERER_HPP

#include "Defines.hpp"
#include "Rect.hpp"
#include "Vec4.hpp"
#include "Vec3.hpp"
#include "Cam3D.hpp"
#include <Model3D.hpp>

struct Texture;
struct Color;

using gmath::Vec2;
using gmath::Rect;
using gmath::Vec3;
using gmath::Vec4;

namespace gdraw {
    enum class TextureWrapMode
    {
        Repeat = 0,
        Clamp = 1,
        MirrorRepeat = 2,
        MirrorClamp = 3
    };

    enum class TextureFilterMode
    {
        Point = 0,
        Bilinear = 1,
        Trilinear = 2,
        Anisotropic4x = 3,
        Anisotropic8x = 4,
        Anisotropic16x = 5
    };

    void BeginDraw();
    void EndDraw();

    void BeginMode3D(const Cam3D& camera);
    void EndMode3D();

    void ClearScreen();
    void DrawPoint3D(const Vec3& position, Color color);
    void DrawTriangle3D(const Vec3& v1, const Vec3& v2, const Vec3& v3, Color color);
    void DrawTriangleStrip3D(const vector<Vec3>& points, Color color);
    void DrawGrid(i32 slices, f32 spacing);
    void DrawModel(const gassets::Model3D& model, const Vec3& position, f32 scale, Color tint);

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
    GAPI void EnableBackfaceCulling();
    GAPI void DisableBackfaceCulling();
    GAPI void EnableDepthMask();
    GAPI void DisableDepthMask();

    GAPI void SetShaderInt(const str& shaderName, const str& location, i32 value);
    GAPI void SetShaderFloat(const str& shaderName, const str& location, f32 value);
    GAPI void SetShaderVec2(const str& shaderName, const str& location, Vec2 vec2);
    GAPI void SetShaderVec3(const str& shaderName, const str& location, Vec3 vec3);
    GAPI void SetShaderVec4(const str& shaderName, const str& location, Vec4 vec4);
    GAPI void SetShaderSampler2D(const str& shaderName, const str& location, const Texture2D& tex);
    GAPI void SetShaderCubemapOnModel(gassets::Model3D& model, const str& shaderName, const str& location,
                                      const TextureCubemap& tex);
}

#endif //GDRAW_RENDERER_HPP
