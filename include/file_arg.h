#ifndef CL_INCLUDE_FILE_ARG_H
#define CL_INCLUDE_FILE_ARG_H

#include <fstream>
#include "ext/stl/include/comment_stream.h"
#include "include/str_arg.h"

namespace cl {

template <typename T, typename R>
struct FileReader {
  bool operator()(std::istream& is, T& t) const {
    std::string path = "";
    is >> path;
    std::ifstream ifs(path);
    stl::comment_stream cs(ifs);
    return ifs.is_open() ? R()(cs, t) : false;
  }
};

template <typename T, typename R = StrReader<T>, typename W = StrWriter<T>>
using FileArg = StrArg<T, FileReader<T, R>, W>; 

} // namespace cl

#endif
