#include "MemoryManagement.h"

MemoryManager *MemoryManager::active_manager = nullptr;

MemoryManager::MemoryManager(size_t start, size_t size) {
    active_manager = (MemoryManager*)this;
    if (size < sizeof(MemoryChunk)) {
        first = nullptr;
    } else {
        first = (MemoryChunk *) start;
        first->is_allocated = false;
        first->next = nullptr;
        first->prev = nullptr;
        first->size = size - sizeof(MemoryChunk);
    }
}

MemoryManager::~MemoryManager() {}

MemoryManager *MemoryManager::getActiveMemoryManager() {
    return active_manager;
}

void *MemoryManager::malloc(size_t size) {
    MemoryChunk *result = nullptr;
    for (MemoryChunk *chunk = first; chunk != nullptr; chunk = chunk->next) {
        if (chunk->size > size && !chunk->is_allocated) {
            result = chunk;
            break;
        }
    }

    if (result == nullptr) {
        return result;
    }

    if (result->size >= size + sizeof(MemoryChunk) + 1) {
        auto *tmp = (MemoryChunk *) ((size_t) result + sizeof(MemoryChunk) + size);
        tmp->is_allocated = false;
        tmp->size = result->size - size - sizeof(MemoryChunk);
        tmp->prev = result;
        tmp->next = result->next;
        if (tmp->next != nullptr) tmp->next->prev = tmp;

        result->size = size;
        result->next = tmp;
    }
    result->is_allocated = true;

    return (void *) (((size_t) result) + sizeof(MemoryChunk));
}

void MemoryManager::free(void *ptr) {
    auto *chunk = (MemoryChunk *) ((size_t) ptr - sizeof(MemoryChunk));
    chunk->is_allocated = false;
    if (chunk->prev != nullptr && !chunk->prev->is_allocated) {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if (chunk->next != nullptr) {
            chunk->next->prev = chunk->prev;
        }
        chunk = chunk->prev;
    }
    if (chunk->next != nullptr && !chunk->next->is_allocated) {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if (chunk->next != nullptr) {
            chunk->next->prev = chunk;
        }
    }
}

void *operator new(size_t size) {
    return MemoryManager::getActiveMemoryManager()->malloc(size);
}

void *operator new[](size_t size) {
    return MemoryManager::getActiveMemoryManager()->malloc(size);
}

void *operator new(size_t size, void *ptr) {
    return ptr;
}

void *operator new[](size_t size, void *ptr) {
    return ptr;
}

void operator delete(void *ptr) {
    MemoryManager::getActiveMemoryManager()->free(ptr);
}

void operator delete[](void *ptr) {
    MemoryManager::getActiveMemoryManager()->free(ptr);
}

void operator delete(void *ptr, size_t size) {
    MemoryManager::getActiveMemoryManager()->free(ptr);
}

void operator delete[](void *ptr, size_t size) {
    MemoryManager::getActiveMemoryManager()->free(ptr);
}
