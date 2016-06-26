#ifndef CL_INCLUDE_ARG_BASE_H
#define CL_INCLUDE_ARG_BASE_H

#include <iostream>
#include <set>
#include <string>
#include "include/arg_table.h"
#include "include/group.h"

namespace cl {

class ArgBase {
  public:
    ArgBase(const std::string& name) : desc_(""), required_(false) {
      names_.insert(name);

      auto& table = ArgTable::get();
      if (!table.groups_.empty()) {
        table.current_->args_.insert(this);
      }
      table.args_.insert(this);
    }
    virtual ~ArgBase() = default;

    typedef std::set<std::string>::const_iterator alias_itr;
    const alias_itr alias_begin() const {
      return names_.begin();
    }
    const alias_itr alias_end() const {
      return names_.end();
    }
    bool matches(const std::string& alias) const {
      return names_.find(alias) != names_.end();
    }
    const std::string& description() const {
      return desc_;
    }
    bool required() const {
      return required_;
    }

    virtual bool read(std::istream& is) = 0;
    virtual void write(std::ostream& os) const = 0;
    virtual size_t arity() const = 0;

  protected:
    std::set<std::string> names_;
    std::string desc_;
    bool required_;
};

} // namespace cl

#endif
