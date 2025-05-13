#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <math.h>

#define BI_DATA_TYPE_BITS_MASK      0b00000111
#define BI_DATA_TYPE_CLASS_MASK     0b11111000
#define BI_DATA_TYPE_FLOATING_MASK  0b10000000
#define BI_DATA_TYPE_UNSIGNED_MASK  0b01000000

typedef enum
{
    BI_DATA_TYPE_BINARY  = 0b00000000,
    BI_DATA_TYPE_INT16   = 0b00001100,
    BI_DATA_TYPE_UINT16  = 0b01001100,
    BI_DATA_TYPE_FLOAT32 = 0b10000101,
} BiDataType;

typedef bool     BiBool;
typedef uint8_t  BiUInt8;
typedef int8_t   BiInt8;
typedef uint16_t BiUInt16;
typedef int16_t  BiInt16;
typedef float    BiFloat32;

static size_t BiGetDataTypeBitWidth(const BiDataType dataType)
{
    return 1 << (dataType & BI_DATA_TYPE_BITS_MASK);
}

static size_t BiGetDataTypeByteWidth(const BiDataType dataType)
{
    return (1 << (dataType & BI_DATA_TYPE_BITS_MASK)) / 8;
}

static size_t BiGetDataTypeSize(const BiDataType dataType, const size_t elements)
{
    return ceil((1 << (dataType & BI_DATA_TYPE_BITS_MASK)) / 8.0f * elements);
}

static bool BiIsUnsigned(const BiDataType dataType)
{
    return dataType & BI_DATA_TYPE_UNSIGNED_MASK;
}

static bool BiIsFloatingPoint(const BiDataType dataType)
{
    return dataType & BI_DATA_TYPE_FLOATING_MASK;
}
#endif //DATA_TYPE_H
