#include <>

void* operator new(size_t size) {
    return malloc(size); // Tu propia implementación de malloc sobre el allocator.cpp
}

void operator delete(void* p, size_t size) {
    free(p);
}