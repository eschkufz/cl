#ifndef CL_INCLUDE_ARG_TABLE_H
#define CL_INCLUDE_ARG_TABLE_H

#include <vector>

namespace cl {

class Arg;
class Group;

struct ArgTable {
  std::vector<Group*> groups_;
  std::vector<Arg*> args_;
  std::vector<std::vector<Arg*>> args_by_group_;
  std::vector<const char*> unrec_;
};

} // namespace cl

#endif
