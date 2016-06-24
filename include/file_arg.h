#ifndef CL_INCLUDE_FILE_ARG_H
#define CL_INCLUDE_FILE_ARG_H

#include "arg.h"
#include "file_reader.h"
#include "val_reader.h"
#include "val_writer.h"

namespace cl {

template <typename T, typename R = ValReader<T>, typename W = ValWriter<T>>
using FileArg = Arg<T, FileReader<T, R>, W>; 

} // namespace cl

#endif
