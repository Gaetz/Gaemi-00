//
// Created by gaetz on 05/11/2022.
//

#include <cmath>
#include "Renderer.hpp"
#include "raylib.h"
#include "AssetsManager.hpp"

namespace render {
    void BeginDraw() {
        BeginDrawing();
    }

    void ClearScreen() {
        ClearBackground(BLACK);
    }

    void EndDraw() {
        EndDrawing();
    }

    void DrawTexture(const Texture& texture, i32 x, i32 y, Color tint) {
        ::DrawTexture(texture, x, y, tint);
    }

    void DrawSprite(const Texture& texture2D, Rect srcRect, Rect dstRect, Color tint) {
        ::DrawTexturePro(texture2D, srcRect.ToRaylib(), dstRect.ToRaylib(), { 0, 0 }, 0, tint);
    }

    void DrawLine(const Vec2& origin, const Vec2& direction, f32 length, Color color) {
        const Vec2 end = (origin + (direction * length));
        ::DrawLine(gmath::Round(origin.x), gmath::Round(origin.y),
                   gmath::Round(end.x), gmath::Round(end.y), color);
    }

    void DrawRectangleLines(const Rect& rect, Color color) {
        Rect rectInt = rect.ToRectInt();
        ::DrawRectangleLines(static_cast<i32>(rectInt.x), static_cast<i32>(rectInt.y),
                             static_cast<i32>(rectInt.width), static_cast<i32>(rectInt.height), color);
    }

    void DrawDefaultText(const str& text, const Vec2& pos, i32 fontSize, Color color) {
        ::DrawText(text.c_str(), gmath::Round(pos.x), gmath::Round(pos.y), fontSize, color);
    }

    void BeginShaderMode(const str& shaderName) {
        ::BeginShaderMode(AssetsManager::GetShader(shaderName));
    }

    void EndShaderMode() {
        ::EndShaderMode();
    }

    void SetTextureWrap(const Texture& texture2D, TextureWrapMode mode) {
        ::SetTextureWrap(texture2D, static_cast<int>(mode));
    }

    void SetTextureFilter(const Texture& texture2D, TextureFilterMode mode) {
        ::SetTextureFilter(texture2D, static_cast<int>(mode));
    }

    void SetShaderInt(const str& shaderName, const str& location, i32 value) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &value, SHADER_UNIFORM_INT);
    }

    void SetShaderFloat(const str& shaderName, const str& location, f32 value) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &value, SHADER_UNIFORM_FLOAT);
    }

    void SetShaderVec2(const str& shaderName, const str& location, Vec2 vec2) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        const auto value = vec2.ToRaylib();
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &value, SHADER_UNIFORM_VEC2);
    }

    void SetShaderVec3(const str& shaderName, const str& location, Vec3 vec3) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        const auto value = vec3.ToRaylib();
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &value, SHADER_UNIFORM_VEC3);
    }

    void SetShaderVec4(const str& shaderName, const str& location, Vec4 vec4) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        const auto value = vec4.ToRaylib();
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &value, SHADER_UNIFORM_VEC4);
    }

    void SetShaderSampler2D(const str& shaderName, const str& location, const Texture2D& tex) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        ::SetShaderValueTexture(shader, GetShaderLocation(shader, location.c_str()), tex);
    }

    void SetShaderCubemapOnModel(Model& model, const str& shaderName, const str& location, const TextureCubemap& tex) {
        const Shader& shader = AssetsManager::GetShader(shaderName);
        i32 mapId { MATERIAL_MAP_CUBEMAP };
        ::SetShaderValue(shader, GetShaderLocation(shader, location.c_str()), &mapId, SHADER_UNIFORM_INT);
        model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = tex;
    }

    void GenerateTextureMipmaps(Texture& tex) {
        ::GenTextureMipmaps(&tex);
    }


}