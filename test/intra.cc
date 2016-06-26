#include <initializer_list>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "include/cl.h"

using namespace cl;
using namespace std;

// Count number of elements
template <typename Itr>
int count(Itr begin, Itr end) {
  vector<typename Itr::value_type> elems(begin, end);
  return elems.size();
}
int num_groups() {
  return count(Args::group_begin(), Args::group_end());
}
int num_args() {
  return count(Args::arg_begin(), Args::arg_end());
}
int num_args(Group* g) {
  return count(g->arg_begin(), g->arg_end());
}

// Make an argv vector
char** argv(const initializer_list<string>& args) {
  static vector<char*> ret;
  ret.clear();
  for (const auto& a : args) {
    ret.push_back((char*)a.c_str());
  }
  return &ret[0];
}

// Global state: guaranteed to be initialized before any tests are invoked
auto& global = FlagArg::create("--global");

// Check that groups and args are organized correctly in arg table
TEST(arg_table, size) {
  EXPECT_EQ(num_groups(), 0);
  EXPECT_EQ(num_args(), 1);

  auto& g1 = Group::create("-g");
  EXPECT_EQ(num_groups(), 1);
  EXPECT_EQ(num_args(&g1), 0);

  FlagArg::create("-f");
  EXPECT_EQ(num_groups(), 1);
  EXPECT_EQ(num_args(&g1), 1);
}

// Check unsuccessful arg parsing
TEST(arg, read_fail) {
  auto& int1 = ValArg<int>::create("--int1");
  Args::read(2, argv({"--int1", "a"}));
  EXPECT_TRUE(int1.error());
}

// Check successful arg parsing
TEST(arg, read_succ) {
  auto& int2 = ValArg<int>::create("--int2").initial(0);
  Args::read(2, argv({"--int2", "20"}));
  EXPECT_FALSE(int2.error());
  EXPECT_EQ(int2.value(), 20);
}

// Check provided flag
TEST(arg, provided) {
  auto& int3 = ValArg<int>::create("--int3");
  EXPECT_FALSE(int3.provided());
  Args::read(2, argv({"--int3", "20"}));
  EXPECT_TRUE(int3.provided());
}

// Check duplicate flag
TEST(arg, duplicate) {
  auto& int4 = ValArg<int>::create("--int4");
  EXPECT_FALSE(int4.duplicated());
  Args::read(4, argv({"--int4", "10", "--int4", "20"}));
  EXPECT_TRUE(int4.duplicated());
}

// Check flag parsing
TEST(flag_arg, read) {
  auto& flag = FlagArg::create("--flag");
  Args::read(1, argv({"--flag"}));
  EXPECT_TRUE(flag.value());
}

// Check unsuccessful file parsing
TEST(file_arg, read_fail) {
  auto& int5 = FileArg<int>::create("--int5").initial(0);
  Args::read(2, argv({"--int5", "./path/to/nowhere/..."}));
  EXPECT_TRUE(int5.error());
}

// Check successful file parsing
TEST(file_arg, read_succ) {
  auto& int6 = FileArg<int>::create("--int6").initial(0);
  Args::read(2, argv({"--int6", "./test/data/ints/int_1.dat"}));
  EXPECT_FALSE(int6.error());
  EXPECT_EQ(int6.value(), 1);
}
