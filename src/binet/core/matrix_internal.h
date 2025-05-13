#ifndef MATRIX_INTERNAL_H
#define MATRIX_INTERNAL_H

typedef struct
{
    BiSize*    shape;
    BiSize*    strides;
    BiRank     rank;
    BiDataType type;

    void* data;
} _BiMatrix;

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
