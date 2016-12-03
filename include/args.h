#ifndef CL_INCLUDE_ARGS_H
#define CL_INCLUDE_ARGS_H

#include <sstream>
#include "ext/patterns/include/singleton.h"
#include "include/group.h"

namespace cl {

struct Args {
  static void read(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
      const auto a = arg_find(argv[i]);
      if (a == arg_end()) {
        patterns::Singleton<ArgTable>::get().unrec_.push_back(argv[i]);
      } else {
        std::stringstream ss;
        ss << (((*a)->arity() == 1 && ++i < argc) ? argv[i] : "");
        (*a)->read(ss);
      }
    }
  }

  typedef std::vector<Group*>::iterator group_itr;
  static group_itr group_begin() { 
    return patterns::Singleton<ArgTable>::get().groups_.begin(); 
  } 
  static group_itr group_end() { 
    return patterns::Singleton<ArgTable>::get().groups_.end(); 
  } 
  static group_itr group_find(const std::string& name) {    
    return std::find_if(group_begin(), group_end(), [name](Group* g){return g->name() == name;});    
  }

  typedef std::vector<Arg*>::iterator arg_itr;
  static arg_itr arg_begin() { 
    return patterns::Singleton<ArgTable>::get().args_.begin(); 
  } 
  static arg_itr arg_end() { 
    return patterns::Singleton<ArgTable>::get().args_.end(); 
  } 
  static arg_itr arg_find(const std::string& alias) {   
    return std::find_if(arg_begin(), arg_end(), [alias](Arg* a){return a->matches(alias);});    
  }

  typedef std::vector<const char*>::iterator unrecognized_itr;
  static unrecognized_itr unrecognized_begin() { 
    return patterns::Singleton<ArgTable>::get().unrec_.begin(); 
  } 
  static unrecognized_itr unrecognized_end() { 
    return patterns::Singleton<ArgTable>::get().unrec_.end(); 
  } 
};

} // namespace cl

#endif
