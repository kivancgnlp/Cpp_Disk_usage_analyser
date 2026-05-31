//
// Created by Kivanc Gunalp on 30.05.2026.
//


#include "RealDirectoryIterator.h"

std::unique_ptr<DirectoryIteratorInterface> RealDirectoryIteratorFactory::getInstance(const std::string &path) {

    bool ok{};
    std::unique_ptr<DirectoryIteratorInterface> dii = std::make_unique<RealDirectoryIterator>(path,ok);

    if (ok) {
        return dii;
    }else {
        return nullptr;
    }


}
