#include <iterator>
#include "include/cl.h"

using namespace cl;
using namespace std;

auto& g1 = Group::create("Native Arguments");
auto& f = FlagArg::create("--flag")
  .alias("-f")
  .description("A boolean flag");
auto& i1 = StrArg<int>::create("--int")
  .usage("<int>")
  .description("An integer value");

struct C {
  int x;
  int y;
  int z;
};

namespace cl {
template <>
struct StrReader<C> {
  bool operator()(istream& is, C& c) const {
    is >> c.x >> c.y >> c.z;
    return !is.fail();
  }
};
template <>
struct StrWriter<C> {
  void operator()(ostream& os, const C& c) const {
    os << c.x << " " << c.y << " " << c.z;
  }
};
}

auto& g2 = Group::create("Complex Arguments");
auto& c = StrArg<C>::create("--complex")
  .usage("<int> <int> <int>")
  .description("A triplet of ints");

namespace cl {
template <>
struct StrReader<vector<int>> {
  bool operator()(istream& is, vector<int>& v) const {
    copy(istream_iterator<int>(is), istream_iterator<int>(), back_inserter(v));
    return true;
  }
};
template <>
struct StrWriter<vector<int>> {
  void operator()(ostream& os, const vector<int>& v) const {
    os << "{ ";
    copy(v.begin(), v.end(), ostream_iterator<int>(os, " "));
    os << "}";
  }
};
}

auto& g3 = Group::create("Higher-Order Arguments");
auto& i2 = FileArg<int>::create("--int-file")
  .description("A file that contains an integer value");
auto& i3 = DirArg<vector<int>>::create("--int-dir")
  .description("A directory that contains files with integer values");

int main(int argc, char** argv) {
  Simple::read(argc, argv);
  return 0;
}
