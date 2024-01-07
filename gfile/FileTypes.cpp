//
// Created by gaetz on 29/08/2023.
//

#include "FileTypes.hpp"
#include "File.hpp"

namespace gfile {
    str FileTypePath(FileType fileType) {
        switch (fileType) {
            case FileType::Data: {
                return gfile::File::GetGameAssetsPath() + "/data/";
            }
            case FileType::System: {
                return gfile::File::GetGameAssetsPath() + "/system/";
            }
            case FileType::Texture: {
                return gfile::File::GetGameAssetsPath() + "/images/";
            }
            default: {
                return "";
            }
        }
    }
}