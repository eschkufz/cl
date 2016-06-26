#ifndef CL_INCLUDE_GROUP_H
#define CL_INCLUDE_GROUP_H

#include <set>
#include <string>
#include "ext/patterns/include/singleton.h"
#include "include/arg_table.h"

namespace cl {

class Arg;

class Group {
  friend class Arg;
  friend class Args;

  public:
    static Group& create(const std::string& name) {
      return *(new Group(name));
    }

    Group(const Group& rhs) = delete;
    Group(const Group&& rhs) = delete;
    Group& operator=(const Group& rhs) = delete;
    Group&& operator==(const Group&& rhs) = delete;

    typedef std::set<Arg*>::const_iterator arg_itr;
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
      auto& table = patterns::Singleton<ArgTable>::get();
      table.current_ = this;
      table.groups_.insert(this);
    }

    std::string name_;
    std::set<Arg*> args_; 
};

} // namespace cl

#endif
