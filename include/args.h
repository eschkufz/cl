#ifndef CL_INCLUDE_ARGS_H
#define CL_INCLUDE_ARGS_H

#include <sstream>

#include "include/arg_table.h"
#include "include/arg_base.h"
#include "include/group.h"

namespace cl {

class Args {
  public:
    static void read(int argc, char** argv) {
      auto& table = ArgTable::get();

      table.anonymous_.clear();
      table.provided_.clear();
      table.unrecognized_.clear();
      table.missing_.clear();
      table.error_.clear();
      table.duplicate_.clear();

      for (int i = 0; i < argc; ++i) {
        if (argv[i][0] != '-') {
          table.anonymous_.insert(argv[i]);
          continue;
        }
        const auto a = find_arg(argv[i]);
        if (a == arg_end()) {
          table.unrecognized_.insert(argv[i]);
          continue;
        }
        int arity = (*a)->arity();
        if (i+arity >= argc) {
          table.missing_.insert(*a);
          continue;
        }
        if (table.provided_.find(*a) != table.provided_.end()) {
          table.duplicate_.insert(*a);
          continue;
        }
        table.provided_.insert(*a);

        std::stringstream ss;
        for (int j = 0; j < arity; ++j) {
          ss << argv[i+1+j] << (j+1 < arity ? " " : "");
        }

        if (!(*a)->read(ss)) {
          table.error_.insert(*a);
        }
        i += arity;
      }
    }

    typedef std::set<Group*>::iterator group_itr;
    static group_itr group_begin() { 
      auto& table = ArgTable::get(); 
      return table.groups_.begin(); 
    } 
    static group_itr group_end() { 
      auto& table = ArgTable::get(); 
      return table.groups_.end(); 
    } 
  
    typedef std::set<ArgBase*>::iterator arg_itr;
    static arg_itr arg_begin() { 
      auto& table = ArgTable::get(); 
      return table.args_.begin(); 
    } 
    static arg_itr arg_end() { 
      auto& table = ArgTable::get(); 
      return table.args_.end(); 
    } 

    typedef std::set<const char*>::iterator anonymous_itr;
    static anonymous_itr anonymous_begin() { 
      auto& table = ArgTable::get(); 
      return table.anonymous_.begin(); 
    } 
    static anonymous_itr anonymous_end() { 
      auto& table = ArgTable::get(); 
      return table.anonymous_.end(); 
    } 

    typedef std::set<ArgBase*>::iterator provided_itr;
    static provided_itr provided_begin() { 
      auto& table = ArgTable::get(); 
      return table.provided_.begin(); 
    } 
    static provided_itr provided_end() { 
      auto& table = ArgTable::get(); 
      return table.provided_.end(); 
    } 

    typedef std::set<const char*>::iterator unrecognized_itr;
    static unrecognized_itr unrecognized_begin() { 
      auto& table = ArgTable::get(); 
      return table.unrecognized_.begin(); 
    } 
    static unrecognized_itr unrecognized_end() { 
      auto& table = ArgTable::get(); 
      return table.unrecognized_.end(); 
    } 

    typedef std::set<ArgBase*>::iterator missing_itr;
    static missing_itr missing_begin() { 
      auto& table = ArgTable::get(); 
      return table.missing_.begin(); 
    } 
    static missing_itr missing_end() { 
      auto& table = ArgTable::get(); 
      return table.missing_.end(); 
    } 

    typedef std::set<ArgBase*>::iterator error_itr;
    static error_itr error_begin() { 
      auto& table = ArgTable::get(); 
      return table.error_.begin(); 
    } 
    static error_itr error_end() { 
      auto& table = ArgTable::get(); 
      return table.error_.end(); 
    } 

    typedef std::set<ArgBase*>::iterator duplicate_itr;
    static duplicate_itr duplicate_begin() { 
      auto& table = ArgTable::get(); 
      return table.duplicate_.begin(); 
    } 
    static duplicate_itr duplicate_end() { 
      auto& table = ArgTable::get(); 
      return table.duplicate_.end(); 
    } 

  private:
    static arg_itr find_arg(const std::string& name) {
      for (auto i = arg_begin(), ie = arg_end(); i != ie; ++i) {
        if ((*i)->matches(name)) {
          return i;
        }
      }
      return arg_end();
    }
};

} // namespace cl

#endif
