#ifndef CL_INCLUDE_VAL_WRITER_H
#define CL_INCLUDE_VAL_WRITER_H

#include <iostream>

namespace cl {

template <typename T>
struct ValWriter {
  void operator()(std::ostream& os, const T& t) const {
    os << t;
  }
};

} // namespace cl

#endif
