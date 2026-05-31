//
// Created by Kivanc Gunalp on 30.05.2026.
//

#ifndef DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H
#define DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H
#include <string>

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

class DirectoryIteratorFactory {

    bool m_mock;
    public:
    explicit DirectoryIteratorFactory(bool mock) : m_mock(mock) {}

    std::unique_ptr<DirectoryIteratorInterface> getInstance(const std::string &path);

};


#endif //DISK_USAGE_ANALYZER_DIRECTORYITERATORINTERFACE_H