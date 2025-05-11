#ifndef MATRIX_STRING_SERIALIZE_H
#define MATRIX_STRING_SERIALIZE_H

#include "binet/core/matrix.h"
#include "binet/core/matrix_internal.h"
#include "binet/api.h"

#include <stdlib.h>

BiNetStatus _BiNetSerializeMatrixRecursive(char** buffer, size_t* capacity, size_t* length,
                                           const _BiNetMatrix* matrix, size_t dimension, size_t offset,
                                           bool pretty);

#endif //MATRIX_STRING_SERIALIZE_H
