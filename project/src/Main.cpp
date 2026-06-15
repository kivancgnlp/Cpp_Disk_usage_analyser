

#include <filesystem>

#include <iostream>
#include <ostream>
#include <clocale>

#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <cpptrace/from_current.hpp>

#include "Stats.h"
#include "Traverser.h"
#include "DirectoryIteratorAbstraction/DirectoryIteratorInterface.h"


int main(int argc, const char * argv[]) {

#ifdef _WIN32
        // Make std::filesystem::path::string() produce UTF-8 on Windows 10 1903+.
        // Without this, the system ANSI code page is used, corrupting non-ASCII paths.
        std::setlocale(LC_ALL, ".UTF-8");
#endif

        CLI::App app{"Kiv disk usage analyzer"};

        std::string path_str = ".";
        app.add_option("-p,--path,path", path_str, "Path to files");
        CLI11_PARSE(app, argc, argv);
        spdlog::info("Calculating disk usage starting with base folder {}", path_str);

        Stats stats{};
        Stats global_stats{};

        CPPTRACE_TRY {
                RealDirectoryIteratorFactory dir_it_fact{};
                list_files_in_the_directory(path_str, stats,global_stats,0,dir_it_fact);
        } CPPTRACE_CATCH(const std::exception& e) {
                auto trace = cpptrace::from_current_exception().to_string();
                spdlog::error("Exception: {}\n{}", e.what(), trace);
        }


        spdlog::info("Total stats : {}", global_stats.to_string<true>());

}
