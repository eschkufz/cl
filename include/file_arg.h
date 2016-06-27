#ifndef CL_INCLUDE_FILE_ARG_H
#define CL_INCLUDE_FILE_ARG_H

#include <fstream>
#include "include/val_arg.h"

namespace cl {

template <typename T, typename R>
struct FileReader {
  bool operator()(std::istream& is, T& t) const {
    std::string path = "";
    is >> path;
    std::ifstream ifs(path);
    return ifs.is_open() ? R()(ifs, t) : false;
  }
};

template <typename T, typename R = ValReader<T>, typename W = ValWriter<T>>
using FileArg = ValArg<T, FileReader<T, R>, W>; 

} // namespace cl

#endif
