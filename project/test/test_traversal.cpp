//
// Traversal tests using the fake directory iterator.
// These exercise the ORDERING logic that a real filesystem can't reproduce
// on demand — in particular the order that previously stranded files.
//
#include <gtest/gtest.h>
#include "FakeDirectoryIterator.h"
#include "../src/Traverser.h"
#include "../src/Stats.h"

// The regression test: a top-level folder whose FILE appears AFTER its
// SUBDIRECTORY in iteration order. Under the old mid-loop migrate, the file
// after the subdir was stranded and lost. The end-of-frame migrate fixes it.
TEST(Traversal, FileAfterSubdirectoryIsNotStranded) {
    FakeDirectoryIteratorFactory factory({
        // root contains one folder "top"
        {"/root", { make_dir("/root/top") }},
        // "top" yields: subdir FIRST, then a file — the stranding order
        {"/root/top", {
            make_dir("/root/top/sub"),
            make_file("/root/top/late.txt", 500, ".txt"),
        }},
        {"/root/top/sub", {
            make_file("/root/top/sub/inner.bin", 1000, ".bin"),
        }},
    });

    Stats stats, global;
    list_files_in_the_directory("/root", stats, global, 0, factory);

    EXPECT_EQ(global.get_processed_file_count(), 2);   // late.txt + inner.bin
    EXPECT_EQ(global.get_total_size(), 1500);          // 500 + 1000, late.txt NOT lost
}

// A top-level folder with NO subdirectory at all. Under the old code its
// migrate never fired (it triggered on seeing a child dir), so its files
// were only rescued if a later folder flushed. Here it's the only folder.
TEST(Traversal, SubdirectorylessFolderIsNotLost) {
    FakeDirectoryIteratorFactory factory({
        {"/root", { make_dir("/root/flat") }},
        {"/root/flat", {
            make_file("/root/flat/a.txt", 100, ".txt"),
            make_file("/root/flat/b.txt", 200, ".txt"),
        }},
    });

    Stats stats, global;
    list_files_in_the_directory("/root", stats, global, 0, factory);

    EXPECT_EQ(global.get_processed_file_count(), 2);
    EXPECT_EQ(global.get_total_size(), 300);
}

// Root-level files (directly in the scanned dir) must be counted.
TEST(Traversal, RootLevelFilesCounted) {
    FakeDirectoryIteratorFactory factory({
        {"/root", {
            make_file("/root/r1.bin", 50, ".bin"),
            make_dir("/root/d"),
            make_file("/root/r2.bin", 70, ".bin"),  // file AFTER a dir, at root level
        }},
        {"/root/d", { make_file("/root/d/x.txt", 30, ".txt") }},
    });

    Stats stats, global;
    list_files_in_the_directory("/root", stats, global, 0, factory);

    EXPECT_EQ(global.get_processed_file_count(), 3);   // r1, r2, x
    EXPECT_EQ(global.get_total_size(), 150);           // 50+70+30
}
