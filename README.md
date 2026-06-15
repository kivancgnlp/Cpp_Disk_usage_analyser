# Disk Usage Analyser

A cross-platform command-line tool (similar to `du`) that recursively scans a directory tree and reports disk usage statistics: total size, per-extension breakdown, and the top 10 largest files.

## Features

- Recursive directory traversal using C++20 `std::filesystem`
- Human-readable sizes (bytes / KB / MB / GB / TB)
- Per-extension statistics sorted by accumulated size
- Top-10 largest files list
- Symlinks are skipped (not followed)
- Error counts for inaccessible directories and files
- Structured logging via [spdlog](https://github.com/gabime/spdlog)
- Stack traces on uncaught exceptions via [cpptrace](https://github.com/jeremy-rifkin/cpptrace)

## Dependencies

Managed via [vcpkg](https://github.com/microsoft/vcpkg):

| Library | Purpose |
|---------|---------|
| [CLI11](https://github.com/CLIUtils/CLI11) | Command-line argument parsing |
| [spdlog](https://github.com/gabime/spdlog) | Structured logging |
| [fmt](https://github.com/fmtlib/fmt) | String formatting |
| [cpptrace](https://github.com/jeremy-rifkin/cpptrace) | Stack traces on exception |
| [GTest](https://github.com/google/googletest) | Unit tests |

## Prerequisites

- C++20-capable compiler (GCC 11+, Clang 13+, MSVC 19.29+)
- CMake 3.5+
- vcpkg with the packages above installed

## Building

The project uses [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html). Edit `CMakePresets.json` to point `CMAKE_TOOLCHAIN_FILE` at your local vcpkg installation before building.

### macOS

```bash
cd project
cmake --preset macOS
cmake --build build/macOS
```

### Windows 10

```powershell
cd project
cmake --preset Windows10
cmake --build build/Windows10
```

> **Note:** The preset has two commented-out vcpkg triplet lines for MinGW (`x64-mingw-dynamic`). Uncomment them if you are building with MinGW instead of MSVC.

## Usage

```
disk_usage_analyzer [OPTIONS] [path]

Options:
  -p, --path TEXT   Path to analyse (default: current directory)
  -h, --help        Print help message

Positional:
  path              Equivalent to --path
```

### Examples

```bash
# Analyse the current directory
./disk_usage_analyzer

# Analyse a specific path
./disk_usage_analyzer --path /home/user/projects

# Short form
./disk_usage_analyzer -p /home/user/projects
```

### Sample output

```
[info] Calculating disk usage starting with base folder /home/user/projects
[info] Processing completed for folder : /home/user/projects/src, folder stats : size : 4.21 MB
...
[info] Total stats :
 processed_files: 312
 processed_folders: 28
 unclassified_entries: 0
 size: 87.43 MB
 File extension stats :
          .cpp     acc size :    45.10 MB    count : 198
           .h      acc size :    18.32 MB    count : 87
          ...
 Biggest files :
    23.50 MB    /home/user/projects/data/large_dataset.bin
    ...
```

## Running Tests

```bash
cd project
cmake --preset macOS          # or Windows10
cmake --build build/macOS
./build/macOS/disk_usage_analyzer_test
```

The test suite uses a `FakeDirectoryIterator` to inject in-memory directory trees, allowing deterministic testing of edge cases (e.g. files appearing after subdirectories in iteration order) that cannot be reliably reproduced on a real filesystem.

## Architecture

```
src/
  Main.cpp                                  Entry point, CLI parsing
  Traverser.{h,cpp}                         Recursive walk logic
  Stats.{h,cpp}                             Accumulates file/folder/extension stats
  Utils.{h,cpp}                             File size helpers, human-readable formatting
  DirectoryIteratorAbstraction/
    DirectoryIteratorInterface.{h,cpp}      Abstract iterator + factory interfaces
    RealDirectoryIterator.h                 std::filesystem-backed implementation

test/
  FakeDirectoryIterator.h                   In-memory iterator for unit tests
  test_traversal.cpp                        Traversal logic tests
  test_utils.cpp                            Utility and Stats tests
```

The `DirectoryIteratorInterface` abstraction decouples the traversal logic from the filesystem, making the core algorithm fully unit-testable without touching disk.

## Windows UTF-8 Notes

On Windows 10 (version 1903+), the program calls `setlocale(LC_ALL, ".UTF-8")` at startup. This makes `std::filesystem::path::string()` produce UTF-8 encoded strings rather than using the system ANSI code page, which would corrupt non-ASCII characters in file paths.

## License

MIT
