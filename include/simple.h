#ifndef CL_INCLUDE_SIMPLE_H
#define CL_INCLUDE_SIMPLE_H

#include "include/arg.h"
#include "include/args.h"
#include "include/flag_arg.h"
#include "include/group.h"

namespace cl {

class Simple {
  public:
    static void read(int argc, char** argv) {
      Group::create("Help and command line options");
      auto& help = FlagArg::create("--help")
        .alias("-h")
        .description("Print this message and quit");

      Args::read(argc, argv);

      if (help) {
        std::vector<Group*> gs(Args::group_begin(), Args::group_end());
        std::sort(gs.begin(), gs.end(), LtGroup());

        for (auto i = gs.begin(), ie = gs.end(); i != ie; ++i) {
          std::cout << std::endl << (*i)->name() << ":" << std::endl;

          std::vector<Arg*> as((*i)->arg_begin(), (*i)->arg_end());
          std::sort(as.begin(), as.end(), LtArg());

          for (auto j = as.begin(), je = as.end(); j != je; ++j) {
            for (auto k = (*j)->alias_begin(), ke = (*j)->alias_end(); k != ke; ++k) {
              std::cout << "  " << *k;
            }
            std::cout << std::endl;
            std::cout << "    Desc:     " << ((*j)->description() == "" ? "<none>" : (*j)->description()) << std::endl;
            std::cout << "    Required: " << ((*j)->required() ? "yes" : "no") << std::endl;
            const auto s = status(*j);
            std::cout << "    " << (s.first ? "Value:    " : "Error:    ") << s.second << std::endl;
          }
        }
        std::cout << std::endl;
        exit(0);
      }

      for (auto i = Args::arg_begin(), ie = Args::arg_end(); i != ie; ++i) {
        const auto s = status(*i);
        if (!s.first) {
          std::cout << "Error (" << *((*i)->alias_begin()) << "): " << s.second << std::endl;
          exit(1);
        }
      }
    }

  private:
    struct LtArg {
      bool operator()(const Arg* a1, const Arg* a2) const {
        return *(a1->alias_begin()) <= *(a2->alias_begin());
      }
    };
    struct LtGroup {
      bool operator()(const Group* g1, const Group* g2) const {
        return g1->name() <= g2->name();
      }
    };

    static std::pair<bool, std::string> status(const Arg* a) {
      if (a->error()) {
        return std::make_pair(false, "Unable to parse argument!");
      } else if (a->duplicated()) {
        return std::make_pair(false, "Duplicate value provided!");
      } else if (a->required() && !a->provided()) {
        return std::make_pair(false, "Required value was not provided!");
      } else {
        std::stringstream ss;
        a->write(ss);
        return std::make_pair(true, ss.str());
      }
    }
};

} // namespace cl

#endif
