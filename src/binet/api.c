#include "binet/api.h"
#include "binet/api_internal.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void* allocation(const size_t size)
{
    return malloc(size);
}

void deallocation(void* pointer)
{
    free(pointer);
}

bool        _initialized = false;
BiLogLevel  _logLevel    = BI_LOG_LEVEL_DEBUG;
BiAllocator _allocator   = {
    .allocationFunction = allocation,
    .deallocationFunction = deallocation
};

BiStatus    _lastErrorStatus   = BI_STATUS_SUCCESS;
const char* _lastErrorFile     = NULL;
uint32_t    _lastErrorLine     = -1;
const char* _lastErrorFunction = NULL;

void _BiSetLastError(const BiStatus status, const char* file, const uint32_t line, const char* function)
{
    if (status == BI_STATUS_SUCCESS)
        return;

    _lastErrorStatus   = status;
    _lastErrorFile     = file;
    _lastErrorLine     = line;
    _lastErrorFunction = function;

#ifdef _DEBUG
    __debugbreak();
#endif
}

void* _BiAllocation(const size_t size)
{
    if (!_initialized)
    {
        BI_SET_STATUS(BI_STATUS_ERROR_NOT_INITIALIZED);
        return NULL;
    }

    return _allocator.allocationFunction(size);
}

void _BiDeallocation(void* pointer)
{
    if (!_initialized)
    {
        BI_SET_STATUS(BI_STATUS_ERROR_NOT_INITIALIZED);
    }

    _allocator.deallocationFunction(pointer);
}

BiStatus BiInitialize(const BiLogLevel logLevel, const BiAllocator* allocator)
{
    if (_initialized)
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALREADY_INITIALIZED);

    _initialized = true;
    _logLevel    = logLevel;

    if (allocator == NULL)
    {
        _allocator.allocationFunction   = allocation;
        _allocator.deallocationFunction = deallocation;
    }
    else
    {
        _allocator.allocationFunction   = allocator->allocationFunction;
        _allocator.deallocationFunction = allocator->deallocationFunction;
    }

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiFinalize()
{
    if (!_initialized)
        BI_RETURN_STATUS(BI_STATUS_ERROR_NOT_INITIALIZED);

    _initialized                    = false;
    _logLevel                       = BI_LOG_LEVEL_INFO;
    _allocator.allocationFunction   = allocation;
    _allocator.deallocationFunction = deallocation;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

void BiPrintLastError()
{
    if (_lastErrorStatus == BI_STATUS_SUCCESS)
        return;

    printf("Last Error %s in %s:%d  %s\r\n", BiGetStatusString(_lastErrorStatus), _lastErrorFile, _lastErrorLine, _lastErrorFunction);
}

BiStatus BiGetLastErrorStatus()
{
    return _lastErrorStatus;
}

const char* BiGetLastErrorFile()
{
    return _lastErrorFile;
}

uint32_t BiGetLastErrorLine()
{
    return _lastErrorLine;
}

const char* BiGetLastErrorFunction()
{
    return _lastErrorFunction;
}
