#include "gtest/gtest.h"

extern "C" {
#include "config.h"
#include "config.c"
}

TEST(ConfigTest, check_add_slash) {
    const char *base = "/some/directory";
    const char *expected = "/some/directory/";
    char *actual = GetDirectoryString(base);
    ASSERT_STREQ(expected, actual);
    free(actual);
    actual = NULL;
}

TEST(ConfigTest, leave_directory_slash_alone) {
    const char *base = "/some/directory/";
    char *actual = GetDirectoryString(base);
    ASSERT_STREQ(base, actual);
    free(actual);
    actual = NULL;
}

TEST(ConfigTest, check_existing_directory) {
    const char *directory = "/tmp";
    int expected = 1;
    int actual = DirectoryExists(directory);
    ASSERT_EQ(expected, actual);
}

TEST(ConfigTest, check_non_writable_directory) {
    const char *directory = "/Users";
    int expected = 0;
    int actual = DirectoryExists(directory);
    ASSERT_EQ(expected, actual);
}