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
      Group::create("Help and Command Line Options");
      auto& help = FlagArg::create("--help")
        .alias("-h")
        .description("Print command line information and quit");
      ValArg<std::string>::create("--args")
        .usage("<path>")
        .description("Expand the contents of this file into command line arguments");

      parse(argc, argv);
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
    static void parse(int argc, char** argv) {
      std::stringstream ss;
      for (int i = 0; i < argc; ++i) {
        ss << "\"" << argv[i] << "\"" << std::endl;
      }
      std::vector<std::string> args;
      expand(ss, args);

      std::vector<char*> cps;
      for (const auto& a : args) {
        cps.push_back((char*)a.c_str());
      }
      Args::read(cps.size(), &cps[0]);
    }

    static void expand(std::istream& is, std::vector<std::string>& args) {
      std::stringstream ss;
      std::string s = "";
      while (getline(is, s)) {
        ss << s.substr(0, line.find('#')) << " ";
      }
      while (getstr(ss, s)) {

      }
    }

    static std::istream& getstr(std::istream& is, std::string& s) {
      is >> s;    
      if (s.front() == '\"') {
        s = s.substr(1);
        if (s.back() == '\"') {
          s = s.substr(0, s.length()-1);
        } else {
          std::string suf = "";
          while (is >> suf && suf.back() != '"') {
            s = s + " " + suf;
          }
          if (is.fail()) {

          }
        }
      }
      return is;
    }

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
