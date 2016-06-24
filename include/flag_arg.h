#ifndef CL_INCLUDE_FLAG_ARG_H
#define CL_INCLUDE_FLAG_ARG_H

#include "arg.h"
#include "flag_reader.h"
#include "val_writer.h"

namespace cl {

typedef Arg<bool, FlagReader, ValWriter<bool>, 0> FlagArg;

} // namespace cl

#endif
