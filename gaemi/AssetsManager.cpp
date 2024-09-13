//
// Created by gaetz on 06/11/2022.
//

#include "AssetsManager.hpp"
#include "File.hpp"
#include "Folder.hpp"
#include "Log.hpp"

using gfile::Folder;
using gfile::File;
using gfile::FileType;

unordered_map<str, Texture> AssetsManager::textures {};
unordered_map<i32, str> AssetsManager::sceneLoadedTextures {};
unordered_map<str, f32> AssetsManager::data {};
unordered_map<str, Shader> AssetsManager::shaders {};
unordered_map<str, Model3D> AssetsManager::models {};
unordered_map<i32, str> AssetsManager::sceneLoadedModels {};

void AssetsManager::Initialize(const str& gameAssetsPath) {
    // Setting default folders
    if (!Folder::Exists(gameAssetsPath)) {
        LOG(LogLevel::Warning) << "No data in game data path";
        return;
    }
    File::SetGameAssetsPath(gameAssetsPath);
    // Default assets
    LoadSystemTexture(defaultTextureId, "invalid-texture.png");
    LoadSystemModel(defaultModelId);
}

void AssetsManager::LoadTexture(const str& name, const str& filename, i32 sceneId) {
    str path = gfile::FileTypePath(gfile::FileType::Texture)+filename;
    Texture texture = ::LoadTexture(path.c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(sceneId, name);
}

void AssetsManager::LoadTextureCubemap(const str& name, const str& filename, CubemapTextureLayout layout, i32 sceneId) {
    str path = gfile::FileTypePath(gfile::FileType::Texture)+filename;
    Image cubemapImage = ::LoadImage(path.c_str());
    Texture cubemapTex = ::LoadTextureCubemap(cubemapImage, static_cast<i32>(layout));
    ::UnloadImage(cubemapImage);
    textures.emplace(name, cubemapTex);
    sceneLoadedTextures.emplace(sceneId, name);
}


void AssetsManager::LoadSystemTexture(const str &name, const str &filename) {
    str path = gfile::FileTypePath(gfile::FileType::System)+filename;
    Texture texture = ::LoadTexture(path.c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(-1, name);
}


Texture& AssetsManager::GetTexture(const str& name) {
    if (!textures.contains(name)) {
        LOG(LogLevel::Error) << "Texture [" << name << "] does not exist in AssetsManager. Returning default texture.";
        return textures.at(defaultTextureId);
    }
    return textures.at(name);
}

void AssetsManager::UnloadSceneTextures(i32 sceneId) {
    for (auto& textureId : sceneLoadedTextures) {
        if (textureId.first == sceneId) {
            UnloadTexture(GetTexture(textureId.second));
            textures.erase(textureId.second);
        }
    }
}

Texture AssetsManager::GenerateTexture(i32 width, i32 height, Color color) {
    Image image = GenImageColor(width, height, color);
    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}


void AssetsManager::LoadData() {

    if (!Folder::Exists(gfile::FileTypePath(FileType::Data))) {
        LOG(LogLevel::Warning) << "No data in game data path";
        return;
    }

    vector<str> dataFiles = Folder::ExploreFiles(gfile::FileTypePath(FileType::Data));
    for (const str& file : dataFiles) {
        const unordered_map<str, f32> fileData { std::move(File::ReadFile(file)) };
        data.insert(fileData.begin(), fileData.end());
    }

//    const unordered_map<str, f32> playerData { std::move(File::ReadFile(gfile::FileType::Data, "player.data")) };
//    data.insert(playerData.begin(), playerData.end());
//    const unordered_map<str, f32> physicsData { std::move(File::ReadFile(gfile::FileType::Data, "physics.data")) };
//    data.insert(physicsData.begin(), physicsData.end());
}

f32 AssetsManager::GetData(const str &name) {
    if (!data.contains(name)) {
        LOG(LogLevel::Error) << "Data value [" << name << "] does not exist in AssetsManager. Returning 0.";
        return 0;
    }
    return data.at(name);
}

void AssetsManager::LoadShader(const str& name, const str& vsFilename, const str& fsFilename) {
    const str vsPath = gfile::FileTypePath(FileType::Shader) + vsFilename;
    const str fsPath = gfile::FileTypePath(FileType::Shader) + fsFilename;
    shaders[name] = ::LoadShader(vsPath.c_str(), fsPath.c_str());
}

void AssetsManager::LoadVertexShader(const str& name, const str& vsFilename) {
    const str vsPath = gfile::FileTypePath(FileType::Shader) + vsFilename;
    shaders[name] = ::LoadShader(vsPath.c_str(), nullptr);
}

void AssetsManager::LoadFragmentShader(const str& name, const str& fsFilename) {
    const str fsPath = gfile::FileTypePath(FileType::Shader) + fsFilename;
    shaders[name] = ::LoadShader(nullptr, fsPath.c_str());
}

Shader AssetsManager::GetShader(const str& name) {
    if (!shaders.contains(name)) {
        LOG(LogLevel::Error) << "Shader [" << name << "] does not exist in AssetsManager.";
    }
    return shaders.at(name);
}

void AssetsManager::LoadModel(const str& name, const str& filename, i32 sceneId) {
    const str path = gfile::FileTypePath(gfile::FileType::Model)+filename;
    Model3D model { ::LoadModel(path.c_str()) };
    models.emplace(name, model);
    sceneLoadedModels.emplace(sceneId, name);
}

void AssetsManager::LoadSystemModel(const str &name) {
    const Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model3D defaultCube { ::LoadModelFromMesh(cube) };
    models.emplace(name, defaultCube);
    sceneLoadedModels.emplace(-1, name);
}


Model3D& AssetsManager::GetModel(const str& name) {
    if (!models.contains(name)) {
        LOG(LogLevel::Error) << "Model [" << name << "] does not exist in AssetsManager. Returning default model.";
        return models.at(defaultModelId);
    }
    return models.at(name);
}

void AssetsManager::UnloadSceneModels(i32 sceneId) {
    for (auto& modelId : sceneLoadedModels) {
        if (modelId.first == sceneId) {
            GetModel(modelId.second).Unload();
            models.erase(modelId.second);
        }
    }
}

Model3D AssetsManager::GenerateCube(f32 sizeX, f32 sizeY, f32 sizeZ) {
    Mesh cube = ::GenMeshCube(sizeX, sizeY, sizeZ);
    Model3D model { ::LoadModelFromMesh(cube) };
    return model;
}

Model3D AssetsManager::GenerateSphere(f32 radius, i32 rings, i32 slices) {
    Mesh sphere = ::GenMeshSphere(radius, rings, slices);
    Model3D model { ::LoadModelFromMesh(sphere) };
    return model;
}

