#ifndef CL_INCLUDE_FILE_ARG_H
#define CL_INCLUDE_FILE_ARG_H

#include <fstream>
#include "val_arg.h"

namespace cl {

template <typename T, typename R>
struct FileReader {
  bool operator()(std::istream& is, T& t) const {
    std::string path = "";
    is >> path;
    if (is.fail()) {
      return false;
    }

    std::ifstream ifs(path);
    if (!ifs.is_open()) {
      return false;
    }
    return R()(ifs, t);
  }
};

template <typename T, typename R = ValReader<T>, typename W = ValWriter<T>>
using FileArg = ValArg<T, FileReader<T, R>, W>; 

} // namespace cl

#endif
