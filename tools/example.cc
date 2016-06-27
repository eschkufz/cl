#include "include/cl.h"

using namespace cl;
using namespace std;

template <>
struct ValWriter<vector<int>> {
  void operator()(ostream& os, const vector<int>& vs) {
    os << "{";
    for (auto i : vs) {
      os << " " << i;
    }
    os << " }";
  }
};

auto& g = Group::create("Simple arguments");
auto& f = FlagArg::create("--flag")
  .alias("-f")
  .description("A boolean flag");
auto& i1 = ValArg<int>::create("--int")
  .description("An integer value");
auto& i2 = FileArg<int>::create("--int-file")
  .description("A file that contains an integer value");
auto& i3 = DirArg<vector<int>>::create("--int-dir")
  .description("A directory that contains files with integer values");

int main(int argc, char** argv) {
  Simple::read(argc, argv);
  return 0;
}
