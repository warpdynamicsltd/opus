#ifndef MEMORY_H
#define MEMORY_H

#include <set>

template <typename T>
class MassDealocator
{
    public:
        static void* operator new(size_t size);

        static std::set<T*> allocated;
        static void clear_allocated();
};

template <typename T>
std::set<T*> MassDealocator<T>::allocated;

template <typename T>
void* MassDealocator<T>::operator new(size_t size)
{
    void *ptr = ::operator new(size);
        if (!ptr) throw std::bad_alloc();
    //std::printf("Allocated %zu bytes at %p\n", size, ptr);
    MassDealocator<T>::allocated.insert(static_cast<T*>(ptr));
    return ptr;
}

template <typename T>
void MassDealocator<T>::clear_allocated()
{
    for (auto &item : allocated) {
        delete item;
    }
    allocated.clear();
}

#endif