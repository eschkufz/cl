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
int num_errors() {
  return count(Args::error_begin(), Args::error_end());
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

// Check that groups and args are organized correctly in arg table
TEST(arg_table, size) {
  // One arg is declared globally in global.cc
  EXPECT_EQ(num_groups(), 0);
  EXPECT_EQ(num_args(), 1);

  auto& g1 = Group::create("g");
  EXPECT_EQ(num_groups(), 1);
  EXPECT_EQ(num_args(&g1), 0);

  FlagArg::create("f");
  EXPECT_EQ(num_groups(), 1);
  EXPECT_EQ(num_args(&g1), 1);
}

// Check the rules for arg naming
TEST(arg_base, name) {
  auto& f1 = FlagArg::create("");
  EXPECT_TRUE(f1.matches("-"));

  auto& f2 = FlagArg::create("-")
    .alias("--")
    .alias("---");
  EXPECT_TRUE(f2.matches("-"));
  EXPECT_TRUE(f2.matches("--"));
  EXPECT_TRUE(f2.matches("---"));

  auto& f3 = FlagArg::create("a")
    .alias("aa");
  EXPECT_TRUE(f3.matches("-a"));
  EXPECT_TRUE(f3.matches("--aa"));
}

// Check unsuccessful arg parsing
TEST(arg, read_fail) {
  Arg<int>::create("int1");
  Args::read(2, argv({"--int1", "a"}));
  EXPECT_EQ(num_errors(), 1);
}

// Check successful arg parsing
TEST(arg, read_succ) {
  auto& int2 = Arg<int>::create("int2").initial(0);
  Args::read(2, argv({"--int2", "20"}));
  EXPECT_EQ(num_errors(), 0);
  EXPECT_EQ(int2.value(), 20);
}

// Check flag parsing
TEST(flag_arg, read) {
  auto& flag = FlagArg::create("flag");
  Args::read(1, argv({"--flag"}));
  EXPECT_TRUE(flag.value());
}

// Check file parsing
TEST(file_arg, read_succ) {
  auto& int3 = FileArg<int>::create("int3").initial(0);
  Args::read(2, argv({"--int3", "./test/data/ints/int_1.dat"}));
  EXPECT_EQ(num_errors(), 0);
  EXPECT_EQ(int3.value(), 1);
}


