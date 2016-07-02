#ifndef CL_INCLUDE_FLAG_ARG_H
#define CL_INCLUDE_FLAG_ARG_H

#include "include/str_arg.h"

namespace cl {

struct FlagReader {
  bool operator()(std::istream& is, bool& b) const {
    (void)is;
    return (b = true);
  }
};

typedef StrArg<bool, FlagReader, StrWriter<bool>, 0> FlagArg;

} // namespace cl

#endif
