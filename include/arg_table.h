#ifndef CL_INCLUDE_ARG_TABLE_H
#define CL_INCLUDE_ARG_TABLE_H

#include <set>

namespace cl {

class ArgBase;
class Group;

class ArgTable {
  friend class Args;
  friend class ArgBase;
  friend class Group;

  private:
    static ArgTable& get() {
      static ArgTable t;
      return t;
    }

    Group* current_;
    std::set<Group*> groups_;
    std::set<ArgBase*> args_;

    std::set<const char*> anonymous_;
    std::set<ArgBase*> provided_;

    std::set<const char*> unrecognized_;
    std::set<ArgBase*> missing_;
    std::set<ArgBase*> error_;
    std::set<ArgBase*> duplicate_;
};

} // namespace cl

#endif
