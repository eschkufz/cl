#ifndef CL_INCLUDE_ARGS_H
#define CL_INCLUDE_ARGS_H

#include <algorithm>
#include <sstream>
#include "ext/patterns/include/singleton.h"
#include "include/arg.h"
#include "include/arg_table.h"
#include "include/group.h"

namespace cl {

class Args {
  public:
    static void read(int argc, char** argv) {
      for (int i = 0; i < argc; ++i) {
        const auto a = find(argv[i]);
        if (a == arg_end()) {
          patterns::Singleton<ArgTable>::get().unrec_.insert(argv[i]);
          continue;
        }
        std::stringstream ss;
        if ((*a)->arity() == 1 && ++i < argc) {
          ss << argv[i];
        }
        (*a)->read(ss);
      }
    }

    typedef std::set<Group*>::iterator group_itr;
    static group_itr group_begin() { 
      return patterns::Singleton<ArgTable>::get().groups_.begin(); 
    } 
    static group_itr group_end() { 
      return patterns::Singleton<ArgTable>::get().groups_.end(); 
    } 
    typedef std::set<Arg*>::iterator arg_itr;
    static arg_itr arg_begin() { 
      return patterns::Singleton<ArgTable>::get().args_.begin(); 
    } 
    static arg_itr arg_end() { 
      return patterns::Singleton<ArgTable>::get().args_.end(); 
    } 
    typedef std::set<const char*>::iterator unrecognized_itr;
    static unrecognized_itr unrecognized_begin() { 
      return patterns::Singleton<ArgTable>::get().unrec_.begin(); 
    } 
    static unrecognized_itr unrecognized_end() { 
      return patterns::Singleton<ArgTable>::get().unrec_.end(); 
    } 

  private:
    static arg_itr find(const char* alias) {
      return std::find_if(arg_begin(), arg_end(), [alias](Arg* a) {
        return a->matches(alias);
      });
    }
};

} // namespace cl

#endif
