#ifndef CL_INCLUDE_FLAG_READER_H
#define CL_INCLUDE_FLAG_READER_H

#include <iostream>

namespace cl {

struct FlagReader {
  bool operator()(std::istream& is, bool& b) const {
    (void)is;
    return (b = true);
  }
};

} // namespace cl

#endif
