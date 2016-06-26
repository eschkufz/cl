#include <algorithm>
#include "gtest/gtest.h"
#include "include/cl.h"

using namespace cl;
using namespace std;

// Check the visibility of args defined in other translation units
TEST(inter, visible) {
  const auto itr = find_if(Args::arg_begin(), Args::arg_end(), [](Arg* a) {
    return a->matches("--global");
  });
  EXPECT_NE(itr, Args::arg_end());
}

