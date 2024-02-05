//
// Created by gaetz on 30/07/2023.
//

#ifndef GFILE_FILE_HPP
#define GFILE_FILE_HPP

#include "Defines.hpp"
#include "FileTypes.hpp"

#include <unordered_map>
using std::unordered_map;

namespace gfile {

    class File {
    public:
        /**
         * Read a file and return a map of the file data
         * @details The file data is expected to be in the format of "key value" pairs, separated by a space
         * @param fileType The type of file to read, will be used to determine the file path
         * @param filename The name of the file to read
         * @return A map of the file data
         */
        static unordered_map<str, f32> ReadFile(FileType fileType, const str& filename);

        /**
         * Read a file and return a map of the file data
         * @details The file data is expected to be in the format of "key value" pairs, separated by a space
         * @param path Direct path to the file
         * @return A map of the file data
         */
        static unordered_map<str, f32> ReadFile(const str& path);

        /**
         * At startup, set the path to the game assets
         * @param path The path to the game assets
         */
        static void SetGameAssetsPath(const str& path);

        /**
         * Get the path to the game assets
         * @return The path to the game assets
         */
        [[nodiscard]] static str GetGameAssetsPath() ;

    private:
        /** Stores the path to the game assets */
        static str gameAssetsPath;
    };

}


#endif //GFILE_FILE_HPP
