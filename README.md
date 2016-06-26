Old Code:

struct ArgComp {
  bool operator()(const ArgBase* a1, const ArgBase* a2) const {
    return *(a1->alias_begin()) <= *(a2->alias_begin());
  }
};

struct ArgWriter {
  void operator()(std::ostream& os, const ArgBase* a) const {
    for (auto i = a->alias_begin(), ie = a->alias_end(); i != ie; ++i) {
      os << "  " << *i;
    }
    os << std::endl;
    const auto d = a->desc() == "" ? "<none>" : a->desc();
    os << "    Desc:  " << d << std::endl;
    const auto u = a->usage() == "" ? "<none>" : a->usage();
    os << "    Usage: " << u << std::endl;
    if (a->assigned()) {
      os << "    Value: ";
      a->write(os);
      os << std::endl;
    }
  }
};

struct GroupComp {
  bool operator()(const Group* g1, const Group* g2) const {
    return g1->name() < g2->name();
  }
};

struct GroupWriter {
  void operator()(std::ostream &os, const Group* g) const {
    const auto n = g->name() == "" ? "<unnamed>" : g->name();
    os << n << ":" << std::endl;
    for (auto i = g->arg_begin(); i != g->arg_end(); ++i) {
      ArgWriter()(os, *i);
    }
  }
};

