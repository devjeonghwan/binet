#ifndef API_INTERNAL_H
#define API_INTERNAL_H

#include "binet/api.h"

#define BINET_RETURN_STATUS(status) do { _BiNetSetLastError(status, __FILE__, __LINE__, __func__); return status; } while(0)

#define BINET_SET_STATUS(status) do { _BiNetSetLastError(status, __FILE__, __LINE__, __func__); } while(0)

void _BiNetSetLastError(BiNetStatus status, const char* file, uint32_t line, const char* function);

void* _BiNetAllocation(size_t size);

void _BiNetDeallocation(void* pointer);

#endif //API_INTERNAL_H
