#ifndef CL_INCLUDE_DIR_ARG_H
#define CL_INCLUDE_DIR_ARG_H

#include <dirent.h>
#include <sstream>
#include <sys/stat.h>
#include "include/file_arg.h"

namespace cl {

template <typename T, typename R>
struct DirReader {
  bool operator()(std::istream& is, T& t) const {
    std::string path = "";
    is >> path;

    DIR* dp = opendir(path.c_str());    
    if (dp == NULL) {
      return false;
    }
    while (dirent* de = readdir(dp)) {
      const std::string file = de->d_name;
      if (file == "." || file == "..") {
        continue;
      }
      std::stringstream ss(path + "/" + file);

      struct stat filestat;
      if (stat(ss.str().c_str(), &filestat)) {
        return false;
      } 
      if (S_ISDIR(filestat.st_mode)) {
        if (!DirReader<T,R>()(ss, t)) {
          return false;
        }
        continue;
      }
      typename T::value_type v;
      if (!FileReader<typename T::value_type,R>()(ss, v)) {
        return false;
      }
      t.push_back(v);
    }

    return true;
  }
};

template <typename T, typename R = ValReader<typename T::value_type>, typename W = ValWriter<T>>
using DirArg = ValArg<T, DirReader<T, R>, W>;

} // namespace cl

#endif
