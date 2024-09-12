//
// Created by gaetz on 06/11/2022.
//

#ifndef GAEMI_ASSETS_MANAGER_HPP
#define GAEMI_ASSETS_MANAGER_HPP

#include <Model3D.hpp>
#include <unordered_map>
#include "Defines.hpp"
#include "raylib.h"

using std::unordered_map;
using render::Model3D;

constexpr char defaultTextureId[] = "default";
constexpr char defaultModelId[] = "default";

enum class CubemapTextureLayout {
    AutoDetect = CUBEMAP_LAYOUT_AUTO_DETECT,
    Vertical = CUBEMAP_LAYOUT_LINE_VERTICAL,
    Horizontal = CUBEMAP_LAYOUT_LINE_HORIZONTAL,
    ThreeByFour = CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,
    FourByThree = CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,
    Equirectangular = CUBEMAP_LAYOUT_PANORAMA
};

class AssetsManager {
public:
    static GAPI void Initialize(const str& gameAssetsPath);

    static GAPI void LoadTexture(const str& name, const str& filename, i32 sceneId);
    static GAPI void LoadTextureCubemap(const str& name, const str& filename, CubemapTextureLayout layout, i32 sceneId);
    static GAPI Texture& GetTexture(const str& name);
    static GAPI void UnloadSceneTextures(i32 sceneId);
    static GAPI Texture GenerateTexture(i32 width, i32 height, Color color);

    static void LoadData();
    static GAPI f32 GetData(const str& name);

    static GAPI void LoadShader(const str& name, const str& vsFilename, const str& fsFilename);
    static GAPI void LoadVertexShader(const str& name, const str& vsFilename);
    static GAPI void LoadFragmentShader(const str& name, const str& fsFilename);
    static GAPI Shader GetShader(const str& name);

    static GAPI void LoadModel(const str& name, const str& filename, i32 sceneId);
    static GAPI Model3D& GetModel(const str& name);
    static GAPI void UnloadSceneModels(i32 sceneId);
    static GAPI Model3D GenerateCube(f32 sizeX, f32 sizeY, f32 sizeZ);

private:
    static void LoadSystemTexture(const str& name, const str& filename);
    static void LoadSystemModel(const str& name);

    static unordered_map<str, Texture> textures;
    static unordered_map<i32, str> sceneLoadedTextures;
    static unordered_map<str, Shader> shaders;
    static unordered_map<str, Model3D> models;
    static unordered_map<i32, str> sceneLoadedModels;

    static unordered_map<str, f32> data;
};


#endif //GAEMI_ASSETS_MANAGER_HPP
