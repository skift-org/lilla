#include <ce-heap/libheap.h>
#include <karm-logger/logger.h>

import Karm.Core;

using namespace Karm;

// MARK: Kmm Implementation ---------------------------------------------------

static Lock _heapLock;
static Heap _heapImpl = {
    .ctx = nullptr,
    .alloc = [](void*, usize ) -> void* {
        notImplemented();
    },
    .free = [](void*, void* , usize ) {
        notImplemented();
    },
    .log = [](void*, enum HeapLogType , char const* , va_list) {
        notImplemented();
    },
    .root = nullptr,
    .best = nullptr,
};

// MARK: New/Delete Implementation ---------------------------------------------

void* operator new(usize size) {
    LockScope scope(_heapLock);
    return heap_calloc(&_heapImpl, size, 1);
}

void* operator new[](usize size) {
    LockScope scope(_heapLock);
    return heap_calloc(&_heapImpl, size, 1);
}

void operator delete(void* ptr) {
    LockScope scope(_heapLock);
    heap_free(&_heapImpl, ptr);
}

void operator delete[](void* ptr) {
    LockScope scope(_heapLock);
    heap_free(&_heapImpl, ptr);
}

void operator delete(void* ptr, usize) {
    LockScope scope(_heapLock);
    heap_free(&_heapImpl, ptr);
}

void operator delete[](void* ptr, usize) {
    LockScope scope(_heapLock);
    heap_free(&_heapImpl, ptr);
}
