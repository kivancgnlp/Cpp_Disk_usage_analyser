cmake_minimum_required(VERSION 3.5...4.0)
project(disk_usage_analyzer)

set(CMAKE_CXX_STANDARD 20)

add_executable(disk_usage_analyzer
        src/Main.cpp
        src/Stats.cpp
        src/Stats.h
        src/Utils.cpp
        src/Utils.h
        src/Traverser.cpp
        src/Traverser.h
        src/DirectoryIteratorAbstraction/DirectoryIteratorInterface.cpp
        src/DirectoryIteratorAbstraction/DirectoryIteratorInterface.h
        src/DirectoryIteratorAbstraction/RealDirectoryIterator.cpp
        src/DirectoryIteratorAbstraction/RealDirectoryIterator.h)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(disk_usage_analyzer PRIVATE CLI11::CLI11)

find_package(cpptrace CONFIG REQUIRED)
target_link_libraries(disk_usage_analyzer PRIVATE cpptrace::cpptrace)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(disk_usage_analyzer PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(disk_usage_analyzer PRIVATE spdlog::spdlog)

# LibC++ hardening begin
#target_compile_definitions(disk_usage_analyzer PRIVATE _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG)
# Sanitizers (development builds)
#target_compile_options(disk_usage_analyzer PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
#target_link_options(disk_usage_analyzer PRIVATE -fsanitize=address,undefined)
# LibC++ hardening end

# GNU hardening begin
# Debug: full checks (breaks ABI — all linked code must also use _GLIBCXX_DEBUG)
#target_compile_definitions(disk_usage_analyzer PRIVATE
#        $<$<CONFIG:Debug>:_GLIBCXX_DEBUG>
#        $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>
#        $<$<CONFIG:Release>:_GLIBCXX_ASSERTIONS>
#)

# Sanitizers (development builds)
#target_compile_options(disk_usage_analyzer PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
#target_link_options(disk_usage_analyzer PRIVATE -fsanitize=address,undefined)
# GNU hardening end

# tests
find_package(GTest CONFIG REQUIRED)
add_executable(disk_usage_analyzer_test test/test_utils.cpp test/test_traversal.cpp src/Traverser.cpp src/Stats.cpp src/Utils.cpp
        src/DirectoryIteratorAbstraction/DirectoryIteratorInterface.cpp
        src/DirectoryIteratorAbstraction/DirectoryIteratorInterface.h
        test/FakeDirectoryIterator.h)
#add_executable(dua_tests tests/test_utils.cpp tests/test_stats.cpp)
target_link_libraries(disk_usage_analyzer_test PRIVATE GTest::gtest_main spdlog::spdlog)


