//
// Test-only fake: an in-memory directory tree that lets a test script
// exactly which entries each directory yields, and in what order.
// Lives in test/ so it never ships in the production binary.
//
#ifndef DISK_USAGE_ANALYZER_FAKEDIRECTORYITERATOR_H
#define DISK_USAGE_ANALYZER_FAKEDIRECTORYITERATOR_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "../src/DirectoryIteratorAbstraction/DirectoryIteratorInterface.h"

// A fake iterator over a fixed, pre-built list of entries for one directory.
// Yields them in the exact order given — including adversarial orders that a
// real filesystem would never let you reproduce on demand.
class FakeDirectoryIterator : public DirectoryIteratorInterface {
    std::vector<DirEntry> m_entries;
    std::size_t m_pos = 0;
public:
    explicit FakeDirectoryIterator(std::vector<DirEntry> entries)
        : m_entries(std::move(entries)) {}

    NextStatus get_next_entry(DirEntry& out) override {
        if (m_pos >= m_entries.size()) return NextStatus::Exhausted;
        out = m_entries[m_pos++];
        return NextStatus::Entry;
    }
};

// Factory backed by a map: path -> the entries that directory contains.
// getInstance returns nullptr for an unknown path (mirrors "couldn't open").
class FakeDirectoryIteratorFactory : public DirectoryIteratorFactoryInterface {
    std::map<std::string, std::vector<DirEntry>> m_tree;
public:
    explicit FakeDirectoryIteratorFactory(std::map<std::string, std::vector<DirEntry>> tree)
        : m_tree(std::move(tree)) {}

    std::unique_ptr<DirectoryIteratorInterface> getInstance(const std::string& path) override {
        auto it = m_tree.find(path);
        if (it == m_tree.end()) return nullptr;
        return std::make_unique<FakeDirectoryIterator>(it->second);
    }
};

// small helpers to build entries readably in tests
inline DirEntry make_file(const std::string& path, std::uintmax_t size,
                          std::optional<std::string> ext = std::nullopt) {
    DirEntry e;
    e.path = path; e.kind = DirEntry::Kind::File; e.size = size; e.ext = ext;
    return e;
}
inline DirEntry make_dir(const std::string& path) {
    DirEntry e;
    e.path = path; e.kind = DirEntry::Kind::Directory;
    return e;
}

#endif
