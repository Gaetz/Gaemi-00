//
// Created by gaetz on 30/07/2023.
//

#include "File.hpp"

#include <sstream>
#include <fstream>
namespace gfile {

    str File::gameAssetsPath;

    unordered_map<str, f32> File::ReadFile(FileType fileType, const str &filename) {
        str path = FileTypePath(fileType) + filename;
        return ReadFile(path);
    }

    unordered_map<str, f32> File::ReadFile(const str &path) {
        unordered_map<str, f32> fileData;
        std::ifstream file { path };
        vector<str> lines {};
        str fileLine;
        while (std::getline(file, fileLine)) {
            lines.push_back(fileLine);
        }
        file.close();

        for (auto& line : lines) {
            std::istringstream lineStream { line };
            vector<str> words;
            str word;
            while (std::getline(lineStream, word, ' ')) {
                words.push_back(word);
            }
            fileData.emplace(words[0], std::stof(words[1]));
        }
        return fileData;
    }

    void File::SetGameAssetsPath(const str &path) {
        gameAssetsPath = path;
    }

    str File::GetGameAssetsPath() {
        return gameAssetsPath;
    }

}