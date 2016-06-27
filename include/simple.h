#ifndef CL_INCLUDE_SIMPLE_H
#define CL_INCLUDE_SIMPLE_H

#include "include/args.h"
#include "include/flag_arg.h"

namespace cl {

class Simple {
  public:
    static void read(int argc, char** argv) {
      Group::create("Help and Command Line Options");
      auto& help = FlagArg::create("--help")
        .alias("-h")
        .description("Print command line information and quit");

      Args::read(argc, argv);
      if (help) {
        write_help(std::cout);
        exit(0);
      }
      for (auto i = Args::arg_begin(), ie = Args::arg_end(); i != ie; ++i) {
        if (error(*i)) {
          write_error(std::cout, *i);
          exit(1);
        }
      }
    }

  private:
    static bool error(const Arg* a) {
      return a->error() || a->duplicated() || (a->required() && !a->provided());
    }

    static void write_error(std::ostream& os, Arg* a) {
      os << "Error (" << *(a->alias_begin()) << "): "; 
      if (a->error()) {
        os << "Unable to parse argument!" << std::endl;
      } else if (a->duplicated()) {
        os << "Argument appears more than once!" << std::endl;
      } else {
        os << "Required value not provided!" << std::endl;
      } 
    }

    static void write_help(std::ostream& os) {
      std::vector<Group*> gs(Args::group_begin(), Args::group_end());
      std::sort(gs.begin(), gs.end(), [](Group* g1, Group* g2) {return g1->name() <= g2->name();});

      for (auto i = gs.begin(), ie = gs.end(); i != ie; ++i) {
        os << std::endl << (*i)->name() << ":" << std::endl;

        std::vector<Arg*> as((*i)->arg_begin(), (*i)->arg_end());
        std::sort(as.begin(), as.end(), [](Arg* a1, Arg* a2) {return *(a1->alias_begin()) <= *(a2->alias_begin());});

        for (auto j = as.begin(), je = as.end(); j != je; ++j) {
          os << " ";
          for (auto k = (*j)->alias_begin(), ke = (*j)->alias_end(); k != ke; ++k) {
            os << *k << " ";
          }
          os << ((*j)->usage() == "" ? "" : (*j)->usage()) << std::endl;
          os << "    Desc:     " << ((*j)->description() == "" ? "<none>" : (*j)->description()) << std::endl;
          os << "    Required: " << ((*j)->required() ? "yes" : "no") << std::endl;
          if (error(*j)) {
            os << "    Error:    ";
            write_error(os, *j);
          } else {
            os << "    Value:    ";
            (*j)->write(os);
          }
          os << std::endl;
        }
      }
      os << std::endl;
    }
};

} // namespace cl

#endif
