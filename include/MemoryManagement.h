#ifndef SGG_OS_MEMORYMANAGEMENT_H
#define SGG_OS_MEMORYMANAGEMENT_H

#include "Types.h"

struct MemoryChunk {
    MemoryChunk *next;
    MemoryChunk *prev;
    bool is_allocated;
    size_t size;
};

class MemoryManager {
public:
    MemoryManager(size_t start, size_t size);

    ~MemoryManager();

    static MemoryManager *getActiveMemoryManager();

    void *malloc(size_t size);

    void free(void *ptr);

protected:
    static MemoryManager *active_manager;
    MemoryChunk *first;
};

void *operator new(size_t size);

void operator delete(void *ptr);

void *operator new[](size_t size);

void operator delete[](void *ptr);

void *operator new(size_t size, void *ptr);

void operator delete(void *ptr, size_t size);

void *operator new[](size_t size, void *ptr);

void operator delete[](void *ptr, size_t size);

#endif //SGG_OS_MEMORYMANAGEMENT_H
