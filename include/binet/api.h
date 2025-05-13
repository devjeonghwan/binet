#ifndef API_H
#define API_H

#if defined(__x86_64__) || defined(__ppc64__) || defined(_WIN64)
#define ARCH_x86_64
#endif
#if defined(__aarch64__) || defined(__arm64__)
#define ARCH_ARM_64
#endif
#if defined(_MSC_VER)
#define COMPILER_MSVC
#endif
#if defined(__GNUC__)
#define COMPILER_GCC
#endif

#if defined(__cplusplus)
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#ifdef COMPILER_MSVC
#ifdef BUILDING_DLL
#define DLLEXPORT EXTERN_C __declspec(dllexport)
#else
#define DLLEXPORT EXTERN_C __declspec(dllimport)
#endif
#define DLLLOCAL
#endif

#ifdef COMPILER_GCC
#define DLLEXPORT EXTERN_C __attribute__ ((visibility("default")))
#define DLLLOCAL EXTERN_C __attribute__ ((visibility("hidden")))
#endif

#include <stdint.h>
#include <stdbool.h>

typedef void* (*BI_ALLOCATION_FUNCTION)(size_t);
typedef void (* BI_DEALLOCATION_FUNCTION)(void*);

typedef struct
{
    BI_ALLOCATION_FUNCTION   allocationFunction;
    BI_DEALLOCATION_FUNCTION deallocationFunction;
} BiAllocator;

typedef enum
{
    BI_STATUS_SUCCESS = 0,

    BI_STATUS_ERROR_INTERNAL        = 100,
    BI_STATUS_ERROR_OVERFLOW        = 101,
    BI_STATUS_ERROR_NOT_IMPLEMENTED = 102,

    BI_STATUS_ERROR_INVALID_ARGUMENT  = 200,
    BI_STATUS_ERROR_INVALID_SHAPE     = 201,
    BI_STATUS_ERROR_INVALID_RANK      = 202,
    BI_STATUS_ERROR_INVALID_DATA_TYPE = 203,

    BI_STATUS_ERROR_ALREADY_INITIALIZED = 700,
    BI_STATUS_ERROR_NOT_INITIALIZED     = 701,

    BI_STATUS_ERROR_ALLOCATION   = 800,
    BI_STATUS_ERROR_DEALLOCATION = 801,

    BI_STATUS_ERROR_UNSUPPORTED_DATA_TYPE = 900,
} BiStatus;

static const char* BiGetStatusString(const BiStatus status)
{
    switch (status)
    {
    case BI_STATUS_SUCCESS:
        return "BI_STATUS_SUCCESS";

    case BI_STATUS_ERROR_INTERNAL:
        return "BI_STATUS_ERROR_INTERNAL";
    case BI_STATUS_ERROR_OVERFLOW:
        return "BI_STATUS_ERROR_OVERFLOW";

    case BI_STATUS_ERROR_INVALID_ARGUMENT:
        return "BI_STATUS_ERROR_INVALID_ARGUMENT";

    case BI_STATUS_ERROR_ALREADY_INITIALIZED:
        return "BI_STATUS_ERROR_ALREADY_INITIALIZED";
    case BI_STATUS_ERROR_NOT_INITIALIZED:
        return "BI_STATUS_ERROR_NOT_INITIALIZED";

    case BI_STATUS_ERROR_ALLOCATION:
        return "BI_STATUS_ERROR_ALLOCATION";
    case BI_STATUS_ERROR_DEALLOCATION:
        return "BI_STATUS_ERROR_DEALLOCATION";

    default:
        return "BI_STATUS_UNKNOWN";
    }
}

typedef enum
{
    BI_LOG_LEVEL_DEBUG   = 0,
    BI_LOG_LEVEL_INFO    = 1,
    BI_LOG_LEVEL_WARNING = 2,
    BI_LOG_LEVEL_ERROR   = 3,
    BI_LOG_LEVEL_DISABLE = 4,
} BiLogLevel;

DLLEXPORT BiStatus BiInitialize(BiLogLevel logLevel, const BiAllocator* allocator);

DLLEXPORT BiStatus BiFinalize();

DLLEXPORT void BiPrintLastError();

DLLEXPORT BiStatus BiGetLastErrorStatus();

DLLEXPORT const char* BiGetLastErrorFile();

DLLEXPORT uint32_t BiGetLastErrorLine();

DLLEXPORT const char* BiGetLastErrorFunction();

#endif //API_H
