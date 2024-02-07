//
// Created by gaetz on 30/07/2023.
//

#ifndef GFILE_FILETYPES_HPP
#define GFILE_FILETYPES_HPP

#include "Defines.hpp"

namespace gfile {

    /**
     * The type of file to read.
     * Will be used to determine the file path.
     */
    enum class FileType {
        Data = 0,
        System = 1,
        Texture = 2,
        Shader = 3,
    };

    /**
     * Get the path to the file type
     * @param fileType The type of file to get the path for
     * @return The path to the file type
     */
    [[nodiscard]]
    str FileTypePath(FileType fileType);

}

#endif //GFILE_FILETYPES_HPP
