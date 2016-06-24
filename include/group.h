#ifndef CL_INCLUDE_GROUP_H
#define CL_INCLUDE_GROUP_H

#include <set>
#include <string>
#include "include/arg_table.h"

namespace cl {

class ArgBase;

class Group {
  friend class ArgBase;
  friend class Args;

  public:
    static Group& create(const std::string& name) {
      return *(new Group(name));
    }

    typedef std::set<ArgBase*>::const_iterator arg_itr;
    arg_itr arg_begin() const {
      return args_.begin();
    }
    arg_itr arg_end() const {
      return args_.end();
    }
    const std::string& name() const {
      return name_;
    }

  private:
    Group(const std::string& name) : name_(name) { 
      auto& table = ArgTable::get();
      table.current_ = this;
      table.groups_.insert(this);
    }

    std::string name_;
    std::set<ArgBase*> args_; 
};

} // namespace cl

#endif
