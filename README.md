#cl
A c++ library for parsing command line arguments.

## Argument Types
This library defines four argument types.

### Flag Arguments
Flag arguments have type ```bool```.
``` c++
bool& b = cl::FlagArg::create("--b");
```
Passing a flag argument's name on the command line will set its value to ```true```.
```
$ ./a.out --b
```

### String Arguments
String arguments can have any type.
``` c++
int& i = cl::StrArg<int>::create("--i");
```
String arguments take a single value.
```
$ ./a.out --i 10 
```
String arguments can also have user-defined types.
``` c++
struct C {
  int x;
  int y;
};

namespace cl {
template <>
struct StrReader<C> {
  void operator()(std::istream& is, C& c) {
    is >> c.x >> c.y;
  }
};
template <>
struct StrWriter<C> {
  void operator()(std::ostream& os, const C& c) {
    os << c.x << " " << c.y;
  }
};
} // namespace cl

C& c = cl::StrArg<C>::create("--c");
```
```
$ ./a.out --c "10 20"
```

### File Arguments
File arguments are string arguments whose values appear in a file. Comments beginning with a ```#``` are ignored.
``` c++
auto& f = cl::FileArg<float>::create("--f");
```
```
$ ./a.out --f <path/to/file.dat>
```
```
# path/to/file.dat
# A file argument
2.0
```

### Directory Arguments
Directory arguments are string arguments whose values appear in a directory. Files are parsed lexicographically.
``` c++
auto& c = cl::DirArg<C>::create("--c");
```
```
$ ./a.out --c <path/to/files/>
```
```
# path/to/files/x.dat
# The x part of a C
10
```
```
# path/to/files/subdir/y.dat
# The y part of a C
20
```

## Declaring Arguments
Declare arguments from anywhere in your code.
``` c++
#include "cl.h"

auto& x = cl::FlagArg::create("--x");
void foo() {
  auto& y = cl::FlagArg::create("--y");
}
```
Use groups to organize arguments.
``` c++
#include "cl.h"

cl::Group::create("Group 1");
auto& z1 = cl::FlagArg::create("--z1");
auto& z2 = cl::FlagArg::create("--z2");

cl::Group::create("Group 2");
// ...
```
Specify additional information using the Arg API:
``` c++
auto& i = cl::StrArg<int>::create("--i")
  .alias("--i2")              // Alternate name for this argument
  .description("arg named i") // Brief description
  .usage("<int>")             // Expected usage
  .required()                 // This argument must be provided
  .initial(10);               // The default value for this argument
```

### Using Arguments
Read arguments with a single function call.
``` c++
#include "cl.h"

int main(int argc, char** argv) {
  cl::Args::read(argc, argv);
  // ...
}
```
Use arguments exactly like you would objects of the same type.
``` c++
#include "cl.h"

auto& x = cl::FlagArg::create("--x");
auto& y = cl::StrArg<int>::create("--y");

void foo() {
  if (x.value()) {
    int z = y.value() + 10;
    // ...
  }
  // ...
}
```
Iterate over arguments after you've read the command line.
``` c++
#include "cl.h"

int main(int argc, char** argv) {
  cl::Args::read(argc, argv);
  
  // All arguments
  for (auto i = cl::Args::arg_begin(), ie = cl::Args::arg_end(); i != ie; ++i) {
    // ...
  }
  auto i = cl::Args::arg_find("--x");
  // Argument sorted by groups
  for (auto i = cl::Args::group_begin(), ie = cl::Args::group_end(); i != ie; ++i) {
    for (auto j = i->arg_begin(), je = i->arg_end(); j != je; ++j) {
      // ...
    }
  }
  auto j = cl::Args::group_find("Group 1");
  // Command line values which weren't associated with arguments
  for (auto i = cl::Args::unrecognized_begin(), ie = cl::Args::unrecognized_end(); i != ie; ++i) {
    // ...
  }
  // ...
}
```
Examine arguments using the Arg API 
``` c++
#include "cl.h"

int main(int argc, char** argv) {
  cl::Args::read(argc, argv);
  
  auto g = cl::Args::group_begin();
  g->name() ;        // Group name
  
  auto a  = cl::Args::arg_begin();
  a->alias_begin();  // Iterators over argument names
  a->alias_end();   
  a->matches("..."); // Is this one of this argument's names?
  a->description();  // Argument description
  a->usage();        // Argument usage
  a->required();     // Is this argument required?
  a->provided();     // Did this argument appear on the command line?
  a->duplicated();   // Did this argument appear more than once?
  a->error();        // Did an error occur while parsing this argument?
```
  
## Building command line tools
Use the Arg API to build your own command line tools or try the example provided in this library.
``` c++
#include "cl.h"

int main(int argc, char** argv) {
  cl::Simple::read(argc, argv);
  // ...
}
```
The example code provides support for help messages, error handling, and config file parsing.
```
$ ./a.out --help
```
```
$ ./a.out --int foo
Error (--int): Unable to parse argument!
```
```
$ ./a.out --config settings.cfg
```
