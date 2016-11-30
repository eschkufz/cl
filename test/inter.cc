#include "gtest/gtest.h"
#include "include/cl.h"

using namespace cl;
using namespace std;

// Check the visibility of args defined in other translation units
TEST(inter, visible) {
  const auto f = [](Arg* a) {
    return std::find(a->alias_begin(), a->alias_end(), "--global") != a->alias_end();
  };
  EXPECT_NE(find_if(Args::arg_begin(), Args::arg_end(), f), Args::arg_end());
}

