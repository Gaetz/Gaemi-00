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
        static unordered_map<str, f32> ReadFile(FileType fileType, const str& filename);
        static unordered_map<str, f32> ReadFile(const str& path);

        static void SetGameAssetsPath(const str& path);
        [[nodiscard]] static str GetGameAssetsPath() ;

    private:
        static str gameAssetsPath;
    };

}


#endif //GFILE_FILE_HPP
