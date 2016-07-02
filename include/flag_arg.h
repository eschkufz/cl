#ifndef CL_INCLUDE_FLAG_ARG_H
#define CL_INCLUDE_FLAG_ARG_H

#include "include/str_arg.h"

namespace cl {

template <>
struct StrReader<bool> {
  bool operator()(std::istream& is, bool& b) const {
    (void)is;
    return (b = true);
  }
};
template <>
struct StrWriter<bool> {
  void operator()(std::ostream& os, const bool& b) const {
    os << (b ? "true" : "false");
  }
};

typedef StrArg<bool, StrReader<bool>, StrWriter<bool>, 0> FlagArg;

} // namespace cl

#endif
