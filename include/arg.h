#ifndef CL_INCLUDE_ARG_H
#define CL_INCLUDE_ARG_H

#include "include/arg_base.h"

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
class Arg : public ArgBase {
  public:
    static Arg& create(const std::string& name) {
      return *(new Arg(name));
    }
    virtual ~Arg() = default;

    Arg& alias(const std::string& a) {
      names_.insert(a);
      return *this;
    }
    Arg& description(const std::string& d) {
      desc_ = d;
      return *this;
    }
    Arg& required() {
      required_ = true;
      return *this;
    }
    Arg& initial(const T& val) {
      val_ = val;
      return *this;
    }

    T& value() {
      return val_;
    }

    virtual bool read(std::istream& is) {
      return R()(is, val_);
    }
    virtual void write(std::ostream& os) const {
      W()(os, val_);
    }
    virtual size_t arity() const {
      return Arity;
    }

  private:
    Arg(const std::string& name) : ArgBase(name), val_() {}

    T val_;
};

} // namespace cl

#endif
