#ifndef CL_INCLUDE_ARG_TABLE_H
#define CL_INCLUDE_ARG_TABLE_H

#include <set>

namespace cl {

class Arg;
class Group;

class ArgTable {
  friend class Arg;
  friend class Args;
  friend class Group;

  private:
    Group* current_;
    std::set<Group*> groups_;
    std::set<Arg*> args_;
    std::set<const char*> unrec_;
};

} // namespace cl

#endif
