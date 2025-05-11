#ifndef MATRIX_INTERNAL_H
#define MATRIX_INTERNAL_H

typedef struct
{
    BiNetSize* shape;
    BiNetSize* strides;
    BiNetRank rank;
    BiNetDataType type;

    void* data;
} _BiNetMatrix;

#endif //MATRIX_INTERNAL_H
