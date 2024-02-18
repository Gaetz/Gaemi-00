//
// Created by gaetz on 06/11/2022.
//

#ifndef GAEMI_ASSETS_MANAGER_HPP
#define GAEMI_ASSETS_MANAGER_HPP

#include <unordered_map>
#include "Defines.hpp"
#include "raylib.h"

using std::unordered_map;

constexpr char defaultTextureId[] = "default";

class AssetsManager {
public:
    static GAPI void Initialize(const str& gameAssetsPath);

    static GAPI void LoadTexture(const str& name, const str& filename, i32 sceneId);
    static GAPI Texture& GetTexture(const str& name);
    static GAPI void UnloadSceneTextures(i32 sceneId);

    static void LoadData();
    static GAPI f32 GetData(const str& name);

    static GAPI void LoadShader(const str& name, const str& vsFilename, const str& fsFilename);
    static GAPI void LoadFragmentShader(const str& name, const str& fsFilename);
    static GAPI Shader GetShader(const str& name);

private:
    static void LoadSystemTexture(const str& name, const str& filename);

    static unordered_map<str, Texture> textures;
    static unordered_map<i32, str> sceneLoadedTextures;
    static unordered_map<str, Shader> shaders;

    static unordered_map<str, f32> data;
};


#endif //GAEMI_ASSETS_MANAGER_HPP
