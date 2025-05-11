#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#define BINET_DATA_TYPE_BITS_MASK   0b00000111
#define BINET_DATA_TYPE_CLASS_MASK  0b11111000
#include <math.h>

typedef enum
{
    BINET_DATA_TYPE_BINARY = 0b00000000,
    BINET_DATA_TYPE_INTEGER16 = 0b00001100,
    BINET_DATA_TYPE_FLOAT32 = 0b00010101,
} BiNetDataType;

static size_t BiNetGetDataTypeBitWidth(const BiNetDataType dataType)
{
    return 1 << (dataType & BINET_DATA_TYPE_BITS_MASK);
}

static size_t BiNetGetDataTypeByteWidth(const BiNetDataType dataType)
{
    return (1 << (dataType & BINET_DATA_TYPE_BITS_MASK)) / 8;
}

static size_t BiNetGetDataTypeSize(const BiNetDataType dataType, const size_t elements)
{
    return ceil((1 << (dataType & BINET_DATA_TYPE_BITS_MASK)) / 8.0f * elements);
}
#endif //DATA_TYPE_H
