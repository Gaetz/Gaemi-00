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
        /**
         * Check if a folder exists
         * @param path The path to the folder
         * @return True if the folder exists, false otherwise
         */
        static bool Exists(const str& path);

        /**
         * Explore files in a folder
         * @param path The path to the folder
         * @return A vector of file paths present in the folder
         */
        static vector<str> ExploreFiles(const str& path);
    };
}



#endif //GFILE_FOLDER_HPP
