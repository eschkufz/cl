#ifndef CL_INCLUDE_VAL_READER_H
#define CL_INCLUDE_VAL_READER_H

#include <iostream>

namespace cl {

template <typename T>
struct ValReader {
  bool operator()(std::istream& is, T& t) const {
    is >> t;
    return !is.fail();
  }
};  

} // namespace cl

#endif
