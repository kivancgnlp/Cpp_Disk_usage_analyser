//
// Created by Kivanc Gunalp on 30.05.2026.
//

#ifndef DISK_USAGE_ANALYZER_REALDIRECTORYITERATOR_H
#define DISK_USAGE_ANALYZER_REALDIRECTORYITERATOR_H

#include <filesystem>
#include "DirectoryIteratorInterface.h"
#include <spdlog/spdlog.h>

#include "../Utils.h"


class RealDirectoryIterator : public DirectoryIteratorInterface{
    std::filesystem::directory_iterator m_dir_it;
    const std::filesystem::directory_iterator m_end_it;
    bool m_first_run = true;

    public:
    RealDirectoryIterator(const std::string &path, bool &ok) {
        std::error_code ec{};
        m_dir_it = std::filesystem::directory_iterator{path,ec};

        ok = ec == std::error_code{};
    }

    NextStatus get_next_entry(DirEntry &out) override {

                NextStatus next_status = NextStatus::Error;
                out = DirEntry{};

                do {
                        if (!m_first_run) {
                                std::error_code ec{};
                                m_dir_it.increment(ec);

                                if (ec) {
                                        spdlog::warn("Could not get next entry");
                                        next_status = NextStatus::Error;
                                        break;
                                }
                        }

                        if (m_dir_it == m_end_it) {
                                next_status = NextStatus::Exhausted;
                                break;
                        }

                        const std::filesystem::path &path_entry = m_dir_it->path();

                        if (std::filesystem::is_symlink(path_entry)) {
                                spdlog::info("symlink {}", path_entry.string());
                                out.path = path_entry;
                                out.kind = DirEntry::Kind::Symlink;
                                if (path_entry.has_extension()) {
                                        out.ext = path_entry.extension();
                                }
                                next_status = NextStatus::Entry;
                                break;

                        }


                        if (is_directory(path_entry)) {
                                spdlog::debug("Processing directory : {}", path_entry.string());
                                out.kind = DirEntry::Kind::Directory;
                                out.path = path_entry;
                                next_status = NextStatus::Entry;
                                break;
                        }

                        if (is_regular_file(path_entry)) {
                                spdlog::log(spdlog::level::debug,"Processing file : {}", path_entry.string());

                                out.kind = DirEntry::Kind::File;
                                out.path = path_entry;

                                if (path_entry.has_extension()) {
                                        out.ext = path_entry.extension();
                                }

                                out.size = get_file_size(path_entry);
                                next_status = NextStatus::Entry;
                                break;
                        }

                        spdlog::warn("Unknown entry : {}", path_entry.string());
                        out.kind = DirEntry::Kind::Unknown;
                        next_status = NextStatus::Entry;


                }while (false);


        m_first_run = false;
        return next_status;
    }
};


#endif //DISK_USAGE_ANALYZER_REALDIRECTORYITERATOR_H