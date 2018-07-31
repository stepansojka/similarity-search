#include "bk_tree.hpp"

#include <gtest/gtest.h>
#include <string>
#include <iostream>

using namespace std;
using bk::bk_tree;

TEST(tree_test, string_matching)
{
  bk_tree root("hello"s);

  root.insert(
     "the darkest places in hell are reserved for those"s
     "who maintain their neutrality in times of moral crisis"s);

  root.insert(
      "and thence we issued forth to see again the stars"s);

  root.insert(
      "there is no greater sorrow than"s
      "to be mindful of the happy time in misery"s);

  // found - exact match
  EXPECT_EQ(
    tuple(
      "there is no greater sorrow than"s
      "to be mindful of the happy time in misery"s,
      0),
    root.search(
      "there is no greater sorrow than"s
      "to be mindful of the happy time in misery"s,
      6));

  // found - distance within threshold
  EXPECT_EQ(
    tuple("and thence we issued forth to see again the stars"s, 8),
    root.search("and thence we came out to see again the stars"s, 15));

  // found - no threshold means closest match
  EXPECT_EQ(
    tuple("and thence we issued forth to see again the stars"s, 6),
    root.search("and thence we issued forth to see again the universe"s));

  // not found - threshold too low
   EXPECT_EQ(
    tuple(""s, 1),
    root.search("and thence we issued forth to see again the universe"s, 1));
}
