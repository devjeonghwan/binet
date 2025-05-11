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

bool _initialized = false;
BiNetLogLevel _logLevel = BINET_LOG_LEVEL_DEBUG;
BiNetAllocator _allocator = {
    .allocationFunction = allocation,
    .deallocationFunction = deallocation
};

BiNetStatus _lastErrorStatus = BINET_STATUS_SUCCESS;
const char* _lastErrorFile = NULL;
uint32_t _lastErrorLine = -1;
const char* _lastErrorFunction = NULL;

void _BiNetSetLastError(const BiNetStatus status, const char* file, const uint32_t line, const char* function)
{
    if (status == BINET_STATUS_SUCCESS)
        return;

    _lastErrorStatus = status;
    _lastErrorFile = file;
    _lastErrorLine = line;
    _lastErrorFunction = function;

#ifdef _DEBUG
    __debugbreak();
#endif
}

void* _BiNetAllocation(const size_t size)
{
    if (!_initialized)
    {
        BINET_SET_STATUS(BINET_STATUS_ERROR_NOT_INITIALIZED);
        return NULL;
    }

    return _allocator.allocationFunction(size);
}

void _BiNetDeallocation(void* pointer)
{
    if (!_initialized)
    {
        BINET_SET_STATUS(BINET_STATUS_ERROR_NOT_INITIALIZED);
    }

    _allocator.deallocationFunction(pointer);
}

BiNetStatus BiNetInitialize(const BiNetLogLevel logLevel, const BiNetAllocator* allocator)
{
    if (_initialized)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALREADY_INITIALIZED);

    _initialized = true;
    _logLevel = logLevel;

    if (allocator == NULL)
    {
        _allocator.allocationFunction = allocation;
        _allocator.deallocationFunction = deallocation;
    }
    else
    {
        _allocator.allocationFunction = allocator->allocationFunction;
        _allocator.deallocationFunction = allocator->deallocationFunction;
    }

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetFinalize()
{
    if (!_initialized)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_NOT_INITIALIZED);

    _initialized = false;
    _logLevel = BINET_LOG_LEVEL_INFO;
    _allocator.allocationFunction = allocation;
    _allocator.deallocationFunction = deallocation;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

void BiNetPrintLastError()
{
    if (_lastErrorStatus == BINET_STATUS_SUCCESS)
        return;

    printf("Last Error %s in %s:%d  %s\r\n", BiNetGetStatusString(_lastErrorStatus), _lastErrorFile, _lastErrorLine, _lastErrorFunction);
}

BiNetStatus BiNetGetLastErrorStatus()
{
    return _lastErrorStatus;
}

const char* BiNetGetLastErrorFile()
{
    return _lastErrorFile;
}

uint32_t BiNetGetLastErrorLine()
{
    return _lastErrorLine;
}

const char* BiNetGetLastErrorFunction()
{
    return _lastErrorFunction;
}