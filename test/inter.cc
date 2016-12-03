#include "gtest/gtest.h"
#include "include/cl.h"

using namespace cl;
using namespace std;

// Check the visibility of args defined in other translation units
TEST(inter, visible) {
  const auto itr = Args::arg_find("--global");
  EXPECT_NE(itr, Args::arg_end());
}

