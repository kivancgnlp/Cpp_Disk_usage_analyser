//
// Created by Kivanc Gunalp on 30.05.2026.
//

#include "Traverser.h"
#include <spdlog/spdlog.h>


void list_files_in_the_directory(const std::string& path_base, Stats &stats,Stats &global_stats, unsigned current_depth, DirectoryIteratorFactoryInterface &dir_it_fact) {
        spdlog::debug("Listing the files in the directory {}", path_base);

        auto dir_it = dir_it_fact.getInstance(path_base);

        if (!dir_it) {
                spdlog::warn("Error in listing the files in the directory {}", path_base);
                stats.increment_directory_info_error_count();
                return;
        }


        while (true) {
                DirEntry dir_entry;
                auto ne = dir_it->get_next_entry(dir_entry);

                if (ne == DirectoryIteratorInterface::NextStatus::Exhausted) {
                        break;
                }

                if (ne == DirectoryIteratorInterface::NextStatus::Error) {
                        stats.increment_directory_info_error_count();
                        break;
                }

                assert(ne == DirectoryIteratorInterface::NextStatus::Entry);

                if (dir_entry.kind == DirEntry::Kind::Symlink) {
                        spdlog::info("skipping symlink {}", dir_entry.path);
                        continue;
                }


                if (dir_entry.kind == DirEntry::Kind::File) {
                        spdlog::log(spdlog::level::debug,"Processing file : {}", dir_entry.path);

                        Stats &current_stats = current_depth == 0 ? global_stats : stats;

                        if (std::optional<std::uintmax_t> size = dir_entry.size) {

                                current_stats.add_file_size_stat(dir_entry.path,size.value());

                                if (dir_entry.ext) {
                                        current_stats.update_extension_stats(dir_entry.ext.value(), size.value());
                                }
                        }else {
                                current_stats.increment_file_get_size_error_count();

                        }

                }else if (dir_entry.kind == DirEntry::Kind::Directory) {
                        spdlog::debug("Processing directory : {}", dir_entry.path);
                        list_files_in_the_directory(dir_entry.path, stats,global_stats, current_depth + 1, dir_it_fact);

                        // Top-level folders (encountered at depth 0) belong to global directly;
                        // nested folders accumulate in stats and ride the end-of-frame migrate.
                        Stats &folder_count_target = current_depth == 0 ? global_stats : stats;
                        folder_count_target.increment_processed_folder_count();

                }else {
                        spdlog::warn("Unknown entry : {}", dir_entry.path);
                        stats.increment_unclassified_entries_count();

                }




        }

        // Flush a completed top-level folder's subtree into global exactly once,
        // after the whole directory has been walked. Doing this at end-of-frame
        // (rather than mid-loop when a child dir is seen) ensures files that
        // appear after a subdirectory, and folders with no subdirectory at all,
        // are never stranded.
        if (current_depth == 1) {
                spdlog::info("Processing completed for folder : {}, folder stats : {}", path_base, stats.to_string<false>());
                global_stats.migrate_stats(std::move(stats));
        }

}
