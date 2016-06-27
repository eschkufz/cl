#ifndef CL_INCLUDE_DIR_ARG_H
#define CL_INCLUDE_DIR_ARG_H

#include <dirent.h>
#include <sstream>
#include <sys/stat.h>
#include <type_traits>
#include "ext/meta/include/is_stl_associative.h"
#include "ext/meta/include/is_stl_sequence.h"
#include "include/file_arg.h"

namespace cl {

template <typename T, typename R>
class DirReader {
  public:
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
        } else {
          typename T::value_type v;
          if (!FileReader<typename T::value_type,R>()(ss, v)) {
            return false;
          }
          add(t, v);
        }
      }

      return true;
    }

  private:
    template <typename C = T>
    void add(typename std::enable_if<meta::is_stl_associative<C>::value, C>::type& c, const typename C::value_type& v) const {
      c.insert(v);
    }
    template <typename C = T>
    void add(typename std::enable_if<meta::is_stl_sequence<C>::value, C>::type& c, const typename C::value_type& v) const {
      c.push_back(v);
    }
};

template <typename T, typename R = ValReader<typename T::value_type>, typename W = ValWriter<T>>
using DirArg = ValArg<T, DirReader<T, R>, W>;

} // namespace cl

#endif
