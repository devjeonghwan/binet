#ifndef MATRIX_INTERNAL_H
#define MATRIX_INTERNAL_H

typedef struct _BiMatrix _BiMatrix;

struct _BiMatrix
{
    BiSize*    shape;
    BiStride*  strides;
    BiRank     rank;
    BiDataType type;

    _BiMatrix* reference;
    uint64_t   referenceCount;
    void*      data;
};

static bool _BiIsContinuous(const BiSize* shape, const BiStride* strides, const BiRank rank)
{
    BiStride expected = 1;

    for (BiRank i = rank; i-- > 0;)
    {
        if (shape[i] == 1)
            continue;

        if (strides[i] != expected)
            return false;

        expected *= shape[i];
    }

    return true;
}

static bool _BiGetBroadcastShape(BiSize* out, const BiSize* shape1, const BiSize* shape2, const BiRank rank)
{
    for (BiRank index = 0; index < rank; ++index)
    {
        const BiSize size1 = shape1[index];
        const BiSize size2 = shape2[index];

        if (size1 == size2 || size2 == 1)
            out[index] = size1;
        else if (size1 == 1)
            out[index] = size2;
        else
            return false;
    }

    return true;
}

static bool _BiCompareShape(const BiSize* shape1, const BiSize* shape2, const BiRank rank)
{
    for (BiRank index = 0; index < rank; ++index)
    {
        const BiSize size1 = shape1[index];
        const BiSize size2 = shape2[index];

        if (size1 != size2)
            return false;
    }

    return true;
}

#endif //MATRIX_INTERNAL_H
