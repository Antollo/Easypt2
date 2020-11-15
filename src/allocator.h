#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstdlib>
#include <array>
#include "console.h"

template <int Size, int MaxLength = 160>
class allocatorBuffer
{
public:
    constexpr allocatorBuffer() : length(MaxLength), head(0), tail(0)
    {
        for (size_t i = 0; i < MaxLength; i++)
            data[i] = &initialMemory[i];
    }
    void *allocate()
    {
        if (length != 0)
        {
            void *ptr = data[tail++];
            if (tail == MaxLength)
                tail = 0;
            length--;
            return ptr;
        }
        return std::malloc(Size);
    }

    void deallocate(void *ptr)
    {
        if (length != MaxLength)
        {
            data[head++] = ptr;
            if (head == MaxLength)
                head = 0;
            length++;
            return;
        }
        if (ptr > &initialMemory[MaxLength - 1] || ptr < &initialMemory[0])
            std::free(ptr);
    }

private:
    int length, head, tail;
    std::array<std::array<char, Size>, MaxLength> initialMemory;
    std::array<void *, MaxLength> data;
};

template <class T>
class allocator
{
public:
    using value_type = T;
    allocator() = default;
    template <class U>
    constexpr allocator(const allocator<U> &) noexcept {}
    constexpr bool operator==(allocator const &) const noexcept { return true; }
    constexpr bool operator!=(allocator const &) const noexcept { return false; }

    T *allocate(std::size_t n)
    {
        if (n <= a)
            return static_cast<T *>(memoryA.allocate());

        if (n <= b)
            return static_cast<T *>(memoryB.allocate());

        if (n <= c)
            return static_cast<T *>(memoryC.allocate());

        if (n <= d)
            return static_cast<T *>(memoryD.allocate());

        return static_cast<T *>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T *ptr, std::size_t n) noexcept
    {
        if (n <= a)
            memoryA.deallocate(ptr);
        else if (n <= b)
            memoryB.deallocate(ptr);
        else if (n <= c)
            memoryC.deallocate(ptr);
        else if (n <= d)
            memoryD.deallocate(ptr);
        else
            std::free(ptr);
    }

private:
    constexpr static int a = 2;
    constexpr static int b = 4;
    constexpr static int c = 8;
    constexpr static int d = 16;
    static allocatorBuffer<a * sizeof(T)> memoryA;
    static allocatorBuffer<b * sizeof(T)> memoryB;
    static allocatorBuffer<c * sizeof(T)> memoryC;
    static allocatorBuffer<d * sizeof(T)> memoryD;
};

template <class T>
allocatorBuffer<allocator<T>::a * sizeof(T)> allocator<T>::memoryA;

template <class T>
allocatorBuffer<allocator<T>::b * sizeof(T)> allocator<T>::memoryB;

template <class T>
allocatorBuffer<allocator<T>::c * sizeof(T)> allocator<T>::memoryC;

template <class T>
allocatorBuffer<allocator<T>::d * sizeof(T)> allocator<T>::memoryD;

#endif /* !ALLOCATOR_H_ */
