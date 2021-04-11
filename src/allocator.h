#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstdlib>
#include <array>
#include "console.h"

template <size_t Size, size_t MaxLength = 256>
class allocatorBuffer
{
public:
    constexpr allocatorBuffer() : length(0), head(0), tail(0) {}
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
        }
        else
            std::free(ptr);
    }

private:
    size_t length, head, tail;
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
        else if (n <= b)
            return static_cast<T *>(memoryB.allocate());
        else if (n <= c)
            return static_cast<T *>(memoryC.allocate());
        else if (n <= d)
            return static_cast<T *>(memoryD.allocate());
        else if (n <= e)
            return static_cast<T *>(memoryE.allocate());
        else if (n <= f)
            return static_cast<T *>(memoryF.allocate());

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
        else if (n <= e)
            memoryE.deallocate(ptr);
        else if (n <= f)
            memoryF.deallocate(ptr);
        else
            std::free(ptr);
    }

private:
    constexpr static size_t a = 2;
    constexpr static size_t b = 4;
    constexpr static size_t c = 8;
    constexpr static size_t d = 16;
    constexpr static size_t e = 32;
    constexpr static size_t f = 64;
    constexpr static size_t MaxLength = 512;
    static inline allocatorBuffer<a * sizeof(T), MaxLength / a> memoryA;
    static inline allocatorBuffer<b * sizeof(T), MaxLength / b> memoryB;
    static inline allocatorBuffer<c * sizeof(T), MaxLength / c> memoryC;
    static inline allocatorBuffer<d * sizeof(T), MaxLength / d> memoryD;
    static inline allocatorBuffer<e * sizeof(T), MaxLength / e> memoryE;
    static inline allocatorBuffer<f * sizeof(T), MaxLength / f> memoryF;
};

#endif /* !ALLOCATOR_H_ */
