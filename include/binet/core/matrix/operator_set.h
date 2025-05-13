#ifndef OPERATOR_SET_H
#define OPERATOR_SET_H

#include "binet/api.h"
#include "binet/core/matrix.h"

DLLEXPORT BiStatus BiSetMatrixBinaryValues(BiMatrix*     destination,
                                           const BiBool* values);

DLLEXPORT BiStatus BiSetMatrixInt16Values(BiMatrix*      destination,
                                          const BiInt16* values);

DLLEXPORT BiStatus BiSetMatrixUInt16Values(BiMatrix*       destination,
                                           const BiUInt16* values);

DLLEXPORT BiStatus BiSetMatrixFloat32Values(BiMatrix*        destination,
                                            const BiFloat32* values);

#endif //OPERATOR_SET_H
