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

void AssetsManager::Initialize(const str& gameAssetsPath) {
    // Setting default folders
    if (!Folder::Exists(gameAssetsPath)) {
        LOG(LogLevel::Warning) << "No data in game data path";
        return;
    }
    File::SetGameAssetsPath(gameAssetsPath);
    // Default image for invalid textures
    LoadSystemTexture(defaultTextureId, "invalid-texture.png");
}

void AssetsManager::LoadTexture(const str& name, const str& filename, i32 sceneId) {
    Texture texture = ::LoadTexture((gfile::FileTypePath(gfile::FileType::Texture)+filename).c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(sceneId, name);
}

void AssetsManager::LoadSystemTexture(const str &name, const str &filename) {
    Texture texture = ::LoadTexture((gfile::FileTypePath(gfile::FileType::System)+filename).c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(-1, name);
}


Texture AssetsManager::GetTexture(const str& name) {
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

void AssetsManager::LoadData() {

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



