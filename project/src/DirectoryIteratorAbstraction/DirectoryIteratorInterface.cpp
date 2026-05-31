//
// Created by Kivanc Gunalp on 30.05.2026.
//


#include "RealDirectoryIterator.h"

std::unique_ptr<DirectoryIteratorInterface> DirectoryIteratorFactory::getInstance(const std::string &path) {

    bool ok{};
    auto ne = std::make_unique<RealDirectoryIterator>(path,ok);

    if (ok) {
        return ne;
    }else {
        return nullptr;
    }


}
