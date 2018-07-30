#include "levenshtein.hpp"

#include <gtest/gtest.h>
#include <string>

using namespace std;
using levenshtein::levenshtein_distance;

TEST(levenshtein_test, equal)
{
  EXPECT_EQ(levenshtein_distance("hello"s, "hello"s), 0);
}

TEST(levenshtein_distance_test, insert)
{
  EXPECT_EQ(levenshtein_distance("isrt"s, "insert"s), 2);
  EXPECT_EQ(levenshtein_distance("insert"s, "i"s), 5);
}

TEST(levenshtein_distance_test, replace)
{
  EXPECT_EQ(levenshtein_distance("reXlace"s, "replace"s), 1);
  EXPECT_EQ(levenshtein_distance("replace"s, "rXplaXe"s), 2);
}

TEST(levenshtein_distance_test, del)
{
  EXPECT_EQ(levenshtein_distance("dlt"s, "delete"s), 3);
  EXPECT_EQ(levenshtein_distance("delete"s, "delte"s), 1);
}

TEST(levenshtein_distance_test, empty)
{
  EXPECT_EQ(levenshtein_distance("empty"s, ""s), 5);
  EXPECT_EQ(levenshtein_distance(""s, "empty"s), 5);
}

TEST(levenshtein_distance_test, wide_char)
{
  EXPECT_EQ(levenshtein_distance(
    L"this is a test"s, L"this is also a TEST"s), 9);
}

TEST(levenshtein_distance_test, all_ops_combined)
{
  EXPECT_EQ(levenshtein_distance("hello world"s, "helo wooXld"s), 3);
  EXPECT_EQ(levenshtein_distance(
    "this is not a love song"s, "this zzis not a lv xong"s), 5);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
