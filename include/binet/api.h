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

typedef void* (*BINET_ALLOCATION_FUNCTION)(size_t);
typedef void (*BINET_DEALLOCATION_FUNCTION)(void*);

typedef struct
{
    BINET_ALLOCATION_FUNCTION allocationFunction;
    BINET_DEALLOCATION_FUNCTION deallocationFunction;
} BiNetAllocator;

typedef enum
{
    BINET_STATUS_SUCCESS = 0,

    BINET_STATUS_ERROR_INTERNAL = 1,
    BINET_STATUS_ERROR_OVERFLOW = 2,

    BINET_STATUS_ERROR_INVALID_ARGUMENT = 3,
    BINET_STATUS_ERROR_INVALID_SHAPE = 4,

    BINET_STATUS_ERROR_ALREADY_INITIALIZED = 5,
    BINET_STATUS_ERROR_NOT_INITIALIZED = 6,

    BINET_STATUS_ERROR_ALLOCATION = 7,
    BINET_STATUS_ERROR_DEALLOCATION = 8,
} BiNetStatus;

static const char* BiNetGetStatusString(const BiNetStatus status)
{
    switch (status)
    {
    case BINET_STATUS_SUCCESS:
        return "BINET_STATUS_SUCCESS";

    case BINET_STATUS_ERROR_INTERNAL:
        return "BINET_STATUS_ERROR_INTERNAL";
    case BINET_STATUS_ERROR_OVERFLOW:
        return "BINET_STATUS_ERROR_OVERFLOW";

    case BINET_STATUS_ERROR_INVALID_ARGUMENT:
        return "BINET_STATUS_ERROR_INVALID_ARGUMENT";

    case BINET_STATUS_ERROR_ALREADY_INITIALIZED:
        return "BINET_STATUS_ERROR_ALREADY_INITIALIZED";
    case BINET_STATUS_ERROR_NOT_INITIALIZED:
        return "BINET_STATUS_ERROR_NOT_INITIALIZED";

    case BINET_STATUS_ERROR_ALLOCATION:
        return "BINET_STATUS_ERROR_ALLOCATION";
    case BINET_STATUS_ERROR_DEALLOCATION:
        return "BINET_STATUS_ERROR_DEALLOCATION";

    default:
        return "BINET_STATUS_UNKNOWN";
    }
}

typedef enum
{
    BINET_LOG_LEVEL_DEBUG = 0,
    BINET_LOG_LEVEL_INFO = 1,
    BINET_LOG_LEVEL_WARNING = 2,
    BINET_LOG_LEVEL_ERROR = 3,
    BINET_LOG_LEVEL_DISABLE = 4,
} BiNetLogLevel;

DLLEXPORT BiNetStatus BiNetInitialize(BiNetLogLevel logLevel, const BiNetAllocator* allocator);

DLLEXPORT BiNetStatus BiNetFinalize();

DLLEXPORT void BiNetPrintLastError();

DLLEXPORT BiNetStatus BiNetGetLastErrorStatus();

DLLEXPORT const char* BiNetGetLastErrorFile();

DLLEXPORT uint32_t BiNetGetLastErrorLine();

DLLEXPORT const char* BiNetGetLastErrorFunction();

#endif //API_H
