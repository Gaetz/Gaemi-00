//
// Created by gaetz on 07/01/24.
//

#include "Folder.hpp"

namespace gfile {

    vector<str> Folder::ExploreFiles(const str &path) {
        vector<str> res;
        for (const auto &entry: std::filesystem::directory_iterator(path)) {
            res.push_back(entry.path());
        }
        return res;
    }

    bool Folder::Exists(const str &path) {
        return std::filesystem::exists(path);
    }
}