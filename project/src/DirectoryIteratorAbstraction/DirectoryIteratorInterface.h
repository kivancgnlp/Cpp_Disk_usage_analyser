//
// Created by Kivanc Gunalp on 30.05.2026.
//

#ifndef DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H
#define DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H

#include <string>
#include <optional>   // [ADD] — std::optional<std::string> ext, optional<uintmax_t> size
#include <cstdint>    // [ADD] — std::uintmax_t
#include <memory>     // [ADD] — std::unique_ptr getInstance return

struct DirEntry {
    std::string path;
    std::optional<std::string> ext;
    std::optional<std::uintmax_t> size{};

    enum class Kind {Unknown, File, Directory, Symlink };
    Kind kind{Kind::Unknown};

};

class DirectoryIteratorInterface {

    public:
    virtual ~DirectoryIteratorInterface() = default;

    enum class NextStatus { Entry, Exhausted, Error };
    // returns Entry (+fills out), Exhausted (stop), Error (a failure — count it and return)
    virtual NextStatus get_next_entry(DirEntry& out) = 0;
};

class DirectoryIteratorFactoryInterface {

public:
    virtual ~DirectoryIteratorFactoryInterface() = default;
    virtual std::unique_ptr<DirectoryIteratorInterface> getInstance(const std::string &path) = 0;

};

class RealDirectoryIteratorFactory : public DirectoryIteratorFactoryInterface {

public:
    std::unique_ptr<DirectoryIteratorInterface> getInstance(const std::string &path) override;
};


#endif //DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H