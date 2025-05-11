#ifndef MATRIX_H
#define MATRIX_H

#include "binet/api.h"
#include "binet/core/data_type.h"
#include "binet/core/string.h"

typedef struct BiNetMatrix BiNetMatrix;

typedef uint16_t BiNetRank;
typedef uint16_t BiNetSize;

DLLEXPORT BiNetStatus BiNetCreateMatrix(BiNetMatrix** matrix,
                                        const BiNetSize* shape,
                                        BiNetRank rank,
                                        BiNetDataType type);

DLLEXPORT BiNetStatus BiNetDestroyMatrix(BiNetMatrix** matrix);

DLLEXPORT BiNetStatus BiNetGetMatrixString(BiNetString** out,
                                           const BiNetMatrix* matrix,
                                           bool pretty);

DLLEXPORT BiNetStatus BiNetGetMatrixDataType(BiNetDataType* out,
                                             const BiNetMatrix* matrix);

DLLEXPORT BiNetStatus BiNetComputeBitwiseXor(BiNetMatrix** destination,
                                             const BiNetMatrix* operand1,
                                             const BiNetMatrix* operand2);

#endif //MATRIX_H
