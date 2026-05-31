//
// Created by Kivanc Gunalp on 30.05.2026.
//

#ifndef DISK_USAGE_ANALYZER_TRAVERSER_H
#define DISK_USAGE_ANALYZER_TRAVERSER_H

#include "Stats.h"
#include "DirectoryIteratorAbstraction/DirectoryIteratorInterface.h"
#include <string>

void list_files_in_the_directory(const std::string& path_base, Stats &stats,Stats &global_stats, unsigned current_depth, DirectoryIteratorFactoryInterface &dir_it_fact);
#endif //DISK_USAGE_ANALYZER_TRAVERSER_H