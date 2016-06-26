#include "include/cl.h"

using namespace cl;

// Global state: guaranteed to be initialized before any tests are invoked
auto& global = FlagArg::create("--global");
