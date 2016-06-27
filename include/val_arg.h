#ifndef CL_INCLUDE_VAL_ARG_H
#define CL_INCLUDE_VAL_ARG_H

#include "include/arg.h"

namespace cl {

template <typename T>
struct ValReader {
  bool operator()(std::istream& is, T& t) const {
    is >> t;
    return !is.fail();
  }
};  
template <typename T>
struct ValWriter {
  void operator()(std::ostream& os, const T& t) const {
    os << t;
  }
};

template <typename T, typename R = ValReader<T>, typename W = ValWriter<T>, size_t Arity = 1>
class ValArg : public Arg {
  public:
    static ValArg& create(const std::string& name) {
      return *(new ValArg(name));
    }
    virtual ~ValArg() = default;

    ValArg& alias(const std::string& a) {
      names_.insert(a);
      return *this;
    }
    ValArg& description(const std::string& d) {
      desc_ = d;
      return *this;
    }
    ValArg& usage(const std::string& u) {
      usage_ = u;
      return *this;
    }
    ValArg& required() {
      req_ = true;
      return *this;
    }
    ValArg& initial(const T& val) {
      val_ = val;
      return *this;
    }
    const T& value() const {
      return val_;
    }
    operator const T&() const {
      return val_;
    }

    virtual void read(std::istream& is) {
      err_ = !R()(is, val_);
      dup_ = prov_;
      prov_ = true;
    }
    virtual void write(std::ostream& os) const {
      W()(os, val_);
    }
    virtual size_t arity() const {
      return Arity;
    }

  private:
    ValArg(const std::string& name) : Arg(name), val_() {}

    T val_;
};

} // namespace cl

#endif
