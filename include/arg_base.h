#ifndef CL_INCLUDE_ARG_BASE_H
#define CL_INCLUDE_ARG_BASE_H

#include <iosfwd>
#include <set>
#include <string>
#include "include/arg_table.h"
#include "include/group.h"

namespace cl {

class ArgBase {
  public:
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
    const std::string& usage() const {
      return usage_;
    }
    bool required() const {
      return required_;
    }

    virtual bool read(std::istream& is) = 0;
    virtual void write(std::ostream& os) const = 0;
    virtual size_t arity() const = 0;

  protected:
    ArgBase(const std::string& name) : desc_(""), usage_(""), required_(false) {
      names_.insert(convert(name));

      auto& table = ArgTable::get();
      if (!table.groups_.empty()) {
        table.current_->args_.insert(this);
      }
      table.args_.insert(this);
    }

    std::string convert(const std::string& s) const {
      if (s.length() > 0 && s[0] == '-') {
        return s;
      } else {
        return (s.length() < 2 ? "-" : "--") + s;
      }
    }

    std::set<std::string> names_;
    std::string desc_;
    std::string usage_;
    bool required_;
};

} // namespace cl

#endif
