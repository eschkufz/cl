#ifndef CL_INCLUDE_GROUP_H
#define CL_INCLUDE_GROUP_H

#include <algorithm>
#include "ext/patterns/include/singleton.h"
#include "include/arg.h"
#include "include/arg_table.h"

namespace cl {

class Group {
  public:
    static Group& create(const std::string& name) {
      return *(new Group(name));
    }

    Group(const Group& rhs) = delete;
    Group(const Group&& rhs) = delete;
    Group& operator=(const Group& rhs) = delete;
    Group&& operator=(const Group&& rhs) = delete;

    const std::string& name() const {
      return name_;
    }
    typedef std::vector<Arg*>::const_iterator arg_itr;
    arg_itr arg_begin() const {
      return patterns::Singleton<ArgTable>::get().args_by_group_[idx_].begin();
    }
    arg_itr arg_end() const {
      return patterns::Singleton<ArgTable>::get().args_by_group_[idx_].end();
    }
    arg_itr arg_find(const std::string& alias) const {
      return std::find_if(arg_begin(), arg_end(), [alias](Arg* a){return a->matches(alias);});
    }

  private:
    Group(const std::string& name) : name_(name) { 
      auto& table = patterns::Singleton<ArgTable>::get();
      table.groups_.push_back(this);
      idx_ = table.args_by_group_.size();
      table.args_by_group_.resize(idx_+1);
    }

    std::string name_;
    size_t idx_;
};

} // namespace cl

#endif
