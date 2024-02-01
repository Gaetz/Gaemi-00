//
// Created by gaetz on 07/01/24.
//

#ifndef GFILE_FOLDER_HPP
#define GFILE_FOLDER_HPP

#include "Defines.hpp"
#include <filesystem>

namespace gfile {
    class Folder {
    public:
        static bool Exists(const str& path);
        static vector<str> ExploreFiles(const str& path);
    };
}



#endif //GFILE_FOLDER_HPP
