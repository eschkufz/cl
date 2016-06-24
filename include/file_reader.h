#ifndef CL_INCLUDE_FILE_READER_H
#define CL_INCLUDE_FILE_READER_H

#include <iostream>
#include <fstream>
#include <string>

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

} // namespace cl

#endif
